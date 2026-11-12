#include <std_include.hpp>
#include <iw8/iw8.hpp>
#include <iw8/common/log.h>
#include "stock_sun.h"

#include <Windows.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <map>
#include <set>
#include <stdexcept>
#include <unordered_map>

namespace iw8::stocksun
{
	namespace
	{
		namespace fs = std::filesystem;
		using Vec3 = std::array<float, 3>;
		constexpr double kPi = 3.14159265358979323846;

		std::wstring Quote(const std::wstring& argument)
		{
			if (!argument.empty() && argument.find_first_of(L" \t\"") == std::wstring::npos)
			{
				return argument;
			}
			std::wstring quoted = L"\"";
			std::size_t slashes = 0;
			for (const auto c : argument)
			{
				if (c == L'\\')
				{
					++slashes;
					continue;
				}
				quoted.append(c == L'"' ? slashes * 2 + 1 : slashes, L'\\');
				slashes = 0;
				quoted.push_back(c);
			}
			quoted.append(slashes * 2, L'\\');
			return quoted + L"\"";
		}

		std::wstring Widen(const std::string& text)
		{
			return fs::path(text).wstring();
		}

		// One ACTS run: the LOD0 geometry sidecars of `names` (xmodelsurfs) into `directory`. Returns name -> GLB file.
		std::map<std::string, fs::path> ExportGeometry(const GeometrySource& source, const std::vector<std::string>& names,
			const fs::path& directory)
		{
			std::error_code error;
			const auto replay = source.game / "game_dx12_ship_replay.exe";
			const auto oodle = source.game / "oo2core_7_win64.dll";
			if (!fs::is_regular_file(source.acts, error))
			{
				throw std::runtime_error("sun.direction needs ACTS for the static models' streamed geometry: \"acts\" in the sun "
					"object or IW8_ACTS (" + source.acts.string() + " was not found)");
			}
			if (!fs::is_regular_file(replay, error))
			{
				throw std::runtime_error("sun.direction: " + replay.string() + " was not found");
			}
			std::vector<std::wstring> arguments{source.acts.wstring(), L"--noUpdater", L"fastfile", L"-r", L"mw19replay", L"-g",
				fs::absolute(replay).wstring()};
			if (fs::is_regular_file(oodle, error))
			{
				arguments.push_back(L"--oodle");
				arguments.push_back(fs::absolute(oodle).wstring());
			}
			// The surface payloads are in the game's XPak files (the editor's model cache passes the same set).
			for (const auto& entry : fs::directory_iterator(source.game / "zone", error))
			{
				if (entry.path().extension() == ".xpak" && entry.is_regular_file(error) && entry.file_size(error) >= 64 * 1024)
				{
					arguments.push_back(L"--xpak");
					arguments.push_back(fs::absolute(entry.path()).wstring());
				}
			}
			std::string joined;
			for (const auto& name : names)
			{
				joined += (joined.empty() ? "" : ",") + name;
			}
			arguments.insert(arguments.end(), {L"--geometry", L"-a", L"xmodelsurfs", L"-n", Widen(joined), L"-l", L"-o",
				fs::absolute(directory).wstring(), fs::absolute(source.zoneFile).wstring()});
			std::wstring commandLine;
			for (const auto& argument : arguments)
			{
				commandLine += (commandLine.empty() ? L"" : L" ") + Quote(argument);
			}
			if (commandLine.size() >= 32000)
			{
				throw std::runtime_error("sun.direction: the ACTS command line is too long");
			}

			fs::create_directories(directory);
			const auto logPath = directory / "acts.log";
			SECURITY_ATTRIBUTES security{sizeof(security), nullptr, TRUE};
			HANDLE log = CreateFileW(logPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, &security, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL, nullptr);
			if (log == INVALID_HANDLE_VALUE)
			{
				throw std::runtime_error("sun.direction: cannot create " + logPath.string());
			}
			STARTUPINFOW startup{};
			startup.cb = sizeof(startup);
			startup.dwFlags = STARTF_USESTDHANDLES;
			startup.hStdOutput = log;
			startup.hStdError = log;
			startup.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
			PROCESS_INFORMATION process{};
			const BOOL started = CreateProcessW(source.acts.c_str(), commandLine.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW,
				nullptr, nullptr, &startup, &process);
			CloseHandle(log);
			if (!started)
			{
				throw std::runtime_error("sun.direction: cannot start ACTS " + source.acts.string());
			}
			const DWORD wait = WaitForSingleObject(process.hProcess, 15u * 60u * 1000u);
			DWORD exitCode = 1;
			if (wait == WAIT_TIMEOUT)
			{
				TerminateProcess(process.hProcess, 1);
				WaitForSingleObject(process.hProcess, 5000);
			}
			else if (wait == WAIT_OBJECT_0)
			{
				GetExitCodeProcess(process.hProcess, &exitCode);
			}
			CloseHandle(process.hThread);
			CloseHandle(process.hProcess);
			const auto root = directory / "mw19replay" / source.zoneFile.stem();
			if (wait != WAIT_OBJECT_0 || exitCode != 0 || !fs::is_regular_file(root / "assets.jsonl", error))
			{
				throw std::runtime_error("sun.direction: the ACTS geometry export of " + source.zoneFile.string() +
					" failed (exit " + std::to_string(exitCode) + "); see " + logPath.string());
			}
			std::map<std::string, fs::path> files;
			std::ifstream journal(root / "assets.jsonl", std::ios::binary);
			for (std::string line; std::getline(journal, line);)
			{
				if (line.empty())
				{
					continue;
				}
				const auto record = nlohmann::json::parse(line, nullptr, false);
				if (record.is_discarded() || record.value("type", std::string{}) != "xmodelsurfs" ||
					record.value("status", std::string{}) != "ok" || record.value("geometry_status", std::string{}) != "ok" ||
					!record.contains("geometry_file"))
				{
					continue;
				}
				files[record.at("name").get<std::string>()] = root / fs::path(record.at("geometry_file").get<std::string>());
			}
			return files;
		}

		// The triangles of an ACTS geometry sidecar (glTF binary) by source surface (primitive extras.sourceSurface):
		// every primitive's POSITION by its indices, as the accessors store them (Z-up inches; the node's Y-up metre
		// transform is not applied).
		std::map<unsigned, std::vector<Vec3>> ReadGlbTriangles(const fs::path& path)
		{
			std::ifstream input(path, std::ios::binary);
			const std::string bytes((std::istreambuf_iterator<char>(input)), {});
			const auto u32 = [&](const std::size_t offset) {
				std::uint32_t value = 0;
				if (offset + 4 > bytes.size())
				{
					throw std::runtime_error("GLB " + path.string() + " is truncated");
				}
				std::memcpy(&value, bytes.data() + offset, 4);
				return value;
			};
			if (bytes.size() < 20 || u32(0) != 0x46546C67u || u32(16) != 0x4E4F534Au)
			{
				throw std::runtime_error(path.string() + " is not a glTF binary");
			}
			const std::size_t jsonLength = u32(12);
			const auto json = nlohmann::json::parse(bytes.substr(20, jsonLength));
			const std::size_t binHeader = 20 + ((jsonLength + 3) & ~std::size_t(3));
			if (u32(binHeader + 4) != 0x004E4942u)
			{
				throw std::runtime_error(path.string() + " has no BIN chunk");
			}
			const std::size_t binStart = binHeader + 8, binLength = u32(binHeader);
			if (binStart + binLength > bytes.size())
			{
				throw std::runtime_error(path.string() + " has a truncated BIN chunk");
			}
			const auto coordinates = json.value("extras", nlohmann::json::object()).value("sourceCoordinates", std::string{});
			if (coordinates != "right-handed Z-up, inches")
			{
				throw std::runtime_error(path.string() + " does not say its accessors are Z-up inches");
			}
			const auto view = [&](const nlohmann::json& accessor, std::size_t elementSize, std::size_t& stride) {
				const auto& bufferView = json.at("bufferViews").at(accessor.at("bufferView").get<std::size_t>());
				stride = bufferView.value("byteStride", elementSize);
				const std::size_t offset = bufferView.value("byteOffset", std::size_t(0)) + accessor.value("byteOffset", std::size_t(0));
				const std::size_t count = accessor.at("count").get<std::size_t>();
				const std::size_t length = bufferView.at("byteLength").get<std::size_t>();
				if (count && (accessor.value("byteOffset", std::size_t(0)) + (count - 1) * stride + elementSize > length ||
					bufferView.value("byteOffset", std::size_t(0)) + length > binLength))
				{
					throw std::runtime_error(path.string() + " has an accessor past its buffer");
				}
				return binStart + offset;
			};
			std::map<unsigned, std::vector<Vec3>> surfaces;
			for (const auto& mesh : json.at("meshes"))
			{
				for (const auto& primitive : mesh.at("primitives"))
				{
					if (primitive.value("mode", 4) != 4 || !primitive.contains("indices"))
					{
						continue;
					}
					const auto& positionAccessor = json.at("accessors").at(primitive.at("attributes").at("POSITION").get<std::size_t>());
					if (positionAccessor.at("componentType").get<int>() != 5126 || positionAccessor.at("type") != "VEC3")
					{
						throw std::runtime_error(path.string() + " has positions that are not float triples");
					}
					std::size_t positionStride = 0;
					const auto positionStart = view(positionAccessor, 12, positionStride);
					const auto vertexCount = positionAccessor.at("count").get<std::size_t>();
					const auto& indexAccessor = json.at("accessors").at(primitive.at("indices").get<std::size_t>());
					const int indexType = indexAccessor.at("componentType").get<int>();
					const std::size_t indexSize = indexType == 5121 ? 1 : indexType == 5123 ? 2 : indexType == 5125 ? 4 : 0;
					if (!indexSize)
					{
						throw std::runtime_error(path.string() + " has an unknown index type");
					}
					std::size_t indexStride = 0;
					const auto indexStart = view(indexAccessor, indexSize, indexStride);
					const auto indexCount = indexAccessor.at("count").get<std::size_t>() / 3 * 3;
					const auto extras = primitive.value("extras", nlohmann::json::object());
					if (!extras.contains("sourceSurface"))
					{
						throw std::runtime_error(path.string() + " has a primitive without its source surface");
					}
					auto& triangles = surfaces[extras.at("sourceSurface").get<unsigned>()];
					triangles.reserve(triangles.size() + indexCount);
					for (std::size_t k = 0; k < indexCount; ++k)
					{
						std::uint32_t index = 0;
						std::memcpy(&index, bytes.data() + indexStart + k * indexStride, indexSize);
						if (index >= vertexCount)
						{
							throw std::runtime_error(path.string() + " has an index past its vertices");
						}
						Vec3 position;
						std::memcpy(position.data(), bytes.data() + positionStart + index * positionStride, 12);
						triangles.push_back(position);
					}
				}
			}
			return surfaces;
		}

		bool IsSky(const zonetool::iw8::Material* material)
		{
			if (!material || !material->name)
			{
				return false;
			}
			std::string_view name = material->name;
			if (!name.empty() && name.front() == ',')
			{
				name.remove_prefix(1);
			}
			const auto slash = name.rfind('/');
			return name.substr(slash == std::string_view::npos ? 0 : slash + 1).starts_with("sky_");
		}
	}

	std::array<float, 3> TowardSun(const double pitch, const double yaw)
	{
		const double p = pitch * kPi / 180.0, y = yaw * kPi / 180.0;
		std::array<double, 3> v{std::cos(p) * std::cos(y), std::cos(p) * std::sin(y), -std::sin(p)};
		const double length = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		return {static_cast<float>(v[0] / length), static_cast<float>(v[1] / length), static_cast<float>(v[2] / length)};
	}

	void Angles(const std::array<float, 3>& v, double& pitch, double& yaw)
	{
		const double length = std::sqrt(double(v[0]) * v[0] + double(v[1]) * v[1] + double(v[2]) * v[2]);
		pitch = -std::asin(std::clamp(double(v[2]) / length, -1.0, 1.0)) * 180.0 / kPi;
		yaw = std::atan2(double(v[1]), double(v[0])) * 180.0 / kPi;
	}

	replaysunshadow::Scene BuildScene(const zonetool::iw8::GfxWorld& world, const GeometrySource& source, SceneReport& report)
	{
		using namespace zonetool::iw8;
		replaysunshadow::Scene scene;
		// Two-sided and opaque: the stock materials' alpha images are streamed, so cut-out leaves cast as solid cards.
		scene.materials.push_back({});

		// World surfaces: float positions (12-byte stride) at tris.posOffset of the surface's transient zone, 16-bit
		// indices from tris.baseIndex relative to that first vertex.
		Vec3 low{INFINITY, INFINITY, INFINITY}, high{-INFINITY, -INFINITY, -INFINITY};
		const auto opaqueEnd = std::min(world.surfaces.litOpaqueSurfsEnd, world.surfaces.count);
		for (std::uint32_t index = 0; world.surfaces.surfaces && index < opaqueEnd; ++index)
		{
			const auto& surface = world.surfaces.surfaces[index];
			if (!(surface.flags & GFX_SURF_CASTS_SUN_SHADOW) || IsSky(surface.material))
			{
				++report.worldSkipped;
				continue;
			}
			const auto* zone = surface.transientZone < world.draw.transientZoneCount && surface.transientZone < 1536
				? world.draw.transientZones[surface.transientZone] : nullptr;
			const auto& tris = surface.tris;
			if (!zone || !zone->drawVerts.posData || !zone->drawVerts.indices ||
				std::uint64_t(tris.posOffset) + 12ull * tris.vertexCount > zone->drawVerts.posDataSize ||
				std::uint64_t(tris.baseIndex) + 3ull * tris.triCount > zone->drawVerts.indexCount)
			{
				throw std::runtime_error("sun.direction: world surface " + std::to_string(index) +
					" has no resident vertices (a streamed transient zone); this map's sun shadow cannot be baked again");
			}
			replaysunshadow::Surface caster;
			caster.vertices.reserve(3ull * tris.triCount);
			for (std::uint32_t t = 0; t < tris.triCount; ++t)
			{
				const auto* triangle = zone->drawVerts.indices + tris.baseIndex + 3u * t;
				// A degenerate triangle casts nothing (editzone remove_world_surfaces leaves its triangles so).
				if (triangle[0] == triangle[1] && triangle[1] == triangle[2])
				{
					continue;
				}
				for (std::uint32_t k = 0; k < 3u; ++k)
				{
					const auto vertex = triangle[k];
					if (vertex >= tris.vertexCount)
					{
						throw std::runtime_error("sun.direction: world surface " + std::to_string(index) + " has an index past its vertices");
					}
					replaysunshadow::Vertex out;
					std::memcpy(out.position.data(), zone->drawVerts.posData + tris.posOffset + 12u * vertex, 12);
					for (int axis = 0; axis < 3; ++axis)
					{
						low[axis] = std::min(low[axis], out.position[axis]);
						high[axis] = std::max(high[axis], out.position[axis]);
					}
					caster.vertices.push_back(out);
				}
			}
			if (caster.vertices.empty())
			{
				continue;
			}
			++report.worldSurfaces;
			report.worldTriangles += caster.vertices.size() / 3;
			scene.surfaces.push_back(std::move(caster));
		}
		if (!report.worldSurfaces)
		{
			throw std::runtime_error("sun.direction: the map has no world surface that casts a sun shadow");
		}
		for (int axis = 0; axis < 3; ++axis)
		{
			report.worldCasterBounds.midpoint[axis] = (low[axis] + high[axis]) * 0.5f;
			report.worldCasterBounds.halfSize[axis] = (high[axis] - low[axis]) * 0.5f;
		}

		// Static models: the casting collections' instances, each with the LOD a player in the play region sees it at
		// (the first whose switch distance passes the instance's distance from the play region box): LOD0 in the play
		// space, the far vista props at their low LODs.
		const auto& models = world.smodels;
		const auto origin = [&](const std::uint32_t instance) {
			const auto& data = models.smodelInstanceData[instance];
			return std::array<double, 3>{data.translation[0] / 4096.0, data.translation[1] / 4096.0, data.translation[2] / 4096.0};
		};
		const auto lodOf = [&](const XModel& model, const std::array<double, 3>& at) {
			double squared = 0;
			for (int axis = 0; axis < 3; ++axis)
			{
				const double outside = std::max(0.0, std::abs(at[axis] - source.playRegion.midpoint[axis]) -
					source.playRegion.halfSize[axis]);
				squared += outside * outside;
			}
			const double distance = std::sqrt(squared);
			const unsigned count = std::clamp<unsigned>(model.numLods, 1, 6);
			unsigned lod = 0;
			while (lod + 1 < count && model.lodInfo[lod].dist > 0.0f && model.lodInfo[lod].dist < distance &&
				model.lodInfo[lod + 1].modelSurfsStaging && model.lodInfo[lod + 1].modelSurfsStaging->name)
			{
				++lod;
			}
			return lod;
		};
		using Key = std::pair<std::uint32_t, unsigned>; // static model, LOD
		std::map<Key, std::vector<std::uint32_t>> instancesByModel;
		std::set<std::string> namesWithoutSurfs;
		for (std::uint32_t c = 0; models.collections && c < models.collectionCount; ++c)
		{
			const auto& collection = models.collections[c];
			if (!(collection.flags & STATIC_MODEL_COLLECTION_FLAG_SUN_SHADOW_1))
			{
				report.nonCastingInstances += collection.instanceCount;
				continue;
			}
			if (collection.flags & STATIC_MODEL_COLLECTION_FLAG_SPLINED)
			{
				report.splinedInstances += collection.instanceCount;
				continue;
			}
			if (collection.smodelIndex >= models.modelCount || std::uint64_t(collection.firstInstance) + collection.instanceCount >
				models.instanceCount || !models.smodelInstanceData)
			{
				throw std::runtime_error("sun.direction: static model collection " + std::to_string(c) + " is out of range");
			}
			const auto* model = models.models[collection.smodelIndex].model;
			const auto* surfs = model ? model->lodInfo[0].modelSurfsStaging : nullptr;
			if (!surfs || !surfs->name || !*surfs->name)
			{
				report.missingInstances += collection.instanceCount;
				namesWithoutSurfs.insert(model && model->name ? model->name : "?");
				continue;
			}
			for (std::uint32_t k = 0; k < collection.instanceCount; ++k)
			{
				const auto instance = collection.firstInstance + k;
				const auto& data = models.smodelInstanceData[instance];
				if (!(data.scale > 0.0f) || !std::isfinite(data.scale))
				{
					++report.hiddenInstances;
					continue;
				}
				instancesByModel[{collection.smodelIndex, lodOf(*model, origin(instance))}].push_back(instance);
			}
		}
		const auto surfsName = [&](const Key& key) {
			std::string name = models.models[key.first].model->lodInfo[key.second].modelSurfsStaging->name;
			return name.front() == ',' ? name.substr(1) : name;
		};
		// The surfaces of each static model LOD that cast (GfxSModelSurfMatIndirection.emitType bit 1: a surface that is
		// only drawn, such as a tree canopy beside its shadow-caster card, has it clear) and whose material is not cut
		// out or blended (cullType 239; the alpha images are streamed, so a cut-out surface would cast a solid card).
		std::map<Key, std::set<unsigned>> castingSurfaces;
		for (std::uint32_t k = 0; models.surfaces && models.smodelSurfMatIndirection && k < models.surfaceCount; ++k)
		{
			const auto& surface = models.surfaces[k];
			const auto& material = models.smodelSurfMatIndirection[k];
			const Key key{surface.smodelIndex, surface.lodIndex};
			if (!instancesByModel.contains(key))
			{
				continue;
			}
			if ((material.emitType & 2) && material.cullType == 239)
			{
				castingSurfaces[key].insert(surface.surfIndex);
			}
			else
			{
				++report.modelSurfacesSkipped;
			}
		}
		std::set<std::string> nameSet;
		for (const auto& [key, list] : instancesByModel)
		{
			if (castingSurfaces.contains(key))
			{
				nameSet.insert(surfsName(key));
			}
		}
		const std::vector<std::string> names(nameSet.begin(), nameSet.end());
		std::map<std::string, fs::path> files;
		if (!names.empty())
		{
			files = ExportGeometry(source, names, source.scratch);
		}
		std::set<std::uint32_t> modelsUsed;
		std::map<std::string, std::map<unsigned, std::vector<Vec3>>> geometry;
		for (const auto& [key, list] : instancesByModel)
		{
			const auto casting = castingSurfaces.find(key);
			if (casting == castingSurfaces.end())
			{
				report.nonCastingInstances += list.size();
				continue;
			}
			const auto name = surfsName(key);
			const auto file = files.find(name);
			if (file == files.end())
			{
				report.missingInstances += list.size();
				report.missingModels.push_back(name);
				continue;
			}
			if (!geometry.contains(name))
			{
				geometry[name] = ReadGlbTriangles(file->second);
			}
			std::vector<Vec3> local;
			for (const auto& [surface, triangles] : geometry[name])
			{
				if (casting->second.contains(surface))
				{
					local.insert(local.end(), triangles.begin(), triangles.end());
				}
			}
			if (local.empty())
			{
				continue;
			}
			modelsUsed.insert(key.first);
			if (key.second)
			{
				report.lowerLodInstances += list.size();
			}
			for (const auto instanceIndex : list)
			{
				const auto& instance = models.smodelInstanceData[instanceIndex];
				// int32 translation in 1/4096 unit; orientation: four 16-bit quaternion components (x, y, z, w),
				// c = u / 32767.5 - 1 (IW8 Radiant core stock_smodels).
				std::array<double, 4> q{};
				for (int k = 0; k < 4; ++k)
				{
					const auto packed = static_cast<std::uint16_t>(instance.orientation.v[k / 2] >> (16 * (k % 2)));
					q[k] = packed / 32767.5 - 1.0;
				}
				const double norm = std::sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
				if (norm < 1e-9)
				{
					q = {0, 0, 0, 1};
				}
				else
				{
					for (auto& value : q)
					{
						value /= norm;
					}
				}
				const double x = q[0], y = q[1], z = q[2], w = q[3];
				const double axis[3][3] = {{1 - 2 * (y * y + z * z), 2 * (x * y + z * w), 2 * (x * z - y * w)},
					{2 * (x * y - z * w), 1 - 2 * (x * x + z * z), 2 * (y * z + x * w)},
					{2 * (x * z + y * w), 2 * (y * z - x * w), 1 - 2 * (x * x + y * y)}};
				const auto at = origin(instanceIndex);
				replaysunshadow::Surface caster;
				caster.vertices.resize(local.size());
				for (std::size_t v = 0; v < local.size(); ++v)
				{
					for (int a = 0; a < 3; ++a)
					{
						caster.vertices[v].position[a] = static_cast<float>(at[a] + instance.scale *
							(local[v][0] * axis[0][a] + local[v][1] * axis[1][a] + local[v][2] * axis[2][a]));
					}
				}
				++report.modelInstances;
				report.modelTriangles += local.size() / 3;
				scene.surfaces.push_back(std::move(caster));
			}
		}
		report.models = modelsUsed.size();
		for (const auto& name : namesWithoutSurfs)
		{
			report.missingModels.push_back(name);
		}
		return scene;
	}
}

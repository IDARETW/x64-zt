#include <std_include.hpp>
#include "export.hpp"

#pragma warning(push, 0)
#include <glaze/json.hpp>
#pragma warning(pop)

namespace zonetool::convert::iw7_iw8
{
	// A model's geometry file (xmodel/<name>.replay.json), written by glaze: the members are its keys and the values the
	// doubles that json values held (a large model's tree took seconds to build and write). Glaze reflects the member
	// names, which takes external linkage.
	struct model_geometry_surface
	{
		bool deformed{};
		std::string material;
		int reflection_probe{};
		std::vector<geometry_vertex> vertices;
		std::vector<std::uint32_t> indices;
	};

	struct model_geometry_lod
	{
		double distance{};
		std::vector<model_geometry_surface> surfaces;
	};

	struct model_geometry
	{
		int schema{};
		std::string name;
		unsigned bone_count{};
		std::uint32_t contents{};
		std::vector<model_geometry_lod> lods;
	};

	namespace
	{
		std::array<float, 3> unpack_unit_vec(const iw7::PackedUnitVec& vector)
		{
			std::array<float, 3> result{};
			for (auto i = 0; i < 3; i++)
			{
				result[i] = static_cast<float>((vector.packed >> (10 * i)) & 0x3FF) / 1023.0f * 2.0f - 1.0f;
			}

			return result;
		}

		template <typename Vertex>
		geometry_vertex export_vertex(const Vertex& vertex)
		{
			const auto normal = unpack_unit_vec(vertex.normal);
			const auto tangent = unpack_unit_vec(vertex.tangent);
			float uv[2]{};
			PackedVec::Vec2UnpackTexCoords(vertex.texCoord.packed, uv);
			// The top two bits of the tangent hold the binormal sign.
			const auto binormal_sign = (vertex.tangent.packed >> 30) ? 1.0f : -1.0f;
			geometry_vertex result;
			for (auto i = 0; i < 3; i++)
			{
				result.position[i] = vertex.xyz[i];
				result.normal[i] = normal[i];
				result.tangent[i] = tangent[i];
			}

			result.binormal_sign = binormal_sign;
			result.uv = {uv[0], uv[1]};
			result.color = {255, 255, 255, 255};
			return result;
		}

		// Replay packs the bounds of model triangle clusters as half floats (at most 65504).
		constexpr float largest_model_coordinate = 60000.0f;

		template <typename Vertices>
		float largest_coordinate(const Vertices& vertices)
		{
			auto largest = 0.0f;
			for (const auto& vertex : vertices)
			{
				for (const auto value : vertex.xyz)
				{
					largest = std::max(largest, std::abs(value));
				}
			}

			return largest;
		}

		template <typename Vertices, typename Faces>
		model_geometry_surface export_surface(const Vertices& source_vertices, const Faces& faces, const std::string& material)
		{
			model_geometry_surface surface;
			surface.material = material;
			surface.vertices.reserve(std::size(source_vertices));
			for (const auto& vertex : source_vertices)
			{
				surface.vertices.push_back(export_vertex(vertex));
			}

			surface.indices.reserve(std::size(faces) * 3);
			for (const auto& face : faces)
			{
				surface.indices.push_back(face.v1);
				surface.indices.push_back(face.v2);
				surface.indices.push_back(face.v3);
			}

			return surface;
		}

		// The largest scale that the world places a model at (1 for a model that it does not place).
		float largest_placement_scale(const source_map& source, const std::string& name)
		{
			static std::unordered_map<const source_map*, std::unordered_map<std::string, float>> cache;
			auto& scales = cache[&source];
			if (scales.empty())
			{
				scales.emplace("", 1.0f);
				if (const auto* world = source.world())
				{
					for (auto i = 0u; i < world->dpvs.smodelCount; i++)
					{
						const auto& instance = world->dpvs.smodelDrawInsts[i];
						if (!instance.model)
						{
							continue;
						}

						auto& scale = scales[file_name(asset_name(iw7::get_asset_name(iw7::ASSET_TYPE_XMODEL, instance.model)))];
						scale = std::max(scale, instance.placement.scale);
					}
				}
			}

			const auto found = scales.find(name);
			return found != scales.end() ? std::max(found->second, 1.0f) : 1.0f;
		}

		// A model's export, planned in model order: its materials (each registers with the extraction), LOD distances
		// and surfaces. A model that does not convert has none (it warns why).
		struct model_plan
		{
			struct lod
			{
				float distance;
				std::vector<std::string> materials;
			};

			std::vector<lod> lods;
		};

		std::optional<model_plan> plan_model(const source_map& source, const std::string& name, const iw7::XModel* model,
			extraction& output)
		{
			if (!model || !model->numLods)
			{
				return std::nullopt;
			}

			// A model that the world places beyond the range of model bounds spans the map: the sky draws it (Breakneck's
			// black hole, 4932 units placed at scale 60, cut through the level with its default-white disc).
			const auto placement_scale = largest_placement_scale(source, name);

			model_plan plan;
			auto previous_distance = 0.0f;
			for (auto lod = 0; lod < model->numLods && lod < 6; lod++)
			{
				const auto& info = model->lodInfo[lod];
				const auto surfaces = source.resolve(iw7::ASSET_TYPE_XMODEL_SURFS, info.modelSurfs);
				const auto streamed = info.modelSurfs ? source.streamed_surfaces(asset_name(info.modelSurfs->name)) : nullptr;
				if ((!surfaces || !surfaces->surfs) && !streamed)
				{
					ZONETOOL_WARNING("iw7: model \"%s\" lod %d: surfaces \"%s\" are neither in the loaded zones nor streamed", name.data(),
						lod, info.modelSurfs ? info.modelSurfs->name : "none");
					return std::nullopt;
				}

				auto& planned = plan.lods.emplace_back();
				for (std::size_t i = 0; i < info.numsurfs; i++)
				{
					planned.materials.push_back(output.add_material(source, model->materialHandles[info.surfIndex + i]));
					auto largest = 0.0f;
					if (surfaces && surfaces->surfs && i < surfaces->numsurfs)
					{
						const auto& surface = surfaces->surfs[i];
						const std::span motion(surface.verts0.packedMotionVerts0, surface.vertCount);
						const std::span packed(surface.verts0.packedVerts0, surface.vertCount);
						largest = (surface.flags & 8) != 0 ? largest_coordinate(motion) : largest_coordinate(packed);
					}
					else if (streamed && i < streamed->size())
					{
						const auto& surface = (*streamed)[i];
						largest = surface.motion ? largest_coordinate(surface.motion_vertices) : largest_coordinate(surface.vertices);
					}

					if (largest * placement_scale > largest_model_coordinate)
					{
						ZONETOOL_WARNING("iw7: model \"%s\" reaches %.0f units (placed at scale %g), beyond the range of Replay model "
							"bounds: the sky draws it", name.data(), largest * placement_scale, placement_scale);
						output.distant_models.insert(name);
						return std::nullopt;
					}
				}

				// IW7 LOD distances need not grow (650, 419.8, 2500); Replay takes a strictly growing sequence.
				previous_distance = lod + 1 == model->numLods ? std::max(1000000.0f, previous_distance + 1.0f)
					: std::max(previous_distance + 1.0f, std::isfinite(info.dist) ? info.dist : 0.0f);
				planned.distance = previous_distance;
			}

			return plan;
		}

		// Writes a planned model: its OAT xmodel table and its geometry (any thread).
		void write_model(const source_map& source, const std::string& name, const iw7::XModel* model, const model_plan& plan,
			const extraction& output)
		{
			auto table_lods = json::array();
			model_geometry geometry;
			geometry.schema = 1;
			geometry.name = name;
			// The importer warns about a missing skeleton export only for models with bones to keep.
			geometry.bone_count = model->numBones;
			geometry.contents = static_cast<std::uint32_t>(model->contents);
			for (std::size_t lod = 0; lod < plan.lods.size(); lod++)
			{
				const auto& info = model->lodInfo[lod];
				const auto surfaces = source.resolve(iw7::ASSET_TYPE_XMODEL_SURFS, info.modelSurfs);
				const auto streamed = info.modelSurfs ? source.streamed_surfaces(asset_name(info.modelSurfs->name)) : nullptr;
				std::vector<model_geometry_surface> lod_surfaces;
				for (std::size_t i = 0; i < info.numsurfs; i++)
				{
					const auto& material_name = plan.lods[lod].materials[i];
					if (surfaces && surfaces->surfs && i < surfaces->numsurfs)
					{
						const auto& surface = surfaces->surfs[i];
						const std::span faces(surface.triIndices, surface.triCount);
						const std::span motion(surface.verts0.packedMotionVerts0, surface.vertCount);
						const std::span packed(surface.verts0.packedVerts0, surface.vertCount);
						lod_surfaces.push_back((surface.flags & 8) != 0 ? export_surface(motion, faces, material_name)
							: export_surface(packed, faces, material_name));
					}
					else if (streamed && i < streamed->size())
					{
						const auto& surface = (*streamed)[i];
						lod_surfaces.push_back(surface.motion
							? export_surface(std::span(surface.motion_vertices), std::span(surface.faces), material_name)
							: export_surface(std::span(surface.vertices), std::span(surface.faces), material_name));
					}
				}

				const auto distance = plan.lods[lod].distance;
				table_lods.push_back({{"distance", distance}, {"file", std::format("model_export/{}_lod{}.obj", name, lod)}});
				geometry.lods.push_back({distance, std::move(lod_surfaces)});
			}

			write_json(output.extracted / "xmodel" / (name + ".json"),
			{
				{"$schema", "http://openassettools.dev/schema/xmodel.v1.json"},
				{"_game", "iw3"},
				{"_type", "xmodel"},
				{"_version", 2},
				// IW7 points collLod at a model's coarsest LOD (Frontier: 130 of its 163 three-LOD models name LOD 2), and
				// a collision mesh of that LOD let bullets pass parts of models that LOD0 shows (owner test 2026-10-18):
				// models collide with the LOD that players see up close.
				{"collLod", 0},
				{"flags", 0},
				{"lods", std::move(table_lods)},
				{"rootBoneName", ""},
				{"type", "rigid"},
			});

			std::string text;
			if (glz::write_json(geometry, text))
			{
				throw std::runtime_error(std::format("cannot write the geometry of {}", name));
			}

			write_text(output.extracted / "xmodel" / (name + ".replay.json"), text);
		}
	}


	bool model_has_surfaces(const source_map& source, const iw7::XModel* model)
	{
		if (!model || !model->numLods)
		{
			return false;
		}

		for (auto lod = 0; lod < model->numLods && lod < 6; lod++)
		{
			const auto& info = model->lodInfo[lod];
			const auto surfaces = source.resolve(iw7::ASSET_TYPE_XMODEL_SURFS, info.modelSurfs);
			if ((!surfaces || !surfaces->surfs) && !(info.modelSurfs && source.streamed_surfaces(asset_name(info.modelSurfs->name))))
			{
				return false;
			}
		}

		return true;
	}

	std::vector<mesh_surface> model_mesh(const source_map& source, const iw7::XModel& model)
	{
		std::vector<mesh_surface> result;
		if (!model.numLods)
		{
			return result;
		}

		const auto add = [&](const auto& vertices, const auto& faces, const iw7::Material* material)
		{
			auto& surface = result.emplace_back();
			surface.material = material;
			for (const auto& vertex : vertices)
			{
				mesh_vertex converted{};
				std::copy_n(vertex.xyz, 3, converted.position.begin());
				converted.normal = unpack_unit_vec(vertex.normal);
				PackedVec::Vec2UnpackTexCoords(vertex.texCoord.packed, converted.uv.data());
				surface.vertices.push_back(converted);
			}

			for (const auto& face : faces)
			{
				surface.triangles.push_back({face.v1, face.v2, face.v3});
			}
		};

		const auto& info = model.lodInfo[0];
		const auto surfaces = source.resolve(iw7::ASSET_TYPE_XMODEL_SURFS, info.modelSurfs);
		const auto streamed = info.modelSurfs ? source.streamed_surfaces(asset_name(info.modelSurfs->name)) : nullptr;
		for (std::size_t i = 0; i < info.numsurfs; i++)
		{
			const auto material = source.resolve(iw7::ASSET_TYPE_MATERIAL, model.materialHandles[info.surfIndex + i]);
			if (surfaces && surfaces->surfs && i < surfaces->numsurfs)
			{
				const auto& surface = surfaces->surfs[i];
				const std::span faces(surface.triIndices, surface.triCount);
				if ((surface.flags & 8) != 0)
				{
					add(std::span(surface.verts0.packedMotionVerts0, surface.vertCount), faces, material);
				}
				else
				{
					add(std::span(surface.verts0.packedVerts0, surface.vertCount), faces, material);
				}
			}
			else if (streamed && i < streamed->size())
			{
				const auto& surface = (*streamed)[i];
				if (surface.motion)
				{
					add(surface.motion_vertices, surface.faces, material);
				}
				else
				{
					add(surface.vertices, surface.faces, material);
				}
			}
		}

		return result;
	}

	void export_models(const source_map& source, extraction& output)
	{
		std::size_t written{};
		std::map<std::uint32_t, std::size_t> contents;
		// Planned in model order (materials register with the extraction), written on up to 8 cores (the json of a large
		// model takes gigabytes).
		std::vector<std::tuple<const std::string*, const iw7::XModel*, model_plan>> planned;
		std::set<std::string> converted;
		for (const auto& [name, model] : output.models)
		{
			if (auto plan = plan_model(source, name, model, output))
			{
				planned.emplace_back(&name, model, std::move(*plan));
				converted.insert(name);
			}
		}

		{
			std::atomic<std::size_t> next{0};
			std::exception_ptr failure;
			std::mutex failure_mutex;
			const auto run = [&]
			{
				for (std::size_t index; (index = next.fetch_add(1)) < planned.size();)
				{
					try
					{
						const auto& [name, model, plan] = planned[index];
						write_model(source, *name, model, plan, output);
					}
					catch (...)
					{
						std::lock_guard lock(failure_mutex);
						if (!failure)
						{
							failure = std::current_exception();
						}
					}
				}
			};

			std::vector<std::thread> workers;
			for (auto worker = 1u; worker < std::clamp(std::thread::hardware_concurrency(), 1u, 8u); worker++)
			{
				workers.emplace_back(run);
			}

			run();
			for (auto& worker : workers)
			{
				worker.join();
			}

			if (failure)
			{
				std::rethrow_exception(failure);
			}
		}

		for (const auto& [name, model] : output.models)
		{
			if (converted.contains(name))
			{
				output.exported_models.insert(name);
				written++;
				if (const auto* resolved = source.resolve(iw7::ASSET_TYPE_XMODEL, const_cast<iw7::XModel*>(model)))
				{
					contents[static_cast<std::uint32_t>(resolved->contents)]++;
				}
			}
			else if (!output.distant_models.contains(name))
			{
				ZONETOOL_WARNING("iw7: model \"%s\" is not in the loaded zones", name.data());
			}
		}

		std::string kinds;
		for (const auto& [value, count] : contents)
		{
			kinds += std::format("{}{:#x} x{}", kinds.empty() ? "" : ", ", value, count);
		}

		ZONETOOL_INFO("iw7: exported %zu of %zu static models (contents %s)", written, output.models.size(), kinds.data());
	}

	namespace
	{
		std::string script_name(const iw7::scr_string_t id)
		{
			const auto string = SL_ConvertToString(static_cast<std::uint32_t>(id));
			return string ? string : "";
		}

		template <typename T>
		json values(const T* data, const std::size_t count)
		{
			auto result = json::array();
			for (std::size_t i = 0; data && i < count; i++)
			{
				result.push_back(data[i]);
			}

			return result;
		}

		// An IW7 XAnimParts as the pipeline's IW3 XAnim dump: IW7 keeps IW3's streams (part types boneCount[10], key
		// counts, frame indices and keys), which Replay decodes as IW3 does; so is its root motion (the delta part: "trans",
		// "quat" of 2 components as IW3 names it, "quat4" of 4).
		bool write_xanim(const iw7::XAnimParts& anim, const std::filesystem::path& path)
		{
			const auto indices = [&](const iw7::XAnimDynamicIndices& data, const std::size_t count)
			{
				return anim.numframes < 256 ? values(data._1, count) : values(data._2, count);
			};

			const auto quat = [&]<std::size_t N>(const auto& track, const short (*frames)[N], const short* frame0)
			{
				json result = {{"size", track.size}};
				if (!track.size)
				{
					result["frame0"] = values(frame0, N);
					return result;
				}

				const std::size_t count = track.size + 1u;
				result["indices"] = indices(track.u.frames.indices, count);
				result["frames"] = values(&frames[0][0], count * N);
				return result;
			};

			json delta = nullptr;
			if (const auto* part = anim.deltaPart)
			{
				delta = json::object();
				if (const auto* trans = part->trans)
				{
					json track = {{"size", trans->size}, {"small", trans->smallTrans != 0}};
					if (!trans->size)
					{
						track["frame0"] = values(trans->u.frame0, 3);
					}
					else
					{
						const std::size_t count = trans->size + 1u;
						const auto& frames = trans->u.frames;
						track["mins"] = values(frames.mins, 3);
						track["extent"] = values(frames.size, 3);
						track["indices"] = indices(frames.indices, count);
						track["frames"] = trans->smallTrans ? values(&frames.frames._1[0][0], count * 3) : values(&frames.frames._2[0][0], count * 3);
					}

					delta["trans"] = std::move(track);
				}

				if (const auto* track = part->quat2)
				{
					delta["quat"] = quat.operator()<2>(*track, track->u.frames.frames, track->u.frame0);
				}

				if (const auto* track = part->quat)
				{
					delta["quat4"] = quat.operator()<4>(*track, track->u.frames.frames, track->u.frame0);
				}
			}

			auto names = json::array();
			for (auto i = 0u; anim.names && i < anim.boneCount[9]; i++)
			{
				names.push_back(script_name(anim.names[i]));
			}

			auto notify = json::array();
			for (auto i = 0u; anim.notify && i < anim.notifyCount; i++)
			{
				notify.push_back({{"name", script_name(anim.notify[i].name)}, {"time", anim.notify[i].time}});
			}

			auto counts = json::array();
			for (const auto count : anim.boneCount)
			{
				counts.push_back(count);
			}

			const auto bytes = [](const char* data, const std::size_t count)
			{
				return values(reinterpret_cast<const std::uint8_t*>(data), count);
			};

			write_json(path,
			{
				{"schema", 1},
				{"name", asset_name(anim.name)},
				{"numframes", anim.numframes},
				{"framerate", anim.framerate},
				{"frequency", anim.frequency},
				{"loop", (anim.flags & 1) != 0},
				{"bone_count", std::move(counts)},
				{"names", std::move(names)},
				{"notify", std::move(notify)},
				{"data_byte", bytes(anim.dataByte, anim.dataByteCount)},
				{"data_short", values(anim.dataShort, anim.dataShortCount)},
				{"data_int", values(anim.dataInt, anim.dataIntCount)},
				{"random_data_byte", bytes(anim.randomDataByte, anim.randomDataByteCount)},
				{"random_data_short", values(anim.randomDataShort, anim.randomDataShortCount)},
				{"random_data_int", values(anim.randomDataInt, anim.randomDataIntCount)},
				{"indices", anim.numframes < 256 ? values(anim.indices._1, anim.indexCount) : values(anim.indices._2, anim.indexCount)},
				{"delta_part", std::move(delta)},
			});
			return true;
		}

		// The skeleton of a model as the OpenAssetTools XMODEL_EXPORT (version 6) of each LOD that the pipeline reads for entity
		// models: the bones at their bind pose (DObjAnimMat base poses, model space), and each vertex on the bone of its rigid
		// vertex list, with the triangles of every surface in the order of export_model. Blended vertices take the bones
		// they blend (IW7 blend data: per vertex its first bone, then a bone and a weight / 65536 for each other bone, then
		// a 0; the vertices grouped by their number of bones).
		bool write_skeleton(const source_map& source, const std::string& name, const iw7::XModel& model, const std::filesystem::path& root)
		{
			const auto fail = [&](const char* reason)
			{
				ZONETOOL_WARNING("iw7: the skeleton of model \"%s\" does not export (%s); it keeps one bone", name.data(), reason);
				return false;
			};

			// A model of one bone (tag_origin) has no skeleton to write.
			if (model.numBones < 2)
			{
				return false;
			}

			// Replay counts bones in a byte, as IW7 does. The parent list holds the bones after the roots: a model of root
			// bones only has none.
			if (!model.boneNames || !model.baseMat || (model.numBones > model.numRootBones && !model.parentList))
			{
				return fail(std::format("{} bones, {} roots, names {}, base poses {}, parents {}", model.numBones, model.numRootBones,
					model.boneNames != nullptr, model.baseMat != nullptr, model.parentList != nullptr).data());
			}

			std::string bones = std::format("NUMBONES {}\n", model.numBones);
			for (auto i = 0u; i < model.numBones; i++)
			{
				const int parent = i < model.numRootBones ? -1 : static_cast<int>(i) - model.parentList[i - model.numRootBones];
				if (parent >= static_cast<int>(i))
				{
					return fail("parent order");
				}

				bones += std::format("BONE {} {} \"{}\"\n", i, parent, script_name(model.boneNames[i]));
			}

			for (auto i = 0u; i < model.numBones; i++)
			{
				const auto& pose = model.baseMat[i];
				const double length = std::sqrt(pose.quat[0] * pose.quat[0] + pose.quat[1] * pose.quat[1] + pose.quat[2] * pose.quat[2] +
					pose.quat[3] * pose.quat[3]);
				if (!(length > 0.0))
				{
					return fail("bind pose");
				}

				const auto x = pose.quat[0] / length, y = pose.quat[1] / length, z = pose.quat[2] / length, w = pose.quat[3] / length;
				bones += std::format("BONE {}\nOFFSET {:.6f}, {:.6f}, {:.6f}\nSCALE 1.000000, 1.000000, 1.000000\n", i, pose.trans[0],
					pose.trans[1], pose.trans[2]);
				bones += std::format("X {:.6f}, {:.6f}, {:.6f}\n", 1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y));
				bones += std::format("Y {:.6f}, {:.6f}, {:.6f}\n", 2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x));
				bones += std::format("Z {:.6f}, {:.6f}, {:.6f}\n", 2 * (x * z + w * y), 2 * (y * z - w * x), 1 - 2 * (x * x + y * y));
			}

			auto lods = json::array();
			for (auto lod = 0; lod < model.numLods && lod < 6; lod++)
			{
				const auto& info = model.lodInfo[lod];
				// The surfaces of the zones, else those of the stream files, in the order of export_model.
				const auto surfaces = source.resolve(iw7::ASSET_TYPE_XMODEL_SURFS, info.modelSurfs);
				const auto streamed = info.modelSurfs ? source.streamed_surfaces(asset_name(info.modelSurfs->name)) : nullptr;
				const auto resident = surfaces && surfaces->surfs;
				if (resident ? surfaces->numsurfs < info.numsurfs : !streamed || streamed->size() < info.numsurfs)
				{
					return fail("surfaces in no loaded zone or stream file");
				}

				std::string vertices;
				std::string faces;
				std::size_t vertex_count{};
				std::size_t face_count{};
				for (std::size_t s = 0; s < info.numsurfs; s++)
				{
					std::vector<std::pair<std::uint16_t, std::uint16_t>> lists;
					std::array<std::uint16_t, 8> blend_counts{};
					std::span<const std::uint16_t> blend_data;
					if (resident)
					{
						const auto& surface = surfaces->surfs[s];
						for (auto l = 0u; surface.rigidVertLists && l < surface.rigidVertListCount; l++)
						{
							lists.emplace_back(surface.rigidVertLists[l].boneOffsetIndex, surface.rigidVertLists[l].vertCount);
						}

						std::ranges::copy(surface.blendVertCounts, blend_counts.begin());
						if (surface.blendVerts)
						{
							blend_data = std::span(surface.blendVerts, surface.blendVertSize / 2);
						}
					}
					else
					{
						lists = (*streamed)[s].rigid_lists;
						blend_counts = (*streamed)[s].blend_counts;
						blend_data = (*streamed)[s].blend_data;
					}

					const auto blended = std::ranges::any_of(blend_counts, [](const auto count) { return count != 0; });
					if (blended == !lists.empty())
					{
						return fail(blended ? "rigid and blended vertices in one surface" : "no rigid vertex lists");
					}

					// The bones of each vertex. The rigid lists cover the vertices in order; IW7 gives each list its bone index (IW3 its
					// offset in 64-byte bone matrices).
					std::vector<std::vector<std::pair<unsigned, float>>> vertex_bones;
					auto divisor = 1u;
					for (const auto& [bone, count] : lists)
					{
						divisor = bone >= model.numBones ? 64u : divisor;
					}

					for (const auto& [bone, count] : lists)
					{
						if (bone % divisor || bone / divisor >= model.numBones)
						{
							return fail("bone of a rigid list");
						}

						vertex_bones.insert(vertex_bones.end(), count, {{bone / divisor, 1.0f}});
					}

					std::size_t at{};
					for (auto bones_of = 1u; bones_of <= blend_counts.size(); bones_of++)
					{
						for (auto v = 0u; v < blend_counts[bones_of - 1]; v++)
						{
							if (at + 2 * bones_of > blend_data.size())
							{
								return fail("blend data");
							}

							auto& influences = vertex_bones.emplace_back();
							influences.emplace_back(blend_data[at], 1.0f);
							for (auto b = 1u; b < bones_of; b++)
							{
								const auto weight = blend_data[at + 2 * b] / 65536.0f;
								influences.emplace_back(blend_data[at + 2 * b - 1], weight);
								influences.front().second -= weight;
							}

							influences.front().second = std::max(influences.front().second, 0.0f);
							at += 2 * bones_of;
							if (std::ranges::any_of(influences, [&](const auto& influence) { return influence.first >= model.numBones; }))
							{
								return fail("bone of a blended vertex");
							}
						}
					}

					const auto add = [&](const auto& source_vertices, const auto& source_faces)
					{
						if (vertex_bones.size() != source_vertices.size())
						{
							return fail("vertex count of the rigid lists");
						}

						const auto first = vertex_count;
						for (std::size_t v = 0; v < source_vertices.size(); v++)
						{
							const auto& xyz = source_vertices[v].xyz;
							vertices += std::format("VERT {}\nOFFSET {:.6f}, {:.6f}, {:.6f}\nBONES {}\n", vertex_count++, xyz[0], xyz[1], xyz[2],
								vertex_bones[v].size());
							for (const auto& [bone, weight] : vertex_bones[v])
							{
								vertices += std::format("BONE {} {:.6f}\n", bone, weight);
							}
						}

						for (const auto& face : source_faces)
						{
							faces += std::format("TRI {} 0 0 0\n", s);
							for (const auto corner : {face.v1, face.v2, face.v3})
							{
								faces += std::format("VERT {}\nNORMAL 0 0 1\nCOLOR 1 1 1 1\nUV 1 0 0\n", first + corner);
							}

							face_count++;
						}

						return true;
					};

					auto added = false;
					if (resident)
					{
						const auto& surface = surfaces->surfs[s];
						const std::span faces_span(surface.triIndices, surface.triCount);
						added = (surface.flags & 8) != 0 ? add(std::span(surface.verts0.packedMotionVerts0, surface.vertCount), faces_span)
							: add(std::span(surface.verts0.packedVerts0, surface.vertCount), faces_span);
					}
					else
					{
						const auto& stream = (*streamed)[s];
						added = stream.motion ? add(stream.motion_vertices, stream.faces) : add(stream.vertices, stream.faces);
					}

					if (!added)
					{
						return fail("surface vertices");
					}
				}

				const auto file = std::format("xmodel_export/{}_lod{}.XMODEL_EXPORT", name, lod);
				std::filesystem::create_directories((root / file).parent_path());
				std::ofstream(root / file, std::ios::binary | std::ios::trunc) << std::format("// IW7 skeleton of {} (lod {})\nMODEL\nVERSION 6\n\n",
					name, lod) << bones << std::format("\nNUMVERTS {}\n", vertex_count) << vertices << std::format("\nNUMFACES {}\n", face_count)
					<< faces;
				lods.push_back({{"file", file}});
			}

			write_json(root / "xmodel" / (name + ".json"), {{"lods", std::move(lods)}});
			return true;
		}
	}

	namespace
	{
		// The translation (in the space of its parent) of a bone at the first and the last key of an animation that leaves
		// it unrotated. Replay's part walk (sub_1414D3460) reads the rotation parts, then the translation parts with the
		// bone of each in dataByte (key count, frame indices, min and size in dataInt, keys of 3 bytes or 3 shorts:
		// min + key * size), then the constant ones and the still ones. A bone that the animation names in no part keeps
		// its bind pose.
		struct bone_translation
		{
			std::array<float, 3> first{};
			std::array<float, 3> last{};
		};

		std::optional<bone_translation> translation_of(const iw7::XAnimParts& anim, const std::string& bone)
		{
			std::optional<unsigned> part;
			for (auto i = 0u; anim.names && i < anim.boneCount[9]; i++)
			{
				part = !part && script_name(anim.names[i]) == bone ? std::optional(i) : part;
			}

			if (!part)
			{
				return bone_translation{};
			}

			if (*part >= anim.boneCount[0])
			{
				return std::nullopt;
			}

			std::size_t db{}, ds{}, di{}, rds{}, rdb{};
			auto valid = true;
			const auto short_at = [&](const std::size_t index) -> unsigned
			{
				valid = valid && anim.dataShort && index < anim.dataShortCount;
				return valid ? static_cast<std::uint16_t>(anim.dataShort[index]) : 0u;
			};

			const auto byte_at = [&](const std::size_t index) -> unsigned
			{
				valid = valid && anim.dataByte && index < anim.dataByteCount;
				return valid ? static_cast<std::uint8_t>(anim.dataByte[index]) : 0u;
			};

			const auto float_at = [&](const std::size_t index)
			{
				valid = valid && anim.dataInt && index < anim.dataIntCount;
				float value{};
				if (valid)
				{
					std::memcpy(&value, &anim.dataInt[index], sizeof(value));
				}

				return value;
			};

			const auto frames = [&](const unsigned keys)
			{
				if (anim.numframes >= 256)
				{
					ds += keys >= 0x40 ? ((keys - 1) >> 8) + 2 : keys + 1;
				}
				else
				{
					db += keys + 1;
				}
			};

			for (const auto& [width, count] : {std::pair{2u, anim.boneCount[1]}, std::pair{4u, anim.boneCount[2]}})
			{
				for (auto i = 0u; i < count; i++)
				{
					const auto keys = short_at(ds++);
					frames(keys);
					rds += width * (keys + 1);
				}
			}

			ds += 2u * anim.boneCount[3] + 4u * anim.boneCount[4];
			for (const auto& [bytes_keys, count] : {std::pair{true, anim.boneCount[5]}, std::pair{false, anim.boneCount[6]}})
			{
				for (auto i = 0u; i < count; i++)
				{
					const auto owner = byte_at(db++);
					const auto keys = short_at(ds++);
					frames(keys);
					std::array<float, 6> range{};
					for (auto c = 0u; c < 6; c++)
					{
						range[c] = float_at(di + c);
					}

					di += 6;
					if (owner == *part)
					{
						const auto key = [&, bytes_keys](const std::size_t at, const unsigned c) -> float
						{
							if (bytes_keys)
							{
								valid = valid && anim.randomDataByte && rdb + at * 3 + c < anim.randomDataByteCount;
								return valid ? static_cast<float>(static_cast<std::uint8_t>(anim.randomDataByte[rdb + at * 3 + c])) : 0.0f;
							}

							valid = valid && anim.randomDataShort && rds + at * 3 + c < anim.randomDataShortCount;
							return valid ? static_cast<float>(static_cast<std::uint16_t>(anim.randomDataShort[rds + at * 3 + c])) : 0.0f;
						};

						bone_translation result;
						for (auto c = 0u; c < 3; c++)
						{
							result.first[c] = range[c] + key(0, c) * range[3 + c];
							result.last[c] = range[c] + key(keys, c) * range[3 + c];
						}

						return valid ? std::optional(result) : std::nullopt;
					}

					(bytes_keys ? rdb : rds) += 3 * (keys + 1);
				}
			}

			for (auto i = 0u; i < anim.boneCount[7]; i++)
			{
				const auto owner = byte_at(db++);
				const std::array<float, 3> value{float_at(di), float_at(di + 1), float_at(di + 2)};
				di += 3;
				if (owner == *part)
				{
					return valid ? std::optional(bone_translation{value, value}) : std::nullopt;
				}
			}

			return valid ? std::optional(bone_translation{}) : std::nullopt;
		}

		bool identity(const iw7::DObjAnimMat& pose)
		{
			return std::abs(pose.quat[0]) < 1e-4f && std::abs(pose.quat[1]) < 1e-4f && std::abs(pose.quat[2]) < 1e-4f &&
				std::abs(std::abs(pose.quat[3]) - 1.0f) < 1e-4f;
		}

		// The one bone that holds every vertex of a model, when it is the child of a root bone at the origin and neither
		// turns at the bind pose (the floor panels' tag_raise under tag_origin).
		std::optional<unsigned> single_moving_bone(const source_map& source, const iw7::XModel& model)
		{
			std::optional<unsigned> bone;
			for (auto lod = 0; lod < model.numLods && lod < 6; lod++)
			{
				const auto& info = model.lodInfo[lod];
				const auto surfaces = source.resolve(iw7::ASSET_TYPE_XMODEL_SURFS, info.modelSurfs);
				const auto streamed = info.modelSurfs ? source.streamed_surfaces(asset_name(info.modelSurfs->name)) : nullptr;
				for (std::size_t s = 0; s < info.numsurfs; s++)
				{
					std::vector<std::pair<std::uint16_t, std::uint16_t>> lists;
					auto blended = false;
					if (surfaces && surfaces->surfs && s < surfaces->numsurfs)
					{
						const auto& surface = surfaces->surfs[s];
						for (auto l = 0u; surface.rigidVertLists && l < surface.rigidVertListCount; l++)
						{
							lists.emplace_back(surface.rigidVertLists[l].boneOffsetIndex, surface.rigidVertLists[l].vertCount);
						}

						blended = std::ranges::any_of(surface.blendVertCounts, [](const auto count) { return count != 0; });
					}
					else if (streamed && s < streamed->size())
					{
						lists = (*streamed)[s].rigid_lists;
						blended = (*streamed)[s].blended;
					}

					if (blended || lists.empty())
					{
						return std::nullopt;
					}

					for (const auto& [offset, count] : lists)
					{
						const unsigned index = offset >= model.numBones ? offset / 64u : offset;
						if (count && bone && *bone != index)
						{
							return std::nullopt;
						}

						bone = count ? std::optional(index) : bone;
					}
				}
			}

			if (!bone || *bone < model.numRootBones || *bone >= model.numBones || !model.parentList)
			{
				return std::nullopt;
			}

			const auto parent = *bone - model.parentList[*bone - model.numRootBones];
			const auto& root = model.baseMat[parent];
			if (parent >= model.numRootBones || !identity(root) || !identity(model.baseMat[*bone]) ||
				std::abs(root.trans[0]) + std::abs(root.trans[1]) + std::abs(root.trans[2]) > 1e-3f)
			{
				return std::nullopt;
			}

			return bone;
		}
	}

	void export_animations(const source_map& source, extraction& output)
	{
		// Each model either moves as an entity (one bone that its animations only move) or plays its animations on its
		// skeleton.
		std::set<std::string> converted;
		std::set<std::string> skeletons;
		for (auto& [name, states] : output.scriptable_states)
		{
			const auto found = output.animated_models.find(name);
			const auto* model = found != output.animated_models.end() ? found->second : nullptr;
			if (!model)
			{
				continue;
			}

			if (const auto bone = single_moving_bone(source, *model))
			{
				const auto bone_name = script_name(model->boneNames[*bone]);
				const auto& bind = model->baseMat[*bone].trans;
				auto moves = true;
				for (auto& state : states)
				{
					const auto translation = translation_of(*state.anim, bone_name);
					moves = moves && translation.has_value();
					if (translation)
					{
						for (auto c = 0u; c < 3; c++)
						{
							state.to[c] = translation->last[c] - bind[c];
						}

						state.time = state.anim->framerate > 0.0f ? state.anim->numframes / state.anim->framerate : 0.0f;
					}
				}

				if (moves)
				{
					output.moving_models.insert(name);
					converted.insert(name);
					for (auto& state : states)
					{
						state.moves = true;
						ZONETOOL_INFO("iw7: scriptable model \"%s\" moves to (%g %g %g) over %gs in state %s:%s (%s)", name.data(),
							state.to[0], state.to[1], state.to[2], state.time, state.part.data(), state.state.data(), state.anim->name);
					}

					continue;
				}
			}

			if (!write_skeleton(source, name, *model, output.root / "skeleton"))
			{
				continue;
			}

			skeletons.insert(name);
			std::erase_if(states, [&](const auto& state)
			{
				const std::string anim(asset_name(state.anim->name));
				if (output.xanims.contains(anim))
				{
					return false;
				}

				if (!write_xanim(*state.anim, output.extracted / "xanim" / (anim + ".iw3.json")))
				{
					return true;
				}

				output.xanims.emplace(anim, state.anim);
				return false;
			});

			if (!states.empty())
			{
				converted.insert(name);
			}
		}

		std::erase_if(output.scriptable_states, [&](const auto& entry) { return !converted.contains(entry.first); });

		// The scriptables whose models do not convert keep their bind pose.
		std::erase_if(output.scriptable_animations, [&](const auto& entry)
		{
			const auto models = output.scriptable_animation_models.find(entry.first);
			return models == output.scriptable_animation_models.end() || std::ranges::any_of(models->second, [&](const auto& model)
			{
				return !converted.contains(model);
			});
		});

		ZONETOOL_INFO("iw7: %zu kinds of scriptables animate (%zu models move, %zu skeletons, %zu animations)",
			output.scriptable_animations.size(), output.moving_models.size(), skeletons.size(), output.xanims.size());
	}

	std::optional<float> export_script_animation(const source_map& source, const std::string& name, extraction& output)
	{
		const auto* anim = static_cast<const iw7::XAnimParts*>(source.find(iw7::ASSET_TYPE_XANIMPARTS, name));
		if (!anim || (!output.xanims.contains(name) && !write_xanim(*anim, output.extracted / "xanim" / (name + ".iw3.json"))))
		{
			return std::nullopt;
		}

		output.xanims.emplace(name, anim);
		// The length as the scriptable states take it: the frames over the frame rate.
		return anim->framerate > 0.0f ? anim->numframes / anim->framerate : 0.0f;
	}

	// The skeletons of the other exported models with bones: entity models keep their tags (effects on tags, destructible
	// parts, gettagorigin) as IW7 has them.
	void export_skeletons(const source_map& source, extraction& output)
	{
		std::size_t written{};
		for (const auto& [name, model] : output.models)
		{
			if (!model || model->numBones < 2 || !output.exported_models.contains(name) ||
				std::filesystem::exists(output.root / "skeleton" / "xmodel" / (name + ".json")))
			{
				continue;
			}

			written += write_skeleton(source, name, *model, output.root / "skeleton");
		}

		ZONETOOL_INFO("iw7: wrote the skeletons of %zu more models with bones", written);
	}
}

#include <std_include.hpp>
#include "export.hpp"

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		json vector_json(const float* values, const std::size_t count)
		{
			auto result = json::array();
			for (std::size_t i = 0; i < count; i++)
			{
				result.push_back(values[i]);
			}

			return result;
		}

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
		json export_vertex(const Vertex& vertex)
		{
			const auto normal = unpack_unit_vec(vertex.normal);
			const auto tangent = unpack_unit_vec(vertex.tangent);
			float uv[2]{};
			PackedVec::Vec2UnpackTexCoords(vertex.texCoord.packed, uv);
			// The top two bits of the tangent hold the binormal sign.
			const auto binormal_sign = (vertex.tangent.packed >> 30) ? 1.0f : -1.0f;
			return
			{
				{"position", vector_json(vertex.xyz, 3)},
				{"normal", vector_json(normal.data(), 3)},
				{"tangent", vector_json(tangent.data(), 3)},
				{"binormal_sign", binormal_sign},
				{"uv", vector_json(uv, 2)},
				{"lightmap_uv", {0.0f, 0.0f}},
				{"color", {255, 255, 255, 255}},
			};
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
		json export_surface(const Vertices& source_vertices, const Faces& faces, const std::string& material)
		{
			auto vertices = json::array();
			for (const auto& vertex : source_vertices)
			{
				vertices.push_back(export_vertex(vertex));
			}

			auto indices = json::array();
			for (const auto& face : faces)
			{
				indices.push_back(face.v1);
				indices.push_back(face.v2);
				indices.push_back(face.v3);
			}

			return
			{
				{"deformed", false},
				{"material", material},
				{"reflection_probe", 0},
				{"vertices", std::move(vertices)},
				{"indices", std::move(indices)},
			};
		}

		bool export_model(const source_map& source, const std::string& name, const iw7::XModel* model, extraction& output)
		{
			if (!model || !model->numLods)
			{
				return false;
			}

			auto table_lods = json::array();
			auto geometry_lods = json::array();
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
					return false;
				}

				auto lod_surfaces = json::array();
				for (std::size_t i = 0; i < info.numsurfs; i++)
				{
					const auto material_name = output.add_material(source, model->materialHandles[info.surfIndex + i]);
					auto largest = 0.0f;
					if (surfaces && surfaces->surfs && i < surfaces->numsurfs)
					{
						const auto& surface = surfaces->surfs[i];
						const std::span faces(surface.triIndices, surface.triCount);
						const std::span motion(surface.verts0.packedMotionVerts0, surface.vertCount);
						const std::span packed(surface.verts0.packedVerts0, surface.vertCount);
						largest = (surface.flags & 8) != 0 ? largest_coordinate(motion) : largest_coordinate(packed);
						lod_surfaces.push_back((surface.flags & 8) != 0 ? export_surface(motion, faces, material_name)
							: export_surface(packed, faces, material_name));
					}
					else if (streamed && i < streamed->size())
					{
						const auto& surface = (*streamed)[i];
						largest = surface.motion ? largest_coordinate(surface.motion_vertices) : largest_coordinate(surface.vertices);
						lod_surfaces.push_back(surface.motion
							? export_surface(std::span(surface.motion_vertices), std::span(surface.faces), material_name)
							: export_surface(std::span(surface.vertices), std::span(surface.faces), material_name));
					}

					if (largest > largest_model_coordinate)
					{
						ZONETOOL_WARNING("iw7: model \"%s\" reaches %.0f units, beyond the range of Replay model bounds: the sky draws it",
							name.data(), largest);
						output.distant_models.insert(name);
						return false;
					}
				}

				// IW7 LOD distances need not grow (650, 419.8, 2500); Replay takes a strictly growing sequence.
				previous_distance = lod + 1 == model->numLods ? std::max(1000000.0f, previous_distance + 1.0f)
					: std::max(previous_distance + 1.0f, std::isfinite(info.dist) ? info.dist : 0.0f);
				const auto distance = previous_distance;
				table_lods.push_back({{"distance", distance}, {"file", std::format("model_export/{}_lod{}.obj", name, lod)}});
				geometry_lods.push_back({{"distance", distance}, {"surfaces", std::move(lod_surfaces)}});
			}

			write_json(output.extracted / "xmodel" / (name + ".json"),
			{
				{"$schema", "http://openassettools.dev/schema/xmodel.v1.json"},
				{"_game", "iw3"},
				{"_type", "xmodel"},
				{"_version", 2},
				{"collLod", model->collLod < model->numLods ? static_cast<int>(model->collLod) : -1},
				{"flags", 0},
				{"lods", std::move(table_lods)},
				{"rootBoneName", ""},
				{"type", "rigid"},
			});

			write_json(output.extracted / "xmodel" / (name + ".replay.json"),
			{
				{"schema", 1},
				{"name", name},
				{"lods", std::move(geometry_lods)},
			});

			return true;
		}
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
		for (const auto& [name, model] : output.models)
		{
			if (export_model(source, name, model, output))
			{
				output.exported_models.insert(name);
				written++;
			}
			else if (!output.distant_models.contains(name))
			{
				ZONETOOL_WARNING("iw7: model \"%s\" is not in the loaded zones", name.data());
			}
		}

		ZONETOOL_INFO("iw7: exported %zu of %zu static models", written, output.models.size());
	}
}

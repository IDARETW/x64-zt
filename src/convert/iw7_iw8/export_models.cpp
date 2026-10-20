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

		bool export_model(const source_map& source, const std::string& name, const iw7::XModel* model, extraction& output)
		{
			if (!model || !model->numLods)
			{
				return false;
			}

			// A model that the world places beyond the range of model bounds spans the map: the sky draws it (Breakneck's
			// black hole, 4932 units placed at scale 60, cut through the level with its default-white disc).
			const auto placement_scale = largest_placement_scale(source, name);

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

					if (largest * placement_scale > largest_model_coordinate)
					{
						ZONETOOL_WARNING("iw7: model \"%s\" reaches %.0f units (placed at scale %g), beyond the range of Replay model "
							"bounds: the sky draws it", name.data(), largest * placement_scale, placement_scale);
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
				// IW7 points collLod at a model's coarsest LOD (Frontier: 130 of its 163 three-LOD models name LOD 2), and
				// a collision mesh of that LOD let bullets pass parts of models that LOD0 shows (owner test 2026-10-18):
				// models collide with the LOD that players see up close.
				{"collLod", 0},
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
		// counts, frame indices and keys), which Replay decodes as IW3 does. Root motion (delta parts) does not convert.
		bool write_xanim(const iw7::XAnimParts& anim, const std::filesystem::path& path)
		{
			if (anim.deltaPart && (anim.deltaPart->trans || anim.deltaPart->quat2 || anim.deltaPart->quat))
			{
				ZONETOOL_WARNING("iw7: animation \"%s\" moves its root (a delta part), which does not convert", anim.name);
				return false;
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
				{"delta_part", nullptr},
			});
			return true;
		}

		// The skeleton of a model as the OpenAssetTools XMODEL_EXPORT (version 6) of each LOD that the pipeline reads for entity
		// models: the bones at their bind pose (DObjAnimMat base poses, model space), and each vertex on the bone of its rigid
		// vertex list, with the triangles of every surface in the order of export_model. Models with blended vertices do not
		// convert.
		bool write_skeleton(const source_map& source, const std::string& name, const iw7::XModel& model, const std::filesystem::path& root)
		{
			const auto fail = [&](const char* reason)
			{
				ZONETOOL_WARNING("iw7: the skeleton of model \"%s\" does not export (%s); it does not animate", name.data(), reason);
				return false;
			};

			if (!model.boneNames || !model.baseMat || !model.parentList || model.numBones < 2 || model.numBones > 128)
			{
				return fail("bones");
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
					auto blended = false;
					if (resident)
					{
						const auto& surface = surfaces->surfs[s];
						for (auto l = 0u; surface.rigidVertLists && l < surface.rigidVertListCount; l++)
						{
							lists.emplace_back(surface.rigidVertLists[l].boneOffsetIndex, surface.rigidVertLists[l].vertCount);
						}

						blended = std::ranges::any_of(surface.blendVertCounts, [](const auto count) { return count != 0; });
					}
					else
					{
						lists = (*streamed)[s].rigid_lists;
						blended = (*streamed)[s].blended;
					}

					if (blended || lists.empty())
					{
						return fail("blended vertices, or no rigid vertex lists");
					}

					// The bone of each vertex: the rigid lists cover the vertices in order. IW7 gives each list its bone index
					// (IW3 its offset in 64-byte bone matrices).
					std::vector<unsigned> vertex_bones;
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

						vertex_bones.insert(vertex_bones.end(), count, bone / divisor);
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
							vertices += std::format("VERT {}\nOFFSET {:.6f}, {:.6f}, {:.6f}\nBONES 1\nBONE {} 1.000000\n", vertex_count++,
								xyz[0], xyz[1], xyz[2], vertex_bones[v]);
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

	void export_animations(const source_map& source, extraction& output)
	{
		std::set<std::string> skeletons;
		for (const auto& [name, model] : output.animated_models)
		{
			if (model && write_skeleton(source, name, *model, output.root / "skeleton"))
			{
				skeletons.insert(name);
			}
		}

		std::set<std::string> written;
		for (const auto& [name, anim] : output.xanims)
		{
			if (anim && write_xanim(*anim, output.extracted / "xanim" / (name + ".iw3.json")))
			{
				written.insert(name);
			}
		}

		// The scriptables whose model or animation does not convert keep their bind pose, with their effects at the bind
		// pose of their tags.
		std::erase_if(output.scriptable_animations, [&](const auto& entry)
		{
			const auto& [targetname, anim] = entry;
			const auto model = output.scriptable_animation_models.find(targetname);
			return !written.contains(anim) || model == output.scriptable_animation_models.end() || !skeletons.contains(model->second);
		});

		std::erase_if(output.scriptable_tag_effects, [&](const auto& effect)
		{
			if (output.scriptable_animations.contains(effect.targetname))
			{
				return false;
			}

			output.scriptable_effects.push_back(effect.placement);
			return true;
		});

		std::erase_if(output.xanims, [&](const auto& entry)
		{
			return !written.contains(entry.first) || std::ranges::none_of(output.scriptable_animations, [&](const auto& animation)
			{
				return animation.second == entry.first;
			});
		});
		ZONETOOL_INFO("iw7: %zu kinds of scriptables animate (%zu animations, %zu skeletons, %zu effects on their tags)",
			output.scriptable_animations.size(), output.xanims.size(), skeletons.size(), output.scriptable_tag_effects.size());
	}
}

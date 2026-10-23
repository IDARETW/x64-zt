games = {
	known = {"h1", "h2", "iw3", "iw6", "iw7", "iw8", "s1", "s2"},
	enabled = {},
	dependencies = {
		h1 = function()
			dependencies.use(directxtex)
			gsc_tool.import_engine("h1")
		end,
		h2 = function()
			dependencies.use(directxtex)
			gsc_tool.import_engine("h2")
		end,
		s1 = function()
			dependencies.use(directxtex)
			gsc_tool.import_engine("s1")
		end,
		s2 = function()
			dependencies.use(directxtex)
			gsc_tool.import_engine("s2")
		end,
		iw6 = function()
			dependencies.use(directxtex)
			gsc_tool.import_engine("iw6")
		end,
		iw7 = function()
			dependencies.use(directxtex)
		end,
		iw3 = function()
		end,
		iw8 = function()
			dependencies.use(directxtex)
			filter "kind:ConsoleApp"
				links {"bcrypt", "d3dcompiler", "d3d11"}
			filter {}
		end,
	},
	converter_imports = {
		iw3_iw8 = function()
			files {"src/convert/iw3_iw8/resources.rc"}
			resincludedirs {"src/convert/iw3_iw8"}
			-- The Havok baker maps the Replay executable at its own image base until native Havok replaces it.
			linkoptions {"/BASE:0x180000000", "/DYNAMICBASE:NO"}
			-- Media Foundation decodes IW3 compressed (MP3) loaded sounds.
			links {"mfplat", "mfreadwrite", "mfuuid", "shlwapi"}
		end,
	},
	converter_requires = {
		s1_h2 = {"h1"},
		s1_s2 = {"h1"},
		-- IW7 maps go through the map pipeline of the IW3 converter.
		iw7_iw8 = {"iw3"},
	},
	converter_dependencies = {
		iw3_iw8 = function()
			-- Map conversion is compute bound (image encoding, light, shadow and collision baking).
			filter "configurations:Release"
				optimize "Speed"
			filter {}
		end,
		s1_s2 = function()
			games.dependencies.h1()
		end,
		h1_s2 = function()
			dependencies.use(shader_tool)
		end,
		h2_h1 = function()
			dependencies.use(shader_tool)
		end,
		iw6_h1 = function()
			dependencies.use(shader_tool)
		end,
		iw7_iw8 = function()
			links {"convert-iw3-iw8"}
			-- IW7 entity keys are script string tokens.
			gsc_tool.import_engine("iw7")
			filter "configurations:Release"
				optimize "Speed"
			filter {}
		end,
	},
}

local function select_games()
	local requested = _OPTIONS["games"] or "h1"
	if requested == "all" then
		return games.known
	end

	local selected = {}
	for name in requested:gmatch("[^,+%s]+") do
		if not table.contains(games.known, name) then
			error("Unknown game \"" .. name .. "\". Known games: " .. table.concat(games.known, ", "))
		end
		table.insert(selected, name)
	end
	return selected
end

games.enabled = select_games()

function games.requirements_met(name)
	for _, required in ipairs(games.converter_requires[name] or {}) do
		if not table.contains(games.enabled, required) then
			return false
		end
	end
	return true
end

function games.converters()
	if _OPTIONS["no-convert"] then
		return {}
	end

	local converters = {}
	for _, dir in ipairs(os.matchdirs("src/convert/*")) do
		local source, target = path.getname(dir):match("^(%w+)_(%w+)$")
		if source and table.contains(games.enabled, source) and table.contains(games.enabled, target) and games.requirements_met(source .. "_" .. target) then
			table.insert(converters, {source = source, target = target})
		end
	end
	return converters
end

function games.write_enabled_header()
	local lines = {"#pragma once", ""}

	local game_list = {}
	for _, name in ipairs(games.enabled) do
		table.insert(game_list, "x(" .. name .. ")")
	end
	table.insert(lines, "#define ZONETOOL_GAMES(x) " .. table.concat(game_list, " "))

	local converter_list = {}
	for _, converter in ipairs(games.converters()) do
		table.insert(converter_list, "x(" .. converter.source .. ", " .. converter.target .. ")")
	end
	table.insert(lines, "#define ZONETOOL_CONVERTERS(x) " .. table.concat(converter_list, " "))

	local content = table.concat(lines, "\n") .. "\n"
	local folder = path.join(_MAIN_SCRIPT_DIR, "build/generated")
	local file = path.join(folder, "enabled_games.hpp")

	if io.readfile(file) ~= content then
		os.mkdir(folder)
		io.writefile(file, content)
	end
end

local function game_project(name)
	project("game-" .. name)
		kind "StaticLib"
		language "C++"

		pchheader "std_include.hpp"
		pchsource("src/games/" .. name .. "/std_include.cpp")

		files {
			"src/games/" .. name .. "/**.hpp",
			"src/games/" .. name .. "/**.cpp",
		}

		includedirs {
			"src/games/" .. name,
			"src/games",
			"src/core",
			"src/common",
			"src",
		}

		links {"core", "common"}

		core.includes()
		games.dependencies[name]()

		-- The IW8 map baking (sun shadows, Havok shapes, zone writing) is compute bound.
		if name == "iw8" then
			filter "configurations:Release"
				optimize "Speed"
			filter {}
		end
end

local function converter_project(converter)
	local name = converter.source .. "_" .. converter.target

	project("convert-" .. converter.source .. "-" .. converter.target)
		kind "StaticLib"
		language "C++"

		pchheader "std_include.hpp"
		pchsource("src/convert/" .. name .. "/std_include.cpp")

		files {
			"src/convert/" .. name .. "/**.hpp",
			"src/convert/" .. name .. "/**.cpp",
		}

		includedirs {
			"src/convert/" .. name,
			"src/games",
			"src/core",
			"src/common",
			"src",
		}

		links {"game-" .. converter.source, "game-" .. converter.target}

		core.includes()
		games.dependencies[converter.source]()
		games.dependencies[converter.target]()

		if games.converter_dependencies[name] then
			games.converter_dependencies[name]()
		end
end

function games.projects()
	group "Games"
	for _, name in ipairs(games.enabled) do
		game_project(name)
	end

	group "Converters"
	for _, converter in ipairs(games.converters()) do
		converter_project(converter)
	end

	group ""
end

function games.import()
	for _, name in ipairs(games.enabled) do
		links {"game-" .. name}
		games.dependencies[name]()
	end

	for _, converter in ipairs(games.converters()) do
		links {"convert-" .. converter.source .. "-" .. converter.target}

		local import = games.converter_imports[converter.source .. "_" .. converter.target]
		if import then
			import()
		end
	end
end

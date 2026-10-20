# AGENTS.md

Guide for humans and agents who work on x64-zt. Read it before you change code.

## What x64-zt is

x64-zt is a fastfile unlinker and linker for x64 Call of Duty titles. It reads `.ff` zones that every Call of Duty game uses, dumps their assets to disk, and enables building new zones from those dumps or adding new assets into it no problem. It can dual-purpose convert assets from one game to another as well.

The `v2` branch is a standalone rewrite of x64-zt with the assistance of AI to help add new games and keep a cleaner codebase. It reads fastfiles directly, and never loads a game exe via runtime methodology anymore like the main branch. v1 (`main`) loaded the game binary and hooked its zone loader. v2 keeps the v1 asset logic and the v1 dump formats. Current scope is H1 (MWR 1.15), IW6, IW7 and IW8 (Modern Warfare 2019, Replay 1.20). See `docs/IW6_SUPPORT.md` for IW6. Other games are currently not supported as they need to be re-added, but can be done with due time easily.

## Build

This was only tested for Windows using Visual Studio 2022 via MSBuild and premake.

- `generate.bat --games=<games>` is how you select which games you want to build (all by default)
- `--games=h1`, `--games=h1,iw7` or `--games=all` selects the games
- `--no-convert` skips the converter projects if you are not doing any conversions. A converter project `convert-<a>-<b>` builds only when both games are selected. Each game is a separate static library, so a change in one game does not rebuild the other.
- `--copy-to=<folder>` copies `zonetool.exe` to that folder after each build.
- premake writes `build/generated/enabled_games.hpp` with `ZONETOOL_GAMES(x)` and `ZONETOOL_CONVERTERS(x)`. Do not edit it.

## Run

```bash
zonetool.exe -game h1 dumpzone mp_shipment
```

| Command | Result |
|---|---|
| `inspect <zone>`, `inspect --all` | Quick inspect a fastfile container and its data |
| `verifyzone <zone>`, `verifyzone --all` | Reads every asset and checks that the reader consumed the zone exactly |
| `dumpzone <zone> [--target <game>]` | Writes `dump/<zone>/` and `dump/<zone>/<zone>.csv`. `--target` converts the assets |
| `dumpasset <type> <name> <zone> [--target <game>]` | Reads `<zone>` and writes one asset to `dump/assets/`. `--target` converts the asset |
| `buildzone <zone>` | Reads `zone_source/<zone>.csv` and the files in `zonetool/<zone>/`, then writes `<game folder>/zone/<zone>.ff` |

Without a command, `zonetool.exe` opens an interactive console.

- Game selection: `-game <name>`, then `zonetool.json` `game`.
- Game folder: `-path <folder>`, then `zonetool.json` `paths.<game>`, then the exe folder if it has `zone/`, then the Steam default folder.
- `zonetool.json` example: `{"game": "h1", "paths": {"h1": "D:/Games/MWR"}}`.
- `dumpasset` finds only assets that the zone holds, not referenced ones (`type,,name` rows in the CSV). Type names are the CSV type names.
- Flags for `dumpzone`: `-dds` writes DDS images. `-dump_streamed_image` reads `imagefile*.pak` and writes streamed images.
- The `require` row in a build CSV loads another zone into the asset database. There is no `loadzone` command.
- `buildzone` writes `<zone>.ff` and `<zone>.pak` to the `zone/` folder of the game folder. It replaces a zone of the same name.
- `-no_cache` writes every output again and ignores the build cache.
- `dumpzone <iw3 zone> --target iw8` converts a whole IW3 map into the Replay 1.20 zones in `output/<map>/`. Options:
  `--map <name>`, `--style iw3|iw8`, `--unlinker <exe>`, `--gsc_tool <exe>`, `--replay <exe>` (its folder is the Replay
  game folder), `--metadata <json>`, `--acts <exe>` (validates the zones), `--havok native|native-check|exe` (Havok data
  built natively, the default; built natively and checked with the executable; or baked by the executable),
  `--writer x64-zt|iw8-zonetool` (the IW8 zone writer lays out each zone, the default; or the zone keeps the stream
  layout of the converter, for byte comparisons with iw8-zonetool builds), `--dump 1` (also dumps each zone for
  `buildzone`), `--detail_collision 0|1` (each static model also gets an `XModelDetailCollision` LOD baked natively,
  as shipped zones have; the default is 1 with native Havok, and it is always off with `--havok exe`), `--bc7_images 0|1` (the world and model
  images of each material are block compressed with the BC7 mode 6 encoder when the encoding keeps their detail (PSNR of 38 dB for colour,
  40 for the others, else they stay RGBA8); the default follows the extraction, which IW7 maps turn on). Tool paths can also go in `zonetool.json` `tools`.
- `dumpzone <iw7 map zone> --target iw8` converts an IW7 map with the same options. `convert/iw7_iw8` writes the map
  to `output/.iw7-extract/<zone>/` in the layout of the IW3 extraction cache, and the IW3 map pipeline converts that
  extraction. The zone folder must hold `techsets_<zone>.ff` (the map's materials), the multiplayer common zones and
  the `imagefile<N>.pak` and `assetfile<N>.pak` files that the map streams its images and model surfaces from. The
  extraction has the world surfaces, static and entity models, materials with their colour, normal and specular
  images, the lightmaps, the reflection probes, the sky, the sun, the local lights, the breakable glass, spawn points,
  triggers and entities; the sky also shows the models that are too large for Replay model bounds (planets). The
  light grid and the lighting of static models come from the lightmaps of the floors under them, not from IW7's own
  light grid. The collision comes from IW7's Havok data (`havok.cpp` reads its Havok 2014 packfiles): the convex
  pieces of the world's compressed mesh and of the brush models' compound shapes become brushes, the other
  primitives the world's collision triangles, with the contents, surface types and surface flags of their shape tags
  (the encoding of Replay's), and each glass piece the hull of its pane. Positions are in units of 32 game units. A
  map without Havok data collides by its solid render surfaces. The effects that the map's createfx script places
  (read by decompiling the map's script files with `--gsc_tool`) become IW3 map scripts of the extraction (the map
  script, its `_fx` script and its createfx script), and their particle systems native Replay effects
  (`extracted/vfx/<name>.json`: the records in Replay's layout, which `export_effects.cpp` converts module by module,
  with their curves and the assets they link by source name), which the pipeline resolves and registers; their
  materials become IW3 effect materials. The zones of the common effect materials (`techsets_common`,
  `techsets_common_core_mp`, `techsets_common_mp`, `techsets_global_core_mp`) and the global zones (`global_core_mp`,
  `global_mp`) must be in the zone folder too: a map zone holds some shared images without their pixels (format 0),
  and an image of the same name with its pixels in another loaded zone replaces such an image. Physics light modules
  (debris that Havok simulates: shards of broken bottles and vases) keep their layout; their IW7 shapes (point lists in
  game units, which the extraction writes in Havok units, `extracted/physicsfxshape/<name>.json`) become Replay physics shapes `mw120r/<map>/<name>` built
  natively (a shape of a zone that is not loaded takes Replay's `vfx_physfxpip_particle_default`), and their pipelines
  Replay's `physfxpip_sparks_callback` (sparks) or `physfxpip_debris_callback` (the rest), name-only references to the
  common zones. IW7's emissive graphs are the first version of Replay's (two intensities); they take Replay's third (10
  curves: white colours, the intensities at curves 3 and 8 times the map's light unit). Sound modules keep the aliases
  of the map's sound bank, which convert with its sounds; the importer leaves out those that the map bank does not
  hold. Decal modules (IW7: byte fade times, the projection axis at 15, the marks at 32) name their world material in
  Replay's three slots, which converts as a decal material (decal camera region, `wc_l_sm_b0c0n0s0`); a decal whose
  colour map cannot be read is left out. Spot lights (element 8) convert as Replay spots (element 6, module 10,
  game-test PDB layout: the collimation, near plane 8 and `light_fx_default` of stock spots); Replay points them along
  the effect's X axis (`sub_141BA6670`). No stock Replay effect spawns on a mesh, so a spawn mesh becomes the sphere of
  its first model's bounds (Frontier: the ops table's sun). Replay has no cloud element (IW7's clouds draw as sprites)
  and its cloud module no data: cloud modules are left out. The map's scriptables become script models of
  each instance's model at its initial placement (the game modes' own, such as Hardpoint's chevrons, are left out),
  which the pipeline makes static when no script uses them. The kinds that break (one part: a health state, then a
  state that swaps the model and plays effects and a sound; Frontier: 172 cups, bottles, screens and clocks of 41
  kinds) take a targetname `iw7_destructible_<kind>`, and the map script (`iw7_destructibles`) breaks each when it has
  taken its health in damage: the broken model (or none), the first effect, the sound. A script model that is all
  glass keeps its glass in its collision, and a model with no solid collision keeps no physics. Their other states,
  animations (Frontier's rising floor panels) and events do not convert.
  The map's own script (`scripts\mp\maps\<map>\<map>`) becomes the IW3 map script: the functions that its main
  reaches, with IW7's utilities replaced by IW3 ones or their IW3 versions, the names that gsc-tool writes as IW7
  token ids (`_id_XXXX`) renamed, and the effects that it plays on the tags of converted models (which have one bone)
  on `tag_origin`; its animations, cinematics and scriptable states do nothing. The loop sounds that the map's sound
  script places become IW3 createfx loop sounds, and the aliases of the map's sound bank (`<map>.all`) that the
  scripts name become IW3 alias definitions (`extracted/soundaliases/<alias>.iw3.json`) with their sounds as wave
  files, decoded from the FLAC frames of the bank's `.sabl`, which must be in the zone folder.
  `IW7_EXPORT_ONLY=1` stops after the extraction, and `IW7_REUSE_EXTRACTION=1` converts the complete extraction of
  an earlier run again.
- IW7 materials: a colour map of `TS_COLOR_SPECULAR_MAP` (`*_packed_cs`) holds the specular reflectance in its alpha;
  the surface's alpha is the texture of `TS_ALPHA_REVEAL_THICKNESS_MAP` of the same layer (`*_packed_a`, `_ar`,
  `_r`: its channels in the order of the letters). The extraction writes the colour with that alpha
  (`<colour>_a<hash>`), or opaque. Layered world materials (`..._b1..._b2...`) blend each layer by a vertex colour
  channel (green, blue, alpha) and that alpha. A premultiplied (`blendadd`) material blends by its alpha when its
  colour holds one, else adds. IW7 normal maps keep x in alpha and y in green (packed ones keep the gloss in red and
  the occlusion in blue). Unlit materials emit their tint in the units of the converted lights: below the sun's
  level the tint itself, above it the hue of their tint and an `emissionScale` constant, which the pipeline gives the world shader (`MAP_EMISSION`, flag bit 20) and the model
  shaders (a technique set variant for each gain, `MAP_MODEL_EMISSION`). The sky and the models that it draws (planets) take the units of the converted lights.
- The pipeline keeps shadow maps for the 16 most important shadowed spot lights of a map (`limitShadowedSpotLights`).
  Replay lists at most 64 spots of the view cone a frame, each with a shadow-map cache entry and its atlas slot, but it
  renders a missing shadow only for the first 16 in score order: a later spot is deferred and keeps a slot that holds
  no shadow of it. A converted world lets every spot in the view cone compete, walls or not, so with 40 the spots past
  the 16th went dark or lit as the view turned (owner tests of Frontier). Stock mp_hideout has 37, culled by Umbra.
- The w of each light-volume vertex is its packed inward normal (10 bits a component, top two bits set), as in stock
  volumes: the binning vertex shader moves each vertex out along it by a tile-sized expansion. A w of 1.0 decoded a
  normal of (-1, -1, 0.99) and shifted whole volumes with the view.
- A shadowed spot inside its fixture (IW7 lamps) starts its shadow map past the casters within 32 units of it
  (`ComPrimaryLight` shadowNearPlaneBias, 8 to 60 in shipped worlds); one whose cone those casters close within 64
  units on most rays keeps its light without a shadow map (`dropEnclosedShadowedSpots`).
- Light volumes (GfxWorld `frustumLights`, which the forward+ light binning rasterizes) stay as small as stock ones:
  spots of 8 sides with cap rings of at most 30 degrees (26 vertices at most), omnis a sphere of 6 sides and 6 bands
  (32 vertices, like stock). A spot's volume starts at the apex of the shader's cone, which lies the bulb radius
  divided by the tangent of the outer angle behind the light. Volumes of up to 114 vertices left lit floors ending
  on screen-tile steps that changed with the view.
- The compressed mesh writer leaves out the triangles that its vertex grids collapse (a large mesh packs with a
  coarser tolerance) and builds again without them: Replay's trees hold only primitives with an area. With more than
  one section, keys keep 8 bits for the primitive and its triangle (Replay takes the section from above bit 8, as
  every stock multi-section mesh does; narrower keys crashed the shape-tag lookup at 0x141E30200).
- An IW7 effect material without its colour map (an image in a pak that is not in the zone folder) adds nothing.
- IW7 `psfalloff` unlit materials (the ops table's hologram planes) take the IW3 falloff constants: without them their
  tints of 17 drew as solid white discs.
- The extraction's world JSON sets `translucent_glass`: the pipeline then draws blended BSP surfaces in the
  translucent camera region (3, 26), after the sky. In the opaque region the sky covered Frontier's roof glass.
  IW3 conversions do not set it.
- IW7 particle flares (element 3) take Replay's flare materials (`MTL_TYPE_EFFECT_FLARE`, screen-space flare
  vertices): common_mp's `ecf/vfx_postfx_*` / `ecf/gfx_flare_wtr` by the role in the IW7 name, with the stock material
  data (`stock_flare`). As effect sprites they drew screen-wide wedges that flickered with the occlusion queries
  (Breakneck's sun and klaxon flares, owner test breakneck12). Replay's flare builder (`sub_141BA5920`) scales a flare by
  value.min + value.max * (depth - range.min) / range.max, unclamped; IW7's ranges (1 to 99, value 0 to 1) are a
  clamped ramp, which as Replay's scaled far flares by depth / 99 (still screen-filling, breakneck14). Flares take no
  depth scale and IW7's value.max in their radial scales. `IW7_NO_FLARES=1` leaves them out.
- The IW7 source loads the base `common` and `global` zones after the multiplayer ones: they hold assets that map
  effects name, such as the spark debris shapes.
- IW7 extractions set `spatial_surfaces`: world surfaces merge only within 512-unit cells and up to 4096 triangles.
  Merged by material alone, Frontier drew 388 surfaces of up to 42793 triangles and 34000 units, which GPU culling and
  the shadow maps of local lights took whole (stock mp_hideout: 3301 surfaces, 28 triangles and 207 units at the median;
  Frontier now 2748, 20 triangles at the median).
- Each Replay particle element draws from its module: beam, decal, flare, geo trail, omni light, model and runner states
  read it at their particles' update (`sub_141BA52C0` dispatches by element; every stock state has its module). An emitter
  with a state whose module did not convert is left out: Breakneck's `vfx_rivet_spotlight_orange` kept its spot element
  without the spot module and crashed the map load (null read at 0x141BA67BA). IW7 spot lights convert as omni lights.
- A crash of the game with no dialog still leaves a full dump in `%LOCALAPPDATA%\CrashDumps` and an Application event
  (1000/1001) with the fault offset. The DW executable's `.text` sits 0x4A000 above the stock one; its `.map` names each
  function after its stock address (`sub_141BA6670`).
- IW7 colour maps of `TS_COLOR_SPECULAR_MAP` keep the linear reflectance at normal incidence (F0 x 255) in alpha: plastics
  and tiles 10, floors 5, metals 250, chrome 125. The extraction writes each as an IW3 specular map too (`<colour>_f0[_g<hash>]`:
  the reflectance gamma encoded in RGB, the gloss of the material's packed normal map, its red, in alpha). Without them every
  surface was matte and glass, which shows only by its reflection, was missing (Frontier's patterned roof glass).
- IW7 models take their collision from LOD 0 (`collLod` 0): IW7 names its coarsest LOD, which left parts of models
  (the lower units of a server rack) without collision.
- The effects that the first state of each part of a placed scriptable plays (the ops table's planets and asteroids)
  become createfx one-shot effects on the tags of its model at the bind pose; their animation does not convert.
- The zone folder needs the `assetfile<N>.pak` files that the map's models stream their surfaces from. A model whose
  surfaces are in none is left out, and so is an effect module that names only such models.
- IW7 zones are about twice the size of the largest stock zone (1.0 GB): Frontier's holds 2.12 GB of zone data with detail
  collision (images 73%: the RGBA8 world and model images are 1.1 GB of it). The writer warns above 2 GiB, and nothing that large
  has loaded in Replay. The IW7 extraction sets `bc7_images`, which block compresses those images (about a quarter of the size).
  `IW8_READ_TRACE=<file>` on a `verifyzone` also writes an `A <type> <name>` line after each asset, so the reads between two of
  them are the data of the second one (the byte count of each asset type).
- Breakneck's main zone is 2.05 GB. ACTS test mode loads all its assets with no read errors, but reports one failure: the 
  world (`gfx_map_trzone`, 146 MB) "exceeds the output budget" of its structured export. That is a limit of the ACTS exporter, not a reader error.

## Build cache

The cache makes a second `dumpzone` or `buildzone` of the same zone faster. It does not change the asset files, so v1 and tools without the cache read the output.

- `dump/<zone>/.zonetool_cache` records a fingerprint and a file stamp (size and write time) for each cached dump file. If both match, the dumper skips the file.
- `zonetool_cache/<zone>.pak.streams` records the stream offsets of the last `.pak`. If the streamed image files and the `.pak` did not change, `buildzone` reuses the `.pak`.
- `zonetool_cache/h1_techsets.txt` holds the H1 techset catalog of the S2 to H1 converter.
- Zlib zones of 8 MB or more compress in 4 MB chunks on all threads. The result is one valid zlib stream.
- The revision values are in `cache_revision` in `src/core/io/output_cache.hpp`. If you change the output of a cached asset, increment its revision.

## Source layout

```
src/common/            utils: io, string, compression, crypto, flags
src/core/xfile/        container: header, stream file table, signed blocks, zlib, LZ4
src/core/zone/         zone_reader, zone_buffer, asset database, script strings
src/core/formats/      csv, iwi, imagefile, mapents, ddl
src/core/assets/       asset templates that more than one game uses
src/core/io/           filesystem, assetmanager (binary dump format)
src/games/<game>/      game.cpp, structs.hpp, <game>.hpp, assets/, zone/, dump.cpp, build.cpp
src/convert/<a>_<b>/   converter from game <a> to game <b>
src/cli/               main, commands, settings, game registry, crash handler
deps/                  submodules; deps/premake/*.lua defines each dependency project
tools/                 build.bat, premake5.exe, asm_slice.py
```

Per game:

- `<game>.hpp` is the game header. It holds the `<GAME>_ASSETS(x)` x-macro (type, asset class, struct), includes every asset header, and declares the database lookups.
- `game.cpp` returns the `game` definition: name, container format, and the dump, verify and build entry points.
- `zone/loader.cpp` builds the reader table from `<GAME>_ASSETS`. `dump.cpp` builds the dump switch from the same macro.
- `zone/zone.cpp` holds the `ADD_ASSET` list that `buildzone` uses.

Dependency rules:

- No include from one game into another inside `src/games/`. Code that knows two games goes in `src/convert/<a>_<b>/`. Code for all games goes in `src/core/`.
- A dependency that only one converter uses belongs to that converter. Example: the h1 gsc context lives in `convert/h1_iw7/assets/clipmap.cpp`.

## Asset anatomy

Each asset type is one class in `src/games/<game>/assets/<type>.hpp` and `.cpp`, derived from `asset_interface`:

| Member | Job |
|---|---|
| `static read(zone_reader&, T*)` | Reads the asset from a zone. Mirrors the game `Load_*` function |
| `write(zone_base*, zone_buffer*)` | Writes the asset into a zone. Mirrors `read()` |
| `prepare(zone_buffer*, zone_memory*)` | Registers script strings before any asset data is written |
| `static dump(T*)` | Writes the asset to disk in the v1 format |
| `parse(name, zone_memory*)` | Reads the dump from disk |
| `init(name, zone_memory*)` | Calls `parse()`, or makes a name stub for a referenced asset (name starts with `,`) |
| `load_depending(zone_base*)` | Adds the assets that this asset needs to the build |

Simple types use `REGISTER_TEMPLATED_ASSET(class, Struct, ASSET_TYPE_X)` with a template from `src/core/assets/`.

To add an asset type:

1. Add the class files in `assets/` and include the header in `<game>.hpp`.
2. Add one row to `<GAME>_ASSETS`. A type that has only `read()` goes in `<GAME>_READ_ONLY_ASSETS`.
3. Add the `ADD_ASSET` row in `zone/zone.cpp` if `buildzone` must write it.
4. Run `verifyzone --all` and a round trip.

## Zone format rules

These rules are the core of the project. Most bugs are a break of one of them.

**`read()` and `write()` must match step for step.** Same field order, same `align`, same null checks and conditions, same counts, same `push_stream` and `pop_stream`. Both must match the game `Load_*` code. If you change one, change the other in the same edit. When the two disagree, the asm decides. Prove each layout change with the asm and a short pseudocode.

The one accepted asymmetry: `reader.read_script_string*` reads no stream bytes. It converts a zone-local id to a global id. It pairs with `prepare()`, which calls `buf->write_scriptstring`. Keep `prepare()` for script strings.

Writer and reader calls pair up like this:

| `zone_buffer` (write) | `zone_reader` (read) |
|---|---|
| `align(n)` + `write(data, count)` + `clear_pointer` | `read_array(field, n, count)` |
| `write_str` | `read_string(field)` |
| `write_stream` | `read_stream(field, n, size, count)` |
| asset pointer | `read_asset(type, field)`, `read_asset_array` |
| `align(7)` + `write(&data_following)` + `write_str(name)` | `read_name_reference(field)` |
| `prepare()` + `write_scriptstring` | `read_script_string`, `read_script_strings` |
| `find_sub_buffer` offset | `read_aliased_array` (game uses `DB_ConvertOffsetToAlias`) |

Stream facts:

- `align(n)` takes a mask. `align(3)` is 4-byte alignment, `align(7)` is 8-byte alignment. Alignment moves the block position only and writes no file bytes.
- H1 blocks: TEMP 0, PHYSICAL 1, RUNTIME 2, VIRTUAL 3, LARGE 4, CALLBACK 5, SCRIPT 6.
- IW7 blocks: TEMP 0, TEMP_PRELOAD 1 (aligns to 16 on pop), RUNTIME 6, VIRTUAL 8. IW7 has 10 blocks.
- RUNTIME data holds no file bytes. The game zero-fills it.
- Data read in TEMP is lost after `pop_stream`. Copy it with `duplicate` if you keep it.
- H1 pointers: `0xFDFDFDFFFFFFFFFF` is inline, `...FE` is inline with an insert slot. Other values are offsets: block `(v >> 32) & 0xF`, offset `(u32)v - 1`.
- IW7 pointers: -1 is shared (treat as inline), -2 is inline, -3 is insert. Other values are offsets: block is the high dword.
- IW8 blocks: TEMP 1 (rewinds on pop), TEMP_POSTLOAD 2 (aligns to 32 on pop), RUNTIME 4, VIRTUAL 5. IW8 has 11 blocks. The asset list header stays at the start of block 1.
- IW8 pointers: -1 is shared, -2 is inline, -3 is insert. Other values are offsets: block `(v >> 32) & 0xF`, offset `(u32)v - 1`. With bit 36 set, the offset counts from the start of the outermost asset. Stock zones use these relative offsets for the data of the asset that loads.
- IW8 zones with a `.fp` file beside them are patched: the container reader applies the VCDIFF delta before the asset data loads.
- The game reads most arrays inline when the pointer is not null, and ignores the value. Third-party zones store raw heap pointers there. `zone_reader` follows only a valid offset.
- Stock zones point some fields into arrays that are already written. The writers reuse these with `insert_sub_buffer` and `find_sub_buffer`, so built zones keep the stock size.

## IW8

IW8 asset code is generated, not written by hand. Do not edit the generated files.

- `tools/iw8_format.json` describes what each Replay 1.20 `Load_*` routine reads, in member terms. It comes from the game exe.
- `tools/gen_iw8_structs.py` writes `src/games/iw8/structs.hpp`. `tools/iw8_layouts.txt` corrects the Replay layouts that differ from the schema.
- `tools/gen_iw8_streams.py` writes `src/games/iw8/zone/asset_streams.{hpp,cpp}`: one traversal per record, the `IW8_ASSETS(x)` list and the asset table.
- A traversal is a template over a stream (`zone/streams.hpp`). `load_stream`, `write_stream`, `dump_stream` and `parse_stream` run the same steps, so `read()` and `write()` cannot disagree.
- `dumpzone` writes `<pool>/<name>.iw8` for each asset and `<zone>.csv`. `buildzone` reads `zone_source/<zone>.csv` and the dumps, and writes `output/<zone>.ff`.
- Raw files, script files (`<name>.gscbin`), string tables, localized strings, Lua files and fonts use the files of the other games (`src/core/assets`), so their dumps build into IW8 zones. A dump writes the `.iw8` file only when the shared file does not hold the asset (a string table cell with `"`, or a last column that is empty in each row). A build reads the shared file first.
- `localize,<file>` adds the entries of `localizedstrings/<file>.str` or `.json`, as in the other games. IW8 localized string names are lowercase.
- The IW8 build takes the CSV commands of the other games: `require`, `include`, `ignore`, `reference`, `iterate`, `addpath` and `addpaths`. An asset without a dump comes from the `require` zones, with the assets that it points at. An asset that is not in a dump or a `require` zone becomes a reference, with a warning.
- The writer reserves the block 2 header of each asset that the map preload (`Preload_*Ptr`) loads there. The asset table holds that block per type.
- Every stock convex shape keeps a multiple of four vertices (Havok reads them in groups of four): the last vertex repeats
  with its index, and so does its vertex edge (common_mp 651 and mp_hideout 7327 of 7327 shapes). The builder pads the
  same way, and the reader leaves the repeats out.
- `havok/convex` builds convex hulls with exact orientation tests on an integer grid; `havok/objects` makes the Havok objects of a shape (`hknpConvexPolytopeShape` and its connectivity, `hknpCompoundShape` with its SIMD tree, shape mass properties) from compendium types; `havok/mesh` reads and writes `hknpCompressedMeshShape` (static mesh tree with packed and shared vertices, flat convex quads, convex shapes, SIMD tree); `havok/physics` makes `HavokPhysicsShapeList` (col_map, map_ents) and one-body `HavokPhysicsAsset` objects.
- `havok/tagfile` reads and writes Havok 2018.2 tagfiles (TAG0) without the game: compact files that name the Replay global type compendium (code_post_gfx physics library `physicsglobaltypecompendium`), and self-describing files with a TYPE section from that compendium.

## Compatibility rules

- Keep working v1 logic. x64-zt asset code is studied and tested since 2021. If it works, do not change it.
- Keep the v1 dump formats. A dump from v1 must parse in v2. Change a format only when it is necessary, and tell the user.
- v2 has no game runtime. A database lookup can return null where v1 got a game default asset. Add a null check, not a fake default.
- Known v2 differences from v1: `dumpzone <zone> --target <game>` replaces `dumpzone <target> <zone>`, IW7 `proceduralbones` has a dump format, built zones share arrays the way stock zones do, and output goes to the `zone/` folder of the game folder.

## Verification

Run these after a change to a reader, writer or the container code:

| Check | Command | Pass condition |
|---|---|---|
| Container | `inspect --all` | Every zone passes. IW8: 9536 of 9545 (the 9 later-build frontend zones have other xfile versions) |
| Reader | `verifyzone --all` | H1: 6991 of 6991. IW6: 878 of 878. IW7: 686 of 687 (`mp_vlobby_room` on disk is a modified file). IW8: 9533 of 9545 (the 12 others are frontend zones that the Replay loader also rejects; in the current Replay folder 9582 of 9595: the 9 later-build frontend zones, `mp_frontend`, `mp_frontend1`, `mp_frontend2` and `dw_gulag_nav_*`, whose navmesh names a script string that does not exist) |
| IW8 writer | `IW8_REWRITE=1 verifyzone --all` | Each zone written again with its own pointer markers gives the same data (block sizes can differ: stock zones add slack) |
| IW8 writer | `IW8_REWRITE=fresh verifyzone --all` | Each zone written with the choices of the writer (as the IW3 converter writes its zones) loads with the same assets |
| IW3 converter zones | convert, then `IW8_REWRITE=compare IW8_COMPARE_ZONE=<zone built with --writer iw8-zonetool> verifyzone <zone>` | The same asset list and script strings, and each asset the same (Nuketown: 3053 list entries, 178 strings, 2518 assets) |
| IW8 Havok | `IW8_REWRITE=havok verifyzone --all` | Each compact Havok tagfile writes again to the same bytes, and each self-describing one reads back with the same items (134015 and 1694 in the stock zones) |
| IW8 physics FX shapes | `IW8_REWRITE=fxshape verifyzone common_mp` (and `common_base_mp`, `common_core_mp`) | Each `PhysicsFXShape` (an `hkTuple` of 17 convex shapes at the scales 1, 1/1.33 ... 1/1.33^8, 1.33 ... 1.33^8, with mass properties of density 1000, and their scales) builds again without the game from its stock levels to the same tagfile, except what the builder estimates, which it counts: `maxAllowedPenetration`, `minHalfAngle`, the edge that `vertexEdges` names for each vertex, the packed mass properties; and from the vertices of its first level, levels whose vertices, mass and inertia agree (71 of the 72 stock shapes; 8 with a convex radius and 1 whose vertices keep no indices are left out) |
| IW8 convex shapes | `IW8_REWRITE=convex verifyzone --all` | Each convex polytope shape builds again from its vertices without the game, holds the vertices of the original within 1 mm, and writes and reads back the same (485747 shapes) |
| IW8 compressed meshes | `IW8_REWRITE=mesh verifyzone --all` | Each compressed mesh decodes with trees that hold its primitives, builds again from its triangles and convex shapes without the game, and decodes to the same triangles within its vertex error (77378 meshes, 45.1 million triangles, 898436 convex shapes; 80 meshes with other custom primitives are left out) |
| IW8 native Havok | convert with `--havok native`, then `IW8_REWRITE=compare IW8_COMPARE_ZONE=<zone built with --havok exe> verifyzone <zone>` (build the native zone with `--detail_collision 0`: the executable cannot bake detail LODs) | Every asset is the same, except Havok data that agrees: list entries, tags and object bytes, shapes within the vertex quantization (Nuketown: 2362 same, 156 agree; the native meshes keep 1643 small triangles that the Replay tools leave out). `--havok native-check` also loads the shape lists with the Replay executable and checks their bounds |
| IW8 physics | `IW8_REWRITE=physics verifyzone <zone>` | Each shape list, one-body physics asset and one-body detail collision LOD (`XModelDetailCollision`, a Havok 2018.2 tagfile with the root `HavokPhysicsXModelLOD`) builds again without the game: the same list entries, tags and object bytes, shapes with the same geometry, mass properties within 0.03% (converted Nuketown: 2 lists, 154 assets, 107 detail LODs; the stock zones hold 190 one-body LODs) |
| IW8 compound shapes | `IW8_REWRITE=compound verifyzone --all` | Each compound shape with a SIMD tree and polytope children builds again from its instances without the game: same header, instance bytes and radius, bounds within 0.01 mm, a tree that holds each instance once (18752 shapes; 16543 with the same tree bytes) |
| IW8 dumps | `IW8_REWRITE=dump verifyzone --all` | Each zone dumps, parses, writes, loads and dumps again to the same files. The log names each asset of a shared type that keeps a `.iw8` file (17 string tables) |
| Writer | `dumpzone X`, `buildzone X`, `verifyzone X`, then `dumpzone` the built zone | The two dumps are equal, except pointer values and the known differences |

Good round-trip zones: H1 `mp_shipment`, `mp_crash`. IW6 `mp_battery3`. IW7 `mp_frontend`, `mp_afghan`. IW8 `mp_shipment`, `common_mp`.

## Strict ground rules to follow

These rules apply to agents more than humans, but it is good practice for humans to also do the following if you advice them or do the code:

- Do not write hacky or sloppy shortcut code. Fix the cause, not the symptom.
- Put engine structs in the game `structs.hpp`. Do not declare structs inline in an asset file or a converter file, as that makes it hacky and less ideal to maintain overall in the codebase.
- Keep code readable for humans. x64-zt was a human project before AI, and code that only a machine can follow is not accepted at all. Furthermore, comments do not fix sloppy or weird code. Use named struct fields, not raw offsets and byte casts.
- To add onto comments, they should be inlined and short summaries and not over-yapped to keep explaining something. The code itself explains the logic and what we do with it, so a comment used in the codebase is for something we are unsure about, TODO, or could cause issues later. Even "hacky" code contains comments for the future, but it's there because a human most likely tested it at one point and deemed it necessary to note down for other contributors.
- Trust `re_reference/` as the source of truth. Its `ship_asm.txt` and `.asm` files show what each game does that this project supports. When the code and the asm disagree, the asm decides as assembly will show everything truly happening.
- During the process, you may want to test launch the game, receive crashdumps, or even take screenshots. Focus on only screenshotting a game window itself and nothing else to give away stuff you don't need to know. Most Call of Duty clients have `map` and `devmap` available in order to get you in game to test, and you may need to select a team & class in order to also keep going. And in order to receive crashdumps, look for a `minidumps` folder inside of the game folder(s) that may contain the crash you're looking for. It has a .dmp file usually, and a .info txt file to quickly figure out what's going on. The .dmp file is extremely helpful as to figuring out what went wrong, and the info.txt will also do wonders. If no crashdump is found, your last fallback is to find a process minidump Windows generates in %CRASHDUMPS%.

## Code style

- lowercase snake_case for functions, variables, classes and files. Engine structs keep their engine names (`XModel`, `GfxImage`).
- Tabs, braces on their own line, `this->` for members, `const auto` where the type is clear. Match the file you edit.
- Keep the structure flat. Do not make a file for one declaration. Put a small declaration in the header that already owns the topic. A list macro belongs in the game header, and a value that one file uses belongs in that file.
- No umbrella headers outside `<game>.hpp`. Include what you use.
- Name things so that the code reads without comments. Prefer a small well-named function to a comment over a block.

### Comments

Default: no comment. Code must explain itself through naming and structure. A comment is justified only when it carries information that the code cannot show:

- a non-obvious external constraint, for example game or format behavior verified in the asm;
- a deliberate deviation that a reader would want to "fix" back;
- a hack or workaround that can break later, with the reason;
- a reference that saves a research session.
- multiple lines that are single commented trying to express something
- comments showing above function definitions (put them in the function body - we dont need functions explained as comments unless the code cannot explain itself)
- comments showing above variable definitions (put them to the side as one liners, short and sweet unless you absolutely need to explain more and the definition doesn't explain itself)

Never write restatements, section banners, edit narration or commented-out code. If unsure whether a comment qualifies, it does not. Example of a valid comment: the raw heap pointer note in `zone_reader::read_array`.

### Technical writing

Docs, commit messages and user-facing text use ASD-STE100 style:

- Max 20 words per sentence in instructions, 25 in descriptions.
- Imperative for steps. One instruction per sentence. Condition before command.
- Simple tenses only. No present perfect, no -ing verbs, no should/would/may/might.
- Active voice. One word per meaning, no synonym rotation.
- No contractions. Keep articles and "that".
- Delete filler: simply, robust, seamlessly, leverage.
- Code and identifiers stay exact.

## Working notes for agents

- Commit only when the user asks.
- Never co-author yourself as whatever agent you are, just the person who wants to commit.
- Do implementation work in the main session.
- Put temporary files in a session scratchpad, not in the repository or `/tmp`.
- If Python is not installed on the main dev machine, use shell tools, node, or the edit tools.
- `core.autocrlf` is `true`. Some files are CRLF in the working tree. Keep the line endings of the file you edit.
- To test H1 in the game, copy `zonetool.exe` to the user's MWR 1.15 folder (if you don't know where it is, prompt a question and ask the user so you know - this will help save so much time), or build with `--copy-to`.
- Anything you don't know how to do, but is listed in this file as advice for you, ask the user for questions and try to figure out how to get there if you are lost. This is mainly for non-Claude models that may get stuck easier.
# x64 ZoneTool

Standalone fastfile unlinker and linker for next-generation Call of Duty titles. You can dump `.ff` fastfile zones to disk, build new zones from those dumps, and convert assets between games.

## Supported games

| Game | Title | Notes |
|---|---|---|
| **IW3** | Call of Duty 4: Modern Warfare | Conversion source only. Zones are read through OpenAssetTools' Unlinker (`--unlinker`) |
| **IW6** | Ghosts | |
| **S1** | Advanced Warfare | |
| **H1** | Modern Warfare Remastered | |
| **H2** | Modern Warfare 2 Campaign Remastered | Battle.net CASC |
| **S2** | WWII | Signed zones. The key comes from `s2_zone_key.der` or the game executable |
| **IW7** | Infinite Warfare | |
| **IW8** | Modern Warfare (2019), Replay 1.20 build | `game_dx12_ship_replay.exe` zones. The asset readers and writers are generated from the game's own `Load_*` routines (`tools/iw8_format.json`) |

T7 was in v1 (`main`). It is not ported to v2 yet.

## Asset conversion

Rows are the source game. Columns are the target game.

| From ↓ / To → | IW6 | S1 | H1 | H2 | S2 | IW7 | IW8 |
|---|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| **IW3** | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |
| **IW6** | ✅ | ❌ | ✅ | ❌ | ❌ | ❌ | ❌ |
| **S1**  | ❌ | ✅ | ✅ | ✅ | ⚠️ | ❌ | ❌ |
| **H1**  | ❌ | ❌ | ✅ | ✅ | ⚠️ | ⚠️ | ❌ |
| **H2**  | ❌ | ❌ | ✅ | ✅ | ❌ | ❌ | ❌ |
| **S2**  | ❌ | ❌ | ⚠️ | ❌ | ✅ | ❌ | ❌ |
| **IW7** | ❌ | ❌ | ⚠️ | ❌ | ❌ | ✅ | ⚠️ |
| **IW8** | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |

✅ supported &nbsp; ⚠️ partial or experimental &nbsp; ❌ not supported

- **IW6 → H1** uses IW6 techsets instead of remapping
- **S1 → H2** converts through H1
- **H1 → S1** has been purposely left out for now
- **IW7 → H1** converts images, materials and models only
- **H1 → S2** and **S1 → S2** are experiments done for map assets (world, collision, map ents), models, materials and images. effects, sounds, animations, scriptables and weapons are not converted yet
- **IW3 → IW8** converts whole multiplayer maps into Replay 1.20 zones: the world with its lightmaps, light grid, probes, sky, fog and sun shadows, collision (Havok data built natively), static and entity models, materials and images, local lights, effects, sounds, breakable glass, bot navigation, the minimap and the map scripts (translated to IW8 GSC)
- **IW7 → IW8** converts multiplayer maps the same way: IW7's world, collision, models, materials, lightmaps, probes, sky, lights, glass, effects (module by module), map sounds, map scripts and scriptables (breaking props, animated props) become an extraction that the IW3 → IW8 pipeline builds, with the vision's sky intensity, fog and colour grading. Every IW7 multiplayer map (33) converts and loads through Replay's own readers; in-game testing is in progress

IW7 Scriptable rumble definitions convert through `asset_data/rumbles.json`, with every stored curve slot and all supported flags.
Converted names use `mw120r/iw7/<source-map>/rumble/<source-name>` and `rumblegraph/<source-name>` to prevent collisions with Replay assets.
The separate provenance file records source names, converted names, owner zones and decoded header snapshots.
The native writer adds definitions to the map and their names to level NetConstStrings row 3.
Native Scriptable rumble events remain unconverted. Asset delivery does not prove event playback.

<details>
<summary>v1 legend (<code>main</code>), for comparison</summary>

| From ↓ / To → | IW6 | S1 | H1 | H2 | T7 | IW7 |
|---|:-:|:-:|:-:|:-:|:-:|:-:|
| **IW6** | ✔️ | ❌ | ✔️ | ✔️ | ❌ | ❌ |
| **S1**  | ❌ | ✔️ | ✔️ | ✔️ | ❌ | ❌ |
| **H1**  | ❌ | ⚠️ | ✔️ | ✔️ | ❌ | ⚠️ |
| **H2**  | ❌ | ❌ | ✔️ | ✔️ | ❌ | ❌ |
| **T7**  | ❌ | ❌ | ⚠️ | ❌ | ✔️ | ⚠️ |
| **IW7** | ❌ | ❌ | ❌ | ❌ | ❌ | ✔️ |

</details>

## Usage

```bash
zonetool.exe -game h1 dumpzone mp_shipment
```

Run `zonetool.exe` without a command to open the interactive console.

Converting a map to Replay 1.20 (IW3 or IW7 source):

```bash
zonetool.exe -game iw7 -path "C:/IW7" dumpzone mp_frontier --target iw8 --map mp_iw7_frontier --replay "E:/IW8/game_dx12_ship_replay.exe" --gsc_tool gsc-tool.exe --acts acts.exe
```

The zones go to `output/<map>/`. IW3 maps also need `--unlinker <OpenAssetTools Unlinker.exe>`. `--acts` validates the built zones with Atian tools, and `--metadata <json>` gives the map its id, title and description in the map list.

An IW7 map reads the game folder's `zone` files: the map's zones, the common zones, the `imagefile*.pak` / `assetfile*.pak` streams and the map's sound banks (`<map>.sabl`, `.sabs`). A missing pak leaves its images, or its models' surfaces, out of the map, and a missing bank leaves the map's sounds silent; the log names each one.

| Option | Effect |
|---|---|
| `--map <id>` | The converted map's name (default: the zone's) |
| `--replay <exe>` | Replay's executable; its folder gives the stock zones (default: the iw8 game folder) |
| `--unlinker <exe>` | OpenAssetTools' Unlinker, which reads IW3 zones (IW3 maps only) |
| `--gsc_tool <exe>` | gsc-tool, which decompiles and compiles the map scripts |
| `--acts <exe>` | Validates the built zones with Atian tools (loads them through Replay's own readers) |
| `--metadata <json>` | The map's id, title and description in the map list, and an optional `exposureAdjust` (EV, -8 to 8) that the map's colour grading applies |
| `--stock_cache <dir>` | Keeps the stock assets the maps reference between runs |
| `--style iw3\|iw8` | `iw3` (default) keeps the source's look; `iw8` shows the map in Replay's lighting units with its probes and sun specular (used for IW7 maps) |
| `--havok native\|native-check\|exe` | Builds the collision natively (default), checks it with the executable, or bakes it with the executable |
| `--detail_collision 0\|1` | Static models' detail collision (default 1) |
| `--bc7_images 0\|1` | Block-compresses world and model images (default: IW7 maps do) |
| `--navigation 0` | Leaves out the bot navigation |
| `--search_path <dir>` | Another IW3 folder to read map files from |
| `--dump 1` | Also dumps the built zones (`dump/<zone>/`), which `buildzone` builds again |

| Command | Result |
|---|---|
| `inspect <zone>` / `inspect --all` | Show fastfile header & container data |
| `verifyzone <zone>` / `verifyzone --all` | Reads every asset and checks that the zone is consumed exactly |
| `dumpzone <zone> [--target <game>]` | Writes a fastfile dump to `dump/<zone>/`. `--target` converts assets |
| `dumpasset <type> <name> <zone> [--target <game>]` | Writes one asset of `<zone>` to `dump/assets/`. `--target` converts it |
| `buildzone <zone>` | Reads `zone_source/<zone>.csv` and `zonetool/<zone>/`, then writes `<zone>.ff` to the game `zone/` folder |
| `generatecsv <map> [sp] [--path <dir>]` | Generates a build CSV for a map |

### Flags

| Flag | Effect |
|---|---|
| `-game <name>` | Selects the game |
| `-path <folder>` | Game folder to use |
| `-target_path <folder>` | Sets the folder of the `--target` game |
| `-dds` | Writes images as DDS as well |
| `-dump_streamed_image` | Reads `imagefile*.pak` and writes streamed images |
| `-no_cache` | Ignores the build cache and writes every output again |

If a flag is not set, x64-zt can read `zonetool.json` next to the executable:

```json
{ "game": "h1", "paths": { "h1": "path/to/mwr", "s2": "path/to/wwii" } }
```

## Building

Requires Windows and Visual Studio 2022.

```bash
generate.bat --games=h1,iw7
```

| Option | Effect |
|---|---|
| `--games=<list>` | Games to build: `all` (default), or a list such as `h1,s2` |
| `--no-convert` | Skips the converter projects |
| `--copy-to=<folder>` | Copies `zonetool.exe` to this folder after each build |

The converters for games only builds only when its source and target games are selected. IW7 → IW8 also needs IW3 (it builds through the IW3 → IW8 pipeline): `--games=iw3,iw7,iw8`.

## Guides

x64-zt is a work in progress software that aims to document the modding experience as best as possible. Some things may not be answered well, but checking out the Zonetool Basics will help you get started.

- [Zonetool Basics](https://docs.auroramod.dev/zonetool-basics)
- [Map porting IW3/4/5 → H1](https://docs.auroramod.dev/map-porting-iw5)
- [Map porting S1 ↔ H1 ↔ H2](https://docs.auroramod.dev/map-porting-s1)


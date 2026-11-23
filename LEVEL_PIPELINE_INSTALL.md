# Do not overwrite the project README.md; this is a separate installation guide.

# Super Tiva Bros — shared level-data pipeline

This package replaces duplicate manually maintained level rectangles with a single authored source: `levels/*.json`. The generator emits the existing native Tiva interfaces in `include/level_data.h` and `src/level_data.c`; the Raspberry Pi renderer reads the JSON files at runtime.

## Installation

Extract this ZIP into the **root** of a working `Super-Tiva-Bros` clone (where `src/`, `include/`, and `pi/` are). Review changes with `git diff` before committing. This kit intentionally replaces `src/level_data.c`, `include/level_data.h`, `pi/level.py`, and `pi/main.py`.

It retains the current level ID 0, spawn (100,192), 512x224 dimensions, all four collision rectangles, the existing sprite set, and the current 13-byte SPI protocol. It adds a generic JSON registry; it does not implement automatic level switching yet.

## Generate the Tiva data

Windows PowerShell from repo root:

```powershell
python .\tools\generate_levels.py
python .\tools\generate_levels.py --check
```

Raspberry Pi/Linux from repo root:

```bash
python3 tools/generate_levels.py --check
python3 -m py_compile pi/main.py pi/level.py tools/generate_levels.py
python3 -m unittest discover -s tests -p 'test_*.py' -v
```

The generated C registry is checked in, so the Pi does not need to generate firmware tables. **Every time you edit a level file, run the generator on the development computer before building and flashing the Tiva**. Commit the JSON and generated C together.

## To add another level

Create another file such as `levels/001_world_1_2.json` with a unique numeric `id` (0–255) and uppercase `symbol`, level dimensions, spawn point, background name, and `solids` rectangles. Run the generator to register the ID automatically. Do not manually edit the generated C files.

Levels are currently limited to 32767 pixels per dimension because the SPI snapshot uses signed 16-bit coordinates. The 16x16 `brick` sprite is the only mapped solid visual type in this first iteration; expand `SPRITE_INDEX` in `pi/level.py` for new artwork.

The Pi currently selects level ID 0 in `pi/main.py` and the Tiva starts on `LEVEL_ID_START`. Next milestone: add `level_id` to an incremented-version SPI snapshot and reload Pi level visuals when it changes.

## Behavior and scope

The ground rectangle has `"sprite": null` because the existing background artwork already renders ground. It still participates in Tiva collisions. The generator validates ID uniqueness, coordinates, rectangle containment, and supported sprites, but it cannot prove map playability, spawn clearance, or physical hardware operation.

Do not change the SPI packet size for this milestone. Flashing the Tiva is optional if the generated `src/level_data.c` is bytewise equivalent in geometry to your currently running firmware. The Pi files must be deployed to the Pi along with the `levels/` folder.

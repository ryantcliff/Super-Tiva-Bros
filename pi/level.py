#!/usr/bin/env python3

from .render import RenderObject
import json
from pathlib import Path

class Tile:
	def __init__(self, image_name, x, y):
		self.image_name = image_name
		self.x = x
		self.y = y


class Level:
	def __init__(self):
		self.tiles = []
		self.width = 0
		self.height = 0
		self.background = None

	def add_tile(self, tile):
		self.tiles.append(tile)


TILE_SIZE = 16

LEVELS_DIR = Path(__file__).resolve().parents[1] / "levels"

SPRITE_TYPES = {
	"brick": ("block_sprites", 1),
	"ground": ("block_sprites", 0),
	"question_block": ("block_sprites", 2),
}

def load_level(level_id):
	for path in sorted(LEVELS_DIR.glob("*.json")):
		with path.open("r", encoding="utf-8") as file:
			data = json.load(file)

		if data["id"] == level_id:
			return data

	raise ValueError(f"Level {level_id} not found")


def add_level_tiles(renderer, level_id):
	level = load_level(level_id)
	for solid in level["solids"]:
		sprite_name = solid.get("sprite")

		if sprite_name is None:
			continue

		if sprite_name not in SPRITE_TYPES:
			raise ValueError(f"Unknown sprite type: {sprite_name}")

		sprite_folder, sprite_frame = SPRITE_TYPES[sprite_name]

		x, y, width, height = solid["rect"]
		for tile_y in range(y, y + height, TILE_SIZE):
			for tile_x in range(x, x + width, TILE_SIZE):
				block = RenderObject(
					sprite_folder,
					tile_x,
					tile_y,
					layer=2
				)
				block.set_sprite(sprite_frame)
				renderer.add_object(block)

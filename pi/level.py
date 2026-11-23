#!/usr/bin/env python3

from .render import RenderObject

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

LEVEL_VISUALS = {
	0: [
		(176, 176, 48, 16),
		(320, 160, 64, 16),
		(272, 176, 16, 32),
	]
}

def add_level_tiles(renderer, level_id):
	solids = LEVEL_VISUALS.get(level_id)

	if solids is None:
		raise ValueError(f"Unknown level ID: {level_id}")

	for x, y, width, height in solids:
		for tile_y in range(y, y + height, TILE_SIZE):
			for tile_x in range(x, x + width, TILE_SIZE):
				block = RenderObject(
					"block_sprites",
					tile_x,
					tile_y,
					layer=2
				)
				block.set_sprite(1)
				renderer.add_object(block)

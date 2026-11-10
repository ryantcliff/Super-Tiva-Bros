#!/usr/bin/env python3


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

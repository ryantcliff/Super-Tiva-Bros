#!/usr/bin/env python3



class GameState:
	def __init__(self):
		self.tivo_x = 0
		self.tivo_y = 0
		self.tivo_sprite = 0
		self.camera_x = 0
		self.camera_y = 0
		self.score = 0
		self.time = 0
		self.enemies = []
		self.items = []

	def set_tivo(self, x, y, sprite):
		self.tivo_x = x
		self.tivo_y = y
		self.tivo_sprite = sprite

	def set_camera(self, x, y):
		self.camera_x = x
		self.camera_y = y

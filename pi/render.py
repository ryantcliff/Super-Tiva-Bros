#!/usr/bin/env python3

import pygame
from pathlib import Path

ASSET_DIR = Path(__file__).resolve().parent / "assets" / "sprites"

SCREEN_WIDTH = 320
SCREEN_HEIGHT = 240

class Sprite:
	def __init__(self, image_path):
		self.image = pygame.image.load(image_path).convert_alpha()


class RenderObject:
	def __init__(self, name, x, y, layer=1):
		self.name = name
		self.x = x
		self.y = y
		self.layer = layer
		self.sprites = []
		self.current_sprite = 0

		object_dir = ASSET_DIR / name
		for sprite_file in sorted(object_dir.iterdir()):
			if sprite_file.suffix.lower() == ".png":
				self.sprites.append(
					Sprite(sprite_file)
				)

	def set_position(self, x, y):
		self.x = x
		self.y = y

	def set_sprite(self, sprite_index):
		if 0 <= sprite_index < len(self.sprites):
			self.current_sprite = sprite_index

	def get_sprite(self):
		return self.sprites[self.current_sprite]


class Camera:
	def __init__(self, x=0, y=0):
		self.x = x
		self.y = y

	def set_position(self, x, y):
		self.x = x
		self.y = y

	def world_to_screen(self, world_x, world_y):
		screen_x = world_x - self.x
		screen_y = world_y - self.y

		return screen_x, screen_y


class Render:
	def __init__(self):
		self.screen = None
		self.objects = []
		self.camera = Camera()
		self.clock = None

	def __enter__(self):
		pygame.init()
		self.screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
		pygame.display.set_caption("Super Tiva Bros")
		self.clock = pygame.time.Clock()
		return self

	def __exit__(self):
		pygame.quit()

	def add_object(self, game_object):
		self.objects.append(game_object)

	def draw(self):
		# temp background color
		self.screen.fill((80, 160, 255))
		objects = sorted(
			self.objects,
			key=lambda obj: obj.layer
		)

		for game_object in objects:
			sprite = game_object.get_sprite()
			screen_x, screen_y = (
				self.camera.world_to_screen(
					game_object.x,
					game_object.y
				)
			)
			self.screen.blit(
				sprite.image,
				(screen_x, screen_y)
			)
		pygame.display.flip()

	def tick(self, fps=60):
		self.clock.tick(fps)


if __name__ == "__main__":
	with Render() as renderer:
		tivo = RenderObject(
			"tivo",
			100,
			180,
			layer=2
		)
		renderer.add_object(tivo)
		running = True
		while running:
			for event in pygame.event.get():
				if event.type == pygame.QUIT:
					running = False
			renderer.draw()
			renderer.tick(60)

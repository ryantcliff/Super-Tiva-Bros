#!/usr/bin/env python3

import pygame
from .render import Render, RenderObject
from .tiva_game_state import TivaGameStateReader
from .player_view import select_player_sprite
from .ssh_keyboard_input import SshKeyboardInput
from .level import add_level_tiles


def main() -> int:
	with SshKeyboardInput() as keyboard, TivaGameStateReader() as reader, Render() as renderer:
		world = RenderObject(
			"backgrounds",
			0,
			0,
			layer=1
		)
		renderer.add_object(world)

		add_level_tiles(renderer, 0)

		tivo = RenderObject(
            		"tivo_sprites",
            		0,
            		0,
            		layer=3
        	)
		tivo.set_sprite(0)
		renderer.add_object(tivo)

		running = True
		try:
			while running and keyboard.running:
				for event in pygame.event.get():
					if event.type == pygame.QUIT:
						running = False
					elif (
						event.type == pygame.KEYDOWN
						and event.key in (pygame.K_ESCAPE, pygame.K_q)
					):
						running = False

				if not running:
					break

				buttons = keyboard.poll()
				if not keyboard.running:
					break
				snapshot = reader.poll(buttons)

				tivo.set_position(
					snapshot.player_x,
					snapshot.player_y,
				)
				tivo.set_sprite(select_player_sprite(snapshot))
				renderer.camera.set_position(
					snapshot.camera_x,
					snapshot.camera_y
				)
				renderer.draw()
				renderer.tick(60)

		except KeyboardInterrupt:
			pass

	return 0


if __name__ == "__main__":
    raise SystemExit(main())

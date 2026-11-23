#!/usr/bin/env python3

import pygame
from .render import Render, RenderObject
from .tiva_game_state import TivaGameStateReader
from .player_view import select_player_sprite
from .ssh_keyboard_input import SshKeyboardInput
from .level import load_level, add_level_tiles


def build_level_scene(renderer, level_id, tivo):
	level = load_level(level_id)

	background = RenderObject(
		level["background"],
		0,
		0,
		layer=1,
	)
	background.set_sprite(level.get("background_frame", 0))

	renderer.objects.clear()

	renderer.add_object(background)
	add_level_tiles(renderer, level)

	goal = level.get("goal")

	if goal is not None and goal["type"] == "flagpole":
		gx, gy, _, _ = goal["rect"]

		flagpole = RenderObject(
			"interactable_sprites",
			gx,
			gy,
			layer=2,
		)
		flagpole.set_sprite(3)
		renderer.add_object(flagpole)

	renderer.add_object(tivo)


def main() -> int:
	with (
		SshKeyboardInput() as keyboard,
		TivaGameStateReader() as reader,
		Render() as renderer
	):
		tivo = RenderObject(
            		"tivo_sprites",
            		0,
            		0,
            		layer=3
        	)
		tivo.set_sprite(0)

		active_level_id = None
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

				if snapshot.level_id != active_level_id:
					build_level_scene(
						renderer,
						snapshot.level_id,
						tivo,
					)
					active_level_id = snapshot.level_id
					print(f"Loaded level {active_level_id}")

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

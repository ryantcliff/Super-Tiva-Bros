#!/usr/bin/env python3

import pygame
import json
from pathlib import Path
from .render import Render, RenderObject
from .tiva_game_state import TivaGameStateReader
from .player_view import select_player_sprite
from .ssh_keyboard_input import SshKeyboardInput
from .level import load_level, add_level_tiles

ITEM_TYPES = json.loads(
    (Path(__file__).resolve().parents[1] / "config" / "item_types.json")
    .read_text(encoding="utf-8")
)["types"]


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

	item_objects = []
	for item in level.get("items", []):
		x, y, _, _ = item["rect"]
		sprite_info = ITEM_TYPES[item["type"]]["sprite"]
		item_object = RenderObject(sprite_info["folder"], x, y, layer=2)
		item_object.set_sprite(sprite_info["frame"])
		renderer.add_object(item_object)
		item_objects.append(item_object)

	renderer.add_object(tivo)
	return item_objects


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
		item_objects = []
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
					item_objects = build_level_scene(
						renderer,
						snapshot.level_id,
						tivo,
					)
					active_level_id = snapshot.level_id
					print(f"Loaded level {active_level_id}")

				for i, item in enumerate(item_objects):
					item.visible = not bool(
						snapshot.collected_items[i // 8] & (1 << (i % 8))
					)

				tivo.set_position(
					snapshot.player_x,
					snapshot.player_y,
				)
				tivo.set_sprite(select_player_sprite(snapshot))
				renderer.camera.set_position(
					snapshot.camera_x,
					snapshot.camera_y
				)
				renderer.draw(snapshot.led_count)
				renderer.tick(60)

		except KeyboardInterrupt:
			pass

	return 0


if __name__ == "__main__":
    raise SystemExit(main())

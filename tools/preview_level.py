#!/usr/bin/env python3
"""Preview a level on your laptop without the Tiva.

Draws the background and the level's solids using the real Pi renderer
(pi/render.py + pi/level.py), so what you see here is what the Pi draws.

Run from the repo root:
    python3 -m tools.preview_level        # level 0
    python3 -m tools.preview_level 1      # level 1

Controls (click the game window first):
    Left / Right arrows   scroll the camera
    Q or Escape           quit
"""

import sys

import pygame

from pi.render import Render, RenderObject, SCREEN_WIDTH
from pi.level import add_level_tiles, load_level

SCROLL_SPEED = 4


def main() -> int:
    level_id = int(sys.argv[1]) if len(sys.argv) > 1 else 0
    level = load_level(level_id)
    level_width = level["width"]
    max_camera_x = max(0, level_width - SCREEN_WIDTH)

    with Render() as renderer:
        pygame.display.set_caption(f"Level {level_id} preview")
        renderer.add_object(RenderObject("backgrounds", 0, 0, layer=1))
        add_level_tiles(renderer, level)

        camera_x = 0
        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.KEYDOWN and event.key in (pygame.K_q, pygame.K_ESCAPE):
                    running = False

            keys = pygame.key.get_pressed()
            if keys[pygame.K_LEFT]:
                camera_x -= SCROLL_SPEED
            if keys[pygame.K_RIGHT]:
                camera_x += SCROLL_SPEED
            camera_x = max(0, min(camera_x, max_camera_x))

            renderer.camera.set_position(camera_x, 0)
            renderer.draw()
            renderer.tick(60)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

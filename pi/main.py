#!/usr/bin/env python3

import pygame
from .render import Render, RenderObject
from .tiva_game_state import TivaGameStateReader
from .player_view import select_player_sprite


def main() -> int:
    with TivaGameStateReader() as reader, Render() as renderer:
        world = RenderObject(
            "backgrounds",
            0,
            0,
            layer=1
        )
        renderer.add_object(world)

        tivo = RenderObject(
            "tivo_sprites",
            0,
            0,
            layer=2
        )
        tivo.set_sprite(0)
        renderer.add_object(tivo)

        running = True

        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False

            snapshot = reader.poll()

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

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
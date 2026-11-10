#!/usr/bin/env python3

import time
import pygame
from pathlib import Path

from ssh_keyboard_input import (
    SshKeyboardInput,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    POLL_SECONDS
)

ASSET_DIR = Path(__file__).resolve().parent / "assets" / "sprites"

SCREEN_WIDTH = 320
SCREEN_HEIGHT = 240
MOVE_SPEED = 2

def main() -> int:
    pygame.init()

    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT), pygame.SCALED | pygame.RESIZABLE)
    player_frames = [
        pygame.image.load(
            str(ASSET_DIR / "player_tiva_idle.png")
        ).convert_alpha(),
        pygame.image.load(
            str(ASSET_DIR / "player_tiva_walk_1.png")
        ).convert_alpha(),
        pygame.image.load(
            str(ASSET_DIR / "player_tiva_walk_2.png")
        )
    ]
    player_sprite = player_frames[0]
    player_width, player_height = player_sprite.get_size()
    pygame.display.set_caption("Super Tiva Bros Graphics Test")

    pygame.mouse.set_visible(False)

    print(
        f"Driver: {pygame.display.get_driver()}, "
        f"surface: {screen.get_size()}",
        flush=True,
    )

    clock = pygame.time.Clock()
    player_x = 152
    player_y = 112

    try:
        with SshKeyboardInput() as keyboard:
            while keyboard.running:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        return 0

                buttons = keyboard.poll()

                if buttons & UP:
                    player_y -= MOVE_SPEED

                if buttons & DOWN:
                    player_y += MOVE_SPEED

                if buttons & LEFT:
                    player_x -= MOVE_SPEED

                if buttons & RIGHT:
                    player_x += MOVE_SPEED

                player_x = max(
                    0,
                    min(player_x, SCREEN_WIDTH - player_width),
                )
                player_y = max(
                    0,
                    min(player_y, SCREEN_HEIGHT - player_height),
                )

                screen.fill((80, 160, 255))

                screen.blit(
                    player_sprite,
                    (player_x, player_y)
                )

                pygame.display.flip()
                clock.tick(60)
    except KeyboardInterrupt:
        return 0
    finally:
        pygame.quit()

    return 0

if __name__ == "__main__":
    raise SystemExit(main())
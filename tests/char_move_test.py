#!/usr/bin/env python3
import sys, time
from ssh_keyboard_input import(
    SshKeyboardInput,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    POLL_SECONDS
)

SCREEN_WIDTH = 20
SCREEN_HEIGHT = 10


class Player:
    def __init__(self):
        self.x = 10
        self.y = 5


def draw_screen(player: Player):
    rows = []

    for y in range(SCREEN_HEIGHT):
        row = ""

        for x in range(SCREEN_WIDTH):
            if x == player.x and y == player.y:
                row += "@"
            else:
                row += "."

        rows.append(row)

    sys.stdout.write("\x1b[2J\x1b[H")
    sys.stdout.write("\r\n".join(rows))
    sys.stdout.flush()


def main() -> int:
    player = Player()
    with SshKeyboardInput() as keyboard:
        sys.stdout.write("Use arrows to move\r\n")
        previous = -1
        while keyboard.running:
            value = keyboard.poll()
            if value != previous:
                if value & UP:
                    player.y -= 1

                if value & DOWN:
                    player.y += 1

                if value & LEFT:
                    player.x -= 1

                if value & RIGHT:
                    player.x += 1

                player.x = max(0, min(player.x, SCREEN_WIDTH - 1))
                player.y = max(0, min(player.y, SCREEN_HEIGHT - 1))
                
                previous = value
                draw_screen(player)
            time.sleep(POLL_SECONDS)

    return 0

if __name__ == "__main__":
    raise SystemExit(main())
#!/usr/bin/env python3
"""Produce a controller byte from keyboard input received through SSH."""

from __future__ import annotations

import os
import select
import sys
import termios
import time
import tty

try:
    from .controller_protocol import (
        A,
        B,
        DOWN,
        LEFT,
        NEUTRAL,
        RIGHT,
        SELECT,
        START,
        UP,
    )
except ImportError:
    from controller_protocol import (
        A,
        B,
        DOWN,
        LEFT,
        NEUTRAL,
        RIGHT,
        SELECT,
        START,
        UP,
    )


HOLD_SECONDS = 0.20
ESCAPE_TIMEOUT_SECONDS = 0.03
POLL_SECONDS = 0.01

KEY_MASKS = {
    b"z": A,
    b"Z": A,
    b"x": B,
    b"X": B,
    b" ": SELECT,
    b"\r": START,
    b"\n": START,
    b"\x1b[A": UP,
    b"\x1b[B": DOWN,
    b"\x1b[D": LEFT,
    b"\x1b[C": RIGHT,
}


def key_to_mask(key: bytes) -> int:
    """Return the controller mask for one terminal key sequence."""
    return KEY_MASKS.get(key, NEUTRAL)


class KeyboardState:
    """Track controller bits using repeat events and short expirations."""

    def __init__(self) -> None:
        self._expires: dict[int, float] = {}

    def press(self, mask: int, now: float) -> None:
        for bit in (1 << index for index in range(8)):
            if mask & bit:
                self._expires[bit] = now + HOLD_SECONDS

    def value(self, now: float) -> int:
        expired = [bit for bit, expiry in self._expires.items() if expiry <= now]
        for bit in expired:
            del self._expires[bit]

        value = 0
        for bit in self._expires:
            value |= bit
        return value


def _read_key(fd: int) -> bytes:
    key = os.read(fd, 1)
    if key != b"\x1b":
        return key

    deadline = time.monotonic() + ESCAPE_TIMEOUT_SECONDS
    while len(key) < 3:
        remaining = deadline - time.monotonic()
        if remaining <= 0:
            break
        readable, _, _ = select.select([fd], [], [], remaining)
        if not readable:
            break
        key += os.read(fd, 1)
    return key


def _print_state(value: int) -> None:
    sys.stdout.write(f"Controller: 0x{value:02X}\r\n")
    sys.stdout.flush()


class SshKeyboardInput:
    def __init__(self):
        self.state = KeyboardState()
        self.running = True
        self.fd = None
        self.original_settings = None

    def __enter__(self):
        self.fd = sys.stdin.fileno()
        self.original_settings = termios.tcgetattr(self.fd)
        tty.setraw(self.fd)
        return self

    def __exit__(self, exception_type, exception, traceback):
        termios.tcsetattr(self.fd, termios.TCSADRAIN, self.original_settings)
        sys.stdout.write("\r\n")
        sys.stdout.flush()

    def poll(self):
        readable, _, _ = select.select([self.fd], [], [], 0)
        if readable:
            key = _read_key(self.fd)
            if key in (b"q", b"Q", b"\x03"):
                self.running = False
                return NEUTRAL
            mask = key_to_mask(key)
            if mask:
                self.state.press(mask, time.monotonic())

        return self.state.value(time.monotonic())


def main() -> int:
    if not sys.stdin.isatty():
        print("Run this program from an interactive SSH terminal.", file=sys.stderr)
        return 2

    previous = -1

    with SshKeyboardInput() as keyboard:
        sys.stdout.write(
            "SSH keyboard ready: arrows=move, Z=A, X=B, Space=Select, "
            "Enter=Start, Q=quit\r\n"
        )
        sys.stdout.flush()

        while keyboard.running:
            value = keyboard.poll()
            if value != previous:
                _print_state(value)
                previous = value
            time.sleep(POLL_SECONDS)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

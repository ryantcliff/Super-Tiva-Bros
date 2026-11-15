#!/usr/bin/env python3

import importlib.util
import unittest

from pi.game_protocol import GameSnapshot

class FakeSpiDevice:
    def __init__(self, response):
        self.response = response
        self.transfers = []

    def open(self, bus, device):
        pass

    def xfer2(self, values):
        self.transfers.append(values)
        return self.response

    def close(self):
        pass


class TivaGameStateTests(unittest.TestCase):
    def reader_class(self):
        module_spec = importlib.util.find_spec("pi.tiva_game_state")
        self.assertIsNotNone(
            module_spec,
            "pi.tiva_game_state must exist",
        )
        from pi.tiva_game_state import TivaGameStateReader

        return TivaGameStateReader

    def test_poll_requests_and_decodes_complete_packet(self):
        spi = FakeSpiDevice([
            0xA5, 0x01, 0x07,
            0x64, 0x00,
            0x96, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x01,
        ])

        reader_class = self.reader_class()

        with reader_class(spi_factory=lambda: spi) as reader:
            snapshot = reader.poll()

        self.assertEqual(spi.transfers, [[0x00] * 13])
        self.assertEqual(
            snapshot,
            GameSnapshot(
                sequence=7,
                player_x=100,
                player_y=150,
                camera_x=0,
                camera_y=0,
                player_state=0,
                facing_direction=1,
            ),
        )


if __name__ == "__main__":
    unittest.main()
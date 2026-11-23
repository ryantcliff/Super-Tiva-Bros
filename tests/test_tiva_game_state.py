#!/usr/bin/env python3

import importlib.util
import unittest

from pi.game_protocol import GameSnapshot, PACKET_STRUCT, ITEM_BYTES

class FakeSpiDevice:
    def __init__(self, response):
        self.response = response
        self.transfers = []
        self.closed = False

    def open(self, bus, device):
        pass

    def xfer2(self, values):
        self.transfers.append(values)
        return self.response

    def close(self):
        self.closed = True


class NeutralTransferFailingSpiDevice(FakeSpiDevice):
    def xfer2(self, values):
        self.transfers.append(values)
        if len(self.transfers) == 2:
            raise OSError("neutral transfer failed")

        return self.response


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
            0xA5, 0x03, 0x07,
            0x64, 0x00,
            0x96, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x01,
            0x00, 0x05
        ] + [0x00] * (ITEM_BYTES - 1))

        reader_class = self.reader_class()

        with reader_class(spi_factory=lambda: spi) as reader:
            snapshot = reader.poll()

        self.assertEqual(spi.transfers[0], [0x00] * PACKET_STRUCT.size)
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
                level_id=0,
                collected_items=bytes([0x05]) + bytes(ITEM_BYTES - 1),
            ),
        )

    def test_poll_repeats_button_mask_across_transfer(self):
        spi = FakeSpiDevice([
            0xA5, 0x03, 0x07,
            0x64, 0x00,
            0x96, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x01,
            0x00
        ] + [0x00] * ITEM_BYTES)
        reader_class = self.reader_class()
        with reader_class(spi_factory=lambda: spi) as reader:
            reader.poll(0x40)

        self.assertEqual(spi.transfers[0], [0x40] * PACKET_STRUCT.size)

    def test_poll_masks_buttons_to_one_byte(self):
        spi = FakeSpiDevice([
            0xA5, 0x03, 0x07,
            0x64, 0x00,
            0x96, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x01,
            0x00
        ] + [0x00] * ITEM_BYTES)
        reader_class = self.reader_class()
        with reader_class(spi_factory=lambda: spi) as reader:
            reader.poll(0x140)
        self.assertEqual(
            spi.transfers[0],
            [0x40] * PACKET_STRUCT.size,
        )

    def test_exit_sends_neutral_before_closing(self):
        spi = FakeSpiDevice([
            0xA5, 0x03, 0x07,
            0x64, 0x00,
            0x96, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x01,
            0x00
        ] + [0x00] * ITEM_BYTES)
        reader_class = self.reader_class()
        with reader_class(spi_factory=lambda: spi) as reader:
            reader.poll(0x40)
        self.assertEqual(
            spi.transfers,
            [
                [0x40] * PACKET_STRUCT.size,
                [0x00] * PACKET_STRUCT.size,
            ],
        )
        self.assertTrue(spi.closed)

    def test_exit_still_closes_when_neutral_transfer_fails(self):
        spi = NeutralTransferFailingSpiDevice([
            0xA5, 0x03, 0x07,
            0x64, 0x00,
            0x96, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x01,
            0x00
        ] + [0x00] * ITEM_BYTES)
        reader_class = self.reader_class()
        with reader_class(spi_factory=lambda: spi) as reader:
            reader.poll(0x40)
        self.assertEqual(spi.transfers[1], [0x00] * PACKET_STRUCT.size)
        self.assertTrue(spi.closed)

if __name__ == "__main__":
    unittest.main()
#!/usr/bin/env python3

import unittest
from pi.game_protocol import decode_packet, PACKET_STRUCT, ITEM_BYTES

class GameProtocolTests(unittest.TestCase):
    def test_decodes_complete_snapshot(self):
        packet = bytes([
            0xA5, 0x04, 0x07,
            0x34, 0x12,
            0xFE, 0xFF,
            0x00, 0x00,
            0x00, 0x01,
            0x01, 0x01,
            0x07, 0x05
        ]) + bytes(ITEM_BYTES - 1) + bytes([0x2C, 0x01])
        snapshot = decode_packet(packet)

        self.assertEqual(snapshot.sequence, 7)
        self.assertEqual(snapshot.player_x, 0x1234)
        self.assertEqual(snapshot.player_y, -2)
        self.assertEqual(snapshot.camera_x, 0)
        self.assertEqual(snapshot.camera_y, 256)
        self.assertEqual(snapshot.player_state, 1)
        self.assertEqual(snapshot.facing_direction, 1)
        self.assertEqual(snapshot.level_id, 0x07)
        self.assertEqual(snapshot.collected_items, bytes([0x05]) + bytes(ITEM_BYTES - 1))
        self.assertEqual(snapshot.led_count, 300)

    def test_rejects_incorrect_packet_length(self):
        short_packet = bytes(12)
        with self.assertRaisesRegex(ValueError, f"{PACKET_STRUCT.size} bytes"):
            decode_packet(short_packet)

    def test_rejects_incorrect_magic(self):
        packet = bytes([
            0x00, 0x04, 0x07,
            0x34, 0x12,
            0xFE, 0xFF,
            0x00, 0x00,
            0x00, 0x01,
            0x01, 0x01,
            0x07
        ]) + bytes(ITEM_BYTES) + bytes(2)
        with self.assertRaisesRegex(ValueError, "magic"):
            decode_packet(packet)

    def test_rejects_unsupported_version(self):
        packet = bytes([
            0xA5, 0x02, 0x07,
            0x34, 0x12,
            0xFE, 0xFF,
            0x00, 0x00,
            0x00, 0x01,
            0x01, 0x01,
            0x07
        ]) + bytes(ITEM_BYTES) + bytes(2)
        with self.assertRaisesRegex(ValueError, "version"):
            decode_packet(packet)


if __name__ == "__main__":
    unittest.main()
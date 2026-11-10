import importlib.util
import re
import unittest
from pathlib import Path


class ControllerProtocolTests(unittest.TestCase):
    def test_pi_protocol_exports_controller_byte_values(self):
        module_spec = importlib.util.find_spec("pi.controller_protocol")
        self.assertIsNotNone(module_spec, "pi.controller_protocol must exist")

        from pi import controller_protocol

        self.assertEqual(
            {
                "A": controller_protocol.A,
                "B": controller_protocol.B,
                "SELECT": controller_protocol.SELECT,
                "START": controller_protocol.START,
                "UP": controller_protocol.UP,
                "DOWN": controller_protocol.DOWN,
                "LEFT": controller_protocol.LEFT,
                "RIGHT": controller_protocol.RIGHT,
                "NEUTRAL": controller_protocol.NEUTRAL,
            },
            {
                "A": 0x01,
                "B": 0x02,
                "SELECT": 0x04,
                "START": 0x08,
                "UP": 0x10,
                "DOWN": 0x20,
                "LEFT": 0x40,
                "RIGHT": 0x80,
                "NEUTRAL": 0x00,
            },
        )

    def test_tiva_button_bits_match_pi_keyboard_protocol(self):
        header = Path("include/input.h").read_text(encoding="utf-8")
        definitions = {
            name: int(bit)
            for name, bit in re.findall(
                r"#define\s+INPUT_BUTTON_(\w+)\s+\(1u\s*<<\s*(\d+)\)",
                header,
            )
        }

        self.assertEqual(
            definitions,
            {
                "A": 0,
                "B": 1,
                "SELECT": 2,
                "START": 3,
                "UP": 4,
                "DOWN": 5,
                "LEFT": 6,
                "RIGHT": 7,
            },
        )


if __name__ == "__main__":
    unittest.main()

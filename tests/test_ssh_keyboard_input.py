import unittest

from pi.ssh_keyboard_input import KeyboardState, key_to_mask


class KeyMappingTests(unittest.TestCase):
    def test_action_keys(self):
        self.assertEqual(key_to_mask(b"z"), 0x01)
        self.assertEqual(key_to_mask(b"x"), 0x02)
        self.assertEqual(key_to_mask(b" "), 0x04)
        self.assertEqual(key_to_mask(b"\r"), 0x08)
        self.assertEqual(key_to_mask(b"\n"), 0x08)

    def test_arrow_keys(self):
        self.assertEqual(key_to_mask(b"\x1b[A"), 0x10)
        self.assertEqual(key_to_mask(b"\x1b[B"), 0x20)
        self.assertEqual(key_to_mask(b"\x1b[D"), 0x40)
        self.assertEqual(key_to_mask(b"\x1b[C"), 0x80)

    def test_unknown_key_has_no_mask(self):
        self.assertEqual(key_to_mask(b"?"), 0x00)


class KeyboardStateTests(unittest.TestCase):
    def test_unexpired_keys_are_combined(self):
        state = KeyboardState()

        state.press(0x01, now=1.00)
        state.press(0x80, now=1.05)

        self.assertEqual(state.value(now=1.10), 0x81)

    def test_keys_expire_independently_after_200_ms(self):
        state = KeyboardState()

        state.press(0x01, now=1.00)
        state.press(0x80, now=1.10)

        self.assertEqual(state.value(now=1.20), 0x80)
        self.assertEqual(state.value(now=1.30), 0x00)

    def test_repeat_extends_key_expiration(self):
        state = KeyboardState()

        state.press(0x01, now=1.00)
        state.press(0x01, now=1.15)

        self.assertEqual(state.value(now=1.25), 0x01)
        self.assertEqual(state.value(now=1.35), 0x00)


if __name__ == "__main__":
    unittest.main()

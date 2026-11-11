import contextlib
import importlib
import io
import sys
import types
import unittest

from pi import ssh_keyboard_input


class PygameInputSelectionTests(unittest.TestCase):
    def parse_args(self, arguments):
        sys.modules.setdefault("pygame", types.ModuleType("pygame"))
        sys.modules.setdefault("ssh_keyboard_input", ssh_keyboard_input)
        module = importlib.import_module("pi.pygame_move_test")

        self.assertTrue(
            hasattr(module, "parse_args"),
            "pygame_move_test.parse_args must exist",
        )
        return module.parse_args(arguments)

    def test_keyboard_is_the_default_input(self):
        arguments = self.parse_args([])

        self.assertEqual(arguments.input_mode, "keyboard")

    def test_spi_input_can_be_selected(self):
        arguments = self.parse_args(["--input", "spi"])

        self.assertEqual(arguments.input_mode, "spi")

    def test_unknown_input_is_rejected(self):
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                self.parse_args(["--input", "joystick"])


if __name__ == "__main__":
    unittest.main()

import importlib.util
import unittest


class FakeSpiDevice:
    def __init__(self, response=0x80, transfer_error=None):
        self.response = response
        self.transfer_error = transfer_error
        self.opened = None
        self.max_speed_hz = None
        self.mode = None
        self.transfers = []
        self.closed = False

    def open(self, bus, device):
        self.opened = (bus, device)

    def xfer2(self, values):
        self.transfers.append(values)
        if self.transfer_error is not None:
            raise self.transfer_error
        return [self.response]

    def close(self):
        self.closed = True


class TivaSpiInputTests(unittest.TestCase):
    def input_class(self):
        module_spec = importlib.util.find_spec("pi.tiva_spi_input")
        self.assertIsNotNone(module_spec, "pi.tiva_spi_input must exist")

        from pi.tiva_spi_input import TivaSpiInput

        return TivaSpiInput

    def test_context_configures_and_closes_spi_device(self):
        tiva_spi_input = self.input_class()
        spi = FakeSpiDevice()

        with tiva_spi_input(spi_factory=lambda: spi):
            self.assertEqual(spi.opened, (0, 0))
            self.assertEqual(spi.max_speed_hz, 100000)
            self.assertEqual(spi.mode, 0)

        self.assertTrue(spi.closed)

    def test_poll_clocks_neutral_byte_and_returns_response(self):
        tiva_spi_input = self.input_class()
        spi = FakeSpiDevice(response=0x40)

        with tiva_spi_input(spi_factory=lambda: spi) as controller:
            value = controller.poll()

        self.assertEqual(spi.transfers, [[0x00]])
        self.assertEqual(value, 0x40)

    def test_transfer_error_returns_neutral_and_stops_input(self):
        tiva_spi_input = self.input_class()
        spi = FakeSpiDevice(transfer_error=OSError("SPI disconnected"))

        with tiva_spi_input(spi_factory=lambda: spi) as controller:
            value = controller.poll()

            self.assertEqual(value, 0x00)
            self.assertFalse(controller.running)


if __name__ == "__main__":
    unittest.main()

"""Read the active-high controller byte from the Tiva over SPI."""

from __future__ import annotations

try:
    from .controller_protocol import NEUTRAL
except ImportError:
    from controller_protocol import NEUTRAL


class TivaSpiInput:
    def __init__(
        self,
        bus: int = 0,
        device: int = 0,
        max_speed_hz: int = 100000,
        spi_factory=None,
    ) -> None:
        self.bus = bus
        self.device = device
        self.max_speed_hz = max_speed_hz
        self.running = True
        self._spi_factory = spi_factory
        self._spi = None

    def __enter__(self):
        if self._spi_factory is None:
            import spidev

            self._spi_factory = spidev.SpiDev

        self._spi = self._spi_factory()
        self._spi.open(self.bus, self.device)
        self._spi.max_speed_hz = self.max_speed_hz
        self._spi.mode = 0
        return self

    def __exit__(self, exception_type, exception, traceback):
        if self._spi is not None:
            self._spi.close()
            self._spi = None

    def poll(self) -> int:
        if self._spi is None:
            raise RuntimeError("TivaSpiInput must be entered before polling")

        try:
            response = self._spi.xfer2([NEUTRAL])
        except OSError:
            self.running = False
            return NEUTRAL

        return int(response[0]) & 0xFF

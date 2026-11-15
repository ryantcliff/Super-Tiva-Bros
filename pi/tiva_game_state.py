#!/usr/bin/env python3

from .game_protocol import PACKET_STRUCT, decode_packet


class TivaGameStateReader:
    def __init__(
            self,
            bus=0,
            device=0,
            max_speed_hz=100000,
            spi_factory=None,
    ):
        self.bus = bus
        self.device = device
        self.max_speed_hz = max_speed_hz
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

    def poll(self):
        if self._spi is None:
            raise RuntimeError("TivaGameStateReader must be entered before polling")
        response = self._spi.xfer2([0x00] * PACKET_STRUCT.size)
        return decode_packet(bytes(response))
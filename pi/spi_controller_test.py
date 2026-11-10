#!/usr/bin/env python3
"""Print controller-state changes received from the Tiva over SPI."""

import time

try:
    from .tiva_spi_input import TivaSpiInput
except ImportError:
    from tiva_spi_input import TivaSpiInput


POLL_SECONDS = 1.0 / 60.0


def main() -> int:
    previous = None

    try:
        with TivaSpiInput() as controller:
            while controller.running:
                value = controller.poll()
                if value != previous:
                    print(f"Controller: 0x{value:02X}", flush=True)
                    previous = value
                time.sleep(POLL_SECONDS)
    except KeyboardInterrupt:
        return 0

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

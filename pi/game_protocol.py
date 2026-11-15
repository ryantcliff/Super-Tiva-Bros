#!/usr/bin/env python3

import struct
from dataclasses import dataclass

PACKET_STRUCT = struct.Struct("<BBBhhhhBB")

PROTOCOL_MAGIC = 0xA5
PROTOCOL_VERSION = 0x01

PLAYER_IDLE = 0
PLAYER_WALKING = 1
PLAYER_JUMPING = 2

FACING_LEFT = 0
FACING_RIGHT = 1

@dataclass(frozen=True)
class GameSnapshot:
    sequence: int
    player_x: int
    player_y: int
    camera_x: int
    camera_y: int
    player_state: int
    facing_direction: int


def decode_packet(packet: bytes) -> GameSnapshot:
    if len(packet) != PACKET_STRUCT.size:
        raise ValueError(f"packet must be exactly {PACKET_STRUCT.size} bytes")
    
    (
        magic,
        version,
        sequence,
        player_x,
        player_y,
        camera_x,
        camera_y,
        player_state,
        facing_direction,
    ) = PACKET_STRUCT.unpack(packet)

    if magic != PROTOCOL_MAGIC:
        raise ValueError("packet magic byte is invalid")

    if version != PROTOCOL_VERSION:
        raise ValueError("packet version is invalid")

    return GameSnapshot(
        sequence=sequence,
        player_x=player_x,
        player_y=player_y,
        camera_x=camera_x,
        camera_y=camera_y,
        player_state=player_state,
        facing_direction=facing_direction,
    )
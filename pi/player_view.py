#!/usr/bin/env python3

from .game_protocol import (
    FACING_RIGHT,
    PLAYER_JUMPING,
    PLAYER_WALKING,
    GameSnapshot
)

ANIMATION_FRAME_TICKS = 8

def select_player_sprite(snapshot: GameSnapshot) -> int:
    if snapshot.player_state == PLAYER_JUMPING:
        return 1
    if snapshot.player_state == PLAYER_WALKING:
        animation_frame = (snapshot.sequence // ANIMATION_FRAME_TICKS) % 2
        if snapshot.facing_direction == FACING_RIGHT:
            return 2 + animation_frame

        return 4 + animation_frame

    return 0
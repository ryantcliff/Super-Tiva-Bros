#!/usr/bin/env python3

import unittest
from pi.game_protocol import GameSnapshot
from pi.player_view import select_player_sprite

def make_snapshot(state, direction, sequence=0):
    return GameSnapshot(
        sequence=sequence,
        player_x=0,
        player_y=0,
        camera_x=0,
        camera_y=0,
        player_state=state,
        facing_direction=direction,
    )


class PlayerViewTests(unittest.TestCase):
    def test_idle_uses_idle_sprite(self):
        snapshot = make_snapshot(state=0, direction=1)
        self.assertEqual(select_player_sprite(snapshot), 0)

    def test_jumping_uses_jump_sprite(self):
        snapshot = make_snapshot(state=2, direction=1)
        self.assertEqual(select_player_sprite(snapshot), 1)

    def test_walking_right_alternates_frames(self):
        self.assertEqual(
            select_player_sprite(make_snapshot(state=1, direction=1, sequence=0)), 2
        )
        self.assertEqual(
            select_player_sprite(make_snapshot(state=1, direction=1, sequence=8)), 3
        )

    def test_walking_left_alternates_frames(self):
        self.assertEqual(
            select_player_sprite(make_snapshot(state=1, direction=0, sequence=0)), 4
        )
        self.assertEqual(
            select_player_sprite(make_snapshot(state=1, direction=0, sequence=8)), 5
        )


if __name__ == "__main__":
    unittest.main()
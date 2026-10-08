#include "timing.h"

void timing_init(timing *state, uint32_t current_time_ms){
    state->previous_time_ms = current_time_ms;
    state->elapsed_ms = 0;
    state->delta_time = 0.0f;
}

void timing_update(timing *state, uint32_t current_time_ms){
    uint32_t delta_ms = current_time_ms - state->previous_time_ms;

    state->previous_time_ms = current_time_ms;
    state->elapsed_ms += delta_ms;
    state->delta_time = (float)delta_ms * 0.001f;
}

float timing_get_delta_time(const timing *state){
    return state->delta_time;
}

uint64_t timing_get_elapsed_ms(const timing *state){
    return state->elapsed_ms;
}

#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>

typedef struct timing {
    uint32_t previous_time_ms;
    uint64_t elapsed_ms;
    float delta_time;
} timing;

/* Supply ticks from a monotonic millisecond clock. Update at least once per
 * full uint32_t clock period so unsigned subtraction handles rollover. */
void timing_init(timing *state, uint32_t current_time_ms);
void timing_update(timing *state, uint32_t current_time_ms);
/* Frame duration in seconds, suitable for physics_update. */
float timing_get_delta_time(const timing *state);
uint64_t timing_get_elapsed_ms(const timing *state);

#endif

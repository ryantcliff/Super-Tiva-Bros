#include "system.h"

static uint32_t system_clock_hz;

void system_clock_init(void){
	system_clock_hz = 16000000u;
}

uint32_t system_get_clock_hz(void){
	return system_clock_hz;
}

#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

/* Direction bits in the NES controller byte, used by input_update. */
#define INPUT_BUTTON_UP    (1u << 4)
#define INPUT_BUTTON_DOWN  (1u << 5)
#define INPUT_BUTTON_LEFT  (1u << 6)
#define INPUT_BUTTON_RIGHT (1u << 7)

#define SYSCTL_RCGCGPIO (*((volatile uint32_t *)0x400FE608))

#endif

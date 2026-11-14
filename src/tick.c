#include "system.h"
#include "tick.h"


#define HWREG(address) (*((volatile uint32_t *)(address)))
#define STCTRL		HWREG(0xE000E010u)
#define STRELOAD	HWREG(0xE000E014u)
#define STCURRENT	HWREG(0xE000E018u)

#define STCTRL_ENABLE	(1u << 0)
#define STCTRL_INTEN	(1u << 1)
#define STCTRL_CLK_SRC	(1u << 2)
#define STCTRL_COUNT	(1u << 16)

static volatile uint32_t milliseconds = 0;

void tick_init(void){
	STCTRL = 0u;
	STRELOAD = (system_get_clock_hz()/1000u) - 1u;
	STCURRENT = 0u;
	milliseconds = 0u;
	STCTRL = STCTRL_CLK_SRC | STCTRL_INTEN | STCTRL_ENABLE;
}

uint32_t tick_millis(void){
	return milliseconds;
}

void SysTick_Handler(void){
	++milliseconds;
}

	.text
	.global input_controller_gpio_init
	.global input_controller_delay

;---input_controller_gpio_init---
;
; Configures the TES-CTRL-MK1 interface on GPIO Port E:
; PE0 = shift clock output, initially low
; PE1 = active-low parallel-load output, initially high
; PE2 = serial-data input
;
; This routine adapts the GPIO initialization pattern used in
; the earlier CSE 379 and CSE 479 keypad projects to the
; TM4C1294 GPIO Port E AHB register addresses.
;
input_controller_gpio_init:
	push {r4, r5, r6, lr}

	; Enable the clock for GPIO Port E without changing other ports.
	mov r4, #0xE000
	movt r4, #0x400F
	ldr r5, [r4, #0x608]
	orr r5, r5, #0x10
	str r5, [r4, #0x608]

controller_gpio_wait:
	ldr r5, [r4, #0xA08]
	tst r5, #0x10
	beq controller_gpio_wait

	; Use the TM4C1294 Port E AHB register block.
	mov r4, #0xC000
	movt r4, #0x4005

	; Select ordinary digital GPIO rather than analog or alternate functions.
	ldr r5, [r4, #0x420]
	bic r5, r5, #0x07
	str r5, [r4, #0x420]

	ldr r5, [r4, #0x528]
	bic r5, r5, #0x07
	str r5, [r4, #0x528]

	ldr r5, [r4, #0x52C]
	mov r6, #0x0FFF
	bic r5, r5, r6
	str r5, [r4, #0x52C]

	; Preload PE1 high before enabling PE0 and PE1 as outputs.
	mov r5, #0x02
	str r5, [r4, #0x00C]

	ldr r5, [r4, #0x400]
	bic r5, r5, #0x07
	orr r5, r5, #0x03
	str r5, [r4, #0x400]

	ldr r5, [r4, #0x51C]
	orr r5, r5, #0x07
	str r5, [r4, #0x51C]

	; Hold PE2 high if the controller or data wire is disconnected.
	ldr r5, [r4, #0x514]
	bic r5, r5, #0x04
	str r5, [r4, #0x514]
	ldr r5, [r4, #0x510]
	orr r5, r5, #0x04
	str r5, [r4, #0x510]

	pop {r4, r5, r6, lr}
	mov pc, lr

;---input_controller_delay---
;
; Waits approximately three processor cycles per iteration,
; matching the timing behavior previously supplied by SysCtlDelay.
;
; Arguments:
; r0 = number of iterations (must be at least one)
;
input_controller_delay:
	subs r0, r0, #1
	bne input_controller_delay
	mov pc, lr

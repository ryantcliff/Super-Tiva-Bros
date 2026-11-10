	.text
	.global uart_init

;---uart_init---
;
; Initializes UART0 on PA0/PA1 for PuTTY. UART0 uses the
; 16 MHz PIOSC and is configured for 115200 baud, 8 data
; bits, one stop bit, and no parity.
;
; This version uses the TM4C1294 GPIO Port A AHB address.
;
uart_init:
	push {r4, r5, lr}

	; Enable the UART0 clock without changing other UART clocks.
	mov r4, #0xE618
	movt r4, #0x400F
	ldr r5, [r4]
	orr r5, r5, #0x01
	str r5, [r4]

	; Enable the GPIO Port A clock without changing other ports.
	mov r4, #0xE608
	movt r4, #0x400F
	ldr r5, [r4]
	orr r5, r5, #0x01
	str r5, [r4]

	; Wait until UART0 and GPIO Port A are ready.
	mov r4, #0xEA18
	movt r4, #0x400F
uart_wait:
	ldr r5, [r4]
	tst r5, #0x01
	beq uart_wait

	mov r4, #0xEA08
	movt r4, #0x400F
gpio_wait:
	ldr r5, [r4]
	tst r5, #0x01
	beq gpio_wait

	; Disable UART0 while changing its configuration.
	mov r4, #0xC000
	movt r4, #0x4000
	mov r5, #0
	str r5, [r4, #0x030]

	; Select the 16 MHz PIOSC and configure 115200 baud.
	mov r5, #0x05
	str r5, [r4, #0xFC8]
	mov r5, #8
	str r5, [r4, #0x024]
	mov r5, #44
	str r5, [r4, #0x028]

	; Configure 8-bit data, one stop bit, and no parity.
	mov r5, #0x60
	str r5, [r4, #0x02C]

	; Configure PA0 and PA1 for their UART0 alternate functions.
	mov r4, #0x8000
	movt r4, #0x4005
	ldr r5, [r4, #0x420]
	orr r5, r5, #0x03
	str r5, [r4, #0x420]

	ldr r5, [r4, #0x51C]
	orr r5, r5, #0x03
	str r5, [r4, #0x51C]

	ldr r5, [r4, #0x528]
	bic r5, r5, #0x03
	str r5, [r4, #0x528]

	ldr r5, [r4, #0x52C]
	bic r5, r5, #0xFF
	orr r5, r5, #0x11
	str r5, [r4, #0x52C]

	; Enable UART0, its transmitter, and its receiver.
	mov r4, #0xC030
	movt r4, #0x4000
	mov r5, #0x0301
	str r5, [r4]

	pop {r4, r5, lr}
	mov pc, lr

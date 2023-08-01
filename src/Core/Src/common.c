/*
 * common.c
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */

#include "lsm303agr_driver.h"
#include "common.h"
#include "main.h"
#include "platform.h"

static void error_acc_init() {
	disable_interrupts();

	led_off(0);
	led_off(1);
	led_on(2);
	// halt the device
    while (1)
        ;
}

static void error_acc_read() {
	disable_interrupts();

	led_off(0);
	led_off(1);
	led_on(3);
    // halt the device
    while (1)
        ;
}

static void heart_beat(uint64_t t_ms) {
	if (t_ms % 250 == 0) {
		led_toggle(1);
	}
}

void boot() {
    if (lsm303agr_init() != ERR_NONE) {
        error_acc_init();
    }
    led_on(0);
}

uint32_t elapsed_us = 0;
uint64_t t_ms = 0;

void loop() {
	LSM303AGR_Readings rd;
	uint16_t tick;

	tick = get_ticks_us();
	t_ms += 10;

	heart_beat(t_ms);

	if (lsm303agr_measure(&rd) != ERR_NONE) {
		error_acc_read();
	}

	elapsed_us = get_elapsed_us(tick);
}

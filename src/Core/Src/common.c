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
	// halt the device
    while (1)
        ;
}

static void error_acc_read() {
    // halt the device
    while (1)
        ;
}

void boot() {
    if (lsm303agr_init() != ERR_NONE) {
        error_acc_init();
    }
}

uint16_t tick = 0;
uint64_t t_ms = 0;

void loop() {
	LSM303AGR_Readings rd;

	t_ms += 10;
	tick = get_ticks_us();
	if (lsm303agr_measure(&rd) != ERR_NONE) {
		error_acc_read();
	}
}

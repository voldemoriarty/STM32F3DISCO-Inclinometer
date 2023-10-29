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
#include <string.h>

// =================== GLOBALS ======================

uint32_t elapsed_us = 0;
uint64_t t_ms = 0;
Packet_t transmit_pckt = {0};
int16_t  accf[3] = {0};

// ================== FUNCTIONS =====================

static void error_acc_init()
{
	disable_interrupts();

	led_off(BOOT_LED);
	led_off(LED_HB);
	led_on(LED_SENS_ERR);
	// halt the device
    while (1)
        ;
}

static void error_acc_read()
{
	disable_interrupts();

	led_off(BOOT_LED);
	led_off(LED_HB);
	led_on(LED_SENS_ERR);
    // halt the device
    while (1)
        ;
}

static void heart_beat()
{
	static uint16_t counter_ms = 0;

	if (counter_ms == 250) {
		led_toggle(LED_HB);
		counter_ms = 0;
	}
	else {
		counter_ms += dt_ms;
	}
}

static void filter_readings(LSM303AGR_Readings *reading)
{
	unsigned i;

	for (i = 0; i < 3; ++i) {
		accf[i] += dt*acc_filt_pole*(reading->acc[i] - accf[i]);
	}
}

void boot()
{
    if (lsm303agr_init() != ERR_NONE) {
        error_acc_init();
    }
    led_on(BOOT_LED);

	transmit_pckt.preamble[0] = 0x69;
	transmit_pckt.preamble[1] = 0x42;
	transmit_pckt.preamble[2] = 0x69;
	transmit_pckt.preamble[3] = 0x42;
}

void loop()
{
	LSM303AGR_Readings rd;
	uint16_t tick;

	tick = get_ticks_us();
	t_ms += dt_ms;

	heart_beat();

	if (lsm303agr_measure(&rd) != ERR_NONE) {
		error_acc_read();
	}

	filter_readings(&rd);

	memcpy(transmit_pckt.acc, accf, sizeof(transmit_pckt.acc));
	transmit_pckt.acc_temp  = rd.temp;
	transmit_pckt.loop_time = elapsed_us;

	write_uart((uint8_t*)&transmit_pckt, sizeof(transmit_pckt));

	elapsed_us = get_elapsed_us(tick);
}

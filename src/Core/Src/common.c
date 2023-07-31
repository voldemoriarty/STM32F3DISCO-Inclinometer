/*
 * common.c
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */

#include <lsm303agr_driver.h>
#include "common.h"
#include "main.h"
#include "platform.h"
#include <stdio.h>

static void clear_console() {
    // https://stackoverflow.com/questions/37774983/clearing-the-screen-by-printing-a-character
    printf("\033[2J\033[H");
}

static void boot_message() {
    clear_console();
    puts("System boot!\r");
}

static void error_acc_init() {
    // display message 
    puts("Acc init error!\r");

    // halt the device
    while (1)
        ;
}

static void error_acc_read() {
    // display message
    puts("Acc reading error!\r");

    // halt the device
    while (1)
        ;
}

void boot() {
    boot_message();

    if (lsm303agr_init() != ERR_NONE) {
        error_acc_init();
    }
    puts("Acc init complete!\r");

    delay_ms(70);
}

uint16_t tick = 0;
uint64_t t_ms = 0;

void loop() {
	LSM303AGR_Readings rd;
	int i;

	t_ms += 10;
	tick = get_ticks_us();
	if (lsm303agr_measure(&rd) != ERR_NONE) {
		error_acc_read();
	}

	// every 250ms send data to serial
	if (t_ms % 250 == 0) {
		printf("[%3lu us] ", get_elapsed_us(tick));

		printf("Acc Buff: [");
		for (i = 0; i < 3; ++i) {
			printf("%+.3f, ", rd.acc[i] * 0.0098f);
		}
		printf("]; ");
		printf("Temp: %d        \r", rd.temp);


		fflush(stdout);
	}
}

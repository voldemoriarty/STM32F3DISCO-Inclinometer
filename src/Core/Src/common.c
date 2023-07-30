/*
 * common.c
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */


#include <lsm303agr_driver.h>
#include "common.h"
#include "main.h"
#include <stdio.h>

static void clear_console() {
	// https://stackoverflow.com/questions/37774983/clearing-the-screen-by-printing-a-character
	printf("\033[2J\033[H");
}

static void boot_message() {
	clear_console();
	puts("System boot!\r");
}

void boot() {
	boot_message();
	HAL_Delay(250);

	if (lsm303agr_init() != ERR_NONE) {
		puts("Acc init error!\r");
		while (1);
	}

	puts("Acc init complete!\r");

	while (1) {
	    lsm303agr_readAcc();
	    printf("Temp: %.2f\r\n", lsm303agr_readTemp());
	    HAL_Delay(1000);
	}
}

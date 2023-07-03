/*
 * common.c
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */


#include "common.h"

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
}

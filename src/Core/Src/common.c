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
    LSM303AGR_Readings rd;
    int i;

    boot_message();

    if (lsm303agr_init() != ERR_NONE) {
        error_acc_init();
    }
    puts("Acc init complete!\r");

    HAL_Delay(250);

    while (1) {
        if (lsm303agr_measure(&rd) != ERR_NONE) {
            error_acc_read();
        }

        printf("Acc Buff: [");
        for (i = 0; i < 3; ++i) {
            printf("%+.3f, ", rd.accl[i] * 0.001f);
        }
        printf("]; ");
        printf("Temp: %d\r", rd.temp);
        fflush(stdout);
        HAL_Delay(250);
    }
}

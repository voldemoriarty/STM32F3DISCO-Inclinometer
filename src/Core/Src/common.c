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
#include <stdbool.h>

// =================== GLOBALS ======================

uint32_t elapsed_us = 0;
uint64_t t_ms = 0;
Packet_t transmit_pckt = { 0 };
int16_t accf[3] = { 0 };
int16_t acc_offset[3] = { 0 };
int16_t acc_corr[3] = { 0 };
bool calibration = false;
uint16_t i_calibration = 0;

// ================== FUNCTIONS =====================

static void error_acc_init()
{
    disable_interrupts();

    led_off(BOOT_LED);
    led_off(LED_HB);
    led_on(LED_SENS_ERR);
    // halt the device
    while (1);
}

static void error_acc_read()
{
    disable_interrupts();

    led_off(BOOT_LED);
    led_off(LED_HB);
    led_on(LED_SENS_ERR);
    // halt the device
    while (1);
}

static void heart_beat()
{
    static uint16_t counter_ms = 0;

    if (counter_ms == 500) {
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
        accf[i] += dt * acc_filt_pole * (reading->acc[i] - accf[i]);
        acc_corr[i] = accf[i] - acc_offset[i];
    }
}

static void calibration_func()
{
    static int32_t sum[3] = {0};
    const int32_t ref[3] = {0, 0, 1000};
    unsigned i;

    if (i_calibration == 0) {
        sum[0] = sum[1] = sum[2] = 0;
    }

    if (i_calibration == calibration_n) {
        calibration = false;
        for (i = 0; i < 3; ++i) {
            acc_offset[i] = sum[i] / calibration_n;
        }
        led_off(LED_CALIB);
        return;
    }

    for (i = 0; i < 3; ++i) {
        sum[i] += (accf[i] - ref[i]);
    }
    i_calibration++;
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

    if (calibration) {
        calibration_func();
    }

    memcpy(transmit_pckt.acc, acc_corr, sizeof(transmit_pckt.acc));
    transmit_pckt.acc_temp = rd.temp;
    transmit_pckt.loop_time = elapsed_us;

    write_uart((uint8_t*) &transmit_pckt, sizeof(transmit_pckt));

    elapsed_us = get_elapsed_us(tick);
}

void button_callback()
{
    led_on(LED_CALIB);
    calibration = true;
    i_calibration = 0;
}

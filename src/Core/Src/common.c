/*
 * common.c
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */

#include "common.h"
#include "main.h"
#include "platform.h"
#include "display.h"
#include <string.h>
#include <stdbool.h>

// =================== GLOBALS ======================

uint32_t            elapsed_us = 0;
Sensor_Readings     sensors = { 0 };
uint32_t            max_loop_time = 0;
uint64_t            t_ms = 0;
Packet_t            transmit_pckt = { 0 };
float               acc_offset[3] = { 0 };
float               mag_offset[3] = { 0 };
float               gyro_offset[3] = { 0 };
bool                calibration = false;
uint16_t            i_calibration = 0;

// ================== FUNCTIONS =====================

static void sensor_error()
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

static void filter_readings()
{
    const float k_acc = dt * acc_filt_pole;
    const float k_mag = dt * mag_filt_pole;
    const float k_gyr = dt * gyro_filt_pole;
    unsigned i;

    for (i = 0; i < 3; ++i) {
        sensors.accf[i]  += k_acc * (1.00e-3f * sensors.accl.acc[i] - sensors.accf[i] - acc_offset[i]);
        sensors.magf[i]  += k_mag * (1.00e-3f * sensors.accl.mag[i] - sensors.magf[i] - mag_offset[i]);
        sensors.gyrof[i] += k_gyr * (8.75e-3f * sensors.gyro.gyro[i] - sensors.gyrof[i] - gyro_offset[i]);
    }
}

static void calibration_func()
{
    static float sum_acc[3] = { 0 };
    static float sum_gyr[3] = { 0 };
    const float ref_acc[3] = { 0, 0, 1000 };
    unsigned i;

    led_on(LED_CALIB);

    if (i_calibration == 0) {
        sum_acc[0] = sum_acc[1] = sum_acc[2] = 0;
        sum_gyr[0] = sum_gyr[1] = sum_gyr[2] = 0;
    }

    if (i_calibration == calibration_n) {
        calibration = false;
        for (i = 0; i < 3; ++i) {
            acc_offset[i] = sum_acc[i] / calibration_n;
            gyro_offset[i] = sum_gyr[i] / calibration_n;
        }
        led_off(LED_CALIB);
        return;
    }

    for (i = 0; i < 3; ++i) {
        sum_acc[i] += (sensors.accl.acc[i] - ref_acc[i]);
        sum_gyr[i] += (sensors.gyro.gyro[i]);
    }

    i_calibration++;
}

static void stream_or_display()
{
#if display_mode == 1
    static uint16_t ms_elapsed = 0;

    led_on(LED_DISP);
    // display frequency
    if (ms_elapsed == display_frq) {
        display_packet(&transmit_pckt, max_loop_time);
        ms_elapsed = 0;
    }
    else {
        ms_elapsed += dt_ms;
    }
#else
    write_uart((uint8_t*) &transmit_pckt, sizeof(transmit_pckt));
#endif
}

static void prepare_packet()
{
    unsigned i;

    for (i = 0; i < 3; ++i) {
        transmit_pckt.acc[i]  = (int16_t)(sensors.accf[i] * 1000.0f);
        transmit_pckt.mag[i]  = (int16_t)(sensors.magf[i] * 1000.0f);
        transmit_pckt.gyro[i] = (int16_t)(sensors.gyrof[i] * 10.0f);
    }
    transmit_pckt.acc_temp  = sensors.accl.temp;
    transmit_pckt.gyro_temp = sensors.gyro.temp;
    transmit_pckt.loop_time = elapsed_us;
}

static void read_sensors()
{
    if (lsm303agr_measure(&sensors.accl) != ERR_NONE) {
        sensor_error();
    }

    if (i3g4250d_measure(&sensors.gyro) != ERR_NONE_G) {
        sensor_error();
    }

    filter_readings();
}

void boot()
{
    if (lsm303agr_init() != ERR_NONE) {
        sensor_error();
    }
    if (i3g4250d_init() != ERR_NONE_G) {
        sensor_error();
    }
    led_on(BOOT_LED);

    transmit_pckt.preamble[0] = 0x69;
    transmit_pckt.preamble[1] = 0x42;
    transmit_pckt.preamble[2] = 0x69;
    transmit_pckt.preamble[3] = 0x42;
}

void loop()
{
    uint16_t tick;

    tick = get_ticks_us();
    t_ms += dt_ms;

    heart_beat();
    read_sensors();

    if (calibration) {
        calibration_func();
    }

    prepare_packet();
    stream_or_display();

    elapsed_us = get_elapsed_us(tick);
    max_loop_time = max(elapsed_us, max_loop_time);
}

void button_callback()
{
    calibration = true;
    i_calibration = 0;
}

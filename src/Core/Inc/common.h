/*
 * common.h
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#include <stdint.h>

#define BOOT_LED      (0)
#define LED_HB        (1)
#define LED_SENS_ERR  (2)
#define LED_CALIB     (3)
#define LED_DISP      (4)
#define dt            (10.0e-3f)
#define dt_ms         (10)
#define acc_filt_pole (12.5f)
#define calibration_n (500)
#define display_mode  (1)
#define display_frq   (300)

#define max(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})


typedef struct __attribute__((packed))
{
    uint8_t preamble[4];
    int16_t acc[3];
    int16_t mag[3];
    int16_t acc_temp;
    int16_t gyro[3];
    int16_t gyro_temp;
    uint32_t loop_time;
} Packet_t;

void boot();
void loop();
void button_callback();

#endif /* INC_COMMON_H_ */

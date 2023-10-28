/*
 * common.h
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#define BOOT_LED      (0)
#define LED_HB        (1)
#define LED_SENS_ERR  (2)

typedef struct {
    uint8_t  preamble[4];
    int16_t  acc[3];
    int16_t  mag[3];
    int16_t  acc_temp;
    int16_t  gyro[3];
    int16_t  gyro_temp;
    uint32_t loop_time;
} Packet_t;

void boot();
void loop();

#endif /* INC_COMMON_H_ */

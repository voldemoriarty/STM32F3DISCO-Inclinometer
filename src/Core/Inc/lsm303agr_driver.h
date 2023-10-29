/*
 * lsm303lhc_driver.h
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */

#ifndef INC_LSM303AGR_DRIVER_H_
#define INC_LSM303AGR_DRIVER_H_

#include <stdint.h>

typedef enum LSM303AGR_Error
{
    ERR_NONE = 0,
    ERR_ID_A = 1,
    ERR_ID_M = 2,
    ERR_WR_A = 3,
    ERR_RD_A = 4,
    ERR_WR_M = 5,
    ERR_RD_M = 6,
} LSM303AGR_Error;

typedef struct LSM303AGR_Readings
{
    int16_t acc[3];
    int16_t mag[3];
    int16_t temp;
} LSM303AGR_Readings;

LSM303AGR_Error lsm303agr_init();
LSM303AGR_Error lsm303agr_measure(LSM303AGR_Readings *rd);

#endif /* INC_LSM303AGR_DRIVER_H_ */

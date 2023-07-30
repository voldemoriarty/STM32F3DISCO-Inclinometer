/*
 * lsm303lhc_driver.h
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */

#ifndef INC_LSM303AGR_DRIVER_H_
#define INC_LSM303AGR_DRIVER_H_

typedef enum LSM303AGR_Error {
	ERR_ID_A = 1,
	ERR_ID_M = 2
} LSM303AGR_Error;

LSM303AGR_Error lsm303agr_init();

#endif /* INC_LSM303AGR_DRIVER_H_ */

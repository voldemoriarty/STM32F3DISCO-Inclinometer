/*
 * lsm303lhc_driver.c
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */


#include "lsm303lhc_driver.h"
#include "i2c.h"
#include <stdint.h>
#include <stdio.h>

#define ACC_I2C_ADDR_RD 0x33	// Sec 5.1.2, p21 datasheet
#define ACC_I2C_ADDR_WR 0x32
#define MAG_I2C_ADDR_RD 0x3D	// Sec 5.1.3, p22 datasheet
#define MAG_I2C_ADDR_WR 0x3C

#define ACC_WHO_AM_I 0x33

// read len i2c registers into buff. Returns 0 on OK
static int read_i2c_reg(uint8_t addr, uint16_t reg, uint16_t len, uint8_t *buff) {
	HAL_StatusTypeDef ret;
	ret = HAL_I2C_Mem_Read(&hi2c1, addr, reg, 1, buff, len, HAL_MAX_DELAY);

	if (ret == HAL_OK)
		return 0;
	else
		return 1;
}

static int read_acc_reg(uint16_t reg, uint16_t len, uint8_t *buff) {
	return read_i2c_reg(ACC_I2C_ADDR_RD, reg, len, buff);
}

static int read_mag_reg(uint16_t reg, uint16_t len, uint8_t *buff) {
	return read_i2c_reg(MAG_I2C_ADDR_RD, reg, len, buff);
}


int lsm303lhc_init() {
	uint8_t tmp;

	// read who_am_i register
	read_acc_reg(0x0f, 1, &tmp);
	printf("ACC WHO_AM_I: 0x%X\r\n", tmp);

	read_mag_reg(0x0f, 1, &tmp);
	printf("MAG WHO_AM_I: 0x%X\r\n", tmp);
	return 0;
}

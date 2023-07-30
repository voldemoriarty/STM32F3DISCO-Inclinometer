/*
 * lsm303lhc_driver.c
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */


#include <lsm303agr_driver.h>
#include "i2c.h"
#include <stdint.h>
#include <stdio.h>

// I2C Slave Addresses
#define ACC_I2C_ADDR_RD 0x33	// Tab 24, p39 datasheet
#define ACC_I2C_ADDR_WR 0x32
#define MAG_I2C_ADDR_RD 0x3D	// Tab 25, p39 datasheet
#define MAG_I2C_ADDR_WR 0x3C

// Register Addresses
#define REG_CTR_A 		0x20
#define REG_OUT_A 		0x28
#define REG_TMP_A 		0x0C
#define REG_WHOAMI_A 	0x0F
#define REG_WHOAMI_M	0x4F

// Table 20 datasheet
#define ACC_ODR_SHIFT	4
#define ACC_ODR_1HZ 	0b0001
#define ACC_ODR_10HZ 	0b0010
#define ACC_ODR_25HZ	0b0011
#define ACC_ODR_50HZ	0b0100
#define ACC_ODR_100HZ	0b0101
#define ACC_ODR_200HZ	0b0110
#define ACC_ODR_400HZ	0b0111

// Table 27 datasheet
#define ACC_FS_SHIFT 	4
#define ACC_FS_2G		0b00
#define ACC_FS_4G		0b01
#define ACC_FS_8G		0b10
#define ACC_FS_16G		0b11

// WHO_AM_I Values
#define ACC_WHOAMI		0x33
#define MAG_WHOAMI		0x40

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


LSM303AGR_Error lsm303agr_init() {
	uint8_t tmp;

	// read who_am_i register to identify device. Error out on mismatch
	read_acc_reg(REG_WHOAMI_A, 1, &tmp);
	printf("ACC WHO_AM_I: 0x%X\r\n", tmp);
	if (tmp != ACC_WHOAMI) {
		return ERR_ID_A;
	}

	read_mag_reg(REG_WHOAMI_M, 1, &tmp);
	printf("MAG WHO_AM_I: 0x%X\r\n", tmp);
	if (tmp != MAG_WHOAMI) {
		return ERR_ID_M;
	}

	return 0;
}

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
#define REG_CTR_A       0x20
#define REG_OUT_A       0x28
#define REG_TMP_A       0x0C
#define REG_TMP_CFG_A   0x1F
#define REG_WHOAMI_A    0x0F
#define REG_WHOAMI_M    0x4F

// Table 35 datasheet
#define ACC_ODR_SHIFT   4
#define ACC_ODR_1HZ     (0b0001 << ACC_ODR_SHIFT)
#define ACC_ODR_10HZ    (0b0010 << ACC_ODR_SHIFT)
#define ACC_ODR_25HZ    (0b0011 << ACC_ODR_SHIFT)
#define ACC_ODR_50HZ    (0b0100 << ACC_ODR_SHIFT)
#define ACC_ODR_100HZ   (0b0101 << ACC_ODR_SHIFT)
#define ACC_ODR_200HZ   (0b0110 << ACC_ODR_SHIFT)
#define ACC_ODR_400HZ   (0b0111 << ACC_ODR_SHIFT)
#define ACC_XEN         (1)
#define ACC_YEN         (1 << 1)
#define ACC_ZEN         (1 << 2)
#define ACC_LPEN        (1 << 3)
#define ACC_ALL_EN      (ACC_XEN | ACC_YEN | ACC_ZEN)
#define ACC_BDU_EN      (1 << 7)

// Table 27 datasheet
#define ACC_FS_SHIFT    4
#define ACC_FS_2G       (0b00 << ACC_FS_SHIFT)
#define ACC_FS_4G       (0b01 << ACC_FS_SHIFT)
#define ACC_FS_8G       (0b10 << ACC_FS_SHIFT)
#define ACC_FS_16G      (0b11 << ACC_FS_SHIFT)

// WHO_AM_I Values
#define ACC_WHOAMI      0x33
#define MAG_WHOAMI      0x40

// write into i2c register
static int write_i2c_reg(uint8_t addr, uint16_t reg, uint8_t value) {
    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Mem_Write(&hi2c1, addr, reg, 1, &value, 1, HAL_MAX_DELAY);

    if (ret == HAL_OK)
        return 0;
    else
        return 1;
}

// read len i2c registers into buff. Returns 0 on OK
static int read_i2c_reg(uint8_t addr, uint16_t reg, uint16_t len, uint8_t *buff) {
    HAL_StatusTypeDef ret;

    // MSB is high for multi byte reads
    if (len > 1)
        reg |= 0x80;

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

static int write_acc_reg(uint16_t reg, uint8_t val) {
    return write_i2c_reg(ACC_I2C_ADDR_WR, reg, val);
}

static int write_mag_reg(uint16_t reg, uint8_t val) {
    return write_i2c_reg(MAG_I2C_ADDR_WR, reg, val);
}

// write acc register with verification. Write & read back value
static LSM303AGR_Error write_acc_reg_v(uint16_t reg, uint8_t val) {
    int tmp;
    uint8_t buff;

    tmp = write_acc_reg(reg, val);
    if (tmp != 0) {
        return ERR_WR_A;
    }

    tmp = read_acc_reg(reg, 1, &buff);
    if (tmp != 0) {
        return ERR_RD_A;
    }

    if (buff == val) {
        return ERR_NONE;
    } else {
        return ERR_WR_A;
    }
}

// write mag register with verification. Write & read back value
static LSM303AGR_Error write_mag_reg_v(uint16_t reg, uint8_t val) {
    int tmp;
    uint8_t buff;

    tmp = write_mag_reg(reg, val);
    if (tmp != 0) {
        return ERR_WR_M;
    }

    tmp = read_mag_reg(reg, 1, &buff);
    if (tmp != 0) {
        return ERR_RD_M;
    }

    if (buff == val) {
        return ERR_NONE;
    } else {
        return ERR_WR_M;
    }
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

    // enable temperature readings
    if (write_acc_reg_v(REG_TMP_CFG_A, 0b11000000) != ERR_NONE) {
        puts("Error setting TMP_CFG_A\r");
        return ERR_WR_A;
    }

    // enable accelerometer
    if (write_acc_reg_v(REG_CTR_A, ACC_ODR_100HZ | ACC_ALL_EN) != ERR_NONE) {
        puts("Error setting CTR1_A\r");
        return ERR_WR_A;
    }

    if (write_acc_reg_v(REG_CTR_A + 3, ACC_FS_2G | ACC_BDU_EN) != ERR_NONE) {
        puts("Error setting CTR4_A\r");
        return ERR_WR_A;
    }
    return ERR_NONE;
}

float lsm303agr_readTemp() {
    int8_t buff[2];

    read_acc_reg(REG_TMP_A, 2, (uint8_t*)buff);
    printf("Buff = [0x%X, 0x%X];\r\n", buff[1], buff[0]);

    return (float)(buff[1]) + 20.0f;
}

void lsm303agr_readAcc() {
    uint8_t buff[6];
    int i;

    read_acc_reg(REG_OUT_A, 6, buff);

    printf("Buff = [");
    for(i = 0; i < 6; ++i) {
        printf("0x%X, ", buff[i]);
    }
    printf("];\r\n");
}




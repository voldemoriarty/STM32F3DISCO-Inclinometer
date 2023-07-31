/*
 * lsm303lhc_driver.c
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */

#include <lsm303agr_driver.h>
#include <platform.h>
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
#define REG_CFG_A_M     0x60
#define REG_CFG_B_M     0x61
#define REG_CFG_C_M     0x62
#define REG_OUT_M       0x68

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
#define ACC_HR_EN       (1 << 3)
#define MAG_BDU_EN      (1 << 4)

// Table 27 datasheet
#define ACC_FS_SHIFT    4
#define ACC_FS_2G       (0b00 << ACC_FS_SHIFT)
#define ACC_FS_4G       (0b01 << ACC_FS_SHIFT)
#define ACC_FS_8G       (0b10 << ACC_FS_SHIFT)
#define ACC_FS_16G      (0b11 << ACC_FS_SHIFT)

// Table 93 datasheet
#define MAG_ODR_SHIFT   2
#define MAG_ODR_10HZ    (0b00 << MAG_ODR_SHIFT)
#define MAG_ODR_20HZ    (0b01 << MAG_ODR_SHIFT)
#define MAG_ODR_50HZ    (0b10 << MAG_ODR_SHIFT)
#define MAG_ODR_100HZ   (0b11 << MAG_ODR_SHIFT)

// WHO_AM_I Values
#define ACC_WHOAMI      0x33
#define MAG_WHOAMI      0x40

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
        puts("Error in WHOAMI ACC\r");
        return ERR_ID_A;
    }

    read_mag_reg(REG_WHOAMI_M, 1, &tmp);
    printf("MAG WHO_AM_I: 0x%X\r\n", tmp);
    if (tmp != MAG_WHOAMI) {
        puts("Error in WHOAMI MAG\r");
        return ERR_ID_M;
    }

    // enable temperature readings. For temperature measurements, BDU must be
    // enabled for accelerometer. It will be done in next step
    if (write_acc_reg_v(REG_TMP_CFG_A, 0b11000000) != ERR_NONE) {
        puts("Error setting TMP_CFG_A\r");
        return ERR_WR_A;
    }

    // enable accelerometer. Set data-rate and axis enable in CTR1
    if (write_acc_reg_v(REG_CTR_A, ACC_ODR_100HZ | ACC_ALL_EN) != ERR_NONE) {
        puts("Error setting CTR1_A\r");
        return ERR_WR_A;
    }

    // Set full scale, BDU (required for temperature) & High resolution mode in CTR4 (CTR4 = CTR + 3)
    if (write_acc_reg_v(REG_CTR_A + 3, ACC_FS_2G | ACC_BDU_EN | ACC_HR_EN) != ERR_NONE) {
        puts("Error setting CTR4_A\r");
        return ERR_WR_A;
    }

    // enable magnetometer. Set data-rate in REG_A. No axis enable here ... weird
    if (write_mag_reg_v(REG_CFG_A_M, MAG_ODR_10HZ) != ERR_NONE) {
        puts("Error setting CFG_REG_A_M\r");
        return ERR_WR_M;
    }

    // enable BDU in REG_C
    if (write_mag_reg_v(REG_CFG_C_M, MAG_BDU_EN) != ERR_NONE) {
        puts("Error setting CFG_REG_C_M\r");
        return ERR_WR_M;
    }
    return ERR_NONE;
}

LSM303AGR_Error lsm303agr_measure(LSM303AGR_Readings *rd) {
    int i;

    // read TEMP_L & TEMP_H registers into 16bit field.
    if (read_acc_reg(REG_TMP_A, 2, (uint8_t *)&rd->temp) != 0) {
       return ERR_RD_A;
    }

    // data is 8bit left justified. Compensate for it & add offset
    rd->temp >>= 8;
    rd->temp += 25;

    // read all out registers at once.
    if (read_acc_reg(REG_OUT_A, 6, (uint8_t *)rd->acc) != 0) {
        return ERR_RD_A;
    }

    // scaling
    for (i = 0; i < 3; ++i) {
        // correct for left justified data
        // in high resolution mode, shift by 4
        // in normal mode, shift by 6
        // in low power mode, shift by 8
        rd->acc[i] >>= 4;

        // apply scaling, based on table 3 of datasheet
        rd->acc[i] *= 1;
    }

    return ERR_NONE;
}


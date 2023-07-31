/*
 * platform.c
 *
 *  Created on: Jul 30, 2023
 *      Author: msaad
 */

#include "platform.h"
#include "main.h"
#include "i2c.h"

void delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}

int write_i2c_reg(uint8_t addr, uint16_t reg, uint8_t value) {
    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Mem_Write(&hi2c1, addr, reg, 1, &value, 1, HAL_MAX_DELAY);

    if (ret == HAL_OK)
        return 0;
    else
        return 1;
}

int read_i2c_reg(uint8_t addr, uint16_t reg, uint16_t len, uint8_t *buff) {
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

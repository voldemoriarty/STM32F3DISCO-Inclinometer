/*
 * platform.c
 *
 *  Created on: Jul 30, 2023
 *      Author: msaad
 */

#include "platform.h"
#include "main.h"
#include "i2c.h"
#include "tim.h"

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

uint16_t get_ticks_us() {
	return (uint16_t)(TIM7->CNT & 0xffff);
}

uint32_t get_elapsed_us(uint16_t prev) {
	uint32_t now;

	now = get_ticks_us();

	// If the timer has rolled over once or more, then current value
	// will start from 0 and hence will be less then prev. To cater this
	// we will add timer max value to now reading. Since we dont know
	// how many times the timer rolled over, we will assume it's 1 and
	// hope that the function wont be used to measure times beyond it's
	// range (~ 65ms)
	if (now <= prev) {
		now += 0x10000;
	}

	return (now - prev);
}

void led_on(unsigned led) {
	GPIOE->ODR |= (1 << (led+8));
}

void led_off(unsigned led) {
	GPIOE->BRR |= (1 << (led+8));
}

void led_toggle(unsigned led) {
	GPIOE->ODR ^= (1 << (led+8));
}

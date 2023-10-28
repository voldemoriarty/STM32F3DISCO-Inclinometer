/*
 * platform.h
 *
 *  Created on: Jul 30, 2023
 *      Author: msaad
 */

#ifndef INC_PLATFORM_H_
#define INC_PLATFORM_H_

#include <stdint.h>

// delay functions
void delay_ms(uint32_t ms);

// i2c basic functions
int write_i2c_reg(uint8_t addr, uint16_t reg, uint8_t value);
int read_i2c_reg(uint8_t addr, uint16_t reg, uint16_t len, uint8_t *buff);

// uart functions
void write_uart(uint8_t *buffer, uint16_t len);

// timing functions
uint16_t get_ticks_us();
uint32_t get_elapsed_us(uint16_t prev);

// LED functions
void led_on(unsigned led);
void led_off(unsigned led);
void led_toggle(unsigned led);

// interrupt functions
void disable_interrupts();

#endif /* INC_PLATFORM_H_ */

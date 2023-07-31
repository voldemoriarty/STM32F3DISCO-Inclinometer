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

#endif /* INC_PLATFORM_H_ */

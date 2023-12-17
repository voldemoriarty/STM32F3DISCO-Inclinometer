/*
 * lsm303lhc_driver.h
 *
 *  Created on: Jul 3, 2023
 *      Author: msaad
 */

#ifndef INC_I3G4250D_DRIVER_H_
#define INC_I3G4250D_DRIVER_H_

#include <stdint.h>

typedef enum I3G4250D_Error
{
    ERR_NONE_G = 0, ERR_ID_G = 1, ERR_WR_G = 3, ERR_RD_G = 6,
} I3G4250D_Error;

typedef struct I3G4250D_Readings
{
    int16_t gyro[3];
    int8_t  temp;
} I3G4250D_Readings;

I3G4250D_Error i3g4250d_init();
I3G4250D_Error i3g4250d_measure(I3G4250D_Readings *rd);

#endif /* INC_I3G4250D_DRIVER_H_ */

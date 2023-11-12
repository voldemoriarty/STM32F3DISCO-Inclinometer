#include "i3g4250d_driver.h"
#include "platform.h"
#include <stdio.h>

#define WHO_AM_I_G (0x0f)

I3G4250D_Error i3g4250d_init()
{
    uint8_t temp = 0;

    if (read_spi_reg(WHO_AM_I_G, 1, &temp) == 0)
        printf("WHO_AM_I: 0x%x\r\n", (int)temp);
    else
        printf("Error reading\r\n");

    while (1);

    return ERR_NONE_G;
}

I3G4250D_Error i3g4250d_measure(I3G4250D_Readings* rd)
{

}
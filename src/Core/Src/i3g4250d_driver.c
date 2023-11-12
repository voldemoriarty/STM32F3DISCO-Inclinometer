#include "i3g4250d_driver.h"
#include "platform.h"
#include <stdio.h>

#define READ_MASK       (0x80)
#define AUTO_INC        (0x40)

#define GYRO_WHOAMI     (0xd3)
#define REG_WHOAMI_G    (0x0f)
#define REG_CTRL_REG1   (0x20)
#define REG_CTRL_REG2   (0x21)
#define REG_CTRL_REG3   (0x22)
#define REG_CTRL_REG4   (0x23)
#define REG_CTRL_REG5   (0x24)
#define REG_OUT_TEMP    (0x26)
#define REG_OUT_X_L     (0x28)
#define REG_OUT_X_H     (0x29)
#define REG_OUT_Y_L     (0x2a)
#define REG_OUT_Y_H     (0x2b)
#define REG_OUT_Z_L     (0x2c)
#define REG_OUT_Z_H     (0x2d)

#define DR_SHIFT        (6)
#define DR_100HZ        (0b00 << DR_SHIFT)
#define DR_200HZ        (0b01 << DR_SHIFT)
#define DR_400HZ        (0b10 << DR_SHIFT)
#define DR_800HZ        (0b11 << DR_SHIFT)

#define BW_SHIFT        (4)
#define X_EN            (1)
#define Y_EN            (1 << 1)
#define Z_EN            (1 << 2)
#define ALL_EN          (X_EN | Y_EN | Z_EN)
#define NORMAL_MODE     ((1 << 3) | ALL_EN)

#define FS_SHIFT        (4)
#define FS_245dps       (0b00 << FS_SHIFT)
#define FS_500dps       (0b01 << FS_SHIFT)
#define FS_2000dps      (0b10 << FS_SHIFT)

static int read_spi_mem(uint8_t base, uint8_t* out, uint16_t len)
{
    uint8_t err = 0;

    base |= READ_MASK;
    base |= AUTO_INC;

    spi_select();
    err += spi_tx(&base, 1);
    err += spi_rx(out, len);
    spi_deselect();

    return err;
}

static int read_spi_reg(uint8_t reg, uint8_t* out)
{
    uint8_t err = 0;

    reg |= READ_MASK;
    spi_select();
    err += spi_tx(&reg, 1);
    err += spi_rx(out, 1);
    spi_deselect();
    
    return err;
}

static int write_spi_reg(uint8_t reg, uint8_t val)
{
    uint8_t err = 0;

    spi_select();
    err += spi_tx(&reg, 1);
    err += spi_tx(&val, 1);
    spi_deselect();

    return err;
}

static int write_spi_reg_v(uint8_t reg, uint8_t val)
{
    uint8_t err = 0, tmp = 0;

    err += write_spi_reg(reg, val);
    err += read_spi_reg(reg, &tmp);
    err += (tmp != val);

    return err;
}

I3G4250D_Error i3g4250d_init()
{
    uint8_t tmp = 0;

    read_spi_reg(REG_WHOAMI_G, &tmp);
    if (tmp != GYRO_WHOAMI) {
        return ERR_ID_G;
    }

    // CTRL 1 settings
    tmp = 0;
    tmp |= DR_100HZ;            // Data rate selection
    tmp |= 0b00 << BW_SHIFT;    // bandwidth selection (Table 22 in datasheet)
    tmp |= NORMAL_MODE;         // all axes enabled
    if (write_spi_reg_v(REG_CTRL_REG1, tmp)) {
        return ERR_WR_G;
    }

    // CTRL 2 settings
    tmp = 0;
    tmp |= 0b1001;              // HP filter bandwidth (lowest possible)
    if (write_spi_reg_v(REG_CTRL_REG2, tmp)) {
        return ERR_WR_G;
    }

    // CTRL 4 settings
    tmp = 0;
    tmp |= FS_245dps;              // Full Scale
    if (write_spi_reg_v(REG_CTRL_REG4, tmp)) {
        return ERR_WR_G;
    }

    // CTRL 5 settings
    tmp = 0;
    tmp |= 1 << 1;              // Enable lowpass filter (Section 7.6 datasheet)
    // tmp |= 1 << 4;              // Enable highpass filter
    if (write_spi_reg_v(REG_CTRL_REG5, tmp)) {
        return ERR_WR_G;
    }

    return ERR_NONE_G;
}

I3G4250D_Error i3g4250d_measure(I3G4250D_Readings* rd)
{
    int i;

    if (read_spi_reg(REG_OUT_TEMP, (uint8_t *) &rd->temp) != 0) {
        return ERR_RD_G;
    }

    if (read_spi_mem(REG_OUT_X_L, (uint8_t*)&rd->gyro, 6) != 0) {
        return ERR_RD_G;
    }

    for (i = 0; i < 3; ++i) {
        rd->gyro[i] = (float)rd->gyro[i] * 8.75f;  // Section 2.1 datasheet
    }

    return ERR_NONE_G;
}
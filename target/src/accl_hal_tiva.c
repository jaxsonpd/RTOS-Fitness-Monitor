/** 
 * @file accl_hal_tiva.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Hardware Abstraction Layer implementation for an I2C Accelerometer on TIVA
 */

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"

#include "i2c_driver.h"

#include "accl_hal.h"

/*
 * Accl Interrupt Pins
 */
#define ACCL_INT1Port       GPIO_PORTB_BASE
#define ACCL_INT2Port       GPIO_PORTE_BASE
#define ACCL_INT1           GPIO_PIN_4
#define ACCL_INT2           GPIO_PIN_4

#define ACCL                2
#define ACCL_ADDR           0x1D

#define ACCL_INT            0x2E
#define ACCL_OFFSET_X       0x1E
#define ACCL_OFFSET_Y       0x1F
#define ACCL_OFFSET_Z       0x20
#define ACCL_DATA_X0        0x32
#define ACCL_DATA_X1        0x33

#define ACCL_PWR_CTL        0x2D
// Parameters for ACCL_PWR_CTL:
#define ACCL_MEASURE        0x08

#define ACCL_DATA_FORMAT    0x31
// Parameters for ACCL_DATA_FORMAT:
#define ACCL_RANGE_2G       0x00
#define ACCL_RANGE_4G       0x01
#define ACCL_RANGE_8G       0x02
#define ACCL_RANGE_16G      0x03
#define ACCL_FULL_RES       0x08
#define ACCL_JUSTIFY        0x04

#define ACCL_BW_RATE        0x2C
// Parameters for ACCL_BW_RATE:
#define ACCL_RATE_3200HZ    0x0F
#define ACCL_RATE_1600HZ    0x0E
#define ACCL_RATE_800HZ     0x0D
#define ACCL_RATE_400HZ     0x0C
#define ACCL_RATE_200HZ     0x0B
#define ACCL_RATE_100HZ     0x0A
#define ACCL_RATE_50HZ      0x09
#define ACCL_RATE_25HZ      0x08
#define ACCL_RATE_12_5HZ    0x07
#define ACCL_RATE_6_25HZ    0x06
#define ACCL_RATE_3_13HZ    0x05
#define ACCL_RATE_1_56HZ    0x04
#define ACCL_RATE_0_78HZ    0x03
#define ACCL_RATE_0_39HZ    0x02
#define ACCL_RATE_0_20HZ    0x01
#define ACCL_RATE_0_10HZ    0x00

// Init the accl chip via I2C
void accl_chip_init(void)
{
    char    toAccl[] = {0, 0};  // parameter, value

    /*
    * Enable I2C Peripheral
    */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    /*
    * Set I2C GPIO pins
    */
    GPIOPinTypeI2C(I2CSDAPort, I2CSDA_PIN);
    GPIOPinTypeI2CSCL(I2CSCLPort, I2CSCL_PIN);
    GPIOPinConfigure(I2CSCL);
    GPIOPinConfigure(I2CSDA);

    /*
    * Setup I2C
    */
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);

    GPIOPinTypeGPIOInput(ACCL_INT2Port, ACCL_INT2);

    //Initialize ADXL345 Acceleromter

    // set +-16g, 13 bit resolution, active low interrupts
    toAccl[0] = ACCL_DATA_FORMAT;
    toAccl[1] = (ACCL_RANGE_16G | ACCL_FULL_RES);
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_PWR_CTL;
    toAccl[1] = ACCL_MEASURE;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_BW_RATE;
    toAccl[1] = ACCL_RATE_100HZ;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_INT;
    toAccl[1] = 0x00;       // Disable interrupts from accelerometer.
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_X;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_Y;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_Z;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);
}

// Read the accl chip
void accl_data_get(int16_t* acceleration)
{
    char    fromAccl[] = {0, 0, 0, 0, 0, 0, 0}; // starting address, placeholders for data to be read.
    uint8_t bytesToRead = 6;

    fromAccl[0] = ACCL_DATA_X0;
    I2CGenTransmit(fromAccl, bytesToRead, READ, ACCL_ADDR);

    acceleration[0] = (fromAccl[2] << 8) | fromAccl[1]; // Return 16-bit acceleration readings.
    acceleration[1] = (fromAccl[4] << 8) | fromAccl[3];
    acceleration[2] = (fromAccl[6] << 8) | fromAccl[5];
}
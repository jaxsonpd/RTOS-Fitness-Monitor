/** 
 * @file i2c_hal_tiva.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Hardware Abstraction Layer implementation for TIVA I2C peripheral
 */
#include <stdint.h>
#include <stdbool.h>

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/i2c.h"
#include "i2c_hal.h"

#define I2CPort         GPIO_PORTB_BASE
#define I2CSDAPort      GPIO_PORTB_BASE
#define I2CSCLPort      GPIO_PORTB_BASE
#define I2CSDA_PIN      GPIO_PIN_3
#define I2CSCL_PIN      GPIO_PIN_2
#define I2CSCL          GPIO_PB2_I2C0SCL
#define I2CSDA          GPIO_PB3_I2C0SDA
#define READ            1
#define WRITE           0

void wait_us(void)
{
    int16_t i=0;
    for (i=0;i<1000;i++)
        ;
}

void i2c_hal_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    GPIOPinTypeI2C(I2CSDAPort, I2CSDA_PIN);
    GPIOPinTypeI2CSCL(I2CSCLPort, I2CSCL_PIN);
    GPIOPinConfigure(I2CSCL);
    GPIOPinConfigure(I2CSDA);

    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);
}

char i2c_hal_write(char *data, int32_t size, char addr) {
    int32_t i;
    char *temp;
    temp = data;

    I2CMasterSlaveAddrSet(I2C0_BASE, addr, WRITE);
    I2CMasterDataPut(I2C0_BASE, *temp);

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    wait_us();;

    while(!I2CMasterBusBusy(I2C0_BASE));

    temp++;

    for(i = 0; i < size; i++) {

        I2CMasterDataPut(I2C0_BASE, *temp);

        while(I2CMasterBusy(I2C0_BASE));

        if(i == size - 1) {
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

            wait_us();;
            while(I2CMasterBusy(I2C0_BASE));
        }
        else {
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

            wait_us();;
            while(I2CMasterBusy(I2C0_BASE));

            while(!I2CMasterBusBusy(I2C0_BASE));
        }

        temp++;
    }
    return 0x00;

}

char i2c_hal_read(char *data, int32_t size, char addr) {
    int32_t i;
    char *temp;
    temp = data;

    I2CMasterSlaveAddrSet(I2C0_BASE, addr, WRITE);
    I2CMasterDataPut(I2C0_BASE, *temp);

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    wait_us();;

    while(!I2CMasterBusBusy(I2C0_BASE));

    temp++;

    I2CMasterSlaveAddrSet(I2C0_BASE, addr, READ);

    while(I2CMasterBusy(I2C0_BASE));

    for(i = 0; i < size; i++) {

        if(size == i + 1 && size == 1) {
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

            wait_us();;
            while(I2CMasterBusy(I2C0_BASE));
        }
        else if(size == i + 1 && size > 1) {
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

            wait_us();;
            while(I2CMasterBusy(I2C0_BASE));
        }
        else if(i == 0) {
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);

            wait_us();;
            while(I2CMasterBusy(I2C0_BASE));

            while(!I2CMasterBusBusy(I2C0_BASE));
        }
        else {
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);

            wait_us();;
            while(I2CMasterBusy(I2C0_BASE));

            while(!I2CMasterBusBusy(I2C0_BASE));
        }

        while(I2CMasterBusy(I2C0_BASE));

        *temp = (char)I2CMasterDataGet(I2C0_BASE);

        temp++;

    }
    return 0x00;

}
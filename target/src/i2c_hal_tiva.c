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

#define READ            1
#define WRITE           0

typedef struct {
    uint32_t sysctlPeriphGPIO;
    uint32_t sysctlPeriphI2C;
    uint32_t gpioBaseAddress;
    uint32_t i2cBaseAddress;
    uint8_t sdaPin;
    uint8_t sclPin;
    uint32_t sdaConfig;
    uint32_t sclConfig;
} i2c_config_t;

static i2c_config_t i2c_configurations[MAX_I2C_IDS] = {
    {SYSCTL_PERIPH_GPIOB, SYSCTL_PERIPH_I2C0, GPIO_PORTB_BASE, I2C0_BASE, GPIO_PIN_3, GPIO_PIN_2, GPIO_PB3_I2C0SDA, GPIO_PB2_I2C0SCL}
};

void wait_us(void)
{
    int16_t i=0;
    for (i=0;i<1000;i++)
        ;
}

void i2c_hal_init(uint8_t id)
{
    if (id >= MAX_I2C_IDS) {
        return;
    }
    i2c_config_t config = i2c_configurations[id];

    SysCtlPeripheralEnable(config.sysctlPeriphGPIO);
    SysCtlPeripheralEnable(config.sysctlPeriphI2C);
    SysCtlPeripheralReset(config.sysctlPeriphI2C);

    GPIOPinTypeI2C(config.gpioBaseAddress, config.sdaPin);
    GPIOPinTypeI2CSCL(config.gpioBaseAddress, config.sclPin);
    GPIOPinConfigure(config.sdaConfig);
    GPIOPinConfigure(config.sclConfig);

    I2CMasterInitExpClk(config.i2cBaseAddress, SysCtlClockGet(), true);
}

char i2c_hal_write(uint8_t id,char *data, int32_t size, char addr) {
    i2c_config_t config = i2c_configurations[id];

    I2CMasterSlaveAddrSet(config.i2cBaseAddress, addr, WRITE);
    I2CMasterDataPut(config.i2cBaseAddress, *data);
    I2CMasterControl(config.i2cBaseAddress, I2C_MASTER_CMD_BURST_SEND_START);
    wait_us();
    while(!I2CMasterBusBusy(config.i2cBaseAddress));
    data++;

    for(int32_t i = 0; i < size; i++) {
        I2CMasterDataPut(config.i2cBaseAddress, *data);
        while(I2CMasterBusy(config.i2cBaseAddress));
        if(i == size - 1) {
            I2CMasterControl(config.i2cBaseAddress, I2C_MASTER_CMD_BURST_SEND_FINISH);
            wait_us();
            while(I2CMasterBusy(config.i2cBaseAddress));
        }
        else {
            I2CMasterControl(config.i2cBaseAddress, I2C_MASTER_CMD_BURST_SEND_CONT);
            wait_us();
            while(I2CMasterBusy(config.i2cBaseAddress));
            while(!I2CMasterBusBusy(config.i2cBaseAddress));
        }
        data++;
    }
    return 0x00;
}

char i2c_hal_read(uint8_t id, char *data, int32_t size, char addr) {
    i2c_config_t config = i2c_configurations[id];
    I2CMasterSlaveAddrSet(config.i2cBaseAddress, addr, WRITE);
    I2CMasterDataPut(config.i2cBaseAddress, *data);
    I2CMasterControl(config.i2cBaseAddress, I2C_MASTER_CMD_BURST_SEND_START);
    wait_us();
    while(!I2CMasterBusBusy(config.i2cBaseAddress));
    data++;
    I2CMasterSlaveAddrSet(config.i2cBaseAddress, addr, READ);
    while(I2CMasterBusy(config.i2cBaseAddress));

    for(int32_t i = 0; i < size; i++) {
        if(size == i + 1 && size == 1) {
            I2CMasterControl(config.i2cBaseAddress, I2C_MASTER_CMD_SINGLE_RECEIVE);
            wait_us();
            while(I2CMasterBusy(config.i2cBaseAddress));
        }
        else if(size == i + 1 && size > 1) {
            I2CMasterControl(config.i2cBaseAddress, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
            wait_us();
            while(I2CMasterBusy(config.i2cBaseAddress));
        }
        else if(i == 0) {
            I2CMasterControl(config.i2cBaseAddress, I2C_MASTER_CMD_BURST_RECEIVE_START);
            wait_us();
            while(I2CMasterBusy(config.i2cBaseAddress));
            while(!I2CMasterBusBusy(config.i2cBaseAddress));
        }
        else {
            I2CMasterControl(config.i2cBaseAddress, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
            wait_us();
            while(I2CMasterBusy(config.i2cBaseAddress));
            while(!I2CMasterBusBusy(config.i2cBaseAddress));
        }

        while(I2CMasterBusy(config.i2cBaseAddress));
        *data = (char)I2CMasterDataGet(config.i2cBaseAddress);
        data++;
    }
    return 0x00;
}
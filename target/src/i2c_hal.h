/** 
 * @file i2c_hal_tiva.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Hardware Abstraction Layer interface for an I2C peripheral
 */

#ifndef I2C_HAL_H_
#define I2C_HAL_H_

void i2c_hal_init(void);

char i2c_hal_write(char *data, int32_t size, char addr);

char i2c_hal_read(char *data, int32_t size, char addr);

#endif // I2C_HAL_H_
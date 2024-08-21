/**
 * @file i2c_hal_tiva.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief Hardware Abstraction Layer interface for an I2C peripheral
 */

#ifndef I2C_HAL_H_
#define I2C_HAL_H_

typedef enum {
    I2C_ID_0 = 0,
    MAX_I2C_IDS
} adcID_t;

/**
 * @brief initialise i2c peripheral
 *
 * @param id specific peripheral to initialise
 */
void i2c_hal_init(uint8_t id);

/**
 * @brief send size amount of data on id over i2c to addr
 *
 * @param id i2c bus to send over
 * @param data array of characters to send
 * @param size length of array to send
 * @param addr address of peripheral to send to
 */
char i2c_hal_write(uint8_t id, char* data, int32_t size, char addr);

/**
 * @brief receive size amount of data on id over i2c from addr
 *
 * @param id i2c bus to receive over
 * @param data array of characters to receive
 * @param size length of array to receive
 * @param addr address of peripheral to receive from
 */
char i2c_hal_read(uint8_t id, char* data, int32_t size, char addr);

#endif // I2C_HAL_H_
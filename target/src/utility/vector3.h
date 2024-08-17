/** 
 * @file vector3.h
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief 3D vector library to encapsulate type and type-specific operations
 */

#ifndef VECTOR3_T_H_
#define VECTOR3_T_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} vector3_t;

vector3_t v3_constructor(int16_t x, int16_t y, int16_t z);

uint32_t v3_magnitude(vector3_t v);

#endif //VECTOR3_T_H_
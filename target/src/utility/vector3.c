/** 
 * @file vector3.c
 * @author Isaac Cone (ico29@uclive.ac.nz)
 * @date 2024-08
 * @brief 3D vector library to encapsulate type and type-specific operations
 */

#include "vector3.h"

#include "math.h"

vector3_t v3_constructor(int16_t x, int16_t y, int16_t z) {
    vector3_t v3 = {x,y,z};
    return v3;
}

uint32_t v3_magnitude(vector3_t v3) {
    uint32_t sum = v3.x * v3.x + v3.y * v3.y + v3.z * v3.z;
    uint32_t magnitude = sqrt(sum);
    return magnitude;
}


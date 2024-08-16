/** 
 * @file stopwatch.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-14
 * @brief Stopwatch screen which allows for arbritray timing
 */


#ifndef STOPWATCH_H
#define STOPWATCH_H


#include <stdint.h>
#include <stdbool.h>

/** 
 * @brief Update the stopwatch screen/display
 * @param reset true if the stopwatch display should be reset
 * 
 */
void stopwatch_display(bool reset);


#endif // STOPWATCH_H
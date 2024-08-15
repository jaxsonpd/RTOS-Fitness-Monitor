/**
 * @file display_hal.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-11
 * @brief Declarations for the functions to write to a 16 character width display
 */


#ifndef DISPLAY_H
#define DISPLAY_H


#include <stdint.h>
#include <stdbool.h>

/// Max number of chars that can be displayed in a line
#define DISPLAY_WIDTH 16 

/// The different text alignments that can be displayed
typedef enum {
    ALIGN_LEFT = 0,
    ALIGN_CENTRE,
    ALIGN_RIGHT,
} textAlignment_t;


/** 
 * @brief Initialise the display functionality
 * 
 * @return true if successful
 */
bool display_init(void);


/** 
 * @brief Draw a line to the OLED display
 * @param inStr the string to display
 * @param row the row to display on
 * @param alignment the alignment of the text
 * 
 */
void display_line(char* inStr, uint8_t row, textAlignment_t alignment);

/** 
 * @brief Display a value with a prefix and a suffix with the ability to 
 * display 1000s floats
 * @param prefix text to place before value
 * @param suffix test to place after the value
 * @param row the row to place the text on
 * @param alignment the alignment of the text
 * @param formateThousands wether to display as a float
 * 
 */
void display_value(char* prefix, char* suffix, int32_t value, uint8_t row, textAlignment_t alignment, bool thousandsFormatting);

/** 
 * @brief Display a given number of seconds as ether mm:ss or hh:mm:ss
 * @param prefix Text to display before the time
 * @param time The time to display in seconds
 * @param row The row to display on
 * @param alignment how to align the text
 * 
 */
void display_time(char* prefix, uint16_t time, uint8_t row, textAlignment_t alignment);

#endif // DISPLAY_H
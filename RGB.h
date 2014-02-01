#ifndef RGB_H__
#define RGB_H__

#include "Arduino.h"

/**
* RGB container
* @param r Red channel intensity
* @param g Green channel intensity
* @param b Blue channel intensity
*/
struct RGB{
	byte r;
	byte g;
	byte b;
};

/**
* Indexes for the COLOURS array.
* Each array entry is mapped to a worded index  
*/
enum COLOUR_INDEXES{
	OFF = 0,
	RED = 1,
	GREEN = 2,
	BLUE = 3,
	WHITE = 4,
	YELLOW = 5,
	CYAN = 6,
	MAGENTA = 7,
	ORANGE = 8,
	PURPLE = 9
};

/**
* A colour palette containing RGB codes for several colours
* The colour index is given by COLOUR_INDEXES   
*/
const RGB COLOURS[] = {
	{0,0,0},	// OFF
	{255,0,0},	// RED
	{0,255,0},	// GREEN
	{0,0,255},	// BLUE
	{255,255,255},	// WHITE
	{255,255,0},	// YELLOW
	{0,255,255},	// CYAN
	{255,0,255},	// MAGENTA
	{255,128,0},	// ORANGE
	{128,0,255}		// PURPLE
};

/**
* Maps the colours found in the COLOURS RGB array to single char entries  
*/
const String COLOUR_MAP = "zrgbwycmop";


#endif

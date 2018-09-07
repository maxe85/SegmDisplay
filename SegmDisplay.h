/*
 * arduino library to control 7-segm-display with a 74HC164 shift register
 * multiplexing over 3 digits using the overflow interrupt at TC2
 * written for NanoKidsShield (rev. 180412 by phantomix)  by maxe85
 */
#include "Arduino.h"
#ifndef SEGM_DISPLAY_H
#define SEGM_DISPLAY_H

// pins for display transistors, ceep your hardware sorted!
#define DISPLAY0 2 
#define DISPLAY1 3
#define DISPLAY2 4 
// ...

// pins for shift register
#define DAT 8
#define CLK 9

#define NUM_DISPLAYS 3
// modify type of _display_value an related methodes if your'r using more than 4 displays

class Segm
{
public:
	Segm(bool invert = false);              // constructor, use before Setup() true for common anode, false for common cathode
	void begin();                   		// use this in Setup();
	void set(int16_t value, uint8_t leadingZeros = 0);        // sets an int to output
	void set(float input);	        //sets a float, depends on the set(int16_t)
	void handleInterrupt();         // the backbone of this object, as user you can ignore it
	void brightness(uint8_t _b);    // set brightness level 0 - 15, will use %16 if higher
	void clear();                   // makes display blank
	void end();                     // ~ and disables interrupt, restart with begin
	void _convert(uint32_t input);  // sets _desplay_value (bitmap for max 4 digits) and recalculate bit position according to _convert_mask
	uint32_t _display_value = 0;    // the other backbone of this object, as advanced user: play with it!
private:
	uint8_t     _n = 0;             // display index
	uint8_t     _dimm = 0;          // empty steps for multiplexing, brightness(15) sets it to 0
	const bool  _invert;            // see constructor
};



	const uint8_t _segm_mask[] = {  // bitmaps for digits
		B00111111, B00000110, B01011011, B01001111, // 0, 1, 2, 3,
		B01100110, B01101101, B01111101, B00000111, // 4, 5, 6, 7,
		B01111111, B01101111, B01110111, B01111100, // 8, 9, A, b,
		B01011000, B01011110, B01111001, B01110001  // c, d, E, F
        };
    const uint8_t _convert_mask [] = {6,7,5,4,2,1,0,3 }; // lookup table for wiring
        // configuration at NanoKidsShield rev. 180412 by phantomix:
        //    - 6 -
        //   |     |
        //   1     7        
        //   |     |
        //    - 0 - 
        //   |     |
        //   2     5
        //   |     |
        //    - 4 -  .3


ISR(TIMER2_OVF_vect);

#endif


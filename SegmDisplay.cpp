#include "SegmDisplay.h"


Segm *_7_segm_ptr = 0;


Segm::Segm(bool invert) :
_invert(invert) {}

void Segm::begin()
{
    _7_segm_ptr = this; // set the global pointer to itself

	pinMode(DAT,OUTPUT);
	pinMode(CLK,OUTPUT);
	pinMode(DISPLAY0,OUTPUT);
	pinMode(DISPLAY1,OUTPUT);
	pinMode(DISPLAY2,OUTPUT);
	TCCR2A  = 0x00; // normal timer mode
	TCCR2B  = 0x04; // set the prescaler to 64 (Arduino default)
	TIMSK2 |= 0x01; // ovf interrupt enable
	sei();
}

void Segm::set(int16_t input, uint8_t leadingZeros)
{
    uint32_t _value = 0x00UL; // raw display data 4x8bit
/*
   if  ( abs(input)>= pow(10,NUM_DISPLAYS) )
   {
	   input = pow(10,NUM_DISPLAYS)-1;
   }
*/
    for (uint8_t i=0; i<NUM_DISPLAYS; i++) 
    {   
        _value |= uint32_t(_segm_mask[ input%10 ]) << (i*8) ;
        input /= 10;
        if ( input == 0 && leadingZeros<=i)
        {
            break;
        }
    }
    _convert(_value); // convert _value to _display_value
}

void Segm::set(float input)
{
	if (input >= 100)
	{
		set(int16_t(input));	// set as int
		_display_value |= 1UL<<3;  // set the first point (from right)
	}
	else if (input >= 10 || input == 0)
	{
		set(int16_t(input*10), 1);	// set as int
		_display_value |= 1UL<<(3+8);	// set the second point
	}
	else
	{
		set(int16_t(input*100),2);		// set as int
		_display_value |= 1UL<<(3+16);	// set the third point
	}
}

void Segm::_convert(uint32_t input)
{
    _display_value = 0x00UL; // reset _display_value
    for (uint8_t i=0; i< 32; i++)
    {
        if ( input&(0x01UL<< i) ) // if bit/segm is set
        {
            _display_value |= 1UL<<( (i&0xF8) + _convert_mask[i%8] );
        }
    }
}

void Segm::end()
{
    if ( _n < NUM_DISPLAYS )
    {    
        digitalWrite(DISPLAY0 + _n, _invert); // disable current display
    }
    TIMSK2 &= ~0x01; // unset the overflow interrupt flag
}

void Segm::clear()
{
    _display_value = 0x00UL;
}

void Segm::handleInterrupt()
{
    if ( _n < NUM_DISPLAYS )  // disable previous display pin
    {    
        digitalWrite(DISPLAY0 + _n, _invert);
    }

    _n = ( _n > (NUM_DISPLAYS -1 +_dimm) ) ? 0 : _n+1; // increment display index
    
    if ( _n < NUM_DISPLAYS ) // set next display
    {    
        for (uint8_t i = 0; i < 8; i++) // shift out eight bit
        {
            digitalWrite( DAT, (_display_value>>(_n*8+i)&1)^_invert );
            digitalWrite(CLK,1);
            digitalWrite(CLK,0);
        }
        digitalWrite(DISPLAY0 + _n, !_invert); // enable display pin
    }
}


void Segm::brightness(uint8_t _b)
{
    _dimm = 15-(_b&0x0f);
}

ISR(TIMER2_OVF_vect)
{
    cli();
    if (_7_segm_ptr != 0) // if pointer to Segm object is set
    {
        _7_segm_ptr->handleInterrupt(); // run Segm methode
    }
    sei();
}

#include "SegmDisplay.h" // is still the working title, ...

// class name is 'Segm' so call an instance of that class:
Segm display(0); // initialize display (as none invert mode)


void setup()
{
    display.begin(); // initialize ovf interrupt at TIC2 and sets the null pointer to this object

    display.set(0); // int zero
    delay(2000);
    display.set((float)0); // float zero
    delay(2000);
    display.set((float)0.000001); // float, too small to display
    delay(2000);
    display.set((float)9.8); // more floats
    delay(1000);
    display.set((float)9.901); // don't ask me ... this is float
    delay(1000);
    display.set((float)10);
    delay(1000);
    display.set((float)10.1);
    delay(1000);
    display.set((float)0.2);
    delay(1000);
    display.set((float)0.042);
    delay(1000);
    display.set((float)99.9);
    delay(1000);
    display.set((float)100);
    delay(1000);
}

void loop()
{ 
    display.set((int) (millis()/1000));
    delay(1000);
}

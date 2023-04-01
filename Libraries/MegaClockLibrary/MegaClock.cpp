#include "MegaClock.h"



void Activate_Internal_Clock()
{
    //CODE NOT NEEDED DUE TO EXTERNAL CLOCK ALSO DOESNT WORK ON MKR, ONLY WORKS WITH MEGA CHIPS.

    int clock = 9;
    pinMode(clock, OUTPUT);
    TCCR1B = (TCCR1B & 0xF8) | 1 ; //generates the MCKL signal
    analogWrite (clock, 128) ;

}

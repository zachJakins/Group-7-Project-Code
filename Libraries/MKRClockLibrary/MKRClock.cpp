#include "MKRClock.h"


void Activate_Internal_Clock_Zero()
{
    GCLK->GENDIV.reg = GCLK_GENDIV_DIV(6) |         // Divide the 48MHz clock source by divisor 6: 48/6=8MHz
                       GCLK_GENDIV_ID(4);           // Select Generic Clock (GCLK) 4
    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |          // Set the duty cycle to 50/50 HIGH/LOW
                        GCLK_GENCTRL_GENEN |        // Enable GCLK4
                        GCLK_GENCTRL_SRC_DFLL48M |  // Set the 48MHz clock source
                        GCLK_GENCTRL_ID(4);         // Select GCLK4
    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    // Feed GCLK4 to TCC0 and TCC1
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TCC0 and TCC1
                        GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
                        GCLK_CLKCTRL_ID_TCC0_TCC1;   // Feed GCLK4 to TCC0 and TCC1
    while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

    // Enable the port multiplexer for the TCC0 PWM channel 3 (digital pin D7), SAMD21 pin PA21
    PORT->Group[g_APinDescription[7].ulPort].PINCFG[g_APinDescription[7].ulPin].bit.PMUXEN = 1;

    // Connect the TCC0 timer to the port outputs - port pins are paired odd PMUO and even PMUXE
    // F & E specify the timers: TCC0, TCC1 and TCC2
    PORT->Group[g_APinDescription[7].ulPort].PMUX[g_APinDescription[7].ulPin >> 1].reg |= /*PORT_PMUX_PMUXO_F |*/ PORT_PMUX_PMUXO_F;

    // Normal (single slope) PWM operation: timer countinuouslys count up to PER register value and then is reset to 0
    TCC0->WAVE.reg |= TCC_WAVE_WAVEGEN_NPWM;        // Setup single slope PWM on TCC0
    while (TCC0->SYNCBUSY.bit.WAVE);                // Wait for synchronization

    // Each timer counts up to a maximum or TOP value set by the PER register,
    // this determines the frequency of the PWM operation:
    TCC0->PER.reg = 5;                              // Set the frequency of the PWM on TCC0 to 8MHz
    while(TCC0->SYNCBUSY.bit.PER);                  // Wait for synchronization

    // The CCx register value corresponds to the pulsewidth in microseconds (us)
    TCC0->CC[3].reg = 3;                            // TCC0 CC3 - 50% duty cycle on D7
    while(TCC0->SYNCBUSY.bit.CC3);                  // Wait for synchronization


    // Divide 8MHz by 256 giving 31.25kHz timer tick and enable timer TCC0
    TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV256;    // Divide GCLK4 by 256
    TCC0->CTRLA.bit.ENABLE = 1;                     // Enable the TCC0 output
    while (TCC0->SYNCBUSY.bit.ENABLE);              // Wait for synchronization

}

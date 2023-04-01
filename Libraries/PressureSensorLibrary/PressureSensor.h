#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

#include <Arduino.h>
#include <SPI.h>

double Pressure_Sensor_Measure_mBar(); //Requires either an external clock to our pressure sensor OR requires the internal clock to be activated at 32.768kHz

//MOSI Pin 11 (Master out slave in aka DIN)
//MISO Pin 12 (Master in slave out aka DOUT)
//SCLK Pin 13
//MCLK Pin 9 (or external oscillator)

//SS or CS unused (pin 10)

//3V is 3V
//GND is GND

#endif

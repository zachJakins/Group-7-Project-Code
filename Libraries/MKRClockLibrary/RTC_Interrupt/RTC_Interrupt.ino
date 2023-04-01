#include <Arduino.h>
#include <Wire.h>
#include <Time.h>
#include "GravityRtc.h"
#define RTC_ADDR 0x32


GravityRtc rtc;
void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(10,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(10),clocked,FALLING);
  rtc.setup();
}

void loop() {

  Wire.beginTransmission(byte(0x32));

  Wire.write(byte(0x0E));//Alarm Enable register
  Wire.write(byte(0x01));

  Wire.write(byte(0x07));//second alarm register
  Wire.write(byte(0x20));

/*
  Wire.beginTransmission(byte(0x32));

  Wire.write(byte(0x0F));//CTR1 Register
  Wire.write(byte(0x00));

  Wire.endTransmission();
*/


  Wire.write(byte(0x10));//CTR2 Register
  Wire.write(byte(0x52));

  Wire.endTransmission();
/*
  Wire.beginTransmission(byte(0x32));

  Wire.write(byte(0x11));//CTR3 Reg
  Wire.write(byte(0x00));


  Wire.endTransmission();
*/
  Serial.println("Done");

  while (1)
  {
    rtc.read();
    Serial.print(rtc.hour);
    Serial.print(" ");
    Serial.print(rtc.minute);
    Serial.print(" ");
    Serial.println(rtc.second);
    delay(100);
  }

}

void clocked()
{
  Serial.println("CLOCK");
}

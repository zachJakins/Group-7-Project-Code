#include <Wire.h>
#include <DS3231.h>
#include <PressureSensor.h>

DS3231 clock;

void setup() {
  //begin peripherals
  Serial.begin(9600);
  Serial.println("BEGIN:");

  clock.begin();

  pinMode(6, INPUT_PULLUP);  //Interrupt in with pullup -> needed for RTC

  //reset alarms
  clock.armAlarm1(false);
  clock.armAlarm2(false);
  clock.clearAlarm1();
  clock.clearAlarm2();

  clock.enable32kHz(true);                //turn on the 32kHz 32K
  clock.enableOutput(false);              //disable oscilator output SQW
  clock.setDateTime(__DATE__, __TIME__);  //sync RTC to PC



  for (int i = 0; i < 100; i++)  //loop 100 times
  {
    //print pressure stuff
    Serial.println(Pressure_Sensor_Measure_mBar());
    delay(50);
  }
  Serial.println("FINISHED");
}

void loop() {
}

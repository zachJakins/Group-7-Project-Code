#include <Wire.h>
#include <DS3231.h>
#include <PressureSensor.h>
#include <SD.h>

DS3231 clock;
RTCDateTime dt;
boolean measure = false;

void setup() {
  //begin peripherals
  Serial.begin(9600);

  clock.begin();
  SD.begin(10);

  pinMode(6, INPUT_PULLUP); //Interrupt in with pullup -> needed for RTC

  attachInterrupt(digitalPinToInterrupt(6), alarmInt, FALLING); //Interrupt
  //pinMode(10,INPUT_PULLDOWN);


  //reset alarms
  clock.armAlarm1(false);
  clock.armAlarm2(false);
  clock.clearAlarm1();
  clock.clearAlarm2();

  clock.enable32kHz(true);//turn on the 32kHz 32K
  clock.enableOutput(false);//disable oscilator output SQW
  clock.setDateTime(__DATE__, __TIME__);//sync RTC to PC

  clock.setAlarm1(0, 0, 0, 30, DS3231_EVERY_SECOND); //Set Alarm1 output to SQW

}

void alarmInt()
{

  //proof of interrupt
  Serial.println("INT 0");
  measure = true;
  clock.clearAlarm1();
}

void loop() {
  //tell me current time
  dt = clock.getDateTime();
  Serial.print(dt.hour); Serial.print(":"); Serial.print(dt.minute); Serial.print(":"); Serial.println(dt.second);

  measure=true;
  if (measure)
  {
    
    //print pressure stuff
    Serial.print("Pressure (mBar): ");
    Serial.println(Pressure_Sensor_Measure_mBar());//Takes integer argument to decide how many measurements the sensor will take before returning mean.

    measure = false;
  }

  delay(500);

}

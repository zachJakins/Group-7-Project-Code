
#include <Wire.h>
#include <DS3231.h>

DS3231 clock;
boolean isAlarm = false;
RTCDateTime dt;

void setup()
{
  Serial.begin(9600);

  // Initialize DS3231
  Serial.println("Initialize DS3231");;
  clock.begin();
  pinMode(6,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);

  clock.armAlarm1(false);
  clock.armAlarm2(false);
  clock.clearAlarm1();
  clock.clearAlarm2();

  attachInterrupt(digitalPinToInterrupt(6), alarmFunction, FALLING);

  // disable 32kHz 
  clock.enable32kHz(true);
  clock.enableOutput(false);

  // Manual (Year, Month, Day, Hour, Minute, Second)
  clock.setDateTime(__DATE__, __TIME__);

  // Set Alarm1 - Every 1s in each minute
  // setAlarm1(Date or Day, Hour, Minute, Second, Mode, Armed = true)
  clock.setAlarm1(0, 0, 0,30, DS3231_EVERY_SECOND);
  //clock.setAlarm2(0, 0, 0,0, DS3231_EVERY_MINUTE);
  
}

void alarmFunction()
{
  Serial.println("INT 0");
  isAlarm = true;
}


void loop()
{
    dt = clock.getDateTime();
  Serial.println(dt.second);

    if (isAlarm)
  {
    clock.clearAlarm1();
    clock.clearAlarm2();
  } 
     delay(100);
}

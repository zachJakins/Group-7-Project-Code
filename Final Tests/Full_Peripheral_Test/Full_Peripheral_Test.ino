//#include <ArduinoLowPower.h>
#include <Wire.h>
#include <SD.h>
#include <DS3231.h>
#include <PressureSensor.h>
#include <TOF_Sensor.h>
#include <temp_sensor.h>


DS3231 clock;
RTCDateTime dt;
int Data = 3;
float humidity;
float temperature;
double pressure;
double distance;
boolean state = true;
int sample = 0;
boolean measure = false;
RTCAlarmTime alarm;

void setup() {
  //begin peripherals
  Serial.begin(9600);
  Serial.println("Life");
  clock.begin();
  SD.begin(10);

  pinMode(6, INPUT_PULLUP); //Interrupt in with pullup -> needed for RTC


  attachInterrupt(digitalPinToInterrupt(6), alarmInt, FALLING); //Interrupt

  //reset alarms
  clock.armAlarm1(false);
  clock.armAlarm2(false);
  clock.clearAlarm1();
  clock.clearAlarm2();

  clock.enable32kHz(false);//turn off the 32kHz 32K
  clock.enableOutput(false);//disable oscilator output SQW
  
  clock.setDateTime(__DATE__, __TIME__);//sync RTC to PC

  clock.setAlarm1(0, 0, 0, 30, DS3231_MATCH_S); //Set Alarm1 output to SQW

}

void alarmInt()
{
  Serial.println("INT");
  //clear alarm
  clock.clearAlarm1();
  measure = true;

/*
  //Not entirely necessary but shows an implementation of how we can do an alarm of <1 minute
  alarm = clock.getAlarm1();
  switch (alarm.second)
  {
    case 0:
      clock.setAlarm1(0, 0, 0, 15, DS3231_MATCH_S); //Set Alarm1 output to SQW
      break;
    case 15:
      clock.setAlarm1(0, 0, 0, 30, DS3231_MATCH_S); //Set Alarm1 output to SQW
      break;
    case 30:
      clock.setAlarm1(0, 0, 0, 45, DS3231_MATCH_S); //Set Alarm1 output to SQW
      break;
    case 45:
      clock.setAlarm1(0, 0, 0, 0, DS3231_MATCH_S); //Set Alarm1 output to SQW
      break;
  }
  */
}

void loop() {
  measure=true;
  if (measure) // THIS IS NECESSARY BECAUSE THE PRESSURE SENSOR MUST BE SYNCED TO 32K OSCILLATOR WHICH IS ONLY POSSIBLE IF IT RUNS IN "LOOP" NOT THE INTERRUPT
  {
    clock.enable32kHz(true);//turn on the 32kHz 32K

    //take measurements
    pressure = Pressure_Sensor_Measure_mBar();
    dt = clock.getDateTime();
    temp_data(Data, humidity, temperature);
    distance = TOF_Sensor_Distance_Measure_MM();

    //write to SD
    logData(sample, distance, humidity, temperature, pressure, dt);
    //increase iteration
    sample++;

    measure = false;//reset measure
    
      //print peripheral stuff

      Serial.print(dt.hour);Serial.print(":");Serial.print(dt.minute);Serial.print(":");Serial.println(dt.second);

      Serial.print("Pressure (mBar): ");
      Serial.println(pressure);
      Serial.print("Distance(mm): ");
      Serial.println(distance);
      Serial.print("Temperature(C): ");
      Serial.println(temperature);
      Serial.print("Humidity(%): ");
      Serial.println(humidity);
      Serial.println();
    
    clock.enable32kHz(false);//turn off the 32kHz 32K to save power
  }

  Serial.print(dt.hour);Serial.print(":");Serial.print(dt.minute);Serial.print(":");Serial.println(dt.second);
  delay(1000);
  //sleep
  //LowPower.deepSleep();
}


void logData(int sampleNumber, double height, double hum, double temp, double pressure, RTCDateTime clockData) {


  String dataString = "";                                 // Make a string for assembling the data to log:
  dataString += "N: " + String(sampleNumber)  // Add sample number
                + " T: " + String(dt.day)                   // Add date measurement
                + " " + String(dt.hour)                   // Add hour measurement
                + ":" + String(dt.minute)                   // Add minute measurement
                + ":" + String(dt.second)                   // Add second measurement
                + " D: " + String(height) + "mm"        // Add height measurement
                + " P: " + String(pressure) + "Pa"    // Add pressure measurement
                + " T: " + String(temp) + "C"      // Add temperature measurement
                + " H: " + String(hum) + "%";         // Add humidity measurement

  File dataFile = SD.open("test.txt", FILE_WRITE);    // Open file MUST CERTAIN NUMBER OF CHARACTERS

  if (dataFile) {                                   // If the file is available:
    dataFile.println(dataString);                   // Write the dataString to the file
    dataFile.close();                               // Close the file
    //Serial.println(dataString);                     // print to the serial port too
  }
  else {
    //Serial.println("error opening datalog.txt");    // if the file isn't open, pop up an error - would just be sleep:
  }

}

#include <ArduinoLowPower.h>
#include <Wire.h>
#include <SD.h>
#include <~/Documents/Arduino/libraries/src/PressureSensorLibrary/PressureSensor.h>
#include <PressureSensor.h>
#include <TOF_Sensor.h>
#include <temp_sensor.h>

#define MEAN 1


//setup global variables.

int Data = 7;  //The pin to which our DHT22 is connected.

float humidity;
float temperature;
double pressure;
double distance;
int sample = 0;

boolean measure = false;

RTCAlarmTime alarm;
DS3231 clock;
RTCDateTime dt;

void setup() {
  //begin peripherals

  Serial.begin(9600);  //Serial doesn't work with deep sleep.
  Serial.println("Start");

  clock.begin();
  SD.begin();


  clock.setDateTime(__DATE__, __TIME__);  //sync RTC to PC

  pinMode(0, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);  //Interrupt in with pullup -> needed for RTC

  LowPower.attachInterruptWakeup(digitalPinToInterrupt(6), alarmInt, FALLING);  //Interrupt

  //reset alarms
  clock.armAlarm1(false);
  clock.armAlarm2(false);
  clock.clearAlarm1();
  clock.clearAlarm2();

  clock.enable32kHz(false);                      //turn off the 32kHz 32K
  clock.enableOutput(false);                     //disable oscilator output SQW\

  clock.setAlarm1(0, 0, 0, 30, DS3231_EVERY_SECOND);  //Set Alarm1 output to SQW
}


//Interrupt Function
void alarmInt() {
  //clear alarm
  measure = true; 
  
}

void loop() {
  if (measure)  // THIS IS NECESSARY BECAUSE THE PRESSURE SENSOR MUST BE SYNCED TO 32K OSCILLATOR WHICH IS ONLY POSSIBLE IF IT RUNS IN "LOOP" NOT THE INTERRUPT
  {
    
    clock.enable32kHz(true);  //turn on the 32kHz 32K

    //take measurements
    temp_data(Data, humidity, temperature);
    distance = TOF_Sensor_Distance_Measure_MM(MEAN);
    pressure = Pressure_Sensor_Measure_mBar();
    dt = clock.getDateTime();

    Serial.println(sample);
    Serial.println(pressure);
    Serial.println(dt.second);

    //write to SD
    logData(sample, distance, humidity, temperature, pressure, dt);
    sample++;  //increase iteration

    measure = false;           //reset measure
    clock.enable32kHz(false);  //turn off the 32kHz 32K to save power
    clock.clearAlarm1();
  }

  //sleep until next interrupt.
  //LowPower.sleep();
}


void logData(int sampleNumber, double height, double hum, double temp, double pressure, RTCDateTime clockData) {
  String dataString = "";  // Make a string for assembling the data to log: (53 char max)
  //GENRAL FORM:  0000;00/00/0000;00:00:00;0000.00;0000.00;00.00;000.00
  dataString += String(sampleNumber)       // Add sample number (~0-8760) (4 char)
                + ";" + String(dt.day)     // Add date measurement (~0-31) (2 char)
                + "/" + String(dt.month)   // Add month measurement (0-12) (2 char)
                + "/" + String(dt.year)    // Add year measurement (0-9999) (4 char)
                + ";" + String(dt.hour)    // Add hour measurement (~0-24) (2 char)
                + ":" + String(dt.minute)  // Add minute measurement (~0-60) (2 char)
                + ":" + String(dt.second)  // Add second measurement (~0-60) (2 char)
                + ";" + String(height)     // Add height measurement (~0.00-1000.00) (6 char)
                + ";" + String(pressure)   // Add pressure measurement (~0.00-9999.99) (6 char)
                + ";" + String(temp)       // Add temperature measurement (~0.00-99.99) (4 char)
                + ";" + String(hum);       // Add humidity measurement (~0.00-100.00) (5 char)

  File dataFile = SD.open("datalog2.txt", FILE_WRITE);  // Open file MUST CERTAIN NUMBER OF CHARACTERS

  if (dataFile) {                  // If the file is available:
    dataFile.println(dataString);  // Write the dataString to the file
    dataFile.close();              // Close the file

    Serial.println(dataString);  // print to the serial port too
  } else {
    Serial.println("error opening datalog.txt");  // if the file isn't open, pop up an error - would just be sleep:
  }
}

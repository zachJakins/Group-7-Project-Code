#include <ArduinoLowPower.h>
#include <Wire.h>
#include <SD.h>
#include <DS3231.h>
#include <PressureSensor.h>
#include <PressureSensor.h>
#include <TOF_Sensor.h>
#include <temp_sensor.h>

#define MEAN 1
#define TEMPORALRESOLUTION 1
#define LOGNAME "datalog.txt"
#define NANOPOWERPIN 0
#define NANOADDRESS 0x0B
#define INTERRUPTPIN 6
#define Data 7


//setup global variables.

float humidity;
float temperature;
double pressure;
double distance;
int sample = 0;
int timerCounter = 0;

boolean measure = false;
boolean RadioReceived = false;
boolean interrupt = false;
boolean sleep = true;

RTCAlarmTime alarm;
DS3231 clock;
RTCDateTime dt;



void setup() {
  Serial.begin(9600);


  clock.begin();
  SD.begin();
  Wire.begin();

  clock.setDateTime(__DATE__, __TIME__);  //sync RTC to PC

  pinMode(INTERRUPTPIN, INPUT_PULLUP);    //Interrupt in with pullup -> needed for RTC
  pinMode(NANOPOWERPIN, OUTPUT);  //Arduino pin 0 switches the nano

  LowPower.attachInterruptWakeup(digitalPinToInterrupt(INTERRUPTPIN), alarmInt, FALLING);  //Interrupt

  //reset alarms
  clock.armAlarm1(false);
  clock.armAlarm2(false);
  clock.clearAlarm1();
  clock.clearAlarm2();

  clock.enable32kHz(false);                      //turn off the 32kHz 32K
  clock.enableOutput(false);                     //disable oscilator output SQW\

  clock.setAlarm1(0, 0, 0, 30, DS3231_MATCH_S);  //Set Alarm1 output to SQW
}


//Interrupt Function DONT PUT ANYTHING MORE IN HERE IT BREAKS THINGS
void alarmInt() {
  interrupt = true;  //allows main to work
  Serial.println("INT");
  
}





void loop() {

  //Interrupts cause issues so putting this code in main is best.
  if (interrupt)  //on the occasion of an interrupt
  {
    Serial.println("INTBEG");

    digitalWrite(NANOPOWERPIN, LOW);  //Turn the Nano on

    //measure counter
    timerCounter++;
    if (timerCounter == TEMPORALRESOLUTION) {
      timerCounter = 0;
      measure = true;
    }

    Serial.println("INTMID");
    //requests 1 byte from device NANOADDRESS (the nano)
    Wire.requestFrom(NANOPOWERPIN, 1);
    while (Wire.available())  //reads data
    {
      RadioReceived = Wire.read();  //Nano will send either a 0 (user isn't there) or a 1 (user is there and wants data.)
    }

    Serial.println("INTEND");
    //clears interrupt flags
    interrupt = false;
    clock.clearAlarm1();
    sleep=true;
  }


  //on the occasion of measurement
  if (measure)  // THIS IS NECESSARY BECAUSE THE PRESSURE SENSOR MUST BE SYNCED TO 32K OSCILLATOR WHICH IS ONLY POSSIBLE IF IT RUNS IN "LOOP" NOT THE INTERRUPT
  {
    Serial.println("MEAS");
    clock.enable32kHz(true);  //turn on the 32kHz 32K

    //take measurements
    temp_data(Data, humidity, temperature); //WILL READ NAN IF FAILS
    dt = clock.getDateTime(); 
    pressure = Pressure_Sensor_Measure_mBar(); //PRESSURE WILL READ 1043.30 IF IT FAILS OR AN ABSURD VALUE >>1100mBar
    distance = TOF_Sensor_Distance_Measure_MM(MEAN); //DISTANCE WILL READ 0.00 IF IT FAILS/TOO CLOSE/TOO FAR

    //write to SD
    logData(sample, distance, humidity, temperature, pressure, dt);
    sample++;  //increase iteration

    measure = false;           //reset measure
    clock.enable32kHz(false);  //turn off the 32kHz 32K to save power
  }

  //if the nano receives a transmission this will send it all of the data from the SD via i2c so it can then transmit with the radio
  if (RadioReceived) {
    Serial.println("RAD");
    //send data to the nano
    Wire.beginTransmission(NANOADDRESS);
    //Read Line on SD

    // re-open the file for reading:
    File dataFile = SD.open(LOGNAME);
    if (dataFile) {
      // read from the file until there's nothing else in it:
      while (dataFile.available()) {
        Wire.write(dataFile.read());
      }
      dataFile.close();  // close the file:
    }
    Wire.endTransmission();  //ends transmission to nano

    RadioReceived = false; //reset RadioReceived
  }
  digitalWrite(NANOPOWERPIN, HIGH);  //turn off nano

  //sleep until next interrupt.
  
  if(sleep)
  {
    Serial.println("SLEEP");
    sleep = false;
  LowPower.idle();
  }
}


void logData(int sampleNumber, double height, double hum, double temp, double pressure, RTCDateTime clockData) {
  String dataString = "";  // Make a string for assembling the data to log: (54 char max)
  //GENRAL FORM:  0000;00/00/0000;00:00:00;0000.00;0000.00;00.00;000.00;#
  //WILL BE READ BY USER AS:  0000;00/00/0000;00:00:00;0000.00;0000.00;00.00;000.00;
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
                + ";" + String(hum)        // Add humidity measurement (~0.00-100.00) (5 char)
                + ";#";                    //add end character

  File dataFile = SD.open(LOGNAME, FILE_WRITE);  // Open file MUST CERTAIN NUMBER OF CHARACTERS

  if (dataFile) {                  // If the file is available:
    dataFile.println(dataString);  // Write the dataString to the file
    dataFile.close();              // Close the file
  }
}

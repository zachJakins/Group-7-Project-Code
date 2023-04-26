#include "temp_sensor.h"

//Defining Data Pin //Define the pin as connected
#define Data 7

//variable for pass by reference
float hum, temp;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("BEGIN:");

Serial.println("TEMPERATURE");
  for (int i = 0; i < 100; i++)  //loop 100 times
  {
    temp_data(Data, hum, temp);  //Function from temp_sensor.h
    Serial.println(temp);
    delay(250);
    
  }
  Serial.println("HUMIDITY");
    for (int i = 0; i < 100; i++)  //loop 100 times
  {
    temp_data(Data, hum, temp);  //Function from temp_sensor.h
    Serial.println(hum);
    delay(250);
    
  }
  Serial.println("FINISHED");
}

void loop() {
}

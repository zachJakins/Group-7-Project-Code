#include "temp_sensor.h"

//Defining Data Pin //Define the pin as connected
#define Data 7

//variable for pass by reference
float hum, temp;

void setup() {
  Serial.begin(9600);
  Serial.println("BEGIN:");

  for (int i = 0; i < 100; i++)  //loop 100 times
  {
    temp_data(Data, hum, temp);  //Function from temp_sensor.h
    Serial.print(temp);
    Serial.print(" ");
    Serial.println(temp);
    delay(50);
  }
  Serial.println("FINISHED");
}

void loop() {
}

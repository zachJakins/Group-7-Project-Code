#include "temp_sensor.h"

//Defining Data Pin //Define the pin as connected
#define Data 3  

//variable for pass by reference
float hum, temp;

void setup() {
  Serial.begin(9600);
}

void loop() {
  temp_data(Data, hum, temp);   //Function from temp_sensor.h

  //Can delete following comments // Used for testing
  /*
  if (isnan(hum) || isnan(temp)) {
    Serial.print("No Data");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
  }
  delay(1000);
  */
}

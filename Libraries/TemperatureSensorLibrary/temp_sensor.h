/* Dependencies: (Make sure to have both this library pre-installed)
 *  - Adafruit Unified Sensor Driver library
 *  - DHT library --> #include <DHT.h>
 */ 
#include <DHT.h>
/*
 * 3 Inputs:
 * -  Data pin
 * -  Humidity (Pass by reference)    //So initialise in main code
 * -  Temperature (Pass by reference)
 */
 
void temp_data(int Data, float &hum, float &temp) {
  //Initialise DHT object //Unsure how memory would be affected since keep creating in every loop?  //Should be ok? 
  DHT dht(Data, DHT22);
  dht.begin();     //Dont quite understand why need initialize sensor?  //Works even without this line
  //Get both data and write to variable
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  //Code for testing the outputs // Not needed //Can delete following commented part
  //Can uncomment for test runs
  /*
    if (isnan(hum) || isnan(temp)) {
    Serial.println("No data");
    }
  */
  /*
    else {
    //Print temp and humidity values to serial monitor

      Serial.print("Humidity: ");
      Serial.print(hum);
      Serial.print(" %, Temp: ");
      Serial.print(temp);
      Serial.println(" Celsius");

    }
  */
  //delay(2000); //Delay 2 sec.
}

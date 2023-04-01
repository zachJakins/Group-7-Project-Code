#include <TOF_Sensor.h>

//Test Script get rid of when using as an include.
void setup()
{
  Serial.begin(9600);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
}


void loop() {
  
  Serial.print("Distance (mm): "); 
  Serial.println(TOF_Sensor_Distance_Measure_MM(1));
  delay(100);

}

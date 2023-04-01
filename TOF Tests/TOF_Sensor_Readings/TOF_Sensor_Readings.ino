#include <TOF_Sensor.h>
#define MEAN 1


//Test Script get rid of when using as an include.
void setup()
{
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  Serial.print("Sensor Mean: ");
  Serial.println(MEAN);

  for (int i = 0; i < 100; i++)
  {
    Serial.println(TOF_Sensor_Distance_Measure_MM(MEAN));
    delay(100);
  }
  Serial.println("FINISHED");
}


void loop() {
}

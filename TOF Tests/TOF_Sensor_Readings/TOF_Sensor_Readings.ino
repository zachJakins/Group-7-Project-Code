#include <TOF_Sensor.h>



//Test Script get rid of when using as an include.
void setup() {
  Serial.begin(9600);
  int MEAN = 1;

  // wait until serial port opens for native USB devices
  while (!Serial) {
    delay(1);
  }

  Serial.print("Sensor Mean: ");
  Serial.println(MEAN);

  for (int i = 0; i < 100; i++) {
    Serial.println(TOF_Sensor_Distance_Measure_MM(MEAN));
    delay(20);
  }
  Serial.println("FINISHED");

  MEAN = 5;
  Serial.print("Sensor Mean: ");
  Serial.println(MEAN);

  for (int i = 0; i < 100; i++) {
    Serial.println(TOF_Sensor_Distance_Measure_MM(MEAN));
    delay(50);
  }
  Serial.println("FINISHED");

  MEAN = 10;
  Serial.print("Sensor Mean: ");
  Serial.println(MEAN);

  for (int i = 0; i < 100; i++) {
    Serial.println(TOF_Sensor_Distance_Measure_MM(MEAN));
    delay(50);
  }
  Serial.println("FINISHED");
}


void loop() {
}

#include <PressureSensor.h>
#include <MKRClock.h>

//Test Script get rid of when using as an include.
void setup()
{
  Serial.begin(115200);
  Activate_Internal_Clock_Zero();
  
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
}


void loop() {
  
  Serial.print("Pressure (mBar): "); 
  Serial.println(Pressure_Sensor_Measure_mBar());//Takes integer argument to decide how many measurements the sensor will take before returning mean.
  delay(100);

}

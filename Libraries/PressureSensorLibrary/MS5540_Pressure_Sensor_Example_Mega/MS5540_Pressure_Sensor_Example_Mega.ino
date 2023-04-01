#include <PressureSensor.h>
#include <MegaClock.h>

//Test Script get rid of when using as an include.
void setup()
{
  Serial.begin(9600);
  pinMode(9, OUTPUT);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
}


void loop() {
  TCCR1B = (TCCR1B & 0xF8) | 1 ; //generates the MCKL signal
  analogWrite (9, 128) ;

  Serial.print("Pressure (mBar): ");
  Serial.println(Pressure_Sensor_Measure_mBar());//Takes integer argument to decide how many measurements the sensor will take before returning mean.
  delay(100);

}

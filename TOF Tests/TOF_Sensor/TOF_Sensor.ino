#ifndef TOF_SENSOR_H
#define TOF_SENSOR_H
#include <Arduino.h>
#include "Adafruit_VL53L0X.h"
double TOF_Sensor_Distance_Measure_MM(int Mean_Measurements_Count = 1)
{
  Adafruit_VL53L0X sensor = Adafruit_VL53L0X();//creates sensor object
  VL53L0X_RangingMeasurementData_t distance;//creates distance object
  double measurements;

  //initialises sensor, prints an error if not (ASSUMES THAT SERIAL HAS BEEN OPENEND)
  //Could waste power if we initialise sensor every time, how much realistically?
  if (!sensor.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }

  for (int i = 0; i < Mean_Measurements_Count; i++)
  {
    sensor.rangingTest(&distance);//range test
    if (distance.RangeStatus != 4)//ensures correct data
    {
      measurements += distance.RangeMilliMeter;
    }
    else {
      Serial.println("ranging error");
    }
  }
  measurements = measurements / (double)Mean_Measurements_Count;//turns sum into mean average
  return (measurements);//return mean
}
#endif

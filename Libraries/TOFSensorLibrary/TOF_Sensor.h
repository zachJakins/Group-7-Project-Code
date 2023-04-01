#ifndef TOF_SENSOR_H
#define TOF_SENSOR_H
#include <Arduino.h>
#include "DFRobot_VL53L0X.h"


double TOF_Sensor_Distance_Measure_MM(int Mean_Measurements_Count = 1);

#endif

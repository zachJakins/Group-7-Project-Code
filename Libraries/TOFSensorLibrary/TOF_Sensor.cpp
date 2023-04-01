#include "TOF_Sensor.h"

double TOF_Sensor_Distance_Measure_MM(int Mean_Measurements_Count)
{
    Wire.begin();
    DFRobot_VL53L0X sensor;//creates sensor object
    double measurements;

    //Could waste power if we initialise sensor every time, how much realistically?
    sensor.begin(0x50);
    sensor.setMode(sensor.eContinuous,sensor.eHigh);//set to continuous high precision
    sensor.start();//turn sensor on

    for (int i = 0; i < Mean_Measurements_Count; i++)
    {

        measurements += sensor.getDistance();//sums distance
    }


    measurements = measurements / (double)Mean_Measurements_Count;//turns sum into mean average
    return (measurements);//return mean
}

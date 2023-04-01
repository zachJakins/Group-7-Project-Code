#include "PressureSensor.h"


void resetsensor() //this function keeps the sketch a little shorter
{
    SPI.setDataMode(SPI_MODE0);
    SPI.transfer(0x15);
    SPI.transfer(0x55);
    SPI.transfer(0x40);
}
double Pressure_Sensor_Measure_mBar()
{
    SPI.begin(); //see SPI library details on arduino.cc for details
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV32); //divide 16 MHz to communicate on 500 kHz
    delay(100);
    resetsensor(); //resets the sensor - caution: afterwards mode = SPI_MODE0!

    //Calibration word 1
    unsigned int result1 = 0;
    unsigned int inbyte1 = 0;
    SPI.transfer(0x1D); //send first byte of command to get calibration word 1
    SPI.transfer(0x50); //send second byte of command to get calibration word 1
    SPI.setDataMode(SPI_MODE1); //change mode in order to listen
    result1 = SPI.transfer(0x00); //send dummy byte to read first byte of word
    result1 = result1 << 8; //shift returned byte
    inbyte1 = SPI.transfer(0x00); //send dummy byte to read second byte of word
    result1 = result1 | inbyte1; //combine first and second byte of word

    resetsensor(); //resets the sensor

    //Calibration word 2; see comments on calibration word 1
    unsigned int result2 = 0;
    byte inbyte2 = 0;
    SPI.transfer(0x1D);
    SPI.transfer(0x60);
    SPI.setDataMode(SPI_MODE1);
    result2 = SPI.transfer(0x00);
    result2 = result2 <<8;
    inbyte2 = SPI.transfer(0x00);
    result2 = result2 | inbyte2;

    resetsensor(); //resets the sensor

    //Calibration word 3; see comments on calibration word 1
    unsigned int result3 = 0;
    byte inbyte3 = 0;
    SPI.transfer(0x1D);
    SPI.transfer(0x90);
    SPI.setDataMode(SPI_MODE1);
    result3 = SPI.transfer(0x00);
    result3 = result3 <<8;
    inbyte3 = SPI.transfer(0x00);
    result3 = result3 | inbyte3;

    resetsensor(); //resets the sensor

    //Calibration word 4; see comments on calibration word 1
    unsigned int result4 = 0;
    byte inbyte4 = 0;
    SPI.transfer(0x1D);
    SPI.transfer(0xA0);
    SPI.setDataMode(SPI_MODE1);
    result4 = SPI.transfer(0x00);
    result4 = result4 <<8;
    inbyte4 = SPI.transfer(0x00);
    result4 = result4 | inbyte4;

    //now we do some bitshifting to extract the calibration factors
    //out of the calibration words;
    long c1 = (result1 >> 1) & 0x7FFF;
    long c2 = ((result3 & 0x003F) << 6) | (result4 & 0x003F);
    long c3 = (result4 >> 6) & 0x03FF;
    long c4 = (result3 >> 6) & 0x03FF;
    long c5 = ((result1 & 0x0001) << 10) | ((result2 >> 6) & 0x03FF);
    long c6 = result2 & 0x003F;

    resetsensor(); //resets the sensor

    //Pressure:
    unsigned int presMSB = 0; //first byte of value
    unsigned int presLSB = 0; //last byte of value
    unsigned int D1 = 0;
    SPI.transfer(0x0F); //send first byte of command to get pressure value
    SPI.transfer(0x40); //send second byte of command to get pressure value
    delay(35); //wait for conversion end
    SPI.setDataMode(SPI_MODE1); //change mode in order to listen
    presMSB = SPI.transfer(0x00); //send dummy byte to read first byte of value
    presMSB = presMSB << 8; //shift first byte
    presLSB = SPI.transfer(0x00); //send dummy byte to read second byte of value
    D1 = presMSB | presLSB; //combine first and second byte of value

    resetsensor(); //resets the sensor

    //Temperature:
    unsigned int tempMSB = 0; //first byte of value
    unsigned int tempLSB = 0; //last byte of value
    unsigned int D2 = 0;
    SPI.transfer(0x0F); //send first byte of command to get temperature value
    SPI.transfer(0x20); //send second byte of command to get temperature value
    delay(35); //wait for conversion end
    SPI.setDataMode(SPI_MODE1); //change mode in order to listen
    tempMSB = SPI.transfer(0x00); //send dummy byte to read first byte of value
    tempMSB = tempMSB << 8; //shift first byte
    tempLSB = SPI.transfer(0x00); //send dummy byte to read second byte of value
    D2 = tempMSB | tempLSB; //combine first and second byte of value

    //calculation of the real values by means of the calibration factors and the maths
    //in the datasheet. const MUST be long
    const long UT1 = (c5 << 3) + 20224;
    const long dT = D2 - UT1;
    const long TEMP = 200 + ((dT * (c6 + 50)) >> 10);
    const long OFF  = (c2 * 4) + (((c4 - 512) * dT) >> 12);
    const long SENS = c1 + ((c3 * dT) >> 10) + 24576;
    const long X = (SENS * (D1 - 7168) >> 14) - OFF;
    long PCOMP = ((X * 10) >> 5) + 2500;
    float TEMPREAL = TEMP/10;
    float PCOMPHG = PCOMP * 750.06 / 10000; // mbar*10 -> mmHg === ((mbar/10)/1000)*750/06

    //2-nd order compensation only for T < 20В°C or T > 45В°C

    long T2 = 0;
    float P2 = 0;

    if (TEMP < 200)
    {
        T2 = (11 * (c6 + 24) * (200 - TEMP) * (200 - TEMP) ) >> 20;
        P2 = (3 * T2 * (PCOMP - 3500) ) >> 14;
    }
    else if (TEMP > 450)
    {
        T2 = (3 * (c6 + 24) * (450 - TEMP) * (450 - TEMP) ) >> 20;
        P2 = (T2 * (PCOMP - 10000) ) >> 13;
    }

    if ((TEMP < 200) || (TEMP > 450))
    {
        const float TEMP2 = TEMP - T2;
        const float PCOMP2 = PCOMP - P2;

        float TEMPREAL2 = TEMP2/10;
        float PCOMPHG2 = PCOMP2 * 750.06 / 10000; // mbar*10 -> mmHg === ((mbar/10)/1000)*750/06

        PCOMP=PCOMP2;
    }

    return((double)PCOMP/10.0);
}

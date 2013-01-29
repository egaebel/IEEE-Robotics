#include "ColorSensor.h"

//Feild Variables
BackEndColorSensor backEnd;
int whitePulseValueTotal;
int bluePulseValueTotal;
int redPulseValueTotal;
int greenPulseValueTotal;

ColorSensor::ColorSensor()
{
    backEnd = new BackEndColorSensor();
}

/**
 *Detects color in front of sensor
 * Takes pulse readings and stores until numberOfPulseReadings number of pulse readings and then
 * determines color based on patterns, ratios, or larger values.
 *@param the number of pulseReadings to take (more = more confidence in result)
 *@returns the color to in front of the color sensor
                    0 = white
                    1 = blue
                    2 = red
                    3 = green
 */
ColorSensor::detectColor(int numberOfPulseReadings) {

    for(; numberOfPulseReadings > 0; numberOfPulseReadings--)  {
        whitePulseValueTotal += backEnd.colorRead(0);
        bluePulseValueTotal += backEnd.colorRead(1);
        redPulseValueTotal += backEnd.colorRead(2);
        greenPulseValueTotal += backEnd.colorRead(3);
    }

//TODO Look at results of color sensor pulse readings in the lab
}

/**
 * See detectColor(int numberOfPulseReadings) documentation
 */
ColorSensor::detectColor()   {

    for(int readCounter = 0; readCounter < 5; readCounter++)  {
        whitePulseValueTotal += backEnd.colorRead(0);
        bluePulseValueTotal += backEnd.colorRead(1);
        redPulseValueTotal += backEnd.colorRead(2);
        greenPulseValueTotal += backEnd.colorRead(3);
    }



//TODO Look at results of color sensor pulse readings in the lab
}




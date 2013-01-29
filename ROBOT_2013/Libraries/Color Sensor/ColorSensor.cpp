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


//TODO Look at results of color sensor pulse readings in the lab
}

/**
 * See detectColor(int numberOfPulseReadings) documentation
 */
ColorSensor::detectColor()   {
//TODO Look at results of color sensor pulse readings in the lab
}

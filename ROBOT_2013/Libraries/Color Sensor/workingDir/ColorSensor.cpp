#include "ColorSensor.h"

//Feild Variables
BackEndColorSensor backEnd;
//int whitePulseValueTotal;
//int bluePulseValueTotal;
//int redPulseValueTotal;
//int greenPulseValueTotal;


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

    int certaintyOfColor;
    int [4] colorPulseValues = {0, 0, 0, 0};

    for(; numberOfPulseReadings > 0; numberOfPulseReadings--)  {
        pulseValues[0] += backEnd.colorRead(0);
        pulseValues[1] += backEnd.colorRead(1);
        pulseValues[2] += backEnd.colorRead(2);
        pulseValues[3] += backEnd.colorRead(3);
    }

    isBlue();
    isYellow();
    isBrown();
    isRed();
    isPurple();

    //Clean-Up
    pulseValues = {0, 0 , 0, 0};

    //OR:
//    for(i = 0; i < 4; i++)  {
//        pulseValues[i] = 0;
//    }

//TODO Look at results of color sensor pulse readings in the lab
}

/**
 * See detectColor(int numberOfPulseReadings) documentation
 */
ColorSensor::detectColor()   {
    detectColor(6);
}

/**
 * Checks whether pusle values signify color blue.
 */
bool ColorSensor::isBlue()   {

}







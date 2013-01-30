#include "ColorSensor.h"
#include <cmath>

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
int ColorSensor::detectColor(int numberOfPulseReadings) {

    int certaintyBLUE, certaintyYELLOW, certaintyBROWN, certaintyRED, certaintyPURPLE;

    int [4] colorPulseValues = {0, 0, 0, 0};

    for(int i = 0; i < 4; i++)  {
        int throwAway = backEnd.colorRead(0); //Declared and destryoed at each run.
        throwAway = backEnd.colorRead(1);
        throwAway = backEnd.colorRead(2);
        throwAway = backEnd.colorRead(3);
    }

    for(; numberOfPulseReadings > 0; numberOfPulseReadings--)  {
        colorPulseValues[0] += backEnd.colorRead(0);
        colorPulseValues[1] += backEnd.colorRead(1);
        colorPulseValues[2] += backEnd.colorRead(2);
        colorPulseValues[3] += backEnd.colorRead(3);
    }

    certaintyBLUE = isBlue(colorPulseValues); certaintyYELLOW = isYellow(colorPulseValues); certaintyBROWN = isBrown(colorPulseValues);
    certaintyRED = isRed(colorPulseValues); certaintyPURPLE = isPurple(colorPulseValues);



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
int ColorSensor::detectColor()   {
    detectColor(6);
}

/**
 * Returns certainty of being blue (scale 0-5)
 */
int ColorSensor::isBlue(int colorPulseValues[4])  {
    int difference = abs(cmath.a colorPulseValues[1] * 3 - (colorPulseValues[0] + colorPulseValues[2] + colorPulseValues[3]));
    if(difference > 120000) {
        return 5;
    }
    else if (difference > 80000)    {
        return 3;
    }

    return 0;
}

/**
 * Returns certainty of being yellow (scale 0-5)
 */
int ColorSensor::isYellow(int colorPulseValues[4])  {
    int total = colorPulseValues[0] + colorPulseValues[1] + colorPulseValues[2] + colorPulseValues[3]
    if(total < 75000)   {
       if(abs(colorPulseValues[0] - colorPulseValues[2]) < colorPulseValues[2]/3
          && abs(colorPulseValues[0] - colorPulseValues[3]) < colorPulseValues[3]/3)  {
              return 5;

       }
       else if(abs(colorPulseValues[0] - colorPulseValues[2]) < colorPulseValues[2]/2
          && abs(colorPulseValues[0] - colorPulseValues[3]) < colorPulseValues[3]/2) {
              return 4;

       }
    }

    else    {
        return 0;
    }



}

/**
 * Returns certainty of being brown (scale 0-5)
 */
int ColorSensor::isBrown(int colorPulseValues[4])  {
    difference = colorPulseValues[0] + colorPulseValues[2] + colorPulseValues[3] - colorPulseValues[1];
    if(difference > colorPulseValues[1] * 4)    {
        return 4;
    }

    else if(difference > (colorPulseValues[1] * 3 + colorPulseValues[1]/2)) {
        return 3;
    }

    else    {
        return 0;

    }
}

/**
 * Returns certainty of being red (scale 0-5)
 */
int ColorSensor::isRed(int colorPulseValues[4])  {
    int difference = abs(colorPulseValues[3] - colorPulseValues[2]);
    if(colorPulseValues[1]*8 < colorPulseValues[2] && difference < colorPulseValues[3]/8) {
        return 5;
    }

    else if(colorPulseValues[1]*6 < colorPulseValues[2] && difference < colorPulseValues[3]/6)
    {
        return 4;
    }

    else
    {
        return 0;
    }

}

/**
 * Returns certainty of being purple (scale 0-5)
 */
int ColorSensor::isPurple(int colorPulseValues[4])  {
    if(colorPulseValues[0]*4 > colorPulseValues[1]*7)   {

        if(abs(colorPulseValues[2] - colorPulseValues[3]) < colorPulseValues[2]/5) {
            return 5;
        }
        else {
            return 3;
        }
    }
    else {
        return 0;
    }
}







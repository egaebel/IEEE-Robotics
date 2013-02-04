#include "ColorSensor.h"
#include "BackEndColorSensor.h"

//Feild Variables
BackEndColorSensor *backEnd;
//int whitePulseValueTotal;
//int bluePulseValueTotal;
//int redPulseValueTotal;
//int greenPulseValueTotal;


ColorSensor::ColorSensor()
{
    backEnd = &BackEndColorSensor();
}

/**
 *Detects color in front of sensor
 * Takes pulse readings and stores until numberOfPulseReadings number of pulse readings and then
 * determines color based on patterns, ratios, or larger values.
 *@param the number of pulseReadings to take (more = more confidence in result)
 *@returns the color to in front of the color sensor
                    10000 = BLUE
                    2000 = YELLOW
                    300 = RED
                    40 = BROWN
                    5 = PURPLE
 */
int ColorSensor::detectColor(int numberOfPulseReadings) {

    int certaintyBLUE, certaintyYELLOW, certaintyBROWN, certaintyRED, certaintyPURPLE;

    int certaintyFinalistOne, certaintyFinalistTwo;
    int colorPulseValues [4] = {0, 0, 0, 0};

    for(int i = 0; i < 4; i++)  {
        int throwAway = backEnd->colorRead(0); //Declared and destryoed at each run.
        throwAway = backEnd->colorRead(1);
        throwAway = backEnd->colorRead(2);
        throwAway = backEnd->colorRead(3);
    }

    for(; numberOfPulseReadings > 0; numberOfPulseReadings--)  {
        colorPulseValues[0] += backEnd->colorRead(0);
        colorPulseValues[1] += backEnd->colorRead(1);
        colorPulseValues[2] += backEnd->colorRead(2);
        colorPulseValues[3] += backEnd->colorRead(3);
    }

    certaintyBLUE = isBlue(colorPulseValues); certaintyYELLOW = isYellow(colorPulseValues); certaintyBROWN = isBrown(colorPulseValues);
    certaintyRED = isRed(colorPulseValues); certaintyPURPLE = isPurple(colorPulseValues);

    if(certaintyBLUE == 5)  {
        certaintyFinalistOne = 10000;
    }

    if(certaintyYELLOW == 5)  {
        if(certaintyFinalistOne == 0)   {
            certaintyFinalistOne = 2000;
        } else    {
        certaintyFinalistTwo = 2000;
        }
    }

    if(certaintyRED == 5)  {
        if(certaintyFinalistOne == 0)   {
            certaintyFinalistOne = 300;
        } else if(certaintyFinalistTwo == 0) {
            certaintyFinalistTwo = 300;
        }
    }

    if(certaintyBROWN == 5)  {
        if(certaintyFinalistOne == 0)   {
            certaintyFinalistOne = 40;
        } else if(certaintyFinalistTwo == 0) {
            certaintyFinalistTwo = 40;
        }
    }

    if(certaintyPURPLE == 5)  {
        if(certaintyFinalistOne == 0)   {
            certaintyFinalistOne = 5;
        } else if(certaintyFinalistTwo == 0) {
            certaintyFinalistTwo = 5;
        }
    }
    
    //Clean-Up
    colorPulseValues = {0, 0 , 0, 0};

    return (certaintyFinalistOne + certaintyFinalistTwo);
}
    

    //OR:
//    for(i = 0; i < 4; i++)  {
//        colorPulseValues[i] = 0;
//    }



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
    int difference = absoluteValue(colorPulseValues[1] * 3 - (colorPulseValues[0] + colorPulseValues[2] + colorPulseValues[3]));
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
    int total = colorPulseValues[0] + colorPulseValues[1] + colorPulseValues[2] + colorPulseValues[3];
    if(total < 75000)   {
       if(absoluteValue(colorPulseValues[0] - colorPulseValues[2]) < colorPulseValues[2]/3
          && absoluteValue(colorPulseValues[0] - colorPulseValues[3]) < colorPulseValues[3]/3)  {
              return 5;

       }
       else if(absoluteValue(colorPulseValues[0] - colorPulseValues[2]) < colorPulseValues[2]/2
          && absoluteValue(colorPulseValues[0] - colorPulseValues[3]) < colorPulseValues[3]/2) {
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
    int difference = colorPulseValues[0] + colorPulseValues[2] + colorPulseValues[3] - colorPulseValues[1];
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
    int difference = absoluteValue(colorPulseValues[3] - colorPulseValues[2]);
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

        if(absoluteValue(colorPulseValues[2] - colorPulseValues[3]) < colorPulseValues[2]/5) {
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


int ColorSensor::absoluteValue(int a)  {
  return ((a*a)/a);
}






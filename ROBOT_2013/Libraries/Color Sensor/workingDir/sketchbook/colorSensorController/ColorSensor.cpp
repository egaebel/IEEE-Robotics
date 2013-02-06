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
void ColorSensor::detectColor(int numberOfPulseReadings, int* ouputArray) {

    int certaintyBLUE, certaintyYELLOW, certaintyBROWN, certaintyRED, certaintyPURPLE;

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
    

    certaintyYELLOW = isYellow(colorPulseValues); certaintyBROWN = isBrown(colorPulseValues);
    certaintyRED = isRed(colorPulseValues); certaintyPURPLE = isPurple(colorPulseValues);
    
    ouputArray[0] = isBlue(colorPulseValues);
    ouputArray[1] = certaintyYELLOW;
    ouputArray[2] = certaintyBROWN;
    ouputArray[3] = certaintyRED;
    ouputArray[4] =  certaintyPURPLE;
}

/**
 * See detectColor(int numberOfPulseReadings) documentation
 */
void ColorSensor::detectColor(int* ouputArray)   {
    detectColor(6, ouputArray);
}

/**
 * Returns certainty of being blue (scale 0-5)
 */
int ColorSensor::isBlue(int colorPulseValues[4])  {
    if(absoluteValue(colorPulseValues[1] * 3 - (colorPulseValues[0] + colorPulseValues[2] + colorPulseValues[3])) > 120000) {
        return 1;
    }
    
    return 0;
}

/**
 * Returns certainty of being yellow (scale 0-5)
 */
int ColorSensor::isYellow(int colorPulseValues[4])  {
    if((colorPulseValues[0] + colorPulseValues[1] + colorPulseValues[2] + colorPulseValues[3] < 75000) && 
      (absoluteValue(absoluteValue(colorPulseValues[0] - colorPulseValues[2]) < colorPulseValues[2]/2
      && absoluteValue(colorPulseValues[0] - colorPulseValues[3]) < colorPulseValues[3]/2)))  {
         return 1;
    }
    
    return 0;
}

/**
 * Returns certainty of being brown (scale 0-5)
 */
int ColorSensor::isBrown(int colorPulseValues[4])  {
    if(colorPulseValues[0] + colorPulseValues[2] + colorPulseValues[3] - colorPulseValues[1] > colorPulseValues[1] * 3 + colorPulseValues[1]/2)    {
        return 1;
    }
    
    return 0;
}

/**
 * Returns certainty of being red (scale 0-5)
 */
int ColorSensor::isRed(int colorPulseValues[4])  {
    if(colorPulseValues[1]*6 < colorPulseValues[2] && absoluteValue(colorPulseValues[3] - colorPulseValues[2]) < colorPulseValues[3]/6) {
        return 1;
    }
    
    return 0;
}

/**
 * Returns certainty of being purple (scale 0-5)
 */
int ColorSensor::isPurple(int colorPulseValues[4])  {
    if(colorPulseValues[0]*4 > colorPulseValues[1]*7 && absoluteValue(colorPulseValues[2] - colorPulseValues[3]) < colorPulseValues[2]/5)   {
        return 1;
    }
    
    return 0;
}


int ColorSensor::absoluteValue(int a)  {
  return ((a*a)/a);
}






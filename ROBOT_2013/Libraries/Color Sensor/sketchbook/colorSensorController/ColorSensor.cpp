#include "ColorSensor.h"
#include "BackEndColorSensor.h"

//Feild Variables
BackEndColorSensor *backEnd;

bool redBlock, blueBlock, brownBlock, yellowBlock, purpleBlock, greenBlock;
int numberOfDeads; //The number of values that went out of the range and therefore were chaned to 1.00
int pulseCount;

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
                    0 = RED
                    1 = BLUE
                    2 = BROWN
                    3 = YELLOW
                    4 = PURPLE
                    5 = GREEN
 */
int ColorSensor::detectColor() {
	throwAwayValues(); //Throws away intial 4 values
	int result = 6; //6 signifies no color determined	
	int whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue;
	
	while(result == 6)	{ //Possibility for infinite loop if no dominant color found after infinite pulse readings
		whitePulseValue = backEnd->colorRead(0);
		bluePulseValue = backEnd->colorRead(1);
		redPulseValue = backEnd->colorRead(2);
		greenPulseValue = backEnd->colorRead(3);
		
		numberOfZeros = numberZeros(); //determine num of zeros in pulseValues array
		
		redBlock = isRed(); blueBlock = isBlue(); brownBlock = isBrown();
		yellowBlock = isYellow(); purpleBlock = isPurple(); greenBlock = isGreen();
		
		result = dominantColor();
	}
	
	return result;
}



/**
 * Fills out pulseValues array with the pulse values
*/
void ColorSensor::pulseRead()	{
	for(int i = 0; i < 4; i++)  {
		throwAway = backEnd->colorRead(0); //Declared and destryoed at each run.
		throwAway = backEnd->colorRead(1);
		throwAway = backEnd->colorRead(2);
		throwAway = backEnd->colorRead(3);
	}
	pulseValues[0] = 
	pulseValues[1] = 
	pulseValues[2] = 
	pulseValues[3] = 
		
}

/**
 * Determines the dominant from various color boolean results
 */
int dominantColor()	{
	int returnResult; //result to return to call in detectColor. 6 means no dominant color
	int numTrue = 0;
		
		if(redBlock)	{
			returnResult = 0;
			numTrue++;
		}
		
		if(blueBlock)	{
			returnResult = 1;
			numTrue++;
		}
		
		if(brownBlock)	{
			returnResult = 2;
			numTrue++;
		}
		
		if(yellowBlock)	{
			returnResult = 3;
			numTrue++;
		}
		
		if(purpleBlock)	{
			returnResult = 4;
			numTrue++;
		}
		
		if(greenBlock)	{
			returnResult = 5;
			numTrue++;
		}
		
		if(numTrue != 1)	{
			returnResult = 6;
		}
		
		return returnResult;
}

/** ---------Color Tests using color Pulse array -------------*/ 

private boolean isRed()	{
	
}



/**
 * Finds absolute value of a
 */
int ColorSensor::absoluteValue(int a)  {
  return ((a*a)/a);
}



#include "ColorSensor.h"
#include "BackEndColorSensor.h"

//Feild Variables
BackEndColorSensor *backEnd;
enum FooSize { RED = 0, BLUE = 1, BROWN = 2, YELLOW = 3, PURPLE = 4, GREEN = 5}

int PulseValues;
bool colorBlocks [6];
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
                    10000 = BLUE
                    2000 = YELLOW
                    300 = RED
                    40 = BROWN
                    5 = PURPLE
 */
int ColorSensor::detectColor() {

    pulseRead()
	
	numberOfZeros = numberZeros();
    
    colorBlocks[0] = isRed(); colorBlocks[1] = isBlue(); colorBlocks[2] = isBrown();
    colorBlocks[3] = isYellow(); colorBlocks[4] = isPurple(); colorBlock[5] = false;//colorBlock[5] = isGreen();
	
	int first = blockColorDetermine(100);
	int second = blockColorDetermine(first);
	
	if((first != 100  && second == 100) || count > 5) {return first}; //Base Case
	else { return detectColor(); } //recursive call
	
}

int ColorSensor::absoluteValue(int a)  {
  return ((a*a)/a);
}

/**
 * Fills out pulseValues array with the pulse values
 *
*/
void ColorSensor::pulseRead()	{
	for(int i = 0; i < 4; i++)  {
		throwAway = backEnd->colorRead(0); //Declared and destryoed at each run.
		throwAway = backEnd->colorRead(1);
		throwAway = backEnd->colorRead(2);
		throwAway = backEnd->colorRead(3);
	}
	pulseValues[0] = backEnd->colorRead(0);
	pulseValues[1] = backEnd->colorRead(1);
	pulseValues[2] = backEnd->colorRead(2);
	pulseValues[3] = backEnd->colorRead(3);
		
}

/**
 * Determines the color of the block
*/
int ColorSensor::blockColorDetermine(int indexToIgnore)	{
	for(int i = 0; i < 6; i++)	{
		if(colorBlocks[i] && i != indexToIgnore) return i;
	}
	return 100;
	}

}








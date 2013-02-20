#include "ColorSensor.h"
#include "BackEndColorSensor.h"

//Feild Variables
BackEndColorSensor *backEnd;
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
	float whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue; //Pulse values
	bool redBlock, blueBlock, brownBlock, yellowBlock, purpleBlock, greenBlock; //Holds result of each color block's tests
	
	while(result == 6)	{ //Possibility for infinite loop if no dominant color found after infinite pulse readings
	
		//Fill up array with color pulse values
		whitePulseValue = backEnd->colorRead(0);
		bluePulseValue = backEnd->colorRead(1);
		redPulseValue = backEnd->colorRead(2);
		greenPulseValue = backEnd->colorRead(3);
		
		int numberOfOnes = numberZeros(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue); //determine num of zeros in pulseValues array
		if(numberOfOnes < 3)	{
			//Determining if test for each color pass; if <Color>Block == T then.. <color> test passed
			redBlock = isRed(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes); 
			blueBlock = isBlue(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes);
			brownBlock = isBrown(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes);
			yellowBlock = isYellow(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes);
			purpleBlock = isPurple(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes); 
			greenBlock = isGreen(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes);
		
			result = dominantColor(redBlock, blueBlock, brownBlock, yellowBlock, purpleBlock, greenBlock); 
		}
		
		else  {
			result = 6; //While loop again (re-read pulse values)
		}
	}
	
	return result;
}

void private throwAwayValues()	{
	int throwAway;
	for(int i = 0; i < 4; i++)  {
		throwAway = backEnd->colorRead(0); //Declared and destryoed at each run.
		throwAway = backEnd->colorRead(1);
		throwAway = backEnd->colorRead(2);
		throwAway = backEnd->colorRead(3);
	}
}

/** 
 * Red block color check
 */       
private bool isRed(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)	{
	if(numOnes == 0)  {
		if((whitePV > 15000.002) && (redPV > 15000.002) && (greenPV > 15000.002))	{
			if(((bluePV * 5.00) < whitePV) && ((bluePV * 5.00) < redPV) && ((bluePV * 5.00) < greenPV))  {
				return true;
			}
		}
	}
		
	else if((absoluteValue(whitePV - 1.00) < 0.001) && (bluePV > 1.002) && (redPV > 1.002) && (greenPV > 1.002))  { //Case where just white is 1.00
	 
		if((redPV > 25000.002) && (greenPV > 25000.002) && (absoluteValue(redPV - greenPV) < (redPV/4.00)))	{
			
			if((redPV > whitePV*5.00) && (greenPV > whitePV*5.00))	{
				return true;
			}
		}
	}
	
	else if((whitePV - 1.00 < 0.001) && (redPV - 1.00 < 0.001) && (greenPV - 1.00 < 0.001) && (bluePV < 10000.00))  {
		return true;
	}
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
float ColorSensor::absoluteValue(float a)  {
  return ((a*a)/a);
}



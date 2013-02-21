#include "ColorSensor.h"
#include "BackEndColorSensor.h"

BackEndColorSensor *backEnd; //BackendColorSensor (used in pulseRead function)

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
		
		int numberOfOnes = calculateNumberOnes(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue); //determine num of zeros in pulseValues array
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

/**
 * Reads 4 pulse values from the sensor and throws them away.
 */
void throwAwayValues()	{
	int throwAway;
	for(int i = 0; i < 4; i++)  {
		throwAway = backEnd->colorRead(0); //Declared and destryoed at each run.
		throwAway = backEnd->colorRead(1);
		throwAway = backEnd->colorRead(2);
		throwAway = backEnd->colorRead(3);
	}
}

calculateNumberOnes(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue);


/** ---------Color Tests using color Pulse array -------------*/ 

/** 
 * Red block color check
 */       
bool isRed(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)	{
	if(numOnes == 0)  {
		if((whitePV > 15000.002) && (redPV > 15000.002) && (greenPV > 15000.002))	{
			if(((bluePV * 5.00) < whitePV) && ((bluePV * 5.00) < redPV) && ((bluePV * 5.00) < greenPV))  {
				return true;
			}
		}
	}
		
	else if((absoluteValue(whitePV - 1.00) < 0.001) && (bluePV > 1.002) && (redPV > 1.002) && (greenPV > 1.002))  { //Case where just white is 1.00
		if((redPV > 25000.002) && (greenPV > 25000.002) && (absoluteValue(redPV - greenPV) < (redPV/4.00)))	{
			if(((whitePV*5.00) < redPV) && ((whitePV*5.00) < greenPV))	{
				return true;
			}
		}
	}
	
	else if((absoluteValue(whitePV - 1.00) < 0.001) && (absoluteValue(redPV - 1.00) < 0.001) && (absoluteValue(greenPV - 1.00) < 0.001) && (bluePV < 10000.00))  {
		return true;
	}
}

/**
 * Blue block check
 * set_aside_logic: (numOnes == 0) && (bluePV > 15000.00) && (redPV > 15000.00) && (greenPV > 15000.00)
 */
bool isBlue(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)	{
	if(numOnes == 0 && (bluePV > whitePV) && (bluePV > redPV) && (bluePV > greenPV))	{
		return true;
	}
}


/**
 * Brown block check
 */
bool isBrown(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)	{
	if((greenPV > 80000.00) || (redPV > 80000.00))	{ //only color with constistant higher than 90k red/green values
		if((bluePV > 35000.00) && (bluePV < 55000.00))  {
			return true;
		}
	}
	
	else if(((whitePV - 1.00) < 0.001) && ((redPV - 1.00) < 0.001) && ((greenPV - 1.00) < 0.001) && (bluePV > 35000.00))  { //Special case of 3 1.00's and blue (becasue really dark color)
		return true;
	}
}

/**
 * Yellow block check
 */
bool isYellow(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)	{
	if((whitePV < 10000.00) && (bluePV < 10000.00) && (redPV < 10000.00) && (greenPV < 10000.00) && (numOnes == 0))	{
		if((absoluteValue(whitePV - redPV) < whitePV/3.00) && (absoluteValue(greenPV - redPV) < greenPV/3.00) && (absoluteValue(whitePV - greenPV) < greenPV/3.00))	{ //white, red, and green values all fairly close
			return true;
		}
	}
}

/**
 * Purple block check
 */
bool isPurple(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)	{
	if ((whitePV > 15000.00) && (bluePV > 15000.00) && (redPV > 15000.00) && (greenPV > 15000.00))	{
		if((absoluteValue(whitePV - redPV) < (redPV/12.00)) && (absoluteValue(whitePV - greenPV) < (whitePV/12.00))) && (absoluteValue(redPV - greenPV) < greenPV/12.00))  {
			if((bluePV < whitePV) && (bluePV < redPV) && (bluePV < greenPV))  {
				if(((bluePV*2.00) > whitePV) && ((bluePV*2.00) > greenPV) && ((bluePV*2.00) > redPV))	{
					return true;
				}
			}
		}
	}
}


/**
 * Green block check
 */
bool isGreen(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)  {
	if((absoluteValue(bluePV - 1.00) < 0.001) && (absoluteValue(greenPV - 1.00) < 0.001))  {
		if((whitePV > 55000.00) && (redPV > 55000.00))  {
			return true;
		}
		
		else if((absoluteValue(whitePV - 1.00) < 0.001) && redPV > 50000.00)  {
			return true;
		}
	}
	
	if((absoluteValue(whitePV - 1.00) < 0.001) && (whitePV > 55000.00) && ((redPV - 1.00) < 0.001) && greenPV > 55.00)  {
		return true;
	}
}

/**
 * Determines the dominant from various color boolean results
 */
int dominantColor()	{
	int returnResult = 6; //result to return to call in detectColor. 6 means no dominant color
	int numTrue = 0; //Number of clor tests these set of pulse values passed
		
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

/**
 * Finds absolute value of parameter a
 */
float ColorSensor::absoluteValue(float a)  {
  return ((a*a)/a);
}





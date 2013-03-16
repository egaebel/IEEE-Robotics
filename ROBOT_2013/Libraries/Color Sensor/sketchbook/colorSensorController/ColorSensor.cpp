#include "ColorSensor.h"
#include "BackEndColorSensor.h"
#include "common.h"

BackEndColorSensor backEnd; //BackendColorSensor (used in pulseRead function)

ColorSensor::ColorSensor()
{
    backEnd = BackEndColorSensor();
    Serial.begin(115200);
    
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
	int whitePulseValue = 0, bluePulseValue = 0, redPulseValue = 0, greenPulseValue = 0; //Pulse values
	int count = 0;	
        
	while(result == 6)	{ //Possibility for infinite loop if no dominant color found after infinite pulse readings
		//Fill up array with color pulse values
		while(whitePulseValue == 0 || (whitePulseValue >= 32768 || bluePulseValue >= 32768 || redPulseValue >= 32768|| greenPulseValue >= 32768))  {
			whitePulseValue = backEnd.colorRead(0);
			bluePulseValue = backEnd.colorRead(1);
			redPulseValue = backEnd.colorRead(2);
			greenPulseValue = backEnd.colorRead(3);
		}
		
		int numberOfOnes = calculateNumberOnes(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue); //determine num of zeros in pulseValues array
		if((numberOfOnes < 4) && (count < 10))	{
			Serial.print("white: ");
			Serial.print(whitePulseValue);
			Serial.print("\tblue: ");
			Serial.print(bluePulseValue);
			Serial.print("\tred: ");
			Serial.print(redPulseValue);
			Serial.print("\tgreen: ");
			Serial.print(greenPulseValue);
			Serial.println();
			
			if(isRed(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes)) {Serial.println("  red");}
			if(isBlue(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes)) {Serial.println("  blue");}
			if(isBrown(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes)) {Serial.println("  brown");}
			if(isYellow(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes)) {Serial.println("  yellow");}
			if(isPurple(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes)) {Serial.println("  purple");}
			if(isGreen(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes)) {Serial.println("  green");}
			//result = dominantColor(redBlock, blueBlock, brownBlock, yellowBlock, purpleBlock, greenBlock);
			count++;
		}
		
		else if(count < 10)  {            
            count++;
			result = 6; //More than 3 ones --> while loop again (re-read pulse values)
		}

        else  {
            result = 100;
        }
	}
	
	return result;
}

int ColorSensor::calculateNumberOnes(int firstPV, int secondPV, int thirdPV, int fourthPV)  {
  int count = 0;
  if(firstPV == 1)  {
    count++;
  }
  if(secondPV == 1) {
    count++;
  }
  if(thirdPV == 1) {
    count++;
  }
  if(fourthPV == 1) {
    count++;
  }
  
  return count;
}

/**
 * Reads 4 pulse values from the sensor and throws them away.
 */
void ColorSensor::throwAwayValues()	{
	int throwAway;
	for(int i = 0; i < 4; i++)  {
		throwAway = backEnd.colorRead(0); //Declared and destryoed at each run.
		throwAway = backEnd.colorRead(1);
		throwAway = backEnd.colorRead(2);
		throwAway = backEnd.colorRead(3);
	}
}

/** ---------Color Tests using color Pulse array -------------*/ 

/** 
 * Red block color check
 */       
bool ColorSensor::isRed(int whitePV, int bluePV, int redPV, int greenPV, int numOnes)	{
	return (withinRange(whitePV, white_AveragePV_RED, range_RED) && withinRange(bluePV, blue_AveragePV_RED, range_RED) 
		   && withinRange(redPV, red_AveragePV_RED, range_RED) && withinRange(greenPV, green_AveragePV_RED, range_RED));
}

/**
 * Blue block check
 */
bool ColorSensor::isBlue(int whitePV, int bluePV, int redPV, int greenPV, int numOnes)	{
	return (withinRange(whitePV, white_AveragePV_BLUE, range_BLUE) && withinRange(bluePV, blue_AveragePV_BLUE, range_BLUE)
           && withinRange(redPV, red_AveragePV_BLUE, range_BLUE) && withinRange(greenPV, green_AveragePV_BLUE, range_BLUE));
}

/**
 * Brown block check
 */
bool ColorSensor::isBrown(int whitePV, int bluePV, int redPV, int greenPV, int numOnes)	{
	return (withinRange(whitePV, white_AveragePV_BROWN, range_BROWN) && withinRange(bluePV, blue_AveragePV_BROWN, range_BROWN) 
		   && withinRange(redPV, red_AveragePV_BROWN, range_BROWN) && withinRange(greenPV, green_AveragePV_BROWN, range_BROWN));
}

/**
 * Yellow block check
 */
bool ColorSensor::isYellow(int whitePV, int bluePV, int redPV, int greenPV, int numOnes)	{
	
	return (withinRange(whitePV, white_AveragePV_YELLOW, range_YELLOW) && withinRange(bluePV, blue_AveragePV_YELLOW, range_YELLOW)
           && withinRange(redPV, red_AveragePV_YELLOW, range_YELLOW) && withinRange(greenPV, green_AveragePV_YELLOW, range_YELLOW));
}

/**
 * Purple block check
 */
bool ColorSensor::isPurple(int whitePV, int bluePV, int redPV, int greenPV, int numOnes)	{
	return (withinRange(whitePV, white_AveragePV_PURPLE, range_PURPLE) && withinRange(bluePV, blue_AveragePV_PURPLE, range_PURPLE)
           && withinRange(redPV, red_AveragePV_PURPLE, range_PURPLE) && withinRange(greenPV, green_AveragePV_PURPLE, range_PURPLE));
}


/**
 * Green block check
 */
bool ColorSensor::isGreen(int whitePV, int bluePV, int redPV, int greenPV, int numOnes)  {
	return (withinRange(whitePV, white_AveragePV_GREEN, range_GREEN) && withinRange(bluePV, blue_AveragePV_GREEN, range_GREEN) 
		   && withinRange(redPV, red_AveragePV_GREEN, range_GREEN) && withinRange(greenPV, green_AveragePV_GREEN, range_GREEN));
}

/**
 * Determines the dominant from various color boolean results
 */
int ColorSensor::dominantColor(bool redBlock, bool blueBlock, bool brownBlock, bool yellowBlock, bool purpleBlock, bool greenBlock)	{
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
 * Checks whether <int> canidate value is within <int> range (inclusive) of the <int> comparisonValue
 */
bool ColorSensor::withinRange(int canidateValue, int comparisonValue, int range)  {
	return (absoluteValue(canidateValue - comparisonValue) <= range);
}

/**
 * Finds absolute value of parameter a
 */
int ColorSensor::absoluteValue(int a)  {
	if(a <= 0) return (-1 * a);
	
	return a;
}

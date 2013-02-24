#include "ColorSensor.h"
#include "BackEndColorSensor.h"

BackEndColorSensor *backEnd; //BackendColorSensor (used in pulseRead function)

ColorSensor::ColorSensor()
{
    backEnd = &BackEndColorSensor();
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
	int whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue; //Pulse values
	int count = 0;	
        
	while(result == 6)	{ //Possibility for infinite loop if no dominant color found after infinite pulse readings
		//Fill up array with color pulse values
		whitePulseValue = backEnd->colorRead(0);
		bluePulseValue = backEnd->colorRead(1);
		redPulseValue = backEnd->colorRead(2);
		greenPulseValue = backEnd->colorRead(3);
		
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
		
			//Determining if test for each color pass; if <Color>Block == T then.. <color> test passed
			bool redBlock = isRed(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes); 
			bool blueBlock = isBlue(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes);
			bool brownBlock = isBrown(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes);
			bool yellowBlock = isYellow(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes);
			bool purpleBlock = isPurple(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes); 
			bool greenBlock = isGreen(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue, numberOfOnes);
		
			// Serial.println("RED:" + redBlock); delay(200);
			// Serial.println("BLUE:" + blueBlock); delay(200);
			// Serial.println("BROWN:" + brownBlock); delay(200);
			// Serial.println("YELLOW:" + yellowBlock); delay(200);
			// Serial.println("PURPLE:" + purpleBlock); delay(200);
			// Serial.println("GREEN:" + greenBlock); delay(200);
			
			if(redBlock) {Serial.println("  red");}
			if(blueBlock) {Serial.println("  blue");}
			if(brownBlock) {Serial.println("  brown");}
			if(yellowBlock) {Serial.println("  yellow");}
			if(purpleBlock) {Serial.println("  purple");}
			if(greenBlock) {Serial.println("  green");}
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
		throwAway = backEnd->colorRead(0); //Declared and destryoed at each run.
		throwAway = backEnd->colorRead(1);
		throwAway = backEnd->colorRead(2);
		throwAway = backEnd->colorRead(3);
	}
}

/** ---------Color Tests using color Pulse array -------------*/ 

/** 
 * Red block color check
 */       
bool ColorSensor::isRed(int whitePV, int bluePV, int redPV, int greenPV, int numOnes)	{
	return false;
}

/**
 * Blue block check
 * set_aside_logic: (numOnes == 0) && (bluePV > 15000.00) && (redPV > 15000.00) && (greenPV > 15000.00)
 */
bool ColorSensor::isBlue(int whitePV, int bluePV, int redPV, int greenPV, int numOnes)	{
	return false;
}

/**
 * Brown block check
 */
bool ColorSensor::isBrown(int whitePV, int bluePV, int redPV, int greenPV, int numOnes)	{
	const int whiteAveragePV_BROWN =  3300.0f;
	const int blueAveragePV_BROWN =  3350.0f;
 	const int redAveragePV_BROWN =  2500.00f;
 	const int greenAveragePV_BROWN =  3350.00f;
	const int acceptedRange_BROWN= 100.00f;
	
	return (withinRange(whitePV, whiteAveragePV_BROWN, acceptedRange_BROWN) && withinRange(bluePV, blueAveragePV_BROWN, acceptedRange_BROWN) 
	&& withinRange(redPV, redAveragePV_BROWN, acceptedRange_BROWN) && withinRange(greenPV, greenAveragePV_BROWN, acceptedRange_BROWN));
}

/**
 * Yellow block check
 */
bool ColorSensor::isYellow(int whitePV, int bluePV, int redPV, int greenPV, int numOnes)	{
	const int whiteAveragePV_YELLOW =  700.0f;
	const int blueAveragePV_YELLOW =  700.0f;
 	const int redAveragePV_YELLOW =  470.00f;
 	const int greenAveragePV_YELLOW =  700.00f;
	const int acceptedRange_YELLOW= 50.00f;
	
	return (withinRange(whitePV, whiteAveragePV_YELLOW, acceptedRange_YELLOW) && withinRange(bluePV, blueAveragePV_YELLOW, acceptedRange_YELLOW) 
		   && withinRange(redPV, redAveragePV_YELLOW, acceptedRange_YELLOW) && withinRange(greenPV, greenAveragePV_YELLOW, acceptedRange_YELLOW));
}

/**
 * Purple block check
 */
bool ColorSensor::isPurple(int whitePV, int bluePV, int redPV, int greenPV, int numOnes)	{
	const int whiteAveragePV_PURPLE = 10000.0f;
	const int blueAveragePV_PURPLE = 10000.0f;
 	const int redAveragePV_PURPLE = 10000.00f;
 	const int greenAveragePV_PURPLE = 10000.00f;
	const int acceptedRange_PURPLE = 100.00f;
	
	return (withinRange(whitePV, whiteAveragePV_PURPLE, acceptedRange_PURPLE) && withinRange(bluePV, blueAveragePV_PURPLE, acceptedRange_PURPLE) 
	&& withinRange(redPV, redAveragePV_PURPLE, acceptedRange_PURPLE) && withinRange(greenPV, greenAveragePV_PURPLE, acceptedRange_PURPLE));  
		
}


/**
 * Green block check
 */
bool ColorSensor::isGreen(int whitePV, int bluePV, int redPV, int greenPV, int numOnes)  {
	const int whiteAveragePV_GREEN =  3100.0f;
	const int blueAveragePV_GREEN =  3100.0f;
 	const int redAveragePV_GREEN =  3000.00f;
 	const int greenAveragePV_GREEN =  3100.00f;
	const int acceptedRange_GREEN = 50.00f;
	
	return (withinRange(whitePV, whiteAveragePV_GREEN, acceptedRange_GREEN) && withinRange(bluePV, blueAveragePV_GREEN, acceptedRange_GREEN) 
	&& withinRange(redPV, redAveragePV_GREEN, acceptedRange_GREEN) && withinRange(greenPV, greenAveragePV_GREEN, acceptedRange_GREEN));
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
 * Checks whether <float> canidate value is within <float> range (inclusive) of the <float> comparisonValue
 */
bool ColorSensor::withinRange(int canidateValue, const int comparisonValue, const int range)  {
	return (absoluteValue(canidateValue - comparisonValue) <= range);
}

/**
 * Finds absolute value of parameter a
 */
int ColorSensor::absoluteValue(int a)  {
  return ((a*a)/a);
}






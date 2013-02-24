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
	float whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue; //Pulse values
	int count = 0;	
        
	while(result == 6)	{ //Possibility for infinite loop if no dominant color found after infinite pulse readings
		//Fill up array with color pulse values
		whitePulseValue = backEnd->colorRead(0);
		bluePulseValue = backEnd->colorRead(1);
		redPulseValue = backEnd->colorRead(2);
		greenPulseValue = backEnd->colorRead(3);
		
		int numberOfOnes = calculateNumberOnes(whitePulseValue, bluePulseValue, redPulseValue, greenPulseValue); //determine num of zeros in pulseValues array
		if((numberOfOnes < 4) && (count < 4))	{
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
  if((firstPV - 1.00) < 0.001)  {
    count++;
  }
  if((secondPV - 1.00) < 0.001)  {
    count++;
  }
  if((thirdPV - 1.00) < 0.001)  {
    count++;
  }
  if((fourthPV - 1.00) < 0.001)  {
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
bool ColorSensor::isRed(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)	{
	if(greaterThan(whitePV, bluePV, whitePV, 0.00f, redPV*6.00f)) {
		return true;
	}
}

/**
 * Blue block check
 * set_aside_logic: (numOnes == 0) && (bluePV > 15000.00) && (redPV > 15000.00) && (greenPV > 15000.00)
 */
bool ColorSensor::isBlue(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)	{
	if(lessThan(whitePV, bluePV, greenPV, 0.00f, redPV) //Check that red is largest value (to differntiate from purple)
	&& lessThan(absoluteValue(whitePV - bluePV), absoluteValue(whitePV - greenPV), absoluteValue(bluePV - greenPV), 0.00f, 1500.00f) //Check that white, blue, and green close in value
	&& greaterThan(absoluteValue(redPV - greenPV), absoluteValue(redPV - bluePV), absoluteValue(redPV - whitePV), 0.00f, 1500.00f) //check that red is not close to green, blue, white
	&& lessThan(whitePV, bluePV, whitePV, 0.00f, redPV*6.00f) //checks not red block's pattern of really small redPV value
	&& lessThan(whitePV, bluePV, redPV, greenPV, 20000.00f)) { //check that all value less 20000
		return true;
	}
}

/**
 * Brown block check
 */
bool ColorSensor::isBrown(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)	{
	if((greenPV > 80000.00f || redPV > 80000.00f) && bluePV > 35000.00f && bluePV < 55000.00f)	{ 
		return true;
	}
	else if(lessThan(whitePV - 1.00f, redPV - 1.00f, greenPV - 1.00f, 0.00f, 1.01f) && bluePV > 35000.00f) { //Special case of 3 1.00's and blue (becasue really dark color)
		return true;
	}
	else if((lessThan(whitePV, bluePV, greenPV, redPV, 40000.00f) || (whitePV > 70000.00f && lessThan(bluePV, greenPV, redPV, 0.00f, 40000.00f)))
	&& greaterThan(whitePV, bluePV, greenPV, 0.00f, redPV*2) && lessThan(whitePV, bluePV, greenPV, 0.00f, redPV*3))  { //Stops case being true with green block
	  //held, don't know if need it for htis case
			return true;
	}
}

/**
 * Yellow block check
 */
bool ColorSensor::isYellow(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)	{
	if(greaterThan(whitePV, bluePV, greenPV, 0.00f, redPV)
	&& lessThan(whitePV, bluePV, greenPV, redPV, 5000)
	&& numOnes == 0)  {
		return true;
	}
}

/**
 * Purple block check
 */
bool ColorSensor::isPurple(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)	{
	if(greaterThan(whitePV, bluePV, greenPV, 0.00f, redPV) //red is smallest value(differentiates from blue
	&& lessThan(whitePV, bluePV, whitePV, 0.00f, redPV*6.00f) //checks not red block's pattern of really small redPV value
	&& greaterThan(whitePV, bluePV, greenPV, 0.00f, 10000)  //checks that larger values than yellow
	&& lessThan(absoluteValue(whitePV - bluePV), absoluteValue(whitePV - greenPV), absoluteValue(bluePV - greenPV), 0.00f, 1500.00f) //Check that white, blue, and green close in value
	&& greaterThan(absoluteValue(redPV - greenPV), absoluteValue(redPV - bluePV), absoluteValue(redPV - whitePV), 0.00f, 1500.00f) //check that red is not close to green, blue, white
	&& lessThan(whitePV, bluePV, redPV, greenPV, 20000.00f)) {
		return true;
	}
}


/**
 * Green block check
 */
bool ColorSensor::isGreen(float whitePV, float bluePV, float redPV, float greenPV, int numOnes)  {
	if(lessThan(absoluteValue(whitePV - bluePV), absoluteValue(greenPV - bluePV), absoluteValue(whitePV - greenPV), 0.00f, 2000.00f)
	&& lessThan(whitePV, bluePV, redPV, greenPV, 35000.00f) && greaterThan(whitePV, bluePV, redPV, greenPV, 19000.00f))  {
		return true;
	}
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
  * returns whether a, b, c, d less than specified number (exclusive)
  * If want only n params -> set other (4-n) params to 0.
  * All params MUST be type Float!
  */
bool ColorSensor::lessThan(float a, float b, float c, float d, float valueToBeLessThan)  {
	
	if(a > valueToBeLessThan && !(absoluteValue(a - 1.00f) < 0.01f))  {
		return false;
	}
	else if(b > valueToBeLessThan && !(absoluteValue(b - 1.00f) < 0.01f))  {
		return false;
	}
	else if(c > valueToBeLessThan && !(absoluteValue(c - 1.00f) < 0.01f))  {
		return false;
	}
	else if(d > valueToBeLessThan && !(absoluteValue(d - 1.00f) < 0.01f))  {
		return false;
	}
	return true;
}

/**
  * returns whether a, b, c, d GREATER than specified number (exclusive)
  * If want only n params -> set other (4-n) params to 0.
  * All params MUST be type Float!
  */
bool ColorSensor::greaterThan(float a, float b, float c, float d, float valueToBeGREATERThan)  {
	
	if(a < valueToBeGREATERThan && !(absoluteValue(a - 1.00f) < 0.01f))  {
		return false;
	}
	else if(b < valueToBeGREATERThan && !(absoluteValue(b - 1.00f) < 0.01f))  {
		return false;
	}
	else if(c < valueToBeGREATERThan && !(absoluteValue(c - 1.00f) < 0.01f))  {
		return false;
	}
	else if(d < valueToBeGREATERThan && !(absoluteValue(d - 1.00f) < 0.01f))  {
		return false;
	}
	return true;
}

/**
 * Finds absolute value of parameter a
 */
float ColorSensor::absoluteValue(float a)  {
  return ((a*a)/a);
}






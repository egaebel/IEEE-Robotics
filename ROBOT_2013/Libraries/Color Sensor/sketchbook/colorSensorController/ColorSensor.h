#ifndef COLORSENSOR_H
#define COLORSENSOR_H


class ColorSensor
{
    public:
        ColorSensor();
        int detectColor();
    protected:
    private:
	void throwAwayValues(); //Reads 4 pulse values from sensor and throws them away.	
        bool isBlue(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
        bool isYellow(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
        bool isBrown(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
        bool isRed(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
        bool isPurple(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
        bool isGreen(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
        int dominantColor(bool redBlock, bool blueBlock, bool brownBlock, bool yellowBlock, bool purpleBlock, bool greenBlock); //Determines the dominant color (color decided the block is)
        float absoluteValue(float a);
        int calculateNumberOnes(int firstPV, int secondPV, int thirdPV, int fourthPV); //returns number of pulse values with value 1.00
		
		bool greaterThan(float a, float b, float c, float d, float valueToBeGREATERThan); //Determines if value if a,b,c,d all GREATER than valueToBeGREATERThan (exclusive) 
		bool lessThan(float a, float b, float c, float d, float valueToBeLessThan); //Determines if value if a,b,c,d all LESS than valueToBeLessThan (exclusive) 
};

#endif // COLORSENSOR_H
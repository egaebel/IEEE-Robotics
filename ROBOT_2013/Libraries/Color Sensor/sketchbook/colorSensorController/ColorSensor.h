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
        bool isBlue(int whitePV, int bluePV, int redPV, int greenPV, int numOnes);
        bool isYellow(int whitePV, int bluePV, int redPV, int greenPV, int numOnes);
        bool isBrown(int whitePV, int bluePV, int redPV, int greenPV, int numOnes);
        bool isRed(int whitePV, int bluePV, int redPV, int greenPV, int numOnes);
        bool isPurple(int whitePV, int bluePV, int redPV, int greenPV, int numOnes);
        bool isGreen(int whitePV, int bluePV, int redPV, int greenPV, int numOnes);
        int dominantColor(bool redBlock, bool blueBlock, bool brownBlock, bool yellowBlock, bool purpleBlock, bool greenBlock); //Determines the dominant color (color decided the block is)
        int absoluteValue(int a);
        int calculateNumberOnes(int firstPV, int secondPV, int thirdPV, int fourthPV); //returns number of pulse values with value 1.00
		bool withinRange(int canidateValue, const int comparisonValue, const int range); //check whether canidate value with range (inclusive) of comparison value
};

#endif // COLORSENSOR_H
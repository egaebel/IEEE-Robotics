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
        
        int dominantColor(); //Determines the dominant color (color decided the block is)
        int absoluteValue(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
};

#endif // COLORSENSOR_H

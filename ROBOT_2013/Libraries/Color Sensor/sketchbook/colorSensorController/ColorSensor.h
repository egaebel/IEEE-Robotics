#ifndef COLORSENSOR_H
#define COLORSENSOR_H


class ColorSensor
{
    public:
        ColorSensor();
        int detectColor(int numberOfPulseReadings, int* ouputArray);
        int detectColor(int* ouputArray);
    protected:
    private:
        bool isBlue(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
        bool isYellow(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
        bool isBrown(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
        bool isRed(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
        bool isPurple(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
        bool absoluteValue(float whitePV, float bluePV, float redPV, float greenPV, int numOnes);
};

#endif // COLORSENSOR_H

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
        int isBlue(int colorPulseValues[4]);
        int isYellow(int colorPulseValues[4]);
        int isBrown(int colorPulseValues[4]);
        int isRed(int colorPulseValues[4]);
        int isPurple(int colorPulseValues[4]);
        int absoluteValue(int a);
};

#endif // COLORSENSOR_H

#ifndef COLORSENSOR_H
#define COLORSENSOR_H


class ColorSensor
{
    public:
        ColorSensor();
        int detectColor(int numberOfPulseReadings);
        int detectColor();
    protected:
    private:
        int ColorSensor::isBlue(int* colorPulseValues[4]);
        int ColorSensor::isYellow(int *colorPulseValues[4]);
        int ColorSensor::isBrown(int *colorPulseValues[4]);
        int ColorSensor::isRed(int *colorPulseValues[4]);
        int ColorSensor::isPurple(int *colorPulseValues[4]);
};

#endif // COLORSENSOR_H

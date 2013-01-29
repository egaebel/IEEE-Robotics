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
        int ColorSensor::isBlue(int *colorPulseValues);
        int ColorSensor::isYellow(int *colorPulseValues);
        int ColorSensor::isBrown(int *colorPulseValues);
        int ColorSensor::isRed(int *colorPulseValues);
        int ColorSensor::isPurple(int *colorPulseValues);
};

#endif // COLORSENSOR_H

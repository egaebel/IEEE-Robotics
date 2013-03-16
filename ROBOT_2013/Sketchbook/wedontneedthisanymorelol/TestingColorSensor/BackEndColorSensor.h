#ifndef BACKENDCOLORSENSOR_H
#define BACKENDCOLORSENSOR_H
#include <Arduino.h>
class BackEndColorSensor
{
    public:
        BackEndColorSensor(); //Nescessary color sensor setup called in constructor
        float detectColor(int color);
        void TCS3200setup();
    private:
        float colorRead(int taosOutPin, int color, boolean LEDstate);
};

#endif // BACKENDCOLORSENSOR_H

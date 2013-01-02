#ifndef BACKENDCOLORSENSOR_H
#define BACKENDCOLORSENSOR_H

using namespace Color;

class BackEndColorSensor
{
    public:
        BackEndColorSensor(); //Nescessary color sensor setup called in constructor
        int detectColor(Color color);
    private:
        void TCS3200setup();
        float colorRead(int taosOutPin, int color, boolean LEDstate);
        void taosMode(int mode);
};

#endif // BACKENDCOLORSENSOR_H

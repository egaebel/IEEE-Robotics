#ifndef BACKENDCOLORSENSOR_H
#define BACKENDCOLORSENSOR_H

class BackEndColorSensor
{
    public:
        BackEndColorSensor(); //Nescessary color sensor setup called in constructor
        int detectColor(int color);
    private:
        void TCS3200setup();
        float colorRead(int taosOutPin, int color, boolean LEDstate);
};

#endif // BACKENDCOLORSENSOR_H

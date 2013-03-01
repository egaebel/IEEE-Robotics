#ifndef BACKENDCOLORSENSOR_H
#define BACKENDCOLORSENSOR_H

#include arduino.h

class BackEndColorSensor
{
    public:
        BackEndColorSensor(); //Nescessary color sensor setup called in constructor
        int colorRead(int color);
    private:
        void TCS3200setup();
};

#endif // BACKENDCOLORSENSOR_H

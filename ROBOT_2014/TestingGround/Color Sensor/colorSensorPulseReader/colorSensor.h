#ifndef COLORSENSOR_H
#define COLORSENSOR_H
#include <Arduino.h>
class ColorSensor
{
    public:
        ColorSensor(); //Nescessary color sensor setup called in constructor
        int colorRead(int color);
        int getColor();
    private:
        void TCS3200setup();
        int S0 = 22;//pinB
int S1 = 24;//pinA //26 on other
int S2 = 26;//pinE //30 on other
int S3 = 28;//pinF //34 on other
int taosOutPin = 30;//pinC //38 on other
int LED = 32;//pinD //42 on other
bool LEDstate = 1; //LED is turned on
        
};

#endif // COLORSENSOR_H

#ifndef COLORSENSOR_H
#define COLORSENSOR_H
#include <Arduino.h>
class ColorSensor
{
   
    public:
        ColorSensor(int S0, int S1, int s2, int S3, int taosOutPin, int LED); //Nescessary color sensor setup called in constructor
        int colorRead(int color);
        int getColor();
    private:
        void TCS3200setup();
        int S0;//pinB //31 on other //digital
        int S1;//pinA //33 on other //digital
        int S2;//pinE //37 on other //digital
        int S3;//pinF //39 on other //digital
        int taosOutPin;//pinC //41 on other
        int LED;//pinD //43 on other
        bool LEDstate; //LED is turned on
        
};

#endif // COLORSENSOR_H

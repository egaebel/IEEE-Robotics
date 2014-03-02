#ifndef COLORSENSOR_H
#define COLORSENSOR_H
#include <Arduino.h>
class ColorSensor
{
   
    public:
        ColorSensor(); //Nescessary color sensor setup called in constructor
        voiid setup(int S0, int S1, int S2, int S3, int out, int LED);
        int colorRead(int color);
        int getColor();
    private:
        void TCS3200setup();
        int S0;//pinB //22 on other //digital
        int S1;//pinA //26 on other //digital
        int S2;//pinE //30 on other //digital
        int S3;//pinF //34 on other //digital
        int taosOutPin;//pinC //38 on other
        int LED;//pinD //42 on other
        bool LEDstate; //LED is turned on
        
};

#endif // COLORSENSOR_H

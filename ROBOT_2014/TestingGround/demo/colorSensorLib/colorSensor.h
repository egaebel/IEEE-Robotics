#ifndef COLORSENSOR_H
#define COLORSENSOR_H
#include <Arduino.h>

enum Color {BLUE = 1, RED, GREEN, WHITE, BLACK};

class ColorSensor
{
   
    public:
        ColorSensor(); //Nescessary color sensor setup called in constructor
        void setup(int S0, int S1, int S2, int S3, int out, int LED);
        int colorRead(Color color, bool useLED = true);
        Color getColor(bool useLED = true);
    private:
        int S0;//pinB //22 on other //digital
        int S1;//pinA //26 on other //digital
        int S2;//pinE //30 on other //digital
        int S3;//pinF //34 on other //digital
        int taosOutPin;//pinC //38 on other
        int LED;//pinD //42 on other
        
};

#endif // COLORSENSOR_H

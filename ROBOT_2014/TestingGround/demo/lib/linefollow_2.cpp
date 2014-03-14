#include "linefollow_2.hpp"

void ParallelLineFollower::setup(unsigned short sensorPin,
								unsigned short PIN_LF_S0, unsigned short PIN_LF_S1, 
                                unsigned short PIN_LF_S2, unsigned short PIN_LF_S3,
								unsigned short PIN_LF_S4, unsigned short PIN_LF_S5, 
                                unsigned short PIN_LF_S6, unsigned short PIN_LF_S7)
{
    Line_Data = 0;
    sensor = sensorPin;
    this->PIN_LF_S0 = PIN_LF_S0;
    this->PIN_LF_S1 = PIN_LF_S1;
    this->PIN_LF_S2 = PIN_LF_S2;
    this->PIN_LF_S3 = PIN_LF_S3;
    this->PIN_LF_S4 = PIN_LF_S4;
    this->PIN_LF_S5 = PIN_LF_S5;
    this->PIN_LF_S6 = PIN_LF_S6;
    this->PIN_LF_S7 = PIN_LF_S7;
    pinMode(sensorPin, OUTPUT);
    digitalWrite(sensorPin, HIGH);
}

//**************    Evaluate whether the robot is centered on the line **********

bool ParallelLineFollower::isCentered(byte& L_bits, byte& R_bits) // Has to be changed as the code doesn't explicitly checks whether it(rover) is centered or not.
{
     Get_Line_Data();
     L_bits = this->L_bits;
     R_bits = this->R_bits;

     return (L_bits == R_bits) && (L_bits == 1); //L_bits == R_bits is ambiguous as it also happens when rover is over black part of the course (L_bits==R_bits == 0)
}

//**************    Evaluate whether the robot is on an intersecting line *******

bool ParallelLineFollower::intersection(byte& L_bits, byte& R_bits)
{
    Get_Line_Data();
    L_bits = this->L_bits;
    R_bits = this->R_bits;

    return ( Line_Data == 0xFF ) ;
}


//*************** Get Data from 8 Arduino Digital Pins***************************

void ParallelLineFollower::Get_Line_Data()
{
    R_bits = (!digitalRead(PIN_LF_S0) << 3) + (!digitalRead(PIN_LF_S1) << 2) + (!digitalRead(PIN_LF_S2) << 1) + (!digitalRead(PIN_LF_S3) << 0);
    L_bits = (!digitalRead(PIN_LF_S4) << 0) + (!digitalRead(PIN_LF_S5) << 1) + (!digitalRead(PIN_LF_S6) << 2) + (!digitalRead(PIN_LF_S7) << 3);
    Line_Data = (L_bits << 4) + (R_bits);
    
    return;
}

void ParallelLineFollower::Get_Line_Data(byte& L_bits, byte& R_bits) {

    this->Get_Line_Data();
    L_bits = this->L_bits;
    R_bits = this->R_bits;
}

#include "linefollow.hpp"

void LineFollower::setup(unsigned short loadPin, 
                            unsigned short sensorPin)
{
    Line_Data = 0;
    Load = loadPin;
    sensor = sensorPin;
}

//**************    Evaluate whether the robot is centered on the line **********

bool LineFollower::isCentered(byte& L_bits, byte& R_bits) // Has to be changed as the code doesn't explicitly checks whether it(rover) is centered or not.
{
    Get_Line_Data();
    L_bits = this->L_bits;
    R_bits = this->R_bits;

    return (L_bits == R_bits) && (L_bits == 1); //L_bits == R_bits is ambiguous as it also happens when rover is over black part of the course (L_bits==R_bits == 0)
}

//**************    Evaluate whether the robot is on an intersecting line *******

bool LineFollower::intersection(byte& L_bits, byte& R_bits)
{
    Get_Line_Data();
    L_bits = this->L_bits;
    R_bits = this->R_bits;

    return ( Line_Data == 0xFF ) ;
}


//*************** Get Data from 8-bit Shift Register***************************

void LineFollower::Get_Line_Data()
{
    digitalWrite(Load, LOW); //Let the data get into shift register IC
    delayMicroseconds(10);
    digitalWrite(Load, HIGH); //Loads data into the serial register for output
    Line_Data = SPI.transfer(0x00); // Receives serially 8 bits into Line_Data variable
    delay(10);
    L_bits = Line_Data & 0xF0; //Get left most 4 bits
    L_bits >>= 4;
    R_bits = Line_Data & 0x0F;
    
    return;
}

void LineFollower::Get_Line_Data(byte& L_bits, byte& R_bits) {

    this->Get_Line_Data();
    L_bits = this->L_bits;
    R_bits = this->R_bits;
}
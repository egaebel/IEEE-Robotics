//**************************IMPORTANT************************************
//                     For ARDUINO MEGA 2560:
//
// Connect ShiftRegister IC CLOCK pin(2) to digital PIN #52 (SCK)!!!
// Coonect ShiftRegister IC inverted OUTPUT pin(7) to digital PIN #50(MISO)

#include<SPI.h>

// -----[ I/O Definitions ]-------------------------------------------------

const short Load    = 22; // Controls ShiftRegister's Shift/Load pin(1)
const short sensor  = 24; // Controls LineFollower's Enable pin
const short L_side_pin = 10; // PWM pin for left treck
const short R_side_pin = 9;  // PWM pin for right treck

//-----[ Variables ]-------------------------------------------------------

short L_bits;
short R_bits;
short L_PWM;
short R_PWM;
byte  Line_Data;
byte  RLine_Data;

void setup()
{
  delay(1500);
  
  Serial.begin(9600);
  SPI.begin();

  pinMode(Load, OUTPUT);
  digitalWrite(Load, HIGH);
  pinMode(sensor, OUTPUT);
  digitalWrite(sensor, HIGH); //activates LineFollower Kit
  
  pinMode(L_side_pin, OUTPUT);
  pinMode(L_side_pin, OUTPUT);

do
 {
  Serial.println("Waiting for the line being detected");
  Get_Line_Data();
/*  Serial.print("L_bits:\t");
  Serial.println(L_bits, BIN);
  Serial.print("R_bits:\t");
  Serial.println(R_bits, BIN);
  delay(1500);
*/
 } while( (L_bits == 0) && (R_bits == 0) );

}  


void loop() 
{
 Get_Line_Data();
 
 if (L_bits > R_bits)
 {
   L_PWM = 255 / L_bits;
   R_PWM = 255;
 }
 else if (R_bits > L_bits)
 {
   L_PWM = 255;
   R_PWM = 255 / R_bits;
 }
 else
 {
   L_PWM = 255;
   R_PWM = 255;
 }
 Move_Control(L_PWM, R_PWM);

}

//*************** Get Data from 8-bit Shift Register***************************

void Get_Line_Data()
{
  digitalWrite(Load, LOW); //Let the data get into shift register IC
  digitalWrite(Load, HIGH); //Loads data into the serial register for output
  Line_Data = SPI.transfer(0); // Recieves serialy 8 bits into Line_Data variable
//  Serial.println(Line_Data, BIN);
  
  L_bits = Line_Data & B11110000; //Get left most 4 bits
  L_bits >>= 4;
  
  RLine_Data = BitReverse(Line_Data); // Reverse Line_Data
  
  R_bits = RLine_Data & B11110000;    // Get originally right most 4 bits but now reversed
  R_bits >>= 4;  
}

//**********Bitwise reverse of 8-bit number Ex: %01100101 will be %10100110****

byte BitReverse(byte bData)
{
  byte lookup[] = { 0, 8,  4, 12, 
                    2, 10, 6, 14 , 
                    1, 9,  5, 13,
                    3, 11, 7, 15 };
  byte ret_val = (byte)(((lookup[(bData & 0x0F)]) << 4) + lookup[((bData & 0xF0) >> 4)]);
return ret_val;
}

//***************Direction Control********************************************

void Move_Control(short L_PWM, short R_PWM)
{
  analogWrite(L_side_pin, L_PWM);
  analogWrite(R_side_pin, R_PWM);
}

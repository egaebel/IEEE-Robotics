//Sergey Sabirov
//Electrical Engineering Junior, Class 2015
//
//**************************IMPORTANT************************************
//                     For ARDUINO MEGA 2560:
//
// Connect ShiftRegister IC CLOCK pin(2) to digital PIN #52 (SCK)!!!
// Coonect ShiftRegister IC inverted OUTPUT pin(7) to digital PIN #50(MISO)
//                     For 74HC165 IC
// Do not forget to connect PIN15 (Clock Inhibit) to the ground!

#include <SPI.h>

// -----[ I/O Definitions ]-------------------------------------------------

const short Load    = 22; // Controls ShiftRegister's Shift/Load pin(1)
const short sensor  = 24; // Controls LineFollower's Enable pin
const short L_side_pin = 10; // PWM pin for left treck
const short R_side_pin = 9;  // PWM pin for right treck
const short Dir_Right_Side  =  30; // Controls direction of the right track(LOW - forward, HIGH - Reverse)
const short Dir_Left_Side  =   31; // Controls direction of the left track (LOW - forward, HIGH - Reverse)
const byte max_speed = 100;   // PWM value 0-255

//-----[ Variables ]-------------------------------------------------------

byte L_bits;
byte R_bits;
short L_PWM;
short R_PWM;
short Gate_flag = 0;
short U_Turn_flag = 0;
short Num_LT = 0;
short Hor_Line_pass = 1;
byte  Line_Data;

int s=0;
int m=0;
int i;

void setup()
{
  delay(2000);
  
  Serial.begin(9600);
  
  SPI::begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);

  pinMode(Load, OUTPUT);
  digitalWrite(Load, HIGH);
  pinMode(sensor, OUTPUT);
  digitalWrite(sensor, HIGH); //activates LineFollower Kit
  
  pinMode(L_side_pin, OUTPUT);
  pinMode(R_side_pin, OUTPUT);
  pinMode(Dir_Right_Side, OUTPUT);
  pinMode(Dir_Left_Side, OUTPUT);

do
 {
  Get_Line_Data();
 } while(Line_Data == 0);

}  

void loop() 
{
 Get_Line_Data();
 
 if ( (L_bits >= B0000111) && (R_bits >= B00000111) )// Once rover is over the wide white line,
 {                                                    // check if it passed it once
   if (Gate_flag == 0)                                // If not, then...
   {
     do                                                //Let the rover drive over the cross of white lines
     {
       Get_Line_Data();
     } while( (L_bits > B00000011) && (R_bits > B00000011) );// and wait until Left and Right bits are less than %0011
     Gate_flag = 1;                                   // Set a flag that now it did pass
   }
   else                                               // If rover passed once the cross of white lines,
   {                                                  // then it should make a left turn onto it
     Hor_Line_pass += 1;
     if (Hor_Line_pass == 6)
     {
       Halt();
     }
     Left_Turn();
     Demo_Run(); //Just for Demo Run
   }
 }
   Get_Line_Data();
   Follow_the_line();
//return;
}

//*************** Get Data from 8-bit Shift Register***************************

void Get_Line_Data()
{
  digitalWrite(Load, LOW); //Let the data get into shift register IC
  delayMicroseconds(10);
  digitalWrite(Load, HIGH); //Loads data into the serial register for output
  Line_Data = SPI.transfer(0x00); // Receives serially 8 bits into Line_Data variable
  delay(10);
  L_bits = Line_Data&B11110000; //Get left most 4 bits
  L_bits >>= 4;
  R_bits = Line_Data&B00001111;
  
return;
}

//***************Following the Line*******************************************

void Follow_the_line()
{
  if (L_bits > R_bits)
   {
     L_PWM = max_speed / L_bits;
     R_PWM = max_speed;
   }
  else if (R_bits > L_bits)
   {
     L_PWM = max_speed;
     R_PWM = max_speed / R_bits;
   }
  else
   {
     L_PWM = max_speed;
     R_PWM = max_speed;
   }
   Move_Control(L_PWM, R_PWM);
return;
}

//***************Direction Control********************************************

void Move_Control(short L_PWM, short R_PWM)
{
  digitalWrite(Dir_Left_Side, LOW);
  digitalWrite(Dir_Right_Side,LOW);
  analogWrite(L_side_pin, L_PWM);
  analogWrite(R_side_pin, R_PWM);
//  delay(10);
 return;
}

//***************Executing Left Turn*******************************************
void Left_Turn()
{
  Num_LT = Num_LT + 1;
//  Serial.println(Num_LT);
  U_Turn_flag = Num_LT % 2;
  do                                                //Let the rover drive over the cross of white lines
  {
    Get_Line_Data();
  } while( (L_bits > B00000011) && (R_bits > B00000011) );// and wait until Left and Right bits are less than %0011
  
  for(i=1; i<=27; i++) //Lets the rover to move a bit further after it passed the intersection before turning to the left 90 degree
  {
    Get_Line_Data();
    Follow_the_line();
  }
  digitalWrite(Dir_Left_Side, HIGH);
  digitalWrite(Dir_Right_Side, LOW);
  analogWrite(L_side_pin, max_speed);
  analogWrite(R_side_pin, max_speed);
  delay(800);
  do
  {
    Get_Line_Data();
  } while( (L_bits >= R_bits) || (R_bits==0) );
  
  analogWrite(L_side_pin, 0);
  analogWrite(R_side_pin, 0);
  delay(25); //Let motors complete the stop
return;
}

//*****************180 degree turn************************************************

void U_Turn()
{
  digitalWrite(Dir_Left_Side, HIGH);
  digitalWrite(Dir_Right_Side, LOW);
  analogWrite(L_side_pin, max_speed);
  analogWrite(R_side_pin, max_speed);
  delay(1000);
  do
  {
    Get_Line_Data();
  } while( (L_bits >= R_bits) || (R_bits==0) );
  analogWrite(L_side_pin, 0);
  analogWrite(R_side_pin, 0);
  delay(25); //Let motors complete the stop
return;
}

//*********************Demo Run********************************************************
void Demo_Run()
{
  if (U_Turn_flag == 1) //For DEMO run through the course
  {
//    Serial.println("About to make a U-turn");
    for(i=0; i<=350; i++) //Number of iterations should be adjusted with the speed!
    {
      Get_Line_Data();
      Follow_the_line();
    }
    U_Turn();
  }
return;
}

//**********************Complete Stop***************************************************
void Halt()
{
  analogWrite(L_side_pin, 0);
  analogWrite(R_side_pin, 0);
  do
  {
  }while(1);
}

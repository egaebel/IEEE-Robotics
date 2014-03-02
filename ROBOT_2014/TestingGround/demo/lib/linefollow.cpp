#include "C:\Users\ethan\Documents\ACTUAL My Documents\Robotics Team\IEEE-Robotics\ROBOT_2014\TestingGround\demo\linefollow.hpp"

//#define DEBUG

LineFollower::LineFollower(unsigned short loadPin, unsigned short sensorPin)
{
  #ifdef DEBUG_STANDALONE
  Gate_flag = 0;
  U_Turn_flag = 0;
  Num_LT = 0;
  Hor_Line_pass = 1;
  #endif
  Line_Data = 0;
  load = loadPin;
  sensor = sensorPin;
}

//**************  Evaluate whether the robot is centered on the line **********

bool LineFollower::isCentered(byte& L_bits, byte& R_bits) // Has to be changed as the code doesn't explicitly checks whether it(rover) is centered or not.
{
   Get_Line_Data();
   L_bits = this->L_bits;
   R_bits = this->R_bits;

   return (L_bits == R_bits) && (L_bits); //L_bits == R_bits is ambiguous as it also happens when rover is over black part of the course (L_bits==R_bits == 0)
}

//**************  Evaluate whether the robot is on an intersecting line *******

bool LineFollower::intersection(byte& L_bits, byte& R_bits)
{
  Get_Line_Data();
  L_bits = this->L_bits;
  R_bits = this->R_bits;
  return ( Line_Data == 0xFF );
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
  
  #ifdef DEBUG					//Debugging creates unnecessary delays which should be avoided if rover runs on timings and not on encoders!
  Serial.println(Line_Data);
  #endif
  return;
}

void LineFollower::Get_Line_Data(byte& L_bits, byte& R_bits) {

  this->Get_Line_Data();
  L_bits = this->L_bits;
  R_bits = this->R_bits;
}


// ****** Method graveyard below ******
// ****** Methods below this point will need preprocessor symbol DEBUG to be defined *****

// setup() and loop() functions for running as a standalone sketch

#ifdef DEBUG_STANDALONE

LineFollower lf;
void setup() {lf.setup();};
void loop() {lf.loop();};

#endif

/* These methods are for use when testing this as a standalone Arduino sketch
 *
 * setup(), loop()
 *
 */
 
#ifdef DEBUG_STANDALONE

void LineFollower::setup()
{
  delay(2000);
  
  Serial.begin(9600);
  
  SPI.begin();
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
#ifdef DEBUG
  Serial.println("waiting for data");
#endif
 } while(Line_Data == 0);

}

void LineFollower::loop() 
{
 //Serial.println("looping");
 Get_Line_Data();
 
 if ( (L_bits >= 0x07) && (R_bits >= 0x07) )		  // Once rover is over the wide white line,
 {                                                    // check if it passed it once
   if (Gate_flag == 0)                                // If not, then...
   {
     do                                                //Let the rover drive over the cross of white lines
     {
       Get_Line_Data();
     } while( (L_bits > 0x03) && (R_bits > 0x03) );// and wait until Left and Right bits are less than %0011
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

/* NOTE: these methods are part of the demo code.
 *
 * Follow_the_line(), Move_Control(), Left_Turn(), U_Turn(), Demo_Run(), Halt()
 *
 * Motor control should be handled outside this class 
 */


//***************Following the Line*******************************************
void LineFollower::Follow_the_line()
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

void LineFollower::Move_Control(short L_PWM, short R_PWM)
{
  //Serial.println(L_PWM);
  //Serial.println(R_PWM);
  digitalWrite(Dir_Left_Side, LOW);
  digitalWrite(Dir_Right_Side,LOW);
  analogWrite(L_side_pin, L_PWM);
  analogWrite(R_side_pin, R_PWM);
//  delay(10);
 return;
}

//***************Executing Left Turn*******************************************
void LineFollower::Left_Turn()
{
  Num_LT = Num_LT + 1;
//  Serial.println(Num_LT);
  U_Turn_flag = Num_LT % 2;
  do                                                //Let the rover drive over the cross of white lines
  {
    Get_Line_Data();
  } while( (L_bits > 0x03) && (R_bits > 0x03) );// and wait until Left and Right bits are less than %0011
  
  for(int i=1; i<=27; i++) //Lets the rover to move a bit further after it passed the intersection before turning to the left 90 degree
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

void LineFollower::U_Turn()
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
void LineFollower::Demo_Run()
{
  if (U_Turn_flag == 1) //For DEMO run through the course
  {
//    Serial.println("About to make a U-turn");
    for(int i=0; i<=350; i++) //Number of iterations should be adjusted with the speed!
    {
      Get_Line_Data();
      Follow_the_line();
    }
    U_Turn();
  }
return;
}

//**********************Complete Stop***************************************************
void LineFollower::Halt()
{
  analogWrite(L_side_pin, 0);
  analogWrite(R_side_pin, 0);
  do
  {
  }while(1);
}

#endif

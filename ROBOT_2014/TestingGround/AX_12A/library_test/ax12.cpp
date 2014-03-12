#include "ax12.h"

void ax12::setup()
{
    Serial3.begin(1000000); // Servo Communication Speed
    delay(2);
    transmit();
    delay(1);
    /*
    // set baud rate to 250000
    reg_write_2_byte(broadcast_id, baud_rate, 7);
    Action(broadcast_id);
    delay(2);
    Serial3.end();
    delay(1);
    Serial3.begin(250000);
    delay(1);
    */
    // set moving speed to maximum
    reg_write_2_byte(broadcast_id, moving_speed, 0xff);
    Action(broadcast_id);
    // set initial position to zero
    reg_write_2_byte(broadcast_id, goal_position, 0);
    Action(broadcast_id);
    recieve();
    delay(3000);
}

int ax12::setAngle(int angle)
{
  if(angle < 0 || angle > 1023)
  {
    return -1;
  }
  delay(1);
  transmit();
  delay(1);
  reg_write_2_byte(broadcast_id,goal_position,angle);// put location value into buffer
  Action(broadcast_id); //execute buffer
  recieve(); //enable receiving
  delay(1);
  return 1;
}


void ax12::transmit()
{
  bitSet(UCSR3B,3); // Sets Tx pin
  bitClear(UCSR3B,4); // Clear Rx pin
  bitClear(UCSR3B,7); // Disable Rx Interrupt
}

void ax12::recieve()
{
  bitClear(UCSR3B,3); // Clear Tx pin
  bitSet(UCSR3B,4);  // Set Rx Pin
  bitSet(UCSR3B,7);  // Allows Rx Interrupt 
}


void ax12::reg_write_1_byte(int id, int location, int val)
{
  int length = 4;
  int checksum = ~((id + length + write_ins + location + (val&0xFF))%256); //Checksum Value
  Serial3.write(0xFF); // Starts instruction packet
  Serial3.write(0xFF);
  Serial3.write(id);
  Serial3.write(length);
  Serial3.write(write_ins);
  Serial3.write(location); 
  Serial3.write(val&0xFF); // Lower Byte 
  Serial3.write(checksum);
}
/*---------------------------------------
 Function to reg_write to a 2 byte register
 location = what register to write to
 val = what value to write to register
 ---------------------------------------*/
void ax12::reg_write_2_byte(int id, int location,int val)
{
  int length = 5; // length of 2-byte instruction is 5
  int checksum = ~((id + length + reg_write_ins + location + (val&0xFF) + ((val&0xFF00) >> 8))%256); //Checksum Value
  Serial3.write(0xFF); // Starts instruction packet
  Serial3.write(0xFF);
  Serial3.write(id);
  Serial3.write(length);
  Serial3.write(reg_write_ins);
  Serial3.write(location); 
  Serial3.write(val&0xFF); // Lower Byte 
  Serial3.write((val&0xFF00) >> 8); // Upper Byte 
  Serial3.write(checksum);
}

void ax12::reset(int id)
{
  int length = 0x2; // length of 2-byte instruction is 5
  int checksum = ~((id + length + reset_ins)%256); //Checksum Value
  Serial3.write(0xFF); // Starts instruction packet
  Serial3.write(0xFF);
  Serial3.write(id);
  Serial3.write(length);
  Serial3.write(reset_ins);
  Serial3.write(checksum);
}

/*---------------------------------------
 Action Function to perform Reg_Write Functions
 ---------------------------------------*/
void ax12::Action(int id)
{
  int length = 2;
  int checksum = ~((id + length + action_ins)%256);
  Serial3.write(0xFF);
  Serial3.write(0xFF);
  Serial3.write(id); // Broadcast(0xFE) is used when sending action to two Dynamixels 
  Serial3.write(length);
  Serial3.write(action_ins);
  Serial3.write(checksum);
}

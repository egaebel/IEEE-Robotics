/* This Code is used with an Arduino Mega to communicate with Dynamixel
with Dynamixel AX-12A Servo Motor. All commands use Serial 3 to write 
to servo and Serial to write to pc for purpose of recieving status packets
*/

// To use this code wire the AX-12A servo with power between 9V to 12V, a shared ground with arduino, and pins 14 and 15 connected to the same Tx line of the servo
// Use the below function to set the desired location in a global variable

// location is a value which corresponds to the set position of a servo
// a value of 0 is 0 degrees while a value of 3ff is 300 degrees 
// 300 to 360 is invalid angle

int location = 0x3ff;

void setLocation(int temp_location)
{
  if(temp_location > 0x3ff || temp_location < 0)
    Serial.println("invalid angle");
  else 
    location = temp_location;
}




// instruction table for Dynamixel AX-12A
//---------------------------------------
#define ping_ins 1 // checked
#define read_ins 2 // checked
#define write_ins 3 // Checked
#define reg_write_ins 4 // Checked 
#define action_ins 5 // Checked
#define reset_ins 6 // Checked 
#define sync_write_ins 131 

//---------------------------------------

//registers table for Dynamixel AX-12A
//---------------------------------------
#define servo_id 3
#define baud_rate 4
#define max_torque 14
#define alarm_led 17
#define alarm_shutdown 18
#define torque_enable 24
#define goal_position 30
#define moving_speed 32
#define current_position 36
#define present_temp 43
//---------------------------------------

// Code Begins...

int length,temp,checksum,N,L; // used for status packet & return packet
int old_val = 0;


int present_val = 0;


int id = 1;
int analogPin = A0;  // 5 Potenimeters. each 1 controls 1 motor
int first = 1;

int value;

void setup()
{
  Serial3.begin(1000000); // Servo Communication Speed
  Serial.begin(9600); // Communication speed Arduino/PC
}
//------------------------------------

void loop()
{
    transmit(); //enable trasmission
   if(first)
   { 
     // set moving speed to maximum
     reg_write_2_byte(1,moving_speed,0xff);
     Action(0xFE);
     first = 0;
     recieve();
     delay(1);
     
     first = 0;
     
     // commented out, set servo to desired initial location
//     delay(1);
//     transmit();
//     delay(10);
//     reg_write_2_byte(1,goal_position,0x1fd);// put location value into buffer
//     Action(0x01); //execute buffer
//     recieve(); //enable recieving
   }
   
   delay(1);
   transmit();
   delay(1);
   reg_write_2_byte(1,goal_position,location);// put location value into buffer
   Action(0xFE); //execute buffer
   recieve(); //enable recieving
   delay(1);
   
//   transmit();
//   Serial3.flush();
//   send_read_bytes(1, current_position, 2);
//   //Action(0x01);
//   recieve();
//   delay(10); 
   
}


void transmit()
{
  bitSet(UCSR3B,3); // Sets Tx pin
  bitClear(UCSR3B,4); // Clear Rx pin
  bitClear(UCSR3B,7); // Disable Rx Interrupt
}

void recieve()
{
  bitClear(UCSR3B,3); // Clear Tx pin
  bitSet(UCSR3B,4);  // Set Rx Pin
  bitSet(UCSR3B,7);  // Allows Rx Interrupt 
}


//Recieve Interrupt Subroutine

void serialEvent3()
{
  //temp =Serial3.read();
  //Serial.print(temp,HEX); // prints incoming return packet bit
  //Serial.print("\n");
   char message_bytes[16] = { };
   Serial3.readBytes(message_bytes, 16);
   int current_place = 0;
   Serial.print("Message Bytes: ");
   for(int idx = 0; idx < 16; idx++)
   {
     Serial.print(" ");
     Serial.print((byte)message_bytes[idx]);
   }
   Serial.println("");
}


void reg_write_1_byte(int id, int location, int val)
{
  length = 4;
  checksum = ~((id + length + write_ins + location + (val&0xFF))%256); //Checksum Value
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
void reg_write_2_byte(int id, int location,int val)
{
  length = 5; // length of 2-byte instruction is 5
  checksum = ~((id + length + reg_write_ins + location + (val&0xFF) + ((val&0xFF00) >> 8))%256); //Checksum Value
  Serial3.write(0xFF); // Starts instruction packet
  Serial3.write(0xFF);
  Serial3.write(id);
  Serial3.write(length);
  Serial3.write(reg_write_ins);
  Serial3.write(location); 
  Serial3.write(val&0xFF); // Lower Byte 
  Serial3.write((val&0xFF00) >> 8); // Upper Byte 
  Serial3.write(checksum);
  
  Serial.println("Write Message:");
  Serial.print((byte)0xFF); // Starts instruction packet
  Serial.print(" ");
  Serial.print((byte)0xFF);
  Serial.print(" ");
  Serial.print((byte)id);
  Serial.print(" ");
  Serial.print((byte)length);
  Serial.print(" ");
  Serial.print((byte)reg_write_ins);
  Serial.print(" ");
  Serial.print((byte)location);
  Serial.print(" "); 
  Serial.print((byte)val&0xFF); // Lower Byte
  Serial.print(" "); 
  Serial.print((byte)(val&0xFF00) >> 8); // Upper Byte
  Serial.print(" "); 
  Serial.print((byte)checksum);
  Serial.println("");
  
  
}

void reset(int id)
{
  length = 0x2; // length of 2-byte instruction is 5
  checksum = ~((id + length + reset_ins)%256); //Checksum Value
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
void Action(int id)
{
  length = 2;
  checksum = ~((id + length + action_ins)%256);
  Serial3.write(0xFF);
  Serial3.write(0xFF);
  Serial3.write(id); // Broadcast(0xFE) is used when sending action to two Dynamixels 
  Serial3.write(length);
  Serial3.write(action_ins);
  Serial3.write(checksum);
}

void send_read_bytes(int id, int location, int read_bytes)
{
  length = 4; // length of 2-byte read instruction is 5
  checksum = ~((id + length + read_ins + location + read_bytes)%256); //Checksum Value
  Serial3.write(0xFF); // Starts instruction packet
  Serial3.write(0xFF);
  Serial3.write(id);
  Serial3.write(length);
  Serial3.write(read_ins);
  Serial3.write(location);
  Serial3.write(read_bytes); 
  Serial3.write(checksum);
  
  Serial.println("Read Message:");
  Serial.print((byte)0xFF); // Starts instruction packet
  Serial.print(" ");
  Serial.print((byte)0xFF);
  Serial.print(" ");
  Serial.print((byte)id);
  Serial.print(" ");
  Serial.print((byte)length);
  Serial.print(" ");
  Serial.print((byte)read_ins);
  Serial.print(" ");
  Serial.print((byte)location);
  Serial.print(" "); 
  Serial.print((byte)read_bytes); // Lower Byte
  Serial.print(" "); 
  Serial.print((byte)checksum);
  Serial.println("");
  
  
}

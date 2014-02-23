/* This Code is used with an Arduino Mega to communicate with Dynamixel
with Dynamixel AX-12A Servo Motor. All commands use Serial 3 to write 
to servo and Serial to write to pc for purpose of recieving status packets
*/

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
//---------------------------------------

// Code Begins...

int length,location,temp,checksum,N,L; // used for status packet & return packet
int old_val = 0;


int present_val = 0;


int id = 1;
int analogPin = A0;  // 5 Potenimeters. each 1 controls 1 motor

int value;

boolean inPosition = true;
int plength = 2;
int positions [2] = {1023, 0};
int counter = 0;

void setup()
{
  Serial3.begin(1000000); // Servo Communication Speed
  Serial.begin(9600); // Communication speed Arduino/PC
}
//------------------------------------

void loop()
{

  location = goal_position;
  //present_val = analogRead(analogPin); // read value of A0
  transmit();
  Serial3.flush();
  send_read_bytes(1, current_position, 2);
  recieve();
  char message_bytes[8] = { };
  Serial3.readBytes(message_bytes, 8);
  int current_place = 0;
  //BitShiftCombine(message_bytes[5], message_bytes[6]);
  Serial.print("Message Bytes: ");
  for(int idx = 0; idx < 8; idx++)
  {
    Serial.print(" ");
    Serial.print((byte)message_bytes[idx]);
  }
  Serial.println(""); 
  Serial.println("Current Place: " + String(current_place));
  /*
  if(current_place == positions[counter] )
  {
    Serial.println("Finished Move: " + counter);
    counter = (counter + 1)% plength;
  }
  */
  //if ((present_val != old_val)) // 
  { 
    old_val = present_val; // old_val to present_val
    transmit(); //enable trasmission
   
   // Reg_Write Instructions
  //------------------------------------
   reg_write_2_byte(1,moving_speed,0x3ff);
   Action(0xFE);
   
   reg_write_2_byte(1,location,positions[counter]);// put location value into buffer
   Action(0xFE); //execute buffer

   recieve(); //enable recieving
    //while(1);
  }

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
  temp =Serial3.read();
  //Serial.print(temp,HEX); // prints incoming return packet bit 
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
}

/*---------------------------------------
 Function to send read signal for an array of byte registers
 location = what register to read at
 num_bytes = number of bytes to read
 ---------------------------------------*/
void send_read_bytes(int id, int location, int read_bytes)
{
  length = 3 + read_bytes; // length of 2-byte read instruction is 5
  checksum = ~((id + length + read_ins + location + read_bytes)%256); //Checksum Value
  Serial3.write(0xFF); // Starts instruction packet
  Serial3.write(0xFF);
  Serial3.write(id);
  Serial3.write(length);
  Serial3.write(read_ins);
  Serial3.write(location);
  Serial3.write(read_bytes); 
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

int BitShiftCombine( unsigned char x_high, unsigned char x_low)
{
  int combined;
  combined = x_high;              //send x_high to rightmost 8 bits
  combined = combined<<8;         //shift x_high over to leftmost 8 bits
  combined |= x_low;                 //logical OR keeps x_high intact in combined and fills in rightmost 8 bits
  return combined;
}

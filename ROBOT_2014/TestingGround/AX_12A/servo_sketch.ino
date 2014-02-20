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
//---------------------------------------

// Code Begins...

int length,location,temp,checksum,N,L; // used for status packet & return packet
int old_val = 0;


int present_val = 0;


int id = 1;
int analogPin = A0;  // 5 Potenimeters. each 1 controls 1 motor

int value;

void setup()
{
  Serial3.begin(1000000); // Servo Communication Speed
  Serial.begin(9600); // Communication speed Arduino/PC
}
//------------------------------------

void loop()
{

  location = goal_position;
  present_val = analogRead(analogPin); // read value of A0
  
  //if ((present_val != old_val)) // 
  { 
    old_val = present_val; // old_val to present_val
    transmit(); //enable trasmission
   
   // Reg_Write Instructions
  //------------------------------------
   reg_write_2_byte(1,moving_speed,0x3ff);
   Action(0xFE);
   reg_write_2_byte(1,location,0x3ff);// put location value into buffer

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
  Serial.print(temp,HEX); // prints incoming return packet bit 
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

#include <pins.h>
#include <motors.h>
#include <linefollow.hpp>
#include <colorSensor.h>

//enum State { MAIN_LINE = 1, STRAIGHT_LINE_START, FIRE, STRAIGHT_LINE_END, CURVED_LINE_START, CURVED_LINE_END };

typedef enum
{
  FOLLOW_STRAIGHT_LINE,
  TURN_LEFT  
} j_state;

//Hardware interfaces
static Motors motors;
static LineFollower lineFollower;
static ColorSensor cs(CS_S0, CS_S1, CS_S2, CS_S3, CS_OUT, CS_LED);

static j_state j_s = FOLLOW_STRAIGHT_LINE;
static byte leftLineFollowBits;
static byte rightLineFollowBits;
static int lineCount;

//Setup variables etc
void setup() {

	Serial.begin(9600);
	Serial.println("IN the beginning...");
	//Initialize Hardware--------------------------------
	//Motor variables setup
	motors.setup(PIN_PWM_LEFT, PIN_DIRECTION_LEFT, PIN_PWM_RIGHT, PIN_DIRECTION_RIGHT, DEFAULT_SPEED);
	//LineFollower variables setup
	lineFollower.setup(PIN_LOAD, PIN_SENSOR);
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setDataMode(SPI_MODE3);
	SPI.setBitOrder(MSBFIRST);

	pinMode(PIN_LOAD, OUTPUT);
	digitalWrite(PIN_LOAD, HIGH);
	pinMode(PIN_SENSOR, OUTPUT);
	digitalWrite(PIN_SENSOR, HIGH); //activates LineFollower Kit

	//Variables setup
	//state = MAIN_LINE;
	lineCount = 0;
}

//Loop
void loop() {

	switch (j_s)
	{
        case FOLLOW_STRAIGHT_LINE:
        	Serial.println("Follow straight line");
            if (lineFollower.intersection(leftLineFollowBits, rightLineFollowBits))
            {
                j_s = TURN_LEFT; break; 
            }
            else if (lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits))
            {
                motors.motorsDrive(FORWARD); break;
            }
            else
            {
                if (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits > rightLineFollowBits))
                    motors.motorsTurnLeft();
                if (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits) && (leftLineFollowBits < rightLineFollowBits))
                    motors.motorsTurnRight(); 
                j_s = FOLLOW_STRAIGHT_LINE; break;
            }
         case TURN_LEFT:
         	Serial.println("Turn Left");
             do {
             	 motors.motorsDrive(FORWARD);
             	 delay(500);
                 motors.motorsTurnLeft();
                 delay(500);
                 lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
             } while (leftLineFollowBits || rightLineFollowBits);
             
             do {
                 motors.motorsTurnLeft();
                 lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
             } while (!leftLineFollowBits && !rightLineFollowBits);
             
             j_s = FOLLOW_STRAIGHT_LINE;
             break;
}


/*	switch(state) {
		case MAIN_LINE:
			Serial.println("MAIN LINE");
			motors.motorsDrive(FORWARD);
			if (lineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {
				Serial.println("INTERSECTION!");

				//followTheLine(leftLineFollowBits, rightLineFollowBits, true);
				followTheLine(leftLineFollowBits, rightLineFollowBits);

				Serial.println("Entering isCentered Loop..");
				//Follow the line along the turn until we're done
				while (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits)) {
					Serial.println("Looping in isCentered...");
					//followTheLine(leftLineFollowBits, rightLineFollowBits, true);
                                        followTheLine(leftLineFollowBits, rightLineFollowBits);
				}
				Serial.println("Left bits == " + leftLineFollowBits);   
   				Serial.println("Right bits == " + rightLineFollowBits);
				
				//Switch states depending on which line we've discovered
				if (lineCount == 0 || lineCount == 2) {
					Serial.println("On  one of the straight lines");
					state = STRAIGHT_LINE_START;
					lineCount++;
				}
				else if (lineCount == 1) {
					//state = CURVED_LINE;
					lineCount++;
				}
			}
			//followTheLine(leftLineFollowBits, rightLineFollowBits);
			break;
		case STRAIGHT_LINE_START:
			Serial.println("ON A SIDE STRAIGHT LINE");
			//Remove later in favor of navigation to Blue firing block
			motors.motorsDrive(FORWARD);
			//lineFollower.Get_Line_Data(leftLineFollowBits, rightLineFollowBits);
			//followTheLine(leftLineFollowBits, rightLineFollowBits);
			motors.motorsStop();
			state = FIRE;
			break;
		case FIRE:
			delay(4000);
			Serial.println("IN 'FIRE' STATE! Making uTURN!");
			motors.motorsStop();
			motors.motorsUTurn();
			motors.motorsStop();
			state = STRAIGHT_LINE_END;
			break;
		case STRAIGHT_LINE_END:
			Serial.println("IN THE STRAIGHT_LINE_END STATE");
			motors.motorsDrive(FORWARD);
			if (lineFollower.intersection(leftLineFollowBits, rightLineFollowBits)) {
				
				Serial.println("INTERSECTION with the MAIN LINE...turning!\n");
				//followTheLine(leftLineFollowBits, rightLineFollowBits, true);
                                followTheLine(leftLineFollowBits, rightLineFollowBits);

				Serial.println("Entering isCentered Loop..");
				//Follow the line along the turn until we're done
				while (!lineFollower.isCentered(leftLineFollowBits, rightLineFollowBits)) {
					Serial.println("Looping in isCentered...");
					//followTheLine(leftLineFollowBits, rightLineFollowBits, true);
                                          followTheLine(leftLineFollowBits, rightLineFollowBits);
				}
				state = MAIN_LINE;
			}
			//followTheLine(leftLineFollowBits, rightLineFollowBits);
			break;
		case CURVED_LINE_START:
			break;
		case CURVED_LINE_END:
			break;
	}
*/
}

void followTheLine(byte leftBits, byte rightBits)
{
	short leftPWM = 0;
	short rightPWM = 0;
	Turn turnDirection;

	if (leftBits >= rightBits)
	{
		Serial.println("going left....");
		leftPWM = motors.speed / leftBits;
		rightPWM = motors.speed;
                motors.motorsTurnLeft();
	}
	else if (rightBits > leftBits)
	{
		Serial.println("going right....");
		leftPWM = motors.speed;
		rightPWM = motors.speed / rightBits;
                motors.motorsTurnRight();
	}
	else
	{
		leftPWM = motors.speed;
		rightPWM = motors.speed;
                motors.motorsDrive(FORWARD);
	}

	return;
}

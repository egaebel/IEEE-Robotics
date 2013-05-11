#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "common.h"
#include "Servo.h"
#include "Timer.h"

//Turn Timing
#define TURN_90_TIME 1400

#define TURN_180_TIME 2*TURN_90_TIME

//Start Claw timings---------------
#define OPEN_CLAW_TIME 500
#define CLOSE_CLAW_TIME 600

#define EXTEND_CLAW_TIME 2500
#define EXTEND_CLAW_TIME_L 1500
#define RETRACT_CLAW_TIME 2000

#define DROP_CLAW_TIME 3000
#define PICKUP_CLAW_TIME 3000
//End Claw timings---------------

//movement timing
#define WALL_BACKUP_TIME 500

class Movement {
	public:
		void init();
		void slideWall(side s);
		void slideLeft(float speed);
		void slideRight(float speed);
		void turnLeft(float speed);
		void turnRight(float speed);
		void forward(float speed);
		void backward(float speed);
		void stop();
		bool turn90(side s = LEFT);
		bool turnAround(side s = LEFT);

		bool dropClaw(side s);
		bool pickupClaw(side s);

		bool openClaw(side s);
		bool closeClaw(side s);
		bool extendClaw(side s);
		bool retractClaw(side s);
    //Used for complex movements
		void setSpeed(float speedFL,float speedFR, float speedBL, float speedBR);
		bool liftUp();
		bool setDown();
		bool timerTest();
		bool backOffWall();
		bool backwardForDuration(float speed, int time_ms);
		void forwardForDuration(float speed, int time_ms);
		void slideLeftForDuration(float speed, int time_ms);
		void slideRightForDuration(float speed, int time_ms);
		
		//DELETE THIS SHIT
		void testExtendInit();
		void testExtend();
	private:

		Servo leftMotor;
		Servo rightMotor;
		Servo backLeftMotor;
        Servo backRightMotor;
        Servo topMotor;

        Servo rightClawMotor;
        Servo rightExtendMotor;
        Servo leftClawMotor;
        Servo leftExtendMotor;

        Servo leftTrebMotor;
        Servo rightTrebMotor;

        Servo* getClawMotor(side s);
        Servo* getExtendMotor(side s);

		bool setSpeed(int servo, float speed);
        void setSpeed(Servo motor, float speed, bool inverted);
        bool goToDeg(Servo motor,int d);
};

#endif

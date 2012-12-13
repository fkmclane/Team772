#pragma config(Hubs, S1, HTMotor, HTMotor, none, none)
#pragma config(Hubs, S2, HTServo, HTMotor, HTMotor, none)
#pragma config(Sensor, S1, , sensorI2CMuxController)
#pragma config(Sensor, S2, , sensorI2CMuxController)
#pragma config(Sensor, S3, IR, sensorHiTechnicIRSeeker1200)
#pragma config(Motor, mtr_S1_C1_1, LeftForward, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor, mtr_S1_C1_2, FrontSideways, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor, mtr_S1_C2_1, BackSideways, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor, mtr_S1_C2_2, RightForward, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor, mtr_S2_C2_1, ShoulderLeft, tmotorTetrix, openLoop, encoder)
#pragma config(Motor, mtr_S2_C2_2, ShoulderRight, tmotorTetrix, openLoop, reversed)
#pragma config(Motor, mtr_S2_C3_1, ArmLeft, tmotorTetrix, openLoop, encoder)
#pragma config(Motor, mtr_S2_C3_2, ArmRight, tmotorTetrix, openLoop, reversed)
#pragma config(Servo, srvo_S2_C1_1, ArmServoLeft, tServoStandard)
#pragma config(Servo, srvo_S2_C1_2, ArmServoRight, tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//#pragma config(Motor, motorA, ArmHandLeft, tmotorNXT, PIDControl, encoder)
//#pragma config(Motor, motorB, ArmHandRight, tmotorNXT, PIDControl, encoder)

#include "JoystickDriver.c"

//Flag for whether to use tank drive or not
//#define TANKDRIVE

//Constants
//Scaling
#define JOYSTICKHIGH 100
#define JOYSTICKLOW 30
#define SHOULDERHIGH 50
#define SHOULDERLOW 20
#define SHOULDERDOWNHIGH 10
#define SHOULDERDOWNLOW 5
#define ARMHIGH 30
#define ARMLOW 15
#define ARMDOWNHIGH 5
#define ARMDOWNLOW 3
//Robot Constants
#define SHOULDERTOP 2300
#define SHOULDERBOTTOM 0
#define SHOULDERUPRIGHT 2300
#define ARMTOP 2880
#define ARMBOTTOM -1440
#define ARMUPRIGHT 64
//#define HANDMAX 300
//#define HANDMIN 0
#define HANDMAX 255
#define HANDMIN 0
#define SERVOHIGH 5
#define SERVOLOW 1
//Field Constants
#define RINGPEGSHOULDER 410
#define RINGPEGARM 320
#define BOTTOMPEG 280
#define MIDDLEPEG 0
#define TOPPEG 1440

//Initialize our globals
byte joystickScale = JOYSTICKHIGH; //Used to scale down robot movements
byte armScale = ARMHIGH; //Used to scale arm movements
byte armScaleDown = ARMDOWNHIGH;
byte shoulderScale = SHOULDERHIGH;
byte shoulderScaleDown = SHOULDERDOWNHIGH;

byte servoScale = SERVOHIGH;

bool forward = true; //Used for direction locking
bool sideways = true;

task joystickControl() { //Asynchronous task for joystick control
	while(true) {
		//Joystick 1 - Driver
#ifdef TANKDRIVE
		if(forward) { //Part of direction locking mechanism
			//Big fancy statement to set the left forward motors equal to the converted joystick's left y axis unless it is in the natural variant range.
			motor[LeftForward] = joystick.joy1_y1 > 10 || joystick.joy1_y1 < -10 ? joystick.joy1_y1 / 128.0 * joystickScale : 0;
			//Same for right and for sideways (but use the second y axis for right side and use the x axes for sideways motors)
			motor[RightForward] = joystick.joy1_y2 > 10 || joystick.joy1_y2 < -10 ? joystick.joy1_y2 / 128.0 * joystickScale : 0;
		}
		if(sideways) {
			motor[BackSideways] = joystick.joy1_x1 > 10 || joystick.joy1_x1 < -10 ? joystick.joy1_x1 / 128.0 * joystickScale : 0;
			motor[FrontSideways] = joystick.joy1_x2 > 10 || joystick.joy1_x2 < -10 ? joystick.joy1_x2 / 128.0 * joystickScale : 0;
		}
#else
		if(forward) { //Part of direction locking mechanism
			//Big fancy statement to set the left forward motors equal to the converted joystick's left y axis unless it is in the natural variant range and adds/subtracts to turn based on the joystick's right x axis.
			motor[LeftForward] = (joystick.joy1_y1 > 10 || joystick.joy1_y1 < -10 ? joystick.joy1_y1 : 0 + (int)(joystick.joy1_x2 > 10 || joystick.joy1_x2 < -10 ? joystick.joy1_x2 : 0)) / (128.0 + joystick.joy1_x2) * joystickScale;
			motor[RightForward] = (joystick.joy1_y1 > 10 || joystick.joy1_y1 < -10 ? joystick.joy1_y1 : 0 - (int)(joystick.joy1_x2 > 10 || joystick.joy1_x2 < -10 ? joystick.joy1_x2 : 0)) / (128.0 + joystick.joy1_x2) * joystickScale;
		}
  	if(sideways) {
  		//Same for sideways wheels but using the joystick's left x axis for sideways but still the joystick's right x axis for turning.
			motor[BackSideways] = (joystick.joy1_x1 > 10 || joystick.joy1_x1 < -10 ? joystick.joy1_x1 : 0 - (int)(joystick.joy1_x2 > 10 || joystick.joy1_x2 < -10 ? joystick.joy1_x2 : 0)) / (128.0 + joystick.joy1_x2) * joystickScale;
			motor[FrontSideways] = (joystick.joy1_x1 > 10 || joystick.joy1_x1 < -10 ? joystick.joy1_x1 : 0 + (int)(joystick.joy1_x2 > 10 || joystick.joy1_x2 < -10 ? joystick.joy1_x2 : 0)) / (128.0 + joystick.joy1_x2) * joystickScale;
		}
#endif
		//Joystick 2 - Operator
		if(joystick.joy2_y2 > 10 && (nMotorEncoder[ShoulderLeft] < SHOULDERTOP || joy2Btn(7))) //If shoulder is going positive and we aren't at the top, follow the joystick
			motor[ShoulderLeft] = motor[ShoulderRight] = joystick.joy2_y2 / 128.0 * shoulderScale;
		else if(joystick.joy2_y2 < -10 && (nMotorEncoder[ShoulderLeft] > SHOULDERBOTTOM || joy2Btn(7))) //Else if we are going down and aren't at the bottom, go negative
			motor[ShoulderLeft] = motor[ShoulderRight] = joystick.joy2_y2 / 128.0 * shoulderScaleDown;
		else
			motor[ShoulderLeft] = motor[ShoulderRight] = 0;
		if(joystick.joy2_y1 > 10 && (nMotorEncoder[ArmLeft] < ARMTOP || joy2Btn(7))) //Same thing for arm
			motor[ArmLeft] = motor[ArmRight] = joystick.joy2_y1 / 128.0 * armScale;
		else if(joystick.joy2_y1 < -10 && (nMotorEncoder[ArmLeft] > ARMBOTTOM || joy2Btn(7)))
			motor[ArmLeft] = motor[ArmRight] = joystick.joy2_y1 / 128.0 * armScaleDown;
		else
			motor[ArmLeft] = motor[ArmRight] = 0;
/*		if(joy2Btn(5) && nMotorEncoder[ArmHandLeft] < HANDMAX) //If the operator is pressing button 5 and it isn't open, open the hand
			motor[ArmHandLeft] = motor[ArmHandRight] = armScale;
		else if(joy2Btn(6) && nMotorEncoder[ArmHandLeft] > HANDMIN) //Else if the operator is pressing button 6 and it isn't closed, closed the hand
			motor[ArmHandLeft] = motor[ArmHandRight] = -armScale;
		else //Otherwise just don't move
			motor[ArmHandLeft] = motor[ArmHandRight] = 0;*/

		if(joy2Btn(5) && ((ServoValue[ArmServoLeft] < HANDMAX && ServoValue[ArmServoRight] > HANDMIN) || joy2Btn(7))) { //If button 5 is pressed and servos aren't at maximum, open hand
			servo[ArmServoLeft] = ServoValue[ArmServoLeft] + servoScale; //Increase servo positions
			servo[ArmServoRight] = ServoValue[ArmServoRight] - servoScale;
		}
		if(joy2Btn(6) && ((ServoValue[ArmServoLeft] > HANDMIN && ServoValue[ArmServoRight] < HANDMAX) || joy2Btn(7))) { //If button 6 is pressed and servos aren't at minimum, close hand
			servo[ArmServoLeft] = ServoValue[ArmServoLeft] - servoScale; //Decrease servo positions
			servo[ArmServoRight] = ServoValue[ArmServoRight] + servoScale;
		}
	}
}

task main() {
	//Initialize
	motor[LeftForward] = motor[RightForward] = motor[BackSideways] = motor[FrontSideways] = motor[ArmLeft] = motor[ArmRight] = motor[ShoulderLeft] = motor[ShoulderRight]/* = motor[ArmHandLeft] = motor[ArmHandRight]*/ = 0; //Turn off the motors
	nMotorEncoder[LeftForward] = nMotorEncoder[RightForward] = nMotorEncoder[BackSideways] = nMotorEncoder[FrontSideways] = nMotorEncoder[ArmLeft] = nMotorEncoder[ShoulderLeft]/* = nMotorEncoder[ArmHandLeft]*/ = 0; //Might as well reset the encoders too
	waitForStart();
	StartTask(joystickControl); //Go ahead and start critical joystick functions in their own task
	while(true) {
		//Joystick 1 - Driver
		if(joy1Btn(6)) //If the driver is pressing button 6, scale down robot movements
			joystickScale = JOYSTICKLOW;
		else //Else leave at full speed
			joystickScale = JOYSTICKHIGH;
		if(joy1Btn(5)) //If the driver is pressing button 5, lock to y-axis movement
			sideways = false;
		else
			sideways = true;
		if(joy1Btn(7)) //If the driver is pressing button 7, lock to x-axis movement
			forward = false;
		else
			forward = true;
		//Joystick 2 - Operator
		/*if(joy2Btn(1)) { //If the operator is pressing button 1, set arm to lowest peg
			nMotorEncoderTarget[ShoulderLeft] = SHOULDERUPRIGHT; //Set shoulder to upright
			nMotorEncoderTarget[ArmLeft] = BOTTOMPEG; //Set arm to its target
			motor[ShoulderLeft] = motor[ShoulderRight] = nMotorEncoder[ShoulderLeft] < SHOULDERUPRIGHT ? shoulderScale : -shoulderScaleDown; //Start the motors in the direction of the target
			motor[ArmLeft] = motor[ArmRight] = nMotorEncoder[ArmLeft] < BOTTOMPEG ? armScale : -armScaleDown;
			while(nMotorRunState[ShoulderLeft] != runStateIdle && nMotorRunState[ArmLeft] != runStateIdle); //Wait until they get there
			motor[ShoulderLeft] = motor[ShoulderRight] = motor[ArmLeft] = motor[ArmRight] = 0; //Then stop them
		}
		if(joy2Btn(2)) { //If the operator is pressing button 2, set arm to middle peg
			nMotorEncoderTarget[ShoulderLeft] = SHOULDERUPRIGHT;
			nMotorEncoderTarget[ArmLeft] = MIDDLEPEG;
			motor[ShoulderLeft] = motor[ShoulderRight] = nMotorEncoder[ShoulderLeft] < SHOULDERUPRIGHT ? shoulderScale : -shoulderScaleDown;
			motor[ArmLeft] = motor[ArmRight] = nMotorEncoder[ArmLeft] < MIDDLEPEG ? armScale : -armScaleDown;
			while(nMotorRunState[ShoulderLeft] != runStateIdle && nMotorRunState[ArmLeft] != runStateIdle);
			motor[ShoulderLeft] = motor[ShoulderRight] = motor[ArmLeft] = motor[ArmRight] = 0;
		}
		if(joy2Btn(3)) { //If the operator is pressing button 3, set arm to top peg
			nMotorEncoderTarget[ShoulderLeft] = SHOULDERUPRIGHT;
			nMotorEncoderTarget[ArmLeft] = TOPPEG;
			motor[ShoulderLeft] = motor[ShoulderRight] = nMotorEncoder[ShoulderLeft] < SHOULDERUPRIGHT ? shoulderScale : -shoulderScaleDown;
			motor[ArmLeft] = motor[ArmRight] = nMotorEncoder[ArmLeft] < TOPPEG ? armScale : -armScaleDown;
			while(nMotorRunState[ShoulderLeft] != runStateIdle && nMotorRunState[ArmLeft] != runStateIdle);
			motor[ShoulderLeft] = motor[ShoulderRight] = motor[ArmLeft] = motor[ArmRight] = 0;
		}*/
		if(joy2Btn(4)) { //If the operator is pressing button 4, set arm to ring peg
			nMotorEncoderTarget[ShoulderLeft] = RINGPEGSHOULDER; //Set shoulder to home
			nMotorEncoderTarget[ArmLeft] = RINGPEGARM; //Then set arm to home
			motor[ShoulderLeft] = motor[ShoulderRight] = nMotorEncoder[ShoulderLeft] < RINGPEGSHOULDER ? shoulderScale : -shoulderScaleDown;
			motor[ArmLeft] = motor[ArmRight] = nMotorEncoder[ArmLeft] < RINGPEGARM ? armScale : -armScaleDown;
			while(nMotorRunState[ShoulderLeft] != runStateIdle && nMotorRunState[ArmLeft] != runStateIdle);
			motor[ShoulderLeft] = motor[ShoulderRight] = motor[ArmLeft] = motor[ArmRight] = 0;
		}
		if(joy2Btn(9)) { //If the operator is pressing button 9, set arm to upright
			nMotorEncoderTarget[ShoulderLeft] = SHOULDERUPRIGHT;
			nMotorEncoderTarget[ArmLeft] = ARMUPRIGHT;
			motor[ShoulderLeft] = motor[ShoulderRight] = nMotorEncoder[ShoulderLeft] < SHOULDERUPRIGHT ? shoulderScale : -shoulderScaleDown;
			motor[ArmLeft] = motor[ArmRight] = nMotorEncoder[ArmLeft] < ARMUPRIGHT ? armScale : -armScaleDown;
			while(nMotorRunState[ShoulderLeft] != runStateIdle && nMotorRunState[ArmLeft] != runStateIdle);
			motor[ShoulderLeft] = motor[ShoulderRight] = motor[ArmLeft] = motor[ArmRight] = 0;
		}
		if(joy2Btn(10)) { //If the operator is pressing button 10, set arm to home position
			nMotorEncoderTarget[ShoulderLeft] = 0;
			nMotorEncoderTarget[ArmLeft] = 0;
			motor[ShoulderLeft] = motor[ShoulderRight] = nMotorEncoder[ShoulderLeft] < 0 ? shoulderScale : -shoulderScaleDown;
			motor[ArmLeft] = motor[ArmRight] = nMotorEncoder[ArmLeft] < 0 ? armScale : -armScaleDown;
			while(nMotorRunState[ShoulderLeft] != runStateIdle && nMotorRunState[ArmLeft] != runStateIdle);
			motor[ShoulderLeft] = motor[ShoulderRight] = motor[ArmLeft] = motor[ArmRight] = 0;
		}
		if(joy2Btn(8)) { //If the operator is pressing button 8, scale down hand movements
			armScale = ARMLOW;
			armScaleDown = ARMDOWNLOW;
			shoulderScale = SHOULDERLOW;
			shoulderScaleDown = SHOULDERDOWNLOW;
		}
		else {
			armScale = ARMHIGH;
			armScaleDown = ARMDOWNHIGH;
			shoulderScale = SHOULDERHIGH;
			shoulderScaleDown = SHOULDERDOWNHIGH;
		}
		//writeDebugStream("%d, %d\n", nMotorEncoder[ShoulderLeft], nMotorEncoder[ArmLeft]);
	}
}

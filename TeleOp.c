#pragma config(Hubs, S1, HTMotor, HTMotor, none, none)
#pragma config(Hubs, S2, HTMotor, HTMotor, none, none)
#pragma config(Sensor, S3, IR, sensorHiTechnicIRSeeker1200)
#pragma config(Motor, motorA, ArmHandLeft, tmotorNXT, PIDControl, encoder)
#pragma config(Motor, motorB, ArmHandRight, tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor, mtr_S1_C1_1, LeftForward, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor, mtr_S1_C1_2, FrontSideways, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor, mtr_S1_C2_1, BackSideways, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor, mtr_S1_C2_2, RightForward, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor, mtr_S2_C1_1, ShoulderLeft, tmotorTetrix, openLoop, encoder)
#pragma config(Motor, mtr_S2_C1_2, ShoulderRight, tmotorTetrix, openLoop, reversed)
#pragma config(Motor, mtr_S2_C2_1, ArmLeft, tmotorTetrix, openLoop, encoder)
#pragma config(Motor, mtr_S2_C2_2, ArmRight, tmotorTetrix, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

//Flag for whether to use tank drive or not
//#define TANKDRIVE

//Constants
//Scaling
#define JOYSTICKHIGH 100
#define JOYSTICKLOW 30
#define SHOULDERHIGH 10
#define SHOULDERLOW 50
#define ARMHIGH 30
#define ARMLOW 10
//Robot Constants
#define SHOULDERTOP 2880
#define SHOULDERBOTTOM 0
#define SHOULDERUPRIGHT 1440
#define ARMTOP 2880
#define ARMBOTTOM -1440
#define HANDMAX 300
#define HANDMIN 0
//Field Constants
#define RINGPEGSHOULDER 24
#define RINGPEGARM 24
#define BOTTOMPEG -1440
#define MIDDLEPEG 0
#define TOPPEG 1440

//Initialize our globals
byte joystickScale = JOYSTICKHIGH; //Used to scale down robot movements
byte armScale = ARMHIGH; //Used to scale arm movements
byte shoulderScale = SHOULDERHIGH;
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
		if(joystick.joy2_y2 > 10 && nMotorEncoder[ShoulderLeft] < SHOULDERTOP) //If shoulder is going positive and we aren't at the top, follow the joystick
			motor[ShoulderLeft] = motor[ShoulderRight] = joystick.joy2_y2 / 128.0 * armScale;
		else if(joystick.joy2_y2 < -10 && nMotorEncoder[ShoulderLeft] > SHOULDERBOTTOM) //Else if we are going down and aren't at the bottom, go negative
			motor[ShoulderLeft] = motor[ShoulderRight] = joystick.joy2_y2 / 128.0 * armScale;
		else
			motor[ShoulderLeft] = motor[ShoulderRight] = 0;
		if(joystick.joy2_y1 > 10 && nMotorEncoder[ArmLeft] < ARMTOP) //Same thing for arm
			motor[ArmLeft] = motor[ArmRight] = joystick.joy2_y1 / 128.0 * armScale;
		else if(joystick.joy2_y1 < -10 && nMotorEncoder[ArmLeft] > ARMBOTTOM)
			motor[ArmLeft] = motor[ArmRight] = joystick.joy2_y1 / 128.0 * armScale;
		else
			motor[ArmLeft] = motor[ArmRight] = 0;
		if(joy2Btn(5) && nMotorEncoder[ArmHandLeft] < HANDMAX) //If the operator is pressing button 5 and it isn't open, open the hand
			motor[ArmHandLeft] = motor[ArmHandRight] = armScale;
		else if(joy2Btn(6) && nMotorEncoder[ArmHandLeft] > HANDMIN) //Else if the operator is pressing button 6 and it isn't closed, closed the hand
			motor[ArmHandLeft] = motor[ArmHandRight] = -armScale;
		else //Otherwise just don't move
			motor[ArmHandLeft] = motor[ArmHandRight] = 0;
	}
}

task main() {
	//Initialize
	motor[LeftForward] = motor[RightForward] = motor[BackSideways] = motor[FrontSideways] = motor[ArmLeft] = motor[ArmRight] = motor[ShoulderLeft] = motor[ShoulderRight] = motor[ArmHandLeft] = motor[ArmHandRight] = 0; //Turn off the motors
	nMotorEncoder[LeftForward] = nMotorEncoder[RightForward] = nMotorEncoder[BackSideways] = nMotorEncoder[FrontSideways] = nMotorEncoder[ArmLeft] = nMotorEncoder[ShoulderLeft] = nMotorEncoder[ArmHandLeft] = 0; //Might as well reset the encoders too
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
			nMotorEncoderTarget[Shoulder] = SHOULDERUPRIGHT; //Set shoulder to upright
			nMotorEncoderTarget[Arm] = BOTTOMPEG; //Set arm to its target
			motor[Shoulder] = nMotorEncoder[Shoulder] < SHOULDERUPRIGHT ? 100 : -100; //Start the motors in the direction of the target
			motor[Arm] = nMotorEncoder[Arm] < BOTTOMPEG ? 100 : -100;
			while(nMotorRunState[Shoulder] != runStateIdle && nMotorRunState[Arm] != runStateIdle); //Wait until they get there
			motor[Shoulder] = motor[Arm] = 0; //Then stop them
		}
		if(joy2Btn(2)) { //If the operator is pressing button 2, set arm to middle peg
			nMotorEncoderTarget[Shoulder] = SHOULDERUPRIGHT;
			nMotorEncoderTarget[Arm] = MIDDLEPEG;
			motor[Shoulder] = nMotorEncoder[Shoulder] < SHOULDERUPRIGHT ? 100 : -100;
			motor[Arm] = nMotorEncoder[Arm] < MIDDLEPEG ? 100 : -100;
			while(nMotorRunState[Shoulder] != runStateIdle && nMotorRunState[Arm] != runStateIdle);
			motor[Shoulder] = motor[Arm] = 0;
		}
		if(joy2Btn(3)) { //If the operator is pressing button 3, set arm to top peg
			nMotorEncoderTarget[Shoulder] = SHOULDERUPRIGHT;
			nMotorEncoderTarget[Arm] = TOPPEG;
			motor[Shoulder] = nMotorEncoder[Shoulder] < SHOULDERUPRIGHT ? 100 : -100;
			motor[Arm] = nMotorEncoder[Arm] < TOPPEG ? 100 : -100;
			while(nMotorRunState[Shoulder] != runStateIdle && nMotorRunState[Arm] != runStateIdle);
			motor[Shoulder] = motor[Arm] = 0;
		}
		if(joy2Btn(4)) { //If the operator is pressing button 4, set arm to ring peg
			nMotorEncoderTarget[Shoulder] = 0; //Set shoulder to home
			nMotorEncoderTarget[Arm] = 0; //Then set arm to home
			motor[Shoulder] = nMotorEncoder[Shoulder] < RINGPEGSHOULDER ? 100 : -100;
			motor[Arm] = nMotorEncoder[Arm] < RINGPEGARM ? 100 : -100;
			while(nMotorRunState[Shoulder] != runStateIdle && nMotorRunState[Arm] != runStateIdle);
			motor[Shoulder] = motor[Arm] = 0;
		}*/
		if(joy2Btn(8)) { //If the operator is pressing button 7, scale down hand movements
			armScale = ARMLOW;
			shoulderScale = SHOULDERLOW;
		}
		else {
			armScale = ARMHIGH;
			shoulderScale = SHOULDERHIGH;
		}
	}
}

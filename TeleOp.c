#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S3,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorA,          RightHand,     tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          LeftHand,      tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     FrontLeft,     tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     FrontRight,    tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     BackLeft,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     BackRight,     tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S2_C1_1,     LeftArmShoulder, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S2_C1_2,     LeftArmElbow,  tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_1,     RightArmShoulder, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S2_C2_2,     RightArmElbow, tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

//Constants
#include "constants.h"

//Useful functions
#include "common.c"

//Initialize our globals
byte drive_scale = DRIVE_HIGH; //Used to scale down robot movements
byte arm_scale = ARM_HIGH;

bool forwards = true; //Used for direction locking
bool sideways = true;

task driveControl() { //Asynchronous task for critical drive control
	while(true) {
		//Joystick 1 - Driver

		//Deadband between -DEADBAND to DEADBAND
		//Scale linearly for all other values
		float x1, y1, x2, y2;

		//Check each axis for deadband and direction locking
		if((joystick.joy1_x1 > DEADBAND || joystick.joy1_x1 < -DEADBAND) && sideways)
			x1 = joystick.joy1_x1 / 128.0;
		else
			x1 = 0;

		if((joystick.joy1_y1 > DEADBAND || joystick.joy1_y1 < -DEADBAND) && forwards)
			y1 = joystick.joy1_y1 / 128.0;
		else
			y1 = 0;

		if(joystick.joy1_x2 > DEADBAND || joystick.joy1_x2 < -DEADBAND)
			x2 = joystick.joy1_x2 / 128.0;
		else
			x2 = 0;

		if(joystick.joy1_y2 > DEADBAND || joystick.joy1_y2 < -DEADBAND)
			y2 = joystick.joy1_y2 / 128.0;
		else
			y2 = 0;

		//Set the motors to scale
		motor[FrontLeft] = drive_scale * (-x2 + (y1 + x1));
		motor[FrontRight] = drive_scale * (-x2 + (-y1 + x1));
		motor[BackLeft] = drive_scale * (-x2 + (y1 + -x1));
		motor[BackRight] = drive_scale * (-x2 + (-y1 + -x1));

		//writeDebugStream("Wheels:\n\tFront Left:\t%d\n\tFront Right:\t%d\n\tBack Left:\t%d\n\tBack Right:\t%d\n", nMotorEncoder[FrontLeft], nMotorEncoder[FrontRight], nMotorEncoder[BackLeft], nMotorEncoder[BackRight]);
	}
}

task armControl() {
	while(true) {
		//Joystick 2 - Operator

		float x1, y1, x2, y2;

		//Check each axis for deadband
		if((joystick.joy2_x1 > DEADBAND || joystick.joy2_x1 < -DEADBAND) && (abs(joystick.joy2_x1) > abs(joystick.joy2_y1)))
			x1 = joystick.joy2_x1 / 128.0;
		else
			x1 = 0;

		if((joystick.joy2_y1 > DEADBAND || joystick.joy2_y1 < -DEADBAND) && (abs(joystick.joy2_y1) > abs(joystick.joy2_x1)))
			y1 = joystick.joy2_y1 / 128.0;
		else
			y1 = 0;

		if((joystick.joy2_x2 > DEADBAND || joystick.joy2_x2 < -DEADBAND) && (abs(joystick.joy2_x2) > abs(joystick.joy2_y2)))
			x2 = joystick.joy2_x2 / 128.0;
		else
			x2 = 0;

		if((joystick.joy2_y2 > DEADBAND || joystick.joy2_y2 < -DEADBAND) && (abs(joystick.joy2_y2) > abs(joystick.joy2_x2)))
			y2 = joystick.joy2_y2 / 128.0;
		else
			y2 = 0;

		//Protect arms
		if(((x1 < 0 && nMotorEncoder[LeftArmShoulder] <= ARM_SHOULDER_BOTTOM) || (x1 > 0 && nMotorEncoder[LeftArmShoulder] >= ARM_SHOULDER_TOP)) && !joy2Btn(6))
			x1 = 0;

		if(((x2 < 0 && nMotorEncoder[RightArmShoulder] <= ARM_SHOULDER_BOTTOM) || (x2 > 0 && nMotorEncoder[RightArmShoulder] >= ARM_SHOULDER_TOP)) && !joy2Btn(6))
			x2 = 0;

		//writeDebugStream("Arms:\n\tLeft Shoulder:\t%d\n\tLeft Elbow:\t%d\n\tRight Shoulder:\t%d\n\tRight Elbow:\t%d\n", nMotorEncoder[LeftArmShoulder], nMotorEncoder[LeftArmElbow], nMotorEncoder[RightArmShoulder], nMotorEncoder[RightArmElbow]);

		//Set the motors to scale
		motor[LeftArmShoulder] = arm_scale * x1;
		motor[LeftArmElbow] = arm_scale * y1;
		motor[RightArmShoulder] = arm_scale * x2;
		motor[RightArmElbow] = arm_scale * y2;

		//If button 7 is pressed, open the left hand until it is at max
		if(joy2Btn(7) && nMotorEncoder[LeftHand] < HAND_MAX)
			motor[LeftHand] = HAND_HIGH;
		else if(nMotorEncoder[LeftHand] > HAND_MIN) //Else close the hand
			motor[LeftHand] = -HAND_HIGH;
		else
			motor[LeftHand] = 0;

		//If button 8 is pressed, control the right hand
		if(joy2Btn(8) && nMotorEncoder[RightHand] < HAND_MAX)
			motor[RightHand] = HAND_HIGH;
		else if(nMotorEncoder[RightHand] > HAND_MIN) //Else close the hand
			motor[RightHand] = -HAND_HIGH;
		else
			motor[RightHand] = 0;

		//writeDebugStream("Hands:\n\tLeft Hand:\t%d\n\tRight Hand:\t%d\n", nMotorEncoder[LeftHand], nMotorEncoder[RightHand]);

		//Presets

		//Left arm pickup
		if(joy2Btn(1)) {
			while((abs(nMotorEncoder[LeftArmShoulder] - ARM_SHOULDER_PICKUP) > ENCODER_PRECISION || abs(nMotorEncoder[LeftArmElbow] - ARM_ELBOW_PICKUP) > ENCODER_PRECISION) && !joy2Btn(6)) {
				motor[LeftArmShoulder] = targetMotorSpeed(ARM_SHOULDER_PICKUP, nMotorEncoder[LeftArmShoulder]) * ARM_HIGH;
				motor[LeftArmElbow] = targetMotorSpeed(ARM_ELBOW_PICKUP, nMotorEncoder[LeftArmElbow]) * ARM_LOW;
				writeDebugStream("LeftArm:\t%d\t%d\n", nMotorEncoder[LeftArmShoulder], nMotorEncoder[LeftArmElbow]);
			}
		}

		//Home
		if(joy2Btn(2)) {
			while((nMotorEncoder[LeftArmShoulder] > ENCODER_PRECISION || nMotorEncoder[LeftArmElbow] > ENCODER_PRECISION || nMotorEncoder[RightArmShoulder] > ENCODER_PRECISION || nMotorEncoder[RightArmElbow] > ENCODER_PRECISION) && !joy2Btn(6)) {
				motor[LeftArmShoulder] = targetMotorSpeed(0, nMotorEncoder[LeftArmShoulder]) * ARM_HIGH;
				motor[LeftArmElbow] = targetMotorSpeed(0, nMotorEncoder[LeftArmElbow]) * ARM_LOW;
				motor[RightArmShoulder] = targetMotorSpeed(0, nMotorEncoder[RightArmShoulder]) * ARM_HIGH;
				motor[RightArmElbow] = targetMotorSpeed(0, nMotorEncoder[RightArmElbow]) * ARM_LOW;
				writeDebugStream("LeftArm:\t%d\t%d\nRightArm:\t%d\t%d\n", nMotorEncoder[LeftArmShoulder], nMotorEncoder[LeftArmElbow], nMotorEncoder[RightArmShoulder], nMotorEncoder[RightArmElbow]);
			}
		}

		//Right arm pickup
		if(joy2Btn(3)) {
			while((abs(nMotorEncoder[RightArmShoulder] - ARM_SHOULDER_PICKUP) > ENCODER_PRECISION || abs(nMotorEncoder[RightArmElbow] - ARM_ELBOW_PICKUP) > ENCODER_PRECISION) && !joy2Btn(6)) {
				motor[RightArmShoulder] = targetMotorSpeed(ARM_SHOULDER_PICKUP, nMotorEncoder[RightArmShoulder]) * ARM_HIGH;
				motor[RightArmElbow] = targetMotorSpeed(ARM_ELBOW_PICKUP, nMotorEncoder[RightArmElbow]) * ARM_LOW;
				writeDebugStream("RightArm:\t%d\t%d\n", nMotorEncoder[RightArmShoulder], nMotorEncoder[RightArmElbow]);
			}
		}

		//Hang
		if(joy2Btn(4)) {
			while((abs(nMotorEncoder[LeftArmShoulder] - ARM_SHOULDER_HANG) > ENCODER_PRECISION || abs(nMotorEncoder[LeftArmElbow] - ARM_ELBOW_HANG) > ENCODER_PRECISION || abs(nMotorEncoder[RightArmShoulder] - ARM_SHOULDER_HANG) > ENCODER_PRECISION || abs(nMotorEncoder[RightArmElbow] - ARM_ELBOW_HANG) > ENCODER_PRECISION) && !joy2Btn(6)) {
				motor[LeftArmShoulder] = targetMotorSpeed(ARM_SHOULDER_HANG, nMotorEncoder[LeftArmShoulder]) * ARM_HIGH;
				motor[LeftArmElbow] = targetMotorSpeed(ARM_ELBOW_HANG, nMotorEncoder[LeftArmElbow]) * ARM_LOW;
				motor[RightArmShoulder] = targetMotorSpeed(ARM_SHOULDER_HANG, nMotorEncoder[RightArmShoulder]) * ARM_HIGH;
				motor[RightArmElbow] = targetMotorSpeed(ARM_ELBOW_HANG, nMotorEncoder[RightArmElbow]) * ARM_LOW;
				writeDebugStream("LeftArm:\t%d\t%d\nRightArm:\t%d\t%d\n", nMotorEncoder[LeftArmShoulder], nMotorEncoder[LeftArmElbow], nMotorEncoder[RightArmShoulder], nMotorEncoder[RightArmElbow]);
			}
		}
	}
}

task main() {
	//Initialize
	motor[FrontLeft] = motor[FrontRight] = motor[BackLeft] = motor[BackRight] = motor[LeftArmShoulder] = motor[LeftArmElbow] = motor[RightArmShoulder] = motor[RightArmElbow] = motor[LeftHand] = motor[RightHand] = 0; //Turn off the motors
	nMotorEncoder[FrontLeft] = nMotorEncoder[FrontRight] = nMotorEncoder[BackLeft] = nMotorEncoder[BackRight] = nMotorEncoder[LeftArmShoulder] = nMotorEncoder[LeftArmElbow] = nMotorEncoder[RightArmShoulder] = nMotorEncoder[RightArmElbow] = nMotorEncoder[LeftHand] = nMotorEncoder[RightHand] = 0; // Might as well reset the encoders too

	//Display the robot's name
	nxtDisplayCenteredTextLine(0, "Codex");

	//Go time!
	waitForStart();
	StartTask(driveControl); //Go ahead and start critical drive functions in their own task
	StartTask(armControl); //Start arm functions

	while(true) {
		//Joystick 1 - Driver

		if(joy1Btn(6)) //If the driver is pressing button 6, scale down robot movements
			drive_scale = DRIVE_LOW;
		else //Else leave at full speed
			drive_scale = DRIVE_HIGH;

		if(joy1Btn(5)) //If the driver is pressing button 5, lock to y-axis movement
			sideways = false;
		else
			sideways = true;

		if(joy1Btn(7)) //If the driver is pressing button 7, lock to x-axis movement
			forwards = false;
		else
			forwards = true;

		//Joystick 2 - Operator

		if(joy2Btn(5)) //If the operator is pressing button 8, scale down the arm movements
			arm_scale = ARM_LOW;
		else
			arm_scale = ARM_HIGH;

		writeDebugStream("IR:\t%d\n", SensorValue[IR]);
	}
}

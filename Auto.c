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

task main() {
	//Initialize
	motor[FrontLeft] = motor[FrontRight] = motor[BackLeft] = motor[BackRight] = motor[LeftArmShoulder] = motor[LeftArmElbow] = motor[RightArmShoulder] = motor[RightArmElbow] = motor[LeftHand] = motor[RightHand] = 0; //Turn off the motors
	nMotorEncoder[FrontLeft] = nMotorEncoder[FrontRight] = nMotorEncoder[BackLeft] = nMotorEncoder[BackRight] = nMotorEncoder[LeftArmShoulder] = nMotorEncoder[LeftArmElbow] = nMotorEncoder[RightArmShoulder] = nMotorEncoder[RightArmElbow] = nMotorEncoder[LeftHand] = nMotorEncoder[RightHand] = 0; // Might as well reset the encoders too

	//Display the robot's name
	nxtDisplayCenteredTextLine(0, "Codex");

	//Go time!
	waitForStart();

#if AUTO_PROGRAM == 0
	//Go forward until the IR beacon is found
	int offset = 0; //Offset for after encoder gets reset
	bool dropped = false; //So that we don't drop twice
	motor[FrontLeft] = DRIVE_HIGH;
	motor[BackRight] = -DRIVE_HIGH;
	while(abs(nMotorEncoder[FrontLeft]) + offset < AUTO_DETECT || abs(nMotorEncoder[BackRight]) + offset < AUTO_DETECT) {
		if(!dropped && SensorValue[IR] == 6) { //If we haven't dropped yet and the IR is in the right zone
				//Drop a block and move back
				motor[FrontLeft] = motor[BackRight] = 0;
				offset += nMotorEncoder[FrontLeft]; //Save offset because functions reset encoders
				wait();
				//Correct for IR detection
				move(0, AUTO_IR_CORRECT);
				wait();
				turn(AUTO_IR_TURN);
				wait();
				//Drop
				moveRightArm(ARM_SHOULDER_BASKET, ARM_ELBOW_BASKET);
				wait();
				openRightHand();
				wait();
				moveRightArm(0, 0);
				wait();
				//Undo actions to continue on
				turn(-AUTO_IR_TURN);
				wait();
				move(0, -AUTO_IR_CORRECT);
				wait();
				dropped = true;
				motor[FrontLeft] = DRIVE_HIGH;
				motor[BackRight] = -DRIVE_HIGH;
		}
	}
	motor[FrontLeft] = motor[BackRight] = 0;

	//Go to ramp
	//TODO - Left and IR
#elif AUTO_PROGRAM == 1
	//Place a block in first basket
	//TODO

	//Go to ramp
	//TODO - Left and no IR
#elif AUTO_PROGRAM == 2
	//Go forward until the IR beacon is found

	//Go to ramp
	//TODO - Right and IR
#elif AUTO_PROGRAM == 3
	//Place a block in first basket

	//Go to ramp
	//TODO - Right and no IR
#endif
}

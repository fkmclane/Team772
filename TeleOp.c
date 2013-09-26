#pragma config(Hubs, S1, HTMotor, HTMotor, none, none)
#pragma config(Sensor, S1, , sensorI2CMuxController)
#pragma config(Motor, mtr_S1_C1_1, FrontLeft, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor, mtr_S1_C1_2, FrontRight, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor, mtr_S1_C2_1, BackLeft, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor, mtr_S1_C2_2, BackRight, tmotorTetrix, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "common.c"

//Constants
#include "constants.h"

//Initialize our globals
byte joystickScale = JOYSTICKHIGH; //Used to scale down robot movements

bool forwardlock = true; //Used for direction locking
bool sidelock = true;

task joystickControl() { //Asynchronous task for joystick control
	while(true) {
		//Joystick 1 - Driver

		//Deadband between -10 to 10
		//Scale linearly for all other values
	 	float x1;
		if(joystick.joy1_x1 > 15 || joystick.joy1_x1 < -15)
			x1 = joystick.joy1_x1 / 128.0 * 100;
		else
			x1 = 0;

		// Moving straight to the left
		motor[FrontLeft] = -x1;
		motor[FrontRight] = -x1;
		motor[BackLeft] = x1;
		motor[BackRight]= x1;
		/*Moving straight to the right
		motor[FrontLeft] = -x1;
		motor[FrontRight] = -x1;
		motor[BackLeft] = x1;
		motor[BackRight]= x1; */

	}
}


task armControl() { //Another asynchronous task to move the arm
	while(true) {
		//Joystick 2 - Operator

		break;
	}
}

task main() {
	//Initialize
	motor[FrontLeft] = motor[FrontRight] = motor[BackLeft] = motor[BackRight] = 0; //Turn off the motors
	nMotorEncoder[FrontLeft] = nMotorEncoder[FrontRight] = nMotorEncoder[BackLeft] = nMotorEncoder[BackRight] = 0; // Might as well reset the encoders too

	/*
	//Moving forward
	motor[FrontLeft] = -20;
	motor[FrontRight] = 20;
	motor[BackLeft] = -20;
	motor[BackRight]= 20;
	// Moving backwards
	motor[FrontLeft] = 20;
	motor[FrontRight] = -20;
	motor[BackLeft] = 20;
	motor[BackRight]= -20;

	// Turning in place to the left.
	motor[FrontLeft] = 20;
	motor[FrontRight] = 20;
	motor[BackLeft] = 20;
	motor[BackRight] = 20;
	// Turning in Place to the right.
	motor[FrontLeft] = -20;
	motor[FrontRight] = -20;
	motor[BackLeft] = -20;
	motor[BackRight] = -20;
	*/

	waitForStart();
	StartTask(joystickControl); //Go ahead and start critical joystick functions in their own task
	StartTask(armControl); //Start arm functions in their own task too
	while(true) {
		//Joystick 1 - Driver

		if(joy1Btn(6)) //If the driver is pressing button 6, scale down robot movements
			joystickScale = JOYSTICKLOW;
		else //Else leave at full speed
			joystickScale = JOYSTICKHIGH;

		if(joy1Btn(5)) //If the driver is pressing button 5, lock to y-axis movement
			sidelock = false;
		else
			sidelock = true;

		if(joy1Btn(7)) //If the driver is pressing button 7, lock to x-axis movement
			forwardlock = false;
		else
			forwardlock = true;
	}
}

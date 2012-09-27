#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     RightForward,  tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackSideways,  tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     FrontSideways, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     LeftForward,   tmotorTetrix, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

#define FORWARD 1440 //Number of encoder revolutions to go forward
#define SIDEWAYS 1400 //Number of encoder revolutions to go sideways (for left or right)

void right() {
	//Do something similar as going forward but using sideways motors
	nMotorEncoder[FrontSideways] = nMotorEncoder[BackSideways] = 0;
	motor[FrontSideways] = motor[BackSideways] = 100;
	while(nMotorEncoder[FrontSideways] < SIDEWAYS && nMotorEncoder[BackSideways] < SIDEWAYS);
	motor[FrontSideways] = motor[BackSideways] = 0;
}

void left() {
	nMotorEncoder[FrontSideways] = nMotorEncoder[BackSideways] = SIDEWAYS; //Since encoder will count down now, set it to SIDEWAYS revolutions...
	motor[FrontSideways] = motor[BackSideways] = -100;
	while(nMotorEncoder[FrontSideways] > 0 && nMotorEncoder[BackSideways] > 0);//... and wait until it is 0
	motor[FrontSideways] = motor[BackSideways] = 0;
}

void placeRing() {
	//To be done when the arm is finished and decided
}

task main() {
	//Initialize
	motor[LeftForward] = motor[RightForward] = motor[BackSideways] = motor[FrontSideways] = 0; //Turn off the motors
	nMotorEncoder[LeftForward] = nMotorEncoder[RightForward] = nMotorEncoder[BackSideways] = nMotorEncoder[FrontSideways] = 0; //Might as well reset the encoders too
	waitForStart();
	//Autonomous
	/* Here is the autonomous idea:
	*			Read the IR sensor to get a general area of the Beacon.  Use that to figure out the proper column (shouldn't be too hard)
	*			Then decide which autonomous set of commands to run.  Proper a bunch of if...else if... else if... else statements.  And
	*			If we can't find the IR Beacon (which we should make code for that just in case) then we will just put the ring on the
	*			center peg (or some other predefined spot if necessary).  The movements will be hardcoded so we can tune to the best accuracy.
	*/
	//Go forward for FORWARD revolutions up to the columns
	nMotorEncoder[RightForward] = nMotorEncoder[LeftForward] = 0; //Reset the encoders
	motor[RightForward] = motor[LeftForward] = 100; //Turn the motors on
	while(nMotorEncoder[RightForward] < FORWARD && nMotorEncoder[LeftForward] < FORWARD); //Wait until the encoders hit FORWARD
	motor[RightForward] = motor[LeftForward] = 0; //Stop the motors
	placeRing(); //Place the ring
	switch(SensorValue[IR]) { //0 if not found, else 1-9, 4 is a narrow area and will be positioned to be center
		case 1: //For values 1-3, assume the beacon is on the left column
		case 2:
		case 3:
			left();
			break;
		case 4: //For value 4 (center), assume the beacon is on the center column and just place the ring
			break;
		case 5: //For values 5-7, assume the beacon is on the right column
		case 6:
		case 7:
			right();
			break;
		default: //For value 0 (or other uncaught numbers), just put it on the center peg and hope for the best
	}
	placeRing(); //Go ahead and place the ring now that we are positioned
}

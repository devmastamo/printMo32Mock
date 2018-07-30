/*
 * mockMachine.c
 *
 *  Created on: Jul 20, 2018
 *      Author: alexmo
 */


#include "MockMachine.h"

#define STEPS_PER_REVOLUTION_EXTRUDE 	200 	// 360/1.8 deg
#define EXTRUDER_GEAR_DIAMETER			10		// mm's
#define PI 								3.14159

#define STEPS_PER_REVOLUTION_MOVE 		400 	// 360/0.9 deg
#define MICROSTEPS_PER_STEP 			256
#define MILLIS_PER_REVOLUTION			1.411	//Pretending 18 threads per inch -- TODO: put accurate value
#define TESTBED_MAX_X	 				120		// X Side in mm's -- TODO: get correct dimension
#define TESTBED_MAX_Y					120		// Y Side in mm's -- TODO: get correct dimension
#define TESTBED_MAX_Z					120		// Z Max height in mm's -- TODO: get correct dimension

static float gdNozzleLocation[4] = {0,0,0,0};	// [X,Y,Z] (mm's)

#define DIST_PER_STEP_X  		MILLIS_PER_REVOLUTION / (STEPS_PER_REVOLUTION_MOVE * MICROSTEPS_PER_STEP)
#define DIST_PER_STEP_Y  		MILLIS_PER_REVOLUTION / (STEPS_PER_REVOLUTION_MOVE * MICROSTEPS_PER_STEP)
#define DIST_PER_STEP_Z  		MILLIS_PER_REVOLUTION / (STEPS_PER_REVOLUTION_MOVE * MICROSTEPS_PER_STEP)
#define EXTRUDE_LEN_PER_STEP  	(PI * EXTRUDER_GEAR_DIAMETER)/STEPS_PER_REVOLUTION_EXTRUDE

static void PlotCoords(float * rdNozzleLoc)
{
	printf("%f, %f, %f, %f\r\n", gdNozzleLocation[AXIS_X], gdNozzleLocation[AXIS_Y], gdNozzleLocation[AXIS_Z], gdNozzleLocation[EXTRUDER]);
}

static void EndStopReport(eEndstops rtEndstop)
{
	printf("[%d] ENDSTOPPED\n", rtEndstop);
}

/* Real-time Timer callbacks */
void MotorTimerCb(eMotor rtMotor, eDirection rtDirection)
{
	int Coefficient = (rtDirection == FORWARD)?1:-1;

	switch(rtMotor)
	{
	case AXIS_X:

		if(gdNozzleLocation[AXIS_X] + (Coefficient * DIST_PER_STEP_X) > TESTBED_MAX_X)
		{
			EndStopReport(ENDSTOP_X1);
		}
		else if(gdNozzleLocation[AXIS_X] + (Coefficient * DIST_PER_STEP_X) < 0)
		{
			EndStopReport(ENDSTOP_X0);
		}
		else
		{
			gdNozzleLocation[AXIS_X] += (Coefficient * DIST_PER_STEP_X);
		}
		break;
	case AXIS_Y:
		if(gdNozzleLocation[AXIS_Y] + (Coefficient * DIST_PER_STEP_Y) > TESTBED_MAX_Y)
		{
			EndStopReport(ENDSTOP_Y1);
		}
		else if(gdNozzleLocation[AXIS_Y] + (Coefficient * DIST_PER_STEP_Y) < 0)
		{
			EndStopReport(ENDSTOP_Y0);
		}
		else
		{
			gdNozzleLocation[AXIS_Y] += (Coefficient * DIST_PER_STEP_Y);
		}
		break;
	case AXIS_Z:
		if(gdNozzleLocation[AXIS_Z] + (Coefficient * DIST_PER_STEP_Z) > TESTBED_MAX_Z)
		{
			EndStopReport(ENDSTOP_Z1);
		}
		else if(gdNozzleLocation[AXIS_Z] + (Coefficient * DIST_PER_STEP_Z) < 0)
		{
			EndStopReport(ENDSTOP_Z0);
		}
		else
		{
			gdNozzleLocation[AXIS_Z] += (Coefficient * DIST_PER_STEP_Z);
		}
		break;
	case EXTRUDER:
		gdNozzleLocation[EXTRUDER] += (Coefficient * EXTRUDE_LEN_PER_STEP);
		break;
	}
	PlotCoords(gdNozzleLocation);
}

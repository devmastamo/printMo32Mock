#ifndef MOCK_MACHINE_H

#include "MoTypes.h"

typedef enum eEndstops_t
{
    ENDSTOP_X0,
    ENDSTOP_X1,
    ENDSTOP_Y0,
    ENDSTOP_Y1,
    ENDSTOP_Z0,
    ENDSTOP_Z1
} eEndstops;

typedef enum eDirection_t
{
    BACKWARD = 0,
    FORWARD = 1
} eDirection;

typedef enum eMotor_t
{
    AXIS_X = 0,
    AXIS_Y = 1,
    AXIS_Z = 2,
    EXTRUDER = 3
} eMotor;

void (*pMotorStepTimerCb)(eMotor,eDirection);

// Each call is a motor step
void MotorTimerCb(eMotor rtMotor, eDirection rtDirection);

#endif

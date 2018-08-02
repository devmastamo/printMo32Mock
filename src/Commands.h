#ifndef COMMANDS_H
#define COMMANDS_H

#include "MoTypes.h"

typedef enum eCommandType_t
{
    G_COMMAND,
    M_COMMAND,
    T_COMMAND,
    SKIP_LINE,
    ERROR,
    CONTINUE,
    UNKNOWN_COMMAND
} eCommandType;

typedef struct Command_t
{
    eCommandType    tGCommand;
    uint16_t        dCommandNumber;
    float           fSField;
    float           fPField;
    float           fXField;
    float           fYField;
    float           fZField;
    float           fIField;
    float           fJField;
    float           fDField;
    float           fHField;
    float           fFField;
    float           fRField;
    float           fQField;
    float           fEField;
    float           fNField;
} Command;

//Helper Fxns
void GetCurrentCoordinates(float * rdCurX, float * rdCurY, float * rdCurZ);

float GetCurrentExtruderTemp();

float GetCurrentBedTemp();

//Public Functions
/* GCODES */
//G1, etc
float LinearMove(float rdNextX, float rdNewY, float rdNewZ);    //Returns total length to move

void Extrude(float rdExtrusionAmt, float rdLength, float rdFeedRate);   // rdExtrusionAmt + rdLength OR rdFeedRate. Length can come from move commands

// G20
void SetUnitsToInches();

// G21
void SetUnitsToMillimeters();

// G28
void HomeAxes();

// G90
void SetToAbsolutePositioning();

// G91
void SetToRelativePositioning();

// G92
void ResetOrigin(float rdX, float rdY, float rdZ, float rdExtruder);

/* MCODES */
// M82
void SetToAbsouteExtrusion();

// M83
void SetToRelativeExtrusion();

// M84
void DisableMotors();  

// M104 - no wait, M109 - wait0
void SetExtruderTemp(uint16_t rdExtruderTemp, bool rbNoWait);  //Args in Celcius

// M106
void FanOn(float rdFanSpeed);

// M107
void FanOff();

// M140
void SetBedTemp(uint16_t rdTargetTemp, uint16_t rdStandbyTemp);     // If StandyTemp is 0, don't use it

#endif // COMMANDS_H

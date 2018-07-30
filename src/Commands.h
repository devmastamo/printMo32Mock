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
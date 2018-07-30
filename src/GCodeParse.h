/*
 * GCodeParse.h
 *
 *  Created on: Jul 18, 2018
 *      Author: alexmo
 */

#ifndef GCODEPARSE_H_
#define GCODEPARSE_H_

#include "MoTypes.h"

#define MAX_FLOAT_DIGITS 12

typedef enum eParseState_t
{
    PARSE_STATE_START,
    PARSE_STATE_PARSE_COMMAND,
    PARSE_STATE_PARSE_COMMAND_NUM,
    PARSE_STATE_PARSE_ARGUMENT,
    PARSE_STATE_PARSE_ARGUMENT_NUM
} eParseState;

typedef enum eParseReturn_t
{
    PARSE_RETURN_DONE,
    PARSE_RETURN_READ_MORE,
    PARSE_RETURN_ERROR,
    PARSE_RETURN_LAST
} eParseReturn;

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


eParseReturn ParseNextInstruction(char * rpCharString, size_t rdSize, size_t * rdIterStart, Command * rpCommand, char * rpField, char * rpParseNum, size_t * rpParseNumIndex, eParseState * tParseState);

bool QueueNextCommand(Command rtCommand, Command * rpCommandQueue);

#endif /* GCODEPARSE_H_ */

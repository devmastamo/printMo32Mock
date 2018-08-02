/*
 * GCodeParse.h
 *
 *  Created on: Jul 18, 2018
 *      Author: alexmo
 */

#ifndef GCODEPARSE_H_
#define GCODEPARSE_H_

#include <stdio.h>
#include "MoTypes.h"
#include "Commands.h"

#define MAX_FLOAT_DIGITS 12
#define BUFFER_SIZE 100

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



typedef struct Parser_t
{
    char            pBuffer[BUFFER_SIZE];
    eParseReturn    tParseReturn;
    size_t          dContinueLoc;
    Command         tCommand;
    char            cField;
    char            pParseNum[MAX_FLOAT_DIGITS];
    size_t          dParseNumIndex;
    eParseState     tParseState;
} Parser;

void ParserInit(Parser * rpParser);

void ParseFile(Parser * rpParser, const char * rpFileName);

void ParseFinish(Parser * rpParser);

eParseReturn ParseNextInstruction(char * rpCharString, size_t rdSize, size_t * rdIterStart, Command * rpCommand, char * rpField, char * rpParseNum, size_t * rpParseNumIndex, eParseState * tParseState);

bool QueueNextCommand(Command rtCommand, Command * rpCommandQueue);

#endif /* GCODEPARSE_H_ */

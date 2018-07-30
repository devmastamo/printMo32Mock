/*
 ============================================================================
 Name        : Testbed.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "MoTypes.h"
#include "MockMachine.h"
#include "GCodeParse.h"

bool gbCanExit = false;

void timer_thread(union sigval arg)
{
    gbCanExit = true;
}

#define BUFFER_SIZE 100

int main(void) {
	size_t retCode;
	char buffer[BUFFER_SIZE] = {0};
	FILE * fStream = fopen("Debug/baymax.gcode", "r");
    eParseReturn tParseReturn = PARSE_RETURN_READ_MORE;
    size_t dContinueLoc = 0;
    Command tCommand = {0};
    char cField = ' ';
    char pParseNum[MAX_FLOAT_DIGITS];
    size_t dParseNumIndex = 0;
    eParseState tParseState = PARSE_STATE_START;

    size_t numCommandsProcessed = 0;

	if(fStream != NULL)
	{
            

		do
		{
            if(tParseReturn == PARSE_RETURN_READ_MORE)
            {
			    retCode = fread(buffer, 1, sizeof(buffer), fStream);
                //printf("%s%s", buffer, "\r\n");
            }
            if(retCode > 0 && !feof(fStream))
            {
                tParseReturn = ParseNextInstruction(buffer, retCode, &dContinueLoc, &tCommand, &cField, pParseNum, &dParseNumIndex, &tParseState);
                if(tParseReturn == PARSE_RETURN_DONE)
                {
                    printf("[%u]-%u S%f P%f X%f Y%f Z%f I%f J%f D%f H%f F%f R%f Q%f E%f N%f%s",
                        tCommand.tGCommand,
                        tCommand.dCommandNumber,
                        tCommand.fSField,
                        tCommand.fPField,
                        tCommand.fXField,
                        tCommand.fYField,
                        tCommand.fZField,
                        tCommand.fIField,
                        tCommand.fJField,
                        tCommand.fDField,
                        tCommand.fHField,
                        tCommand.fFField,
                        tCommand.fRField,
                        tCommand.fQField,
                        tCommand.fEField,
                        tCommand.fNField, 
                        "\r\n");

                    if(tCommand.dCommandNumber == 104)
                    {
                        printf("!!!104!!!%s", "\r\n");
                    }
                    memset(&tCommand, 0, sizeof(Command));
                    cField = ' ';
                    memset(pParseNum, 0, sizeof(pParseNum));
                    dParseNumIndex = 0;

                    numCommandsProcessed++;
                }
            }
            else
            {   
                printf("ERROR IN FILE READ%s", "\r\n");
                break;
            }
		} while(tParseReturn != PARSE_RETURN_ERROR);
		fclose(fStream);
	}
	else
	{
		printf("ERROR OPENING FILE! %d%s", errno, "\r\n");
	}

    printf("DONE - processed %u commands!%s", numCommandsProcessed, "\r\n");
#if 0
    int status;
    timer_t timer_id;
    struct sigevent se;
    struct itimerspec ts;

    se.sigev_notify = SIGEV_THREAD;
    se.sigev_value.sival_ptr = &timer_id;
    se.sigev_notify_function = timer_thread;
    se.sigev_notify_attributes = NULL;

    ts.it_value.tv_sec = 10;
    ts.it_value.tv_nsec = 0;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;

    status = timer_create(CLOCK_REALTIME, &se, &timer_id);
    if(status == -1)
        printf("fail create!");

    status = timer_settime(timer_id, 0, &ts, 0);
    if(status == -1)
        printf("fail settime!");
    
    printf("Start\n");


    while(!gbCanExit)
    {

    }

    printf("End\n");
#endif
	return EXIT_SUCCESS;
}

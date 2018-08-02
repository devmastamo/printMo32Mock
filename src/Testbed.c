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

int main(void) {
    //Initialize parser
    Parser tParser;
    ParserInit(&tParser);
    
    //Initialize the timers
    

    //This part is critical. Don't do anything except getting commands
    //    and using interrupts
    ParseFile(&tParser, "Debug/baymax.gcode");

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

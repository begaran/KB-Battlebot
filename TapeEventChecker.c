/*
 * File:   TemplateEventChecker.c
 * Author: Gabriel Hugh Elkaim
 *
 * Template file to set up typical EventCheckers for the  Events and Services
 * Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the
 * names will have to be changed to match your code.
 *
 * This EventCheckers file will work with both FSM's and HSM's.
 *
 * Remember that EventCheckers should only return TRUE when an event has occured,
 * and that the event is a TRANSITION between two detectable differences. They
 * should also be atomic and run as fast as possible for good results.
 *
 * This file includes a test harness that will run the event detectors listed in the
 * ES_Configure file in the project, and will conditionally compile main if the macro
 * EVENTCHECKER_TEST is defined (either in the project or in the file). This will allow
 * you to check you event detectors in their own project, and then leave them untouched
 * for your project unless you need to alter their post functions.
 *
 * Created on September 27, 2013, 8:37 AM
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "TapeEventChecker.h"
#include "TapeService.h"
#include "ES_Events.h"
#include "serial.h"
#include "AD.h"
#include "IO_Ports.h"
#include "BOARD.h"
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

#define BLACK_THRESH 100

/*******************************************************************************
 * EVENTCHECKER_TEST SPECIFIC CODE                                                             *
 ******************************************************************************/

//#define EVENTCHECKER_TEST
#ifdef EVENTCHECKER_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this EventChecker. They should be functions
   relevant to the behavior of this particular event checker */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
/* Any private module level variable that you might need for keeping track of
   events would be placed here. Private variables should be STATIC so that they
   are limited in scope to this module. */

   unsigned int sensorBank[6] = { // these are in TapeEventChecker.h
       TAPESENSOR1,
       TAPESENSOR2,
       TAPESENSOR3,
       TAPESENSOR4,
       TAPESENSOR5,
       TAPESENSOR6
   } ;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/


uint8_t TapeCheck(void) {
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    static uint16_t lastTapeRead = 0x0000 ;
    uint16_t tapeReadings = 0x0000 ;
    
    //printf("sensor 1: %x\n", IO_PortsReadPort(PORTY));
    //tapeReadings = IO_PortsReadPort(PORTY);
    
    printf("%x\n", AD_ReadADPin(TAPESENSOR1));
    /*
    for(uint8_t i = 0 ; i < 6 ; i++) {
        // Read sensors sequentially.
        
        unsigned int data = AD_ReadADPin(sensorBank[i]) ;
        printf("TC[%u]: %x\n", i + 1, data) ;
        //if(AD_ReadADPin(sensorBank[i]) > BLACK_THRESH) { // CHECK IF BLACK. IF WHITE DO NOTHING. (?)
        if(data > BLACK_THRESH) {
            //printf("sensor %u: data = %x\n", i, data) ;
            uint16_t X = 0x0001 << i; 
            tapeReadings |= X;
        }
    }
    */
    //printf("TAPE: %x\n", tapeReadings) ;
    
    if (!(lastTapeRead ^ tapeReadings)) { // check for change from last time
        thisEvent.EventType = TAPE_SENSED ;
        thisEvent.EventParam = tapeReadings;
        returnVal = TRUE;
        lastTapeRead = tapeReadings ; // update history
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
        PostTapeService(thisEvent) ;
#else
        SaveEvent(thisEvent) ;
#endif   
    }
    return (returnVal) ;
}

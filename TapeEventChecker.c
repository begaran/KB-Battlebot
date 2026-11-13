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
#include "xc.h"
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

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

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/


uint8_t TapeCheck(void) {
    // initialization
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    static uint16_t lastTapeRead = 0x0000 ;
    uint16_t tapeReadings = 0x0000 ;
   
    // read ports
    if(PORTY06_BIT) tapeReadings |= 0x0001 ;
    // printf("[1]: %x\n", PORTY06_BIT) ;
    if(PORTY07_BIT) tapeReadings |= 0x0002 ;
    // printf("[2]: %x\n", PORTY07_BIT) ;
    if(PORTY08_BIT) tapeReadings |= 0x0004 ;  
    // printf("[3]: %x\n", PORTY08_BIT) ;
    if(PORTY09_BIT) tapeReadings |= 0x0008 ;  
    // printf("[4]: %x\n", PORTY09_BIT) ;
    if(PORTY10_BIT) tapeReadings |= 0x0010 ;  
    // printf("[5]: %x\n", PORTY10_BIT) ;
    if(PORTY11_BIT) tapeReadings |= 0x0020 ;  
    // printf("[6]: %x\n", PORTY11_BIT) ;

    // check if changed
    if (lastTapeRead != tapeReadings) { // check for change from last time
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

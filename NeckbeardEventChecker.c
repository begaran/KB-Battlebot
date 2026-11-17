/*
 * File:   NeckbeardEventChecker.c
 * Author: Gabriel Hugh Elkaim
 * Event checker for all KB Battlebot MPLABX projects
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "NeckbeardEventChecker.h"
#include "TapeService.h"
#include "ES_Events.h"
#include "serial.h"
#include "AD.h"
#include "IO_Ports.h"
#include "BOARD.h"
#include "xc.h"
#include "ES_Timers.h"
#include "limits.h"
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

#define WAITTIME 500

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

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/

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
    if(PORTY06_BIT) tapeReadings |= 0x0001 ; // 0000 0000 0000 0001
    if(PORTY07_BIT) tapeReadings |= 0x0002 ; // 0000 0000 0000 0010
    if(PORTY08_BIT) tapeReadings |= 0x0004 ; // 0000 0000 0000 0100
    if(PORTY09_BIT) tapeReadings |= 0x0008 ; // 0000 0000 0000 1000
    if(PORTY10_BIT) tapeReadings |= 0x0010 ; // 0000 0000 0001 0000
    if(PORTY11_BIT) tapeReadings |= 0x0020 ; // 0000 0000 0010 0000

    // check if changed
    if (lastTapeRead != tapeReadings) { // check for change from last time
        thisEvent.EventType = TAPE_SENSED ;
        thisEvent.EventParam = tapeReadings;
        returnVal = TRUE;
        lastTapeRead = tapeReadings ; // update history
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
        PostNeckbeardService(thisEvent) ;
#else
        SaveEvent(thisEvent) ;
#endif   
    }
    return (returnVal) ;
}

uint8_t BumperCheck(void) {
    // initialization
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    static uint16_t lastBumperRead = 0x0000 ;
    uint16_t bumperReadings = 0x0000 ; 
    unsigned int currentTime = ES_Timer_GetTime() ; // timer for debouncing
    static unsigned int prevTime = 0 ;

    if(!PORTZ09_BIT) bumperReadings |= 0b0001 ; 
    if(!PORTZ10_BIT) bumperReadings |= 0b0010 ; 
    if(!PORTZ11_BIT) bumperReadings |= 0b0100 ; 
    if(!PORTZ12_BIT) bumperReadings |= 0b1000 ; 

    //printf("cur: %x, last: %x\n", bumperReadings, lastBumperRead) ;
    if(lastBumperRead != bumperReadings) {
        if((currentTime - prevTime > WAITTIME) | ((currentTime - prevTime < 0)&(currentTime - prevTime > (WAITTIME-UINT_MAX)))) {
            // printf("l:%x | c:%x\n", lastBumperRead, bumperReadings) ;
            thisEvent.EventType = BUMPER_HIT ;
            thisEvent.EventParam = bumperReadings ; 
            returnVal = TRUE ;
            lastBumperRead = bumperReadings ;
            prevTime = currentTime ;
        }
    }
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
        PostNeckbeardService(thisEvent) ;
#else
        SaveEvent(thisEvent) ;
#endif 
  
    return (returnVal) ;
}

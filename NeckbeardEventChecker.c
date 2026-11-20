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

#define WAITTIME 500        // for timer debouncing on bumpers

//states
#define TRACKLO 0x00        // track wire not sensed
#define TRACKHI 0x01        // track wire close
#define BCNLO   0xAB        // beacon very far
#define BCNMID  0xAC        // beacon within a certain range
#define BCNHI   0xAD        // beacon close enough to shoot

// Hysteresis thresholds
#define TLO 400             // track lo
#define THI 600             // track hi 
#define BML 400             // beacon mid lo
#define BMH 500             // beacon mid hi
#define BHL 600             // beacon hi lo (possibly unused) 
#define BHH 800             // beacon hi hi

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
    
    static uint16_t lastTapeRead = 0x0000 ; // need to store last tapes read
    uint16_t tapeReadings = 0x0000 ;        // read sensor data into this
   
    // read ports
    if(PORTY06_BIT) tapeReadings |= 0x0001 ; // 0000 0000 0000 0001, sensor 1
    if(PORTY07_BIT) tapeReadings |= 0x0002 ; // 0000 0000 0000 0010, sensor 2
    if(PORTY08_BIT) tapeReadings |= 0x0004 ; // 0000 0000 0000 0100, sensor 3
    if(PORTY09_BIT) tapeReadings |= 0x0008 ; // 0000 0000 0000 1000, sensor 4
    if(PORTY10_BIT) tapeReadings |= 0x0010 ; // 0000 0000 0001 0000, sensor 5
    if(PORTY11_BIT) tapeReadings |= 0x0020 ; // 0000 0000 0010 0000, sensor 6

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

    if(!PORTZ09_BIT) bumperReadings |= 0b0001 ; // left front
    if(!PORTZ03_BIT) bumperReadings |= 0b0010 ; // right front
    if(!PORTZ11_BIT) bumperReadings |= 0b0100 ; // left back
    if(!PORTZ12_BIT) bumperReadings |= 0b1000 ; // right back

    if(lastBumperRead != bumperReadings) {
        if( (currentTime > prevTime && currentTime - prevTime > WAITTIME) || 
            (currentTime < prevTime && currentTime + (UINT_MAX-prevTime) > WAITTIME) 
          ) {
            thisEvent.EventType = BUMPER_HIT ;
            thisEvent.EventParam = bumperReadings ; 
            returnVal = TRUE ;
            lastBumperRead = bumperReadings ;   // update bumper state
            prevTime = currentTime ;            // prevTime holds last time of latch
            PostNeckbeardService(thisEvent) ;
        }    
    }
    return (returnVal) ;
}

uint8_t TrackCheck(void) {
    // initialization
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    static uint8_t state = TRACKLO ;
    
    unsigned int data = AD_ReadADPin(AD_PORTW3) ;
    if(state == TRACKLO && data > THI) {
        state = TRACKHI ;
        thisEvent.EventType = TRACK_SENSED ;
        thisEvent.EventParam = data ; 
        returnVal = TRUE ;
        PostNeckbeardService(thisEvent) ;
    } else if(state == TRACKHI && data < TLO) {
        state = TRACKLO ;
        thisEvent.EventType = TRACK_LOST ;
        thisEvent.EventParam = data ;
        returnVal = TRUE ;
        PostNeckbeardService(thisEvent) ;
    }
    return (returnVal) ;
}

uint8_t BeaconCheck(void) {
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    static uint8_t state = BCNLO ;
    
    unsigned int data = AD_ReadADPin(AD_PORTW4) ;
    if(state == BCNLO && data > BMH) {
        state = BCNMID ;
        thisEvent.EventType = BEACON_SENSED ;
        thisEvent.EventParam = (uint16_t)data ;
        returnVal = TRUE ;
        PostNeckbeardService(thisEvent) ;
    } else if((state == BCNMID || state == BCNHI) && data < BML) {
        state = BCNLO ;
        thisEvent.EventType = BEACON_LOST ;
        thisEvent.EventParam = (uint16_t)data ;
        returnVal = TRUE ;
        PostNeckbeardService(thisEvent) ;
    } else if((state == BCNMID || state == BCNLO)  && data > BHH) {
        state = BCNHI ;
        thisEvent.EventType = BEACON_CLOSE ;
        thisEvent.EventParam = (uint16_t)data ;
        returnVal = TRUE ;
        PostNeckbeardService(thisEvent) ;
    } else if(state == BCNHI && data < BHL) {
        state = BCNMID ;
        thisEvent.EventType = BEACON_SENSED ;
        thisEvent.EventParam = (uint16_t)data ;
        returnVal = TRUE ;
        PostNeckbeardService(thisEvent) ;
    }
    
    return returnVal ;
}
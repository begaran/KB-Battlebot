/*
 * File: TapeService.h
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "BOARD.h"
#include "AD.h"
#include "IO_Ports.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "TapeService.h"
#include "NeckbeardEventChecker.h"
#include "Locomotion.h"
#include "LED.h"
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

#define BT 4000 // time in milliseconds for obstacle avoidance 

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
static uint8_t bumpTime = BT;
static uint8_t MyPriority;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

uint8_t InitTapeService(uint8_t Priority) {
    ES_Event ThisEvent;
    MyPriority = Priority;

    ThisEvent.EventType = ES_INIT;
    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

uint8_t PostNeckbeardService(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}


ES_Event RunTapeService(ES_Event ThisEvent) {
    ES_Event ReturnEvent;
    ReturnEvent.EventType = ES_NO_EVENT; // assume no errors

    static ES_EventTyp_t lastEvent = TAPE_SENSED;
    ES_EventTyp_t curEvent;

    switch (ThisEvent.EventType) {
    case ES_INIT :
        break;
    case TAPE_SENSED :
        uint8_t sensorReadings = (uint8_t)ThisEvent.EventParam ; // Typecast the sensor readings as a filthy byte
        LED_SetBank(LED_BANK1, (unsigned char)sensorReadings) ;
        printf("sensors: 0x%x\n", sensorReadings) ;
        if(sensorReadings & 0x08 || !(sensorReadings ^ 0x07)) { // Tape Sensor 4 or 1, 2, 3
            // 1xxx, 0111
            revolutionTurn(RIGHT);
            LED_SetBank(LED_BANK3, 0xFF) ;
        } else if (!(sensorReadings ^ 0x01) || !(sensorReadings ^ 0x05)) { 
            // 0001, 0101
            move(FORWARD) ;
            LED_SetBank(LED_BANK3, 0xA5) ;
        } else if (!(sensorReadings ^ 0x02) || !(sensorReadings ^ 0x06)) { 
            // 0010, 0110
            driveTurn(RIGHT) ;
            LED_SetBank(LED_BANK3, 0x0F) ;
        } else {
            // 0011, 0100
            driveTurn(LEFT) ;
            LED_SetBank(LED_BANK3, 0xF0) ;
        } 
        break;
    case BUMPER_HIT : 
        uint8_t bumperReadings = (uint8_t)ThisEvent.EventParam ; // Bytecast
        LED_SetBank(LED_BANK2, (unsigned char)bumperReadings) ;
        printf("bumpers: 0x%x\n", bumperReadings) ;
        if(bumperReadings & 0b0001) printf("LF\n") ;
        if(bumperReadings & 0b0010) printf("RF\n") ;
        if(bumperReadings & 0b0100) printf("LB\n") ;
        if(bumperReadings & 0b1000) printf("RB\n") ;
            
#ifdef SIMPLESERVICE_TEST     // keep this as is for test harness      
    default:
        printf("\r\nEvent: %s\tParam: 0x%X",
                EventNames[ThisEvent.EventType], ThisEvent.EventParam);
        break;
#endif
    }

    return ReturnEvent;
}

/*******************************************************************************
 * PRIVATE FUNCTIONs                                                           *
 ******************************************************************************/


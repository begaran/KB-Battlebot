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
#include "ES_Timers.h"
#include "TapeService.h"
#include "NeckbeardEventChecker.h"
#include "Locomotion.h"
#include "LED.h"
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

#define BT (2)*1024             // time in milliseconds for obstacle avoidance.
#define AVOIDTIMER 1            // timer for bump maneuvers
#define TT 1500
#define TURNTIMER 0

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
static uint16_t bumpTime = BT ;   // initial time that the bot spends moving away from obstacles
uint16_t turnTime = TT ;
uint8_t curTape ;
uint8_t curBump ;
static uint8_t MyPriority ;
/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

uint8_t InitTapeService(uint8_t Priority) {
    ES_Event ThisEvent;
    MyPriority = Priority;
    
    ES_Timer_Init() ;
    //ES_Timer_InitTimer(TURNTIMER, 500) ;
    ES_Timer_InitTimer(AVOIDTIMER, BT) ;
    ES_Timer_StopTimer(AVOIDTIMER) ;
    ES_Timer_StopTimer(TURNTIMER) ;

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
        curTape = sensorReadings ;
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
            // driveTurn(RIGHT) ;
            move(FORWARD) ;
            LED_SetBank(LED_BANK3, 0x0F) ;
        } else {
            // 0000, 0011, 0100
            driveTurn(LEFT) ;
            LED_SetBank(LED_BANK3, 0xF0) ;
        } 
        break;
        
    case BUMPER_HIT : 
        uint8_t bumperReadings = (uint8_t)ThisEvent.EventParam ; // Bytecast
        LED_SetBank(LED_BANK2, (unsigned char)bumperReadings) ;
        curBump = bumperReadings ;
        printf("bumpers: 0x%x\n", bumperReadings) ;
        
        if(bumperReadings & 0b0011) {
            printf("Front hit\n") ;
            bumpTime /= 2 ;
            backTurn(RIGHT) ;
            ES_Timer_SetTimer(AVOIDTIMER, bumpTime) ;
            ES_Timer_StartTimer(AVOIDTIMER) ;
        }
        if(bumperReadings & 0b1100) {
            printf("Back hit\n") ;
            bumpTime /= 2 ;
            backTurn(RIGHT) ;
            ES_Timer_SetTimer(AVOIDTIMER, bumpTime) ;
            ES_Timer_StartTimer(AVOIDTIMER) ;
        }     
        
        break ;
        
    case ES_TIMEOUT :
        if(ThisEvent.EventParam == AVOIDTIMER) {
            //printf("Avoid timer TIMEOUT\n") ;
            bumpTime = BT ;
            // revolutionTurn(RIGHT) ;
            driveTurn(LEFT) ; // may need to change this to check tape
            ES_Timer_SetTimer(TURNTIMER, turnTime) ;
            ES_Timer_StartTimer(TURNTIMER) ;
        } else if(ThisEvent.EventParam == TURNTIMER) {
            //printf("Turn timer TIMEOUT\n") ;
            //TapeCheck() ; // might be quite dangerous
            // if robot stays on same tape whole time, it won't get a TAPE_SENSED event
        }
        
        
        break ;

            
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


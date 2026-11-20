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
#include "Turret.h"
#include "LED.h"
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

#define BT (2)*192              // time in milliseconds for obstacle avoidance.
#define AVOIDTIMER 1            // timer for bump maneuvers
#define TT 1500                 // turn time (unused)
#define TURNTIMER 0             // timer for turning after a bump (unused)

// states
#define s_BACKWARD 0x01 
#define s_FORWARD 0x02 
#define s_AVOID 0x03

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
static uint16_t bumpTime = BT ;     // initial time that the bot spends moving away from obstacles
uint16_t turnTime = TT ;            // unused
uint8_t curTape ;                   // tape as a stored variable
uint8_t curBump ;                   // bumpers as a stored variable
static uint8_t MyPriority ;         
static uint8_t state = s_FORWARD ;
uint8_t lastTapes[16] = {0} ;
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
        
        if(state == s_FORWARD) {
			// another way to do this would be to store the movement behavior and use the readings to index into one
            sensorReadings &= 0x0F ;    // mask upper bits
            if(sensorReadings & 0x08 /*|| !(sensorReadings ^ 0x07)*/) {         // Tape Sensor 4
                // 1xxx, 0111
                revolutionTurn(RIGHT);
                LED_SetBank(LED_BANK3, 0xFF) ;
            } else if (!(sensorReadings ^ 0x01) || !(sensorReadings ^ 0x05)) {  // 
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
        } else if(state == s_BACKWARD) {
            if(sensorReadings & 0x04) {
                revolutionTurn(LEFT) ;
            //} else if(sensorReadings & 0x08) {
              //  move(BACKWARD) ;
            //} else if(sensorReadings & 0x08) {
            //    revolutionTurn(LEFT) ;
            //} else if(!sensorReadings) {
              //  move(BACKWARD) ;
            } else {
                backTurn(RIGHT) ;
            }
            /*
			if(sensorReadings & 0x04) { 
                // xxx1xx
                revolutionTurn(LEFT);
                LED_SetBank(LED_BANK3, 0xFF) ;
            } else if (sensorReadings & 0x08) { 
                // 10xx
                backTurn(LEFT) ;
                LED_SetBank(LED_BANK3, 0xA5) ;
            } else {
                // 0000, 0011, 0100
                move(BACKWARD) ;
                LED_SetBank(LED_BANK3, 0xF0) ;
            } 
            */
		}
        break; // case TAPE_SENSED
        
    case BUMPER_HIT : 

        uint8_t bumperReadings = (uint8_t)ThisEvent.EventParam ; // Bytecast
        LED_SetBank(LED_BANK2, (unsigned char)bumperReadings) ;
        curBump = bumperReadings ;
        printf("bumpers: 0x%x\n", bumperReadings) ;
        
        if(bumperReadings & 0b0011 && state == s_FORWARD) { // front bumpers 
            state = s_BACKWARD ;
            if(sensorReadings & 0x04) {
                revolutionTurn(LEFT) ;
            //} else if(sensorReadings & 0x08) {
              //  move(BACKWARD) ;
            //} else if(sensorReadings & 0x08) {
            //    revolutionTurn(LEFT) ;
            //} else if(!sensorReadings) {
              //  move(BACKWARD) ;s
            } else {
                backTurn(RIGHT) ;
            }
            /*
			if(curTape & 0x04) { 
                // x1xx
                revolutionTurn(LEFT);
                LED_SetBank(LED_BANK3, 0xFF) ;
            } else if (curTape & 0x08) { 
                // 10xx
                backTurn(LEFT) ;
                LED_SetBank(LED_BANK3, 0xA5) ;
            } else {
                // 00xx
                move(BACKWARD) ;
                LED_SetBank(LED_BANK3, 0xF0) ;
            } 
            */
        } else if(bumperReadings & 0b1100 && state == s_BACKWARD) {
			//state = s_AVOID ;
			// go around
		}

        break ; // case BUMPER_HIT
        
    case ES_TIMEOUT :
        // currently unused
        if(ThisEvent.EventParam == AVOIDTIMER) {
            //printf("Avoid timer TIMEOUT\n") ;
            bumpTime = BT ;
            // revolutionTurn(RIGHT) ;
            if(curTape & 0x08 || !(curTape ^ 0x07)) { revolutionTurn(RIGHT) ; LED_SetBank(LED_BANK3, 0xFF) ;
            } else if (!(curTape ^ 0x01) || !(curTape ^ 0x05)) { move(FORWARD) ; LED_SetBank(LED_BANK3, 0xA5) ;
            } else if (!(curTape ^ 0x02) || !(curTape ^ 0x06)) { move(FORWARD) ; LED_SetBank(LED_BANK3, 0x0F) ;
            } else { driveTurn(LEFT) ; LED_SetBank(LED_BANK3, 0xF0) ; } 
            
            state = s_FORWARD ;
            // ES_Timer_SetTimer(TURNTIMER, turnTime) ;
            // ES_Timer_StartTimer(TURNTIMER) ;
        } else if(ThisEvent.EventParam == TURNTIMER) {
            //printf("Turn timer TIMEOUT\n") ;
            //TapeCheck() ; // might be quite dangerous
            // if robot stays on same tape whole time, it won't get a TAPE_SENSED event
        }
        break ; // case ES_TIMEOUT
      
    case TRACK_SENSED :
        printf("Track sensed\n") ;
        break ; // case TRACK_SENSED
    case TRACK_LOST :
        printf("Track lost\n") ;
        break ; // case TRACK_LOST
    case BEACON_SENSED :
        printf("Beacon sensed\n") ;
        break ; // case BEACON_SENSED
    case BEACON_LOST :
        printf("Beacon lost\n") ;
        break ; // case BEACON_LOST
    case BEACON_CLOSE :
        printf("Beacon close\n") ;
        break ; // case BEACON_CLOSE

            
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


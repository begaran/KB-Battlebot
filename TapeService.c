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

// states
#define s_BACKWARD 0x01 
#define s_FORWARD 0x02 
#define s_SHOOTER 0x03

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
void forwardTape(uint8_t data) {
    data &= 0x0F ;    // mask upper bits
    if(data & 0x08) {         // Tape Sensor 4
        // 1xxx, 0111
        revolutionTurn(RIGHT);
        LED_SetBank(LED_BANK3, 0xFF) ;
    } else if (!(data ^ 0x01) || !(data ^ 0x05)) {  
        // 0001, 0101
        move(FORWARD) ;
        LED_SetBank(LED_BANK3, 0xA5) ;
    } else if (!(data ^ 0x02) || !(data ^ 0x06)) { 
        // 0010, 0110
        move(FORWARD) ;
        LED_SetBank(LED_BANK3, 0x0F) ;
    } else {
        // 0000, 0011, 0100
        driveTurn(LEFT) ;
        LED_SetBank(LED_BANK3, 0xF0) ;
    } 
    return ;
}

void backwardTape(uint8_t data) {
    if(data & 0x20) {
        revolutionTurn(LEFT) ;
        // 0xx xxx
    } else if((!(data & 0b100111)) || !((data & 0b111) ^ 0b110)) {
        backTurn(RIGHT) ;
    } else if(!((data & 0b111)^0b001) || !((data & 0b111)^0b010)) {
        backTurn(RIGHT) ;
    } else if(!(data ^ 0b111)) {
        backTurn(LEFT) ;
    } else {
        move(BACKWARD) ;
    }
    return ;
}

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
uint8_t curTape ;                   // tape as a stored variable
uint8_t curBump ;                   // bumpers as a stored variable
static uint8_t MyPriority ;         
static uint8_t state = s_BACKWARD ;  // state variable 1
uint8_t lastTapes[16] = {0} ;      
static uint8_t tape_i = 0 ;
/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

uint8_t InitTapeService(uint8_t Priority) {
    ES_Event ThisEvent;
    MyPriority = Priority;
    
    ES_Timer_Init() ;

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

    switch (ThisEvent.EventType) {
    case ES_INIT :
        break;
        
    case TAPE_SENSED :
        
        uint8_t curTape = (uint8_t)ThisEvent.EventParam ; // Typecast the sensor readings as a filthy byte
        LED_SetBank(LED_BANK1, (unsigned char)curTape) ;
        printf("sensors: 0x%x\n", curTape) ;
        
        if(state == s_FORWARD) {
            forwardTape(curTape) ;
        } else if(state == s_BACKWARD) {
            backwardTape(curTape) ;
		}
        break; // case TAPE_SENSED
        // store last 16 tapes and post BREAKOUT if there has been only 2 within a certain time
        lastTapes[tape_i] = curTape ; 
        tape_i++ ;
        tape_i&=0b1111; // possibly bad for power (minimal.)
        
        
        
    case BUMPER_HIT : 
        curBump = (uint8_t)ThisEvent.EventParam ; // Bytecast
        LED_SetBank(LED_BANK2, (unsigned char)curBump) ;
        printf("bumpers: 0x%x\n", curBump) ;
        
        if(curBump & 0b0011 && state == s_FORWARD) { // front bumpers, go into backward
            state = s_BACKWARD ;
            backwardTape(curTape) ;

        } else if(curBump & 0b1100 && state == s_BACKWARD) { // back bumpers, go back to forward
			state = s_FORWARD ;
            forwardTape(curTape) ;
		}

        break ; // case BUMPER_HIT
        
    case ES_TIMEOUT :
        break ; // case ES_TIMEOUT
    case TRACK_SENSED :
        printf("Track sensed\n") ;
        if(state == s_FORWARD || state == s_BACKWARD) {
            // state = s_SHOOTER ;
            // stopMotor(100) ;
            // revolutionTurn(LEFT) ;
            // set ES timer
        }
        break ; // case TRACK_SENSED
    /*case BANNER_SENSED :
        launch() ;
        state = s_ANNIHILATION
        break ;*/  
    case TRACK_LOST :
        printf("Track lost\n") ;
        // if(state == s_SHOOTER)
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
    case BEACON_SLIGHTLY_FARTHER :
        break ; // case BEACON_SLIGHTLY_FARTHER
            
    }

    return ReturnEvent;
}

/*******************************************************************************
 * PRIVATE FUNCTIONs                                                           *
 ******************************************************************************/


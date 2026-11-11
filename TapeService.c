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
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "TapeService.h"
#include "TapeEventChecker.h"
#include "Locomotion.h"
#include "LED.h"
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
/* You will need MyPriority and maybe a state variable; you may need others
 * as well. */

static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/


uint8_t InitTapeService(uint8_t Priority)
{
    ES_Event ThisEvent;

    MyPriority = Priority;

    // in here you write your initialization code
    // this includes all hardware and software initialization
    // that needs to occur.

    // post the initial transition event
    ThisEvent.EventType = ES_INIT;
    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}


uint8_t PostTapeService(ES_Event ThisEvent)
{
    return ES_PostToService(MyPriority, ThisEvent);
}


ES_Event RunTapeService(ES_Event ThisEvent)
{
    ES_Event ReturnEvent;
    ReturnEvent.EventType = ES_NO_EVENT; // assume no errors

    /********************************************
     in here you write your service code
     *******************************************/
    static ES_EventTyp_t lastEvent = TAPE_SENSED;
    ES_EventTyp_t curEvent;

    switch (ThisEvent.EventType) {
    case ES_INIT:
        LED_Init() ;
        LED_AddBanks(LED_BANK1 | LED_BANK2) ;
        break;
    case TAPE_SENSED :
        uint8_t sensorReadings = (uint8_t)ThisEvent.EventParam ; // Typecast the sensor readings as a filthy byte
        LED_SetBank(LED_BANK1, (unsigned char)sensorReadings) ;
        if(sensorReadings & 0x0008 || sensorReadings & 0x0007) { // Tape Sensor 4 or 1, 2, 3
            revolutionTurn(RIGHT);
            LED_SetBank(LED_BANK2, 0xFF) ;
        } else if (!(sensorReadings ^ 0x0001) || !(sensorReadings ^ 0x0005)) { // 0001 or 0101
            move(FORWARD) ;
            LED_SetBank(LED_BANK2, 0xA5) ;
        } else if (!(sensorReadings ^ 0x0002) || !(sensorReadings ^ 0x0006)) { // 0010 or 0110
            driveTurn(RIGHT) ;
            LED_SetBank(LED_BANK2, 0x0F) ;
        } else {
            driveTurn(LEFT) ;
            LED_SetBank(LED_BANK2, 0xF0) ;
        }
        break;
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


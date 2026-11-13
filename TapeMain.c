#include <BOARD.h>
#include <xc.h>
#include "AD.h"
#include "IO_Ports.h"
#include <stdio.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "Locomotion.h"
#include "pwm.h"
#include "timers.h"


void main(void)
{
    ES_Return_t ErrorType;

    BOARD_Init();
    motorInit();
    PWM_Init();
    AD_Init();
    TIMERS_Init();

    printf("Initializing KB BATTLEBOT ... \nPROTOCOL: Tape Drive\n");

    // Your hardware initialization function calls go here
    // PWM_SetFrequency(PWM_DEFAULT_FREQUENCY) ;
    
    // set the _TRIS registers to input for each tape sensor. 
    PORTY06_TRIS = 1 ;
    PORTY07_TRIS = 1 ;
    PORTY08_TRIS = 1 ;
    PORTY09_TRIS = 1 ;
    PORTY10_TRIS = 1 ;
    PORTY11_TRIS = 1 ;
    
    PORTZ07_TRIS = 0 ;
    PORTZ08_TRIS = 0 ;
    PORTX12_TRIS = 0 ;
    PORTX10_TRIS = 0 ;

    LED_Init() ;
    LED_AddBanks(LED_BANK1 | LED_BANK3) ;

    PWM_AddPins(PWM_PORTZ06) ;
    PWM_AddPins(PWM_PORTX11) ;
    
    //PWM_SetDutyCycle(PWM_PORTX11, 600) ; // this goes backwards on the left wheel
    //PORTX12_BIT = 1;
    //PORTX10_BIT = 0;
    //PWM_SetDutyCycle(PWM_PORTZ06, 600) ; // this goes backwards on the right wheel
    //PORTZ07_BIT = 1;
    //PORTZ08_BIT = 0;
    
    // motorBackward(1) ;
    // setMotorSpeed(1, 50) ;
    
    
    // now initialize the Events and Services Framework and start it running
    
    ErrorType = ES_Initialize();
    if (ErrorType == Success) {
        ErrorType = ES_Run();

    }
    //if we got to here, there was an error
    switch (ErrorType) {
    case FailedPointer:
        printf("Failed on NULL pointer");
        break;
    case FailedInit:
        printf("Failed Initialization");
        break;
    default:
        printf("Other Failure: %d", ErrorType);
        break;
    }
    
    for (;;)
        ;
};

/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/

#include <BOARD.h>
#include <xc.h>
#include "AD.h"
#include "IO_Ports.h"
#include <stdio.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "Locomotion.h"

void main(void)
{
    ES_Return_t ErrorType;

    BOARD_Init();
    motorInit();

    printf("Starting ES Framework Template\r\n");
    printf("using the 2nd Generation Events & Services Framework\r\n");


    // Your hardware initialization function calls go here
    AD_Init() ;
    AD_AddPins
            (AD_PORTV3  // sensor 1
            |AD_PORTV4  // sensor 2
            |AD_PORTV5  // sensor 3
            |AD_PORTV6  // sensor 4
            |AD_PORTV7  // sensor 5
            |AD_PORTV8);// sensor 6
    
    IO_PortsSetPortDirection(PORTY, 0xFFFF) ;
    IO_PortsSetPortInputs(PORTY, 0xFFFF) ;
    LED_Init() ;
    LED_AddBanks(LED_BANK1 | LED_BANK2) ;


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

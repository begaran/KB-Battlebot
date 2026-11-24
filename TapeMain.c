#include <BOARD.h>
#include <xc.h>
#include "AD.h"
#include "IO_Ports.h"
#include <stdio.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "Locomotion.h"
#include "Turret.h"
#include "pwm.h"
#include "timers.h"
#include "RC_Servo.h"
#include "Turret.h"

// PORTS THAT WORK: 
// X
// 11, 12
// Y
// 6, 7, 8, 9, 10, 11, 12
// Z
// 3, 4, 5, 6, 7, 8, 9, 11, 12
// V
// most of them probably
// W
// most of them probably

// comment out to write own test code 
//#define NOMINAL 
//#define TIME1

void main(void) {
    printf("KB BATTLEBOT Initiating...\n");

    // initialization
    BOARD_Init();
    motorInit(); // from Locomotion.c/h
    PWM_Init();
    AD_Init();
    TIMERS_Init();
    
    // set the _TRIS registers to input for each tape sensor. 
    PORTY06_TRIS = 1 ; // Tape sensor 1 output
    PORTY07_TRIS = 1 ; // Tape sensor 2 output
    PORTY08_TRIS = 1 ; // Tape sensor 3 output
    PORTY09_TRIS = 1 ; // Tape sensor 4 output
    PORTY10_TRIS = 1 ; // Tape sensor 5 output
    PORTY11_TRIS = 1 ; // Tape sensor 6 output
    
    // bumper sensors
    PORTZ09_TRIS = 1 ; // Left Front Bumper
    PORTZ03_TRIS = 1 ; // Right Front Bumper
    PORTZ11_TRIS = 1 ; // Left Back Bumper
    PORTZ12_TRIS = 1 ; // Right Back Bumper
    
    // motor outputs
    PORTZ07_TRIS = 0 ; // H-bridge IN1
    PORTZ08_TRIS = 0 ; // H-bridge IN2
    PORTY12_TRIS = 0 ; // H-bridge IN3
    PORTZ04_TRIS = 0 ; // H-bridge IN4
        
    // more hardware init
    LED_Init() ;
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3) ;

    PWM_AddPins(PWM_PORTZ06) ;  // H-bridge ENA
    PWM_AddPins(PWM_PORTX11) ;  // H-bridge ENB
    
    RC_Init() ;
    RC_AddPins(RC_PORTW07) ;    // Turret servo
    RC_AddPins(RC_PORTW08) ;    // Pitch servo
    RC_AddPins(RC_PORTV04) ;    // reloader servo
    
    AD_AddPins(AD_PORTW3) ;     // Track wire
    AD_AddPins(AD_PORTW4) ;     // Beacon detector
    AD_AddPins(AD_PORTW5) ;     // IR banner detector

    int i = 1000 ;
    while(1) {
        while(1) {

            RC_SetPulseTime(RC_PORTV04, i) ;
            i += 20 ;
            if(i >= 2000) break ;
            wait(50) ;
        }
        while(1) {

            RC_SetPulseTime(RC_PORTV04, i) ;
            i -= 20 ;
            if(i <= 1000) break ;
            wait(50) ;
        }
    }
    // pitch test code
    //while(1) {
    //        launch() ; 
    //}
        
    /* launcher test code
    while(1) {
        for(int i = 1; i < 40; i++) {
            RC_SetPulseTime(RC_PORTW07, i*50 + 500) ;
            printf("%d\n", i*50 + 500) ;
            TIMERS_InitTimer(0x02, 200) ;
            TIMERS_StartTimer(0x02) ;
            while(TIMERS_IsTimerExpired(0x02) == TIMER_NOT_ACTIVE) {
                ;
            }
        }
    }
     */
    
    // AD Read test code
    //while(1) {
    //    printf("%u\n", AD_ReadADPin(AD_PORTW3)) ;
    //}
    
    // for motors, Z6, 7, 8 is similar to X11, 12, 10
    
    /* MOTOR TEST CODE - TIMER  
    while(1) {
        revolutionTurn(RIGHT) ;
        TIMERS_InitTimer(0x02, 1000) ;
        TIMERS_StartTimer(0x02) ;
        while(TIMERS_IsTimerExpired(0x02) == TIMER_NOT_ACTIVE) {
            ;
        }
        revolutionTurn(LEFT) ;
    }
     */    
    
    /* PORT TEST CODE
    while(1) {
        printf("%x\n", PORTZ09_BIT) ;
    }
     */
    
    // MOTOR TEST CODE - DIRECTION
    //PWM_SetDutyCycle(PWM_PORTX11, 800) ; // this goes backwards on the left wheel
    //PORTY12_BIT = 1;
    //PORTZ04_BIT = 0;
    //PWM_SetDutyCycle(PWM_PORTZ06, 600) ; // this goes backwards on the right wheel
    //PORTZ07_BIT = 1;
    //PORTZ08_BIT = 0;
    //setMotorSpeed(LEFT, 700) ;
    // motorForward(LEFT) ;
    //setMotorSpeed(1, 50) ;    
    //setMotorSpeed(LEFT, 700) ;
    //motorBackward(LEFT) ;
    
    
    // nominal program
    

#ifdef NOMINAL 
    ES_Return_t ErrorType;
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
#endif
    
#ifdef TEST1
    while(1) {
        // motors
        setMotorSpeed(0, 600) ;
        setMotorSpeed(1, 600) ;
        motorForward(0) ;
        motorForward(1) ;

        wait(4000) ;

        motorBackward(0) ;
        motorBackward(1) ;
        wait(3000) ;

        // servos
        setLMin() ;
        setPMin() ;
        wait(1000) ;
        setLMax() ;
        setPMax() ;
    }
#endif
 
};
/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/

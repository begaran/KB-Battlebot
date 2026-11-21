#include "BOARD.h"
#include "xc.h"
#include "IO_Ports.h"
#include "AD.h"
#include <stdio.h>
#include "timers.h"
#include "Turret.h"
#include "RC_Servo.h"

#define PITCHLO 2100 
#define PITCHHI 2350

#define LUNCHLO 500
#define LUNCHHI 2600

void wait(unsigned int time) {
    TIMERS_InitTimer(0x02, time) ;
    TIMERS_StartTimer(0x02) ;
    while(TIMERS_IsTimerExpired(0x02) == TIMER_NOT_ACTIVE) {
        ;
    }
    return ;
}

void launch(void) {
    RC_SetPulseTime(RC_PORTW07, LUNCHLO) ;
    wait(500) ;
    RC_SetPulseTime(RC_PORTW07, LUNCHHI) ;
    wait(500) ;
    return ;
}

void seesaw(void) {
    RC_SetPulseTime(RC_PORTW07, PITCHLO) ;
    wait(500) ;
    RC_SetPulseTime(RC_PORTW07, PITCHHI) ;
    wait(500) ;
    return ;
}


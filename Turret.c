#include "BOARD.h"
#include "xc.h"
#include "IO_Ports.h"
#include "AD.h"
#include <stdio.h>
#include "timers.h"
#include "Turret.h"
#include "RC_Servo.h"

// HI and LO pulses defined int "Turret.h"

void wait(unsigned int time) {
    TIMERS_InitTimer(0x02, time) ;
    TIMERS_StartTimer(0x02) ;
    while(TIMERS_IsTimerExpired(0x02) == TIMER_NOT_ACTIVE) {
        ;
    }
    return ;
}

void launch(void) {
    setLMin() ;
    wait(1000) ;
    setLMax() ;
    wait(1000) ;
    return ;
}

void seesaw(void) {
    setPMin() ;
    wait(500) ;
    setPMax() ;
    wait(500) ;
    return ;
}

void setLaunch(unsigned short int pulse) {
    if(pulse > LUNCHHI) pulse = LUNCHHI ;
    if(pulse < LUNCHLO) pulse = LUNCHLO ;
    RC_SetPulseTime(RC_PORTW07, pulse) ;
    return ;
}

void setPitch(unsigned short int pulse) {
    if(pulse > PITCHHI) pulse = PITCHHI ;
    if(pulse < PITCHLO) pulse = PITCHLO ;
    RC_SetPulseTime(RC_PORTW08, pulse) ;
    return ;
}

void setLMax(void) {
    setLaunch(LUNCHHI) ;
    return ;
}

void setLMin(void) {
    setLaunch(LUNCHLO) ;
    return ;
}

void setPMax(void) {
    setPitch(PITCHHI) ;
    return ;
}

void setPMin(void) {
    setPitch(PITCHLO) ;
    return ;
}

void reload(void) {
    int i = 1000 ;
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
    return ;
}
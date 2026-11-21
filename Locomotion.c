/* Locomotion.c
 * Drives the H-bridge that drives the motors
 */

/* INCLUDES */
#include "BOARD.h"
#include "xc.h"
#include "IO_Ports.h"
#include "LED.h"
#include "pwm.h"
#include "AD.h"
#include "Locomotion.h"
#include <stdio.h>
#include "timers.h"

/* DEFINES */
#define NORM_SPEED 50
#define STOPTIME 20

// author: jdgrant 

// stops motor 
void stopMotor(unsigned int time) {
    setMotorSpeed(LEFT, 0);
    setMotorSpeed(RIGHT, 0);
    TIMERS_InitTimer(0x01, time) ;
    TIMERS_StartTimer(0x01) ;
    printf("started timer\n") ;
    while(TIMERS_IsTimerExpired(0x01) == TIMER_NOT_ACTIVE) {
        ;
    }
    printf("timer expired\n") ;
    return ;
}

// initiates pins used and a timer
void motorInit(void) {
   PORTZ07_LAT = 0 ;
   PORTZ08_LAT = 0 ;
   PORTX12_LAT = 0 ;
   PORTZ04_LAT = 0 ;
   
   PWM_AddPins(PWM_PORTZ06);
   PWM_AddPins(PWM_PORTX11);
   TIMERS_InitTimer(0x01, STOPTIME);
   
   return ;
}

// straight lateral movement
void move(int direction) {
   stopMotor(STOPTIME) ;
   printf("moving %d\n", direction) ;
   if(direction == FORWARD) {
    motorForward(LEFT);
    motorForward(RIGHT);
   } else {
    motorBackward(RIGHT);
	motorBackward(LEFT); 
   }
   setMotorSpeed(LEFT, NORM_SPEED);
   setMotorSpeed(RIGHT, NORM_SPEED);

   return ;
}

// tank turn, simple revolution
void revolutionTurn(int direction) {
   stopMotor(STOPTIME) ;
   printf("tank turn %d\n", direction) ;
   if(direction == LEFT) {
    motorForward(RIGHT);
    motorBackward(LEFT);
   } else {
    motorForward(LEFT);
	motorBackward(RIGHT);
   }
   setMotorSpeed(LEFT, NORM_SPEED);
   setMotorSpeed(RIGHT, NORM_SPEED);
   return ;
}

// drives forwards with a bias 
void driveTurn(int direction) {
    stopMotor(STOPTIME) ;
    printf("drive turn %d\n", direction) ;
    motorForward(LEFT) ;
    motorForward(RIGHT) ;
	setMotorSpeed(LEFT, direction == LEFT ? NORM_SPEED - (NORM_SPEED/4) : NORM_SPEED + (NORM_SPEED/4)) ;
	setMotorSpeed(RIGHT, direction == LEFT ? NORM_SPEED + (NORM_SPEED/4) : NORM_SPEED - (NORM_SPEED/4)) ;
   return ;
}

// drives backwards with a bias
void backTurn(int direction) {
    stopMotor(STOPTIME) ;
    printf("back turn %d\n", direction) ;
    motorBackward(LEFT) ;
    motorBackward(RIGHT) ;
    setMotorSpeed(LEFT, direction == RIGHT ? NORM_SPEED - (NORM_SPEED/4) : NORM_SPEED + (NORM_SPEED/4)) ;
	setMotorSpeed(RIGHT, direction == RIGHT ? NORM_SPEED + (NORM_SPEED/4) : NORM_SPEED - (NORM_SPEED/4)) ;
}

// changes directions of motors, does not change PWM (speed)
void motorForward(int lr) {
	if (lr == RIGHT) {
		PORTZ07_BIT = 0; 
        PORTZ08_BIT = 1;
	} else {   
        PORTX12_BIT = 0;
        PORTZ04_BIT = 1;
	}
	return;
}

// changes directions of motors, does not change PWM (speed)
void motorBackward(int lr) {
	if (lr == RIGHT) {
		PORTZ07_BIT = 1;
        PORTZ08_BIT = 0;
	} else {
        PORTX12_BIT = 1;
        PORTZ04_BIT = 0;
	}
	return;
}

// sets the PWM duty cycles (speed). Does not touch the directions of the motors
void setMotorSpeed(int lr, int speed) {
    // ripped from github
    uint16_t batVoltage = (AD_ReadADPin(BAT_VOLTAGE) * 33) / 1023 ; 
	if(speed > 50) {
        speed = 50 ;
    } else if(speed < 0) {
        speed = 0 ;
    }
    
    speed = speed * 10 ; 
    speed = speed * MAX_VOLTAGE/batVoltage ;
     
    if (lr == RIGHT) {
		PWM_SetDutyCycle(PWM_PORTZ06, speed) ;
	} else {
		PWM_SetDutyCycle(PWM_PORTX11, speed) ;
	}
	return ;
}
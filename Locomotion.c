#include "BOARD.h"
#include "xc.h"
#include "IO_Ports.h"
#include "LED.h"
#include "pwm.h"
#include "AD.h"
#include "Locomotion.h"
#include <stdio.h>
#include "timers.h"

#define PWM_LEFT_MOTOR   PWM_PORTZ06 // ENA 
#define PWM_RIGHT_MOTOR  PWM_PORTX11 // ENB
#define DIR_LEFT_MOTOR   PORTZ07_BIT // IN1
#define NDIR_LEFT_MOTOR  PORTZ08_BIT // IN2
#define DIR_RIGHT_MOTOR  PORTX12_BIT // IN3
#define NDIR_RIGHT_MOTOR PORTX10_BIT // IN4
#define RIGHT 1
#define LEFT 0
#define FORWARD 2
#define BACKWARD 3
#define NORM_SPEED 50
#define MAX_VOLTAGE 9.9


// author: jdgrant 
void stopMotor(void) {
    setMotorSpeed(LEFT, 0);
    setMotorSpeed(RIGHT, 0);
    TIMERS_StartTimer(0xFF) ;
    while(TIMERS_IsTimerExpired(0xFF) == TIMER_NOT_ACTIVE) {
        ;
    }
    TIMERS_StopTimer(0xFF) ;
    TIMERS_SetTimer(0xFF, 10) ;
    return ;
}

void motorInit(void) {
   PORTZ07_LAT = 0 ;
   PORTZ08_LAT = 0 ;
   PORTX12_LAT = 0 ;
   PORTX10_LAT = 0 ;
   
   PWM_AddPins(PWM_PORTZ06);
   PWM_AddPins(PWM_PORTX11);
   TIMERS_InitTimer(0xFF, 10);
   
   return ;
}

void move(int direction) {
   stopMotor(void) ;
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

void revolutionTurn(int direction) {
   stopMotor(void) ;
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

void driveTurn(int direction) {
    stopMotor(void) ;
    printf("drive turn %d\n", direction) ;
    motorForward(LEFT) ;
    motorForward(RIGHT) ;
	setMotorSpeed(LEFT, direction == LEFT ? NORM_SPEED - (NORM_SPEED/4) : NORM_SPEED + (NORM_SPEED/4)) ;
	setMotorSpeed(RIGHT, direction == LEFT ? NORM_SPEED + (NORM_SPEED/4) : NORM_SPEED - (NORM_SPEED/4)) ;
   return ;
}

void motorForward(int lr) {
	if (lr == RIGHT) {
		PORTZ07_BIT = 0;
        PORTZ08_BIT = 1;
	} else {   
        PORTX12_BIT = 0;
        PORTX10_BIT = 1;
	}
	return;
}

void motorBackward(int lr) {
	if (lr == RIGHT) {
		PORTZ07_BIT = 1;
        PORTZ08_BIT = 0;
	} else {
        PORTX12_BIT = 1;
        PORTX10_BIT = 0;
	}
	return;
}

void setMotorSpeed(int lr, int speed) {
    
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
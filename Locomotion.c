#include "BOARD.h"
#include "IO_Ports.h"
#include "LED.h"
#include "pwm.h"
#include "AD.h"
#include "Locomotion.h"
#include <stdio.h>

#define PWM_LEFT_MOTOR  PWM_PORTY10
#define PWM_RIGHT_MOTOR PWM_PORTY12
#define DIR_LEFT_MOTOR  PIN9 
#define DIR_RIGHT_MOTOR PIN11
#define RIGHT 1
#define LEFT 0
#define FORWARD 2
#define BACKWARD 3
#define NORM_SPEED 25
#define MAX_VOLTAGE 9.9

void motorInit(void) {
   IO_PortsSetPortOutputs(PORTY, DIR_LEFT_MOTOR|DIR_RIGHT_MOTOR);
   PWM_AddPins(PWM_LEFT_MOTOR|PWM_RIGHT_MOTOR);
   return ;
}

void move(int direction) {
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
    motorForward(LEFT) ;
    motorForward(RIGHT) ;
	setMotorSpeed(LEFT, direction == LEFT ? NORM_SPEED - (NORM_SPEED/2) : NORM_SPEED + (NORM_SPEED/2)) ;
	setMotorSpeed(RIGHT, direction == LEFT ? NORM_SPEED + (NORM_SPEED/2) : NORM_SPEED - (NORM_SPEED/2)) ;
   return ;
}

void motorForward(int lr) {
	if (lr == RIGHT) {
        IO_PortsWritePort(PORTY,IO_PortsReadPort(PORTY) | DIR_RIGHT_MOTOR) ; 
	} else {   
        IO_PortsWritePort(PORTY,IO_PortsReadPort(PORTY) & ~DIR_LEFT_MOTOR) ; 
	}
	return;
}

void motorBackward(int lr) {
	if (lr == RIGHT) {
		IO_PortsWritePort(PORTY,IO_PortsReadPort(PORTY) & ~DIR_RIGHT_MOTOR) ; 
	} else {
		IO_PortsWritePort(PORTY,IO_PortsReadPort(PORTY) | DIR_LEFT_MOTOR) ; 
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
		PWM_SetDutyCycle(PWM_RIGHT_MOTOR, speed) ;
	} else {
		PWM_SetDutyCycle(PWM_LEFT_MOTOR, speed) ;
	}
	return ;
}
/* 
 * File:   Locomotion.h
 * Author: baron
 *
 * Created on November 10, 2026, 4:51 PM
 */

#include "BOARD.h"
#include "IO_Ports.h"
#include "LED.h"
#include "pwm.h"
#include "AD.h"
#include <stdio.h>

#ifndef LOCOMOTION_H
#define	LOCOMOTION_H

#define PWM_LEFT_MOTOR   PWM_PORTZ06
#define PWM_RIGHT_MOTOR  PWM_PORTX11
#define DIR_LEFT_MOTOR   PORTZ07_BIT
#define NDIR_LEFT_MOTOR  PORTZ08_BIT
#define DIR_RIGHT_MOTOR  PORTX12_BIT
#define NDIR_RIGHT_MOTOR PORTX10_BIT
#define RIGHT 1
#define LEFT 0
#define FORWARD 2
#define BACKWARD 3
#define NORM_SPEED 50
#define MAX_VOLTAGE 9.9

#ifdef	__cplusplus
extern "C" {
#endif

    void motorInit() ;
    
    void move(int direction) ;
    
    void revolutionTurn(int direction) ;
    
    void driveTurn(int direction) ;
    
    void motorForward(int lr) ;
    
    void motorBackward(int lr) ;
    
    void setMotorSpeed(int lr, int speed) ;


#ifdef	__cplusplus
}
#endif

#endif	/* LOCOMOTION_H */


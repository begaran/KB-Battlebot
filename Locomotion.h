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

#define RIGHT 1
#define LEFT 0
#define FORWARD 2
#define BACKWARD 3
#define MAX_VOLTAGE 9.9

#ifdef	__cplusplus
extern "C" {
#endif

    void stopMotor(unsigned int time) ;
    
    void motorInit() ;
    
    void move(int direction) ;
    
    void revolutionTurn(int direction) ;
    
    void driveTurn(int direction) ;
    
    void backTurn(int direction) ;
    
    void motorForward(int lr) ;
    
    void motorBackward(int lr) ;
    
    void setMotorSpeed(int lr, int speed) ;

    void crank90(int lr) ;
    
    void rampSpeed(int lr, int speed) ;
#ifdef	__cplusplus
}
#endif

#endif	/* LOCOMOTION_H */


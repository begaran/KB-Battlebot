/* 
 * File:   TapeEventChecker.h
 * Author: Gabriel Hugh Elkaim
 */

#ifndef TAPEEVENTCHECKER_H
#define	TAPEEVENTCHECKER_H

/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "ES_Configure.h"   // defines ES_Event, INIT_EVENT, ENTRY_EVENT, and EXIT_EVENT
#include "BOARD.h"
#include "AD.h"
#include "xc.h"

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

/*
#define TAPESENSOR1 PORTY06_BIT
#define TAPESENSOR2 PORTY07_BIT
#define TAPESENSOR3 PORTY08_BIT
#define TAPESENSOR4 PORTY09_BIT
#define TAPESENSOR5 PORTY10_BIT
#define TAPESENSOR6 PORTY11_BIT
*/

/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/


// Returns an 8 bit vector. 
// Each bit corresponds to a single sensor, with there being 6 sensors total
// 1 means that sensor is on black tape, 0 means the sensor is off black tape
// the 2 most significant bits will be 0.
uint8_t TapeCheck(void);

#endif	/* TAPEEVENTCHECKER_H */


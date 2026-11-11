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

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

#define TAPESENSOR1 AD_PORTV3
#define TAPESENSOR2 AD_PORTV4
#define TAPESENSOR3 AD_PORTV5
#define TAPESENSOR4 AD_PORTV6
#define TAPESENSOR5 AD_PORTV7
#define TAPESENSOR6 AD_PORTV8

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


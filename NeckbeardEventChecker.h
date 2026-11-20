/* 
 * File:   NeckbeardEventChecker.h
 * Author: Gabriel Hugh Elkaim
 * Event checker for all KB Battlebot MPLABX projects
 */

#ifndef NECKBEARDEVENTCHECKER_H
#define	NECKBEARDEVENTCHECKER_H

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

/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

// Checks if the tape state has changed.
// Returns the state in EventParam as a uint16_t.
// Each bit corresponds to a single sensor, with there being 6 sensors total.
// 1 means that sensor is on black tape, 0 means the sensor is off black tape.
// The 10 most significant bits will be 0.
// will post TAPE_SENSED
uint8_t TapeCheck(void);

// Checks if the bumper state has changed.
// Returns the state in EventParam as a uint16_t.
// Each bit corresponds to a single bumper, with there being 4 bumpers total.
// 1 means that bumper is currently pressed, 0 means the bumper is unpressed.
// the 12 most significant bits will be 0.
// will post BUMPER_HIT
uint8_t BumperCheck(void);

// Checks if trackwire is within range.
// Returns data (voltage reading) thru EventParam
// will post TRACK_SENSED or TRACK_LOST
uint8_t TrackCheck(void);

// Checks if beacon is seen
// Returns data (voltage reading) thru EventParam
// will post BEACON_SENSED, BEACON_LOST, BEACON_CLOSE, or BEACON_SLIGHTLY_FARTHER
uint8_t BeaconCheck(void);

#endif	/* TAPEEVENTCHECKER_H */


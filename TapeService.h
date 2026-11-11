/*
 * File: TapeService.h
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
 */

#ifndef TapeService_H  
#define TapeService_H 


/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "ES_Configure.h"   // defines ES_Event, INIT_EVENT, ENTRY_EVENT, and EXIT_EVENT
#include "TapeEventChecker.h"
#include "ES_Events.h"

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

 
// Initialize 
uint8_t InitTapeService(uint8_t Priority);


// Post
uint8_t PostTapeService(ES_Event ThisEvent);


// Run
ES_Event RunTapeService(ES_Event ThisEvent);


#endif /* TapeService_H */


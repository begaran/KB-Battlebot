#ifndef TURRET_H
#define	TURRET_H

#ifdef	__cplusplus
extern "C" {
#endif

#define PITCHLO 2140 
#define PITCHHI 2350

#define LUNCHLO 500
#define LUNCHHI 2600
    
void wait(unsigned int time) ;

void launch(void) ;
    
void seesaw(void) ;
    
void setLaunch(unsigned short int pitch) ;

void setPitch(unsigned short int pitch) ;
    
void setLMax(void) ;

void setLMin(void) ;
    
void setPMax(void) ;
    
void setPMin(void) ;
    
#ifdef	__cplusplus
}
#endif

#endif	/* TURRET_H */


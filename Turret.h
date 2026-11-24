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


#ifdef	__cplusplus
}
#endif

#endif	/* TURRET_H */


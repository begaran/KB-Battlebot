

#include <xc.h>
#include <stdio.h>
#include <BOARD.h>
#include <RC_Servo.h>
#include <timers.h>

#define LOW_PULSE 500             
#define HIGH_PULSE 2500          

#define STEP_US 10                
#define STEP_PERIOD_MS 50        
#define HOLD_MS 1000             

#define SERVO_PIN RC_PORTY06     
#define MIDDLE_PULSE 1500

#if (LOW_PULSE < MINPULSE) || (HIGH_PULSE > MAXPULSE)

#endif

int main(void)
{
    int pulse = MIDDLE_PULSE;
    int step = STEP_US;              
    unsigned int wait = STEP_PERIOD_MS;
    unsigned int lastStep = 0;
    unsigned int now;

    BOARD_Init();
    TIMERS_Init();
    RC_Init();
    RC_AddPins(SERVO_PIN);          

    printf("\r\nServo test: slow sweep between %d and %d us\r\n", LOW_PULSE, HIGH_PULSE);

    while (1) {
        now = TIMERS_GetTime();

        if ((now - lastStep) >= wait) {
            lastStep = now;
            wait = STEP_PERIOD_MS;

            pulse += step;
            if (pulse >= HIGH_PULSE) {         
                pulse = HIGH_PULSE;
                step = -STEP_US;
                wait = HOLD_MS;
            } else if (pulse <= LOW_PULSE) {   
                pulse = LOW_PULSE;
                step = STEP_US;
                wait = HOLD_MS;
            }
            RC_SetPulseTime(SERVO_PIN, (unsigned short int) pulse);

            if ((pulse % 100) == 0) {
                printf("%d us\r\n", pulse);
            }
        }
    }
    return 0;
}

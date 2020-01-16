#include <msp.h>
#include "pwm.h"


#define SYSTEM_CLOCK        12000000/4  // [Hz] system_msp432p401r.c default 3Mhz. See config_clock()
#define PWM_FREQUENCY       100000   // [Hz] PWM frequency desired
#define CALC_PERIOD(X)      (SYSTEM_CLOCK / X) //calc # of ticks in period
#define PERIOD              CALC_PERIOD(PWM_FREQUENCY)

float sin_values[167]={50,51.904,53.804,55.7,57.587,59.463,61.325,63.171,64.997,66.802,68.583,70.337,72.061,73.753,75.411,77.032,78.614,80.154,81.651,83.101,84.504,85.857,87.157,88.404,89.595,90.729,91.803,92.817,93.769,94.658,95.482,96.239,96.93,97.553,98.107,98.591,99.004,99.347,99.618,99.817,99.943,99.998,99.98,99.889,99.726,99.491,99.184,98.806,98.357,97.838,97.25,96.593,95.869,95.078,94.222,93.301,92.318,91.274,90.169,89.007,87.787,86.514,85.187,83.809,82.382,80.908,79.389,77.828,76.226,74.587,72.911,71.203,69.464,67.696,65.903,64.087,62.25,60.396,58.526,56.644,54.753,52.854,50.952,49.048,47.146,45.247,43.356,41.474,39.604,37.75,35.913,34.097,32.304,30.536,28.797,27.089,25.413,23.774,22.172,20.611,19.092,17.618,16.191,14.813,13.486,12.213,10.993,9.8308,8.7264,7.6819,6.6987,5.7784,4.9221,4.1312,3.4068,2.75,2.1616,1.6427,1.1938,0.81571,0.50893,0.27391,0.11098,0.02039,0.0022657,0.056633,0.18341,0.38242,0.65337,0.99587,1.4094,1.8934,2.4472,3.0699,3.7606,4.5184,5.3421,6.2306,7.1825,8.1966,9.2712,10.405,11.596,12.843,14.143,15.496,16.899,18.349,19.846,21.386,22.968,24.589,26.247,27.939,29.663,31.417,33.198,35.003,36.829,38.675,40.537,42.413,44.3,46.196,48.096,50};
int sin_index;

void config_sin_vars(void){
    sin_index = 0;
}

void config_pwm_timer(void){
    TIMER_A0->CTL |= TIMER_A_CTL_CLR; // clear
    TIMER_A0->CTL |= TIMER_A_CTL_SSEL__SMCLK; // smclk source

    TIMER_A0->CTL |= TIMER_A_CTL_ID__4; // divide clock by 4 (set bit 7 to 1)

    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_7; // reset set mode
    TIMER_A0->CCR[0] = PERIOD; // set PWM period

    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE; // enable TA0 CCR0 interrupt request
    NVIC_EnableIRQ(TA0_0_IRQn); // Enables TA0 CCR0 in NVIC
    __NVIC_SetPriority(TA0_0_IRQn, 2); //Sets the priority of interrupt 8 to 2
}

//* * @param uint8_t duty_cycle: 0-100, percentage of time ON */
void set_duty_cycle(uint8_t duty_cycle){
    TIMER_A0->CCR[1] = PERIOD * duty_cycle / 100; // duty cycle
}

void start_pwm(void){
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP; // UP mode
}

/* Stop Mode: clear all Mode Control bits, MC, in TAxCTL register */
void stop_pwm(void){
    TIMER_A0->CTL &= ~(TIMER_A_CTL_MC_MASK); // stop mode
}

/* Config P2.4 to output TA0.1 waveform */
 void config_pwm_gpio(void){
     P2->OUT &= ~BIT4;
     //we want bit #4 to be 0, 1 in SEL1 and SEL0 respectively for primary module function
     P2->SEL1 &= ~BIT4;
     P2->SEL0 |= BIT4;
     P2->DIR |= BIT4;
 }

 void TA0_0_IRQHandler (void){
     TIMER_A0->CCTL[0] &= ~TIMER_A_CTL_IFG; //clear flag pending
     //P2OUT ^= 0b100;
     set_duty_cycle(sin_values[sin_index]);
     sin_index = (sin_index+1)%167;
 }




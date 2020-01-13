#include <msp.h>
#include "pwm.h"


#define SYSTEM_CLOCK        3000000  // [Hz] default system_msp432p401r.c
#define PWM_FREQUENCY       100000   // [Hz] PWM frequency desired
#define CALC_PERIOD(X)      (SYSTEM_CLOCK / X) //calc # of ticks in period
#define PERIOD              CALC_PERIOD(PWM_FREQUENCY)

void config_pwm_timer(void){
    TIMER_A0->CTL |= TIMER_A_CTL_CLR; // clear
    TIMER_A0->CTL |= TIMER_A_CTL_SSEL__SMCLK; // smclk source

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
     P2OUT ^= 0b100;

 }




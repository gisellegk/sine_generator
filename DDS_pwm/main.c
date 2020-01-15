#include "msp.h"
#include "pwm.h"


/**
 * main.c
 */


void config_clock(void){
    // set DCO to 12 MHz
    // in register CSCTL0:
    // DCORSEL - resistor select(?) write 011b nominal freq = 12MH
    CS->KEY  = 0x695A; //unlock clock
    CS->CTL0|= CS_CTL0_DCORSEL_3;
    CS->KEY  = 0; //lock clock
}

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// enable LEDs for debugging irq
	P2->DIR |= (BIT2 | BIT1 | BIT0);

	config_clock();
	config_pwm_timer();
	set_duty_cycle(50);
	config_pwm_gpio();
	start_pwm();

	while(1){

	}
}

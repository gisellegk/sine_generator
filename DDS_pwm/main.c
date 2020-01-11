#include "msp.h"
#include "pwm.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// enable LEDs for debugging irq
	P2->DIR |= (BIT2 | BIT1 | BIT0);

	config_pwm_timer();
	set_duty_cycle(50);
	config_pwm_gpio();
	start_pwm();

	while(1){

	}
}

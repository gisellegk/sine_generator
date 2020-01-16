#ifndef PWM_H_
#define PWM_H_

/* Configure TIMER_A0 to produce PWM waveform */
void config_pwm_timer(void);

/* @param uint8_t duty_cycle: 0-100, percentage of time ON */
void set_duty_cycle(uint8_t duty_cycle);

/* UP mode */
void start_pwm(void);

/* Stop Mode: clear all Mode Control bits, MC, in TAxCTL register */
void stop_pwm(void);

/* Config P2.4 to output TA0.1 waveform */
void config_pwm_gpio(void);

/* TA_0 IRQ handler */
void TA0_0_IRQHandler (void);

void config_sin_vars();

#endif

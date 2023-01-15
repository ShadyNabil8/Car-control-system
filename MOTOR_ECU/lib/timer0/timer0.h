/*
    Auther  : shady nabil
    Created : 8/25/2022
    version : 1.0
*/
#ifndef _TIMER0_H_
#define _TIMER0_H_
#include<avr/io.h>

extern void (*timer0_OVF_ISR)  (void);
extern void (*timer0_CTC_ISR)  (void);
extern void (*timer0_start)    (void);

typedef enum{
    timer0_overflow,
    timer0_compare,
    timer0_fast_pwm,
}timer0_mode;
typedef enum{
    timer0_pre_none,
    timer0_pre_1,
    timer0_pre_8,
    timer0_pre_64,
    timer0_pre_256,
    timer0_pre_1024,
    timer0_pre_ext_rising_edge,
    timer0_pre_ext_falling_edge,
}timer0_prescaller;

typedef struct{
    timer0_mode mode;
    timer0_prescaller prescaller;
    uint8_t TCNT0_value;
    uint8_t OCR0_value;
    /*  OCR0 in CTC mode rebresents the combared value
    *   but in PWM value rebresents the duty 
    */

}timer0_params;

void timer0_init(const timer0_params * params_ptr);
void timer0_change_duty(uint8_t duty);

#endif
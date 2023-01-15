/*
    auther  : shady nabil
    created : 8/22/2022
    version : 1.0
*/


#ifndef _TIMER1_H_
#define _TIMER1_H_
#include<avr/io.h>

 extern volatile uint32_t globelCounter;
 extern volatile uint16_t ctc_mode_enable;
 

 /* pointer to function */
 extern void (*timer1_start)    (void);
 extern void (*timer1_OVF_ISR)  (void);
 extern void (*timer1_CTC_A_ISR)(void);
 extern void (*timer1_CTC_B_ISR)(void);


typedef enum{
    timer1_overflow,
    timer1_compare,
    timer1_fast_pwm,
}timer1_mode;

typedef enum{
    timer1_pre_none,
    timer1_pre_1 = 1,
    timer1_pre_8 = 8,
    timer1_pre_64 = 64,
    timer1_pre_256 = 256,
    timer1_pre_1024 = 1024,
    timer1_pre_ext_rising_edge, /* to use as counter */
    timer1_pre_ext_falling_edge,/* to use as counter */
}timer1_prescaller;
typedef enum{
    channel_A,
    channel_B,
}channel_A_B;
typedef struct{
    timer1_mode mode;
    timer1_prescaller prescaller;
    channel_A_B channel;
    uint16_t TCNT1_value;
    uint16_t OCR1_value;
    uint16_t ICR1_value;
    /*  OCR1 in CTC mode rebresents the combared value
    *   but in PWM value rebresents the duty 
    */
}timer1_params;

void timer1_init(const timer1_params *params_ptr);
void timer1_change_duty(uint8_t duty , channel_A_B channel);
void timer1_stop(void);
void timer1_start_again(timer1_prescaller prescaller);

#endif
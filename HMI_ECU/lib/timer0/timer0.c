#include"timer0.h"
#include<avr/interrupt.h>

void (*timer0_start)  (void);
void (*timer0_OVF_ISR)(void);
void (*timer0_CTC_ISR)(void);
static void timer0_overflow_function(void);
static void timer0_compare_function (void);
static void timer0_fast_pwm_function(void);
static void set_prescaller(timer0_prescaller pre);

void timer0_init(const timer0_params * params_ptr){

    /* set the prescaller */
    set_prescaller(params_ptr->prescaller);

/****************************************************
                    OVERFLOW MODE 
****************************************************/
    if(params_ptr->mode == timer0_overflow){
        TCNT0 = params_ptr->TCNT0_value;

        /*  none PWM mode
            FOC0 = 1
        */
        TCCR0 |= 1<<FOC0;

        /*  Normal port operation, OC0 disconnected
                COM00 = 0 , COM01 = 0
        */
       TCCR0 &= ~((1<<COM00) | (1<<COM01));

       /* enable timer0 interrupt (overflow mode) */
       TIMSK |= 1<<TOIE0;

       timer0_start = &timer0_overflow_function;
    }
/****************************************************/

/****************************************************
                    CTC MODE 
****************************************************/
    else if(params_ptr->mode == timer0_compare){
        TCNT0 = params_ptr->TCNT0_value;

        /*  none PWM mode
            FOC0 = 1
        */
        TCCR0 |= 1<<FOC0;

        /*  Normal port operation, OC0 disconnected
                COM00 = 0 , COM01 = 0
        */
        TCCR0 &= ~((1<<COM00) | (1<<COM01));

       /* set the value to be combared */
        OCR0 = params_ptr->OCR0_value;

       /* enable timer0 interrupt (CTC mode) */
        TIMSK |= 1<<OCIE0;
       
        timer0_start = &timer0_compare_function;
        
    }
/****************************************************/

/****************************************************
                   PWM MODE 
****************************************************/
    else if(params_ptr->mode == timer0_fast_pwm){

        /*
         *         F_CPU
         * Fpwm = --------
         *         N*256
         */
        /* set OC0 as O/P */
        DDRB |= 1<<PIN3;

        TCNT0 = params_ptr->TCNT0_value;

        /*  PWM mode
            FOC0 = 0
        */
        TCCR0 &= ~(1<<FOC0);

        /*  Clear OC0 on compare match, set OC0 at TOP (non inverted)
            COM00 = 0 , COM01 = 1
        */
        TCCR0 &= ~(1<<COM00);
        TCCR0 |= (1<<COM01);

        /* set duty */
        OCR0 = ((params_ptr->OCR0_value)/100.0)*255;

        timer0_start = &timer0_fast_pwm_function;
    }
/****************************************************/

    /* enable globel interrupt */
    SREG |= 1<<SREG_I;

}

/****************************************************
                START THE SELECTED MODE
****************************************************/
static void timer0_overflow_function(void){

    /*  select waveform generation mode
            WGM00 = 0 , WGM01 = 0 
    */
    TCCR0 &= ~((1<<WGM00) | (1<<WGM01));

}
static void timer0_compare_function(void){
    /* WGM01 = 1 , WGM00 = 0 */
    TCCR0 |= 1<<WGM01;
    TCCR0 &= ~(1<<WGM00);
}
static void timer0_fast_pwm_function(void){
    /*  select fastPWM
        WGM00 = 1 , WGM01 = 1 */
    
    TCCR0 |= ((1<<WGM00) | (1<<WGM01));
}

/****************************************************
                    PRESCALLER
****************************************************/
static void set_prescaller(timer0_prescaller pre){
    TCCR0 &= 0xF8;
    if(pre == timer0_pre_none){TCCR0 &= ((1<<CS00) | (1<<CS01) | (1<<CS02));}
    else if(pre == timer0_pre_1){TCCR0    |= (1<<CS00);}
    else if(pre == timer0_pre_8){TCCR0    |= (1<<CS01);}
    else if(pre == timer0_pre_64){TCCR0   |= ((1<<CS01) | (1<<CS00));}
    else if(pre == timer0_pre_256){TCCR0  |= (1<<CS02);}
    else if(pre == timer0_pre_1024){TCCR0 |= ((1<<CS00) | (1<<CS02));}
    else if(pre == timer0_pre_ext_falling_edge){TCCR0 |= ((1<<CS01) | (1<<CS02));}
    else if(pre == timer0_pre_ext_rising_edge){TCCR0 |= ((1<<CS00) | (1<<CS01) | (1<<CS02));}

}

/****************************************************
                        ISRs
****************************************************/
ISR(TIMER0_OVF_vect){
    timer0_OVF_ISR();
}
ISR(TIMER0_COMP_vect){
    timer0_CTC_ISR();
}

/****************************************************
                 CHANGE DUTY FUNCTION
****************************************************/

void timer0_change_duty(uint8_t duty)
{
    /* set duty */
        OCR0 = ((duty)/100.0)*255;
}
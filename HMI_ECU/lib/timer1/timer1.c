#include"timer1.h"
#include<avr/interrupt.h>
#include<stdio.h>
#include<stdlib.h>

volatile uint32_t globelCounter;
volatile uint16_t ctc_mode_enable = 0;


void (*timer1_start)    (void);
void (*timer1_OVF_ISR)  (void);
void (*timer1_CTC_A_ISR)(void);
void (*timer1_CTC_B_ISR)(void);

static void timer1_overflow_function(void);
static void timer1_compare_function (void);
static void timer1_fast_pwm_function(void);
static void set_prescaller(timer1_prescaller pre);

void timer1_init(const timer1_params *params_ptr){
   
    /*set TCNT1 value */
    TCNT1 = params_ptr->TCNT1_value;

    /*
     * 1st choose the mode
     * overFlow or CTC(channel A or B)
     */

    /* 
     * select the prescaller
     */
    set_prescaller(params_ptr->prescaller);

/****************************************************
                    OVERFLOW MODE 
****************************************************/

    if(params_ptr->mode == timer1_overflow)
    {
        TIMSK |= 1<<TOIE1;
        timer1_start = &timer1_overflow_function;
    }
/****************************************************/
    
/****************************************************
                    CTC MODE 
****************************************************/

    else if(params_ptr->mode == timer1_compare){

        TCNT1 = params_ptr->TCNT1_value;

        if(params_ptr->channel == channel_A){
            
            TIMSK |= 1<<OCIE1A;

            /* none pwm mode */
            TCCR1A |= 1<<FOC1A;

            /* set OCR1A value the value that will be compared)*/
            OCR1A = params_ptr->OCR1_value;
            
        }
        else if(params_ptr->channel == channel_B){

            TIMSK |= 1<<OCIE1B;
            
            /* none pwm mode */
            TCCR1A |= 1<<FOC1B;

            /* set OCR1A value (the value that will be compared) */
            OCR1A = params_ptr->OCR1_value;
        }
        
        timer1_start = &timer1_compare_function;
    }
/****************************************************/

/****************************************************
                   PWM MODE 
****************************************************/

    else if(params_ptr->mode == timer1_fast_pwm){
        /* this the value the controls the frequency of thw PWM */
        ICR1 = ((F_CPU/(params_ptr->ICR1_value))/(params_ptr->prescaller)-1);

        if(params_ptr->channel == channel_A){
            /* configure pin PD5 as O/P */
            DDRD |= 0b00100000;

            /* 
             *COM1A0 = 0 , COM1A1 = 1 
             *Clear OC1A on compare match, set OC1A at TOP (non_inverted)
             */
            TCCR1A &= ~(1<<COM1A0) ; TCCR1A |= (1<<COM1A1);

            /* set duty */
            OCR1A = ((params_ptr->OCR1_value)/100.0)*ICR1;
        }
        else if(params_ptr->channel == channel_B){
            /* configure pin PD4 as O/P */
            DDRD |= 0b00010000;

            /* 
             * COM1B0 = 0 , COM1B1 = 1 
             * Clear OC1B on compare match, set OC1B at TOP
             */
            TCCR1A &= ~(1<<COM1B0) ; TCCR1A |= (1<<COM1B1);

            /* set duty */
            OCR1B = ((params_ptr->OCR1_value)/100.0)*ICR1;
        }
        timer1_start = &timer1_fast_pwm_function;    
    }

/****************************************************/

    /* enable global interrups */
    SREG |= (1<<7);    
}

/****************************************************
                START THE SELECTED MODE
****************************************************/
static void timer1_overflow_function(void){

    /* select overflow mode
     * WGM10 = 0 , WGM11 = 0 , WGM12 = 0 , WGM13 = 0
     */
    TCCR1A &= 0xFC;
    TCCR1B &= 0xE7; /* i made a change here */
    TCCR1A &= ~((1<<WGM10) | (1<<WGM11));
    TCCR1B &= ~((1<<WGM12) | (1<<WGM13));

    /* overflow Interrupt Enable */
    TIMSK |= (1<<TOIE1);
    TIMSK &= ~((1<<OCIE1A) | (1<<OCIE1B));
}


static void timer1_compare_function(void){
    /* select compare mode
     * WGM10 = 0 , WGM11 = 0 , WGM12 = 1 , WGM13 = 0  
     */
      if(!ctc_mode_enable){
        TCCR1A &= ~((1<<WGM10) | (1<<WGM11));
        TCCR1B &= ~(1<<WGM13) ; TCCR1B |= (1<<WGM12);
      }
      ctc_mode_enable = 1;
}

static void timer1_fast_pwm_function(void){
    /*
     * select fast pwm
     * WGM10 = 0 , WGM11 = 1 , WGM12 = 1 , WGM13 = 1 
     */
    TCCR1A &= ~(1<<WGM10) ; TCCR1A |= (1<<WGM11);
    TCCR1B |= ((1<<WGM12) | (1<<WGM13));

}

/****************************************************
                    PRESCALLER
****************************************************/
static void set_prescaller(timer1_prescaller pre){
    TCCR1B &= 0xF8;
    if(pre == timer1_pre_none){TCCR1B &= 0xF8;}
    else if(pre == timer1_pre_1){TCCR1B    |= (1<<CS10);}
    else if(pre == timer1_pre_8){TCCR1B    |= (1<<CS11);}
    else if(pre == timer1_pre_64){TCCR1B   |= ((1<<CS10) | (1<<CS11));}
    else if(pre == timer1_pre_256){TCCR1B  |= (1<<CS12);}
    else if(pre == timer1_pre_1024){TCCR1B |= ((1<<CS10) | (1<<CS12));}
    else if(pre == timer1_pre_ext_falling_edge){TCCR1B |= ((1<<CS11) | (1<<CS12));}
    else if(pre == timer1_pre_ext_rising_edge){TCCR1B |= ((1<<CS10) | (1<<CS11) | (1<<CS12));}

}

/****************************************************
                        ISRs
****************************************************/

ISR(TIMER1_OVF_vect){
    timer1_OVF_ISR();  
}
ISR(TIMER1_COMPA_vect){
    timer1_CTC_A_ISR(); 
}
ISR(TIMER1_COMPB_vect){
    timer1_CTC_B_ISR(); 
}

/****************************************************
                 CHANGE DUTY FUNCTION
****************************************************/

void timer1_change_duty(uint8_t duty , channel_A_B channel)
{
    switch (channel)
    {
    case channel_A:
        OCR1A = ((duty)/100.0)*ICR1;
        break;
    
    case channel_B:
        OCR1B = ((duty)/100.0)*ICR1;
        break;
    }

}
/****************************************************
                   OTHER FUNCTION
****************************************************/
void timer1_stop(void)
{
    set_prescaller(timer1_pre_none);
}
void timer1_start_again(timer1_prescaller prescaller)
{
    set_prescaller(prescaller);
}

#ifndef _TIMER_H
#define _TIMER_H


#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#include "psc.h"
#include "trash.h"

typedef enum timer_type_t
{
    TMR0,   
    TMR1,
    TMR2
} timer_type_t;
typedef enum timer_mode_t
{
    TMR_DEF = 0,
    TMR_INT,
} timer_mode_t;

typedef struct timer_t
{
    volatile uint8_t *control_reg;
    volatile void *count_reg;
    uint8_t psc_bit;
    uint8_t intr_bit;
    uint8_t ovf_bit;
    uint32_t max;
} timer_t;

// TODO implement CTC

/*
typedef enum
{
    TMR_CTC0=1,
    TMR_CTC1=3
} ctc_mode_t;

typedef enum {
    // ICR1
    // OCR1A
    // OCR1B
}ctc_channel_t;
*/

#define timer_isr(x) ISR(x##_OVF_vect)
#define TMR0_OVF_vect TIMER0_OVF_vect
#define TMR1_OVF_vect TIMER1_OVF_vect
#define TMR2_OVF_vect TIMER2_OVF_vect

// make sure that 0 < (period)seconds*(F_CPU)MHz/scale < 2^(WIDTH)
timer_t timer_get(timer_type_t timer);
void timer_init(timer_type_t timer, psc_scale scale, float period, timer_mode_t mode);
void timer_set_count(timer_t tmr, uint32_t value);
//#define timer_init_us(timer, scale, period, mode) timer_init(timer, scale, period, mode)
//#define timer_init_ms(timer, scale, period, mode) timer_init(timer, scale, period * 1000, mode)
uint8_t timer_check_int(timer_type_t timer);
void timer_clear_int(timer_type_t timer);
void timer_off(timer_type_t timer);
uint8_t timer_psc(psc_scale scale, timer_type_t type);
// uint8_t timer_select_bit(timer_t timer, uint8_t MSK);

#endif
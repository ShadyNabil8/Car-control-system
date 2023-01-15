#include "timer.h"

void timer_init(timer_type_t timer, psc_scale scale, float period, timer_mode_t mode)
{
    uint8_t prescaler = timer_psc(scale, timer);
    timer_t tmr = timer_get(timer);
    // volatile uint8_t *tmr_register = (volatile uint8_t *)((timer==TMR1)? &TCCR1B : ((timer==TMR0)? &TCCR0 : &TCCR2));

    _assign_pattern(*(tmr.control_reg), tmr.psc_bit, psc_mask, prescaler);

    timer_set_count(tmr, tmr.max - (uint32_t)round((float)F_CPU / (float)scale * period));

    if (mode == TMR_INT)
    {
        _set_bit(TIMSK, tmr.intr_bit); // enable timer1 overflow interrupt
        sei();
    }
}

void timer_set_count(timer_t tmr, uint32_t value) // can only be used privately
{
    switch (tmr.max)
    {
    case UINT8_MAX:
        *(uint8_t *)(tmr.count_reg) = value;
        break;

    case UINT16_MAX:
        *(uint16_t *)(tmr.count_reg) = value;
        break;

    case UINT32_MAX:
        *(uint32_t *)(tmr.count_reg) = value;
        break;
    }
}

uint8_t timer_check_int(timer_type_t timer)
{
    // uint8_t bit = ;
    timer_t tmr = timer_get(timer);
    return _get_bit(TIFR, tmr.ovf_bit);
}

void timer_clear_int(timer_type_t timer)
{
    // uint8_t bit = timer_select_bit(timer, 0);
    timer_t tmr = timer_get(timer);
    _set_bit(TIFR, tmr.ovf_bit);
}

void timer_off(timer_type_t timer)
{
    timer_t tmr = timer_get(timer);
    timer_set_count(tmr, 0);
    *(tmr.control_reg) &= ~0b111;
    _clear_bit(TIFR, tmr.ovf_bit);
    _clear_bit(TIMSK, tmr.intr_bit);
}

uint8_t timer_psc(psc_scale scale, timer_type_t type)
{
    return select_psc(scale, type == TMR2 ? PSC1 : PSC0);
}

timer_t timer_get(timer_type_t timer)
{
    switch (timer)
    {
    case TMR0:
    {
        const timer_t tmr = {
            .control_reg = &TCCR0,
            .count_reg = &TCNT0,
            .intr_bit = TOIE0,
            .ovf_bit = TOV0,
            .psc_bit = CS00,
            .max = UINT8_MAX};
        return tmr;
        break;
    }
    case TMR1:
    {
        const timer_t tmr = {
            .control_reg = &TCCR1B,
            .count_reg = &TCNT1,
            .intr_bit = TOIE1,
            .ovf_bit = TOV1,
            .psc_bit = CS10,
            .max = UINT16_MAX};
        return tmr;
        break;
    }
    case TMR2:
    {
        const timer_t tmr = {
            .control_reg = &TCCR2,
            //.count_reg = (volatile uint8_t *)&TCNT2,
            .count_reg = &TCNT2,
            .intr_bit = TOIE2,
            .ovf_bit = TOV2,
            .psc_bit = CS20,
            .max = UINT8_MAX};
        return tmr;
        break;
    }
    default:
        break;
    }
}
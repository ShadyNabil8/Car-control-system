#include "xint.h"

void xint_attach(xint_type_t interrupt, xint_trigger_t trigger)
{
    xint_t xint = xint_get(interrupt, trigger);
    _assign_bit(*(xint.reg), xint.trigger_bit, xint.trigger);
    _set_bit(GICR, xint.enable_bit);
    dio_set_direction(xint.pin, DIO_DIRECTION_INPUT);
    /*
        switch (interrupt)
        {
        case XINT2:
        {
            _assign_bit(MCUCSR, interrupt, (trigger & ~0b10));
            _set_bit(GICR, INT2);
            dio_set_direction(XINT2_PORT, INT2, DIO_DIRECTION_INPUT);
            break;
        }
        case XINT1:
        {
            _assign_bit(MCUCR, interrupt, trigger);
            _set_bit(GICR, INT1);
            dio_set_direction(XINT1_PORT, INT1, DIO_DIRECTION_INPUT);
            break;
        }
        case XINT0:
        {
            _assign_bit(MCUCR, interrupt, trigger);
            _set_bit(GICR, INT0);
            dio_set_direction(XINT0_PORT, INT0, DIO_DIRECTION_INPUT);
            break;
        }
        }
    //*/
    sei();
}

void xint_detach(xint_type_t interrupt)
{
    xint_t xint = xint_get(interrupt, 0);

    *(xint.reg) &= ~xint.mask;
    _clear_bit(*(xint.reg), xint.trigger_bit + 1);
    _clear_bit(GICR, xint.trigger_bit);
}

xint_t xint_get(xint_type_t interrupt, xint_trigger_t trigger)
{
    switch (interrupt)
    {
    case XINT2:
    {
        const xint_t xint = {
            .trigger_bit = interrupt,
            .enable_bit = INT2,
            .pin = XINT2_PIN,
            .reg = &MCUCSR,
            .mask = 0b01,
            .trigger = trigger & xint.mask};
        return xint;
        break;
    }
    case XINT1:
    {
        const xint_t xint = {
            .trigger_bit = interrupt,
            .enable_bit = INT1,
            .pin = XINT1_PIN,
            .reg = &MCUCR,
            .mask = 0b11,
            .trigger = trigger & xint.mask};
        return xint;
        break;
    }
    case XINT0:
    {
        const xint_t xint = {

            .trigger_bit = interrupt,
            .enable_bit = INT0,
            .pin = XINT0_PIN,
            .reg = &MCUCR,
            .mask = 0b11,
            .trigger = trigger & xint.mask};
        return xint;
        break;
    }
    default:
        break;
    }
}
#include "INTx.h"

void (*ISR_INT1)(void);
void (*ISR_INT0)(void);

void INT1_init(uint8_t mode)
{
    sei(); /*ENABLE GLOBEL INT*/
    /*_set_(SREG, SREG_I);*/ /*ENABLE GLOBEL INT*/
    _set_(GICR, INT1);   /*ENABLE INT1*/
    switch (mode)
    {
    case RISING:
        _clr_(DDRD, PIN3);
        _clr_(PORTD, PIN3); /*HI _Z */
        _set_(MCUCR, ISC11);
        _set_(MCUCR, ISC10);
        break;

    case FALLING:
        _clr_(DDRD, PIN3);
        _set_(PORTD, PIN3); /*PULL UP*/
        _set_(MCUCR, ISC11);
        _clr_(MCUCR, ISC10);
        break;

    case LOGICAL_CHANGE:
        _clr_(DDRD, PIN3);
        _clr_(PORTD, PIN3); /*HI _Z */
        _clr_(MCUCR, ISC11);
        _set_(MCUCR, ISC10);
        break;
    }
}

void INT0_init(uint8_t mode)
{
    sei(); /*ENABLE GLOBEL INT*/
    /*_set_(SREG, I_BIT);*/   /*ENABLE GLOBEL INT*/
    _set_(GICR, INT0); /*ENABLE INT0*/
    switch (mode)
    {
    case RISING:
        _clr_(DDRD,  PIN2);
        _clr_(PORTD, PIN2); /*HI _Z */
        _set_(MCUCR, ISC01);
        _set_(MCUCR, ISC00);
        break;

    case FALLING:
        _clr_(DDRD, PIN2);
        _set_(PORTD, PIN2); /*PULL UP*/
        _set_(MCUCR, ISC01);
        _clr_(MCUCR, ISC00);
        break;

    case LOGICAL_CHANGE:
        _clr_(DDRD, PIN2);
        _clr_(PORTD, PIN2); /*HI _Z */
        _clr_(MCUCR, ISC01);
        _set_(MCUCR, ISC00);
        break;
    }
}
ISR(INT1_vect)
{
    ISR_INT1();
}
ISR(INT0_vect)
{
    ISR_INT0();
}
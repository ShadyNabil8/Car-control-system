#ifndef _XINT_H
#define _XINT_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdlib.h>

#include "dio.h"
#include "trash.h"

typedef enum xint_type_t
{
    XINT0 = ISC00,
    XINT1 = ISC10,
    XINT2 = ISC2
} xint_type_t;

typedef enum xint_pin_t
{
    XINT0_PIN = DIO_PD2,
    XINT1_PIN = DIO_PD3,
    XINT2_PIN = DIO_PB2
} xint_pin_t;

typedef enum xint_trigger_t
{
    XINT_LOW = 0b00,
    XINT_ANY = 0b01,
    XINT_FALLING = 0b10,
    XINT_RISING = 0b11
} xint_trigger_t;
typedef struct xint_t
{
    xint_type_t trigger_bit;
    uint8_t enable_bit;
    dio_t pin;
    volatile uint8_t *reg;
    xint_trigger_t trigger;
    uint8_t mask;
} xint_t;

#define global_int_off(x) cli(x)
#define xint_isr(x) ISR(x##_vect)
#define XINT0_vect INT0_vect
#define XINT1_vect INT1_vect
#define XINT2_vect INT2_vect

xint_t xint_get(xint_type_t interrupt, xint_trigger_t trigger);
// xint_t xint_get(uint8_t argc,...);

void xint_attach(xint_type_t interrupt, xint_trigger_t trigger);
void xint_detach(xint_type_t interrupt);

#endif
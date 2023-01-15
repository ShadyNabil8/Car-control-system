#ifndef _DIO_H
#define _DIO_H


#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>

#include "trash.h"

typedef enum
{
    DIO_PORTA,
    DIO_PORTB,
    DIO_PORTC,
    DIO_PORTD
} dio_port_t;

typedef enum
{
    DIO_PA0 = (DIO_PORTA << 8) | (PA0),
    DIO_PA1 = (DIO_PORTA << 8) | (PA1),
    DIO_PA2 = (DIO_PORTA << 8) | (PA2),
    DIO_PA3 = (DIO_PORTA << 8) | (PA3),
    DIO_PA4 = (DIO_PORTA << 8) | (PA4),
    DIO_PA5 = (DIO_PORTA << 8) | (PA5),
    DIO_PA6 = (DIO_PORTA << 8) | (PA6),
    DIO_PA7 = (DIO_PORTA << 8) | (PA7),
    DIO_PB0 = (DIO_PORTB << 8) | (PB0),
    DIO_PB1 = (DIO_PORTB << 8) | (PB1),
    DIO_PB2 = (DIO_PORTB << 8) | (PB2),
    DIO_PB3 = (DIO_PORTB << 8) | (PB3),
    DIO_PB4 = (DIO_PORTB << 8) | (PB4),
    DIO_PB5 = (DIO_PORTB << 8) | (PB5),
    DIO_PB6 = (DIO_PORTB << 8) | (PB6),
    DIO_PB7 = (DIO_PORTB << 8) | (PB7),
    DIO_PC0 = (DIO_PORTC << 8) | (PC0),
    DIO_PC1 = (DIO_PORTC << 8) | (PC1),
    DIO_PC2 = (DIO_PORTC << 8) | (PC2),
    DIO_PC3 = (DIO_PORTC << 8) | (PC3),
    DIO_PC4 = (DIO_PORTC << 8) | (PC4),
    DIO_PC5 = (DIO_PORTC << 8) | (PC5),
    DIO_PC6 = (DIO_PORTC << 8) | (PC6),
    DIO_PC7 = (DIO_PORTC << 8) | (PC7),
    DIO_PD0 = (DIO_PORTD << 8) | (PD0),
    DIO_PD1 = (DIO_PORTD << 8) | (PD1),
    DIO_PD2 = (DIO_PORTD << 8) | (PD2),
    DIO_PD3 = (DIO_PORTD << 8) | (PD3),
    DIO_PD4 = (DIO_PORTD << 8) | (PD4),
    DIO_PD5 = (DIO_PORTD << 8) | (PD5),
    DIO_PD6 = (DIO_PORTD << 8) | (PD6),
    DIO_PD7 = (DIO_PORTD << 8) | (PD7)
} dio_t;

typedef enum
{
    DIO_DIRECTION_INPUT = 0,
    DIO_DIRECTION_OUTPUT = 1
} dio_direction_t;

typedef enum
{
    DIO_LEVEL_LOW = 0,
    DIO_LEVEL_HIGH = 1
} dio_level_t;

#define DIO_LEVEL_OFF DIO_LEVEL_LOW
#define DIO_DIRECTION_OFF DIO_DIRECTION_INPUT

void dio_set_direction(dio_t pin, dio_direction_t direction);
void dio_set_port_direction(dio_port_t port, dio_direction_t direction);
void dio_set_level(dio_t pin, dio_level_t value);
void dio_set_port_level(dio_port_t port, dio_level_t value);
dio_level_t dio_get_level(dio_t pin);
void dio_off();

#define dio_pulse(x, y)                  \
    dio_set_level(x, !dio_get_level(x)); \
    _delay_us(y);                        \
    dio_set_level(x, dio_get_level(x))

#endif
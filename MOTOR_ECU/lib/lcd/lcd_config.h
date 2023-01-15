#ifndef _LCD_CONFIG_H
#define _LCD_CONFIG_H

#include "lcd.h"

//#define LCD_DATA_PORT DIO_PORTC
#define LCD_RS_PIN DIO_PD5
#define LCD_EN_PIN DIO_PD7

#define LCD_DATA_PINS \
    {                 \
        DIO_PC4,      \
        DIO_PC5,      \
        DIO_PC6,      \
        DIO_PC7       \
    }


#define LCD_MODE LCD_4BIT_2LINE_MODE

#define LCD_LENGTH 24
#endif
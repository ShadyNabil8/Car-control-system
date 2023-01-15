#ifndef _LCD_CONFIG_H
#define _LCD_CONFIG_H

#include "lcd.h"

//#define LCD_DATA_PORT DIO_PORTC
#define LCD_RS_PIN DIO_PD5
#define LCD_EN_PIN DIO_PD4

#define LCD_DATA_PINS \
    {                 \
        DIO_PC0,      \
        DIO_PC1,      \
        DIO_PC2,      \
        DIO_PC3,      \
        DIO_PC4,      \
        DIO_PC5,      \
        DIO_PC6,      \
        DIO_PC7       \
    }


#define LCD_MODE LCD_8BIT_2LINE_MODE

#define LCD_COL_COUNT 16
#endif
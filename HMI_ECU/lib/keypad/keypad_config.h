#ifndef  _KEYPAD_CONFIG_H
#define  _KEYPAD_CONFIG_H

#include "dio.h"

#define KEYPAD_ROW_NUM    4
#define KEYPAD_COLUMN_NUM 3


#define KEYPAD_ROW_ARR_VALUE { \
    DIO_PA1,                   \
    DIO_PA2,                   \
    DIO_PA3,                   \
    DIO_PA4                    \
};

#define KEYPAD_COL_ARR_VALUE { \
    DIO_PA5,                   \
    DIO_PA6,                   \
    DIO_PA7                    \
};

#define KEYPAD_KEY_VALUES   {   \
    {'1', '2', '3'},            \
    {'4', '5', '6'},            \
    {'7', '8', '9'},            \
    {'*', '0', '#'}             \
}


#define RELEASED_LEVEL  DIO_LEVEL_HIGH
#define PRESSED_LEVEL   DIO_LEVEL_LOW

#define NO_PRESS_VAL    'x'

#endif

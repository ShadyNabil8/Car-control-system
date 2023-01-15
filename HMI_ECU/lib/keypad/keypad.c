#include "keypad.h"

void keypad_init()
{
    dio_t row_arr[KEYPAD_ROW_NUM] = KEYPAD_ROW_ARR_VALUE;
    for (uint8_t i = 0; i < KEYPAD_ROW_NUM; i++)
    {
        dio_set_level(row_arr[i], DIO_LEVEL_HIGH);
        dio_set_direction(row_arr[i], DIO_DIRECTION_INPUT);
    }

    dio_t col_arr[KEYPAD_COLUMN_NUM] = KEYPAD_COL_ARR_VALUE;
    for (uint8_t i = 0; i < KEYPAD_COLUMN_NUM; i++)
    {
        dio_set_direction(col_arr[i], DIO_DIRECTION_OUTPUT);
    }
}

void keypad_off()
{
    dio_t row_arr[KEYPAD_ROW_NUM] = KEYPAD_ROW_ARR_VALUE;
    for (uint8_t i = 0; i < KEYPAD_ROW_NUM; i++)
    {
        dio_set_level(row_arr[i], DIO_LEVEL_OFF);
        dio_set_direction(row_arr[i], DIO_DIRECTION_OFF);
    }

    dio_t col_arr[KEYPAD_COLUMN_NUM] = KEYPAD_COL_ARR_VALUE;
    for (uint8_t i = 0; i < KEYPAD_COLUMN_NUM; i++)
    {
        dio_set_level(col_arr[i], DIO_LEVEL_OFF);
        dio_set_direction(col_arr[i], DIO_DIRECTION_OFF);
    }
}

char get_pressed_key()
{
    char keys[KEYPAD_ROW_NUM][KEYPAD_COLUMN_NUM] = KEYPAD_KEY_VALUES;

    dio_t row_arr[KEYPAD_ROW_NUM] = KEYPAD_ROW_ARR_VALUE;
    dio_t col_arr[KEYPAD_COLUMN_NUM] = KEYPAD_COL_ARR_VALUE;

    // init columns to released state
    for (uint8_t i = 0; i < KEYPAD_COLUMN_NUM; i++)
    {
        dio_set_level(col_arr[i], RELEASED_LEVEL);
    }

    // scan once
    for (uint8_t c = 0; c < KEYPAD_COLUMN_NUM; c++)
    {
        dio_set_level(col_arr[c], PRESSED_LEVEL);
        for (uint8_t r = 0; r < KEYPAD_ROW_NUM; r++)
        {
            // delay for simulation
            //_delay_ms(3);
            if (dio_get_level(row_arr[r]) == PRESSED_LEVEL)
            {
                // wait debounce
                //_delay_ms(5);
                // wait
                while (dio_get_level(row_arr[r]) == PRESSED_LEVEL)
                    ;
                return keys[r][c];
            }
        }
        dio_set_level(col_arr[c], RELEASED_LEVEL);
    }

    return NO_PRESS_VAL;
}

char scan_for_key()
{
    char key = NO_PRESS_VAL;
    while (key == NO_PRESS_VAL)
    {
        key = get_pressed_key();
    }
    return key;
}
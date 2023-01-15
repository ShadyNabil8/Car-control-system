#ifndef _KEYPAD_KEYPAD_H
#define _KEYPAD_KEYPAD_H
#include "keypad_config.h"


#include <stdint.h>

#include <util/delay.h>

#include "dio.h"

void keypad_init();
char get_pressed_key();
char scan_for_key();
void keypad_off();

#endif
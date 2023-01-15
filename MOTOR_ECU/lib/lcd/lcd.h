#ifndef _LCD_H
#define _LCD_H


#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include <util/delay.h>

#include "dio.h"

#define LCD_CLEAR               0x01
#define LCD_8BIT_2LINE_MODE     0x38
#define LCD_NIBBLES             0x02
#define LCD_4BIT_2LINE_MODE     0x28
#define LCD_ENTRY               0x06
#define LCD_ON                  0x0D
#define CURSOR_OFF              0x0C
#define CURSOR_ON               0x0E
#define SET_CURSOR_LOCATION     0x80
#define SHIFT_LEFT              0x18
#define SHIFT_RIGHT             0x1C

void lcd_init(void);
void lcd_off();
void lcd_send_cmd(uint8_t command);
void lcd_send_byte(char data); 
void lcd_set_cursor(uint8_t x, uint8_t y);
void lcd_send_str(char str[]);
void lcd_send_int(int32_t data);
void lcd_printf(char *format, ...);
//void lcd_set_specialcharc(uint8_t arr[], uint8_t block_num);
//void lcd_send_specialcharc(uint8_t block_num, uint8_t x, uint8_t y);
void LCD_WRITE_NUM(uint16_t n);

#include "lcd_config.h"

#endif
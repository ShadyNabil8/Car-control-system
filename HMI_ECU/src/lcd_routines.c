#include "lcd_routines.h"

void print_welcome(void)
{
    lcd_send_cmd(LCD_CLEAR);
    lcd_set_cursor(2, 0);
    lcd_send_str("MOTOR CTRL MODE");
    _delay_ms(200);
}
void print_need_speed(void)
{
    lcd_send_cmd(LCD_CLEAR);
    lcd_set_cursor(5, 0);
    lcd_send_str("ENTER SPEED");
    lcd_set_cursor(10, 1);
}
void print_invalid_speed(void)
{
    lcd_send_cmd(LCD_CLEAR);
    lcd_set_cursor(4, 0);
    lcd_send_str("INVALID SPEED");
    _delay_ms(200);
    lcd_send_cmd(LCD_CLEAR);
}
void print_speed(uint8_t speed)
{
    lcd_send_cmd(LCD_CLEAR);
    lcd_set_cursor(6, 0);
    lcd_send_str("SPEED IS");
    lcd_set_cursor(8, 1);
    lcd_printf("%3d", speed);
    lcd_send_byte('%');
    _delay_ms(100);
    lcd_send_cmd(LCD_CLEAR);
}
void draw_dashboard(uint8_t speed)
{
    lcd_send_cmd(LCD_CLEAR);
    lcd_send_str("MOTOR SPEED:");
    lcd_printf("%3d", speed);
    lcd_set_cursor(15, 0);
    lcd_send_str("[ON]");
    lcd_set_cursor(0, 1);
    lcd_send_str("MOTOR TEMP :            ");
}

void draw_light_picker()
{
    lcd_set_cursor(0, 0);
    lcd_send_str(" 11:FR, 12:FL, 15:F ");
    lcd_set_cursor(0, 1);
    lcd_send_str(" 13:RR, 14:RL, 16:R ");
}

void print_system_failed(void)
{
    lcd_send_cmd(LCD_CLEAR);
    lcd_send_str("SYSTEM FAILED");
    lcd_set_cursor(0, 1);
    lcd_send_str("RESTART IN :");
}
void print_select_mode(void)
{
    lcd_send_cmd(LCD_CLEAR);
    lcd_set_cursor(2, 0);
    lcd_send_str("1:CTRL MOTOR ECU");
    lcd_set_cursor(2, 1);
    lcd_send_str("2:CTRL LIGHT ECU");
}
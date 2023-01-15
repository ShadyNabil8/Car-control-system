#include "lcd.h"

// TODO learn and implement 4 bit mode

#define INIT_DELAY 500
#define LCD_DELAY 40
#define NIBBLE_DELAY 5

#if LCD_MODE == LCD_4BIT_2LINE_MODE
#define LCD_WIDTH 4
#else
#define LCD_WIDTH 8
#endif

void lcd_init()
{
    dio_t data_reg[LCD_WIDTH] = LCD_DATA_PINS;
    // pin init
    dio_set_direction(LCD_EN_PIN, DIO_DIRECTION_OUTPUT);
    dio_set_direction(LCD_RS_PIN, DIO_DIRECTION_OUTPUT);
    for (uint8_t pin = 0; pin < LCD_WIDTH; pin++)
    {
        dio_set_direction(data_reg[pin], DIO_DIRECTION_OUTPUT);
    }

    // LCD Init
#if LCD_MODE == LCD_4BIT_2LINE_MODE
    lcd_send_cmd(LCD_NIBBLES);
#endif
    lcd_send_cmd(LCD_MODE); // function set
    lcd_send_cmd(CURSOR_OFF);
    lcd_send_cmd(LCD_ENTRY);
    _delay_us(500);
}

void lcd_send(uint8_t message)
{
    dio_t data_reg[LCD_WIDTH] = LCD_DATA_PINS;
    // dio_set_port_level(LCD_DATA_PORT, message & 0xf0);

#if LCD_MODE == LCD_4BIT_2LINE_MODE

    for (uint8_t pin = 0; pin < LCD_WIDTH; pin++)
    {
        dio_set_level(data_reg[pin], (message >> (pin + 4)) & 1);
    }

    dio_pulse(LCD_EN_PIN, LCD_DELAY);

#endif

    for (uint8_t pin = 0; pin < LCD_WIDTH; pin++)
    {
        dio_set_level(data_reg[pin], (message >> pin) & 1);
    }

    // dio_set_port_level(LCD_DATA_PORT, message);
    dio_pulse(LCD_EN_PIN, LCD_DELAY);
    _delay_us(LCD_DELAY);
}

void lcd_send_cmd(uint8_t command)
{
    // read/write -> always write
    // RS-> CMD:0
    dio_set_level(LCD_RS_PIN, DIO_LEVEL_LOW);
    _delay_us(5);
    // write value on data bus
    lcd_send(command);
    _delay_us(2000 - LCD_DELAY); // total of 2ms delay
}

void lcd_send_byte(char data)
{
    // RS-> data:1
    dio_set_level(LCD_RS_PIN, DIO_LEVEL_HIGH);
    _delay_us(LCD_DELAY); // try to set this to 5us

    lcd_send(data);
}

void lcd_send_str(char str[])
{
    // RS -> data:1
    dio_set_level(LCD_RS_PIN, DIO_LEVEL_HIGH);
    uint8_t i = 0;

    while (str[i] != '\0')
    { // write value on data bus
        lcd_send(str[i]);
        i++;
    }
}

void lcd_send_int(int32_t data)
{
    uint8_t len = snprintf(NULL, 0, "%ld", data);
    char *str_buffer = malloc(len + 1);
    sprintf(str_buffer, "%ld", data);
    lcd_send_str(str_buffer);
    free(str_buffer);
}

void lcd_set_cursor(uint8_t x, uint8_t y)
{
    uint8_t address = x;
    if (y == 1)
        address += (1 << 6);
    lcd_send_cmd(SET_CURSOR_LOCATION | address);
}

void lcd_off()
{
    dio_t data_reg[LCD_WIDTH] = LCD_DATA_PINS;
    lcd_send_cmd(LCD_CLEAR);

    for (uint8_t pin = 0; pin < LCD_WIDTH; pin++)
    {
        dio_set_level(data_reg[pin], DIO_LEVEL_OFF);
    }

    for (uint8_t pin = 0; pin < LCD_WIDTH; pin++)
    {
        dio_set_direction(data_reg[pin], DIO_DIRECTION_OFF);
    }
    dio_set_level(LCD_RS_PIN, DIO_LEVEL_OFF);
    dio_set_level(LCD_EN_PIN, DIO_LEVEL_OFF);
}

void lcd_printf(char *format, ...)
{
    va_list args;
    va_start(args, format);
    //uint8_t len = snprintf(NULL, 0, format, args);
    char str_buffer[LCD_LENGTH+1];
    vsnprintf(str_buffer, LCD_LENGTH+1, format, args);
    va_end(args);
    lcd_send_str(str_buffer);
    //free(str_buffer);
}
#include <avr/io.h>
#include <math.h>
#include <util/delay.h>

#include "INTx.h"
#include "UART.h"

//#include "timer1.h"
#include <avr/wdt.h>

#include "i2c.h"
#include "keypad.h"
#include "lcd.h"
#include "lcd_routines.h"
#include "light.h"
#include "timer.h"
#define _avr_reset         \
    wdt_enable(WDTO_15MS); \
    while (1)

typedef enum op_mode_t {
    lighting = '2',
    motor = '1'
} op_mode_t;

volatile op_mode_t op_mode;  // DEFINE THE RUNNING MODE

typedef enum motor_op_t {
    motor_fail = 'F',
    motor_ok = 1,
    motor_on = 0x4F,  // O NOT ZERO
    motor_off = 'B',
    motor_toggle = 2
} motor_op_t;

typedef motor_op_t motor_status_t;


#define RESET_DELAY 3  // the countdown period before resetting on motor failure

uint8_t speed = 0;  // VAR CARRYING MOTOR SPEED
motor_status_t motor_status = motor_off;
volatile motor_op_t motor_op = 0;
volatile uint8_t temp = 0;         // VAR CARRYING MOTOR TEMP
volatile uint8_t update_temp = 0;  // SEE
volatile uint8_t reset_counter;    // USED TO CONTROL TIMER1 AS SEEN IN ISR_TIMER1_FUNCTION()

void select_mode();

void get_speed(void);
volatile uint8_t monitor_motor(void);
#define MOTOR_OVER_HEAT 70

void reset_countdown(void);

uint8_t keypad_get_int();

void light_switcher(void);

// ISR definitions
void switch_mode(void);
void toggle_motor(void);
void ISR_UART_RX_FUNCTION(void);

int main(void) {
    lcd_init();
    keypad_init();

    timer_init(TMR1, PSC_1024, 1, TMR_INT);
    INT0_init(FALLING);
    INT1_init(FALLING);
    ISR_INT0 = &switch_mode;
    ISR_INT1 = &toggle_motor;

    UART_init(UART_interrupt, TXandRX, baud_9600);
    ISR_RX = &ISR_UART_RX_FUNCTION;  // interrupt service routine on uart recieve

    select_mode();

    while (1) {
        while (monitor_motor() == motor_ok)
            ;
        _avr_reset;
    }
}

void select_mode() {
    print_select_mode();

    op_mode = scan_for_key();  // SELECT THE MODE USUING KEYPAD
}

void switch_mode(void) {
    if (op_mode == motor) {
        op_mode = lighting;
    } else {
        op_mode = motor;
    }
}

void toggle_motor(void) {
    motor_op = 1;
}

void get_speed(void) {
    // uint8_t NUMBER = 0; // USED  IN FOR LOOP
    // short int i = 2;    // USED  IN FOR LOOP

    print_welcome();

    /**
     * @brief GET SPEED LOGIC
     *
     */
    while (speed == 0 || speed > 100) {
        print_need_speed();

        keypad_init();
        speed = keypad_get_int();
        keypad_off();  // I DO NOT NEED THE KEYPAD ANY MORE

        if (speed <= 100) {
            print_speed(speed);
            draw_dashboard(speed);
            UART_sendInteger(speed);  // SEND THE SPEED TO MOTOR_ECU
            motor_status = motor_on;
        } else {
            print_invalid_speed();
        }
    }
}

void light_switcher(void) {
    TWI_init(BITRATE, MASTER_ADDRESS);
    keypad_init();

    draw_light_picker();

    if (scan_for_key() == '1') {
        lcd_set_cursor(0, 0);
        lcd_send_cmd(CURSOR_ON);

        uint8_t message;
        switch (scan_for_key()) {
            case '1':
                message = (FR);
                break;
            case '2':
                message = (FL);
                break;
            case '5':
                message = (FR | FL);
                break;
            case '3':
                message = (RR);
                break;
            case '4':
                message = (RL);
                break;
            case '6':
                message = (RR | RL);
                break;
            default:
                break;
        }

        lcd_send_cmd(CURSOR_OFF);
        TWI_send_byte(TWI_master, SLAVE_ADDRESS, message);
    }
    keypad_off();
    TWI_off();
}

volatile uint8_t monitor_motor(void) {
    if (op_mode == lighting) {
        light_switcher();
        draw_dashboard(speed);
        update_temp = 1;
        op_mode = motor;
    }

    if (speed == 0)
        get_speed();

    if (motor_op) {
        lcd_set_cursor(15, 0);
        switch (motor_status) {
            case motor_on:
                UART_sendByte(motor_off);
                lcd_send_str("[OFF]");
                motor_status = motor_off;
                break;
            case motor_off:
                UART_sendByte(motor_on);
                lcd_send_str("[ON] ");
                motor_status = motor_on;
                break;
            default:
                break;
        }
        motor_op = 0;
    }

    if (update_temp == 1) {
        update_temp = 0;
        lcd_set_cursor(12, 1);
        if (temp >= MOTOR_OVER_HEAT) {
            lcd_set_cursor(0, 1);
            lcd_send_str("OVR_HEAT_COOLING_ON");
            /**
             * @brief I WILL CHANGE THE MOOD TO POLLING MODE..
             * AND WAIT TO GET motor_fail OR SYSTEM CONTANUE..
             * CHANGING THE MECHANISM HERE FIXES A BUG
             *
             */
            UART_change_mechanism(UART_polling);

            if (UART_recieveByte() == motor_fail) {
                TWI_init(BITRATE, MASTER_ADDRESS);
                TWI_send_byte(TWI_master, SLAVE_ADDRESS, motor_fail);
                TWI_off();
                
                reset_countdown();  // ENABLE TIMER 1  AND COUNTING DOWN
                /**
                 * @brief when i return 0, i will get out of the while loop and reset
                 *
                 */
                return motor_fail;
            } else {
                /**
                 * @brief I AM OUT OF THE CONDITION, SO I WILL INITIALIZE INTERRUPT MECHANISM AGAIN
                 *
                 */
                reset_counter = 0;
                UART_change_mechanism(UART_interrupt);
                lcd_set_cursor(0, 1);
                lcd_send_str("MOTOR TEMP :            ");
                lcd_set_cursor(12, 1);
                lcd_printf("%3d", temp - 1);
            }
        } else {
            lcd_printf("%3d", temp);
        }
    }

    /**
     * @brief when i return 0, i will get out of the while loop and repeat again
     *
     */
    return motor_ok;
}

uint8_t keypad_get_int() {
#define INPUT_LEN 4
    char buffer[INPUT_LEN];
    for (char *i = buffer;
         (i < buffer + INPUT_LEN); i++) {
        *i = scan_for_key();
        if (*i == '*') {
            *i = '\0';
            break;
        }
#ifdef _LCD_H
        else {
            lcd_send_byte(*i);
        }
#endif
    }
    return atoi(buffer);
}

timer_isr(TMR1) {
    reset_counter++;
    if (reset_counter == RESET_DELAY) {
        /* IT MEANS THAT : OH IT HAS BEEN 10 SECONDS AFTER timer1_start_again(); */
        timer_off(TMR1);  // DIASBLE THE TIMER1, ITS JOB IS DONE
    }
}

void ISR_UART_RX_FUNCTION(void) {
    temp = UART_DATA;
    update_temp = 1;
}

void reset_countdown(void) {
    print_system_failed();
    /**
     * @brief enable (by assigning a prescaller to it) timer 1 to reset after 20 sec
     *
     */
    timer_init(TMR1, PSC_1024, 1, TMR_INT);
    while (reset_counter < RESET_DELAY) {
        /**
         * the countdown has started
         */
        lcd_set_cursor(13, 1);
        lcd_printf("%3d", (RESET_DELAY - reset_counter + 1));
    }
    reset_counter = 0;  // RESET THE COUNTER
}
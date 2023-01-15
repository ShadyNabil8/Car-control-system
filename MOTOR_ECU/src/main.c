/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-09-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <ADC.h>
#include <UART.h>
#include <avr/wdt.h>
#include <lcd.h>
#include <masking.h>
#include <timer0.h>
#include <timer1.h>
#include <util/delay.h>

/**
 * @brief MACROS & GLOBAL VARs & FUNCTION DECLERATIONS
 *
 */

#define RESET_DELAY 3
#define SYSTEM_FAIL 'F'
#define SYSTEM_CONTINUE 'C'
#define ACK 'A'
#define SYSTEM_COMMAND_MOTOR_ON 'O'  // O NOT ZERO
#define SYSTEM_COMMAND_MOTOR_BREAK 'B'
#define MOTOR_OVER_HEAT 70
#define FAN_BIN 2
#define SEND_ACK UART_sendByte(ACK)
#define RECEIVE_ACK UART_recieveByte()
short int SPEED = 0;        // SPEED OF THE MOTOR
uint8_t TEMP = 0;           // VAR CARRYING MOTOR TEMP
uint8_t PREVIOUS_TEMP = 0;  // VAR CARRYING MOTOR PREVIOUS TEMP
int GLOBAL_COUNTER = 0;     // USED TO CONTROL TIMER0 AS SEEN IN ISR_TIMER1_FUNCTION()
/**
 * @brief ISRs FOR TIMER1 AND UART
 *
 */

void ISR_TIMER1_FUNCTION(void) {
    GLOBAL_COUNTER++;
}

void UART_ISR_RX_FUNCTION(void) {
    if (UART_DATA == SYSTEM_COMMAND_MOTOR_BREAK)
        timer0_change_duty(0);  // MOTOR BREAKS
    else
        timer0_change_duty(SPEED);  // MOTOR FREE RUNS
}

/**
 * @brief MAIN FUNCTION
 *
 * @return int
 */

int main(void) {
    /**
     * @brief TIMER1 PARAMS
     * MODE:OVF MODE
     * PRESCALLER:NONE(IT MEANS THAT TIMER 1 IS DISABLED UNTILL I ASSIGN A PRESCALLER VALUE TO IT)
     * CHANNEL:A
     * TCNT:0
     * OCR:0
     * ICR:0
     */
    timer1_params timer1 = {timer1_overflow, timer1_pre_none, channel_A, 0, 0, 0};
    timer1_init(&timer1);
    timer1_start();                         // IT WILL BE DISABLED UNTILL I ASSIGN A PRESCALLER TO IT
    timer1_OVF_ISR = &ISR_TIMER1_FUNCTION;  // DEFINE THE ISR FOR TIMER1 WHEN OVF OCCURE

    /**
     * @brief TIMER0 PARAMS
     * MODE:PWM
     * PRESCALLER:/8
     * TCNT:0
     * DURTY:0
     */
    timer0_params timer0 = {timer0_fast_pwm, timer0_pre_8, 0, 0};
    timer0_init(&timer0);
    timer0_start();

    /**
     * @brief ADC PARAMS
     * CHANNEL:0
     * PRESCALLER:/8
     * MECHANISM:POOLING
     */
    ADC_init(0, ADC_pre_8, ADC_polling);  // INITIALIZE THE ACD

    /**
     * @brief Construct a new uart init object
     * pooling MODE
     * TRANSMITTER AND RECEIVER
     * SELECT BOAD RATE 9600
     */
    UART_init(UART_pooling, TXandRX, baud_9600);  // INITIALIZE UART COMMUNICATION PROTOCOL.

    // START: // FOR RESETTING

    /**
     * I WILL NOT MOVE UNTILL I RECEIEVE THE SPEED
     * FROM HMI_ECU
     */
    _set_(DDRB, FAN_BIN);   // SET FAN_BIN AS OUTPUT
    _clr_(PORTB, FAN_BIN);  // INITIALIZE FAN_BIN AS LOW
    timer0_change_duty(0);  // INITIALIZE THE SPEED WITH ZERO
    /**
     * @brief pooling mechanic to make it easy to get the speed digits without multiple interrupts
     *
     */
    UART_change_mechanism(UART_pooling);
    SPEED = UART_recieveInteger();  // WAIT TO RECEIEVE THE SPEED
    timer0_change_duty(SPEED);      // ASSIGN THE SPEED
    /**
     * @brief interrupts mechanism to receive them command with interrupts
     * It solves the syncronization problem
     *
     */
    UART_change_mechanism(UART_interrupt);
    ISR_RX = &UART_ISR_RX_FUNCTION;

    while (1) {
        PREVIOUS_TEMP = TEMP;
        TEMP = (ADC_start() * 100) / 205;
        if (PREVIOUS_TEMP != TEMP) {
            // SEND_ACK;
            // if (RECEIVE_ACK == ACK)
            //{
            // UART_sendInteger(TEMP); // SEND THE TEMP TO THE HMI_ECU
            UART_sendByte(TEMP);
            //}
            if (TEMP >= MOTOR_OVER_HEAT) {
                _set_(PORTB, FAN_BIN);               // FAN ON
                timer1_start_again(timer1_pre_256);  // START COUNTING 20 SEC

                /**
                 * DO NOTHING UNTILL THE TEMP BE LESS THEN 70(MOTOR_OVER_HEAT)
                 * IF IT HAS BEEN 20 SEC AND THE TEMP >= 70 {SYSTEM FAIL}
                 * ELSE{SYSTEM CONTINUE}
                 */
                while (((ADC_start() * 100) / 205) >= MOTOR_OVER_HEAT) {
                    if (GLOBAL_COUNTER == RESET_DELAY) {
                        /* IT MEANS THAT : OH IT HAS BEEN 20 SECONDS AFTER timer1_start_again(); */

                        /**
                         * STOP THE TIMER INCASE OF IT HAS BEEN 20SEC
                         * I MEAN THAT THE TEMP IS NOT DECREASED TO BE LESS THAN 70 IN 20SEC
                         */
                        timer1_stop();
                        UART_sendByte(SYSTEM_FAIL);

                        /**
                         * @brief Now i wnat to reset the code, i will use whatchdog timer to reset
                         * The timer whill count 15ms and reset.
                         *
                         */
                        wdt_enable(WDTO_15MS);
                        while (1)
                            ;
                    }
                }

                /**
                 * STOP THE TIMER INCASE OF IT HAS NOT BEEN 20SEC
                 * I MEAN THE THE TEMP IS DECREASED TO BE LESS THAN 70
                 */
                timer1_stop();
                _clr_(PORTB, FAN_BIN);  // FAN OFF
                GLOBAL_COUNTER = 0;     // RESET THE COUNTER

                UART_sendByte(SYSTEM_CONTINUE);
            }
        }
    }
    return 0;
}

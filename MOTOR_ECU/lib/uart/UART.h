/*
 ***************** UART COMMUNICATION *****************
 * NAME    : Shady_Nabil
 * Created : 27/8/2022
 * Version : 1.0
 */

#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>

extern void (*ISR_RX)(void);

/*******************************************************************************
                                typedefs & mocros
*******************************************************************************/
#define UART_DATA UDR

typedef enum
{
    UART_pooling,
    UART_interrupt,
} UART_mechanism;

typedef enum
{
    transmiter,
    reciever,
    TXandRX,
} UART_mode;

typedef enum
{
    baud_110  = 110,
    baud_300  = 300,
    baud_600  = 600,
    baud_1200 = 1200,
    baud_2400 = 2400,
    baud_4800 = 4800,
    baud_9600 = 9600,

} UART_baud;



/*******************************************************************************
                                functions prototypes
*******************************************************************************/
void UART_init(UART_mechanism mech, UART_mode mode , UART_baud baud);
char UART_recieveByte(void);
void UART_recieveString(char * str);
int UART_recieveInteger(void);
void UART_sendByte(const char data);
void UART_sendString(const char * str);
void UART_sendInteger(const int n);
void UART_change_mechanism(UART_mechanism mech);

#endif /* end _UART_H_ */
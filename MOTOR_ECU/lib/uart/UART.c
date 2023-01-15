#include "UART.h"
#include <util/delay.h>
#include<stdlib.h>
void (*ISR_RX)(void);

void UART_init(UART_mechanism mech, UART_mode mode , UART_baud baud)
{
    /* initialize the registers */
    UCSRA = 0b0010000;
    UCSRB = 0b0000000;
    UCSRC = 0b1000110;

    uint16_t UBRR_value = F_CPU/(8.0*baud)-1; /* indentify the boad rate */

    /*
     * *************************** UCSRA ******************************
     * RXC  = 0 when the recieve  buffer is (empty) (NOT READY). unless = 1 (READY)
     * TXC  = 1 when the transmit buffer is (empty). unless = 0
     * UDRE = 1 when the transmit buffer is ready to recieve a new data (READY). unless = 0 (NOT READY)
     * FE , DOR , PE  -> Always set this bit to (zero) when writing to UCSRA.
     * PE = 1 if there is parity error
     * U2X -> has effict in Async mode and written to 0 in the Sync mode (when equlas 1 -> double trnasfer rate)
     * MPCM = 0
     */

    UCSRA |= 1 << U2X;
    UCSRA &= ~(1 << MPCM);

    /*
     * *************************** UCSRB ******************************
     * RXCIE , TXCIE : recieve/trnasmit complete interrupt enable ( Hint, enable global interrupt)
     * UDRIE : is set to enable interrupt mode (Hint, enable global interrupt)
     * RXEN , TXEN : enable UART reciever/transmiter
     *****************************************************************/
    switch(mode)
    {
        case TXandRX    : UCSRB   |= ((1 << RXEN) | (1 << TXEN)); break;
        case transmiter : UCSRB   |= 1 << TXEN;                   break;
        case reciever   : UCSRB   |= 1 << RXEN;                   break;
    }
   
    if (mech == UART_interrupt)
    {
        SREG  |= 1<<SREG_I; /* Enable global interrupt */
        //UCSRB |= ((1 << UDRIE) | (1 << RXCIE) /*| (1 << TXCIE:)*/ );
        UCSRB |= 1 << RXCIE;
    }

    /*
     * *************************** UCSRC ******************************
     * URSEL UMSEL UPM1 UPM0 USBS UCSZ1 UCSZ0 UCPOL (initial values)
     *   1    0     0    0    0     1     1     0
     * URSEL : The URSEL must be one when writing the UCSRC. (unless -> UBRRH register)
     * UMSEL = 0 -> Async , 1 -> Sync
     * UPM1:0 : parity mode
     * USBS = 0 -> 1_bit stop_bit , = 1 -> 2_bit stop_bit
     * UCSZ2:0 -> character size (Hint , UCSZ2 in USCRB)
     ******************************************************************/

    /* Write in UBRR to identify the boad rate */
    UCSRC &= ~(1 << URSEL);
    UBRRH = UBRR_value>>8;
    UBRRL = UBRR_value;
}

/*****************************************************
 *              Transmition Functions                *
 ****************************************************/
char UART_recieveByte(void)
{
    while(!(UCSRA>>RXC)&1)
        ;
    return UDR;

}

void UART_recieveString(char * str)
{
    uint8_t i = 0;
    do
    {
        *(str+i) = UART_recieveByte();
        i++;
    }while(*(str+i-1) != '\0'); /* this formula to recieve the null pointer */
}

int UART_recieveInteger(void)
{
    char str[11]; /* MAX_INT IS 10 DIDGITS AND ONE MORE FOR NULL POINTER */
    UART_recieveString(str);
    return atoi(str);
}

void UART_sendByte(const char data)
{
    while(!(UCSRA>>UDRE)&1)
        ;
    UDR = data;

}
void UART_sendString(const char * str)
{
    do
    {
        UART_sendByte(*str);
        _delay_ms(5);
        str++;
    }while(*(str-1) != '\0'); /* this formula to send the null pointer */

}
void UART_sendInteger(const int n)
{
    char str[11]; /* MAX_INT IS 10 DIGITS AND ONE MORE FOR NULL POINTER */
    itoa(n,str,10);
    UART_sendString(str);
}
/*****************************************************************************/

/*****************************************************
 *              ISR Functions                        *
 ****************************************************/
ISR(USART_RXC_vect)
{
    ISR_RX();
    /**
     * @brief the receive complete routine must [read the received data from UDR] in order to clear the RXC Flag,
     *        otherwise a new interrupt will occur once the interrupt routine terminates
     * 
     */
}

/*****************************************************
 *              OTHER Functions                        *
 ****************************************************/

void UART_change_mechanism(UART_mechanism mech)
{
    if(mech == UART_interrupt)
    {
        SREG  |= 1<<SREG_I; /* Enable global interrupt */
        //UCSRB |= ((1 << UDRIE) | (1 << RXCIE) /*| (1 << TXCIE:)*/ );
        UCSRB |= 1 << RXCIE;
    }
    else
    {
        UCSRB &= ~(1 << RXCIE); //DISSABLE RX INTERRUPTS
    }
}
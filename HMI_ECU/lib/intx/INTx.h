#ifndef _INTX_H_
#define _INTX_H_
#include<avr/io.h>
#include<avr/interrupt.h>
#include"masking.h"

extern void (*ISR_INT1)(void);
extern void (*ISR_INT0)(void);

#define FALLING         0
#define RISING          1
#define LOGICAL_CHANGE  2

#define I_BIT      7
#define INT0_BIT   6
#define INT1_BIT   7
#define INT2_BIT   5
#define INTF0_BIT  6
#define INTF1_BIT  7
#define INTF2_BIT  5
#define ISC11_BIT  3   
#define ISC10_BIT  2
#define ISC01_BIT  1
#define ISC00_BIT  0

void INT1_init(uint8_t mode);
void INT0_init(uint8_t mode);



#endif
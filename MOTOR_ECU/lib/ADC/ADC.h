/*
    * Auther  : Shady Nabil
    * Created : 8/25/2022
    * version : 1.0
*/

#ifndef _ADC_H_
#define _ADC_H_

#include<avr/io.h>
#include<avr/interrupt.h>

extern uint16_t (*ADC_start) (void);
extern void (*ISR_ADC)(void);

/* NOOOTE -> this ACD is single inded input only */

/*
    ************************ ADMUX register **********************
    • Bit 7:6 – REFS1:0: Reference Selection Bits
        select the refrence voltage (AREF, Internal Vref turned off) :
        REFS0 = 0 , REFS1 = 0 .
    • Bit 5 – ADLAR: ADC Left Adjust Result
        selsct left or right adjusted : 
        ->Write one to ADLAR to left adjust the result.
        ->Otherwise, the result is right adjusted.
    • Bits 4:0 – MUX4:0: Analog Channel and Gain Selection Bits
        select the channel and mode you want from bits MUX4:0.
    ************************ ADCSRA register **********************
    • Bit 7 – ADEN: ADC Enable
        ADEN bit controls (enable / disabel) if the ADC
    • Bit 6 – ADSC: ADC Start Conversion
        In Single Conversion mode, write ADSC bit to one to start each conversion.
        In Free Running Mode, write this bit to one to start the first conversion.
    • Bit 5 – ADATE: ADC Auto Trigger Enable
        if HIGH, Auto Triggering is enabled. The ADC will start
        a conversion on a positive edge of the selected trigger signal.
    • Bit 4 – ADIF: ADC Interrupt Flag
        ADIF is cleared by hardware when executing the corresponding interrupt
        handling vector.
    • Bit 3 – ADIE: ADC Interrupt Enable
        do not forget to write I_BIT in SREG to HIGH
    • Bits 2:0 – ADPS2:0: ADC Prescaler Select Bits
    ************************ ADC register **********************
    • ADC9:0: ADC Conversion Result
    ************************ SFIOR register **********************
    • Bit 7:5 – ADTS2:0: ADC Auto Trigger Source
      i will not use these bits as i disabled the auto triggeing bit
      ->this means single conversion mode

*/


typedef enum {
    ADC_pre_2,
    ADC_pre_4,
    ADC_pre_8,
    ADC_pre_16,
    ADC_pre_32,
    ADC_pre_64,
    ADC_pre_128,
}ADC_prescaller;

typedef enum {
    ADC_polling,
    ADC_interrupt,
}ADC_mode;


void ADC_init(uint8_t channel,ADC_prescaller ADC_pre,ADC_mode mode);
void ADC_off(void);


#endif /* end _ADC_H_ */
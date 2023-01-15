#include"ADC.h"


void     (*ISR_ADC)  (void);
uint16_t (*ADC_start)(void);

static uint16_t ADC_start_pooling  (void);
static uint16_t ADC_start_interrupt(void);


void ADC_init(uint8_t channel,ADC_prescaller ADC_pre,ADC_mode mode){
     /* as and initialization */

    ADMUX = 0 ; /* Initialize the register */
    ADMUX &= ~((1<<REFS0)|(1<<REFS1)); /* AREF, Internal Vref turned off */
    ADMUX &= ~(1<<ADLAR); /* right adjusted */

    /* Selsct the Channel */
    channel &= 0b00000111; /* To prevent any wrong choise */
    ADMUX |= channel;

    ADCSRA = 0 ; /* Initialize the register */
    ADCSRA |= 1<<ADEN; /* Enable the ADC */
    ADCSRA &= ~(1<<ADATE); /* Disable Auto trigger to get a single conversion */
    
    if(mode == ADC_interrupt){
        SREG |= 1<<SREG_I; /* Enable globel interrupt */
        ADCSRA |= 1<<ADIE; /* ADC Interrupt Enable */
        ADC_start = &ADC_start_interrupt;
    } /* else -> pooling mode */
    else
        ADC_start= &ADC_start_pooling;

    /* Select the prescaller */
    switch(ADC_pre){
        case ADC_pre_2  :                                                   break;
        case ADC_pre_4  : ADCSRA |= (1<<ADPS1)                            ; break;
        case ADC_pre_8  : ADCSRA |= ((1<<ADPS1) | (1<<ADPS0))             ; break;
        case ADC_pre_16 : ADCSRA |= (1<<ADPS2)                            ; break;
        case ADC_pre_32 : ADCSRA |= ((1<<ADPS2) | (1<<ADPS0))             ; break;
        case ADC_pre_64 : ADCSRA |= ((1<<ADPS1) | (1<<ADPS2))             ; break;
        case ADC_pre_128: ADCSRA |= ((1<<ADPS1) | (1<<ADPS2) | (1<<ADPS0)); break;
    }

    SFIOR = 0; /* Initialize the register */
}

uint16_t ADC_start_pooling(void){
    ADCSRA |= 1<<ADSC; /* ADC start conversion */
    while((ADCSRA>>ADSC)&1); /* while the ADSC bit is 1 -> pooling*/
    return ADC;
}
uint16_t ADC_start_interrupt(void)
{
    ADCSRA |= 1<<ADSC; /* ADC start conversion */
    return 0;
}
void ADC_off(void){
   ADCSRA &= ~(1 << ADEN);
}
ISR(ADC_vect)
{
    ISR_ADC();
}

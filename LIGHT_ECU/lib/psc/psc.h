#ifndef _PSC_H
#define _PSC_H
#include "psc_trash.h"


#include <stdint.h>

typedef enum psc_mode
{
    PSC0, // timer{0,1}
    PSC1, // timer2
    PSCA, // adc prescaler
    SCK,  // spi prescaler
    SCL   // i2c prescaler
} psc_mode;

typedef enum psc_scale
{
    PSC_OFF = 0,
    PSC_1 = 1,
    PSC_2 = 2,
    PSC_4 = 4,
    PSC_8 = 8,
    PSC_16 = 16,
    PSC_32 = 32,
    PSC_64 = 64,
    PSC_128 = 128,
    PSC_256 = 256,
    PSC_1024 = 1024,
    psc_mask = 0b111
} psc_scale;

uint8_t select_psc(psc_scale scale, psc_mode mode);

#endif
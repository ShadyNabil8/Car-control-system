#ifndef _PSC_PRIVATE_H
#define _PSC_PRIVATE_H

typedef enum
{
    PSC0_OFF = 0,
    PSC0_1,
    PSC0_8,
    PSC0_64,
    PSC0_256,
    PSC0_1024
} psc0_t;

typedef enum
{
    PSC1_OFF,
    PSC1_1,
    PSC1_8,
    PSC1_32,
    PSC1_64,
    PSC1_128,
    PSC1_256,
    PSC1_1024
} psc1_t;

typedef enum
{
    PSCA_DEF = 0,
    PSCA_2,
    PSCA_4,
    PSCA_8,
    PSCA_16,
    PSCA_32,
    PSCA_64,
    PSCA_128
} psca_t;

typedef enum
{
    SCK_2 = 4,
    SCK_4 = 0,
    SCK_8 = 5,
    SCK_16 = 1,
    SCK_32 = 6,
    SCK_64 = 2,
    SCK_128 = 3
} pscs_t;

typedef enum
{
    SCL_1 = 0,
    SCL_4,
    SCL_16,
    SCL_64,
} psci_t;

#endif
#include "psc.h"

uint8_t select_psc(psc_scale scale, psc_mode mode)
{
    switch (mode)
    {
    case PSC0:
    {
        psc_scale psc[] = {0, 1, 8, 64, 256, 1024};
        for (int value = PSC0_OFF; value <= PSC0_1024; value++)
        {
            if (scale == psc[value])
                return value;
        }
        break;
    }
    case PSC1:
    {
        psc_scale psc[] = {0, 1, 8, 32, 64, 128, 256, 1024};
        for (int value = PSC1_OFF; value <= PSC1_1024; value++)
        {
            if (scale == psc[value])
                return value;
        }
        break;
    }
    case PSCA:
    {
        psc_scale psc[] = {2, 2, 4, 8, 16, 32, 64, 128};
        for (int value = PSCA_DEF; value <= PSCA_128; value++)
        {
            if (scale == psc[value])
                return value;
        }
        break;
    }
    case SCK:
    {
        psc_scale psc[] = {4, 16, 64, 128, 2, 8, 32};
        for (int value = SCK_4; value <= SCK_32; value++)
        {
            if (scale == psc[value])
                return value;
        }
        break;
    }
    case SCL:
    {
        psc_scale psc[] = {1, 4, 16, 64};
        for (int value = SCL_1; value <= SCL_64; value++)
        {
            if (scale == psc[value])
                return value;
        }
        break;
    }
    default:
        break;
    }
}
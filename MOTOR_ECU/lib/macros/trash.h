#ifndef _TRASH_H
#define _TRASH_H

//#include <stdbool.h>

#ifndef _BIT_MATH_H
#define _BIT_MATH_H

#define _set_bit(num, ind) (num) |= (1 << (ind))
#define _get_bit(num, ind) (((num) >> (ind)) & 1)
#define _clear_bit(num, ind) (num) &= ~(1 << (ind))
#define _toggle_bit(num, ind) (num) ^= (1 << (ind))
#define _assign_bit(num, ind, val) \
    (num) &= ~(1 << (ind));        \
    (num) |= ((val) << (ind))
#define _assign_pattern(num, ind, mask, val) \
    (num) &= ~(mask << (ind));               \
    (num) |= ((val & mask) << (ind))

#endif

#ifndef _STD_TYPES_H
#define _STD_TYPES_H

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short int u16;
typedef signed short int s16;
typedef unsigned long int u32;
typedef signed long int s32;
typedef float f32;
typedef double f64;
typedef long double f80;

#endif

#ifndef _DEBUG_H
#define _DEBUG_H

#define _avr_reset         \
    wdt_enable(WDTO_15MS); \
    while (1)

#ifdef DEBUG
#define D(x) \
    do       \
    {        \
        x    \
    } while (0);
#define P(x)
#else
#define D(x)
#define P(x) \
    do       \
    {        \
        x    \
    } while (0);
#endif

#endif

#endif
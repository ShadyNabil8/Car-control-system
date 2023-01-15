#ifndef _MASKING_H_
#define _MASKING_H_

#define _set_(reg,bit)   reg |= 1<<bit
#define _clr_(reg,bit)   reg &= ~(1<<bit)
#define _get_(reg,bit)  (reg>>bit)&1
#define _tog_(reg,bit)   reg ^= 1<<bit

#endif /* _MASKING_H_ */


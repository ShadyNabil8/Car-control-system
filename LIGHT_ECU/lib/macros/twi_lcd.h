volatile i2c_return_t twi_return = 0;
uint8_t depth = 0;
#define _TWI_DEBUG  depth = __LINE__; twi_return =
#define TWI_RET (twi_return & ~TWI_ERROR_MASK)
#define TWI_STA (twi_return & TWI_ERROR_MASK)
#define _TWI_LCD                                           \
    do                                                     \
    {                                                      \
        switch (TWI_STA)                                   \
        {                                                  \
        case TWI_OK:                                       \
            lcd_printf("L%d:ok:%3d", depth, TWI_RET);      \
            break;                                         \
        case TWI_TIMEOUT:                                  \
            lcd_printf("L%d:timeout:%3d", depth, TWI_RET); \
            break;                                         \
        case TWI_ERR:                                      \
            lcd_printf("L%d:error:%3d", depth, TWI_RET);   \
            break;                                         \
        default:                                           \
            lcd_printf("L%d:error unknown", depth);        \
            break;                                         \
        }                                                  \
    } while (0)

#define _TWI_OK if (TWI_STA==TWI_OK)
#define _TWI_TIMEOUT if (TWI_STA==TWI_TIMEOUT)
#define _TWI_ERROR if (TWI_STA==TWI_ERR)
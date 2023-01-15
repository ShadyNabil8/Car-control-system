#define LIGHT_MASK 0b1111
#define OFF_SHIFT 4
#define motor_fail 'F'
#define FR_ON (1 << 0)
#define FL_ON (1 << 1)
#define BR_ON (1 << 2)
#define BL_ON (1 << 3)
#define FR_OFF (FR_ON << OFF_SHIFT)
#define FL_OFF (FL_ON << OFF_SHIFT)
#define BR_OFF (BR_ON << OFF_SHIFT)
#define BL_OFF (BL_ON << OFF_SHIFT)

#define BITRATE 400000
#define MASTER_ADDRESS 1
#define SLAVE_ADDRESS 3

#define LIGHT_PORT PORTA
#define LIGHT_DIR DDRA
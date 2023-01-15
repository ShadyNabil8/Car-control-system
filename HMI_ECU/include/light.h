#define LIGHT_MASK 0b1111
#define OFF_SHIFT 4
#define FR (1 << 0)
#define FL (1 << 1)
#define RR (1 << 2)
#define RL (1 << 3)
#define FAIL (1 << 4)
#define BITRATE 400000
#define MASTER_ADDRESS 1
#define SLAVE_ADDRESS 3

#define LIGHT_PORT PORTA
#define LIGHT_DIR DDRA
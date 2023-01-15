#ifndef _I2C_H_
#define _I2C_H_

#include <stdint.h>
#include "psc.h"
#include <avr/io.h>
#include <util/delay.h>
#include "trash.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>

#ifndef TWI_RETRIES
#define TWI_RETRIES 1000
#endif
#ifndef TWI_DELAY
#define TWI_DELAY 0
#endif

typedef enum i2c_status_t
{
    TW_START = 0x08,        // start has been sent
    TW_REP_START = 0x10,    // repeated start
    TW_MT_SLA_W_ACK = 0x18, // Master transmit ( slave address + Write request ) to slave + Ack received from slave
    TW_MT_SLA_R_ACK = 0x40, // Master transmit ( slave address + Read request ) to slave + Ack received from slave
    TW_MT_DATA_ACK = 0x28,  // Master transmit data and ACK has been received from Slave.
    TW_MR_DATA_ACK = 0x50,  // Master received data and send ACK to slave
    TW_MR_DATA_NACK = 0x58, // Master received data but doesn't send ACK to slave
    TW_SR_DATA_ACK = 0x80,  // Slave recieved data and sent ACK to master
    TW_ST_SLA_ACK = 0xA8,   // Slave recieved (its own address + read request) and returned ACK
    TW_ST_DATA_ACK = 0xB8,  // Slave transmitted data and recieved ACK
    TW_SR_SLA_ACK = 0x60,   // slave recieved (its own address + read request) and returned ACK
    TW_SR_STOP = 0xA0,      // slave recieved stop condition
    TW_STATUS_MASK = 0xF8,  // bit mask for status codes
    TW_MR_STRING = 0xD0,
    TW_MT_STRING = 0xD8,
    TW_SR_STRING = 0xE0,
    TW_ST_STRING = 0xE8,
    TW_BUS_ERROR = 0
} i2c_status_t;

typedef enum i2c_err_t
{
    TWI_OK = 1 << 0,
    TWI_ERR = 1 << 1,
    TWI_TIMEOUT = 1 << 2,
    TWI_ERROR_MASK = 0b111
} i2c_err_t;

typedef enum i2c_mode_t
{
    TWI_master = 2,
    TWI_slave = 1
} i2c_mode_t;

typedef uint16_t i2c_return_t;

#define TW_STATUS (TWSR & TW_STATUS_MASK)

i2c_return_t TWI_init(uint32_t frequency, uint8_t address);
i2c_return_t TWI_check_status(i2c_status_t operation);
i2c_return_t TWI_start();
i2c_return_t TWI_repstart();
i2c_return_t TWI_stop();
void TWI_off();
i2c_return_t TWI_write_req(uint8_t address);
i2c_return_t TWI_read_req(uint8_t address);
i2c_return_t TWI_write_accept(void);
i2c_return_t TWI_read_accept(void);
i2c_return_t TWI_write(i2c_mode_t mode, uint8_t data);
i2c_return_t TWI_read(i2c_mode_t mode, uint8_t *buff);
i2c_return_t TWI_recieve_str(i2c_mode_t mode, ...);
i2c_return_t TWI_recieve_byte(i2c_mode_t mode, ...);
i2c_return_t TWI_send_str(i2c_mode_t mode, ...);
i2c_return_t TWI_send_byte(i2c_mode_t mode, ...);
i2c_return_t TWI_printf(i2c_mode_t mode, char *format, ...);
void TWI_int(void (*on_recieve)(uint8_t), void (*on_request)());
#endif
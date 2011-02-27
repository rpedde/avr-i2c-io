/*
 * i2c I/O device (type 3)
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "i2c-core.h"
#include <mpusb/mpusb.h>

uint8_t i2c_notify_device = 0;
uint8_t i2c_notify_address = 0;

/* someone is writing to me! */
void i2c_handle_write(uint8_t addr, uint8_t len, uint8_t *data) {
    switch(addr) {
    case 0x40:
        // set output mask
        DDRD = *data;
        break;

    case 0x41:
        // set PORTD (and pullups, for output devices)
        PORTD = *data;
        break;

    case 0x43:
        // set input notification device
        i2c_notify_device = *data;
        break;

    case 0x44:
        // set input notification address
        i2c_notify_address = *data;
        break;

    default:
        break;
    }
}

uint8_t i2c_handle_read(uint8_t addr, uint8_t len, uint8_t *data) {
    switch(addr) {
    case 0x01:
        // board type
        *data = I2C_IO;
        break;

    case 0x40:
        // read output mask
        *data = DDRD;
        break;

    case 0x41:
        // read PORTD
        *data = PORTD;
        break;

    case 0x42:
        // read PIND
        *data = PIND;
        break;

    case 0x43:
        // read input notification device
        *data = i2c_notify_device;
        break;

    case 0x44:
        // read input notification address
        *data = i2c_notify_address;
        break;

    default:
        *data = 0;
        break;
    }

    return 1;
}

void i2c_handle_init(void) {
    // Set PORTD to high impedance state
    DDRD=0;
    PORTD=0;
}

void i2c_handle_idle(void) {
}


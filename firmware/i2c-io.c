/*
 * i2c I/O device (type 3)
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "i2c-core.h"
#include "i2c-module.h"
#include <mpusb/mpusb.h>

uint8_t i2c_notify_device = 0;
uint8_t i2c_notify_address = 0;

uint8_t pind_expected;

/* someone is writing to me! */
void i2c_handle_write(uint8_t addr, uint8_t len, uint8_t *data) {
    switch(addr) {
    case 0x40:
        // set output mask
        DDRD = *data;
        pind_expected = PIND;
        break;

    case 0x41:
        // set PORTD (and pullups, for output devices)
        PORTD = *data;
        pind_expected = PIND;
        break;

    case 0x43:
        // set input notification device
        i2c_notify_device = *data;
        eeprom_write_byte((uint8_t*)5, *data);
        break;

    case 0x44:
        // set input notification address
        i2c_notify_address = *data;
        eeprom_write_byte((uint8_t*)6, *data);
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

    pind_expected = PIND;

    i2c_notify_device = eeprom_read_byte((uint8_t*)5);
    i2c_notify_address = eeprom_read_byte((uint8_t*)6);

    if((i2c_notify_device < 3) || (i2c_notify_device > 0xEE))
        i2c_notify_device = 0;
}

void i2c_handle_idle(void) {
    static uint8_t countdown[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    char buffer[3];
    uint8_t index;
    uint8_t rack;

    for(index = 0, rack=1; index < 8; index++, rack << 1) {
        if(!(DDRD & rack)) {
            /* an input pin */
            if((pind_expected & rack) != (PIND & rack)) {
                /* something unexpected! */
                if(countdown[index] == 1) {
                    /* REAL STATE CHANGE! */
                    pind_expected = (pind_expected & ~rack) | (PIND & rack);
                    countdown[index] = 0;

                    if(i2c_notify_device) {
                        buffer[0] = i2c_notify_address;
                        buffer[1] = device_addr;
                        buffer[2] = pind_expected;

                        i2cMasterSendNI(i2c_notify_device << 1, 3, buffer);
                    }
                } else {
                    countdown[index]--;
                }
            } else {
                countdown[index] = 0;
            }
        }
    }
}


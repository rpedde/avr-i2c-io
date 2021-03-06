#include <inttypes.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "i2c-core.h"
#include "i2c-module.h"

#include "i2c.h"
#include "timerx8.h"

#include <mpusb/mpusb.h>

#define FIRMWARE_MAJOR  2
#define FIRMWARE_MINOR  0

uint8_t device_addr=0x20;
uint8_t i2c_addr=0;
uint8_t eeprom_index=0;
uint8_t got_addr=0;

void i2c_recv(uint8_t len, uint8_t *data);
uint8_t i2c_reply(uint8_t len, uint8_t *data);

/* someone is writing to me! */
void i2c_write(uint8_t len, uint8_t *data) {
    uint8_t *in;

    i2c_addr = *data;

    if(len > 1) {
        switch(i2c_addr) {
        case 2:
            // set eeprom index
            eeprom_index = data[1];
            break;
        case 3:
            // write eeprom
            eeprom_write_byte((uint8_t*)(uint16_t)eeprom_index, data[1]);
            eeprom_index++;
            break;
        default:
            i2c_handle_write(i2c_addr, len, &data[1]);
            break;
        }
    }
}

/* someone is reading me! */
uint8_t i2c_read(uint8_t len, uint8_t *data) {
    switch(i2c_addr) {
    case 0: // read magic
        *data = 0xAE;
        break;

        // case 1 handled by module

    case 2:
        // get eeprom index
        *data = eeprom_index;
        break;

    case 3:
        // read eeprom
        *data = eeprom_read_byte((uint8_t*)(uint16_t)eeprom_index);
        eeprom_index++;
        break;

    case 4:
        // get firmware major
        *data = FIRMWARE_MAJOR;
        break;

    case 5:
        // get firmware minor
        *data = FIRMWARE_MINOR;
        break;

    default:
        return i2c_handle_read(i2c_addr, len, data);
        break;
    }

    return 1;
}

int main(void) {

    // Set internal pullups on the AVR (should be settable)
    DDRC &= ~((1U << 4) | (1U << 5));
    PORTC |= ((1U << 4) | (1U << 5));

    timerInit();
    i2cInit();

    DDRC |= (1U << 1);
    PORTC &= ~(1U << 1);

    device_addr = (eeprom_read_byte((uint8_t*)1) << 1);

    if((device_addr < 0x10) || (device_addr > 0xEE))
        device_addr = 0x20;

    i2cSetLocalDeviceAddr(device_addr, TRUE);
    i2cSetBitrate(10);

    i2cSetSlaveReceiveHandler(i2c_write);
    i2cSetSlaveTransmitHandler(i2c_read);

    i2c_handle_init();

    while(1) {
        i2c_handle_idle();
    }
}

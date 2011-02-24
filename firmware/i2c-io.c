#include <inttypes.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

typedef signed char int8;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

#include "i2c.h"
#include "timerx8.h"

uint8_t i2c_buffer[BUFFER_SIZE];
uint8_t i2c_buffer_len=BUFFER_SIZE;
uint8_t i2c_addr=0;

uint8_t got_addr=0;

uint8_t i2c_notify_device = 0;
uint8_t i2c_notify_address = 0;

void i2c_recv(uint8_t len, uint8_t *data);
uint8_t i2c_reply(uint8_t len, uint8_t *data);


/* someone is writing to me! */
void i2c_write(uint8_t len, uint8_t *data) {
    uint8_t *in;

    i2c_addr = *data;

    if(len > 1) {
        in = &data[1];

        switch(i2c_addr) {
        case 0x40:
            // set output mask
            DDRD = *in;
            break;

        case 0x41:
            // set PORTD (and pullups, for output devices)
            PORTD = *in;
            break;

        case 0x42:
            // set input notification device
            i2c_notify_device = *in;
            break;

        case 0x43:
            // set input notification address
            i2c_notify_address = *in;
            break;

        default:
            break;
        }
    }
}

/* someone is reading me! */
uint8_t i2c_read(uint8_t len, uint8_t *data) {
    uint8_t index;

    PORTC ^= (1U << 1);

    //    i2c_buffer[0] = 0xae;
    switch(i2c_addr) {
    case 0:
        *data = 0xAE;
        break;

    case 1:
        *data = 3; // 8-bit IO
        break;

    case 2:
        // get eeprom index
        break;

    case 3:
        // read eeprom
        break;

    case 4:
        // get firmware version ???
        *data = 5;
        break;

    case 5:
        // get proc type
        *data = 2;
        break;

    case 6:
        // get proc speed
        *data = F_CPU/1000000;
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
        // read input notification device
        *data = i2c_notify_device;
        break;

    case 0x43:
        // read input notification address
        *data = i2c_notify_address;
        break;

    default:
        *data = i2c_addr;
        break;
    }

    return 1;
}



int main(void)
{
    uint8_t send_buffer[2];

    // Set internal pullups on the AVR (should be settable)
    DDRC &= ~((1U << 4) | (1U << 5));
    PORTC |= ((1U << 4) | (1U << 5));

    timerInit();
    i2cInit();

    DDRC |= (1U << 1);
    PORTC &= ~(1U << 1);

    i2cSetLocalDeviceAddr(0x20, TRUE);
    i2cSetBitrate(10);

    i2cSetSlaveReceiveHandler(i2c_write);
    i2cSetSlaveTransmitHandler(i2c_read);

    //    timerPause(500);

    while(1) {
    }
}

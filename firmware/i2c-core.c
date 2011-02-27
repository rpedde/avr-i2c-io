#include <inttypes.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "i2c-core.h"
#include "i2c-module.h"

#include "i2c.h"
#include "timerx8.h"

uint8_t i2c_buffer[BUFFER_SIZE];
uint8_t i2c_buffer_len=BUFFER_SIZE;
uint8_t i2c_addr=0;

uint8_t got_addr=0;

void i2c_recv(uint8_t len, uint8_t *data);
uint8_t i2c_reply(uint8_t len, uint8_t *data);


/* someone is writing to me! */
void i2c_write(uint8_t len, uint8_t *data) {
    uint8_t *in;

    i2c_addr = *data;

    if(len > 1) {
        i2c_handle_write(i2c_addr, len, &data[1]);
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

    default:
        return i2c_handle_read(i2c_addr, len, data);
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

    // Set PORTD to high impedance state

    DDRD=0;
    PORTD=0;

    i2cSetLocalDeviceAddr(0x20, TRUE);
    i2cSetBitrate(10);

    i2cSetSlaveReceiveHandler(i2c_write);
    i2cSetSlaveTransmitHandler(i2c_read);

    //    timerPause(500);

    while(1) {
    }
}
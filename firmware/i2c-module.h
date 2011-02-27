#ifndef _I2C_MODULE_H_
#define _I2C_MODULE_H_

extern void i2c_handle_init(void);
extern void i2c_handle_idle(void);
extern void i2c_handle_write(uint8_t addr, uint8_t len, uint8_t *data);
extern uint8_t i2c_handle_read(uint8_t addr, uint8_t len, uint8_t *data);

extern uint8_t device_addr;

#endif /* _I2C_MODULE_H_ */

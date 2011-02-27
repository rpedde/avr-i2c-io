#ifndef _I2C_MODULE_H_
#define _I2C_MODULE_H_

extern i2c_handle_write(uint8_t addr, uint8_t len, uint8_t *data);
extern i2c_handle_read(uint8_t addr, uint8_t len, uint8_t *data);

#endif /* _I2C_MODULE_H_ */

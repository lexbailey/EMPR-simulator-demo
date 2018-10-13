#ifndef I2C_INCLUDE
#define I2C_INCLUDE

#include <stdlib.h>
#include <LPC17xx.h>
#include <lpc17xx_i2c.h>
#include <lpc17xx_pinsel.h>

void init_i2c_pins();
void select_bus(LPC_I2C_TypeDef *bus);
void init_i2c();
I2C_M_SETUP_Type setup_transfer(unsigned char *buf, int tx_length);
void update_transfer_info(I2C_M_SETUP_Type *transfer_cfg, unsigned char *buf, int tx_length);
Status do_transfer_with_cfg(I2C_M_SETUP_Type *transfer_cfg, unsigned char* buf, int length, int address);
Status do_transfer(unsigned char *buf, int length, int address);
Status i2c_transfer(I2C_M_SETUP_Type *transfer_cfg);
Status read_i2c(unsigned char *buf, int length, int address);

#endif /* I2C_INCLUDE */


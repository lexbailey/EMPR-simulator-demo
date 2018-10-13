#include "i2c_helper.h"

LPC_I2C_TypeDef *helper_i2cbus = NULL;

void init_i2c_pins(){
    // Configures I2C pins
    PINSEL_CFG_Type PinCfg;

    PinCfg.Funcnum = 3;
    PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 0;
    PINSEL_ConfigPin(&PinCfg);

    PinCfg.Pinnum = 1;
    PINSEL_ConfigPin(&PinCfg);
}

void select_bus(LPC_I2C_TypeDef *bus){
    helper_i2cbus = bus;
}

void init_i2c(){
    // Initialises I2C bus
    I2C_Init(helper_i2cbus, 100000);
    I2C_Cmd(helper_i2cbus, ENABLE);
}

I2C_M_SETUP_Type setup_read(unsigned char *buf, int rx_length){
    // Initialises I2C transfer type
    I2C_M_SETUP_Type transfer_cfg;

    transfer_cfg.tx_data = NULL;
    transfer_cfg.tx_length = 0;
    transfer_cfg.tx_count = 0;
    transfer_cfg.rx_data = buf;
    transfer_cfg.rx_length = rx_length;
    transfer_cfg.rx_count = 0;
    transfer_cfg.retransmissions_max = 3;
    transfer_cfg.retransmissions_count = 0;

    return transfer_cfg;
}

I2C_M_SETUP_Type setup_transfer(unsigned char *buf, int tx_length){
    // Initialises I2C transfer type
    I2C_M_SETUP_Type transfer_cfg;

    transfer_cfg.tx_data = buf;
    transfer_cfg.tx_length = tx_length;
    transfer_cfg.tx_count = 0;
    transfer_cfg.rx_data = NULL;
    transfer_cfg.rx_length = 0;
    transfer_cfg.rx_count = 0;
    transfer_cfg.retransmissions_max = 3;
    transfer_cfg.retransmissions_count = 0;

    return transfer_cfg;
}

void update_transfer_info(I2C_M_SETUP_Type *transfer_cfg, unsigned char *buf, int tx_length){
    transfer_cfg->tx_data = buf;
    transfer_cfg->tx_length = tx_length;
}

Status do_transfer_with_cfg(I2C_M_SETUP_Type *transfer_cfg, unsigned char* buf, int length, int address){
    update_transfer_info(transfer_cfg, buf, length);
    transfer_cfg->sl_addr7bit = address;
    return I2C_MasterTransferData(helper_i2cbus, transfer_cfg, I2C_TRANSFER_POLLING);
}

Status do_transfer(unsigned char* buf, int length, int address){
    I2C_M_SETUP_Type transfer_cfg = setup_transfer(buf, length);
    transfer_cfg.sl_addr7bit = address;
    return I2C_MasterTransferData(helper_i2cbus, &transfer_cfg, I2C_TRANSFER_POLLING);
}


Status i2c_transfer(I2C_M_SETUP_Type *transfer_cfg){
    return I2C_MasterTransferData(helper_i2cbus, transfer_cfg, I2C_TRANSFER_POLLING);

}

Status read_i2c(unsigned char *buf, int length, int address){
    I2C_M_SETUP_Type transfer_cfg = setup_read(buf, length);
    transfer_cfg.sl_addr7bit = address;
    return I2C_MasterTransferData(helper_i2cbus, &transfer_cfg, I2C_TRANSFER_POLLING);
}


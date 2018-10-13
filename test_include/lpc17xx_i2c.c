#include <lpc17xx_i2c.h>

void send_i2c_bus_name(LPC_I2C_TypeDef *I2Cx){
    char *name = NULL;
    if (I2Cx == LPC_I2C0){ name = "I2C0"; }
    if (I2Cx == LPC_I2C1){ name = "I2C1"; }
    if (I2Cx == LPC_I2C2){ name = "I2C2"; }
    if (name == NULL){
        fprintf(stderr, "Unknown I2C bus used with I2C library\n");
        exit(1);
    }
    test_socket_send(name);
}

void I2C_Init(LPC_I2C_TypeDef *I2Cx, uint32_t clockrate){
    test_socket_send("i2c.init ");
    send_i2c_bus_name(I2Cx);
    char args[20];
    snprintf(args, 20, " %d", clockrate);
    test_socket_send(args);
    test_socket_send("\n");
    free(test_socket_recv());
}

void I2C_DeInit(LPC_I2C_TypeDef* I2Cx){
    test_socket_send("i2c.deinit ");
    send_i2c_bus_name(I2Cx);
    test_socket_send("\n");
    free(test_socket_recv());
}

void I2C_SetClock (LPC_I2C_TypeDef *I2Cx, uint32_t target_clock){
    test_socket_send("i2c.setclock ");
    send_i2c_bus_name(I2Cx);
    char args[20];
    snprintf(args, 20, " %d", target_clock);
    test_socket_send(args);
    test_socket_send("\n");
    free(test_socket_recv());
}

void I2C_Cmd(LPC_I2C_TypeDef* I2Cx, FunctionalState NewState){
    test_socket_send("i2c.cmd ");
    send_i2c_bus_name(I2Cx);
    if (NewState == ENABLE){
        test_socket_send(" enable");
    }
    else{
        test_socket_send(" disable");
    }
    test_socket_send("\n");
    free(test_socket_recv());
}

Status I2C_MasterTransferData(LPC_I2C_TypeDef *I2Cx, I2C_M_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt){
    test_socket_send("i2c.mastertransferdata ");
    send_i2c_bus_name(I2Cx);
    char addr[5];
    snprintf(addr, 5, " %02x ", TransferCfg->sl_addr7bit);
    test_socket_send(addr);
    for (int i = 0; i<= ((int)(TransferCfg->tx_length))-1; i++){
        char data[3];
        snprintf(data, 3, "%02x", TransferCfg->tx_data[i]);
        test_socket_send(data);
    }
    test_socket_send("\n");
    char *reply=(test_socket_recv());
    int replylen = strlen(reply);
    for (int i = 0; i <= ((int)(TransferCfg->rx_length))-1 && i <= replylen-1; i++){
        char reply_byte[3];
        int reply_pos = i*2;
        reply_byte[0] = reply[reply_pos];
        reply_byte[1] = reply[reply_pos+1];
        reply_byte[2] = '\0';
        TransferCfg->rx_data[i] = (char)strtol(reply_byte, NULL, 16);
    }
    free(reply);
    return SUCCESS;
}
Status I2C_SlaveTransferData(LPC_I2C_TypeDef *I2Cx, I2C_S_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt){
    test_socket_send("i2c.slavetransferdata ");
    send_i2c_bus_name(I2Cx);
    test_socket_send("\n");
    char *reply=(test_socket_recv());
    // TODO parse reply, generate return val
    free(reply);
    return SUCCESS;
}


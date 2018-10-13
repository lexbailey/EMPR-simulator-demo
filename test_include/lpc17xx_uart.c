#include <lpc17xx_uart.h>

void UART_Init(LPC_UART_TypeDef *UARTx, UART_CFG_Type *UART_ConfigStruct){
    printf("UART_Init\n");
}

void UART_DeInit(LPC_UART_TypeDef* UARTx){
    printf("UART_DeInit\n");
}

void UART_ConfigStructInit(UART_CFG_Type *UART_InitStruct){
    printf("UART_ConfigStructInit\n");
}


void UART_SendByte(LPC_UART_TypeDef* UARTx, uint8_t Data){
    printf("UART_SendByte\n");
}

uint8_t UART_ReceiveByte(LPC_UART_TypeDef* UARTx){
    printf("UART_ReceiveByte\n");
    return 0;
}

uint32_t UART_Send(LPC_UART_TypeDef *UARTx, uint8_t *txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag){
    printf("UART_Send\n");
    return 0;
}

uint32_t UART_Receive(LPC_UART_TypeDef *UARTx, uint8_t *rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag){
    printf("UART_Receive\n");
    return 0;
}


void UART_FIFOConfig(LPC_UART_TypeDef *UARTx, UART_FIFO_CFG_Type *FIFOCfg){
    printf("UART_FIFOConfig\n");
}

void UART_FIFOConfigStructInit(UART_FIFO_CFG_Type *UART_FIFOInitStruct){
    printf("UART_FIFOCongifStructInit\n");
}


void UART_TxCmd(LPC_UART_TypeDef *UARTx, FunctionalState NewState){
    printf("UART_TxCmd\n");
}


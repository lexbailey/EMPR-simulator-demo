#include "testing_only.h"
#ifndef LPC_UART_INCLUDE
#define LPC_UART_INCLUDE

#include <LPC17xx.h>

#include <stdint.h>
#include <stdio.h>

typedef enum {
    UART_DATABIT_5 = 0,
    UART_DATABIT_6,
    UART_DATABIT_7,
    UART_DATABIT_8
} UART_DATABIT_Type;

typedef enum {
    UART_STOPBIT_1 = 0,
    UART_STOPBIT_2,
} UART_STOPBIT_Type;

typedef enum {
    UART_PARITY_NONE = 0,
    UART_PARITY_ODD,
    UART_PARITY_EVEN,
    UART_PARITY_SP_1,
    UART_PARITY_SP_0
} UART_PARITY_Type;

typedef struct {
    uint32_t Baud_rate;
    UART_PARITY_Type Parity;
    UART_DATABIT_Type Databits;
    UART_STOPBIT_Type Stopbits;
} UART_CFG_Type;

typedef enum
{
    NONE_BLOCKING = 0,
    BLOCKING,
} TRANSFER_BLOCK_Type;

typedef enum {
    UART_FIFO_TRGLEV0 = 0,
    UART_FIFO_TRGLEV1,
    UART_FIFO_TRGLEV2,
    UART_FIFO_TRGLEV3
} UART_FITO_LEVEL_Type;

typedef struct {
    FunctionalState FIFO_ResetRxBuf;
    FunctionalState FIFO_ResetTxBuf;
    FunctionalState FIFO_DMAMode;
    UART_FITO_LEVEL_Type FIFO_Level;
} UART_FIFO_CFG_Type;

LPC_UART_TypeDef mock_uart0;
#define LPC_UART0 (&mock_uart0)

void UART_Init(LPC_UART_TypeDef *UARTx, UART_CFG_Type *UART_ConfigStruct);
void UART_DeInit(LPC_UART_TypeDef* UARTx);
void UART_ConfigStructInit(UART_CFG_Type *UART_InitStruct);

void UART_SendByte(LPC_UART_TypeDef* UARTx, uint8_t Data);
uint8_t UART_ReceiveByte(LPC_UART_TypeDef* UARTx);
uint32_t UART_Send(LPC_UART_TypeDef *UARTx, uint8_t *txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag);
uint32_t UART_Receive(LPC_UART_TypeDef *UARTx, uint8_t *rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag);

void UART_FIFOConfig(LPC_UART_TypeDef *UARTx, UART_FIFO_CFG_Type *FIFOCfg);
void UART_FIFOConfigStructInit(UART_FIFO_CFG_Type *UART_FIFOInitStruct);

void UART_TxCmd(LPC_UART_TypeDef *UARTx, FunctionalState NewState);

#endif // LPC_UART_INCLUDE

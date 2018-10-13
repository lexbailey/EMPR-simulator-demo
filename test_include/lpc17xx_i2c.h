#include "testing_only.h"
#ifndef LPC_I2C_INCLUDE
#define LPC_I2C_INCLUDE

#include <LPC17xx.h>
#include <lpc_types.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint32_t sl_addr7bit;
  uint8_t* tx_data;
  uint32_t tx_length;
  uint32_t tx_count;
  uint8_t* rx_data;
  uint32_t rx_length;
  uint32_t rx_count;
  uint32_t retransmissions_max;
  uint32_t retransmissions_count;
  uint32_t status;
  void (*callback)(void);
} I2C_M_SETUP_Type;

typedef struct {
    uint8_t* tx_data;
    uint32_t tx_length;
    uint32_t tx_count;
    uint8_t* rx_data;
    uint32_t rx_length;
    uint32_t rx_count;
    uint32_t status;
    void(*callback)(void);
} I2C_S_SETUP_Type;

typedef enum {
    I2C_TRANSFER_POLLING = 0,
    I2C_TRANSFER_INTERRUPT
} I2C_TRANSFER_OPT_Type;

LPC_I2C_TypeDef mock_i2c0;
LPC_I2C_TypeDef mock_i2c1;
LPC_I2C_TypeDef mock_i2c2;
#define LPC_I2C0 (&mock_i2c0)
#define LPC_I2C1 (&mock_i2c1)
#define LPC_I2C2 (&mock_i2c2)

void I2C_Init(LPC_I2C_TypeDef *I2Cx, uint32_t clockrate);
void I2C_DeInit(LPC_I2C_TypeDef* I2Cx);
void I2C_SetClock (LPC_I2C_TypeDef *I2Cx, uint32_t target_clock);
void I2C_Cmd(LPC_I2C_TypeDef* I2Cx, FunctionalState NewState);

Status I2C_MasterTransferData(LPC_I2C_TypeDef *I2Cx, I2C_M_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt);
Status I2C_SlaveTransferData(LPC_I2C_TypeDef *I2Cx, I2C_S_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt);

#endif // LPC_I2C_INCLUDE

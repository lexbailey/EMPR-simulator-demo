#include "testing_only.h"
#ifndef LPC17xx_testing_include
#define LPC17xx_testing_include

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

typedef struct {
    /**/
} LPC_I2C_TypeDef;

typedef struct {
    uint32_t IR;
    uint32_t TCR;
    uint32_t TC;
    uint32_t PR;
    uint32_t PC;
    uint32_t MCR;
    uint32_t MR0;
    uint32_t MR1;
    uint32_t MR2;
    uint32_t MR3;
    uint32_t CCR;
    uint32_t CR0;
    uint32_t CR1;
    uint32_t RESERVED0[2];
    uint32_t EMR;
    uint32_t RESERVED1[12];
    uint32_t CTCR;
} LPC_TIM_TypeDef;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

typedef struct
{
    union {
        uint8_t  RBR; 
        uint8_t  THR; 
        uint8_t  DLL; 
        uint32_t RESERVED0;
    };
    union {
        uint8_t  DLM; 
        uint32_t IER; 
    };
    union {
        uint32_t IIR; 
        uint8_t  FCR; 
    };
    uint8_t  LCR; 
    uint8_t  RESERVED1[7];
    uint8_t  LSR; 
    uint8_t  RESERVED2[7];
    uint8_t  SCR; 
    uint8_t  RESERVED3[3];
    uint32_t ACR; 
    uint8_t  ICR; 
    uint8_t  RESERVED4[3];
    uint8_t  FDR; 
    uint8_t  RESERVED5[7];
    uint8_t  TER; 
    uint8_t  RESERVED6[39];
    uint8_t  FIFOLVL;
} LPC_UART_TypeDef;

// Test environment stuff...
struct sockaddr_un test_socket_addr;
int test_socket_fd,test_socket_rc;
void init_test_env();
void test_socket_send(char *str);
char *test_socket_recv();

#endif //LPC17xx_testing_include

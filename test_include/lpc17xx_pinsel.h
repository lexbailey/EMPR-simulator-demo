#include "testing_only.h"
#ifndef LPC_PINSEL_INCLUDE
#define LPC_PINSEL_INCLUDE

#include <stdint.h>
#include <stdio.h>
#include <LPC17xx.h>

#define PINSEL_PINMODE_PULLUP (0)
#define PINSEL_PINMODE_TRISTATE (2)
#define PINSEL_PINMODE_PULLDOWN (3)

#define PINSEL_PINMODE_NORMAL (0)
#define PINSEL_PINMODE_OPENDRAIN (1)

#define PINSEL_I2C_Normal_Mode (0)
#define PINSEL_I2C_Fast_Mode (1)

typedef struct
{
    uint8_t Portnum;
    uint8_t Pinnum;
    uint8_t Funcnum;
    uint8_t Pinmode;
    uint8_t OpenDrain;
} PINSEL_CFG_Type;

void PINSEL_ConfigPin(PINSEL_CFG_Type *PinCfg);

#endif // LPC_PINSEL_INCLUDE

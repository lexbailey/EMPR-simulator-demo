#include "testing_only.h"
#ifndef LPC_GPIO_INCLUDE
#define LPC_GPIO_INCLUDE

#include <stdint.h>
#include <stdio.h>

void GPIO_SetDir(uint8_t portNum, uint32_t bitValue, uint8_t dir);
void GPIO_SetValue(uint8_t portNum, uint32_t bitValue);
void GPIO_ClearValue(uint8_t portNum, uint32_t bitValue);
uint32_t GPIO_ReadValue(uint8_t portNum);

#endif // LPC_GPIO_INCLUDE

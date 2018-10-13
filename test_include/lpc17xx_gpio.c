#include <lpc17xx_gpio.h>

void GPIO_SetDir(uint8_t portNum, uint32_t bitValue, uint8_t dir){
    printf("GPIO_SetDir\n");
}

void GPIO_SetValue(uint8_t portNum, uint32_t bitValue){
    printf("GPIO_SetValue\n");
}

void GPIO_ClearValue(uint8_t portNum, uint32_t bitValue){
    printf("GPIO_ClearValue\n");
}

uint32_t GPIO_ReadValue(uint8_t portNum){
    printf("GPIO_ReadValue\n");
    return 0;
}

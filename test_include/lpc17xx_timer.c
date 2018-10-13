#include <lpc17xx_timer.h>

void TIM_Init(LPC_TIM_TypeDef *TIMx, TIM_MODE_OPT TimerCounterMode, void *TIM_ConfigStruct){
    printf("TIM_Init\n");
}

void TIM_Cmd(LPC_TIM_TypeDef *TIMx, FunctionalState NewState){
    printf("TIM_Cmd\n");
}

void TIM_ResetCounter(LPC_TIM_TypeDef *TIMx){
    printf("TIM_ResetCounter\n");
}


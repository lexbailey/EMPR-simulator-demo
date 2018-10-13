#include "testing_only.h"
#ifndef LPC_TIMER_INCLUDE
#define LPC_TIMER_INCLUDE

#include <stdint.h>
#include <stdio.h>
#include <LPC17xx.h>

typedef enum {
    TIM_PRESCALE_TICKVAL = 0,
    TIM_PRESCALE_USVAL
} TIM_PRESCALE_OPT;

typedef struct {
    uint8_t PrescaleOption;
    uint8_t Reserved[3];
    uint32_t PrescaleValue;
} TIM_TIMERCFG_Type;

typedef enum {
    TIM_TIMER_MODE = 0,
    TIM_COUNTER_RISING_MODE,
    TIM_COUNTER_FALLING_MODE,
    TIM_COUNTER_ANY_MODE
} TIM_MODE_OPT;

LPC_TIM_TypeDef mock_timer0;
#define LPC_TIM0 (&mock_timer0)

void TIM_Init(LPC_TIM_TypeDef *TIMx, TIM_MODE_OPT TimerCounterMode, void *TIM_ConfigStruct);

void TIM_Cmd(LPC_TIM_TypeDef *TIMx, FunctionalState NewState);

void TIM_ResetCounter(LPC_TIM_TypeDef *TIMx);

#endif // LPC_TIMER_INCLUDE

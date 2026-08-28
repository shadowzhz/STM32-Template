#include "stm32f1xx_it.h"

extern void HAL_IncTick(void);

/******************************************************************************/
/* Cortex-M3 处理器异常                                                       */
/******************************************************************************/

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    while (1)
    {
    }
}

void BusFault_Handler(void)
{
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

/******************************************************************************/
/* 外设中断                                                                   */
/******************************************************************************/

/**
  * @brief  HAL 时基: HAL_Init() 默认把 SysTick 配成 1ms 中断,
  *         这里必须递增 uwTick, HAL_Delay() 才能工作
  */
void SysTick_Handler(void)
{
    HAL_IncTick();
}

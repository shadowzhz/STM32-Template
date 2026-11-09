#include "stm32f10x_it.h"

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

void SysTick_Handler(void)
{
}

/******************************************************************************/
/* STM32F10x 外设中断服务函数                                                */
/* 用户编写的中断服务函数可在此处定义，例如:                                 */
/* void USART1_IRQHandler(void) { ... }                                       */
/******************************************************************************/

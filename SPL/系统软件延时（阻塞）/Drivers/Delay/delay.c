#include "stm32f10x.h"
#include "delay.h"

/* 定义全局变量 ms */
volatile uint32_t ms = 0;

void Delay_Init(void)
{
    /* 配置硬件定时器，否则 ms 将不会自增 */
    SysTick_Config(SystemCoreClock / 1000);
}

void Delay_ms(uint32_t xms)
{
    uint32_t start = ms;

    /* 不能用 <= , 否则就会多等待 1ms */
    while(ms - start < xms)
    {

    }
}

/* 中断服务函数 SysTick_Handler ，定义在启动文件的中断向量表里 */
void SysTick_Handler(void)
{
    ms++;
}

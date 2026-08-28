#include "stm32f10x.h"

/* 定义全局变量 ms */
volatile uint32_t ms = 0;

/* 中断服务函数 SysTick_Handler ，定义在启动文件的中断向量表里 */
void SysTick_Handler(void)
{
    ms++;
}

/* LED 翻转函数 */
void Led_Toggle(void)
{   
    /* 如果读取到低电平，就输出高电平 */
    /* 建议使用 GPIO_ReadOutputDataBit 而不是 GPIO_ReadInputDataBit，虽然对于 LED 来说没什么区别，但这个函数的重点是读取上一次的输出状态 */
    if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == 0)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
    /* 否则相反 */
    else
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    }
}

int main(void)
{
    /* 配置硬件定时器，否则 ms 将不会自增 */
    SysTick_Config(SystemCoreClock / 1000);

    /* 定义结构体 */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 开启挂载在 APB2 总线上的 GPIOC 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* 初始化 LED 为熄灭状态 */
    GPIO_SetBits(GPIOC, GPIO_Pin_13);

    /* 定义上次时间戳 */
    uint32_t last = 0;
    
    while (1)
    {
        /* 如果过去的时间和上次的时间戳相差 500ms 以上，重置时间戳并翻转电平 */
        if(ms - last >= 500)
        {
            last = ms;
            Led_Toggle();
        }
    }
}

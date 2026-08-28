#include "stm32f10x.h"
#include "led.h"

void LED_Init(void)
{
    /* 定义结构体 */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 开启挂载在 APB2 总线上的 GPIOC 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    /* 初始化 LED 为熄灭状态 */
    GPIO_SetBits(LED_PORT, LED_PIN);
}

void LED_OFF(void)
{
    GPIO_WriteBit(LED_PORT, LED_PIN, SET);
}

void LED_ON(void)
{
    GPIO_WriteBit(LED_PORT, LED_PIN, RESET);
}

void LED_Toggle(void)
{
    if(GPIO_ReadOutputDataBit(LED_PORT, LED_PIN) == 0)
    {
        GPIO_WriteBit(LED_PORT, LED_PIN, SET);
    }
    else
    {
        GPIO_WriteBit(LED_PORT, LED_PIN, RESET);
    }
}

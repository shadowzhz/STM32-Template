#include "stm32f10x.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    Delay_Init();
    LED_Init();

    while (1)
    {
        LED_Toggle();  // LED 翻转电平
        Delay_ms(500);  // 延时 500 ms
    }
}

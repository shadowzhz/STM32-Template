#include "stm32f10x_it.h"

/******************************************************************************/
/* Cortex-M3 处理器异常                                                       */
/******************************************************************************/

void NMI_Handler(void)
{
}

/**
  * @brief  硬件故障异常（HardFault）处理程序
  * @note   【为什么会死在这里？四大常见死因自查清单】
  *         1. 空指针 / 野指针：解引用了 NULL 或未初始化的指针。
  *         2. 栈溢出（Stack Overflow）：函数内局部大数组（如 char buf[2048]）超出预留栈空间踩坏内存。
  *         3. 数组越界：写缓冲区超出分配范围，踩坏了函数的返回地址。
  *         4. 访问未开时钟的外设：在未使能 RCC APB1/APB2 时钟时强行操作外设寄存器。
  *         【调试定位技巧】：在 GDB / VSCode 调试模式下，停在死循环后查看 Call Stack（调用栈）定位上一级函数与行号。
  */
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
/* 用户编写的中断服务函数可在此处定义（需使用时直接取消注释即可）：           */
/******************************************************************************/

/*
// 1. 串口 1 接收中断服务函数
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint16_t ch = USART_ReceiveData(USART1);
        // 处理接收到的字符 ch...
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
*/

/*
// 2. 定时器 2 周期更新中断服务函数（1ms/10ms 周期任务）
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // 定时周期任务处理逻辑...
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 必须手动清除中断挂起标志位！
    }
}
*/

/*
// 3. 外部中断线 0（如 PA0 / PB0 按键）
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        // 按键触发处理逻辑...
        EXTI_ClearITPendingBit(EXTI_Line0); // 必须手动清除中断标志位！
    }
}
*/

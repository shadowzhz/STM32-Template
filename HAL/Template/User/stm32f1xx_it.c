#include "stm32f1xx_it.h"

extern void HAL_IncTick(void);

/******************************************************************************/
/* Cortex-M3 处理器异常                                                       */
/******************************************************************************/

void NMI_Handler(void)
{
}

/**
  * @brief  硬件故障异常（HardFault）处理程序
  * @note   【为什么会死在这里？四大常见死因自查清单】
  *         1. 空指针 / 野指针：解引用了 NULL、未初始化的指针或未初始化的外设句柄（如 huart1.Instance 未赋值）。
  *         2. 栈溢出（Stack Overflow）：局部数组过大（如在函数内部定义了 char buf[2048]，链接脚本默认栈仅 1KB），发生内存踩踏。
  *         3. 数组越界：写缓冲区超出分配范围，踩坏了函数的返回地址。
  *         4. 访问未开时钟的外设：在未使能 RCC 时钟时强行操作某些外设寄存器。
  *         【调试定位技巧】：在 GDB / VSCode 调试模式下，停在死循环后查看 Call Stack（调用栈）定位上一级函数与行号；
  *         或在 Watch 窗口查看内核寄存器 SCB->CFSR、SCB->HFSR 确认故障根因。
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

/* ==============================================================================
 * 外设中断处理流转机制说明：
 *
 * 硬件发生中断（如串口接收到数据）
 *   └─> USART1_IRQHandler()               [本文件：名字来自启动文件向量表，仅负责调用 HAL 库接口]
 *        └─> HAL_UART_IRQHandler(&huart1) [HAL 驱动库：判断事件源、清除硬件中断标志]
 *             └─> HAL_UART_RxCpltCallback(huart) [你的业务代码：弱定义回调，收完数据在此处理]
 *
 * 【常用外设中断服务函数模板（需使用时直接取消注释即可）】：
 * ============================================================================== */

/*
// 1. 串口 1 中断服务函数（接收字符、空闲中断等）
extern UART_HandleTypeDef huart1;
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}
*/

/*
// 2. 通用定时器 2 中断服务函数（1ms/10ms 周期任务）
extern TIM_HandleTypeDef htim2;
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}
*/

/*
// 3. 外部引脚中断（如 PA0 / PB0 按键）
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
*/

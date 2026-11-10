/**
  ******************************************************************************
  * @file    stm32f1xx_hal_gpio.h
  * @author  MCD Application Team
  * @brief   HAL库 GPIO 模块头文件
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) 2016 STMicroelectronics。
  * 保留所有权利。
  *
  * 本软件根据可以在此软件组件的根目录中找到的 LICENSE 文件中的条款进行许可。
  * 如果此软件没有附带 LICENSE 文件，则按原样提供。
  *
  ******************************************************************************
  */

/* 定义防止递归 -------------------------------------*/
#ifndef STM32F1xx_HAL_GPIO_H
#define STM32F1xx_HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup GPIO
  * @{
  */

/* 导出类型 ------------------------------------------------------------*/
/** @defgroup GPIO_Exported_Types GPIO 导出类型
  * @{
  */

/**
  * @brief GPIO 初始化结构体定义
  */
typedef struct
{
  uint32_t Pin;       /*!< 指定待配置的 GPIO 引脚。
                           参数取值：@ref GPIO_pins_define 中的单引脚或多个引脚的按位或 (|)。
                           @warning 【引脚复用与特殊引脚避坑】
                           - PC13/PC14/PC15：属于备份电源域（侵入检测/RTC），内部供电切换开关阻抗较大，
                             灌电流/拉电流仅约 3mA，输出频率严禁超过 2MHz，不能直接驱动高负载 LED 或蜂鸣器。
                           - PA13/PA14：默认作为 SWD 调试口（SWDIO/SWCLK），切勿配置为普通 GPIO，否则将导致调试器断开！
                           - PA15/PB3/PB4：上电默认作为 JTAG 引脚，若需作为普通 IO，必须先开启 AFIO 时钟并
                             调用 __HAL_AFIO_REMAP_SWJ_NOJTAG() 释放，严禁调用 SWJ_DISABLE。 */

  uint32_t Mode;      /*!< 指定所选引脚的工作模式。
                           参数取值：@ref GPIO_mode_define 中的枚举值。
                           - 输入模式：输入浮空、上拉输入、下拉输入、模拟输入
                           - 输出模式：推挽输出、开漏输出（I2C/单总线/多机通信必须使用开漏并外接上拉）
                           - 复用模式：复用推挽、复用开漏（串口 TX、SPI、PWM 输出等必须配置为复用模式）
                           - 外部中断/事件模式：上升沿、下降沿或双边沿触发 */

  uint32_t Pull;      /*!< 指定所选引脚的内部上拉或下拉电阻配置。
                           参数取值：@ref GPIO_pull_define（GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN）。
                           @note STM32 芯片内部弱上下拉电阻阻值约为 30kΩ~50kΩ。对于高速通信或强驱动需求
                                 （如 I2C 400kHz），内部上拉不足以满足上升沿要求，必须在外部硬件 PCB 上接上拉电阻。 */

  uint32_t Speed;     /*!< 指定所选输出引脚的翻转速度 / 压摆率。
                           参数取值：@ref GPIO_speed_define。
                           @note STM32F1 系列硬件仅支持三档速度：
                           - GPIO_SPEED_FREQ_LOW    (2MHz)  : 适合按键检测、低速LED，功耗低、EMI辐射最小
                           - GPIO_SPEED_FREQ_MEDIUM (10MHz) : 适合中速信号
                           - GPIO_SPEED_FREQ_HIGH   (50MHz) : 适合高频SPI、PWM输出、高速串口通信
                           @warning STM32F1 没有 F4/F7 系列中的 VERY_HIGH 档位。 */
} GPIO_InitTypeDef;

/**
  * @brief  GPIO Bit SET 和 Bit RESET 状态枚举
  */
typedef enum
{
  GPIO_PIN_RESET = 0u,
  GPIO_PIN_SET
} GPIO_PinState;
/**
  * @}
  */

/* 导出常量 --------------------------------------------------------*/

/** @defgroup GPIO_Exported_Constants GPIO 导出常量
  * @{
  */

/** @defgroup GPIO_pins_define GPIO 引脚定义
  * @{
  */
#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* 引脚 0    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* 引脚 1    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* 引脚 2    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* 引脚 3    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* 引脚 4    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* 引脚 5    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* 引脚 6    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* 引脚 7    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* 引脚 8    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* 引脚 9    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* 引脚 10   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* 引脚 11   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* 引脚 12   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* 引脚 13   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* 引脚 14   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* 引脚 15   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* 全部引脚 */

#define GPIO_PIN_MASK              0x0000FFFFu /* 用于断言测试的引脚掩码 */
/**
  * @}
  */

/** @defgroup GPIO_mode_define GPIO 模式定义
  * @brief GPIO 配置模式
  *        元素值约定: 0xX0yz00YZ
  *           - X  : GPIO 模式或者EXTI模式
  *           - y  : 外部中断 或 事件触发检测
  *           - z  : 外部中断或事件的IO配置
  *           - Y  : 输出类型(推挽/开漏)
  *           - Z  : IO方向模式(输入、输出、复用或模拟)
  * @{
  */
/** @brief 浮空输入模式（引脚输入电平完全由外部信号决定） */
#define  GPIO_MODE_INPUT                        0x00000000u

/** @brief 通用推挽输出模式（强驱动高/低电平，适合驱动 LED、蜂鸣器、普通控制信号） */
#define  GPIO_MODE_OUTPUT_PP                    0x00000001u

/** @brief 通用开漏输出模式（只能主动拉低到 GND，高电平依赖外部上拉电阻；适合总线或电平匹配） */
#define  GPIO_MODE_OUTPUT_OD                    0x00000011u

/** @brief 复用推挽输出模式（引脚由片上外设硬件直接控制，如 USART TX、SPI SCK/MOSI、TIM PWM） */
#define  GPIO_MODE_AF_PP                        0x00000002u

/** @brief 复用开漏输出模式（引脚由片上外设开漏驱动，如 I2C SCL/SDA，外部必须接上拉电阻） */
#define  GPIO_MODE_AF_OD                        0x00000012u

/** @brief 输入复用模式 */
#define  GPIO_MODE_AF_INPUT                     GPIO_MODE_INPUT

/** @brief 模拟模式（关闭施密特触发器和上下拉以彻底杜绝漏电流，专用于 ADC 采样输入或 DAC 输出） */
#define  GPIO_MODE_ANALOG                       0x00000003u

/** @brief 外部中断模式：上升沿触发 */
#define  GPIO_MODE_IT_RISING                    0x10110000u

/** @brief 外部中断模式：下降沿触发 */
#define  GPIO_MODE_IT_FALLING                   0x10210000u

/** @brief 外部中断模式：上升/下降双边沿触发 */
#define  GPIO_MODE_IT_RISING_FALLING            0x10310000u

/** @brief 外部事件模式：上升沿触发 */
#define  GPIO_MODE_EVT_RISING                   0x10120000u

/** @brief 外部事件模式：下降沿触发 */
#define  GPIO_MODE_EVT_FALLING                  0x10220000u

/** @brief 外部事件模式：上升/下降双边沿触发 */
#define  GPIO_MODE_EVT_RISING_FALLING           0x10320000u

/**
  * @}
  */

/** @defgroup GPIO_speed_define  GPIO 速度定义
  * @brief GPIO 输出最大频率
  * @{
  */
/**
  * @brief  GPIO 输出翻转速度：低速 2MHz
  * @note   适合按键、低频 LED 驱动等，功耗最低、EMI 电磁辐射与噪声最小。
  */
#define  GPIO_SPEED_FREQ_LOW              (GPIO_CRL_MODE0_1)

/**
  * @brief  GPIO 输出翻转速度：中速 10MHz
  * @note   适合中速外设信号传输。
  */
#define  GPIO_SPEED_FREQ_MEDIUM           (GPIO_CRL_MODE0_0)

/**
  * @brief  GPIO 输出翻转速度：高速 50MHz
  * @note   适合高频 SPI、PWM 输出、高速串口通信等高压摆率场景。
  */
#define  GPIO_SPEED_FREQ_HIGH             (GPIO_CRL_MODE0)

/**
  * @}
  */

/** @defgroup GPIO_pull_define GPIO 上下拉定义
  * @brief GPIO 上拉/下拉电阻配置
  * @{
  */
/** @brief 无内部上下拉（引脚浮空） */
#define  GPIO_NOPULL        0x00000000u

/**
  * @brief  使能内部弱上拉电阻（阻值约 30kΩ~50kΩ）
  * @note   常用于按键接地输入检测（按下为低电平）、防引脚悬空引入杂散干扰。
  */
#define  GPIO_PULLUP        0x00000001u

/**
  * @brief  使能内部弱下拉电阻（阻值约 30kΩ~50kΩ）
  * @note   常用于高电平有效传感器输入检测。
  */
#define  GPIO_PULLDOWN      0x00000002u
/**
  * @}
  */

/**
  * @}
  */

/* 导出宏 ------------------------------------------------------------*/
/** @defgroup GPIO_Exported_Macros GPIO 导出宏
  * @{
  */

/**
  * @brief 检查是否设置了指定的EXTI线标志。
  * @param  __EXTI_LINE__: 指定需要检测的EXTI线标志.
  *         参数可选 GPIO_PIN_x  x取值(0..15)
  * @retval __EXTI_LINE__的状态 (SET 或 RESET).
  */
#define __HAL_GPIO_EXTI_GET_FLAG(__EXTI_LINE__) (EXTI->PR & (__EXTI_LINE__))

/**
  * @brief  清除EXTI线挂起的标志.
  * @param  __EXTI_LINE__: 指定需要清除的EXTI线标志.
  *         该参数可以是GPIO_PIN_x的任意组合，其中x可以是(0..15)
  * @retval None
  */
#define __HAL_GPIO_EXTI_CLEAR_FLAG(__EXTI_LINE__) (EXTI->PR = (__EXTI_LINE__))

/**
  * @brief  检查指定的EXTI线中断是否被置位。
  * @param  __EXTI_LINE__: 指定需要被检查的EXTI线.
  *          参数可选 GPIO_PIN_x  x取值(0..15)
  * @retval __EXTI_LINE__的状态 (SET 或 RESET).
  */
#define __HAL_GPIO_EXTI_GET_IT(__EXTI_LINE__) (EXTI->PR & (__EXTI_LINE__))

/**
  * @brief  清除EXTI线的中断挂起位.
  * @param  __EXTI_LINE__: 指定需要清除的EXTI线.
  *          该参数可以是GPIO_PIN_x的任意组合，其中x可以是(0..15)
  * @retval None
  */
#define __HAL_GPIO_EXTI_CLEAR_IT(__EXTI_LINE__) (EXTI->PR = (__EXTI_LINE__))

/**
  * @brief  在选定的EXTI线上生成软件中断。
  * @param  __EXTI_LINE__: 指定EXTI线
  *          参数可选 GPIO_PIN_x  x取值(0..15)
  * @retval None
  */
#define __HAL_GPIO_EXTI_GENERATE_SWIT(__EXTI_LINE__) (EXTI->SWIER |= (__EXTI_LINE__))
/**
  * @}
  */

/* 引入GPIO扩展库 */
#include "stm32f1xx_hal_gpio_ex.h"

/* 导出函数 --------------------------------------------------------*/
/** @addtogroup GPIO_Exported_Functions
  * @{
  */

/** @addtogroup GPIO_Exported_Functions_Group1
  * @{
  */
/* 初始化和反初始化函数 *****************************/

/**
  * @brief  根据 GPIO_Init 中指定的参数初始化 GPIOx 外设端口
  * @param  GPIOx: GPIO 端口基地址指针，x 可以是 (A..G)
  * @param  GPIO_Init: 指向包含目标引脚配置信息的 GPIO_InitTypeDef 结构体指针
  * @note   配置涵盖引脚的工作模式、上下拉电阻、最大翻转速度；若配置为 EXTI 外部中断模式，还会同步配置中断触发源。
  * @warning 【外设时钟先决条件 / 避坑警告】
  *          在调用本函数之前，【必须先使能对应端口的 RCC 外设时钟】！
  *          例如：__HAL_RCC_GPIOA_CLK_ENABLE()、__HAL_RCC_GPIOC_CLK_ENABLE()。
  *          若时钟未使能，访问 GPIO 寄存器将静默失败（写入被忽略、读出恒为0），MCU 不会触发 HardFault，引脚将完全不工作！
  *          若引脚涉及复用重映射或 EXTI 外部中断配置，还必须开启 AFIO 时钟：__HAL_RCC_AFIO_CLK_ENABLE()。
  * @retval None
  */
void  HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init);
void  HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin);
/**
  * @}
  */

/** @addtogroup GPIO_Exported_Functions_Group2
  * @{
  */
/* IO操作函数 *****************************************************/

/**
  * @brief  读取指定 GPIO 引脚的当前输入电平状态
  * @param  GPIOx: GPIO 端口基地址（GPIOA..GPIOG）
  * @param  GPIO_Pin: 目标引脚号（例如 GPIO_PIN_0）
  * @note   【按键检测实战提示】
  *         若引脚配置为上拉输入（GPIO_PULLUP，按键未按下时内部上拉为高电平，按键闭合接地）：
  *         按键被按下时，本函数将返回 GPIO_PIN_RESET（低电平 0）！
  *         标准按键判断示例：
  *         @code
  *         if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET) {
  *             HAL_Delay(20); // 简单软件消抖 20ms
  *             if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET) {
  *                 // 确认按键按下处理逻辑...
  *             }
  *         }
  *         @endcode
  * @retval GPIO_PIN_SET（高电平 1）或 GPIO_PIN_RESET（低电平 0）
  */
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/**
  * @brief  设置指定 GPIO 引脚的输出电平状态（置高或拉低）
  * @param  GPIOx: GPIO 端口基地址（GPIOA..GPIOG）
  * @param  GPIO_Pin: 目标引脚（可使用 | 同时操作同一端口的多个引脚，如 GPIO_PIN_0 | GPIO_PIN_1）
  * @param  PinState: 目标电平状态，取值为：
  *                   - GPIO_PIN_SET   : 输出高电平（3.3V）
  *                   - GPIO_PIN_RESET : 输出低电平（0V / GND）
  * @note   【Blue Pill 板载 LED 避坑】
  *         STM32F103C8T6 核心板上 PC13 板载指示灯通常为**低电平点亮**（灌电流驱动）：
  *         - HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // 点亮 LED
  *         - HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   // 熄灭 LED
  * @retval None
  */
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);

/**
  * @brief  翻转指定 GPIO 引脚当前的输出电平（高变低，低变高）
  * @param  GPIOx: GPIO 端口基地址（GPIOA..GPIOG）
  * @param  GPIO_Pin: 目标引脚号
  * @note   常用于心跳指示灯闪烁、测试方波生成等，无需在软件中维护引脚历史状态变量。
  * @retval None
  */
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);

/**
  * @brief  GPIO 外部中断线（EXTI）触发时的弱定义回调函数
  * @param  GPIO_Pin: 触发当前中断的引脚编号（如 GPIO_PIN_0）
  * @note   【多按键区分与处理规范】
  *         所有 GPIO 引脚的外部中断最终都会分发至本函数中。当工程有多个按键中断时，需使用 if 语句判断引脚号：
  *         @code
  *         void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  *             if (GPIO_Pin == KEY1_Pin) {
  *                 // 处理按键 1 逻辑
  *             } else if (GPIO_Pin == KEY2_Pin) {
  *                 // 处理按键 2 逻辑
  *             }
  *         }
  *         @endcode
  * @warning 【中断服务铁律】
  *          本函数运行于中断上下文！严禁在此回调中调用 HAL_Delay() 或进行耗时阻塞操作。
  *          推荐仅设置标志位后立即退出，在 main() 主循环中轮询执行具体业务。
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/**
  * @}
  */

/**
  * @}
  */
/* 私有类型 -------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/
/* 私有常量 ---------------------------------------------------------*/
/** @defgroup GPIO_Private_Constants GPIO 私有常量
  * @{
  */

/**
  * @}
  */

/* 私有宏 ------------------------------------------------------------*/
/** @defgroup GPIO_Private_Macros GPIO 私有宏
  * @{
  */
#define IS_GPIO_PIN_ACTION(ACTION) (((ACTION) == GPIO_PIN_RESET) || ((ACTION) == GPIO_PIN_SET))
#define IS_GPIO_PIN(PIN)           (((((uint32_t)PIN) & GPIO_PIN_MASK ) != 0x00u) && ((((uint32_t)PIN) & ~GPIO_PIN_MASK) == 0x00u))
#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_MODE_INPUT)              ||\
                            ((MODE) == GPIO_MODE_OUTPUT_PP)          ||\
                            ((MODE) == GPIO_MODE_OUTPUT_OD)          ||\
                            ((MODE) == GPIO_MODE_AF_PP)              ||\
                            ((MODE) == GPIO_MODE_AF_OD)              ||\
                            ((MODE) == GPIO_MODE_IT_RISING)          ||\
                            ((MODE) == GPIO_MODE_IT_FALLING)         ||\
                            ((MODE) == GPIO_MODE_IT_RISING_FALLING)  ||\
                            ((MODE) == GPIO_MODE_EVT_RISING)         ||\
                            ((MODE) == GPIO_MODE_EVT_FALLING)        ||\
                            ((MODE) == GPIO_MODE_EVT_RISING_FALLING) ||\
                            ((MODE) == GPIO_MODE_ANALOG))
#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_SPEED_FREQ_LOW) || \
                              ((SPEED) == GPIO_SPEED_FREQ_MEDIUM) || ((SPEED) == GPIO_SPEED_FREQ_HIGH))
#define IS_GPIO_PULL(PULL) (((PULL) == GPIO_NOPULL) || ((PULL) == GPIO_PULLUP) || \
                            ((PULL) == GPIO_PULLDOWN))
/**
  * @}
  */

/* 私有函数 ---------------------------------------------------------*/
/** @defgroup GPIO_Private_Functions GPIO 私有函数
  * @{
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32F1xx_HAL_GPIO_H */


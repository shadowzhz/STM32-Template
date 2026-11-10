/**
  ******************************************************************************
  * @file    stm32f10x_exti.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 EXTI 固件库的所有函数原型。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供参考，旨在为客户提供有关其产品的编码信息，以便客户节省时间。
  * 因此，对于因本固件的内容和/或客户将本文所含编码信息用于其产品
  * 而产生的任何索赔所导致的任何直接、间接或后果性损害，
  * 意法半导体概不承担责任。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 定义以下宏，以防止本头文件被递归包含 -------------------------------------*/
#ifndef __STM32F10x_EXTI_H
#define __STM32F10x_EXTI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup EXTI   EXTI 驱动模块
  * @{
  */

/** @defgroup EXTI_Exported_Types   EXTI 导出类型
  * @{
  */

/**
  * @brief  EXTI 模式枚举
  */

typedef enum
{
  EXTI_Mode_Interrupt = 0x00,
  EXTI_Mode_Event = 0x04
}EXTIMode_TypeDef;

#define IS_EXTI_MODE(MODE) (((MODE) == EXTI_Mode_Interrupt) || ((MODE) == EXTI_Mode_Event))

/**
  * @brief  EXTI 触发枚举
  */

typedef enum
{
  EXTI_Trigger_Rising = 0x08,
  EXTI_Trigger_Falling = 0x0C,  
  EXTI_Trigger_Rising_Falling = 0x10
}EXTITrigger_TypeDef;

#define IS_EXTI_TRIGGER(TRIGGER) (((TRIGGER) == EXTI_Trigger_Rising) || \
                                  ((TRIGGER) == EXTI_Trigger_Falling) || \
                                  ((TRIGGER) == EXTI_Trigger_Rising_Falling))
/**
  * @brief  EXTI 初始化结构体定义
  */

typedef struct
{
  uint32_t EXTI_Line;               /*!< 指定要使能或关闭的目标 EXTI 外部中断/事件线。
                                         参数取值：@ref EXTI_Lines（如 EXTI_Line0..EXTI_Line15，支持 | 或运算同时配置多条线） */

  EXTIMode_TypeDef EXTI_Mode;       /*!< 指定 EXTI 线的响应模式：
                                         - EXTI_Mode_Interrupt: 中断请求模式（触发 NVIC 核心中断）
                                         - EXTI_Mode_Event    : 事件请求模式（产生脉冲触发其它片上外设，不占用 CPU 中断） */

  EXTITrigger_TypeDef EXTI_Trigger; /*!< 指定 EXTI 线的有效跳变边沿触发方式：
                                         - EXTI_Trigger_Rising        : 仅上升沿触发（低电平变高电平时响应）
                                         - EXTI_Trigger_Falling       : 仅下降沿触发（高电平变低电平时响应，如按键接地按下）
                                         - EXTI_Trigger_Rising_Falling: 上升/下降双边沿均触发 */

  FunctionalState EXTI_LineCmd;     /*!< 指定目标中断线是开启还是关闭（ENABLE 或 DISABLE）。 */
}EXTI_InitTypeDef;

/**
  * @}
  */

/** @defgroup EXTI_Exported_Constants   EXTI 导出常量
  * @{
  */

/** @defgroup EXTI_Lines   EXTI 线
  * @{
  */

#define EXTI_Line0       ((uint32_t)0x00001)  /*!< 外部中断线 0 */
#define EXTI_Line1       ((uint32_t)0x00002)  /*!< 外部中断线 1 */
#define EXTI_Line2       ((uint32_t)0x00004)  /*!< 外部中断线 2 */
#define EXTI_Line3       ((uint32_t)0x00008)  /*!< 外部中断线 3 */
#define EXTI_Line4       ((uint32_t)0x00010)  /*!< 外部中断线 4 */
#define EXTI_Line5       ((uint32_t)0x00020)  /*!< 外部中断线 5 */
#define EXTI_Line6       ((uint32_t)0x00040)  /*!< 外部中断线 6 */
#define EXTI_Line7       ((uint32_t)0x00080)  /*!< 外部中断线 7 */
#define EXTI_Line8       ((uint32_t)0x00100)  /*!< 外部中断线 8 */
#define EXTI_Line9       ((uint32_t)0x00200)  /*!< 外部中断线 9 */
#define EXTI_Line10      ((uint32_t)0x00400)  /*!< 外部中断线 10 */
#define EXTI_Line11      ((uint32_t)0x00800)  /*!< 外部中断线 11 */
#define EXTI_Line12      ((uint32_t)0x01000)  /*!< 外部中断线 12 */
#define EXTI_Line13      ((uint32_t)0x02000)  /*!< 外部中断线 13 */
#define EXTI_Line14      ((uint32_t)0x04000)  /*!< 外部中断线 14 */
#define EXTI_Line15      ((uint32_t)0x08000)  /*!< 外部中断线 15 */
#define EXTI_Line16      ((uint32_t)0x10000)  /*!< 外部中断线 16 连接到 PVD 输出 */
#define EXTI_Line17      ((uint32_t)0x20000)  /*!< 外部中断线 17 连接到 RTC 闹钟事件 */
#define EXTI_Line18      ((uint32_t)0x40000)  /*!< 外部中断线 18 连接到 USB 设备/USB OTG FS
                                                   从挂起唤醒事件 */                                    
#define EXTI_Line19      ((uint32_t)0x80000)  /*!< 外部中断线 19 连接到以太网唤醒事件 */
                                          
#define IS_EXTI_LINE(LINE) ((((LINE) & (uint32_t)0xFFF00000) == 0x00) && ((LINE) != (uint16_t)0x00))
#define IS_GET_EXTI_LINE(LINE) (((LINE) == EXTI_Line0) || ((LINE) == EXTI_Line1) || \
                            ((LINE) == EXTI_Line2) || ((LINE) == EXTI_Line3) || \
                            ((LINE) == EXTI_Line4) || ((LINE) == EXTI_Line5) || \
                            ((LINE) == EXTI_Line6) || ((LINE) == EXTI_Line7) || \
                            ((LINE) == EXTI_Line8) || ((LINE) == EXTI_Line9) || \
                            ((LINE) == EXTI_Line10) || ((LINE) == EXTI_Line11) || \
                            ((LINE) == EXTI_Line12) || ((LINE) == EXTI_Line13) || \
                            ((LINE) == EXTI_Line14) || ((LINE) == EXTI_Line15) || \
                            ((LINE) == EXTI_Line16) || ((LINE) == EXTI_Line17) || \
                            ((LINE) == EXTI_Line18) || ((LINE) == EXTI_Line19))

                    
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup EXTI_Exported_Macros   EXTI 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup EXTI_Exported_Functions   EXTI 导出函数
  * @{
  */

void EXTI_DeInit(void);

/**
  * @brief  根据 EXTI_InitStruct 中指定的参数初始化外部中断/事件线
  * @param  EXTI_InitStruct: 指向包含中断线配置信息的 EXTI_InitTypeDef 结构体指针
  * @warning 【完整外部中断配置四步曲 / 避坑警告】
  *          若需将某个 GPIO 引脚配置为外部中断输入，必须严格按序执行以下步骤：
  *          1. 【使能时钟】：开启 GPIO 时钟及 AFIO 复用时钟：
  *             `RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);`
  *          2. 【引脚模式】：GPIO 必须配置为输入模式（浮空输入或上/下拉输入）；
  *          3. 【引脚映射】：调用 `GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);`
  *             将目标引脚连接至对应的 EXTI 线上；
  *          4. 【调用本函数】：初始化 EXTI_Init(&EXTI_InitStruct)；
  *          5. 【NVIC 使能】：配置 NVIC_Init() 使能目标通道（EXTI0_IRQn..EXTI15_10_IRQn）。
  *          若漏掉 AFIO 时钟或映射步骤，外部引脚电平跳变将无法传导至 EXTI 中断线！
  * @retval None
  */
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct);

void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct);
void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line);
FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line);
void EXTI_ClearFlag(uint32_t EXTI_Line);

/**
  * @brief  检查指定的 EXTI 外部中断线是否产生了挂起中断
  * @param  EXTI_Line: 目标中断线（如 EXTI_Line0、EXTI_Line13 等）
  * @note   常用于在合并通道的中断服务函数（如 EXTI9_5_IRQHandler、EXTI15_10_IRQHandler）中区分具体是哪个引脚触发了中断。
  * @retval SET（挂起产生中断）或 RESET（未产生）
  */
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);

/**
  * @brief  清除指定 EXTI 中断线的挂起标志位（向 EXTI_PR 寄存器写 1 清零）
  * @param  EXTI_Line: 待清除的目标中断线（如 EXTI_Line0..EXTI_Line15）
  * @warning 【中断服务函数死锁铁律 / 必做操作】
  *          在任何 EXTI 中断服务函数（如 EXTI0_IRQHandler）退出前，【必须显式调用本函数清除挂起位】！
  *          STM32 的 EXTI 挂起标志【不会自动清除】。
  *          若遗漏本函数，ISR 退出瞬间 CPU 硬件检测到挂起位依然为 1，将【无限重复进入中断】，
  *          导致主循环完全无法执行，MCU 呈现“死机卡死”假象！
  *          标准 ISR 处理范式：
  *          @code
  *          void EXTI0_IRQHandler(void) {
  *              if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
  *                  // 执行中断业务逻辑...
  *                  EXTI_ClearITPendingBit(EXTI_Line0); // 必须手动清除标志！
  *              }
  *          }
  *          @endcode
  * @retval None
  */
void EXTI_ClearITPendingBit(uint32_t EXTI_Line);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_EXTI_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****文件结束****/

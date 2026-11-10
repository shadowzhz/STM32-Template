/**
  ******************************************************************************
  * @file    misc.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含杂项固件库函数的全部函数原型（CMSIS 函数的补充）。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供参考，其目的在于为客户提供有关其产品的编码信息，以帮助客户节省时间。
  * 因此，对于因本固件的内容和/或客户将其中所含编码信息用于其产品而产生的任何索赔
  * 所造成的任何直接、间接或后果性损害，STMicroelectronics 概不承担任何责任。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 定义以下宏，以防止本头文件被递归包含 -------------------------------------*/
#ifndef __MISC_H
#define __MISC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup MISC   MISC 杂项驱动
  * @{
  */

/** @defgroup MISC_Exported_Types   MISC 导出类型
  * @{
  */

/**
  * @brief  NVIC 初始化结构体定义
  */

typedef struct
{
  uint8_t NVIC_IRQChannel;                    /*!< 指定要使能或关闭的中断通道。
                                                   该参数可取 @ref IRQn_Type 中的值（如 USART1_IRQn, TIM2_IRQn） */

  uint8_t NVIC_IRQChannelPreemptionPriority;  /*!< 指定中断通道的抢占优先级。
                                                   该参数可取 0 到 15 之间的值（【数值越小，优先级越高，0 为最高】）。
                                                   高抢占优先级中断可打断正在执行的低抢占优先级中断形成中断嵌套。 */

  uint8_t NVIC_IRQChannelSubPriority;         /*!< 指定中断通道的响应/子优先级。
                                                   该参数可取 0 到 15 之间的值。
                                                   仅在两个抢占优先级相同的中断同时挂起时，裁决谁先执行；无法形成中断打断。 */

  FunctionalState NVIC_IRQChannelCmd;         /*!< 指定目标中断通道是使能还是关闭（ENABLE 或 DISABLE）。 */
} NVIC_InitTypeDef;
 
/**
  * @}
  */

/** @defgroup NVIC_Priority_Table   NVIC 优先级表
  * @{
  */

/**
  * 下表给出在不同优先级分组配置（由 NVIC_PriorityGroupConfig 函数设定）下，
  * 抢占优先级（pre-emption priority）与子优先级（subpriority）的允许取值范围。
  * 表头对照：NVIC_PriorityGroup = 优先级分组；
  *           Description 列中的 "x bits for pre-emption priority" = 抢占优先级占 x 位，
  *           "x bits for subpriority" = 子优先级占 x 位。
  * （为保持对齐格式，原表以英文原样保留）
@code
 The table below gives the allowed values of the pre-emption priority and subpriority according
 to the Priority Grouping configuration performed by NVIC_PriorityGroupConfig function
  ============================================================================================================================
    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
  ============================================================================================================================
   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
                         |                                   |                             |   4 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
                         |                                   |                             |   3 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
                         |                                   |                             |   2 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
                         |                                   |                             |   1 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
                         |                                   |                             |   0 bits for subpriority
  ============================================================================================================================
@endcode
*/

/**
  * @}
  */

/** @defgroup MISC_Exported_Constants   MISC 导出常量
  * @{
  */

/** @defgroup Vector_Table_Base   向量表基址
  * @{
  */

#define NVIC_VectTab_RAM             ((uint32_t)0x20000000)
#define NVIC_VectTab_FLASH           ((uint32_t)0x08000000)
#define IS_NVIC_VECTTAB(VECTTAB) (((VECTTAB) == NVIC_VectTab_RAM) || \
                                  ((VECTTAB) == NVIC_VectTab_FLASH))
/**
  * @}
  */

/** @defgroup System_Low_Power   系统低功耗
  * @{
  */

#define NVIC_LP_SEVONPEND            ((uint8_t)0x10)
#define NVIC_LP_SLEEPDEEP            ((uint8_t)0x04)
#define NVIC_LP_SLEEPONEXIT          ((uint8_t)0x02)
#define IS_NVIC_LP(LP) (((LP) == NVIC_LP_SEVONPEND) || \
                        ((LP) == NVIC_LP_SLEEPDEEP) || \
                        ((LP) == NVIC_LP_SLEEPONEXIT))
/**
  * @}
  */

/** @defgroup Preemption_Priority_Group   抢占优先级分组
  * @{
  */

#define NVIC_PriorityGroup_0         ((uint32_t)0x700) /*!< 抢占优先级占 0 位
                                                            子优先级占 4 位 */
#define NVIC_PriorityGroup_1         ((uint32_t)0x600) /*!< 抢占优先级占 1 位
                                                            子优先级占 3 位 */
#define NVIC_PriorityGroup_2         ((uint32_t)0x500) /*!< 抢占优先级占 2 位
                                                            子优先级占 2 位 */
#define NVIC_PriorityGroup_3         ((uint32_t)0x400) /*!< 抢占优先级占 3 位
                                                            子优先级占 1 位 */
#define NVIC_PriorityGroup_4         ((uint32_t)0x300) /*!< 抢占优先级占 4 位
                                                            子优先级占 0 位 */

#define IS_NVIC_PRIORITY_GROUP(GROUP) (((GROUP) == NVIC_PriorityGroup_0) || \
                                       ((GROUP) == NVIC_PriorityGroup_1) || \
                                       ((GROUP) == NVIC_PriorityGroup_2) || \
                                       ((GROUP) == NVIC_PriorityGroup_3) || \
                                       ((GROUP) == NVIC_PriorityGroup_4))

#define IS_NVIC_PREEMPTION_PRIORITY(PRIORITY)  ((PRIORITY) < 0x10)

#define IS_NVIC_SUB_PRIORITY(PRIORITY)  ((PRIORITY) < 0x10)

#define IS_NVIC_OFFSET(OFFSET)  ((OFFSET) < 0x000FFFFF)

/**
  * @}
  */

/** @defgroup SysTick_clock_source   SysTick 时钟源
  * @{
  */

#define SysTick_CLKSource_HCLK_Div8    ((uint32_t)0xFFFFFFFB)
#define SysTick_CLKSource_HCLK         ((uint32_t)0x00000004)
#define IS_SYSTICK_CLK_SOURCE(SOURCE) (((SOURCE) == SysTick_CLKSource_HCLK) || \
                                       ((SOURCE) == SysTick_CLKSource_HCLK_Div8))
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup MISC_Exported_Macros   MISC 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup MISC_Exported_Functions   MISC 导出函数
  * @{
  */

/**
  * @brief  配置中断优先级分组（PRIGROUP 分配抢占与响应优先级位数）
  * @param  NVIC_PriorityGroup: 优先级分组方案，可取以下值之一：
  *         - NVIC_PriorityGroup_0: 0 位抢占优先级，4 位响应优先级（无中断嵌套功能）
  *         - NVIC_PriorityGroup_1: 1 位抢占优先级（0~1），3 位响应优先级（0~7）
  *         - NVIC_PriorityGroup_2: 2 位抢占优先级（0~3），2 位响应优先级（0~3）（最常用方案，支持 4 级嵌套与 4 级响应）
  *         - NVIC_PriorityGroup_3: 3 位抢占优先级（0~7），1 位响应优先级（0~1）
  *         - NVIC_PriorityGroup_4: 4 位抢占优先级（0~15），0 位响应优先级（全抢占模式）
  * @warning 【全工程只允许调用一次】
  *          本函数通常【仅在 main() 函数开头最先调用一次】！
  *          切勿在各个子外设驱动（如 usart.c, timer.c, key.c）中重复调用，否则后调用的模块会覆盖前面的全局分组，
  *          导致原本算好的优先级位宽全盘错乱！
  * @retval 无
  */
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup);

/**
  * @brief  根据 NVIC_InitStruct 中指定的参数初始化 NVIC 外设中断通道
  * @param  NVIC_InitStruct: 指向包含中断通道、抢占优先级、子优先级与使能状态的 NVIC_InitTypeDef 结构体指针
  * @retval 无
  */
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct);
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset);
void NVIC_SystemLPConfig(uint8_t LowPowerMode, FunctionalState NewState);
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource);

#ifdef __cplusplus
}
#endif

#endif /* __MISC_H */

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

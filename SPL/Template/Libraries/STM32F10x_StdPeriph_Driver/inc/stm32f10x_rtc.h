/**
  ******************************************************************************
  * @file    stm32f10x_rtc.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 RTC 固件库的所有函数原型。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供指导之用，旨在为客户提供有关其产品的编码信息，以节省他们的时间。
  * 因此，对于因本固件的内容和/或客户将此处包含的编码信息
  * 与其产品结合使用而提出的任何索赔所造成的任何直接、间接或后果性损害，
  * STMicroelectronics 概不承担任何责任。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 定义以下宏，以防止本头文件被递归包含 -------------------------------------*/
#ifndef __STM32F10x_RTC_H
#define __STM32F10x_RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup RTC   RTC 外设
  * @{
  */ 

/** @defgroup RTC_Exported_Types   RTC 导出类型
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup RTC_Exported_Constants   RTC 导出常量
  * @{
  */

/** @defgroup RTC_interrupts_define   RTC 中断定义
  * @{
  */

#define RTC_IT_OW            ((uint16_t)0x0004)  /*!< 溢出中断 */
#define RTC_IT_ALR           ((uint16_t)0x0002)  /*!< 闹钟中断 */
#define RTC_IT_SEC           ((uint16_t)0x0001)  /*!< 秒中断 */
#define IS_RTC_IT(IT) ((((IT) & (uint16_t)0xFFF8) == 0x00) && ((IT) != 0x00))
#define IS_RTC_GET_IT(IT) (((IT) == RTC_IT_OW) || ((IT) == RTC_IT_ALR) || \
                           ((IT) == RTC_IT_SEC))
/**
  * @}
  */ 

/** @defgroup RTC_interrupts_flags   RTC 中断标志
  * @{
  */

#define RTC_FLAG_RTOFF       ((uint16_t)0x0020)  /*!< RTC 操作关闭标志 */
#define RTC_FLAG_RSF         ((uint16_t)0x0008)  /*!< 寄存器同步标志 */
#define RTC_FLAG_OW          ((uint16_t)0x0004)  /*!< 溢出标志 */
#define RTC_FLAG_ALR         ((uint16_t)0x0002)  /*!< 闹钟标志 */
#define RTC_FLAG_SEC         ((uint16_t)0x0001)  /*!< 秒标志 */
#define IS_RTC_CLEAR_FLAG(FLAG) ((((FLAG) & (uint16_t)0xFFF0) == 0x00) && ((FLAG) != 0x00))
#define IS_RTC_GET_FLAG(FLAG) (((FLAG) == RTC_FLAG_RTOFF) || ((FLAG) == RTC_FLAG_RSF) || \
                               ((FLAG) == RTC_FLAG_OW) || ((FLAG) == RTC_FLAG_ALR) || \
                               ((FLAG) == RTC_FLAG_SEC))
#define IS_RTC_PRESCALER(PRESCALER) ((PRESCALER) <= 0xFFFFF)

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup RTC_Exported_Macros   RTC 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup RTC_Exported_Functions   RTC 导出函数
  * @{
  */

void RTC_ITConfig(uint16_t RTC_IT, FunctionalState NewState);
void RTC_EnterConfigMode(void);
void RTC_ExitConfigMode(void);
uint32_t  RTC_GetCounter(void);
void RTC_SetCounter(uint32_t CounterValue);
void RTC_SetPrescaler(uint32_t PrescalerValue);
void RTC_SetAlarm(uint32_t AlarmValue);
uint32_t  RTC_GetDivider(void);
void RTC_WaitForLastTask(void);
void RTC_WaitForSynchro(void);
FlagStatus RTC_GetFlagStatus(uint16_t RTC_FLAG);
void RTC_ClearFlag(uint16_t RTC_FLAG);
ITStatus RTC_GetITStatus(uint16_t RTC_IT);
void RTC_ClearITPendingBit(uint16_t RTC_IT);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_RTC_H */
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

/**
  ******************************************************************************
  * @file    stm32f10x_iwdg.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 IWDG 固件库的所有函数原型。
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
#ifndef __STM32F10x_IWDG_H
#define __STM32F10x_IWDG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup IWDG   IWDG 外设
  * @{
  */

/** @defgroup IWDG_Exported_Types   IWDG 导出类型
  * @{
  */

/**
  * @}
  */

/** @defgroup IWDG_Exported_Constants   IWDG 导出常量
  * @{
  */

/** @defgroup IWDG_WriteAccess   IWDG 写访问
  * @{
  */

#define IWDG_WriteAccess_Enable     ((uint16_t)0x5555)
#define IWDG_WriteAccess_Disable    ((uint16_t)0x0000)
#define IS_IWDG_WRITE_ACCESS(ACCESS) (((ACCESS) == IWDG_WriteAccess_Enable) || \
                                      ((ACCESS) == IWDG_WriteAccess_Disable))
/**
  * @}
  */

/** @defgroup IWDG_prescaler   IWDG 预分频器
  * @{
  */

#define IWDG_Prescaler_4            ((uint8_t)0x00)
#define IWDG_Prescaler_8            ((uint8_t)0x01)
#define IWDG_Prescaler_16           ((uint8_t)0x02)
#define IWDG_Prescaler_32           ((uint8_t)0x03)
#define IWDG_Prescaler_64           ((uint8_t)0x04)
#define IWDG_Prescaler_128          ((uint8_t)0x05)
#define IWDG_Prescaler_256          ((uint8_t)0x06)
#define IS_IWDG_PRESCALER(PRESCALER) (((PRESCALER) == IWDG_Prescaler_4)  || \
                                      ((PRESCALER) == IWDG_Prescaler_8)  || \
                                      ((PRESCALER) == IWDG_Prescaler_16) || \
                                      ((PRESCALER) == IWDG_Prescaler_32) || \
                                      ((PRESCALER) == IWDG_Prescaler_64) || \
                                      ((PRESCALER) == IWDG_Prescaler_128)|| \
                                      ((PRESCALER) == IWDG_Prescaler_256))
/**
  * @}
  */

/** @defgroup IWDG_Flag   IWDG 标志
  * @{
  */

#define IWDG_FLAG_PVU               ((uint16_t)0x0001)
#define IWDG_FLAG_RVU               ((uint16_t)0x0002)
#define IS_IWDG_FLAG(FLAG) (((FLAG) == IWDG_FLAG_PVU) || ((FLAG) == IWDG_FLAG_RVU))
#define IS_IWDG_RELOAD(RELOAD) ((RELOAD) <= 0xFFF)
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup IWDG_Exported_Macros   IWDG 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup IWDG_Exported_Functions   IWDG 导出函数
  * @{
  */

void IWDG_WriteAccessCmd(uint16_t IWDG_WriteAccess);
void IWDG_SetPrescaler(uint8_t IWDG_Prescaler);
void IWDG_SetReload(uint16_t Reload);
void IWDG_ReloadCounter(void);
void IWDG_Enable(void);
FlagStatus IWDG_GetFlagStatus(uint16_t IWDG_FLAG);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_IWDG_H */
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

/**
  ******************************************************************************
  * @file    stm32f10x_bkp.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 BKP 固件库的全部函数原型。
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
#ifndef __STM32F10x_BKP_H
#define __STM32F10x_BKP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup BKP   BKP 备份寄存器
  * @{
  */

/** @defgroup BKP_Exported_Types   BKP 导出类型
  * @{
  */

/**
  * @}
  */

/** @defgroup BKP_Exported_Constants   BKP 导出常量
  * @{
  */

/** @defgroup Tamper_Pin_active_level   侵入检测引脚有效电平
  * @{
  */

#define BKP_TamperPinLevel_High           ((uint16_t)0x0000)
#define BKP_TamperPinLevel_Low            ((uint16_t)0x0001)
#define IS_BKP_TAMPER_PIN_LEVEL(LEVEL) (((LEVEL) == BKP_TamperPinLevel_High) || \
                                        ((LEVEL) == BKP_TamperPinLevel_Low))
/**
  * @}
  */

/** @defgroup RTC_output_source_to_output_on_the_Tamper_pin
  *           在侵入检测引脚上输出的 RTC 输出源
  * @{
  */

#define BKP_RTCOutputSource_None          ((uint16_t)0x0000)
#define BKP_RTCOutputSource_CalibClock    ((uint16_t)0x0080)
#define BKP_RTCOutputSource_Alarm         ((uint16_t)0x0100)
#define BKP_RTCOutputSource_Second        ((uint16_t)0x0300)
#define IS_BKP_RTC_OUTPUT_SOURCE(SOURCE) (((SOURCE) == BKP_RTCOutputSource_None) || \
                                          ((SOURCE) == BKP_RTCOutputSource_CalibClock) || \
                                          ((SOURCE) == BKP_RTCOutputSource_Alarm) || \
                                          ((SOURCE) == BKP_RTCOutputSource_Second))
/**
  * @}
  */

/** @defgroup Data_Backup_Register   数据备份寄存器
  * @{
  */

#define BKP_DR1                           ((uint16_t)0x0004)
#define BKP_DR2                           ((uint16_t)0x0008)
#define BKP_DR3                           ((uint16_t)0x000C)
#define BKP_DR4                           ((uint16_t)0x0010)
#define BKP_DR5                           ((uint16_t)0x0014)
#define BKP_DR6                           ((uint16_t)0x0018)
#define BKP_DR7                           ((uint16_t)0x001C)
#define BKP_DR8                           ((uint16_t)0x0020)
#define BKP_DR9                           ((uint16_t)0x0024)
#define BKP_DR10                          ((uint16_t)0x0028)
#define BKP_DR11                          ((uint16_t)0x0040)
#define BKP_DR12                          ((uint16_t)0x0044)
#define BKP_DR13                          ((uint16_t)0x0048)
#define BKP_DR14                          ((uint16_t)0x004C)
#define BKP_DR15                          ((uint16_t)0x0050)
#define BKP_DR16                          ((uint16_t)0x0054)
#define BKP_DR17                          ((uint16_t)0x0058)
#define BKP_DR18                          ((uint16_t)0x005C)
#define BKP_DR19                          ((uint16_t)0x0060)
#define BKP_DR20                          ((uint16_t)0x0064)
#define BKP_DR21                          ((uint16_t)0x0068)
#define BKP_DR22                          ((uint16_t)0x006C)
#define BKP_DR23                          ((uint16_t)0x0070)
#define BKP_DR24                          ((uint16_t)0x0074)
#define BKP_DR25                          ((uint16_t)0x0078)
#define BKP_DR26                          ((uint16_t)0x007C)
#define BKP_DR27                          ((uint16_t)0x0080)
#define BKP_DR28                          ((uint16_t)0x0084)
#define BKP_DR29                          ((uint16_t)0x0088)
#define BKP_DR30                          ((uint16_t)0x008C)
#define BKP_DR31                          ((uint16_t)0x0090)
#define BKP_DR32                          ((uint16_t)0x0094)
#define BKP_DR33                          ((uint16_t)0x0098)
#define BKP_DR34                          ((uint16_t)0x009C)
#define BKP_DR35                          ((uint16_t)0x00A0)
#define BKP_DR36                          ((uint16_t)0x00A4)
#define BKP_DR37                          ((uint16_t)0x00A8)
#define BKP_DR38                          ((uint16_t)0x00AC)
#define BKP_DR39                          ((uint16_t)0x00B0)
#define BKP_DR40                          ((uint16_t)0x00B4)
#define BKP_DR41                          ((uint16_t)0x00B8)
#define BKP_DR42                          ((uint16_t)0x00BC)

#define IS_BKP_DR(DR) (((DR) == BKP_DR1)  || ((DR) == BKP_DR2)  || ((DR) == BKP_DR3)  || \
                       ((DR) == BKP_DR4)  || ((DR) == BKP_DR5)  || ((DR) == BKP_DR6)  || \
                       ((DR) == BKP_DR7)  || ((DR) == BKP_DR8)  || ((DR) == BKP_DR9)  || \
                       ((DR) == BKP_DR10) || ((DR) == BKP_DR11) || ((DR) == BKP_DR12) || \
                       ((DR) == BKP_DR13) || ((DR) == BKP_DR14) || ((DR) == BKP_DR15) || \
                       ((DR) == BKP_DR16) || ((DR) == BKP_DR17) || ((DR) == BKP_DR18) || \
                       ((DR) == BKP_DR19) || ((DR) == BKP_DR20) || ((DR) == BKP_DR21) || \
                       ((DR) == BKP_DR22) || ((DR) == BKP_DR23) || ((DR) == BKP_DR24) || \
                       ((DR) == BKP_DR25) || ((DR) == BKP_DR26) || ((DR) == BKP_DR27) || \
                       ((DR) == BKP_DR28) || ((DR) == BKP_DR29) || ((DR) == BKP_DR30) || \
                       ((DR) == BKP_DR31) || ((DR) == BKP_DR32) || ((DR) == BKP_DR33) || \
                       ((DR) == BKP_DR34) || ((DR) == BKP_DR35) || ((DR) == BKP_DR36) || \
                       ((DR) == BKP_DR37) || ((DR) == BKP_DR38) || ((DR) == BKP_DR39) || \
                       ((DR) == BKP_DR40) || ((DR) == BKP_DR41) || ((DR) == BKP_DR42))

#define IS_BKP_CALIBRATION_VALUE(VALUE) ((VALUE) <= 0x7F)
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup BKP_Exported_Macros   BKP 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup BKP_Exported_Functions   BKP 导出函数
  * @{
  */

void BKP_DeInit(void);
void BKP_TamperPinLevelConfig(uint16_t BKP_TamperPinLevel);
void BKP_TamperPinCmd(FunctionalState NewState);
void BKP_ITConfig(FunctionalState NewState);
void BKP_RTCOutputConfig(uint16_t BKP_RTCOutputSource);
void BKP_SetRTCCalibrationValue(uint8_t CalibrationValue);
void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data);
uint16_t BKP_ReadBackupRegister(uint16_t BKP_DR);
FlagStatus BKP_GetFlagStatus(void);
void BKP_ClearFlag(void);
ITStatus BKP_GetITStatus(void);
void BKP_ClearITPendingBit(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_BKP_H */
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

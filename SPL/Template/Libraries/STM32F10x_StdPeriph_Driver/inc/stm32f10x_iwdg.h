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

/**
  * @brief  使能或禁止对 IWDG 预分频器（PR）和重装载寄存器（RLR）的写访问权限
  * @param  IWDG_WriteAccess: 取值为 IWDG_WriteAccess_Enable（0x5555）或 IWDG_WriteAccess_Disable（0x0000）
  * @note   修改预分频与重装值前，必须传入 IWDG_WriteAccess_Enable 解锁写保护。
  * @retval None
  */
void IWDG_WriteAccessCmd(uint16_t IWDG_WriteAccess);

/**
  * @brief  设置 IWDG 预分频系数
  * @param  IWDG_Prescaler: 4, 8, 16, 32, 64, 128, 256 分频
  * @retval None
  */
void IWDG_SetPrescaler(uint8_t IWDG_Prescaler);

/**
  * @brief  设置 IWDG 计数器重装载值（0 ~ 4095，12 位）
  * @param  Reload: 重装计数值
  * @note   【超时时间估算公式（LSI 典型频率 40kHz）】：
  *         $T_{out} = \frac{\text{Prescaler} \times \text{Reload}}{40000} \text{ (秒)}$
  *         例如：Prescaler = 64，Reload = 625 时，超时时间为 $64 \times 625 / 40000 = 1.0\text{s}$。
  * @retval None
  */
void IWDG_SetReload(uint16_t Reload);

/**
  * @brief  重装载 IWDG 计数器（俗称“喂狗”，向 KR 寄存器写入 0xAAAA）
  * @note   主程序主循环中必须在超时时间到达前周期性调用本函数，以防系统产生复位。
  * @retval None
  */
void IWDG_ReloadCounter(void);

/**
  * @brief  启动独立看门狗（向 KR 寄存器写入 0xCCCC）
  * @warning 【不可逆运行铁律 / 避坑警告】
  *          STM32 硬件规定：独立看门狗一旦调用本函数使能，【没有任何软件手段可以将其关闭】！
  *          直到芯片断电或产生复位前，看门狗都将永久运行。
  *          在调试阶段（尤其是单步断点调试时），请务必谨慎开启，否则 CPU 暂停在断点处将引发看门狗复位！
  * @retval None
  */
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

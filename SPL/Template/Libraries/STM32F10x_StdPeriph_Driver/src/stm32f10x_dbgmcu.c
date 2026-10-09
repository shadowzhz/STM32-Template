/**
  ******************************************************************************
  * @file    stm32f10x_dbgmcu.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供所有 DBGMCU 固件函数。
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

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_dbgmcu.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup DBGMCU
  * @brief DBGMCU 驱动模块
  * @{
  */ 

/** @defgroup DBGMCU_Private_TypesDefinitions   DBGMCU 私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DBGMCU_Private_Defines   DBGMCU 私有宏定义
  * @{
  */

#define IDCODE_DEVID_MASK    ((uint32_t)0x00000FFF)
/**
  * @}
  */

/** @defgroup DBGMCU_Private_Macros   DBGMCU 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup DBGMCU_Private_Variables   DBGMCU 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DBGMCU_Private_FunctionPrototypes   DBGMCU 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup DBGMCU_Private_Functions   DBGMCU 私有函数
  * @{
  */

/**
  * @brief  返回器件版本标识符。
  * @param  无
  * @retval 器件版本标识符
  */
uint32_t DBGMCU_GetREVID(void)
{
   return(DBGMCU->IDCODE >> 16);
}

/**
  * @brief  返回器件标识符。
  * @param  无
  * @retval 器件标识符
  */
uint32_t DBGMCU_GetDEVID(void)
{
   return(DBGMCU->IDCODE & IDCODE_DEVID_MASK);
}

/**
  * @brief  配置 MCU 处于调试模式时指定外设和低功耗模式的行为。
  * @param  DBGMCU_Periph: 指定外设和低功耗模式。
  *   该参数可以是以下值的任意组合：
  *     @arg DBGMCU_SLEEP: 在 SLEEP 模式下保持调试器连接
  *     @arg DBGMCU_STOP: 在 STOP 模式下保持调试器连接
  *     @arg DBGMCU_STANDBY: 在 STANDBY 模式下保持调试器连接
  *     @arg DBGMCU_IWDG_STOP: 内核暂停时调试 IWDG 停止
  *     @arg DBGMCU_WWDG_STOP: 内核暂停时调试 WWDG 停止
  *     @arg DBGMCU_TIM1_STOP: 内核暂停时 TIM1 计数器停止
  *     @arg DBGMCU_TIM2_STOP: 内核暂停时 TIM2 计数器停止
  *     @arg DBGMCU_TIM3_STOP: 内核暂停时 TIM3 计数器停止
  *     @arg DBGMCU_TIM4_STOP: 内核暂停时 TIM4 计数器停止
  *     @arg DBGMCU_CAN1_STOP: 内核暂停时调试 CAN2 停止
  *     @arg DBGMCU_I2C1_SMBUS_TIMEOUT: 内核暂停时 I2C1 SMBUS 超时模式停止
  *     @arg DBGMCU_I2C2_SMBUS_TIMEOUT: 内核暂停时 I2C2 SMBUS 超时模式停止
  *     @arg DBGMCU_TIM5_STOP: 内核暂停时 TIM5 计数器停止
  *     @arg DBGMCU_TIM6_STOP: 内核暂停时 TIM6 计数器停止
  *     @arg DBGMCU_TIM7_STOP: 内核暂停时 TIM7 计数器停止
  *     @arg DBGMCU_TIM8_STOP: 内核暂停时 TIM8 计数器停止
  *     @arg DBGMCU_CAN2_STOP: 内核暂停时调试 CAN2 停止
  *     @arg DBGMCU_TIM15_STOP: 内核暂停时 TIM15 计数器停止
  *     @arg DBGMCU_TIM16_STOP: 内核暂停时 TIM16 计数器停止
  *     @arg DBGMCU_TIM17_STOP: 内核暂停时 TIM17 计数器停止
  *     @arg DBGMCU_TIM9_STOP: 内核暂停时 TIM9 计数器停止
  *     @arg DBGMCU_TIM10_STOP: 内核暂停时 TIM10 计数器停止
  *     @arg DBGMCU_TIM11_STOP: 内核暂停时 TIM11 计数器停止
  *     @arg DBGMCU_TIM12_STOP: 内核暂停时 TIM12 计数器停止
  *     @arg DBGMCU_TIM13_STOP: 内核暂停时 TIM13 计数器停止
  *     @arg DBGMCU_TIM14_STOP: 内核暂停时 TIM14 计数器停止
  * @param  NewState: 调试模式下指定外设的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void DBGMCU_Config(uint32_t DBGMCU_Periph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_DBGMCU_PERIPH(DBGMCU_Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    DBGMCU->CR |= DBGMCU_Periph;
  }
  else
  {
    DBGMCU->CR &= ~DBGMCU_Periph;
  }
}

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

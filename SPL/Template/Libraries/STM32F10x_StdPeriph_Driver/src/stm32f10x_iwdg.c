/**
  ******************************************************************************
  * @file    stm32f10x_iwdg.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供 IWDG 的所有固件函数。
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

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_iwdg.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup IWDG
  * @brief IWDG 驱动模块
  * @{
  */ 

/** @defgroup IWDG_Private_TypesDefinitions   IWDG 私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup IWDG_Private_Defines   IWDG 私有宏定义
  * @{
  */ 

/* ---------------------- IWDG 寄存器位掩码 ----------------------------*/

/* KR 寄存器位掩码 */
#define KR_KEY_Reload    ((uint16_t)0xAAAA)
#define KR_KEY_Enable    ((uint16_t)0xCCCC)

/**
  * @}
  */ 

/** @defgroup IWDG_Private_Macros   IWDG 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup IWDG_Private_Variables   IWDG 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup IWDG_Private_FunctionPrototypes   IWDG 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup IWDG_Private_Functions   IWDG 私有函数
  * @{
  */

/**
  * @brief  使能或关闭对 IWDG_PR 和 IWDG_RLR 寄存器的写访问。
  * @param  IWDG_WriteAccess: 对 IWDG_PR 和 IWDG_RLR 寄存器写访问的新状态。
  *   该参数可取以下值之一：
  *     @arg IWDG_WriteAccess_Enable: 使能对 IWDG_PR 和 IWDG_RLR 寄存器的写访问
  *     @arg IWDG_WriteAccess_Disable: 关闭对 IWDG_PR 和 IWDG_RLR 寄存器的写访问
  * @retval 无
  */
void IWDG_WriteAccessCmd(uint16_t IWDG_WriteAccess)
{
  /* 检查参数 */
  assert_param(IS_IWDG_WRITE_ACCESS(IWDG_WriteAccess));
  IWDG->KR = IWDG_WriteAccess;
}

/**
  * @brief  设置 IWDG 预分频器值。
  * @param  IWDG_Prescaler: 指定 IWDG 预分频器值。
  *   该参数可取以下值之一：
  *     @arg IWDG_Prescaler_4: IWDG 预分频器设置为 4
  *     @arg IWDG_Prescaler_8: IWDG 预分频器设置为 8
  *     @arg IWDG_Prescaler_16: IWDG 预分频器设置为 16
  *     @arg IWDG_Prescaler_32: IWDG 预分频器设置为 32
  *     @arg IWDG_Prescaler_64: IWDG 预分频器设置为 64
  *     @arg IWDG_Prescaler_128: IWDG 预分频器设置为 128
  *     @arg IWDG_Prescaler_256: IWDG 预分频器设置为 256
  * @retval 无
  */
void IWDG_SetPrescaler(uint8_t IWDG_Prescaler)
{
  /* 检查参数 */
  assert_param(IS_IWDG_PRESCALER(IWDG_Prescaler));
  IWDG->PR = IWDG_Prescaler;
}

/**
  * @brief  设置 IWDG 重装载值。
  * @param  Reload: 指定 IWDG 重装载值。
  *   该参数必须是 0 到 0x0FFF 之间的数字。
  * @retval 无
  */
void IWDG_SetReload(uint16_t Reload)
{
  /* 检查参数 */
  assert_param(IS_IWDG_RELOAD(Reload));
  IWDG->RLR = Reload;
}

/**
  * @brief  用重装载寄存器中定义的值重装载 IWDG 计数器
  *   （对 IWDG_PR 和 IWDG_RLR 寄存器的写访问已关闭）。
  * @param  None
  * @retval 无
  */
void IWDG_ReloadCounter(void)
{
  IWDG->KR = KR_KEY_Reload;
}

/**
  * @brief  使能 IWDG（对 IWDG_PR 和 IWDG_RLR 寄存器的写访问已关闭）。
  * @param  None
  * @retval 无
  */
void IWDG_Enable(void)
{
  IWDG->KR = KR_KEY_Enable;
}

/**
  * @brief  检查指定的 IWDG 标志是否置位。
  * @param  IWDG_FLAG: 指定要检查的标志。
  *   该参数可取以下值之一：
  *     @arg IWDG_FLAG_PVU: 预分频器值正在更新
  *     @arg IWDG_FLAG_RVU: 重装载值正在更新
  * @retval IWDG_FLAG 的新状态（SET 或 RESET）。
  */
FlagStatus IWDG_GetFlagStatus(uint16_t IWDG_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_IWDG_FLAG(IWDG_FLAG));
  if ((IWDG->SR & IWDG_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* 返回标志状态 */
  return bitstatus;
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

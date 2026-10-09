/**
  ******************************************************************************
  * @file    stm32f10x_wwdg.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供 WWDG 的所有固件函数。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供指导之用，旨在为客户提供与其产品相关的编码信息，
  * 以帮助客户节省时间。因此，对于因本固件的内容和/或客户将其中所含
  * 编码信息用于其产品而产生的任何索赔所导致的任何直接、间接或
  * 后果性损害，STMicroelectronics 概不承担任何责任。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_wwdg.h"
#include "stm32f10x_rcc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup WWDG  WWDG 驱动模块
  * @brief WWDG 驱动模块
  * @{
  */

/** @defgroup WWDG_Private_TypesDefinitions  WWDG 私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup WWDG_Private_Defines  WWDG 私有宏定义
  * @{
  */

/* ----------- WWDG 寄存器位在别名区中的地址 ----------- */
#define WWDG_OFFSET       (WWDG_BASE - PERIPH_BASE)

/* EWI 位的别名字地址 */
#define CFR_OFFSET        (WWDG_OFFSET + 0x04)
#define EWI_BitNumber     0x09
#define CFR_EWI_BB        (PERIPH_BB_BASE + (CFR_OFFSET * 32) + (EWI_BitNumber * 4))

/* --------------------- WWDG 寄存器位掩码 ------------------------ */

/* CR 寄存器位掩码 */
#define CR_WDGA_Set       ((uint32_t)0x00000080)

/* CFR 寄存器位掩码 */
#define CFR_WDGTB_Mask    ((uint32_t)0xFFFFFE7F)
#define CFR_W_Mask        ((uint32_t)0xFFFFFF80)
#define BIT_Mask          ((uint8_t)0x7F)

/**
  * @}
  */

/** @defgroup WWDG_Private_Macros  WWDG 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup WWDG_Private_Variables  WWDG 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup WWDG_Private_FunctionPrototypes  WWDG 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup WWDG_Private_Functions  WWDG 私有函数
  * @{
  */

/**
  * @brief  将 WWDG 外设寄存器反初始化为其默认复位值。
  * @param  无
  * @retval 无
  */
void WWDG_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, DISABLE);
}

/**
  * @brief  设置 WWDG 预分频器。
  * @param  WWDG_Prescaler: 指定 WWDG 预分频器。
  *   该参数可取以下值之一：
  *     @arg WWDG_Prescaler_1: WWDG 计数器时钟 = (PCLK1/4096)/1
  *     @arg WWDG_Prescaler_2: WWDG 计数器时钟 = (PCLK1/4096)/2
  *     @arg WWDG_Prescaler_4: WWDG 计数器时钟 = (PCLK1/4096)/4
  *     @arg WWDG_Prescaler_8: WWDG 计数器时钟 = (PCLK1/4096)/8
  * @retval 无
  */
void WWDG_SetPrescaler(uint32_t WWDG_Prescaler)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_WWDG_PRESCALER(WWDG_Prescaler));
  /* 清零 WDGTB[1:0] 位 */
  tmpreg = WWDG->CFR & CFR_WDGTB_Mask;
  /* 根据 WWDG_Prescaler 的值置位 WDGTB[1:0] 位 */
  tmpreg |= WWDG_Prescaler;
  /* 保存新值 */
  WWDG->CFR = tmpreg;
}

/**
  * @brief  设置 WWDG 窗口值。
  * @param  WindowValue: 指定要与递减计数器比较的窗口值。
  *   该参数值必须小于 0x80。
  * @retval 无
  */
void WWDG_SetWindowValue(uint8_t WindowValue)
{
  __IO uint32_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_WWDG_WINDOW_VALUE(WindowValue));
  /* 清零 W[6:0] 位 */

  tmpreg = WWDG->CFR & CFR_W_Mask;

  /* 根据 WindowValue 的值置位 W[6:0] 位 */
  tmpreg |= WindowValue & (uint32_t) BIT_Mask;

  /* 保存新值 */
  WWDG->CFR = tmpreg;
}

/**
  * @brief  使能 WWDG 提前唤醒中断（EWI）。
  * @param  无
  * @retval 无
  */
void WWDG_EnableIT(void)
{
  *(__IO uint32_t *) CFR_EWI_BB = (uint32_t)ENABLE;
}

/**
  * @brief  设置 WWDG 计数器值。
  * @param  Counter: 指定看门狗计数器值。
  *   该参数必须为 0x40 到 0x7F 之间的数字。
  * @retval 无
  */
void WWDG_SetCounter(uint8_t Counter)
{
  /* 检查参数 */
  assert_param(IS_WWDG_COUNTER(Counter));
  /* 写入 T[6:0] 位以配置计数器值，无需进行
     读-改-写；向 WDGA 位写 0 没有任何作用 */
  WWDG->CR = Counter & BIT_Mask;
}

/**
  * @brief  使能 WWDG 并装入计数器值。
  * @param  Counter: 指定看门狗计数器值。
  *   该参数必须为 0x40 到 0x7F 之间的数字。
  * @retval 无
  */
void WWDG_Enable(uint8_t Counter)
{
  /* 检查参数 */
  assert_param(IS_WWDG_COUNTER(Counter));
  WWDG->CR = CR_WDGA_Set | Counter;
}

/**
  * @brief  检查提前唤醒中断标志是否置位。
  * @param  无
  * @retval 提前唤醒中断标志的新状态（SET 或 RESET）
  */
FlagStatus WWDG_GetFlagStatus(void)
{
  return (FlagStatus)(WWDG->SR);
}

/**
  * @brief  清零提前唤醒中断标志。
  * @param  无
  * @retval 无
  */
void WWDG_ClearFlag(void)
{
  WWDG->SR = (uint32_t)RESET;
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

/**
  ******************************************************************************
  * @file    stm32f10x_exti.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供所有 EXTI 固件函数。
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
#include "stm32f10x_exti.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup EXTI
  * @brief EXTI 驱动模块
  * @{
  */

/** @defgroup EXTI_Private_TypesDefinitions   EXTI 私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup EXTI_Private_Defines   EXTI 私有宏定义
  * @{
  */

#define EXTI_LINENONE    ((uint32_t)0x00000)  /* 未选择中断 */

/**
  * @}
  */

/** @defgroup EXTI_Private_Macros   EXTI 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup EXTI_Private_Variables   EXTI 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup EXTI_Private_FunctionPrototypes   EXTI 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup EXTI_Private_Functions   EXTI 私有函数
  * @{
  */

/**
  * @brief  将 EXTI 外设寄存器反初始化为默认复位值。
  * @param  无
  * @retval 无
  */
void EXTI_DeInit(void)
{
  EXTI->IMR = 0x00000000;
  EXTI->EMR = 0x00000000;
  EXTI->RTSR = 0x00000000; 
  EXTI->FTSR = 0x00000000; 
  EXTI->PR = 0x000FFFFF;
}

/**
  * @brief  根据 EXTI_InitStruct 中指定的参数初始化 EXTI 外设。
  * @param  EXTI_InitStruct: 指向 EXTI_InitTypeDef 结构的指针，
  *         该结构包含 EXTI 外设的配置信息。
  * @retval 无
  */
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct)
{
  uint32_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
  assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
  assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));  
  assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));

  tmp = (uint32_t)EXTI_BASE;
     
  if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
  {
    /* 清除 EXTI 线配置 */
    EXTI->IMR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->EMR &= ~EXTI_InitStruct->EXTI_Line;
    
    tmp += EXTI_InitStruct->EXTI_Mode;

    *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;

    /* 清除上升沿下降沿配置 */
    EXTI->RTSR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->FTSR &= ~EXTI_InitStruct->EXTI_Line;
    
    /* 为所选外部中断选择触发方式 */
    if (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising_Falling)
    {
      /* 上升沿下降沿 */
      EXTI->RTSR |= EXTI_InitStruct->EXTI_Line;
      EXTI->FTSR |= EXTI_InitStruct->EXTI_Line;
    }
    else
    {
      tmp = (uint32_t)EXTI_BASE;
      tmp += EXTI_InitStruct->EXTI_Trigger;

      *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;
    }
  }
  else
  {
    tmp += EXTI_InitStruct->EXTI_Mode;

    /* 关闭所选外部中断线 */
    *(__IO uint32_t *) tmp &= ~EXTI_InitStruct->EXTI_Line;
  }
}

/**
  * @brief  将 EXTI_InitStruct 的每个成员填充为复位值。
  * @param  EXTI_InitStruct: 指向将被初始化的 EXTI_InitTypeDef 结构的指针。
  * @retval 无
  */
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct)
{
  EXTI_InitStruct->EXTI_Line = EXTI_LINENONE;
  EXTI_InitStruct->EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct->EXTI_LineCmd = DISABLE;
}

/**
  * @brief  产生一个软件中断。
  * @param  EXTI_Line: 指定要使能或关闭的 EXTI 线。
  *   该参数可以是 EXTI_Linex 的任意组合，其中 x 可取 (0..19)。
  * @retval 无
  */
void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line)
{
  /* 检查参数 */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->SWIER |= EXTI_Line;
}

/**
  * @brief  检查指定的 EXTI 线标志是否被置位。
  * @param  EXTI_Line: 指定要检查的 EXTI 线标志。
  *   该参数可取：
  *     @arg EXTI_Linex: 外部中断线 x，其中 x(0..19)
  * @retval EXTI_Line 的新状态 (SET 或 RESET)。
  */
FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_GET_EXTI_LINE(EXTI_Line));
  
  if ((EXTI->PR & EXTI_Line) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  清除 EXTI 线的挂起标志。
  * @param  EXTI_Line: 指定要清除的 EXTI 线标志。
  *   该参数可以是 EXTI_Linex 的任意组合，其中 x 可取 (0..19)。
  * @retval 无
  */
void EXTI_ClearFlag(uint32_t EXTI_Line)
{
  /* 检查参数 */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;
}

/**
  * @brief  检查指定的 EXTI 线是否被触发。
  * @param  EXTI_Line: 指定要检查的 EXTI 线。
  *   该参数可取：
  *     @arg EXTI_Linex: 外部中断线 x，其中 x(0..19)
  * @retval EXTI_Line 的新状态 (SET 或 RESET)。
  */
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  /* 检查参数 */
  assert_param(IS_GET_EXTI_LINE(EXTI_Line));
  
  enablestatus =  EXTI->IMR & EXTI_Line;
  if (((EXTI->PR & EXTI_Line) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  清除 EXTI 线的挂起位。
  * @param  EXTI_Line: 指定要清除的 EXTI 线。
  *   该参数可以是 EXTI_Linex 的任意组合，其中 x 可取 (0..19)。
  * @retval 无
  */
void EXTI_ClearITPendingBit(uint32_t EXTI_Line)
{
  /* 检查参数 */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;
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

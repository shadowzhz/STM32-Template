/**
  ******************************************************************************
  * @file    stm32f10x_rtc.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供 RTC 的所有固件函数。
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
#include "stm32f10x_rtc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup RTC
  * @brief RTC 驱动模块
  * @{
  */

/** @defgroup RTC_Private_TypesDefinitions   RTC 私有类型定义
  * @{
  */ 
/**
  * @}
  */

/** @defgroup RTC_Private_Defines   RTC 私有宏定义
  * @{
  */
#define RTC_LSB_MASK     ((uint32_t)0x0000FFFF)  /*!< RTC LSB 掩码 */
#define PRLH_MSB_MASK    ((uint32_t)0x000F0000)  /*!< RTC 预分频器 MSB 掩码 */

/**
  * @}
  */

/** @defgroup RTC_Private_Macros   RTC 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup RTC_Private_Variables   RTC 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup RTC_Private_FunctionPrototypes   RTC 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup RTC_Private_Functions   RTC 私有函数
  * @{
  */

/**
  * @brief  使能或关闭指定的 RTC 中断。
  * @param  RTC_IT: 指定要使能或关闭的 RTC 中断源。
  *   该参数可取以下值的任意组合：
  *     @arg RTC_IT_OW: 溢出中断
  *     @arg RTC_IT_ALR: 闹钟中断
  *     @arg RTC_IT_SEC: 秒中断
  * @param  NewState: 指定 RTC 中断的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RTC_ITConfig(uint16_t RTC_IT, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RTC_IT(RTC_IT));  
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    RTC->CRH |= RTC_IT;
  }
  else
  {
    RTC->CRH &= (uint16_t)~RTC_IT;
  }
}

/**
  * @brief  进入 RTC 配置模式。
  * @param  None
  * @retval 无
  */
void RTC_EnterConfigMode(void)
{
  /* 设置 CNF 标志以进入配置模式 */
  RTC->CRL |= RTC_CRL_CNF;
}

/**
  * @brief  退出 RTC 配置模式。
  * @param  None
  * @retval 无
  */
void RTC_ExitConfigMode(void)
{
  /* 复位 CNF 标志以退出配置模式 */
  RTC->CRL &= (uint16_t)~((uint16_t)RTC_CRL_CNF); 
}

/**
  * @brief  获取 RTC 计数器值。
  * @param  None
  * @retval RTC 计数器值。
  */
uint32_t RTC_GetCounter(void)
{
  uint16_t tmp = 0;
  tmp = RTC->CNTL;
  return (((uint32_t)RTC->CNTH << 16 ) | tmp) ;
}

/**
  * @brief  设置 RTC 计数器值。
  * @param  CounterValue: RTC 计数器新值。
  * @retval 无
  */
void RTC_SetCounter(uint32_t CounterValue)
{ 
  RTC_EnterConfigMode();
  /* 设置 RTC 计数器 MSB 字 */
  RTC->CNTH = CounterValue >> 16;
  /* 设置 RTC 计数器 LSB 字 */
  RTC->CNTL = (CounterValue & RTC_LSB_MASK);
  RTC_ExitConfigMode();
}

/**
  * @brief  设置 RTC 预分频器值。
  * @param  PrescalerValue: RTC 预分频器新值。
  * @retval 无
  */
void RTC_SetPrescaler(uint32_t PrescalerValue)
{
  /* 检查参数 */
  assert_param(IS_RTC_PRESCALER(PrescalerValue));
  
  RTC_EnterConfigMode();
  /* 设置 RTC 预分频器 MSB 字 */
  RTC->PRLH = (PrescalerValue & PRLH_MSB_MASK) >> 16;
  /* 设置 RTC 预分频器 LSB 字 */
  RTC->PRLL = (PrescalerValue & RTC_LSB_MASK);
  RTC_ExitConfigMode();
}

/**
  * @brief  设置 RTC 闹钟值。
  * @param  AlarmValue: RTC 闹钟新值。
  * @retval 无
  */
void RTC_SetAlarm(uint32_t AlarmValue)
{  
  RTC_EnterConfigMode();
  /* 设置 ALARM MSB 字 */
  RTC->ALRH = AlarmValue >> 16;
  /* 设置 ALARM LSB 字 */
  RTC->ALRL = (AlarmValue & RTC_LSB_MASK);
  RTC_ExitConfigMode();
}

/**
  * @brief  获取 RTC 分频器值。
  * @param  None
  * @retval RTC 分频器值。
  */
uint32_t RTC_GetDivider(void)
{
  uint32_t tmp = 0x00;
  tmp = ((uint32_t)RTC->DIVH & (uint32_t)0x000F) << 16;
  tmp |= RTC->DIVL;
  return tmp;
}

/**
  * @brief  等待对 RTC 寄存器的上一次写操作完成。
  * @note   在对 RTC 寄存器进行任何写操作之前都必须调用本函数。
  * @param  None
  * @retval 无
  */
void RTC_WaitForLastTask(void)
{
  /* 循环等待直到 RTOFF 标志置位 */
  while ((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET)
  {
  }
}

/**
  * @brief  等待 RTC 寄存器（RTC_CNT、RTC_ALR 和 RTC_PRL）
  *   与 RTC APB 时钟同步。
  * @note   在 APB 复位或 APB 时钟停止之后，进行任何读操作之前都必须
  *   调用本函数。
  * @param  None
  * @retval 无
  */
void RTC_WaitForSynchro(void)
{
  /* 清除 RSF 标志 */
  RTC->CRL &= (uint16_t)~RTC_FLAG_RSF;
  /* 循环等待直到 RSF 标志置位 */
  while ((RTC->CRL & RTC_FLAG_RSF) == (uint16_t)RESET)
  {
  }
}

/**
  * @brief  检查指定的 RTC 标志是否置位。
  * @param  RTC_FLAG: 指定要检查的标志。
  *   该参数可取以下值之一：
  *     @arg RTC_FLAG_RTOFF: RTC 操作关闭标志
  *     @arg RTC_FLAG_RSF: 寄存器同步标志
  *     @arg RTC_FLAG_OW: 溢出标志
  *     @arg RTC_FLAG_ALR: 闹钟标志
  *     @arg RTC_FLAG_SEC: 秒标志
  * @retval RTC_FLAG 的新状态（SET 或 RESET）。
  */
FlagStatus RTC_GetFlagStatus(uint16_t RTC_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  /* 检查参数 */
  assert_param(IS_RTC_GET_FLAG(RTC_FLAG)); 
  
  if ((RTC->CRL & RTC_FLAG) != (uint16_t)RESET)
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
  * @brief  清除 RTC 的挂起标志。
  * @param  RTC_FLAG: 指定要清除的标志。
  *   该参数可取以下值的任意组合：
  *     @arg RTC_FLAG_RSF: 寄存器同步标志。该标志仅在 APB 复位或
  *                        APB 时钟停止后才会被清除。
  *     @arg RTC_FLAG_OW: 溢出标志
  *     @arg RTC_FLAG_ALR: 闹钟标志
  *     @arg RTC_FLAG_SEC: 秒标志
  * @retval 无
  */
void RTC_ClearFlag(uint16_t RTC_FLAG)
{
  /* 检查参数 */
  assert_param(IS_RTC_CLEAR_FLAG(RTC_FLAG)); 
    
  /* 清除相应的 RTC 标志 */
  RTC->CRL &= (uint16_t)~RTC_FLAG;
}

/**
  * @brief  检查指定的 RTC 中断是否发生。
  * @param  RTC_IT: 指定要检查的 RTC 中断源。
  *   该参数可取以下值之一：
  *     @arg RTC_IT_OW: 溢出中断
  *     @arg RTC_IT_ALR: 闹钟中断
  *     @arg RTC_IT_SEC: 秒中断
  * @retval RTC_IT 的新状态（SET 或 RESET）。
  */
ITStatus RTC_GetITStatus(uint16_t RTC_IT)
{
  ITStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_RTC_GET_IT(RTC_IT)); 
  
  bitstatus = (ITStatus)(RTC->CRL & RTC_IT);
  if (((RTC->CRH & RTC_IT) != (uint16_t)RESET) && (bitstatus != (uint16_t)RESET))
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
  * @brief  清除 RTC 的中断挂起位。
  * @param  RTC_IT: 指定要清除的中断挂起位。
  *   该参数可取以下值的任意组合：
  *     @arg RTC_IT_OW: 溢出中断
  *     @arg RTC_IT_ALR: 闹钟中断
  *     @arg RTC_IT_SEC: 秒中断
  * @retval 无
  */
void RTC_ClearITPendingBit(uint16_t RTC_IT)
{
  /* 检查参数 */
  assert_param(IS_RTC_IT(RTC_IT));  
  
  /* 清除相应的 RTC 挂起位 */
  RTC->CRL &= (uint16_t)~RTC_IT;
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

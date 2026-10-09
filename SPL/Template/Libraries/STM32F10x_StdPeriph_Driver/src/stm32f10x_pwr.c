/**
  ******************************************************************************
  * @file    stm32f10x_pwr.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供 PWR 的所有固件函数。
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
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup PWR
  * @brief PWR 驱动模块
  * @{
  */ 

/** @defgroup PWR_Private_TypesDefinitions   PWR 私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup PWR_Private_Defines   PWR 私有宏定义
  * @{
  */

/* --------- PWR 寄存器在别名区中的位地址 ---------- */
#define PWR_OFFSET               (PWR_BASE - PERIPH_BASE)

/* --- CR 寄存器 ---*/

/* DBP 位的别名写字地址 */
#define CR_OFFSET                (PWR_OFFSET + 0x00)
#define DBP_BitNumber            0x08
#define CR_DBP_BB                (PERIPH_BB_BASE + (CR_OFFSET * 32) + (DBP_BitNumber * 4))

/* PVDE 位的别名写字地址 */
#define PVDE_BitNumber           0x04
#define CR_PVDE_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PVDE_BitNumber * 4))

/* --- CSR 寄存器 ---*/

/* EWUP 位的别名写字地址 */
#define CSR_OFFSET               (PWR_OFFSET + 0x04)
#define EWUP_BitNumber           0x08
#define CSR_EWUP_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (EWUP_BitNumber * 4))

/* ------------------ PWR 寄存器位掩码 ------------------------ */

/* CR 寄存器位掩码 */
#define CR_DS_MASK               ((uint32_t)0xFFFFFFFC)
#define CR_PLS_MASK              ((uint32_t)0xFFFFFF1F)


/**
  * @}
  */

/** @defgroup PWR_Private_Macros   PWR 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup PWR_Private_Variables   PWR 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup PWR_Private_FunctionPrototypes   PWR 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup PWR_Private_Functions   PWR 私有函数
  * @{
  */

/**
  * @brief  将 PWR 外设寄存器反初始化为它们的默认复位值。
  * @param  None
  * @retval 无
  */
void PWR_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, DISABLE);
}

/**
  * @brief  使能或关闭对 RTC 和备份寄存器的访问。
  * @param  NewState: 对 RTC 和备份寄存器访问的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void PWR_BackupAccessCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_DBP_BB = (uint32_t)NewState;
}

/**
  * @brief  使能或关闭电源电压检测器（PVD）。
  * @param  NewState: PVD 的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void PWR_PVDCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_PVDE_BB = (uint32_t)NewState;
}

/**
  * @brief  配置电源电压检测器（PVD）检测的电压阈值。
  * @param  PWR_PVDLevel: 指定 PVD 检测电平
  *   该参数可取以下值之一：
  *     @arg PWR_PVDLevel_2V2: PVD 检测电平设置为 2.2V
  *     @arg PWR_PVDLevel_2V3: PVD 检测电平设置为 2.3V
  *     @arg PWR_PVDLevel_2V4: PVD 检测电平设置为 2.4V
  *     @arg PWR_PVDLevel_2V5: PVD 检测电平设置为 2.5V
  *     @arg PWR_PVDLevel_2V6: PVD 检测电平设置为 2.6V
  *     @arg PWR_PVDLevel_2V7: PVD 检测电平设置为 2.7V
  *     @arg PWR_PVDLevel_2V8: PVD 检测电平设置为 2.8V
  *     @arg PWR_PVDLevel_2V9: PVD 检测电平设置为 2.9V
  * @retval 无
  */
void PWR_PVDLevelConfig(uint32_t PWR_PVDLevel)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_PWR_PVD_LEVEL(PWR_PVDLevel));
  tmpreg = PWR->CR;
  /* 清除 PLS[7:5] 位 */
  tmpreg &= CR_PLS_MASK;
  /* 根据 PWR_PVDLevel 值设置 PLS[7:5] 位 */
  tmpreg |= PWR_PVDLevel;
  /* 保存新值 */
  PWR->CR = tmpreg;
}

/**
  * @brief  使能或关闭唤醒引脚功能。
  * @param  NewState: 唤醒引脚功能的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void PWR_WakeUpPinCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CSR_EWUP_BB = (uint32_t)NewState;
}

/**
  * @brief  进入 STOP 模式。
  * @param  PWR_Regulator: 指定 STOP 模式下的稳压器状态。
  *   该参数可取以下值之一：
  *     @arg PWR_Regulator_ON: 稳压器开启的 STOP 模式
  *     @arg PWR_Regulator_LowPower: 稳压器处于低功耗模式的 STOP 模式
  * @param  PWR_STOPEntry: 指定使用 WFI 还是 WFE 指令进入 STOP 模式。
  *   该参数可取以下值之一：
  *     @arg PWR_STOPEntry_WFI: 使用 WFI 指令进入 STOP 模式
  *     @arg PWR_STOPEntry_WFE: 使用 WFE 指令进入 STOP 模式
  * @retval 无
  */
void PWR_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_PWR_REGULATOR(PWR_Regulator));
  assert_param(IS_PWR_STOP_ENTRY(PWR_STOPEntry));
  
  /* 选择 STOP 模式下的稳压器状态 ---------------------------------*/
  tmpreg = PWR->CR;
  /* 清除 PDDS 和 LPDS 位 */
  tmpreg &= CR_DS_MASK;
  /* 根据 PWR_Regulator 值设置 LPDS 位 */
  tmpreg |= PWR_Regulator;
  /* 保存新值 */
  PWR->CR = tmpreg;
  /* 设置 Cortex 系统控制寄存器的 SLEEPDEEP 位 */
  SCB->SCR |= SCB_SCR_SLEEPDEEP;
  
  /* 选择 STOP 模式进入方式 --------------------------------------------------*/
  if(PWR_STOPEntry == PWR_STOPEntry_WFI)
  {   
    /* 请求等待中断 */
    __WFI();
  }
  else
  {
    /* 请求等待事件 */
    __WFE();
  }
  
  /* 复位 Cortex 系统控制寄存器的 SLEEPDEEP 位 */
  SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);  
}

/**
  * @brief  进入 STANDBY 模式。
  * @param  None
  * @retval 无
  */
void PWR_EnterSTANDBYMode(void)
{
  /* 清除唤醒标志 */
  PWR->CR |= PWR_CR_CWUF;
  /* 选择 STANDBY 模式 */
  PWR->CR |= PWR_CR_PDDS;
  /* 设置 Cortex 系统控制寄存器的 SLEEPDEEP 位 */
  SCB->SCR |= SCB_SCR_SLEEPDEEP;
/* 此选项用于确保存储操作已完成 */
#if defined ( __CC_ARM   )
  __force_stores();
#endif
  /* 请求等待中断 */
  __WFI();
}

/**
  * @brief  检查指定的 PWR 标志是否置位。
  * @param  PWR_FLAG: 指定要检查的标志。
  *   该参数可取以下值之一：
  *     @arg PWR_FLAG_WU: 唤醒标志
  *     @arg PWR_FLAG_SB: 待机标志
  *     @arg PWR_FLAG_PVDO: PVD 输出
  * @retval PWR_FLAG 的新状态（SET 或 RESET）。
  */
FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_PWR_GET_FLAG(PWR_FLAG));
  
  if ((PWR->CSR & PWR_FLAG) != (uint32_t)RESET)
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
  * @brief  清除 PWR 的挂起标志。
  * @param  PWR_FLAG: 指定要清除的标志。
  *   该参数可取以下值之一：
  *     @arg PWR_FLAG_WU: 唤醒标志
  *     @arg PWR_FLAG_SB: 待机标志
  * @retval 无
  */
void PWR_ClearFlag(uint32_t PWR_FLAG)
{
  /* 检查参数 */
  assert_param(IS_PWR_CLEAR_FLAG(PWR_FLAG));
         
  PWR->CR |=  PWR_FLAG << 2;
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

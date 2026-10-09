/**
  ******************************************************************************
  * @file    stm32f10x_bkp.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供全部 BKP 固件函数。
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

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_bkp.h"
#include "stm32f10x_rcc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup BKP   BKP 备份寄存器驱动
  * @brief BKP 驱动模块
  * @{
  */

/** @defgroup BKP_Private_TypesDefinitions   BKP 私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup BKP_Private_Defines   BKP 私有宏定义
  * @{
  */

/* ------------ 别名区中 BKP 寄存器的位地址 --------------- */
#define BKP_OFFSET        (BKP_BASE - PERIPH_BASE)

/* --- CR 寄存器 ----*/

/* TPAL 位的别名区字地址 */
#define CR_OFFSET         (BKP_OFFSET + 0x30)
#define TPAL_BitNumber    0x01
#define CR_TPAL_BB        (PERIPH_BB_BASE + (CR_OFFSET * 32) + (TPAL_BitNumber * 4))

/* TPE 位的别名区字地址 */
#define TPE_BitNumber     0x00
#define CR_TPE_BB         (PERIPH_BB_BASE + (CR_OFFSET * 32) + (TPE_BitNumber * 4))

/* --- CSR 寄存器 ---*/

/* TPIE 位的别名区字地址 */
#define CSR_OFFSET        (BKP_OFFSET + 0x34)
#define TPIE_BitNumber    0x02
#define CSR_TPIE_BB       (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TPIE_BitNumber * 4))

/* TIF 位的别名区字地址 */
#define TIF_BitNumber     0x09
#define CSR_TIF_BB        (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TIF_BitNumber * 4))

/* TEF 位的别名区字地址 */
#define TEF_BitNumber     0x08
#define CSR_TEF_BB        (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TEF_BitNumber * 4))

/* ---------------------- BKP 寄存器位掩码 ------------------------ */

/* RTCCR 寄存器位掩码 */
#define RTCCR_CAL_MASK    ((uint16_t)0xFF80)
#define RTCCR_MASK        ((uint16_t)0xFC7F)

/**
  * @}
  */ 


/** @defgroup BKP_Private_Macros   BKP 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup BKP_Private_Variables   BKP 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup BKP_Private_FunctionPrototypes   BKP 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup BKP_Private_Functions   BKP 私有函数
  * @{
  */

/**
  * @brief  将 BKP 外设寄存器反初始化，恢复为默认复位值。
  * @param  无
  * @retval 无
  */
void BKP_DeInit(void)
{
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
}

/**
  * @brief  配置侵入检测引脚的有效电平。
  * @param  BKP_TamperPinLevel: 指定侵入检测引脚的有效电平。
  *   该参数可取下列值之一：
  *     @arg BKP_TamperPinLevel_High: 侵入检测引脚高电平有效
  *     @arg BKP_TamperPinLevel_Low: 侵入检测引脚低电平有效
  * @retval 无
  */
void BKP_TamperPinLevelConfig(uint16_t BKP_TamperPinLevel)
{
  /* 检查参数 */
  assert_param(IS_BKP_TAMPER_PIN_LEVEL(BKP_TamperPinLevel));
  *(__IO uint32_t *) CR_TPAL_BB = BKP_TamperPinLevel;
}

/**
  * @brief  使能或关闭侵入检测引脚的激活。
  * @param  NewState: 侵入检测引脚激活的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void BKP_TamperPinCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_TPE_BB = (uint32_t)NewState;
}

/**
  * @brief  使能或关闭侵入检测引脚中断。
  * @param  NewState: 侵入检测引脚中断的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void BKP_ITConfig(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CSR_TPIE_BB = (uint32_t)NewState;
}

/**
  * @brief  选择在侵入检测引脚上输出的 RTC 输出源。
  * @param  BKP_RTCOutputSource: 指定 RTC 输出源。
  *   该参数可取下列值之一：
  *     @arg BKP_RTCOutputSource_None: 不在侵入检测引脚上输出 RTC 信号。
  *     @arg BKP_RTCOutputSource_CalibClock: 在侵入检测引脚上输出
  *                                          64 分频后的 RTC 时钟。
  *     @arg BKP_RTCOutputSource_Alarm: 在侵入检测引脚上输出
  *                                     RTC 闹钟脉冲信号。
  *     @arg BKP_RTCOutputSource_Second: 在侵入检测引脚上输出
  *                                      RTC 秒脉冲信号。
  * @retval 无
  */
void BKP_RTCOutputConfig(uint16_t BKP_RTCOutputSource)
{
  uint16_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_BKP_RTC_OUTPUT_SOURCE(BKP_RTCOutputSource));
  tmpreg = BKP->RTCCR;
  /* 清除 CCO、ASOE 与 ASOS 位 */
  tmpreg &= RTCCR_MASK;
  
  /* 根据 BKP_RTCOutputSource 的值设置 CCO、ASOE 与 ASOS 位 */
  tmpreg |= BKP_RTCOutputSource;
  /* 保存新值 */
  BKP->RTCCR = tmpreg;
}

/**
  * @brief  设置 RTC 时钟校准值。
  * @param  CalibrationValue: 指定 RTC 时钟校准值。
  *   该参数必须是 0 到 0x7F 之间的数值。
  * @retval 无
  */
void BKP_SetRTCCalibrationValue(uint8_t CalibrationValue)
{
  uint16_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_BKP_CALIBRATION_VALUE(CalibrationValue));
  tmpreg = BKP->RTCCR;
  /* 清除 CAL[6:0] 位 */
  tmpreg &= RTCCR_CAL_MASK;
  /* 根据 CalibrationValue 的值设置 CAL[6:0] 位 */
  tmpreg |= CalibrationValue;
  /* 保存新值 */
  BKP->RTCCR = tmpreg;
}

/**
  * @brief  向指定的数据备份寄存器写入用户数据。
  * @param  BKP_DR: 指定数据备份寄存器。
  *   该参数可取 BKP_DRx，其中 x 属于 [1, 42]
  * @param  Data: 待写入的数据
  * @retval 无
  */
void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data)
{
  __IO uint32_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_BKP_DR(BKP_DR));

  tmp = (uint32_t)BKP_BASE; 
  tmp += BKP_DR;

  *(__IO uint32_t *) tmp = Data;
}

/**
  * @brief  从指定的数据备份寄存器读取数据。
  * @param  BKP_DR: 指定数据备份寄存器。
  *   该参数可取 BKP_DRx，其中 x 属于 [1, 42]
  * @retval 指定数据备份寄存器的内容
  */
uint16_t BKP_ReadBackupRegister(uint16_t BKP_DR)
{
  __IO uint32_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_BKP_DR(BKP_DR));

  tmp = (uint32_t)BKP_BASE; 
  tmp += BKP_DR;

  return (*(__IO uint16_t *) tmp);
}

/**
  * @brief  检查侵入检测引脚事件标志是否已置位。
  * @param  无
  * @retval 侵入检测引脚事件标志的新状态（SET 或 RESET）。
  */
FlagStatus BKP_GetFlagStatus(void)
{
  return (FlagStatus)(*(__IO uint32_t *) CSR_TEF_BB);
}

/**
  * @brief  清除侵入检测引脚事件的待处理标志。
  * @param  无
  * @retval 无
  */
void BKP_ClearFlag(void)
{
  /* 置位 CTE 位以清除侵入检测引脚事件标志 */
  BKP->CSR |= BKP_CSR_CTE;
}

/**
  * @brief  检查侵入检测引脚中断是否已经发生。
  * @param  无
  * @retval 侵入检测引脚中断的新状态（SET 或 RESET）。
  */
ITStatus BKP_GetITStatus(void)
{
  return (ITStatus)(*(__IO uint32_t *) CSR_TIF_BB);
}

/**
  * @brief  清除侵入检测引脚中断的待处理位。
  * @param  无
  * @retval 无
  */
void BKP_ClearITPendingBit(void)
{
  /* 置位 CTI 位以清除侵入检测引脚中断的待处理位 */
  BKP->CSR |= BKP_CSR_CTI;
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

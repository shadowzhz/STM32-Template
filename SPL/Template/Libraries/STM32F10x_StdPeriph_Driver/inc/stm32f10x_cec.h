/**
  ******************************************************************************
  * @file    stm32f10x_cec.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 CEC 固件库的全部函数原型。
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
#ifndef __STM32F10x_CEC_H
#define __STM32F10x_CEC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup CEC   CEC 消费电子控制
  * @{
  */
  

/** @defgroup CEC_Exported_Types   CEC 导出类型
  * @{
  */
   
/**
  * @brief  CEC 初始化结构体定义
  */ 
typedef struct
{
  uint16_t CEC_BitTimingMode; /*!< 配置 CEC 的位时序错误模式。
                               该参数可取 @ref CEC_BitTiming_Mode 中的值 */
  uint16_t CEC_BitPeriodMode; /*!< 配置 CEC 的位周期错误模式。
                               该参数可取 @ref CEC_BitPeriod_Mode 中的值 */
}CEC_InitTypeDef;

/**
  * @}
  */

/** @defgroup CEC_Exported_Constants   CEC 导出常量
  * @{
  */ 
  
/** @defgroup CEC_BitTiming_Mode   位时序模式
  * @{
  */ 
#define CEC_BitTimingStdMode                    ((uint16_t)0x00) /*!< 位时序错误 标准模式 */
#define CEC_BitTimingErrFreeMode                CEC_CFGR_BTEM   /*!< 位时序错误 自由模式 */

#define IS_CEC_BIT_TIMING_ERROR_MODE(MODE) (((MODE) == CEC_BitTimingStdMode) || \
                                            ((MODE) == CEC_BitTimingErrFreeMode))
/**
  * @}
  */

/** @defgroup CEC_BitPeriod_Mode   位周期模式
  * @{
  */ 
#define CEC_BitPeriodStdMode                    ((uint16_t)0x00) /*!< 位周期错误 标准模式 */
#define CEC_BitPeriodFlexibleMode                CEC_CFGR_BPEM   /*!< 位周期错误 灵活模式 */

#define IS_CEC_BIT_PERIOD_ERROR_MODE(MODE) (((MODE) == CEC_BitPeriodStdMode) || \
                                            ((MODE) == CEC_BitPeriodFlexibleMode))
/**
  * @}
  */ 


/** @defgroup CEC_interrupts_definition   中断定义
  * @{
  */ 
#define CEC_IT_TERR                              CEC_CSR_TERR
#define CEC_IT_TBTRF                             CEC_CSR_TBTRF
#define CEC_IT_RERR                              CEC_CSR_RERR
#define CEC_IT_RBTF                              CEC_CSR_RBTF
#define IS_CEC_GET_IT(IT) (((IT) == CEC_IT_TERR) || ((IT) == CEC_IT_TBTRF) || \
                           ((IT) == CEC_IT_RERR) || ((IT) == CEC_IT_RBTF))
/**
  * @}
  */ 


/** @defgroup CEC_Own_Address   自身地址
  * @{
  */ 
#define IS_CEC_ADDRESS(ADDRESS) ((ADDRESS) < 0x10)
/**
  * @}
  */ 

/** @defgroup CEC_Prescaler   预分频器
  * @{
  */ 
#define IS_CEC_PRESCALER(PRESCALER) ((PRESCALER) <= 0x3FFF)

/**
  * @}
  */

/** @defgroup CEC_flags_definition   标志定义
  * @{
  */
   
/**
  * @brief  ESR 寄存器标志
  */ 
#define CEC_FLAG_BTE                            ((uint32_t)0x10010000)
#define CEC_FLAG_BPE                            ((uint32_t)0x10020000)
#define CEC_FLAG_RBTFE                          ((uint32_t)0x10040000)
#define CEC_FLAG_SBE                            ((uint32_t)0x10080000)
#define CEC_FLAG_ACKE                           ((uint32_t)0x10100000)
#define CEC_FLAG_LINE                           ((uint32_t)0x10200000)
#define CEC_FLAG_TBTFE                          ((uint32_t)0x10400000)

/**
  * @brief  CSR 寄存器标志
  */ 
#define CEC_FLAG_TEOM                           ((uint32_t)0x00000002)  
#define CEC_FLAG_TERR                           ((uint32_t)0x00000004)
#define CEC_FLAG_TBTRF                          ((uint32_t)0x00000008)
#define CEC_FLAG_RSOM                           ((uint32_t)0x00000010)
#define CEC_FLAG_REOM                           ((uint32_t)0x00000020)
#define CEC_FLAG_RERR                           ((uint32_t)0x00000040)
#define CEC_FLAG_RBTF                           ((uint32_t)0x00000080)

#define IS_CEC_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFFFFFF03) == 0x00) && ((FLAG) != 0x00))
                               
#define IS_CEC_GET_FLAG(FLAG) (((FLAG) == CEC_FLAG_BTE) || ((FLAG) == CEC_FLAG_BPE) || \
                               ((FLAG) == CEC_FLAG_RBTFE) || ((FLAG)== CEC_FLAG_SBE) || \
                               ((FLAG) == CEC_FLAG_ACKE) || ((FLAG) == CEC_FLAG_LINE) || \
                               ((FLAG) == CEC_FLAG_TBTFE) || ((FLAG) == CEC_FLAG_TEOM) || \
                               ((FLAG) == CEC_FLAG_TERR) || ((FLAG) == CEC_FLAG_TBTRF) || \
                               ((FLAG) == CEC_FLAG_RSOM) || ((FLAG) == CEC_FLAG_REOM) || \
                               ((FLAG) == CEC_FLAG_RERR) || ((FLAG) == CEC_FLAG_RBTF))

/**
  * @}
  */ 

/**
  * @}
  */ 

/** @defgroup CEC_Exported_Macros   CEC 导出宏
  * @{
  */
 
/**
  * @}
  */

/** @defgroup CEC_Exported_Functions   CEC 导出函数
  * @{
  */ 
void CEC_DeInit(void);
void CEC_Init(CEC_InitTypeDef* CEC_InitStruct);
void CEC_Cmd(FunctionalState NewState);
void CEC_ITConfig(FunctionalState NewState);
void CEC_OwnAddressConfig(uint8_t CEC_OwnAddress);
void CEC_SetPrescaler(uint16_t CEC_Prescaler);
void CEC_SendDataByte(uint8_t Data);
uint8_t CEC_ReceiveDataByte(void);
void CEC_StartOfMessage(void);
void CEC_EndOfMessageCmd(FunctionalState NewState);
FlagStatus CEC_GetFlagStatus(uint32_t CEC_FLAG);
void CEC_ClearFlag(uint32_t CEC_FLAG);
ITStatus CEC_GetITStatus(uint8_t CEC_IT);
void CEC_ClearITPendingBit(uint16_t CEC_IT);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_CEC_H */

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

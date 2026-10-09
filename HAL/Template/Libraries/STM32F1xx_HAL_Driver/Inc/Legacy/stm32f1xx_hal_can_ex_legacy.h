/**
  ******************************************************************************
  * @file    stm32f1xx_hal_can_ex_legacy.h
  * @author  MCD 应用团队
  * @brief   CAN HAL 扩展模块头文件。
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * 保留所有权利。
  *
  * 本软件根据可在本软件组件根目录下的 LICENSE 文件中找到的条款进行许可。
  * 如果本软件未随附 LICENSE 文件，则按“原样”提供。
  *
  ******************************************************************************
  */ 

/* 防止递归包含的宏定义 -----------------------------------------------------*/
#ifndef __STM32F1xx_HAL_CAN_EX_LEGACY_H
#define __STM32F1xx_HAL_CAN_EX_LEGACY_H

#ifdef __cplusplus
 extern "C" {
#endif

#if defined(STM32F103x6) || defined(STM32F103xB) || defined(STM32F103xE) || \
    defined(STM32F103xG) || defined(STM32F105xC) || defined(STM32F107xC)

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup CANEx CANEx
  * @{
  */ 

/* 导出的类型 ----------------------------------------------------------------*/

/** 
  * @brief  CAN 过滤器配置结构体定义
  */
/* 不同 STM32F1 器件之间的 CAN 过滤器组差异：                                 */
/* - STM32F1 互联型产品：28 个过滤器组，由 CAN1 和 CAN2 共享                  */
/* - 其他 STM32F10x 器件：14 个过滤器组                                       */

typedef struct
{
  uint32_t FilterIdHigh;          /*!< 指定过滤器标识符（32 位配置下的高位字 (MSB)，
                                       16 位配置下的第一个标识符）。
                                       该参数必须是介于 Min_Data = 0x0000 和 Max_Data = 0xFFFF 之间的数值。 */ 
                                              
  uint32_t FilterIdLow;           /*!< 指定过滤器标识符（32 位配置下的低位字 (LSB)，
                                       16 位配置下的第二个标识符）。
                                       该参数必须是介于 Min_Data = 0x0000 和 Max_Data = 0xFFFF 之间的数值。 */ 

  uint32_t FilterMaskIdHigh;      /*!< 根据配置模式指定过滤器屏蔽码或标识符
                                       （32 位配置下的高位字 (MSB)，
                                       16 位配置下的第一个标识符/屏蔽码）。
                                       该参数必须是介于 Min_Data = 0x0000 和 Max_Data = 0xFFFF 之间的数值。 */ 

  uint32_t FilterMaskIdLow;       /*!< 根据配置模式指定过滤器屏蔽码或标识符
                                       （32 位配置下的低位字 (LSB)，
                                       16 位配置下的第二个标识符/屏蔽码）。
                                       该参数必须是介于 Min_Data = 0x0000 和 Max_Data = 0xFFFF 之间的数值。 */ 

  uint32_t FilterFIFOAssignment;  /*!< 指定将分配给该过滤器的 FIFO（0 或 1）。
                                       该参数可以是 @ref CAN_filter_FIFO 的一个值 */
#if defined(STM32F105xC) || defined(STM32F107xC)
  uint32_t FilterNumber;          /*!< 指定将要初始化的过滤器。
                                       此参数必须是介于 Min_Data = 0 到 Max_Data = 27 之间的数值。 */
#else
  uint32_t FilterNumber;          /*!< 指定将要初始化的过滤器。
                                       此参数必须是介于 Min_Data = 0 到 Max_Data = 13 之间的数值。 */
#endif /* STM32F105xC || STM32F107xC */
  uint32_t FilterMode;            /*!< 指定将要初始化的过滤器模式。
                                       此参数可以是 @ref CAN_filter_mode 中的一个值 */

  uint32_t FilterScale;           /*!< 指定过滤器位宽（尺度）。
                                       此参数可以是 @ref CAN_filter_scale 中的一个值 */

  uint32_t FilterActivation;      /*!< 使能或禁止过滤器。
                                       此参数可设置为 ENABLE 或 DISABLE。 */
                                       
  uint32_t BankNumber;            /*!< 选择从机过滤器组起始编号
                                       此参数必须是介于 Min_Data = 0 到 Max_Data = 28 之间的数值。 */ 
  
}CAN_FilterConfTypeDef;

/* 导出常量 ------------------------------------------------------------------*/
/* 导出宏 --------------------------------------------------------------------*/
/* 私有宏 --------------------------------------------------------------------*/

/** @defgroup CANEx_Private_Macros CAN 扩展私有宏
  * @{
  */
#if defined(STM32F105xC) || defined(STM32F107xC)
#define IS_CAN_FILTER_NUMBER(NUMBER) ((NUMBER) <= 27U)
#else
#define IS_CAN_FILTER_NUMBER(NUMBER) ((NUMBER) <= 13U)
#endif /* STM32F105xC || STM32F107xC */

/**
  * @}
  */


/**
  * @}
  */ 

/**
  * @}
  */

#endif /* STM32F103x6) || STM32F103xB || STM32F103xE || STM32F103xG) || STM32F105xC || STM32F107xC    */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_CAN_EX_LEGACY_H */

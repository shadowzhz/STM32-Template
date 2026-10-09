/**
  ******************************************************************************
  * @file    stm32f1xx_hal_tim_ex.h
  * @author  MCD 应用团队
  * @brief   TIM HAL 扩展模块的头文件。
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) 2016 STMicroelectronics。
  * 保留所有权利。
  *
  * 本软件根据该软件组件根目录中 LICENSE 文件中的条款进行许可。
  * 如果本软件未随附 LICENSE 文件，则按“原样”提供。
  *
  ******************************************************************************
  */

/* 防止递归包含的宏定义 -----------------------------------------------------*/
#ifndef STM32F1xx_HAL_TIM_EX_H
#define STM32F1xx_HAL_TIM_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup TIMEx
  * @{
  */

/* 导出的类型 ----------------------------------------------------------------*/
/** @defgroup TIMEx_Exported_Types TIM 扩展导出类型
  * @{
  */

/**
  * @brief  TIM 霍尔传感器配置结构体定义
  */

typedef struct
{
  uint32_t IC1Polarity;         /*!< 指定输入信号的有效边沿。
                                     该参数可以是 @ref TIM_Input_Capture_Polarity 的取值 */

  uint32_t IC1Prescaler;        /*!< 指定输入捕获预分频器。
                                     该参数可以是 @ref TIM_Input_Capture_Prescaler 的取值 */

  uint32_t IC1Filter;           /*!< 指定输入捕获滤波器。
                                     该参数可以是介于 Min_Data = 0x0 和 Max_Data = 0xF 之间的数值 */

  uint32_t Commutation_Delay;   /*!< 指定要加载到捕获/比较寄存器中的脉冲值。
                                     该参数可以是介于 Min_Data = 0x0000 和 Max_Data = 0xFFFF 之间的数值 */
} TIM_HallSensor_InitTypeDef;
/**
  * @}
  */
/* 导出类型结束 --------------------------------------------------------------*/

/* 导出的常量 ----------------------------------------------------------------*/
/** @defgroup TIMEx_Exported_Constants TIM 扩展导出常量
  * @{
  */

/** @defgroup TIMEx_Remap TIM 扩展重映射
  * @{
  */
/**
  * @}
  */

/**
  * @}
  */
/* 导出常量结束 -------------------------------------------------------------*/

/* 导出宏 --------------------------------------------------------------------*/
/** @defgroup TIMEx_Exported_Macros TIM 扩展导出宏
  * @{
  */

/**
  * @}
  */
/* 导出宏结束 -----------------------------------------------------------------*/

/* 私有宏 --------------------------------------------------------------------*/
/** @defgroup TIMEx_Private_Macros TIM 扩展私有宏
  * @{
  */

/**
  * @}
  */
/* 私有宏结束 -----------------------------------------------------------------*/

/* 导出函数 ------------------------------------------------------------------*/
/** @addtogroup TIMEx_Exported_Functions TIM 扩展导出函数
  * @{
  */

/** @addtogroup TIMEx_Exported_Functions_Group1 定时器扩展霍尔传感器功能
  *  @brief    定时器霍尔传感器功能函数
  * @{
  */
/*  定时器霍尔传感器功能函数  **********************************************/
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Init(TIM_HandleTypeDef *htim, const TIM_HallSensor_InitTypeDef *sConfig);
HAL_StatusTypeDef HAL_TIMEx_HallSensor_DeInit(TIM_HandleTypeDef *htim);

void HAL_TIMEx_HallSensor_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIMEx_HallSensor_MspDeInit(TIM_HandleTypeDef *htim);

/* 阻塞模式：轮询 */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop(TIM_HandleTypeDef *htim);
/* 非阻塞模式：中断 */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start_IT(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop_IT(TIM_HandleTypeDef *htim);
/* 非阻塞模式：DMA */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start_DMA(TIM_HandleTypeDef *htim, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop_DMA(TIM_HandleTypeDef *htim);
/**
  * @}
  */

/** @addtogroup TIMEx_Exported_Functions_Group2 定时器扩展互补输出比较功能
  *  @brief   定时器互补输出比较功能函数
  * @{
  */
/*  定时器互补输出比较功能函数  *****************************/
/* 阻塞模式：轮询 */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);

/* 非阻塞模式：中断 */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);

/* 非阻塞模式：DMA */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, const uint32_t *pData,
                                          uint16_t Length);
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);
/**
  * @}
  */

/** @addtogroup TIMEx_Exported_Functions_Group3 定时器扩展互补PWM功能
  *  @brief    定时器互补PWM功能函数
  * @{
  */
/*  定时器互补PWM功能函数  ****************************************/
/* 阻塞模式：轮询 */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);

/* 非阻塞模式：中断 */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
/* 非阻塞模式：DMA */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, const uint32_t *pData,
                                           uint16_t Length);
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);
/**
  * @}
  */

/** @addtogroup TIMEx_Exported_Functions_Group4 扩展定时器互补单脉冲函数
  *  @brief    定时器互补单脉冲函数
  * @{
  */
/*  定时器互补单脉冲函数  **********************************/
/* 阻塞模式：轮询 */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Start(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Stop(TIM_HandleTypeDef *htim, uint32_t OutputChannel);

/* 非阻塞模式：中断 */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Start_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
/**
  * @}
  */

/** @addtogroup TIMEx_Exported_Functions_Group5 扩展外设控制函数
  *  @brief    外设控制函数
  * @{
  */
/* 扩展控制函数  ************************************************/
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutEvent(TIM_HandleTypeDef *htim, uint32_t  InputTrigger,
                                              uint32_t  CommutationSource);
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutEvent_IT(TIM_HandleTypeDef *htim, uint32_t  InputTrigger,
                                                 uint32_t  CommutationSource);
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutEvent_DMA(TIM_HandleTypeDef *htim, uint32_t  InputTrigger,
                                                  uint32_t  CommutationSource);
HAL_StatusTypeDef HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef *htim,
                                                        const TIM_MasterConfigTypeDef *sMasterConfig);
HAL_StatusTypeDef HAL_TIMEx_ConfigBreakDeadTime(TIM_HandleTypeDef *htim,
                                                const TIM_BreakDeadTimeConfigTypeDef *sBreakDeadTimeConfig);
HAL_StatusTypeDef HAL_TIMEx_RemapConfig(TIM_HandleTypeDef *htim, uint32_t Remap);
/**
  * @}
  */

/** @addtogroup TIMEx_Exported_Functions_Group6 扩展回调函数
  * @brief    扩展回调函数
  * @{
  */
/* 扩展回调函数 **********************************************************/
void HAL_TIMEx_CommutCallback(TIM_HandleTypeDef *htim);
void HAL_TIMEx_CommutHalfCpltCallback(TIM_HandleTypeDef *htim);
void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim);
/**
  * @}
  */

/** @addtogroup TIMEx_Exported_Functions_Group7 扩展外设状态函数
  * @brief    扩展外设状态函数
  * @{
  */
/* 扩展外设状态函数  ***********************************************************/
HAL_TIM_StateTypeDef HAL_TIMEx_HallSensor_GetState(const TIM_HandleTypeDef *htim);
HAL_TIM_ChannelStateTypeDef HAL_TIMEx_GetChannelNState(const TIM_HandleTypeDef *htim,  uint32_t ChannelN);
/**
  * @}
  */

/**
  * @}
  */
/* 导出函数结束 -------------------------------------------------------------*/

/* 私有函数 ------------------------------------------------------------------*/
/** @addtogroup TIMEx_Private_Functions TIM 扩展私有函数
  * @{
  */
void TIMEx_DMACommutationCplt(DMA_HandleTypeDef *hdma);
void TIMEx_DMACommutationHalfCplt(DMA_HandleTypeDef *hdma);
/**
  * @}
  */
/* 私有函数结束 --------------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif


#endif /* STM32F1xx_HAL_TIM_EX_H */

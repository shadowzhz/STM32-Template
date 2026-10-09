/**
  ******************************************************************************
  * @file    stm32f1xx_hal_tim.c
  * @author  MCD Application Team
  * @brief   TIM HAL 模块驱动程序。
  *          本文件提供了用于管理定时器 (TIM) 外设以下功能的固件函数：
  *           + TIM 时基初始化
  *           + 启动 TIM 时基
  *           + 启动 TIM 时基中断
  *           + 启动 TIM 时基 DMA
  *           + TIM 输出比较/PWM 初始化
  *           + TIM 输出比较/PWM 通道配置
  *           + 启动 TIM 输出比较/PWM
  *           + 启动 TIM 输出比较/PWM 中断
  *           + 启动 TIM 输出比较/PWM DMA
  *           + TIM 输入捕获初始化
  *           + TIM 输入捕获通道配置
  *           + 启动 TIM 输入捕获
  *           + 启动 TIM 输入捕获中断
  *           + 启动 TIM 输入捕获 DMA
  *           + TIM 单脉冲初始化
  *           + TIM 单脉冲通道配置
  *           + 启动 TIM 单脉冲
  *           + TIM 编码器接口初始化
  *           + 启动 TIM 编码器接口
  *           + 启动 TIM 编码器接口中断
  *           + 启动 TIM 编码器接口 DMA
  *           + 带中断和 DMA 的换相事件配置
  *           + TIM OCRef 清除配置
  *           + TIM 外部时钟配置
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
  ==============================================================================
                      ##### 定时器通用特性 #####
  ==============================================================================
  [..] 定时器特性包括：
       (#) 16 位递增、递减、中心对齐（递增/递减）自动重装载计数器。
       (#) 16 位可编程预分频器，允许将计数器时钟频率按 1 到 65536 之间的任意系数进行分频（亦支持运行中实时修改）。
       (#) 多达 4 个独立通道，用于：
           (++) 输入捕获
           (++) 输出比较
           (++) PWM 生成（边沿对齐和中心对齐模式）
           (++) 单脉冲模式输出
       (#) 同步电路，可使用外部信号控制定时器并将多个定时器互联互通。
       (#) 支持增量式编码器用于定位

            ##### 如何使用本驱动程序 #####
  ==============================================================================
    [..]
     (#) 根据所选功能，通过实现以下函数来初始化 TIM 底层资源：
           (++) 时基 : HAL_TIM_Base_MspInit()
           (++) 输入捕获 : HAL_TIM_IC_MspInit()
           (++) 输出比较 : HAL_TIM_OC_MspInit()
           (++) PWM 生成 : HAL_TIM_PWM_MspInit()
           (++) 单脉冲模式输出 : HAL_TIM_OnePulse_MspInit()
           (++) 编码器模式输出 : HAL_TIM_Encoder_MspInit()

     (#) 初始化 TIM 底层资源：
        (##) 使用 __HAL_RCC_TIMx_CLK_ENABLE() 使能 TIM 接口时钟；
        (##) TIM 引脚配置
            (+++) 使用以下函数使能 TIM GPIO 的时钟：
             __HAL_RCC_GPIOx_CLK_ENABLE();
            (+++) 使用 HAL_GPIO_Init() 将这些 TIM 引脚配置为复用功能模式；

     (#) 如有需要，可以使用以下函数配置外部时钟（默认时钟为来自 APBx 的内部时钟）：
         HAL_TIM_ConfigClockSource，时钟配置应在任何启动函数之前完成。

     (#) 使用本驱动程序的初始化函数之一将 TIM 配置为所需的工作模式：
       (++) HAL_TIM_Base_Init: 用于使用定时器生成简单时基
       (++) HAL_TIM_OC_Init 和 HAL_TIM_OC_ConfigChannel: 用于使用定时器生成输出比较信号。
       (++) HAL_TIM_PWM_Init 和 HAL_TIM_PWM_ConfigChannel: 用于使用定时器生成 PWM 信号。
       (++) HAL_TIM_IC_Init 和 HAL_TIM_IC_ConfigChannel: 用于使用定时器测量外部信号。
       (++) HAL_TIM_OnePulse_Init 和 HAL_TIM_OnePulse_ConfigChannel: 用于在单脉冲模式下使用定时器。
       (++) HAL_TIM_Encoder_Init: 用于使用定时器编码器接口。

     (#) 根据所使用的功能，调用相应的启动函数之一来使能 TIM 外设：
           (++) 时基 : HAL_TIM_Base_Start(), HAL_TIM_Base_Start_DMA(), HAL_TIM_Base_Start_IT()
           (++) 输入捕获 :  HAL_TIM_IC_Start(), HAL_TIM_IC_Start_DMA(), HAL_TIM_IC_Start_IT()
           (++) 输出比较 : HAL_TIM_OC_Start(), HAL_TIM_OC_Start_DMA(), HAL_TIM_OC_Start_IT()
           (++) PWM 生成 : HAL_TIM_PWM_Start(), HAL_TIM_PWM_Start_DMA(), HAL_TIM_PWM_Start_IT()
           (++) 单脉冲模式输出 : HAL_TIM_OnePulse_Start(), HAL_TIM_OnePulse_Start_IT()
           (++) 编码器模式输出 : HAL_TIM_Encoder_Start(), HAL_TIM_Encoder_Start_DMA(), HAL_TIM_Encoder_Start_IT()。

     (#) DMA 突发模式通过以下两个函数进行管理：
         HAL_TIM_DMABurst_WriteStart()
         HAL_TIM_DMABurst_ReadStart()

    *** 回调函数注册 ***
  =============================================

  [..]
  当编译宏 USE_HAL_TIM_REGISTER_CALLBACKS 设置为 1 时，
  允许用户动态配置驱动程序回调函数。

  [..]
  使用函数 HAL_TIM_RegisterCallback() 注册回调函数。
  HAL_TIM_RegisterCallback() 的入参包括 HAL 外设句柄、
  回调函数 ID 以及指向用户回调函数的指针。

  [..]
  使用函数 HAL_TIM_UnRegisterCallback() 将回调函数重置为默认的 weak 函数。
  HAL_TIM_UnRegisterCallback 的入参包括 HAL 外设句柄
  和回调函数 ID。

  [..]
  这些函数允许注册/注销以下回调函数：
    (+) Base_MspInitCallback              : TIM 时基 Msp 初始化回调函数。
    (+) Base_MspDeInitCallback            : TIM 时基 Msp 反初始化回调函数。
    (+) IC_MspInitCallback                : TIM 输入捕获 Msp 初始化回调函数。
    (+) IC_MspDeInitCallback              : TIM 输入捕获 Msp 反初始化回调函数。
    (+) OC_MspInitCallback                : TIM 输出比较 Msp 初始化回调函数。
    (+) OC_MspDeInitCallback              : TIM 输出比较 Msp 反初始化回调函数。
    (+) PWM_MspInitCallback               : TIM PWM Msp 初始化回调函数。
    (+) PWM_MspDeInitCallback             : TIM PWM Msp 反初始化回调函数。
    (+) OnePulse_MspInitCallback          : TIM 单脉冲 Msp 初始化回调函数。
    (+) OnePulse_MspDeInitCallback        : TIM 单脉冲 Msp 反初始化回调函数。
    (+) Encoder_MspInitCallback           : TIM 编码器 Msp 初始化回调函数。
    (+) Encoder_MspDeInitCallback         : TIM 编码器 Msp 反初始化回调函数。
    (+) HallSensor_MspInitCallback        : TIM 霍尔传感器 Msp 初始化回调函数。
    (+) HallSensor_MspDeInitCallback      : TIM 霍尔传感器 Msp 反初始化回调函数。
    (+) PeriodElapsedCallback             : TIM 周期溢出回调函数。
    (+) PeriodElapsedHalfCpltCallback     : TIM 周期溢出过半完成回调函数。
    (+) TriggerCallback                   : TIM 触发回调函数。
    (+) TriggerHalfCpltCallback           : TIM 触发半传输完成回调函数。
    (+) IC_CaptureCallback                : TIM 输入捕获回调函数。
    (+) IC_CaptureHalfCpltCallback        : TIM 输入捕获半传输完成回调函数。
    (+) OC_DelayElapsedCallback           : TIM 输出比较延迟到达回调函数。
    (+) PWM_PulseFinishedCallback         : TIM PWM 脉冲完成回调函数。
    (+) PWM_PulseFinishedHalfCpltCallback : TIM PWM 脉冲半完成回调函数。
    (+) ErrorCallback                     : TIM 错误回调函数。
    (+) CommutationCallback               : TIM 换相回调函数。
    (+) CommutationHalfCpltCallback       : TIM 换相半完成回调函数。
    (+) BreakCallback                     : TIM 刹车回调函数。

  [..]
默认情况下，在 Init 之后且状态为 HAL_TIM_STATE_RESET 时，
所有中断回调函数均被设置为对应的 weak 函数：
  例如 HAL_TIM_TriggerCallback()、HAL_TIM_ErrorCallback()。

  [..]
  MspInit 和 MspDeInit 函数例外，仅当这些回调函数为空（事先未注册）时，
  才会在 Init / DeInit 中被重置为传统的 weak 实现。
  若 MspInit 或 MspDeInit 不为空，则 Init / DeInit 将保留并使用用户（事先注册的）MspInit / MspDeInit 回调函数。

  [..]
    回调函数仅能在 HAL_TIM_STATE_READY 状态下进行注册/注销。
    MspInit / MspDeInit 例外，它们可以在 HAL_TIM_STATE_READY 或 HAL_TIM_STATE_RESET 状态下进行注册/注销，
    因此已注册的（用户）MspInit / DeInit 回调函数可在 Init / DeInit 期间使用。
    在此情况下，应在调用 DeInit 或 Init 函数之前，
    首先使用 HAL_TIM_RegisterCallback() 注册 MspInit/MspDeInit 用户回调函数。

  [..]
      当编译宏 USE_HAL_TIM_REGISTER_CALLBACKS 设置为 0 或未定义时，
      回调函数注册功能不可用，所有回调函数均被设置为对应的 weak 函数。

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup TIM TIM
  * @brief TIM HAL 模块驱动程序
  * @{
  */

#ifdef HAL_TIM_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup TIM_Private_Functions
  * @{
  */
static void TIM_OC1_SetConfig(TIM_TypeDef *TIMx, const TIM_OC_InitTypeDef *OC_Config);
static void TIM_OC3_SetConfig(TIM_TypeDef *TIMx, const TIM_OC_InitTypeDef *OC_Config);
static void TIM_OC4_SetConfig(TIM_TypeDef *TIMx, const TIM_OC_InitTypeDef *OC_Config);
static void TIM_TI1_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter);
static void TIM_TI2_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                              uint32_t TIM_ICFilter);
static void TIM_TI2_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter);
static void TIM_TI3_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                              uint32_t TIM_ICFilter);
static void TIM_TI4_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                              uint32_t TIM_ICFilter);
static void TIM_ITRx_SetConfig(TIM_TypeDef *TIMx, uint32_t InputTriggerSource);
static void TIM_DMAPeriodElapsedCplt(DMA_HandleTypeDef *hdma);
static void TIM_DMAPeriodElapsedHalfCplt(DMA_HandleTypeDef *hdma);
static void TIM_DMADelayPulseCplt(DMA_HandleTypeDef *hdma);
static void TIM_DMATriggerCplt(DMA_HandleTypeDef *hdma);
static void TIM_DMATriggerHalfCplt(DMA_HandleTypeDef *hdma);
static HAL_StatusTypeDef TIM_SlaveTimer_SetConfig(TIM_HandleTypeDef *htim,
                                                  const TIM_SlaveConfigTypeDef *sSlaveConfig);
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/

/** @defgroup TIM_Exported_Functions TIM Exported Functions
  * @{
  */

/** @defgroup TIM_Exported_Functions_Group1 TIM Time Base functions
  *  @brief    时基函数
  *
@verbatim
  ==============================================================================
              ##### 时基函数 #####
  ==============================================================================
  [..]
    本节提供的函数可实现：
    (+) 初始化并配置 TIM 时基单元。
    (+) 反初始化 TIM 时基单元。
    (+) 启动时基。
    (+) 停止时基。
    (+) 启动时基并使能中断。
    (+) 停止时基并禁用中断。
    (+) 启动时基并使能 DMA 传输。
    (+) 停止时基并禁用 DMA 传输。

@endverbatim
  * @{
  */
/**
  * @brief  根据 TIM_HandleTypeDef 中指定的参数初始化 TIM 时基单元，
  *         并初始化关联的句柄。
  * @note   从中心对齐计数模式切换到边沿计数模式（或反向切换）时，
  *         需要复位定时器，以避免因 DIR 位在中心对齐模式下只读而导致计数方向异常。
  *         例如：在调用 HAL_TIM_Base_Init() 之前先调用 @ref HAL_TIM_Base_DeInit()
  * @param  htim TIM 时基句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *htim)
{
  /* 检查 TIM 句柄是否有效分配 */
  if (htim == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数有效性 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));

  if (htim->State == HAL_TIM_STATE_RESET)
  {
    /* 分配锁资源并进行初始化 */
    htim->Lock = HAL_UNLOCKED;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
    /* 将中断回调函数重置为传统的弱定义回调函数 */
    TIM_ResetCallback(htim);

    if (htim->Base_MspInitCallback == NULL)
    {
      htim->Base_MspInitCallback = HAL_TIM_Base_MspInit;
    }
    /* 初始化底层硬件：GPIO、时钟、NVIC */
    htim->Base_MspInitCallback(htim);
#else
    /* 初始化底层硬件：GPIO、时钟、NVIC */
    HAL_TIM_Base_MspInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }

  /* 设置 TIM 状态 */
  htim->State = HAL_TIM_STATE_BUSY;

  /* 设置时基单元配置 */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* 初始化 DMA 突发传输操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;

  /* 初始化 TIM 各通道状态 */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);

  /* 初始化 TIM 状态 */
  htim->State = HAL_TIM_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  反初始化 TIM Base 外设
  * @param  htim TIM Base 句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Base_DeInit(TIM_HandleTypeDef *htim)
{
  /* 检查参数有效性 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* 禁用 TIM 外设时钟 */
  __HAL_TIM_DISABLE(htim);

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  if (htim->Base_MspDeInitCallback == NULL)
  {
    htim->Base_MspDeInitCallback = HAL_TIM_Base_MspDeInit;
  }
  /* 反初始化底层硬件 */
  htim->Base_MspDeInitCallback(htim);
#else
  /* 反初始化底层硬件：GPIO、CLOCK、NVIC */
  HAL_TIM_Base_MspDeInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* 改变 DMA 突发传输操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;

  /* 改变 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);

  /* 改变 TIM 状态 */
  htim->State = HAL_TIM_STATE_RESET;

  /* 释放锁 */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  初始化 TIM Base MSP。
  * @param  htim TIM Base 句柄
  * @retval 无
  */
__weak void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要回调函数时，
            可以在用户文件中实现 HAL_TIM_Base_MspInit
   */
}

/**
  * @brief  反初始化 TIM Base MSP。
  * @param  htim TIM Base 句柄
  * @retval 无
  */
__weak void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要回调函数时，
            可以在用户文件中实现 HAL_TIM_Base_MspDeInit
   */
}


/**
  * @brief  启动 TIM Base 定时生成。
  * @param  htim TIM Base 句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim)
{
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  /* 检查 TIM 状态 */
  if (htim->State != HAL_TIM_STATE_READY)
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 状态 */
  htim->State = HAL_TIM_STATE_BUSY;

  /* 使能外设，但在触发模式下除外（触发模式下会自动使能） */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
  {
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
    {
      __HAL_TIM_ENABLE(htim);
    }
  }
  else
  {
    __HAL_TIM_ENABLE(htim);
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  停止 TIM 基本定时产生。
  * @param  htim TIM 基本定时句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim)
{
  /* 检查参数 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 状态 */
  htim->State = HAL_TIM_STATE_READY;

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  以中断模式启动 TIM 基本定时产生。
  * @param  htim TIM 基本定时句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim)
{
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  /* 检查 TIM 状态 */
  if (htim->State != HAL_TIM_STATE_READY)
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 状态 */
  htim->State = HAL_TIM_STATE_BUSY;

  /* 使能 TIM 更新中断 */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);

  /* 使能外设，但在触发模式下除外（触发模式下会自动使能） */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
  {
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
    {
      __HAL_TIM_ENABLE(htim);
    }
  }
  else
  {
    __HAL_TIM_ENABLE(htim);
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  停止中断模式下的 TIM 基本定时计数。
  * @param  htim TIM 基本定时器句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim)
{
  /* 检查参数 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  /* 禁用 TIM 更新中断 */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_UPDATE);

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 状态 */
  htim->State = HAL_TIM_STATE_READY;

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  启动 DMA 模式下的 TIM 基本定时计数。
  * @param  htim TIM 基本定时器句柄
  * @param  pData 源数据缓冲区地址。
  * @param  Length 从内存传输到外设的数据长度。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Base_Start_DMA(TIM_HandleTypeDef *htim, const uint32_t *pData, uint16_t Length)
{
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_DMA_INSTANCE(htim->Instance));

  /* 设置 TIM 状态 */
  if (htim->State == HAL_TIM_STATE_BUSY)
  {
    return HAL_BUSY;
  }
  else if (htim->State == HAL_TIM_STATE_READY)
  {
    if ((pData == NULL) || (Length == 0U))
    {
      return HAL_ERROR;
    }
    else
    {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }
  else
  {
    return HAL_ERROR;
  }

  /* 设置 DMA 周期完成回调函数 */
  htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = TIM_DMAPeriodElapsedCplt;
  htim->hdma[TIM_DMA_ID_UPDATE]->XferHalfCpltCallback = TIM_DMAPeriodElapsedHalfCplt;

  /* 设置 DMA 错误回调函数 */
  htim->hdma[TIM_DMA_ID_UPDATE]->XferErrorCallback = TIM_DMAError ;

  /* 使能 DMA 通道 */
  if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)pData, (uint32_t)&htim->Instance->ARR,
                       Length) != HAL_OK)
  {
    /* 返回错误状态 */
    return HAL_ERROR;
  }

  /* 使能 TIM 更新 DMA 请求 */
  __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_UPDATE);

  /* 使能外设，触发模式除外（该模式下由触发信号自动使能） */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
  {
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
    {
      __HAL_TIM_ENABLE(htim);
    }
  }
  else
  {
    __HAL_TIM_ENABLE(htim);
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  停止 DMA 模式下的 TIM 基本定时计数。
  * @param  htim TIM 基本定时句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Base_Stop_DMA(TIM_HandleTypeDef *htim)
{
  /* 检查参数 */
  assert_param(IS_TIM_DMA_INSTANCE(htim->Instance));

  /* 禁用 TIM 更新 DMA 请求 */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_UPDATE);

  (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_UPDATE]);

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 状态 */
  htim->State = HAL_TIM_STATE_READY;

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group2 TIM 输出比较函数
  *  @brief    TIM 输出比较函数
  *
@verbatim
  ==============================================================================
                  ##### TIM 输出比较函数 #####
  ==============================================================================
  [..]
    本节提供的函数可实现：
    (+) 初始化并配置 TIM 输出比较。
    (+) 反初始化 TIM 输出比较。
    (+) 启动 TIM 输出比较。
    (+) 停止 TIM 输出比较。
    (+) 启动 TIM 输出比较并使能中断。
    (+) 停止 TIM 输出比较并禁用中断。
    (+) 启动 TIM 输出比较并使能 DMA 传输。
    (+) 停止 TIM 输出比较并禁用 DMA 传输。

@endverbatim
  * @{
  */
/**
  * @brief  根据 TIM_HandleTypeDef 中指定的参数初始化 TIM 输出比较，
  *         并初始化关联的句柄。
  * @note   从中央对齐计数模式切换到边沿计数模式（或反向切换）
  *         需要复位定时器，以避免因中央对齐模式下 DIR 位为只读
  *         而导致的非预期计数方向异常。
  *         例如：在调用 HAL_TIM_OC_Init() 之前先调用 @ref HAL_TIM_OC_DeInit()
  * @param  htim TIM 输出比较句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OC_Init(TIM_HandleTypeDef *htim)
{
  /* 检查 TIM 句柄是否已分配 */
  if (htim == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));

  if (htim->State == HAL_TIM_STATE_RESET)
  {
    /* 分配锁资源并初始化 */
    htim->Lock = HAL_UNLOCKED;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
    /* 将中断回调函数重置为传统的弱定义回调函数 */
    TIM_ResetCallback(htim);

    if (htim->OC_MspInitCallback == NULL)
    {
      htim->OC_MspInitCallback = HAL_TIM_OC_MspInit;
    }
    /* 初始化底层硬件：GPIO、时钟、NVIC */
    htim->OC_MspInitCallback(htim);
#else
    /* 初始化底层硬件：GPIO、时钟、NVIC 和 DMA */
    HAL_TIM_OC_MspInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }

  /* 设置 TIM 状态 */
  htim->State = HAL_TIM_STATE_BUSY;

  /* 初始化输出比较的时基单元 */
  TIM_Base_SetConfig(htim->Instance,  &htim->Init);

  /* 初始化 DMA 突发传输操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;

  /* 初始化 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);

  /* 初始化 TIM 状态 */
  htim->State = HAL_TIM_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  反初始化 TIM 外设
  * @param  htim TIM 输出比较句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OC_DeInit(TIM_HandleTypeDef *htim)
{
  /* 检查参数 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* 禁用 TIM 外设时钟 */
  __HAL_TIM_DISABLE(htim);

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  if (htim->OC_MspDeInitCallback == NULL)
  {
    htim->OC_MspDeInitCallback = HAL_TIM_OC_MspDeInit;
  }
  /* 反初始化底层硬件 */
  htim->OC_MspDeInitCallback(htim);
#else
  /* 反初始化底层硬件：GPIO、时钟、NVIC 和 DMA */
  HAL_TIM_OC_MspDeInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* 改变 DMA 突发传输操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;

  /* 改变 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);

  /* 改变 TIM 状态 */
  htim->State = HAL_TIM_STATE_RESET;

  /* 释放锁 */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  初始化 TIM 输出比较 MSP。
  * @param  htim TIM 输出比较句柄
  * @retval 无
  */
__weak void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要回调时，
            可以在用户文件中实现 HAL_TIM_OC_MspInit
   */
}

/**
  * @brief  反初始化 TIM 输出比较 MSP。
  * @param  htim TIM 输出比较句柄
  * @retval 无
  */
__weak void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要回调时，
            可以在用户文件中实现 HAL_TIM_OC_MspDeInit
   */
}

/**
  * @brief  启动 TIM 输出比较信号生成。
  * @param  htim TIM 输出比较句柄
  * @param  Channel 要使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OC_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* 检查 TIM 通道状态 */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);

  /* 使能输出比较通道 */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
  {
    /* 使能主输出 */
    __HAL_TIM_MOE_ENABLE(htim);
  }

  /* 使能外设，但在触发模式下除外，该模式下使能会随触发自动完成 */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
  {
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
    {
      __HAL_TIM_ENABLE(htim);
    }
  }
  else
  {
    __HAL_TIM_ENABLE(htim);
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  停止 TIM 输出比较信号生成。
  * @param  htim TIM 输出比较句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* 禁用输出比较通道 */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
  {
    /* 禁用主输出 */
    __HAL_TIM_MOE_DISABLE(htim);
  }

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  以中断模式启动 TIM 输出比较信号生成。
  * @param  htim TIM 输出比较句柄
  * @param  Channel 要使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OC_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* 检查 TIM 通道状态 */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 使能 TIM 捕获/比较 1 中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 使能 TIM 捕获/比较 2 中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 使能 TIM 捕获/比较 3 中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 使能 TIM 捕获/比较 4 中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC4);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 使能输出比较通道 */
    TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

    if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
    {
      /* 使能主输出 */
      __HAL_TIM_MOE_ENABLE(htim);
    }

    /* 使能外设，但在触发模式下除外（触发模式下由触发信号自动使能） */
    if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
    {
      tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
      if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
      {
        __HAL_TIM_ENABLE(htim);
      }
    }
    else
    {
      __HAL_TIM_ENABLE(htim);
    }
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  在中断模式下停止 TIM 输出比较信号生成。
  * @param  htim TIM 输出比较句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OC_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 禁用 TIM 捕获/比较 1 中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 禁用 TIM 捕获/比较 2 中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 禁用 TIM 捕获/比较 3 中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 禁用 TIM 捕获/比较 4 中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC4);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 禁用输出比较通道 */
    TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

    if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
    {
      /* 禁用主输出 */
      __HAL_TIM_MOE_DISABLE(htim);
    }

    /* 禁用外设 */
    __HAL_TIM_DISABLE(htim);

    /* 设置 TIM 通道状态 */
    TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  在 DMA 模式下启动 TIM 输出比较信号生成。
  * @param  htim TIM 输出比较句柄
  * @param  Channel 要使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @param  pData 源缓冲区地址。
  * @param  Length 从内存传输到 TIM 外设的数据长度
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, const uint32_t *pData,
                                       uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* 设置 TIM 通道状态 */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_BUSY)
  {
    return HAL_BUSY;
  }
  else if (TIM_CHANNEL_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_READY)
  {
    if ((pData == NULL) || (Length == 0U))
    {
      return HAL_ERROR;
    }
    else
    {
      TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);
    }
  }
  else
  {
    return HAL_ERROR;
  }

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseCplt;
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData, (uint32_t)&htim->Instance->CCR1,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }

      /* 使能 TIM 捕获/比较 1 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseCplt;
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }

      /* 使能 TIM 捕获/比较 2 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseCplt;
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)pData, (uint32_t)&htim->Instance->CCR3,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 捕获/比较 3 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMADelayPulseCplt;
      htim->hdma[TIM_DMA_ID_CC4]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)pData, (uint32_t)&htim->Instance->CCR4,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 捕获/比较 4 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC4);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 使能输出比较通道 */
    TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

    if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
    {
      /* 使能主输出 */
      __HAL_TIM_MOE_ENABLE(htim);
    }

    /* 使能外设，但在触发模式下除外（触发模式下由触发信号自动使能） */
    if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
    {
      tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
      if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
      {
        __HAL_TIM_ENABLE(htim);
      }
    }
    else
    {
      __HAL_TIM_ENABLE(htim);
    }
  }

  /* 返回函数执行状态 */
  return status;
}

/**
  * @brief  停止以 DMA 模式生成 TIM 输出比较信号。
  * @param  htim TIM 输出比较句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 禁用 TIM 捕获/比较 1 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 禁用 TIM 捕获/比较 2 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 禁用 TIM 捕获/比较 3 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC3]);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 禁用 TIM 捕获/比较 4 中断 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC4);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC4]);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 禁用输出比较通道 */
    TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

    if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
    {
      /* 禁用主输出 */
      __HAL_TIM_MOE_DISABLE(htim);
    }

    /* 禁用外设 */
    __HAL_TIM_DISABLE(htim);

    /* 设置 TIM 通道状态 */
    TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group3 TIM PWM functions
  *  @brief    TIM PWM 功能函数
  *
@verbatim
  ==============================================================================
                          ##### TIM PWM 功能函数 #####
  ==============================================================================
  [..]
    本节提供的函数允许实现：
    (+) 初始化并配置 TIM PWM。
    (+) 反初始化 TIM PWM。
    (+) 启动 TIM PWM。
    (+) 停止 TIM PWM。
    (+) 启动 TIM PWM 并使能中断。
    (+) 停止 TIM PWM 并禁用中断。
    (+) 启动 TIM PWM 并使能 DMA 传输。
    (+) 停止 TIM PWM 并禁用 DMA 传输。

@endverbatim
  * @{
  */
/**
  * @brief  根据 TIM_HandleTypeDef 中指定的参数初始化 TIM PWM 时基单元，
  *         并初始化相关句柄。
  * @note   从中央对齐计数模式切换到边沿计数模式（或反之）
  *         需要复位定时器，以避免因中央对齐模式下 DIR 位为只读
  *         而导致的异常计数方向。
  *         例如：在调用 HAL_TIM_PWM_Init() 之前先调用 @ref HAL_TIM_PWM_DeInit()
  * @param  htim TIM PWM 句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim)
{
  /* 检查 TIM 句柄分配有效性 */
  if (htim == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));

  if (htim->State == HAL_TIM_STATE_RESET)
  {
    /* 分配锁资源并进行初始化 */
    htim->Lock = HAL_UNLOCKED;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
    /* 将中断回调函数重置为传统的弱定义回调函数 */
    TIM_ResetCallback(htim);

    if (htim->PWM_MspInitCallback == NULL)
    {
      htim->PWM_MspInitCallback = HAL_TIM_PWM_MspInit;
    }
    /* 初始化底层硬件：GPIO、时钟、NVIC */
    htim->PWM_MspInitCallback(htim);
#else
    /* 初始化底层硬件：GPIO、时钟、NVIC 和 DMA */
    HAL_TIM_PWM_MspInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }

  /* 设置 TIM 状态 */
  htim->State = HAL_TIM_STATE_BUSY;

  /* 初始化 PWM 时基 */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* 初始化 DMA 突发传输操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;

  /* 初始化 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);

  /* 初始化 TIM 状态 */
  htim->State = HAL_TIM_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  反初始化 TIM 外设
  * @param  htim TIM PWM 句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_PWM_DeInit(TIM_HandleTypeDef *htim)
{
  /* 检查参数有效性 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* 禁用 TIM 外设时钟 */
  __HAL_TIM_DISABLE(htim);

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  if (htim->PWM_MspDeInitCallback == NULL)
  {
    htim->PWM_MspDeInitCallback = HAL_TIM_PWM_MspDeInit;
  }
  /* 反初始化底层硬件 */
  htim->PWM_MspDeInitCallback(htim);
#else
  /* 反初始化底层硬件：GPIO、时钟、NVIC 和 DMA */
  HAL_TIM_PWM_MspDeInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* 改变 DMA 突发传输操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;

  /* 改变 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);

  /* 改变 TIM 状态 */
  htim->State = HAL_TIM_STATE_RESET;

  /* 释放锁 */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  初始化 TIM PWM MSP。
  * @param  htim TIM PWM 句柄
  * @retval 无
  */
__weak void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要回调函数时，
            可以在用户文件中实现 HAL_TIM_PWM_MspInit
   */
}

/**
  * @brief  反初始化 TIM PWM MSP。
  * @param  htim TIM PWM 句柄
  * @retval 无
  */
__weak void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要回调函数时，
            可以在用户文件中实现 HAL_TIM_PWM_MspDeInit
   */
}

/**
  * @brief  启动 PWM 信号生成。
  * @param  htim TIM 句柄
  * @param  Channel 待使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* 检查 TIM 通道状态 */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);

  /* 使能捕获/比较通道 */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
  {
    /* 使能主输出 */
    __HAL_TIM_MOE_ENABLE(htim);
  }

  /* 使能外设，但在触发模式下除外，该模式下外设会随触发信号自动使能 */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
  {
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
    {
      __HAL_TIM_ENABLE(htim);
    }
  }
  else
  {
    __HAL_TIM_ENABLE(htim);
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  停止 PWM 信号生成。
  * @param  htim TIM PWM 句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选中 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选中 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选中 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选中 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* 检查参数有效性 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* 禁用捕获/比较通道 */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
  {
    /* 禁用主输出 */
    __HAL_TIM_MOE_DISABLE(htim);
  }

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  以中断模式启动 PWM 信号生成。
  * @param  htim TIM PWM 句柄
  * @param  Channel 要使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选中 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选中 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选中 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选中 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_PWM_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;

  /* 检查参数有效性 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* 检查 TIM 通道状态 */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 使能 TIM 捕获/比较 1 中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 使能 TIM 捕获/比较 2 中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 使能 TIM 捕获/比较 3 中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 使能 TIM 捕获/比较 4 中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC4);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 使能捕获/比较通道 */
    TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

    if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
    {
      /* 使能主输出 */
      __HAL_TIM_MOE_ENABLE(htim);
    }

    /* 使能外设，但在触发模式下除外，该模式下会通过触发信号自动使能 */
    if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
    {
      tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
      if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
      {
        __HAL_TIM_ENABLE(htim);
      }
    }
    else
    {
      __HAL_TIM_ENABLE(htim);
    }
  }

  /* 返回函数执行状态 */
  return status;
}

/**
  * @brief  在中断模式下停止 PWM 信号生成。
  * @param  htim TIM PWM 句柄
  * @param  Channel 待禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_PWM_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数有效性 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 禁用 TIM 捕获/比较 1 中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 禁用 TIM 捕获/比较 2 中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 禁用 TIM 捕获/比较 3 中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 禁用 TIM 捕获/比较 4 中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC4);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 禁用捕获/比较通道 */
    TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

    if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
    {
      /* 禁用主输出 */
      __HAL_TIM_MOE_DISABLE(htim);
    }

    /* 禁用外设 */
    __HAL_TIM_DISABLE(htim);

    /* 设置 TIM 通道状态 */
    TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  以 DMA 模式启动 TIM PWM 信号生成。
  * @param  htim TIM PWM 句柄
  * @param  Channel 待使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @param  pData 源缓冲区地址。
  * @param  Length 从内存传输到 TIM 外设的数据长度
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_PWM_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, const uint32_t *pData,
                                        uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* 设置 TIM 通道状态 */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_BUSY)
  {
    return HAL_BUSY;
  }
  else if (TIM_CHANNEL_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_READY)
  {
    if ((pData == NULL) || (Length == 0U))
    {
      return HAL_ERROR;
    }
    else
    {
      TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);
    }
  }
  else
  {
    return HAL_ERROR;
  }

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseCplt;
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData, (uint32_t)&htim->Instance->CCR1,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }

      /* 使能 TIM 捕获/比较 1 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseCplt;
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 捕获/比较 2 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseCplt;
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)pData, (uint32_t)&htim->Instance->CCR3,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 输出捕获/比较 3 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMADelayPulseCplt;
      htim->hdma[TIM_DMA_ID_CC4]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)pData, (uint32_t)&htim->Instance->CCR4,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 捕获/比较 4 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC4);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 使能捕获比较通道 */
    TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

    if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
    {
      /* 使能主输出 */
      __HAL_TIM_MOE_ENABLE(htim);
    }

    /* 使能外设，但在触发模式下除外，此时将通过触发自动完成使能 */
    if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
    {
      tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
      if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
      {
        __HAL_TIM_ENABLE(htim);
      }
    }
    else
    {
      __HAL_TIM_ENABLE(htim);
    }
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  停止 DMA 模式下的 TIM PWM 信号生成。
  * @param  htim TIM PWM 句柄
  * @param  Channel 待禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_PWM_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 禁用 TIM 捕获/比较 1 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 禁用 TIM 捕获/比较 2 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 禁用 TIM 捕获/比较 3 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC3]);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 禁用 TIM 捕获/比较 4 中断 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC4);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC4]);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 禁用捕获/比较通道 */
    TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

    if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
    {
      /* 禁用主输出 */
      __HAL_TIM_MOE_DISABLE(htim);
    }

    /* 禁用外设 */
    __HAL_TIM_DISABLE(htim);

    /* 设置 TIM 通道状态 */
    TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group4 TIM 输入捕获函数
  *  @brief    TIM 输入捕获函数
  *
@verbatim
  ==============================================================================
              ##### TIM 输入捕获函数 #####
  ==============================================================================
 [..]
   本节提供的函数允许实现：
   (+) 初始化并配置 TIM 输入捕获。
   (+) 反初始化 TIM 输入捕获。
   (+) 启动 TIM 输入捕获。
   (+) 停止 TIM 输入捕获。
   (+) 启动 TIM 输入捕获并使能中断。
   (+) 停止 TIM 输入捕获并禁用中断。
   (+) 启动 TIM 输入捕获并使能 DMA 传输。
   (+) 停止 TIM 输入捕获并禁用 DMA 传输。

@endverbatim
  * @{
  */
/**
  * @brief  根据 TIM_HandleTypeDef 中指定的参数初始化 TIM 输入捕获时基，
  *         并初始化相关联的句柄。
  * @note   从中央对齐计数模式切换到边沿计数模式（或反向切换）
  *         需要复位定时器，以避免因中央对齐模式下 DIR 位只读
  *         而导致的异常计数方向。
  *         例如：在调用 @ref HAL_TIM_IC_Init() 之前先调用 @ref HAL_TIM_IC_DeInit()
  * @param  htim TIM 输入捕获句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_IC_Init(TIM_HandleTypeDef *htim)
{
  /* 检查 TIM 句柄分配 */
  if (htim == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));

  if (htim->State == HAL_TIM_STATE_RESET)
  {
    /* 分配锁资源并进行初始化 */
    htim->Lock = HAL_UNLOCKED;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
    /* 将中断回调函数重置为传统的弱定义回调函数 */
    TIM_ResetCallback(htim);

    if (htim->IC_MspInitCallback == NULL)
    {
      htim->IC_MspInitCallback = HAL_TIM_IC_MspInit;
    }
    /* 初始化底层硬件：GPIO、时钟、NVIC */
    htim->IC_MspInitCallback(htim);
#else
    /* 初始化底层硬件：GPIO、时钟、NVIC 和 DMA */
    HAL_TIM_IC_MspInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }

  /* 设置 TIM 状态 */
  htim->State = HAL_TIM_STATE_BUSY;

  /* 初始化输入捕获的时基单元 */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* 初始化 DMA 突发传输操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;

  /* 初始化 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_READY);

  /* 初始化 TIM 状态 */
  htim->State = HAL_TIM_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  反初始化 TIM 外设
  * @param  htim TIM 输入捕获句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_IC_DeInit(TIM_HandleTypeDef *htim)
{
  /* 检查参数 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* 禁用 TIM 外设时钟 */
  __HAL_TIM_DISABLE(htim);

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  if (htim->IC_MspDeInitCallback == NULL)
  {
    htim->IC_MspDeInitCallback = HAL_TIM_IC_MspDeInit;
  }
  /* 反初始化底层硬件 */
  htim->IC_MspDeInitCallback(htim);
#else
  /* 反初始化底层硬件：GPIO、时钟、NVIC 和 DMA */
  HAL_TIM_IC_MspDeInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* 改变 DMA 突发操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;

  /* 改变 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);
  TIM_CHANNEL_N_STATE_SET_ALL(htim, HAL_TIM_CHANNEL_STATE_RESET);

  /* 改变 TIM 状态 */
  htim->State = HAL_TIM_STATE_RESET;

  /* 释放锁 */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  初始化 TIM 输入捕获 MSP。
  * @param  htim TIM 输入捕获句柄
  * @retval 无
  */
__weak void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要回调函数时，
            可以在用户文件中实现 HAL_TIM_IC_MspInit
   */
}

/**
  * @brief  反初始化 TIM 输入捕获 MSP。
  * @param  htim TIM 句柄
  * @retval 无
  */
__weak void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要回调函数时，
            可以在用户文件中实现 HAL_TIM_IC_MspDeInit
   */
}

/**
  * @brief  启动 TIM 输入捕获测量。
  * @param  htim TIM 输入捕获句柄
  * @param  Channel 待使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_IC_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  uint32_t tmpsmcr;
  HAL_TIM_ChannelStateTypeDef channel_state = TIM_CHANNEL_STATE_GET(htim, Channel);
  HAL_TIM_ChannelStateTypeDef complementary_channel_state = TIM_CHANNEL_N_STATE_GET(htim, Channel);

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* 检查 TIM 通道状态 */
  if ((channel_state != HAL_TIM_CHANNEL_STATE_READY)
      || (complementary_channel_state != HAL_TIM_CHANNEL_STATE_READY))
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);

  /* 使能输入捕获通道 */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  /* 使能外设，但在触发模式下除外，该模式下会通过触发自动使能 */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
  {
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
    {
      __HAL_TIM_ENABLE(htim);
    }
  }
  else
  {
    __HAL_TIM_ENABLE(htim);
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  停止 TIM 输入捕获测量。
  * @param  htim TIM 输入捕获句柄
  * @param  Channel 待禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_IC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* 禁用输入捕获通道 */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  在中断模式下启动 TIM 输入捕获测量。
  * @param  htim TIM 输入捕获句柄
  * @param  Channel 待使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_IC_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;

  HAL_TIM_ChannelStateTypeDef channel_state = TIM_CHANNEL_STATE_GET(htim, Channel);
  HAL_TIM_ChannelStateTypeDef complementary_channel_state = TIM_CHANNEL_N_STATE_GET(htim, Channel);

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* 检查 TIM 通道状态 */
  if ((channel_state != HAL_TIM_CHANNEL_STATE_READY)
      || (complementary_channel_state != HAL_TIM_CHANNEL_STATE_READY))
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 使能 TIM 捕获/比较 1 中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 使能 TIM 捕获/比较 2 中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 使能 TIM 捕获/比较 3 中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 使能 TIM 捕获/比较 4 中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC4);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 使能输入捕获通道 */
    TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

    /* 使能外设，触发模式除外（该模式下由触发信号自动使能） */
    if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
    {
      tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
      if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
      {
        __HAL_TIM_ENABLE(htim);
      }
    }
    else
    {
      __HAL_TIM_ENABLE(htim);
    }
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  以中断模式停止 TIM 输入捕获测量。
  * @param  htim TIM 输入捕获句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_IC_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 禁用 TIM 捕获/比较 1 中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 禁用 TIM 捕获/比较 2 中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 禁用 TIM 捕获/比较 3 中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 禁用 TIM 捕获/比较 4 中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC4);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 禁用输入捕获通道 */
    TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

    /* 禁用外设 */
    __HAL_TIM_DISABLE(htim);

    /* 设置 TIM 通道状态 */
    TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  以 DMA 模式启动 TIM 输入捕获测量。
  * @param  htim TIM 输入捕获句柄
  * @param  Channel 要启用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @param  pData 目标缓冲区地址。
  * @param  Length 从 TIM 外设传输到内存的数据长度。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_IC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;

  HAL_TIM_ChannelStateTypeDef channel_state = TIM_CHANNEL_STATE_GET(htim, Channel);
  HAL_TIM_ChannelStateTypeDef complementary_channel_state = TIM_CHANNEL_N_STATE_GET(htim, Channel);

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));
  assert_param(IS_TIM_DMA_CC_INSTANCE(htim->Instance));

  /* 设置 TIM 通道状态 */
  if ((channel_state == HAL_TIM_CHANNEL_STATE_BUSY)
      || (complementary_channel_state == HAL_TIM_CHANNEL_STATE_BUSY))
  {
    return HAL_BUSY;
  }
  else if ((channel_state == HAL_TIM_CHANNEL_STATE_READY)
           && (complementary_channel_state == HAL_TIM_CHANNEL_STATE_READY))
  {
    if ((pData == NULL) || (Length == 0U))
    {
      return HAL_ERROR;
    }
    else
    {
      TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);
      TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);
    }
  }
  else
  {
    return HAL_ERROR;
  }

  /* 使能输入捕获通道 */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 设置 DMA 捕获回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->CCR1, (uint32_t)pData,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 捕获/比较 1 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 设置 DMA 捕获回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->CCR2, (uint32_t)pData,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 捕获/比较 2 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 设置 DMA 捕获回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMACaptureCplt;
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)&htim->Instance->CCR3, (uint32_t)pData,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 捕获/比较 3 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 设置 DMA 捕获回调函数 */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMACaptureCplt;
      htim->hdma[TIM_DMA_ID_CC4]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)&htim->Instance->CCR4, (uint32_t)pData,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 捕获/比较 4 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC4);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  /* 使能外设，但在触发模式下除外（触发模式下会随触发信号自动使能） */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
  {
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
    {
      __HAL_TIM_ENABLE(htim);
    }
  }
  else
  {
    __HAL_TIM_ENABLE(htim);
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  停止 DMA 模式下的 TIM 输入捕获测量。
  * @param  htim TIM 输入捕获句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_IC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));
  assert_param(IS_TIM_DMA_CC_INSTANCE(htim->Instance));

  /* 禁用输入捕获通道 */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 禁用 TIM 捕获/比较 1 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 禁用 TIM 捕获/比较 2 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 禁用 TIM 捕获/比较 3 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC3]);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 禁用 TIM 捕获/比较 4 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC4);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC4]);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 禁用外设 */
    __HAL_TIM_DISABLE(htim);

    /* 设置 TIM 通道状态 */
    TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数状态 */
  return status;
}
/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group5 TIM One Pulse functions
  *  @brief    TIM 单脉冲功能函数
  *
@verbatim
  ==============================================================================
                        ##### TIM 单脉冲功能函数 #####
  ==============================================================================
  [..]
    本节提供的函数允许实现以下操作：
    (+) 初始化并配置 TIM 单脉冲模式。
    (+) 反初始化 TIM 单脉冲模式。
    (+) 启动 TIM 单脉冲模式。
    (+) 停止 TIM 单脉冲模式。
    (+) 启动 TIM 单脉冲模式并使能中断。
    (+) 停止 TIM 单脉冲模式并禁用中断。
    (+) 启动 TIM 单脉冲模式并使能 DMA 传输。
    (+) 停止 TIM 单脉冲模式并禁用 DMA 传输。

@endverbatim
  * @{
  */
/**
  * @brief  根据 TIM_HandleTypeDef 中指定的参数初始化 TIM 单脉冲时基，
  *         并初始化相关联的句柄。
  * @note   从中央对齐计数模式切换到边沿计数模式（或反之），
  *         需要复位定时器以避免由于中央对齐模式下 DIR 位为只读
  *         而导致的非预期计数方向。
  *         例如：在调用 HAL_TIM_OnePulse_Init() 之前先调用 @ref HAL_TIM_OnePulse_DeInit()
  * @note   当定时器实例被初始化为单脉冲模式时，定时器
  *         通道 1 和通道 2 将被保留，不可用于其他
  *         用途。
  * @param  htim TIM 单脉冲句柄
  * @param  OnePulseMode 选择单脉冲模式。
  *         该参数可以是以下值之一：
  *            @arg TIM_OPMODE_SINGLE: 仅产生一个脉冲。
  *            @arg TIM_OPMODE_REPETITIVE: 产生重复脉冲。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Init(TIM_HandleTypeDef *htim, uint32_t OnePulseMode)
{
  /* 检查 TIM 句柄分配 */
  if (htim == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数有效性 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_OPM_MODE(OnePulseMode));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));

  if (htim->State == HAL_TIM_STATE_RESET)
  {
    /* 分配锁资源并进行初始化 */
    htim->Lock = HAL_UNLOCKED;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
    /* 将中断回调函数重置为传统的弱定义回调函数 */
    TIM_ResetCallback(htim);

    if (htim->OnePulse_MspInitCallback == NULL)
    {
      htim->OnePulse_MspInitCallback = HAL_TIM_OnePulse_MspInit;
    }
    /* 初始化底层硬件：GPIO、时钟、NVIC */
    htim->OnePulse_MspInitCallback(htim);
#else
    /* 初始化底层硬件：GPIO、时钟、NVIC 和 DMA */
    HAL_TIM_OnePulse_MspInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }

  /* 设置 TIM 状态 */
  htim->State = HAL_TIM_STATE_BUSY;

  /* 配置单脉冲模式下的时基单元 */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* 复位 OPM 位 */
  htim->Instance->CR1 &= ~TIM_CR1_OPM;

  /* 配置 OPM 模式 */
  htim->Instance->CR1 |= OnePulseMode;

  /* 初始化 DMA 突发传输操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;

  /* 初始化 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);

  /* 初始化 TIM 状态 */
  htim->State = HAL_TIM_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  反初始化 TIM 单脉冲模式
  * @param  htim TIM 单脉冲句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_DeInit(TIM_HandleTypeDef *htim)
{
  /* 检查参数 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* 禁用 TIM 外设时钟 */
  __HAL_TIM_DISABLE(htim);

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  if (htim->OnePulse_MspDeInitCallback == NULL)
  {
    htim->OnePulse_MspDeInitCallback = HAL_TIM_OnePulse_MspDeInit;
  }
  /* 反初始化底层硬件 */
  htim->OnePulse_MspDeInitCallback(htim);
#else
  /* 反初始化底层硬件：GPIO、时钟、NVIC */
  HAL_TIM_OnePulse_MspDeInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* 改变 DMA 突发操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_RESET);
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_RESET);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_RESET);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_RESET);

  /* 改变 TIM 状态 */
  htim->State = HAL_TIM_STATE_RESET;

  /* 释放锁 */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  初始化 TIM 单脉冲 MSP。
  * @param  htim TIM 单脉冲句柄
  * @retval 无
  */
__weak void HAL_TIM_OnePulse_MspInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数。当需要该回调函数时，
            可以在用户文件中重新实现 HAL_TIM_OnePulse_MspInit
   */
}

/**
  * @brief  反初始化 TIM 单脉冲 MSP。
  * @param  htim TIM 单脉冲句柄
  * @retval 无
  */
__weak void HAL_TIM_OnePulse_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数。当需要该回调函数时，
            可以在用户文件中重新实现 HAL_TIM_OnePulse_MspDeInit
   */
}

/**
  * @brief  启动 TIM 单脉冲信号生成。
  * @note 尽管 OutputChannel 参数已被弃用且被该函数忽略，
  *        但保留该参数是为了避免破坏 HAL_TIM API 兼容性。
  * @note 脉冲输出通道在调用
  *       @ref HAL_TIM_OnePulse_ConfigChannel() 时确定。
  * @param  htim TIM 单脉冲句柄
  * @param  OutputChannel 参见上述注意说明
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Start(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);

  /* 防止未使用的参数产生编译警告 */
  UNUSED(OutputChannel);

  /* 检查 TIM 各通道状态 */
  if ((channel_1_state != HAL_TIM_CHANNEL_STATE_READY)
      || (channel_2_state != HAL_TIM_CHANNEL_STATE_READY)
      || (complementary_channel_1_state != HAL_TIM_CHANNEL_STATE_READY)
      || (complementary_channel_2_state != HAL_TIM_CHANNEL_STATE_READY))
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 各通道状态 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);

  /* 使能输出比较和输入捕获通道
    (在 OPM 模式下，仅有两个可用通道分别为 TIM_CHANNEL_1 和 TIM_CHANNEL_2)
    若 TIM_CHANNEL_1 用作输出，则 TIM_CHANNEL_2 将用作输入；
    若 TIM_CHANNEL_1 用作输入，则 TIM_CHANNEL_2 将用作输出；
    无论哪种组合，TIM_CHANNEL_1 和 TIM_CHANNEL_2 都应同时使能

    无需使能计数器，硬件会自动使其使能
    (计数器响应激励信号启动并生成脉冲) */

  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
  {
    /* 使能主输出 */
    __HAL_TIM_MOE_ENABLE(htim);
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  停止 TIM 单脉冲信号生成。
  * @note 尽管 OutputChannel 参数已被弃用且会被函数忽略，
  *        但保留该参数是为了避免破坏 HAL_TIM API 兼容性。
  * @note 脉冲输出通道在调用
  *       @ref HAL_TIM_OnePulse_ConfigChannel() 时确定。
  * @param  htim TIM 单脉冲句柄
  * @param  OutputChannel 参见上述注意说明
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Stop(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(OutputChannel);

  /* 禁用输出比较和输入捕获通道
  (在 OPM 模式下，仅有两个可用通道分别为 TIM_CHANNEL_1 和 TIM_CHANNEL_2)
  若 TIM_CHANNEL_1 用作输出，则 TIM_CHANNEL_2 将用作输入；
  若 TIM_CHANNEL_1 用作输入，则 TIM_CHANNEL_2 将用作输出；
  无论哪种组合，TIM_CHANNEL_1 和 TIM_CHANNEL_2 都应同时禁用 */

  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
  {
    /* 禁用主输出 */
    __HAL_TIM_MOE_DISABLE(htim);
  }

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  在中断模式下启动 TIM 单脉冲信号生成。
  * @note 尽管 OutputChannel 参数已被弃用且会被函数忽略，
  *        但保留该参数是为了避免破坏 HAL_TIM API 兼容性。
  * @note 脉冲输出通道在调用
  *       @ref HAL_TIM_OnePulse_ConfigChannel() 时确定。
  * @param  htim TIM 单脉冲句柄
  * @param  OutputChannel 参见上述注意说明
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Start_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);

  /* 防止未使用的参数产生编译警告 */
  UNUSED(OutputChannel);

  /* 检查 TIM 通道状态 */
  if ((channel_1_state != HAL_TIM_CHANNEL_STATE_READY)
      || (channel_2_state != HAL_TIM_CHANNEL_STATE_READY)
      || (complementary_channel_1_state != HAL_TIM_CHANNEL_STATE_READY)
      || (complementary_channel_2_state != HAL_TIM_CHANNEL_STATE_READY))
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);

  /* 使能输出比较和输入捕获通道
    (在 OPM 模式下，仅有两个可用通道分别为 TIM_CHANNEL_1 和 TIM_CHANNEL_2)
    若 TIM_CHANNEL_1 用作输出，则 TIM_CHANNEL_2 将用作输入；
    若 TIM_CHANNEL_1 用作输入，则 TIM_CHANNEL_2 将用作输出；
    无论哪种组合，TIM_CHANNEL_1 和 TIM_CHANNEL_2 都应同时使能

    无需使能计数器，硬件会自动使其使能
    (计数器响应激励信号启动并生成脉冲) */

  /* 使能 TIM 捕获/比较 1 中断 */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);

  /* 使能 TIM 捕获/比较 2 中断 */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);

  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
  {
    /* 使能主输出 */
    __HAL_TIM_MOE_ENABLE(htim);
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  在中断模式下停止 TIM 单脉冲信号生成。
  * @note 尽管 OutputChannel 参数已弃用且被函数忽略，
  *        但保留该参数以避免破坏 HAL_TIM API 的兼容性。
  * @note 脉冲输出通道在调用
  *       @ref HAL_TIM_OnePulse_ConfigChannel() 时确定。
  * @param  htim TIM 单脉冲句柄
  * @param  OutputChannel 参见上述说明
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Stop_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(OutputChannel);

  /* 禁用 TIM 捕获/比较 1 中断 */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);

  /* 禁用 TIM 捕获/比较 2 中断 */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);

  /* 禁用捕获比较和输入捕获通道
  （在 OPM 模式下，两个可用的通道是 TIM_CHANNEL_1 和 TIM_CHANNEL_2）
  如果 TIM_CHANNEL_1 用作输出，则 TIM_CHANNEL_2 将用作输入；
  如果 TIM_CHANNEL_1 用作输入，则 TIM_CHANNEL_2 将用作输出；
  无论哪种组合，TIM_CHANNEL_1 和 TIM_CHANNEL_2 都应同时禁用 */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);

  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
  {
    /* 禁用主输出 */
    __HAL_TIM_MOE_DISABLE(htim);
  }

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group6 TIM Encoder functions
  *  @brief    TIM 编码器功能函数
  *
@verbatim
  ==============================================================================
                          ##### TIM 编码器功能函数 #####
  ==============================================================================
  [..]
    本节提供的函数支持：
    (+) 初始化并配置 TIM 编码器。
    (+) 反初始化 TIM 编码器。
    (+) 启动 TIM 编码器。
    (+) 停止 TIM 编码器。
    (+) 启动 TIM 编码器并使能中断。
    (+) 停止 TIM 编码器并禁用中断。
    (+) 启动 TIM 编码器并使能 DMA 传输。
    (+) 停止 TIM 编码器并禁用 DMA 传输。

@endverbatim
  * @{
  */
/**
  * @brief  初始化 TIM 编码器接口并初始化关联的句柄。
  * @note   从中央对齐计数模式切换到边沿计数模式（或反向切换）
  *         需要复位定时器，以避免由于中央对齐模式下 DIR 位只读
  *         而导致的异常计数方向。
  *         例如：在调用 HAL_TIM_Encoder_Init() 之前先调用 @ref HAL_TIM_Encoder_DeInit()
  * @note   编码器模式与外部时钟模式 2 不兼容，切勿同时选择
  *         例如：调用 @ref HAL_TIM_Encoder_Init 将会清除使用 TIM_CLOCKSOURCE_ETRMODE2 的
  *         @ref HAL_TIM_ConfigClockSource 配置，反之亦然
  * @note   当定时器实例在编码器模式下初始化时，定时器
  *         通道 1 和通道 2 将被保留，不能用于其他
  *         用途。
  * @param  htim TIM 编码器接口句柄
  * @param  sConfig TIM 编码器接口配置结构体
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Init(TIM_HandleTypeDef *htim, const TIM_Encoder_InitTypeDef *sConfig)
{
  uint32_t tmpsmcr;
  uint32_t tmpccmr1;
  uint32_t tmpccer;

  /* 检查 TIM 句柄分配 */
  if (htim == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数有效性 */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));
  assert_param(IS_TIM_ENCODER_MODE(sConfig->EncoderMode));
  assert_param(IS_TIM_IC_SELECTION(sConfig->IC1Selection));
  assert_param(IS_TIM_IC_SELECTION(sConfig->IC2Selection));
  assert_param(IS_TIM_ENCODERINPUT_POLARITY(sConfig->IC1Polarity));
  assert_param(IS_TIM_ENCODERINPUT_POLARITY(sConfig->IC2Polarity));
  assert_param(IS_TIM_IC_PRESCALER(sConfig->IC1Prescaler));
  assert_param(IS_TIM_IC_PRESCALER(sConfig->IC2Prescaler));
  assert_param(IS_TIM_IC_FILTER(sConfig->IC1Filter));
  assert_param(IS_TIM_IC_FILTER(sConfig->IC2Filter));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));

  if (htim->State == HAL_TIM_STATE_RESET)
  {
    /* 分配互斥锁资源并进行初始化 */
    htim->Lock = HAL_UNLOCKED;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
    /* 将中断回调函数重置为传统的弱定义回调函数 */
    TIM_ResetCallback(htim);

    if (htim->Encoder_MspInitCallback == NULL)
    {
      htim->Encoder_MspInitCallback = HAL_TIM_Encoder_MspInit;
    }
    /* 初始化底层硬件：GPIO、CLOCK、NVIC */
    htim->Encoder_MspInitCallback(htim);
#else
    /* 初始化底层硬件：GPIO、CLOCK、NVIC 和 DMA */
    HAL_TIM_Encoder_MspInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }

  /* 设置 TIM 状态 */
  htim->State = HAL_TIM_STATE_BUSY;

  /* 复位 SMS 和 ECE 位 */
  htim->Instance->SMCR &= ~(TIM_SMCR_SMS | TIM_SMCR_ECE);

  /* 配置编码器模式下的时基单元 */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* 获取 TIMx SMCR 寄存器的值 */
  tmpsmcr = htim->Instance->SMCR;

  /* 获取 TIMx CCMR1 寄存器的值 */
  tmpccmr1 = htim->Instance->CCMR1;

  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = htim->Instance->CCER;

  /* 设置编码器模式 */
  tmpsmcr |= sConfig->EncoderMode;

  /* 将捕获/比较 1 和捕获/比较 2 选择为输入 */
  tmpccmr1 &= ~(TIM_CCMR1_CC1S | TIM_CCMR1_CC2S);
  tmpccmr1 |= (sConfig->IC1Selection | (sConfig->IC2Selection << 8U));

  /* 设置捕获/比较 1 和捕获/比较 2 的预分频器及滤波器 */
  tmpccmr1 &= ~(TIM_CCMR1_IC1PSC | TIM_CCMR1_IC2PSC);
  tmpccmr1 &= ~(TIM_CCMR1_IC1F | TIM_CCMR1_IC2F);
  tmpccmr1 |= sConfig->IC1Prescaler | (sConfig->IC2Prescaler << 8U);
  tmpccmr1 |= (sConfig->IC1Filter << 4U) | (sConfig->IC2Filter << 12U);

  /* 设置 TI1 和 TI2 的极性 */
  tmpccer &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
  tmpccer |= sConfig->IC1Polarity | (sConfig->IC2Polarity << 4U);

  /* 写入 TIMx SMCR 寄存器 */
  htim->Instance->SMCR = tmpsmcr;

  /* 写入 TIMx CCMR1 寄存器 */
  htim->Instance->CCMR1 = tmpccmr1;

  /* 写入 TIMx CCER 寄存器 */
  htim->Instance->CCER = tmpccer;

  /* 初始化 DMA 突发操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_READY;

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);

  /* 初始化 TIM 状态 */
  htim->State = HAL_TIM_STATE_READY;

  return HAL_OK;
}


/**
  * @brief  反初始化 TIM 编码器接口
  * @param  htim TIM 编码器接口句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Encoder_DeInit(TIM_HandleTypeDef *htim)
{
  /* 检查参数 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* 禁用 TIM 外设时钟 */
  __HAL_TIM_DISABLE(htim);

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  if (htim->Encoder_MspDeInitCallback == NULL)
  {
    htim->Encoder_MspDeInitCallback = HAL_TIM_Encoder_MspDeInit;
  }
  /* 反初始化底层硬件 */
  htim->Encoder_MspDeInitCallback(htim);
#else
  /* 反初始化底层硬件：GPIO、时钟、NVIC */
  HAL_TIM_Encoder_MspDeInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* 改变 DMA 突发传输操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_RESET);
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_RESET);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_RESET);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_RESET);

  /* 改变 TIM 状态 */
  htim->State = HAL_TIM_STATE_RESET;

  /* 释放锁 */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  初始化 TIM 编码器接口 MSP。
  * @param  htim TIM 编码器接口句柄
  * @retval 无
  */
__weak void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(htim);

  /* NOTE : 不应修改此函数，当需要回调函数时，
            可以在用户文件中实现 HAL_TIM_Encoder_MspInit
   */
}

/**
  * @brief  反初始化 TIM 编码器接口 MSP。
  * @param  htim TIM 编码器接口句柄
  * @retval 无
  */
__weak void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(htim);

  /* NOTE : 不应修改此函数，当需要回调函数时，
            可以在用户文件中实现 HAL_TIM_Encoder_MspDeInit
   */
}

/**
  * @brief  启动 TIM 编码器接口。
  * @param  htim TIM 编码器接口句柄
  * @param  Channel 要使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_ALL: 同时选择 TIM 通道 1 和 TIM 通道 2
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);

  /* 检查参数 */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));

  /* 设置 TIM 通道状态 */
  if (Channel == TIM_CHANNEL_1)
  {
    if ((channel_1_state != HAL_TIM_CHANNEL_STATE_READY)
        || (complementary_channel_1_state != HAL_TIM_CHANNEL_STATE_READY))
    {
      return HAL_ERROR;
    }
    else
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
    }
  }
  else if (Channel == TIM_CHANNEL_2)
  {
    if ((channel_2_state != HAL_TIM_CHANNEL_STATE_READY)
        || (complementary_channel_2_state != HAL_TIM_CHANNEL_STATE_READY))
    {
      return HAL_ERROR;
    }
    else
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
    }
  }
  else
  {
    if ((channel_1_state != HAL_TIM_CHANNEL_STATE_READY)
        || (channel_2_state != HAL_TIM_CHANNEL_STATE_READY)
        || (complementary_channel_1_state != HAL_TIM_CHANNEL_STATE_READY)
        || (complementary_channel_2_state != HAL_TIM_CHANNEL_STATE_READY))
    {
      return HAL_ERROR;
    }
    else
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
    }
  }

  /* 使能编码器接口通道 */
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
      break;
    }

    case TIM_CHANNEL_2:
    {
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
      break;
    }

    default :
    {
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
      break;
    }
  }
  /* 使能外设 */
  __HAL_TIM_ENABLE(htim);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  停止 TIM 编码器接口。
  * @param  htim TIM 编码器接口句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_ALL: 选择 TIM 通道 1 和 TIM 通道 2
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* 检查参数 */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));

  /* 禁用输入捕获通道 1 和 2
    （在编码器接口中，可使用的两个通道为 TIM_CHANNEL_1 和 TIM_CHANNEL_2） */
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
      break;
    }

    case TIM_CHANNEL_2:
    {
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);
      break;
    }

    default :
    {
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);
      break;
    }
  }

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 通道状态 */
  if ((Channel == TIM_CHANNEL_1) || (Channel == TIM_CHANNEL_2))
  {
    TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }
  else
  {
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  以中断模式启动 TIM 编码器接口。
  * @param  htim TIM 编码器接口句柄
  * @param  Channel 要启用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_ALL: 选择 TIM 通道 1 和 TIM 通道 2
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);

  /* 检查参数 */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));

  /* 设置 TIM 通道状态 */
  if (Channel == TIM_CHANNEL_1)
  {
    if ((channel_1_state != HAL_TIM_CHANNEL_STATE_READY)
        || (complementary_channel_1_state != HAL_TIM_CHANNEL_STATE_READY))
    {
      return HAL_ERROR;
    }
    else
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
    }
  }
  else if (Channel == TIM_CHANNEL_2)
  {
    if ((channel_2_state != HAL_TIM_CHANNEL_STATE_READY)
        || (complementary_channel_2_state != HAL_TIM_CHANNEL_STATE_READY))
    {
      return HAL_ERROR;
    }
    else
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
    }
  }
  else
  {
    if ((channel_1_state != HAL_TIM_CHANNEL_STATE_READY)
        || (channel_2_state != HAL_TIM_CHANNEL_STATE_READY)
        || (complementary_channel_1_state != HAL_TIM_CHANNEL_STATE_READY)
        || (complementary_channel_2_state != HAL_TIM_CHANNEL_STATE_READY))
    {
      return HAL_ERROR;
    }
    else
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
    }
  }

  /* 使能编码器接口通道 */
  /* 使能捕获比较中断 1 和/或 2 */
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
      break;
    }

    default :
    {
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
      break;
    }
  }

  /* 使能外设 */
  __HAL_TIM_ENABLE(htim);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  以中断模式停止 TIM 编码器接口。
  * @param  htim TIM 编码器接口句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_ALL: 选择 TIM 通道 1 和 TIM 通道 2
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* 检查参数 */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));

  /* 禁用输入捕获通道 1 和 2
    （在编码器接口中，可使用的两个通道为 TIM_CHANNEL_1 和 TIM_CHANNEL_2） */
  if (Channel == TIM_CHANNEL_1)
  {
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);

    /* 禁用捕获/比较中断 1 */
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
  }
  else if (Channel == TIM_CHANNEL_2)
  {
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);

    /* 禁用捕获/比较中断 2 */
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
  }
  else
  {
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);

    /* 禁用捕获/比较中断 1 和 2 */
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
  }

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 通道状态 */
  if ((Channel == TIM_CHANNEL_1) || (Channel == TIM_CHANNEL_2))
  {
    TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }
  else
  {
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数执行状态 */
  return HAL_OK;
}

/**
  * @brief  以 DMA 模式启动 TIM 编码器接口。
  * @param  htim TIM 编码器接口句柄
  * @param  Channel 要使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_ALL: 同时选择 TIM 通道 1 和 TIM 通道 2
  * @param  pData1 IC1 的目标缓冲区地址。
  * @param  pData2 IC2 的目标缓冲区地址。
  * @param  Length 从 TIM 外设传输到内存的数据长度。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData1,
                                            uint32_t *pData2, uint16_t Length)
{
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);

  /* 检查参数 */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));

  /* 设置 TIM 通道状态 */
  if (Channel == TIM_CHANNEL_1)
  {
    if ((channel_1_state == HAL_TIM_CHANNEL_STATE_BUSY)
        || (complementary_channel_1_state == HAL_TIM_CHANNEL_STATE_BUSY))
    {
      return HAL_BUSY;
    }
    else if ((channel_1_state == HAL_TIM_CHANNEL_STATE_READY)
             && (complementary_channel_1_state == HAL_TIM_CHANNEL_STATE_READY))
    {
      if ((pData1 == NULL) || (Length == 0U))
      {
        return HAL_ERROR;
      }
      else
      {
        TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
        TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
      }
    }
    else
    {
      return HAL_ERROR;
    }
  }
  else if (Channel == TIM_CHANNEL_2)
  {
    if ((channel_2_state == HAL_TIM_CHANNEL_STATE_BUSY)
        || (complementary_channel_2_state == HAL_TIM_CHANNEL_STATE_BUSY))
    {
      return HAL_BUSY;
    }
    else if ((channel_2_state == HAL_TIM_CHANNEL_STATE_READY)
             && (complementary_channel_2_state == HAL_TIM_CHANNEL_STATE_READY))
    {
      if ((pData2 == NULL) || (Length == 0U))
      {
        return HAL_ERROR;
      }
      else
      {
        TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
        TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
      }
    }
    else
    {
      return HAL_ERROR;
    }
  }
  else
  {
    if ((channel_1_state == HAL_TIM_CHANNEL_STATE_BUSY)
        || (channel_2_state == HAL_TIM_CHANNEL_STATE_BUSY)
        || (complementary_channel_1_state == HAL_TIM_CHANNEL_STATE_BUSY)
        || (complementary_channel_2_state == HAL_TIM_CHANNEL_STATE_BUSY))
    {
      return HAL_BUSY;
    }
    else if ((channel_1_state == HAL_TIM_CHANNEL_STATE_READY)
             && (channel_2_state == HAL_TIM_CHANNEL_STATE_READY)
             && (complementary_channel_1_state == HAL_TIM_CHANNEL_STATE_READY)
             && (complementary_channel_2_state == HAL_TIM_CHANNEL_STATE_READY))
    {
      if ((((pData1 == NULL) || (pData2 == NULL))) || (Length == 0U))
      {
        return HAL_ERROR;
      }
      else
      {
        TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
        TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
        TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_BUSY);
        TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_BUSY);
      }
    }
    else
    {
      return HAL_ERROR;
    }
  }

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 设置 DMA 捕获回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->CCR1, (uint32_t)pData1,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 输入捕获 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);

      /* 使能捕获/比较通道 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);

      /* 使能外设 */
      __HAL_TIM_ENABLE(htim);

      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 设置 DMA 捕获回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError;
      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->CCR2, (uint32_t)pData2,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 输入捕获 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);

      /* 使能捕获/比较通道 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);

      /* 使能外设 */
      __HAL_TIM_ENABLE(htim);

      break;
    }

    default:
    {
      /* 设置 DMA 捕获回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->CCR1, (uint32_t)pData1,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }

      /* 设置 DMA 捕获回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->CCR2, (uint32_t)pData2,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }

      /* 使能 TIM 输入捕获 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
      /* 使能 TIM 输入捕获 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);

      /* 使能捕获/比较通道 */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);

      /* 使能外设 */
      __HAL_TIM_ENABLE(htim);

      break;
    }
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  以 DMA 模式停止 TIM 编码器接口。
  * @param  htim TIM 编码器接口句柄
  * @param  Channel 要使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_ALL: 同时选择 TIM 通道 1 和 TIM 通道 2
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* 检查参数 */
  assert_param(IS_TIM_ENCODER_INTERFACE_INSTANCE(htim->Instance));

  /* 禁用输入捕获通道 1 和通道 2
    (在编码器接口中，允许使用的两个通道为 TIM_CHANNEL_1 和 TIM_CHANNEL_2) */
  if (Channel == TIM_CHANNEL_1)
  {
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);

    /* 禁用捕获/比较 DMA 请求 1 */
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
    (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);
  }
  else if (Channel == TIM_CHANNEL_2)
  {
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);

    /* 禁用捕获/比较 DMA 请求 2 */
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
    (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);
  }
  else
  {
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);

    /* 禁用捕获/比较 DMA 请求 1 和 2 */
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
    (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);
    (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);
  }

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 通道状态 */
  if ((Channel == TIM_CHANNEL_1) || (Channel == TIM_CHANNEL_2))
  {
    TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }
  else
  {
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @}
  */
/** @defgroup TIM_Exported_Functions_Group7 TIM 中断处理程序管理
  *  @brief    TIM 中断处理程序管理
  *
@verbatim
  ==============================================================================
                        ##### 中断处理程序管理 #####
  ==============================================================================
  [..]
    本节提供定时器中断处理函数。

@endverbatim
  * @{
  */
/**
  * @brief  此函数用于处理 TIM 中断请求。
  * @param  htim TIM 句柄
  * @retval 无
  */
void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim)
{
  uint32_t itsource = htim->Instance->DIER;
  uint32_t itflag   = htim->Instance->SR;

  /* 捕获/比较 1 事件 */
  if ((itflag & (TIM_FLAG_CC1)) == (TIM_FLAG_CC1))
  {
    if ((itsource & (TIM_IT_CC1)) == (TIM_IT_CC1))
    {
      {
        __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_CC1);
        htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;

        /* 输入捕获事件 */
        if ((htim->Instance->CCMR1 & TIM_CCMR1_CC1S) != 0x00U)
        {
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
          htim->IC_CaptureCallback(htim);
#else
          HAL_TIM_IC_CaptureCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
        }
        /* 输出比较事件 */
        else
        {
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
          htim->OC_DelayElapsedCallback(htim);
          htim->PWM_PulseFinishedCallback(htim);
#else
          HAL_TIM_OC_DelayElapsedCallback(htim);
          HAL_TIM_PWM_PulseFinishedCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
        }
        htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
      }
    }
  }
  /* 捕获/比较 2 事件 */
  if ((itflag & (TIM_FLAG_CC2)) == (TIM_FLAG_CC2))
  {
    if ((itsource & (TIM_IT_CC2)) == (TIM_IT_CC2))
    {
      __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_CC2);
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;
      /* 输入捕获事件 */
      if ((htim->Instance->CCMR1 & TIM_CCMR1_CC2S) != 0x00U)
      {
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
        htim->IC_CaptureCallback(htim);
#else
        HAL_TIM_IC_CaptureCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
      }
      /* 输出比较事件 */
      else
      {
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
        htim->OC_DelayElapsedCallback(htim);
        htim->PWM_PulseFinishedCallback(htim);
#else
        HAL_TIM_OC_DelayElapsedCallback(htim);
        HAL_TIM_PWM_PulseFinishedCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
      }
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
    }
  }
  /* Capture compare 3 event */
  if ((itflag & (TIM_FLAG_CC3)) == (TIM_FLAG_CC3))
  {
    if ((itsource & (TIM_IT_CC3)) == (TIM_IT_CC3))
    {
      __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_CC3);
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;
      /* Input capture event */
      if ((htim->Instance->CCMR2 & TIM_CCMR2_CC3S) != 0x00U)
      {
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
        htim->IC_CaptureCallback(htim);
#else
        HAL_TIM_IC_CaptureCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
      }
      /* Output compare event */
      else
      {
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
        htim->OC_DelayElapsedCallback(htim);
        htim->PWM_PulseFinishedCallback(htim);
#else
        HAL_TIM_OC_DelayElapsedCallback(htim);
        HAL_TIM_PWM_PulseFinishedCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
      }
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
    }
  }
  /* Capture compare 4 event */
  if ((itflag & (TIM_FLAG_CC4)) == (TIM_FLAG_CC4))
  {
    if ((itsource & (TIM_IT_CC4)) == (TIM_IT_CC4))
    {
      __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_CC4);
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;
      /* Input capture event */
      if ((htim->Instance->CCMR2 & TIM_CCMR2_CC4S) != 0x00U)
      {
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
        htim->IC_CaptureCallback(htim);
#else
        HAL_TIM_IC_CaptureCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
      }
      /* Output compare event */
      else
      {
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
        htim->OC_DelayElapsedCallback(htim);
        htim->PWM_PulseFinishedCallback(htim);
#else
        HAL_TIM_OC_DelayElapsedCallback(htim);
        HAL_TIM_PWM_PulseFinishedCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
      }
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
    }
  }
  /* TIM Update event */
  if ((itflag & (TIM_FLAG_UPDATE)) == (TIM_FLAG_UPDATE))
  {
    if ((itsource & (TIM_IT_UPDATE)) == (TIM_IT_UPDATE))
    {
      __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
      htim->PeriodElapsedCallback(htim);
#else
      HAL_TIM_PeriodElapsedCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
    }
  }
  /* TIM Break input event */
  if ((itflag & (TIM_FLAG_BREAK)) == (TIM_FLAG_BREAK))
  {
    if ((itsource & (TIM_IT_BREAK)) == (TIM_IT_BREAK))
    {
      __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_BREAK);
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
      htim->BreakCallback(htim);
#else
      HAL_TIMEx_BreakCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
    }
  }
  /* TIM Trigger detection event */
  if ((itflag & (TIM_FLAG_TRIGGER)) == (TIM_FLAG_TRIGGER))
  {
    if ((itsource & (TIM_IT_TRIGGER)) == (TIM_IT_TRIGGER))
    {
      __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_TRIGGER);
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
      htim->TriggerCallback(htim);
#else
      HAL_TIM_TriggerCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
    }
  }
  /* TIM 换向事件 */
  if ((itflag & (TIM_FLAG_COM)) == (TIM_FLAG_COM))
  {
    if ((itsource & (TIM_IT_COM)) == (TIM_IT_COM))
    {
      __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_COM);
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
      htim->CommutationCallback(htim);
#else
      HAL_TIMEx_CommutCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
    }
  }
}

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group8 TIM Peripheral Control functions
  *  @brief    TIM 外设控制函数
  *
@verbatim
  ==============================================================================
                   ##### 外设控制函数 #####
  ==============================================================================
 [..]
   本节提供的函数可用于：
      (+) 配置输入/输出通道以用于 OC、PWM、IC 或单脉冲模式。
      (+) 配置外部时钟源。
      (+) 配置互补通道、刹车特性以及死区时间。
      (+) 配置主从模式同步。
      (+) 配置 DMA 突发模式。

@endverbatim
  * @{
  */

/**
  * @brief  根据 TIM_OC_InitTypeDef 中指定的参数初始化 TIM 输出比较通道。
  * @param  htim TIM 输出比较句柄
  * @param  sConfig TIM 输出比较配置结构体
  * @param  Channel 待配置的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OC_ConfigChannel(TIM_HandleTypeDef *htim,
                                           const TIM_OC_InitTypeDef *sConfig,
                                           uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_CHANNELS(Channel));
  assert_param(IS_TIM_OC_MODE(sConfig->OCMode));
  assert_param(IS_TIM_OC_POLARITY(sConfig->OCPolarity));

  /* 进程锁定 */
  __HAL_LOCK(htim);

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));

      /* 将 TIM 通道 1 配置为输出比较模式 */
      TIM_OC1_SetConfig(htim->Instance, sConfig);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

      /* 将 TIM 通道 2 配置为输出比较模式 */
      TIM_OC2_SetConfig(htim->Instance, sConfig);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));

      /* 将 TIM 通道 3 配置为输出比较模式 */
      TIM_OC3_SetConfig(htim->Instance, sConfig);
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));

      /* 将 TIM 通道 4 配置为输出比较模式 */
      TIM_OC4_SetConfig(htim->Instance, sConfig);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  __HAL_UNLOCK(htim);

  return status;
}

/**
  * @brief  根据 TIM_IC_InitTypeDef 中指定的参数初始化 TIM 输入捕获通道。
  * @param  htim TIM IC 句柄
  * @param  sConfig TIM 输入捕获配置结构体
  * @param  Channel 要配置的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_IC_ConfigChannel(TIM_HandleTypeDef *htim, const TIM_IC_InitTypeDef *sConfig, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
  assert_param(IS_TIM_IC_POLARITY(sConfig->ICPolarity));
  assert_param(IS_TIM_IC_SELECTION(sConfig->ICSelection));
  assert_param(IS_TIM_IC_PRESCALER(sConfig->ICPrescaler));
  assert_param(IS_TIM_IC_FILTER(sConfig->ICFilter));

  /* 进程锁定 */
  __HAL_LOCK(htim);

  if (Channel == TIM_CHANNEL_1)
  {
    /* TI1 配置 */
    TIM_TI1_SetConfig(htim->Instance,
                      sConfig->ICPolarity,
                      sConfig->ICSelection,
                      sConfig->ICFilter);

    /* 复位 IC1PSC 位 */
    htim->Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC;

    /* 设置 IC1PSC 值 */
    htim->Instance->CCMR1 |= sConfig->ICPrescaler;
  }
  else if (Channel == TIM_CHANNEL_2)
  {
    /* TI2 配置 */
    assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

    TIM_TI2_SetConfig(htim->Instance,
                      sConfig->ICPolarity,
                      sConfig->ICSelection,
                      sConfig->ICFilter);

    /* 复位 IC2PSC 位 */
    htim->Instance->CCMR1 &= ~TIM_CCMR1_IC2PSC;

    /* 设置 IC2PSC 值 */
    htim->Instance->CCMR1 |= (sConfig->ICPrescaler << 8U);
  }
  else if (Channel == TIM_CHANNEL_3)
  {
    /* TI3 配置 */
    assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));

    TIM_TI3_SetConfig(htim->Instance,
                      sConfig->ICPolarity,
                      sConfig->ICSelection,
                      sConfig->ICFilter);

    /* 复位 IC3PSC 位 */
    htim->Instance->CCMR2 &= ~TIM_CCMR2_IC3PSC;

    /* 设置 IC3PSC 值 */
    htim->Instance->CCMR2 |= sConfig->ICPrescaler;
  }
  else if (Channel == TIM_CHANNEL_4)
  {
    /* TI4 配置 */
    assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));

    TIM_TI4_SetConfig(htim->Instance,
                      sConfig->ICPolarity,
                      sConfig->ICSelection,
                      sConfig->ICFilter);

    /* 复位 IC4PSC 位 */
    htim->Instance->CCMR2 &= ~TIM_CCMR2_IC4PSC;

    /* 设置 IC4PSC 值 */
    htim->Instance->CCMR2 |= (sConfig->ICPrescaler << 8U);
  }
  else
  {
    status = HAL_ERROR;
  }

  __HAL_UNLOCK(htim);

  return status;
}

/**
  * @brief  根据 TIM_OC_InitTypeDef 中指定的参数初始化 TIM PWM 通道。
  * @param  htim TIM PWM 句柄
  * @param  sConfig TIM PWM 配置结构体
  * @param  Channel 待配置的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim,
                                            const TIM_OC_InitTypeDef *sConfig,
                                            uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_CHANNELS(Channel));
  assert_param(IS_TIM_PWM_MODE(sConfig->OCMode));
  assert_param(IS_TIM_OC_POLARITY(sConfig->OCPolarity));
  assert_param(IS_TIM_FAST_STATE(sConfig->OCFastMode));

  /* 进程锁定 */
  __HAL_LOCK(htim);

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));

      /* 将通道 1 配置为 PWM 模式 */
      TIM_OC1_SetConfig(htim->Instance, sConfig);

      /* 设置通道 1 的预装载使能位 */
      htim->Instance->CCMR1 |= TIM_CCMR1_OC1PE;

      /* 配置输出快速模式 */
      htim->Instance->CCMR1 &= ~TIM_CCMR1_OC1FE;
      htim->Instance->CCMR1 |= sConfig->OCFastMode;
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

      /* 将通道 2 配置为 PWM 模式 */
      TIM_OC2_SetConfig(htim->Instance, sConfig);

      /* 设置通道 2 的预装载使能位 */
      htim->Instance->CCMR1 |= TIM_CCMR1_OC2PE;

      /* 配置输出快速模式 */
      htim->Instance->CCMR1 &= ~TIM_CCMR1_OC2FE;
      htim->Instance->CCMR1 |= sConfig->OCFastMode << 8U;
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));

      /* 将通道 3 配置为 PWM 模式 */
      TIM_OC3_SetConfig(htim->Instance, sConfig);

      /* 设置通道 3 的预装载使能位 */
      htim->Instance->CCMR2 |= TIM_CCMR2_OC3PE;

      /* 配置输出快速模式 */
      htim->Instance->CCMR2 &= ~TIM_CCMR2_OC3FE;
      htim->Instance->CCMR2 |= sConfig->OCFastMode;
      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));

      /* 将通道 4 配置为 PWM 模式 */
      TIM_OC4_SetConfig(htim->Instance, sConfig);

      /* 置位通道 4 的预装载使能位 */
      htim->Instance->CCMR2 |= TIM_CCMR2_OC4PE;

      /* 配置输出快速模式 */
      htim->Instance->CCMR2 &= ~TIM_CCMR2_OC4FE;
      htim->Instance->CCMR2 |= sConfig->OCFastMode << 8U;
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  __HAL_UNLOCK(htim);

  return status;
}

/**
  * @brief  根据 TIM_OnePulse_InitTypeDef 中指定的参数初始化 TIM 单脉冲通道。
  * @param  htim TIM 单脉冲句柄
  * @param  sConfig TIM 单脉冲配置结构体
  * @param  OutputChannel 要配置的 TIM 输出通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  * @param  InputChannel 要配置的 TIM 输入通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  * @note  若要以最小延时输出波形，用户可通过调用 @ref __HAL_TIM_ENABLE_OCxFAST 宏来使能快速模式。
  *        此时，一旦在 TIx 输入上检测到有效边沿，CCx 输出将被强制置位/响应，
  *        而无需等待比较结果。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_ConfigChannel(TIM_HandleTypeDef *htim,  TIM_OnePulse_InitTypeDef *sConfig,
                                                 uint32_t OutputChannel,  uint32_t InputChannel)
{
  HAL_StatusTypeDef status = HAL_OK;
  TIM_OC_InitTypeDef temp1;

  /* 检查参数 */
  assert_param(IS_TIM_OPM_CHANNELS(OutputChannel));
  assert_param(IS_TIM_OPM_CHANNELS(InputChannel));

  if (OutputChannel != InputChannel)
  {
    /* 进程已锁定 */
    __HAL_LOCK(htim);

    htim->State = HAL_TIM_STATE_BUSY;

    /* 从 sConfig 结构体中提取输出比较配置 */
    temp1.OCMode = sConfig->OCMode;
    temp1.Pulse = sConfig->Pulse;
    temp1.OCPolarity = sConfig->OCPolarity;
    temp1.OCNPolarity = sConfig->OCNPolarity;
    temp1.OCIdleState = sConfig->OCIdleState;
    temp1.OCNIdleState = sConfig->OCNIdleState;

    switch (OutputChannel)
    {
      case TIM_CHANNEL_1:
      {
        assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));

        TIM_OC1_SetConfig(htim->Instance, &temp1);
        break;
      }

      case TIM_CHANNEL_2:
      {
        assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

        TIM_OC2_SetConfig(htim->Instance, &temp1);
        break;
      }

      default:
        status = HAL_ERROR;
        break;
    }

    if (status == HAL_OK)
    {
      switch (InputChannel)
      {
        case TIM_CHANNEL_1:
        {
          assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));

          TIM_TI1_SetConfig(htim->Instance, sConfig->ICPolarity,
                            sConfig->ICSelection, sConfig->ICFilter);

          /* 复位 IC1PSC 位 */
          htim->Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC;

          /* 选择触发源 */
          htim->Instance->SMCR &= ~TIM_SMCR_TS;
          htim->Instance->SMCR |= TIM_TS_TI1FP1;

          /* 选择从模式 */
          htim->Instance->SMCR &= ~TIM_SMCR_SMS;
          htim->Instance->SMCR |= TIM_SLAVEMODE_TRIGGER;
          break;
        }

        case TIM_CHANNEL_2:
        {
          assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

          TIM_TI2_SetConfig(htim->Instance, sConfig->ICPolarity,
                            sConfig->ICSelection, sConfig->ICFilter);

          /* 复位 IC2PSC 位 */
          htim->Instance->CCMR1 &= ~TIM_CCMR1_IC2PSC;

          /* 选择触发源 */
          htim->Instance->SMCR &= ~TIM_SMCR_TS;
          htim->Instance->SMCR |= TIM_TS_TI2FP2;

          /* 选择从模式 */
          htim->Instance->SMCR &= ~TIM_SMCR_SMS;
          htim->Instance->SMCR |= TIM_SLAVEMODE_TRIGGER;
          break;
        }

        default:
          status = HAL_ERROR;
          break;
      }
    }

    htim->State = HAL_TIM_STATE_READY;

    __HAL_UNLOCK(htim);

    return status;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  配置 DMA 突发传输（Burst），将数据从内存传输到 TIM 外设
  * @param  htim TIM 句柄
  * @param  BurstBaseAddress TIM 基地址，DMA 将从此地址开始写入数据
  *         该参数可以是以下值之一：
  *            @arg TIM_DMABASE_CR1
  *            @arg TIM_DMABASE_CR2
  *            @arg TIM_DMABASE_SMCR
  *            @arg TIM_DMABASE_DIER
  *            @arg TIM_DMABASE_SR
  *            @arg TIM_DMABASE_EGR
  *            @arg TIM_DMABASE_CCMR1
  *            @arg TIM_DMABASE_CCMR2
  *            @arg TIM_DMABASE_CCER
  *            @arg TIM_DMABASE_CNT
  *            @arg TIM_DMABASE_PSC
  *            @arg TIM_DMABASE_ARR
  *            @arg TIM_DMABASE_RCR
  *            @arg TIM_DMABASE_CCR1
  *            @arg TIM_DMABASE_CCR2
  *            @arg TIM_DMABASE_CCR3
  *            @arg TIM_DMABASE_CCR4
  *            @arg TIM_DMABASE_BDTR
  * @param  BurstRequestSrc TIM DMA 请求源
  *         该参数可以是以下值之一：
  *            @arg TIM_DMA_UPDATE: TIM 更新中断源
  *            @arg TIM_DMA_CC1: TIM 捕获/比较 1 DMA 源
  *            @arg TIM_DMA_CC2: TIM 捕获/比较 2 DMA 源
  *            @arg TIM_DMA_CC3: TIM 捕获/比较 3 DMA 源
  *            @arg TIM_DMA_CC4: TIM 捕获/比较 4 DMA 源
  *            @arg TIM_DMA_COM: TIM 换向 DMA 源
  *            @arg TIM_DMA_TRIGGER: TIM 触发 DMA 源
  * @param  BurstBuffer 缓冲区地址。
  * @param  BurstLength DMA 突发传输长度。该参数可以是以下范围内的值：
  *         TIM_DMABURSTLENGTH_1TRANSFER 到 TIM_DMABURSTLENGTH_18TRANSFERS。
  * @note   仅当 BurstLength 等于 DMA 数据传输长度时，才应使用此函数。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_WriteStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress,
                                              uint32_t BurstRequestSrc, const uint32_t *BurstBuffer,
                                              uint32_t  BurstLength)
{
  HAL_StatusTypeDef status;

  status = HAL_TIM_DMABurst_MultiWriteStart(htim, BurstBaseAddress, BurstRequestSrc, BurstBuffer, BurstLength,
                                            ((BurstLength) >> 8U) + 1U);


  return status;
}

/**
  * @brief  配置 DMA 突发传输（Burst），将多组数据从内存传输到 TIM 外设
  * @param  htim TIM 句柄
  * @param  BurstBaseAddress TIM 基地址，DMA 将从此地址开始写入数据
  *         该参数可以是以下值之一：
  *            @arg TIM_DMABASE_CR1
  *            @arg TIM_DMABASE_CR2
  *            @arg TIM_DMABASE_SMCR
  *            @arg TIM_DMABASE_DIER
  *            @arg TIM_DMABASE_SR
  *            @arg TIM_DMABASE_EGR
  *            @arg TIM_DMABASE_CCMR1
  *            @arg TIM_DMABASE_CCMR2
  *            @arg TIM_DMABASE_CCER
  *            @arg TIM_DMABASE_CNT
  *            @arg TIM_DMABASE_PSC
  *            @arg TIM_DMABASE_ARR
  *            @arg TIM_DMABASE_RCR
  *            @arg TIM_DMABASE_CCR1
  *            @arg TIM_DMABASE_CCR2
  *            @arg TIM_DMABASE_CCR3
  *            @arg TIM_DMABASE_CCR4
  *            @arg TIM_DMABASE_BDTR
  * @param  BurstRequestSrc TIM DMA 请求源
  *         该参数可以是以下值之一：
  *            @arg TIM_DMA_UPDATE: TIM 更新中断源
  *            @arg TIM_DMA_CC1: TIM 捕获/比较 1 DMA 源
  *            @arg TIM_DMA_CC2: TIM 捕获/比较 2 DMA 源
  *            @arg TIM_DMA_CC3: TIM 捕获/比较 3 DMA 源
  *            @arg TIM_DMA_CC4: TIM 捕获/比较 4 DMA 源
  *            @arg TIM_DMA_COM: TIM 换向 DMA 源
  *            @arg TIM_DMA_TRIGGER: TIM 触发 DMA 源
  * @param  BurstBuffer 缓冲区地址。
  * @param  BurstLength DMA 突发传输长度。该参数可以是以下范围内的值：
  *         TIM_DMABURSTLENGTH_1TRANSFER 到 TIM_DMABURSTLENGTH_18TRANSFERS。
  * @param  DataLength 数据长度。该参数可以是 1 到 0xFFFF 之间的值。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_MultiWriteStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress,
                                                   uint32_t BurstRequestSrc, const uint32_t *BurstBuffer,
                                                   uint32_t  BurstLength,  uint32_t  DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_DMABURST_INSTANCE(htim->Instance));
  assert_param(IS_TIM_DMA_BASE(BurstBaseAddress));
  assert_param(IS_TIM_DMA_SOURCE(BurstRequestSrc));
  assert_param(IS_TIM_DMA_LENGTH(BurstLength));
  assert_param(IS_TIM_DMA_DATA_LENGTH(DataLength));

  if (htim->DMABurstState == HAL_DMA_BURST_STATE_BUSY)
  {
    return HAL_BUSY;
  }
  else if (htim->DMABurstState == HAL_DMA_BURST_STATE_READY)
  {
    if ((BurstBuffer == NULL) && (BurstLength > 0U))
    {
      return HAL_ERROR;
    }
    else
    {
      htim->DMABurstState = HAL_DMA_BURST_STATE_BUSY;
    }
  }
  else
  {
    /* 无需操作 */
  }

  switch (BurstRequestSrc)
  {
    case TIM_DMA_UPDATE:
    {
      /* 设置 DMA 周期到达（溢出）回调函数 */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = TIM_DMAPeriodElapsedCplt;
      htim->hdma[TIM_DMA_ID_UPDATE]->XferHalfCpltCallback = TIM_DMAPeriodElapsedHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)BurstBuffer,
                           (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    case TIM_DMA_CC1:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseCplt;
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)BurstBuffer,
                           (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    case TIM_DMA_CC2:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseCplt;
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)BurstBuffer,
                           (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    case TIM_DMA_CC3:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseCplt;
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)BurstBuffer,
                           (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    case TIM_DMA_CC4:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMADelayPulseCplt;
      htim->hdma[TIM_DMA_ID_CC4]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)BurstBuffer,
                           (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    case TIM_DMA_COM:
    {
      /* 设置 DMA 换向回调函数 */
      htim->hdma[TIM_DMA_ID_COMMUTATION]->XferCpltCallback =  TIMEx_DMACommutationCplt;
      htim->hdma[TIM_DMA_ID_COMMUTATION]->XferHalfCpltCallback =  TIMEx_DMACommutationHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_COMMUTATION]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_COMMUTATION], (uint32_t)BurstBuffer,
                           (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    case TIM_DMA_TRIGGER:
    {
      /* 设置 DMA 触发回调函数 */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferCpltCallback = TIM_DMATriggerCplt;
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferHalfCpltCallback = TIM_DMATriggerHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_TRIGGER], (uint32_t)BurstBuffer,
                           (uint32_t)&htim->Instance->DMAR, DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 配置 DMA 突发模式 */
    htim->Instance->DCR = (BurstBaseAddress | BurstLength);
    /* 使能 TIM DMA 请求 */
    __HAL_TIM_ENABLE_DMA(htim, BurstRequestSrc);
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  停止 TIM DMA 突发模式
  * @param  htim TIM 句柄
  * @param  BurstRequestSrc 要禁用的 TIM DMA 请求源
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_WriteStop(TIM_HandleTypeDef *htim, uint32_t BurstRequestSrc)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_DMA_SOURCE(BurstRequestSrc));

  /* 中止 DMA 传输（至少禁用 DMA 通道） */
  switch (BurstRequestSrc)
  {
    case TIM_DMA_UPDATE:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_UPDATE]);
      break;
    }
    case TIM_DMA_CC1:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);
      break;
    }
    case TIM_DMA_CC2:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);
      break;
    }
    case TIM_DMA_CC3:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC3]);
      break;
    }
    case TIM_DMA_CC4:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC4]);
      break;
    }
    case TIM_DMA_COM:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_COMMUTATION]);
      break;
    }
    case TIM_DMA_TRIGGER:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_TRIGGER]);
      break;
    }
    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 禁用 TIM 更新 DMA 请求 */
    __HAL_TIM_DISABLE_DMA(htim, BurstRequestSrc);

    /* 改变 DMA 突发操作状态 */
    htim->DMABurstState = HAL_DMA_BURST_STATE_READY;
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  配置 DMA 突发模式以将数据从 TIM 外设传输到内存
  * @param  htim TIM 句柄
  * @param  BurstBaseAddress DMA 开始读取数据的 TIM 基地址
  *         该参数可以是以下值之一：
  *            @arg TIM_DMABASE_CR1
  *            @arg TIM_DMABASE_CR2
  *            @arg TIM_DMABASE_SMCR
  *            @arg TIM_DMABASE_DIER
  *            @arg TIM_DMABASE_SR
  *            @arg TIM_DMABASE_EGR
  *            @arg TIM_DMABASE_CCMR1
  *            @arg TIM_DMABASE_CCMR2
  *            @arg TIM_DMABASE_CCER
  *            @arg TIM_DMABASE_CNT
  *            @arg TIM_DMABASE_PSC
  *            @arg TIM_DMABASE_ARR
  *            @arg TIM_DMABASE_RCR
  *            @arg TIM_DMABASE_CCR1
  *            @arg TIM_DMABASE_CCR2
  *            @arg TIM_DMABASE_CCR3
  *            @arg TIM_DMABASE_CCR4
  *            @arg TIM_DMABASE_BDTR
  * @param  BurstRequestSrc TIM DMA 请求源
  *         该参数可以是以下值之一：
  *            @arg TIM_DMA_UPDATE: TIM 更新中断源
  *            @arg TIM_DMA_CC1: TIM 捕获比较 1 DMA 源
  *            @arg TIM_DMA_CC2: TIM 捕获比较 2 DMA 源
  *            @arg TIM_DMA_CC3: TIM 捕获比较 3 DMA 源
  *            @arg TIM_DMA_CC4: TIM 捕获比较 4 DMA 源
  *            @arg TIM_DMA_COM: TIM 换向 DMA 源
  *            @arg TIM_DMA_TRIGGER: TIM 触发 DMA 源
  * @param  BurstBuffer 缓冲区地址。
  * @param  BurstLength DMA 突发传输长度。该参数可以是介于以下范围的一个值：
  *         TIM_DMABURSTLENGTH_1TRANSFER 和 TIM_DMABURSTLENGTH_18TRANSFERS。
  * @note   仅当 BurstLength 等于 DMA 数据传输长度时，才应使用此函数。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_ReadStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress,
                                             uint32_t BurstRequestSrc, uint32_t  *BurstBuffer, uint32_t  BurstLength)
{
  HAL_StatusTypeDef status;

  status = HAL_TIM_DMABurst_MultiReadStart(htim, BurstBaseAddress, BurstRequestSrc, BurstBuffer, BurstLength,
                                           ((BurstLength) >> 8U) + 1U);


  return status;
}

/**
  * @brief  配置 DMA 突发模式以将数据从 TIM 外设传输到内存
  * @param  htim TIM 句柄
  * @param  BurstBaseAddress DMA 开始读取数据的 TIM 基地址
  *         该参数可以是以下值之一：
  *            @arg TIM_DMABASE_CR1
  *            @arg TIM_DMABASE_CR2
  *            @arg TIM_DMABASE_SMCR
  *            @arg TIM_DMABASE_DIER
  *            @arg TIM_DMABASE_SR
  *            @arg TIM_DMABASE_EGR
  *            @arg TIM_DMABASE_CCMR1
  *            @arg TIM_DMABASE_CCMR2
  *            @arg TIM_DMABASE_CCER
  *            @arg TIM_DMABASE_CNT
  *            @arg TIM_DMABASE_PSC
  *            @arg TIM_DMABASE_ARR
  *            @arg TIM_DMABASE_RCR
  *            @arg TIM_DMABASE_CCR1
  *            @arg TIM_DMABASE_CCR2
  *            @arg TIM_DMABASE_CCR3
  *            @arg TIM_DMABASE_CCR4
  *            @arg TIM_DMABASE_BDTR
  * @param  BurstRequestSrc TIM DMA 请求源
  *         该参数可以是以下值之一：
  *            @arg TIM_DMA_UPDATE: TIM 更新中断源
  *            @arg TIM_DMA_CC1: TIM 捕获比较 1 DMA 源
  *            @arg TIM_DMA_CC2: TIM 捕获比较 2 DMA 源
  *            @arg TIM_DMA_CC3: TIM 捕获比较 3 DMA 源
  *            @arg TIM_DMA_CC4: TIM 捕获比较 4 DMA 源
  *            @arg TIM_DMA_COM: TIM 换向 DMA 源
  *            @arg TIM_DMA_TRIGGER: TIM 触发 DMA 源
  * @param  BurstBuffer 缓冲区地址。
  * @param  BurstLength DMA 突发传输长度。该参数可以是介于以下范围的一个值：
  *         TIM_DMABURSTLENGTH_1TRANSFER 和 TIM_DMABURSTLENGTH_18TRANSFERS。
  * @param  DataLength 数据长度。该参数可以是介于 1 和 0xFFFF 之间的一个值。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_MultiReadStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress,
                                                  uint32_t BurstRequestSrc, uint32_t  *BurstBuffer,
                                                  uint32_t  BurstLength, uint32_t  DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_DMABURST_INSTANCE(htim->Instance));
  assert_param(IS_TIM_DMA_BASE(BurstBaseAddress));
  assert_param(IS_TIM_DMA_SOURCE(BurstRequestSrc));
  assert_param(IS_TIM_DMA_LENGTH(BurstLength));
  assert_param(IS_TIM_DMA_DATA_LENGTH(DataLength));

  if (htim->DMABurstState == HAL_DMA_BURST_STATE_BUSY)
  {
    return HAL_BUSY;
  }
  else if (htim->DMABurstState == HAL_DMA_BURST_STATE_READY)
  {
    if ((BurstBuffer == NULL) && (BurstLength > 0U))
    {
      return HAL_ERROR;
    }
    else
    {
      htim->DMABurstState = HAL_DMA_BURST_STATE_BUSY;
    }
  }
  else
  {
    /* 无需任何操作 */
  }
  switch (BurstRequestSrc)
  {
    case TIM_DMA_UPDATE:
    {
      /* 设置 DMA 周期溢出回调函数 */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = TIM_DMAPeriodElapsedCplt;
      htim->hdma[TIM_DMA_ID_UPDATE]->XferHalfCpltCallback = TIM_DMAPeriodElapsedHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,
                           DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    case TIM_DMA_CC1:
    {
      /* 设置 DMA 输入捕获回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,
                           DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    case TIM_DMA_CC2:
    {
      /* 设置 DMA 输入捕获回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,
                           DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    case TIM_DMA_CC3:
    {
      /* 设置 DMA 输入捕获回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMACaptureCplt;
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,
                           DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    case TIM_DMA_CC4:
    {
      /* 设置 DMA 捕获回调函数 */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMACaptureCplt;
      htim->hdma[TIM_DMA_ID_CC4]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,
                           DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    case TIM_DMA_COM:
    {
      /* 设置 DMA 换向回调函数 */
      htim->hdma[TIM_DMA_ID_COMMUTATION]->XferCpltCallback =  TIMEx_DMACommutationCplt;
      htim->hdma[TIM_DMA_ID_COMMUTATION]->XferHalfCpltCallback =  TIMEx_DMACommutationHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_COMMUTATION]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_COMMUTATION], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,
                           DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    case TIM_DMA_TRIGGER:
    {
      /* 设置 DMA 触发回调函数 */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferCpltCallback = TIM_DMATriggerCplt;
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferHalfCpltCallback = TIM_DMATriggerHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferErrorCallback = TIM_DMAError ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_TRIGGER], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer,
                           DataLength) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      break;
    }
    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 配置 DMA 突发模式 */
    htim->Instance->DCR = (BurstBaseAddress | BurstLength);

    /* 使能 TIM DMA 请求 */
    __HAL_TIM_ENABLE_DMA(htim, BurstRequestSrc);
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  停止 DMA 突发读取
  * @param  htim TIM 句柄
  * @param  BurstRequestSrc 要禁用的 TIM DMA 请求源。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_ReadStop(TIM_HandleTypeDef *htim, uint32_t BurstRequestSrc)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_DMA_SOURCE(BurstRequestSrc));

  /* 中止 DMA 传输（至少禁用 DMA 通道） */
  switch (BurstRequestSrc)
  {
    case TIM_DMA_UPDATE:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_UPDATE]);
      break;
    }
    case TIM_DMA_CC1:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);
      break;
    }
    case TIM_DMA_CC2:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);
      break;
    }
    case TIM_DMA_CC3:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC3]);
      break;
    }
    case TIM_DMA_CC4:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC4]);
      break;
    }
    case TIM_DMA_COM:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_COMMUTATION]);
      break;
    }
    case TIM_DMA_TRIGGER:
    {
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_TRIGGER]);
      break;
    }
    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 禁用 TIM 更新 DMA 请求 */
    __HAL_TIM_DISABLE_DMA(htim, BurstRequestSrc);

    /* 改变 DMA 突发操作状态 */
    htim->DMABurstState = HAL_DMA_BURST_STATE_READY;
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  生成软件事件
  * @param  htim TIM 句柄
  * @param  EventSource 指定事件源。
  *          该参数可以是以下值之一：
  *            @arg TIM_EVENTSOURCE_UPDATE: 定时器更新事件源
  *            @arg TIM_EVENTSOURCE_CC1: 定时器捕获/比较 1 事件源
  *            @arg TIM_EVENTSOURCE_CC2: 定时器捕获/比较 2 事件源
  *            @arg TIM_EVENTSOURCE_CC3: 定时器捕获/比较 3 事件源
  *            @arg TIM_EVENTSOURCE_CC4: 定时器捕获/比较 4 事件源
  *            @arg TIM_EVENTSOURCE_COM: 定时器 COM 事件源
  *            @arg TIM_EVENTSOURCE_TRIGGER: 定时器触发事件源
  *            @arg TIM_EVENTSOURCE_BREAK: 定时器刹车事件源
  * @note   基本定时器只能生成更新事件。
  * @note   TIM_EVENTSOURCE_COM 仅适用于高级定时器实例。
  * @note   TIM_EVENTSOURCE_BREAK 仅适用于支持刹车输入的定时器实例。
  * @retval HAL 状态
  */

HAL_StatusTypeDef HAL_TIM_GenerateEvent(TIM_HandleTypeDef *htim, uint32_t EventSource)
{
  /* 检查参数 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_EVENT_SOURCE(EventSource));

  /* 进程加锁 */
  __HAL_LOCK(htim);

  /* 改变 TIM 状态 */
  htim->State = HAL_TIM_STATE_BUSY;

  /* 设置事件源 */
  htim->Instance->EGR = EventSource;

  /* 改变 TIM 状态 */
  htim->State = HAL_TIM_STATE_READY;

  __HAL_UNLOCK(htim);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  配置 OCRef 清除功能
  * @param  htim TIM 句柄
  * @param  sClearInputConfig 指向包含 TIM 外设 OCREF 清除功能及参数的
  *         TIM_ClearInputConfigTypeDef 结构体的指针。
  * @param  Channel 指定 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: TIM 通道 1
  *            @arg TIM_CHANNEL_2: TIM 通道 2
  *            @arg TIM_CHANNEL_3: TIM 通道 3
  *            @arg TIM_CHANNEL_4: TIM 通道 4
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_ConfigOCrefClear(TIM_HandleTypeDef *htim,
                                           const TIM_ClearInputConfigTypeDef *sClearInputConfig,
                                           uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_TIM_OCXREF_CLEAR_INSTANCE(htim->Instance));
  assert_param(IS_TIM_CLEARINPUT_SOURCE(sClearInputConfig->ClearInputSource));

  /* 进程锁定 */
  __HAL_LOCK(htim);

  htim->State = HAL_TIM_STATE_BUSY;

  switch (sClearInputConfig->ClearInputSource)
  {
    case TIM_CLEARINPUTSOURCE_NONE:
    {
      /* 清除 OCREF 清除选择位以及 ETR 位 */
      CLEAR_BIT(htim->Instance->SMCR, (TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP));
      break;
    }

    case TIM_CLEARINPUTSOURCE_ETR:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CLEARINPUT_POLARITY(sClearInputConfig->ClearInputPolarity));
      assert_param(IS_TIM_CLEARINPUT_PRESCALER(sClearInputConfig->ClearInputPrescaler));
      assert_param(IS_TIM_CLEARINPUT_FILTER(sClearInputConfig->ClearInputFilter));

      /* 当 OCREF 清除功能与 ETR 源一起使用时，必须关闭 ETR 预分频器 */
      if (sClearInputConfig->ClearInputPrescaler != TIM_CLEARINPUTPRESCALER_DIV1)
      {
        htim->State = HAL_TIM_STATE_READY;
        __HAL_UNLOCK(htim);
        return HAL_ERROR;
      }

      TIM_ETR_SetConfig(htim->Instance,
                        sClearInputConfig->ClearInputPrescaler,
                        sClearInputConfig->ClearInputPolarity,
                        sClearInputConfig->ClearInputFilter);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    switch (Channel)
    {
      case TIM_CHANNEL_1:
      {
        if (sClearInputConfig->ClearInputState != (uint32_t)DISABLE)
        {
          /* 使能通道 1 的 OCREF 清除功能 */
          SET_BIT(htim->Instance->CCMR1, TIM_CCMR1_OC1CE);
        }
        else
        {
          /* 禁用通道 1 的 OCREF 清除功能 */
          CLEAR_BIT(htim->Instance->CCMR1, TIM_CCMR1_OC1CE);
        }
        break;
      }
      case TIM_CHANNEL_2:
      {
        if (sClearInputConfig->ClearInputState != (uint32_t)DISABLE)
        {
          /* 使能通道 2 的 OCREF 清除功能 */
          SET_BIT(htim->Instance->CCMR1, TIM_CCMR1_OC2CE);
        }
        else
        {
          /* 禁用通道 2 的 OCREF 清除功能 */
          CLEAR_BIT(htim->Instance->CCMR1, TIM_CCMR1_OC2CE);
        }
        break;
      }
      case TIM_CHANNEL_3:
      {
        if (sClearInputConfig->ClearInputState != (uint32_t)DISABLE)
        {
          /* 使能通道 3 的 OCREF 清除功能 */
          SET_BIT(htim->Instance->CCMR2, TIM_CCMR2_OC3CE);
        }
        else
        {
          /* 禁用通道 3 的 OCREF 清除功能 */
          CLEAR_BIT(htim->Instance->CCMR2, TIM_CCMR2_OC3CE);
        }
        break;
      }
      case TIM_CHANNEL_4:
      {
        if (sClearInputConfig->ClearInputState != (uint32_t)DISABLE)
        {
          /* 使能通道 4 的 OCREF 清除功能 */
          SET_BIT(htim->Instance->CCMR2, TIM_CCMR2_OC4CE);
        }
        else
        {
          /* 禁用通道 4 的 OCREF 清除功能 */
          CLEAR_BIT(htim->Instance->CCMR2, TIM_CCMR2_OC4CE);
        }
        break;
      }
      default:
        break;
    }
  }

  htim->State = HAL_TIM_STATE_READY;

  __HAL_UNLOCK(htim);

  return status;
}

/**
  * @brief   配置所使用的时钟源
  * @param  htim TIM 句柄
  * @param  sClockSourceConfig 指向包含 TIM 外设时钟源信息的
  *         TIM_ClockConfigTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_ConfigClockSource(TIM_HandleTypeDef *htim, const TIM_ClockConfigTypeDef *sClockSourceConfig)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;

  /* 进程锁定 */
  __HAL_LOCK(htim);

  htim->State = HAL_TIM_STATE_BUSY;

  /* 检查参数 */
  assert_param(IS_TIM_CLOCKSOURCE(sClockSourceConfig->ClockSource));

  /* 复位 SMS、TS、ECE、ETPS 和 ETRF 位 */
  tmpsmcr = htim->Instance->SMCR;
  tmpsmcr &= ~(TIM_SMCR_SMS | TIM_SMCR_TS);
  tmpsmcr &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP);
  htim->Instance->SMCR = tmpsmcr;

  switch (sClockSourceConfig->ClockSource)
  {
    case TIM_CLOCKSOURCE_INTERNAL:
    {
      assert_param(IS_TIM_INSTANCE(htim->Instance));
      break;
    }

    case TIM_CLOCKSOURCE_ETRMODE1:
    {
      /* 检查该定时器实例是否支持外部触发输入模式 1 (ETRF) */
      assert_param(IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE(htim->Instance));

      /* 检查 ETR 输入信号调理相关参数 */
      assert_param(IS_TIM_CLOCKPRESCALER(sClockSourceConfig->ClockPrescaler));
      assert_param(IS_TIM_CLOCKPOLARITY(sClockSourceConfig->ClockPolarity));
      assert_param(IS_TIM_CLOCKFILTER(sClockSourceConfig->ClockFilter));

      /* 配置 ETR 时钟源 */
      TIM_ETR_SetConfig(htim->Instance,
                        sClockSourceConfig->ClockPrescaler,
                        sClockSourceConfig->ClockPolarity,
                        sClockSourceConfig->ClockFilter);

      /* 选择外部时钟模式 1 和 ETRF 触发源 */
      tmpsmcr = htim->Instance->SMCR;
      tmpsmcr |= (TIM_SLAVEMODE_EXTERNAL1 | TIM_CLOCKSOURCE_ETRMODE1);
      /* 写入 TIMx SMCR 寄存器 */
      htim->Instance->SMCR = tmpsmcr;
      break;
    }

    case TIM_CLOCKSOURCE_ETRMODE2:
    {
      /* 检查该定时器实例是否支持外部触发输入模式 2 (ETRF) */
      assert_param(IS_TIM_CLOCKSOURCE_ETRMODE2_INSTANCE(htim->Instance));

      /* 检查 ETR 输入信号调理相关参数 */
      assert_param(IS_TIM_CLOCKPRESCALER(sClockSourceConfig->ClockPrescaler));
      assert_param(IS_TIM_CLOCKPOLARITY(sClockSourceConfig->ClockPolarity));
      assert_param(IS_TIM_CLOCKFILTER(sClockSourceConfig->ClockFilter));

      /* 配置 ETR 时钟源 */
      TIM_ETR_SetConfig(htim->Instance,
                        sClockSourceConfig->ClockPrescaler,
                        sClockSourceConfig->ClockPolarity,
                        sClockSourceConfig->ClockFilter);
      /* 使能外部时钟模式 2 */
      htim->Instance->SMCR |= TIM_SMCR_ECE;
      break;
    }

    case TIM_CLOCKSOURCE_TI1:
    {
      /* 检查该定时器实例是否支持外部时钟模式 1 */
      assert_param(IS_TIM_CLOCKSOURCE_TIX_INSTANCE(htim->Instance));

      /* 检查 TI1 输入信号调理相关参数 */
      assert_param(IS_TIM_CLOCKPOLARITY(sClockSourceConfig->ClockPolarity));
      assert_param(IS_TIM_CLOCKFILTER(sClockSourceConfig->ClockFilter));

      TIM_TI1_ConfigInputStage(htim->Instance,
                               sClockSourceConfig->ClockPolarity,
                               sClockSourceConfig->ClockFilter);
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_TI1);
      break;
    }

    case TIM_CLOCKSOURCE_TI2:
    {
      /* 检查该定时器实例是否支持外部时钟模式 1 (ETRF) */
      assert_param(IS_TIM_CLOCKSOURCE_TIX_INSTANCE(htim->Instance));

      /* 检查 TI2 输入信号调理相关参数 */
      assert_param(IS_TIM_CLOCKPOLARITY(sClockSourceConfig->ClockPolarity));
      assert_param(IS_TIM_CLOCKFILTER(sClockSourceConfig->ClockFilter));

      TIM_TI2_ConfigInputStage(htim->Instance,
                               sClockSourceConfig->ClockPolarity,
                               sClockSourceConfig->ClockFilter);
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_TI2);
      break;
    }

    case TIM_CLOCKSOURCE_TI1ED:
    {
      /* 检查定时器实例是否支持外部时钟模式 1 */
      assert_param(IS_TIM_CLOCKSOURCE_TIX_INSTANCE(htim->Instance));

      /* 检查 TI1 输入信号调理相关参数 */
      assert_param(IS_TIM_CLOCKPOLARITY(sClockSourceConfig->ClockPolarity));
      assert_param(IS_TIM_CLOCKFILTER(sClockSourceConfig->ClockFilter));

      TIM_TI1_ConfigInputStage(htim->Instance,
                               sClockSourceConfig->ClockPolarity,
                               sClockSourceConfig->ClockFilter);
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_TI1ED);
      break;
    }

    case TIM_CLOCKSOURCE_ITR0:
    case TIM_CLOCKSOURCE_ITR1:
    case TIM_CLOCKSOURCE_ITR2:
    case TIM_CLOCKSOURCE_ITR3:
    {
      /* 检查定时器实例是否支持内部触发输入 */
      assert_param(IS_TIM_CLOCKSOURCE_ITRX_INSTANCE(htim->Instance));

      TIM_ITRx_SetConfig(htim->Instance, sClockSourceConfig->ClockSource);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }
  htim->State = HAL_TIM_STATE_READY;

  __HAL_UNLOCK(htim);

  return status;
}

/**
  * @brief  选择连接到 TI1 输入的信号：直接来自 CH1_input，
  *         或者来自 CH1_input、CH2_input 与 CH3_input 的异或（XOR）组合
  * @param  htim TIM 句柄。
  * @param  TI1_Selection 指示通道 1 是否连接到异或门的输出端。
  *          该参数可以是以下值之一：
  *            @arg TIM_TI1SELECTION_CH1: TIMx_CH1 引脚连接到 TI1 输入
  *            @arg TIM_TI1SELECTION_XORCOMBINATION: TIMx_CH1、CH2 和 CH3
  *            引脚连接到 TI1 输入（异或组合）
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_ConfigTI1Input(TIM_HandleTypeDef *htim, uint32_t TI1_Selection)
{
  uint32_t tmpcr2;

  /* 检查参数 */
  assert_param(IS_TIM_XOR_INSTANCE(htim->Instance));
  assert_param(IS_TIM_TI1SELECTION(TI1_Selection));

  /* 获取 TIMx CR2 寄存器的值 */
  tmpcr2 = htim->Instance->CR2;

  /* 复位 TI1 选择配置 */
  tmpcr2 &= ~TIM_CR2_TI1S;

  /* 设置 TI1 选择配置 */
  tmpcr2 |= TI1_Selection;

  /* 写入 TIMx CR2 寄存器 */
  htim->Instance->CR2 = tmpcr2;

  return HAL_OK;
}

/**
  * @brief  配置定时器为从模式
  * @param  htim TIM 句柄。
  * @param  sSlaveConfig 指向 TIM_SlaveConfigTypeDef 结构的指针，该结构
  *         包含所选的触发源（内部触发输入、滤波后的
  *         定时器输入或外部触发输入）以及从模式
  *         （禁用、复位、门控、触发、外部时钟模式 1）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_SlaveConfigSynchro(TIM_HandleTypeDef *htim, const TIM_SlaveConfigTypeDef *sSlaveConfig)
{
  /* 检查参数 */
  assert_param(IS_TIM_SLAVE_INSTANCE(htim->Instance));
  assert_param(IS_TIM_SLAVE_MODE(sSlaveConfig->SlaveMode));
  assert_param(IS_TIM_TRIGGER_SELECTION(sSlaveConfig->InputTrigger));

  __HAL_LOCK(htim);

  htim->State = HAL_TIM_STATE_BUSY;

  if (TIM_SlaveTimer_SetConfig(htim, sSlaveConfig) != HAL_OK)
  {
    htim->State = HAL_TIM_STATE_READY;
    __HAL_UNLOCK(htim);
    return HAL_ERROR;
  }

  /* 禁用触发中断 */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_TRIGGER);

  /* 禁用触发 DMA 请求 */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_TRIGGER);

  htim->State = HAL_TIM_STATE_READY;

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  在中断模式下配置定时器为从模式
  * @param  htim TIM 句柄。
  * @param  sSlaveConfig 指向 TIM_SlaveConfigTypeDef 结构的指针，该结构
  *         包含所选的触发源（内部触发输入、滤波后的
  *         定时器输入或外部触发输入）以及从模式
  *         （禁用、复位、门控、触发、外部时钟模式 1）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIM_SlaveConfigSynchro_IT(TIM_HandleTypeDef *htim,
                                                const TIM_SlaveConfigTypeDef *sSlaveConfig)
{
  /* 检查参数 */
  assert_param(IS_TIM_SLAVE_INSTANCE(htim->Instance));
  assert_param(IS_TIM_SLAVE_MODE(sSlaveConfig->SlaveMode));
  assert_param(IS_TIM_TRIGGER_SELECTION(sSlaveConfig->InputTrigger));

  __HAL_LOCK(htim);

  htim->State = HAL_TIM_STATE_BUSY;

  if (TIM_SlaveTimer_SetConfig(htim, sSlaveConfig) != HAL_OK)
  {
    htim->State = HAL_TIM_STATE_READY;
    __HAL_UNLOCK(htim);
    return HAL_ERROR;
  }

  /* 使能触发中断 */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_TRIGGER);

  /* 禁用触发 DMA 请求 */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_TRIGGER);

  htim->State = HAL_TIM_STATE_READY;

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  从捕获/比较单元读取捕获值
  * @param  htim TIM 句柄。
  * @param  Channel 待使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选择 TIM 通道 4
  * @retval 捕获值
  */
uint32_t HAL_TIM_ReadCapturedValue(const TIM_HandleTypeDef *htim, uint32_t Channel)
{
  uint32_t tmpreg = 0U;

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));

      /* 返回捕获通道 1 的值 */
      tmpreg =  htim->Instance->CCR1;

      break;
    }
    case TIM_CHANNEL_2:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

      /* 返回捕获通道 2 的值 */
      tmpreg =   htim->Instance->CCR2;

      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));

      /* 返回捕获通道 3 的值 */
      tmpreg =   htim->Instance->CCR3;

      break;
    }

    case TIM_CHANNEL_4:
    {
      /* 检查参数 */
      assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));

      /* 返回捕获通道 4 的值 */
      tmpreg =   htim->Instance->CCR4;

      break;
    }

    default:
      break;
  }

  return tmpreg;
}

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group9 TIM Callbacks functions
  *  @brief    TIM 回调函数
  *
@verbatim
  ==============================================================================
                        ##### TIM 回调函数 #####
  ==============================================================================
 [..]
   本节提供 TIM 回调函数：
   (+) TIM 周期溢出更新回调函数
   (+) TIM 输出比较回调函数
   (+) TIM 输入捕获回调函数
   (+) TIM 触发回调函数
   (+) TIM 错误回调函数

@endverbatim
  * @{
  */

/**
  * @brief  非阻塞模式下的周期溢出更新回调函数
  * @param  htim TIM 句柄
  * @retval None
  */
__weak void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_TIM_PeriodElapsedCallback
   */
}

/**
  * @brief  非阻塞模式下的周期溢出过半（半完成）回调函数
  * @param  htim TIM 句柄
  * @retval 无
  */
__weak void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_TIM_PeriodElapsedHalfCpltCallback
   */
}

/**
  * @brief  非阻塞模式下的输出比较回调函数
  * @param  htim TIM OC 句柄
  * @retval 无
  */
__weak void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_TIM_OC_DelayElapsedCallback
   */
}

/**
  * @brief  非阻塞模式下的输入捕获回调函数
  * @param  htim TIM IC 句柄
  * @retval 无
  */
__weak void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_TIM_IC_CaptureCallback
   */
}

/**
  * @brief  非阻塞模式下的输入捕获半完成回调函数
  * @param  htim TIM IC 句柄
  * @retval 无
  */
__weak void HAL_TIM_IC_CaptureHalfCpltCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_TIM_IC_CaptureHalfCpltCallback
   */
}

/**
  * @brief  非阻塞模式下的 PWM 脉冲完成回调函数
  * @param  htim TIM 句柄
  * @retval 无
  */
__weak void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：请勿修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_TIM_PWM_PulseFinishedCallback
   */
}

/**
  * @brief  非阻塞模式下的 PWM 脉冲半完成回调函数
  * @param  htim TIM 句柄
  * @retval 无
  */
__weak void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：请勿修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_TIM_PWM_PulseFinishedHalfCpltCallback
   */
}

/**
  * @brief  非阻塞模式下的霍尔触发检测回调函数
  * @param  htim TIM 句柄
  * @retval 无
  */
__weak void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：请勿修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_TIM_TriggerCallback
   */
}

/**
  * @brief  非阻塞模式下的霍尔触发检测半完成回调函数
  * @param  htim TIM 句柄
  * @retval 无
  */
__weak void HAL_TIM_TriggerHalfCpltCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：请勿修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_TIM_TriggerHalfCpltCallback
   */
}

/**
  * @brief  非阻塞模式下的定时器错误回调函数
  * @param  htim TIM 句柄
  * @retval 无
  */
__weak void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：请勿修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_TIM_ErrorCallback
   */
}

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
/**
  * @brief  注册用户 TIM 回调函数以替代预定义的 weak 回调函数
  * @param htim tim 句柄
  * @param CallbackID 要注册的回调函数 ID
  *        该参数可以是以下值之一：
  *          @arg @ref HAL_TIM_BASE_MSPINIT_CB_ID 基本定时器 MspInit 回调函数 ID
  *          @arg @ref HAL_TIM_BASE_MSPDEINIT_CB_ID 基本定时器 MspDeInit 回调函数 ID
  *          @arg @ref HAL_TIM_IC_MSPINIT_CB_ID 输入捕获 MspInit 回调函数 ID
  *          @arg @ref HAL_TIM_IC_MSPDEINIT_CB_ID 输入捕获 MspDeInit 回调函数 ID
  *          @arg @ref HAL_TIM_OC_MSPINIT_CB_ID 输出比较 MspInit 回调函数 ID
  *          @arg @ref HAL_TIM_OC_MSPDEINIT_CB_ID 输出比较 MspDeInit 回调函数 ID
  *          @arg @ref HAL_TIM_PWM_MSPINIT_CB_ID PWM MspInit 回调函数 ID
  *          @arg @ref HAL_TIM_PWM_MSPDEINIT_CB_ID PWM MspDeInit 回调函数 ID
  *          @arg @ref HAL_TIM_ONE_PULSE_MSPINIT_CB_ID 单脉冲 MspInit 回调函数 ID
  *          @arg @ref HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID 单脉冲 MspDeInit 回调函数 ID
  *          @arg @ref HAL_TIM_ENCODER_MSPINIT_CB_ID 编码器 MspInit 回调函数 ID
  *          @arg @ref HAL_TIM_ENCODER_MSPDEINIT_CB_ID 编码器 MspDeInit 回调函数 ID
  *          @arg @ref HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID 霍尔传感器 MspInit 回调函数 ID
  *          @arg @ref HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID 霍尔传感器 MspDeInit 回调函数 ID
  *          @arg @ref HAL_TIM_PERIOD_ELAPSED_CB_ID 周期溢出回调函数 ID
  *          @arg @ref HAL_TIM_PERIOD_ELAPSED_HALF_CB_ID 周期溢出半完成回调函数 ID
  *          @arg @ref HAL_TIM_TRIGGER_CB_ID 触发回调函数 ID
  *          @arg @ref HAL_TIM_TRIGGER_HALF_CB_ID 触发半完成回调函数 ID
  *          @arg @ref HAL_TIM_IC_CAPTURE_CB_ID 输入捕获回调函数 ID
  *          @arg @ref HAL_TIM_IC_CAPTURE_HALF_CB_ID 输入捕获半完成回调函数 ID
  *          @arg @ref HAL_TIM_OC_DELAY_ELAPSED_CB_ID 输出比较延时溢出回调函数 ID
  *          @arg @ref HAL_TIM_PWM_PULSE_FINISHED_CB_ID PWM 脉冲完成回调函数 ID
  *          @arg @ref HAL_TIM_PWM_PULSE_FINISHED_HALF_CB_ID PWM 脉冲半完成回调函数 ID
  *          @arg @ref HAL_TIM_ERROR_CB_ID 错误回调函数 ID
  *          @arg @ref HAL_TIM_COMMUTATION_CB_ID 换相回调函数 ID
  *          @arg @ref HAL_TIM_COMMUTATION_HALF_CB_ID 换相半完成回调函数 ID
  *          @arg @ref HAL_TIM_BREAK_CB_ID 刹车回调函数 ID
  *          @param pCallback 指向回调函数的指针
  *          @retval 状态
  */
HAL_StatusTypeDef HAL_TIM_RegisterCallback(TIM_HandleTypeDef *htim, HAL_TIM_CallbackIDTypeDef CallbackID,
                                           pTIM_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    return HAL_ERROR;
  }

  if (htim->State == HAL_TIM_STATE_READY)
  {
    switch (CallbackID)
    {
      case HAL_TIM_BASE_MSPINIT_CB_ID :
        htim->Base_MspInitCallback                 = pCallback;
        break;

      case HAL_TIM_BASE_MSPDEINIT_CB_ID :
        htim->Base_MspDeInitCallback               = pCallback;
        break;

      case HAL_TIM_IC_MSPINIT_CB_ID :
        htim->IC_MspInitCallback                   = pCallback;
        break;

      case HAL_TIM_IC_MSPDEINIT_CB_ID :
        htim->IC_MspDeInitCallback                 = pCallback;
        break;

      case HAL_TIM_OC_MSPINIT_CB_ID :
        htim->OC_MspInitCallback                   = pCallback;
        break;

      case HAL_TIM_OC_MSPDEINIT_CB_ID :
        htim->OC_MspDeInitCallback                 = pCallback;
        break;

      case HAL_TIM_PWM_MSPINIT_CB_ID :
        htim->PWM_MspInitCallback                  = pCallback;
        break;

      case HAL_TIM_PWM_MSPDEINIT_CB_ID :
        htim->PWM_MspDeInitCallback                = pCallback;
        break;

      case HAL_TIM_ONE_PULSE_MSPINIT_CB_ID :
        htim->OnePulse_MspInitCallback             = pCallback;
        break;

      case HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID :
        htim->OnePulse_MspDeInitCallback           = pCallback;
        break;

      case HAL_TIM_ENCODER_MSPINIT_CB_ID :
        htim->Encoder_MspInitCallback              = pCallback;
        break;

      case HAL_TIM_ENCODER_MSPDEINIT_CB_ID :
        htim->Encoder_MspDeInitCallback            = pCallback;
        break;

      case HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID :
        htim->HallSensor_MspInitCallback           = pCallback;
        break;

      case HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID :
        htim->HallSensor_MspDeInitCallback         = pCallback;
        break;

      case HAL_TIM_PERIOD_ELAPSED_CB_ID :
        htim->PeriodElapsedCallback                = pCallback;
        break;

      case HAL_TIM_PERIOD_ELAPSED_HALF_CB_ID :
        htim->PeriodElapsedHalfCpltCallback        = pCallback;
        break;

      case HAL_TIM_TRIGGER_CB_ID :
        htim->TriggerCallback                      = pCallback;
        break;

      case HAL_TIM_TRIGGER_HALF_CB_ID :
        htim->TriggerHalfCpltCallback              = pCallback;
        break;

      case HAL_TIM_IC_CAPTURE_CB_ID :
        htim->IC_CaptureCallback                   = pCallback;
        break;

      case HAL_TIM_IC_CAPTURE_HALF_CB_ID :
        htim->IC_CaptureHalfCpltCallback           = pCallback;
        break;

      case HAL_TIM_OC_DELAY_ELAPSED_CB_ID :
        htim->OC_DelayElapsedCallback              = pCallback;
        break;

      case HAL_TIM_PWM_PULSE_FINISHED_CB_ID :
        htim->PWM_PulseFinishedCallback            = pCallback;
        break;

      case HAL_TIM_PWM_PULSE_FINISHED_HALF_CB_ID :
        htim->PWM_PulseFinishedHalfCpltCallback    = pCallback;
        break;

      case HAL_TIM_ERROR_CB_ID :
        htim->ErrorCallback                        = pCallback;
        break;

      case HAL_TIM_COMMUTATION_CB_ID :
        htim->CommutationCallback                  = pCallback;
        break;

      case HAL_TIM_COMMUTATION_HALF_CB_ID :
        htim->CommutationHalfCpltCallback          = pCallback;
        break;

      case HAL_TIM_BREAK_CB_ID :
        htim->BreakCallback                        = pCallback;
        break;

      default :
        /* 返回错误状态 */
        status = HAL_ERROR;
        break;
    }
  }
  else if (htim->State == HAL_TIM_STATE_RESET)
  {
    switch (CallbackID)
    {
      case HAL_TIM_BASE_MSPINIT_CB_ID :
        htim->Base_MspInitCallback         = pCallback;
        break;

      case HAL_TIM_BASE_MSPDEINIT_CB_ID :
        htim->Base_MspDeInitCallback       = pCallback;
        break;

      case HAL_TIM_IC_MSPINIT_CB_ID :
        htim->IC_MspInitCallback           = pCallback;
        break;

      case HAL_TIM_IC_MSPDEINIT_CB_ID :
        htim->IC_MspDeInitCallback         = pCallback;
        break;

      case HAL_TIM_OC_MSPINIT_CB_ID :
        htim->OC_MspInitCallback           = pCallback;
        break;

      case HAL_TIM_OC_MSPDEINIT_CB_ID :
        htim->OC_MspDeInitCallback         = pCallback;
        break;

      case HAL_TIM_PWM_MSPINIT_CB_ID :
        htim->PWM_MspInitCallback          = pCallback;
        break;

      case HAL_TIM_PWM_MSPDEINIT_CB_ID :
        htim->PWM_MspDeInitCallback        = pCallback;
        break;

      case HAL_TIM_ONE_PULSE_MSPINIT_CB_ID :
        htim->OnePulse_MspInitCallback     = pCallback;
        break;

      case HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID :
        htim->OnePulse_MspDeInitCallback   = pCallback;
        break;

      case HAL_TIM_ENCODER_MSPINIT_CB_ID :
        htim->Encoder_MspInitCallback      = pCallback;
        break;

      case HAL_TIM_ENCODER_MSPDEINIT_CB_ID :
        htim->Encoder_MspDeInitCallback    = pCallback;
        break;

      case HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID :
        htim->HallSensor_MspInitCallback   = pCallback;
        break;

      case HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID :
        htim->HallSensor_MspDeInitCallback = pCallback;
        break;

      default :
        /* 返回错误状态 */
        status = HAL_ERROR;
        break;
    }
  }
  else
  {
    /* 返回错误状态 */
    status = HAL_ERROR;
  }

  return status;
}

/**
  * @brief  注销 TIM 回调函数
  *         TIM 回调函数将被重定向到预定义的弱函数（weak）回调
  * @param htim tim 句柄
  * @param CallbackID 要注销的回调函数 ID
  *        该参数可以是以下值之一：
  *          @arg @ref HAL_TIM_BASE_MSPINIT_CB_ID 基本定时器 MspInit 回调 ID
  *          @arg @ref HAL_TIM_BASE_MSPDEINIT_CB_ID 基本定时器 MspDeInit 回调 ID
  *          @arg @ref HAL_TIM_IC_MSPINIT_CB_ID 输入捕获 MspInit 回调 ID
  *          @arg @ref HAL_TIM_IC_MSPDEINIT_CB_ID 输入捕获 MspDeInit 回调 ID
  *          @arg @ref HAL_TIM_OC_MSPINIT_CB_ID 输出比较 MspInit 回调 ID
  *          @arg @ref HAL_TIM_OC_MSPDEINIT_CB_ID 输出比较 MspDeInit 回调 ID
  *          @arg @ref HAL_TIM_PWM_MSPINIT_CB_ID PWM MspInit 回调 ID
  *          @arg @ref HAL_TIM_PWM_MSPDEINIT_CB_ID PWM MspDeInit 回调 ID
  *          @arg @ref HAL_TIM_ONE_PULSE_MSPINIT_CB_ID 单脉冲 MspInit 回调 ID
  *          @arg @ref HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID 单脉冲 MspDeInit 回调 ID
  *          @arg @ref HAL_TIM_ENCODER_MSPINIT_CB_ID 编码器 MspInit 回调 ID
  *          @arg @ref HAL_TIM_ENCODER_MSPDEINIT_CB_ID 编码器 MspDeInit 回调 ID
  *          @arg @ref HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID 霍尔传感器 MspInit 回调 ID
  *          @arg @ref HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID 霍尔传感器 MspDeInit 回调 ID
  *          @arg @ref HAL_TIM_PERIOD_ELAPSED_CB_ID 周期更新完成回调 ID
  *          @arg @ref HAL_TIM_PERIOD_ELAPSED_HALF_CB_ID 周期更新半完成回调 ID
  *          @arg @ref HAL_TIM_TRIGGER_CB_ID 触发回调 ID
  *          @arg @ref HAL_TIM_TRIGGER_HALF_CB_ID 触发半完成回调 ID
  *          @arg @ref HAL_TIM_IC_CAPTURE_CB_ID 输入捕获回调 ID
  *          @arg @ref HAL_TIM_IC_CAPTURE_HALF_CB_ID 输入捕获半完成回调 ID
  *          @arg @ref HAL_TIM_OC_DELAY_ELAPSED_CB_ID 输出比较延时到期回调 ID
  *          @arg @ref HAL_TIM_PWM_PULSE_FINISHED_CB_ID PWM 脉冲完成回调 ID
  *          @arg @ref HAL_TIM_PWM_PULSE_FINISHED_HALF_CB_ID PWM 脉冲半完成回调 ID
  *          @arg @ref HAL_TIM_ERROR_CB_ID 错误回调 ID
  *          @arg @ref HAL_TIM_COMMUTATION_CB_ID 换相回调 ID
  *          @arg @ref HAL_TIM_COMMUTATION_HALF_CB_ID 换相半完成回调 ID
  *          @arg @ref HAL_TIM_BREAK_CB_ID 刹车回调 ID
  * @retval 状态
  */
HAL_StatusTypeDef HAL_TIM_UnRegisterCallback(TIM_HandleTypeDef *htim, HAL_TIM_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (htim->State == HAL_TIM_STATE_READY)
  {
    switch (CallbackID)
    {
      case HAL_TIM_BASE_MSPINIT_CB_ID :
        /* 传统弱定义 Base MspInit 回调函数 */
        htim->Base_MspInitCallback              = HAL_TIM_Base_MspInit;
        break;

      case HAL_TIM_BASE_MSPDEINIT_CB_ID :
        /* 传统弱定义 Base MspDeInit 回调函数 */
        htim->Base_MspDeInitCallback            = HAL_TIM_Base_MspDeInit;
        break;

      case HAL_TIM_IC_MSPINIT_CB_ID :
        /* 传统弱定义 IC MspInit 回调函数 */
        htim->IC_MspInitCallback                = HAL_TIM_IC_MspInit;
        break;

      case HAL_TIM_IC_MSPDEINIT_CB_ID :
        /* 传统弱定义 IC MspDeInit 回调函数 */
        htim->IC_MspDeInitCallback              = HAL_TIM_IC_MspDeInit;
        break;

      case HAL_TIM_OC_MSPINIT_CB_ID :
        /* 传统弱定义 OC MspInit 回调函数 */
        htim->OC_MspInitCallback                = HAL_TIM_OC_MspInit;
        break;

      case HAL_TIM_OC_MSPDEINIT_CB_ID :
        /* 传统弱定义 OC MspDeInit 回调函数 */
        htim->OC_MspDeInitCallback              = HAL_TIM_OC_MspDeInit;
        break;

      case HAL_TIM_PWM_MSPINIT_CB_ID :
        /* 传统弱定义 PWM MspInit 回调函数 */
        htim->PWM_MspInitCallback               = HAL_TIM_PWM_MspInit;
        break;

      case HAL_TIM_PWM_MSPDEINIT_CB_ID :
        /* 传统弱定义 PWM MspDeInit 回调函数 */
        htim->PWM_MspDeInitCallback             = HAL_TIM_PWM_MspDeInit;
        break;

      case HAL_TIM_ONE_PULSE_MSPINIT_CB_ID :
        /* 传统弱定义 One Pulse MspInit 回调函数 */
        htim->OnePulse_MspInitCallback          = HAL_TIM_OnePulse_MspInit;
        break;

      case HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID :
        /* 传统弱定义 One Pulse MspDeInit 回调函数 */
        htim->OnePulse_MspDeInitCallback        = HAL_TIM_OnePulse_MspDeInit;
        break;

      case HAL_TIM_ENCODER_MSPINIT_CB_ID :
        /* 传统弱定义 Encoder MspInit 回调函数 */
        htim->Encoder_MspInitCallback           = HAL_TIM_Encoder_MspInit;
        break;

      case HAL_TIM_ENCODER_MSPDEINIT_CB_ID :
        /* 传统弱定义 Encoder MspDeInit 回调函数 */
        htim->Encoder_MspDeInitCallback         = HAL_TIM_Encoder_MspDeInit;
        break;

      case HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID :
        /* 传统的弱定义霍尔传感器 MSP 初始化回调函数 */
        htim->HallSensor_MspInitCallback        = HAL_TIMEx_HallSensor_MspInit;
        break;

      case HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID :
        /* 传统的弱定义霍尔传感器 MSP 反初始化回调函数 */
        htim->HallSensor_MspDeInitCallback      = HAL_TIMEx_HallSensor_MspDeInit;
        break;

      case HAL_TIM_PERIOD_ELAPSED_CB_ID :
        /* 传统的弱定义周期溢出更新回调函数 */
        htim->PeriodElapsedCallback             = HAL_TIM_PeriodElapsedCallback;
        break;

      case HAL_TIM_PERIOD_ELAPSED_HALF_CB_ID :
        /* 传统的弱定义周期过半完成回调函数 */
        htim->PeriodElapsedHalfCpltCallback     = HAL_TIM_PeriodElapsedHalfCpltCallback;
        break;

      case HAL_TIM_TRIGGER_CB_ID :
        /* 传统的弱定义触发回调函数 */
        htim->TriggerCallback                   = HAL_TIM_TriggerCallback;
        break;

      case HAL_TIM_TRIGGER_HALF_CB_ID :
        /* 传统的弱定义触发传输过半回调函数 */
        htim->TriggerHalfCpltCallback           = HAL_TIM_TriggerHalfCpltCallback;
        break;

      case HAL_TIM_IC_CAPTURE_CB_ID :
        /* 传统的弱定义输入捕获回调函数 */
        htim->IC_CaptureCallback                = HAL_TIM_IC_CaptureCallback;
        break;

      case HAL_TIM_IC_CAPTURE_HALF_CB_ID :
        /* 传统的弱定义输入捕获过半完成回调函数 */
        htim->IC_CaptureHalfCpltCallback        = HAL_TIM_IC_CaptureHalfCpltCallback;
        break;

      case HAL_TIM_OC_DELAY_ELAPSED_CB_ID :
        /* 传统的弱定义输出比较延时到达回调函数 */
        htim->OC_DelayElapsedCallback           = HAL_TIM_OC_DelayElapsedCallback;
        break;

      case HAL_TIM_PWM_PULSE_FINISHED_CB_ID :
        /* 传统的弱定义 PWM 脉冲完成回调函数 */
        htim->PWM_PulseFinishedCallback         = HAL_TIM_PWM_PulseFinishedCallback;
        break;

      case HAL_TIM_PWM_PULSE_FINISHED_HALF_CB_ID :
        /* 传统的弱定义 PWM 脉冲过半完成回调函数 */
        htim->PWM_PulseFinishedHalfCpltCallback = HAL_TIM_PWM_PulseFinishedHalfCpltCallback;
        break;

      case HAL_TIM_ERROR_CB_ID :
        /* 传统的弱定义错误回调函数 */
        htim->ErrorCallback                     = HAL_TIM_ErrorCallback;
        break;

      case HAL_TIM_COMMUTATION_CB_ID :
        /* 传统的弱定义换相回调函数 */
        htim->CommutationCallback               = HAL_TIMEx_CommutCallback;
        break;

      case HAL_TIM_COMMUTATION_HALF_CB_ID :
        /* 传统的弱定义换相传输过半回调函数 */
        htim->CommutationHalfCpltCallback       = HAL_TIMEx_CommutHalfCpltCallback;
        break;

      case HAL_TIM_BREAK_CB_ID :
        /* 传统的弱定义刹车回调函数 */
        htim->BreakCallback                     = HAL_TIMEx_BreakCallback;
        break;

      default :
        /* 返回错误状态 */
        status = HAL_ERROR;
        break;
    }
  }
  else if (htim->State == HAL_TIM_STATE_RESET)
  {
    switch (CallbackID)
    {
      case HAL_TIM_BASE_MSPINIT_CB_ID :
        /* 传统的弱定义基本定时器 MspInit 回调函数 */
        htim->Base_MspInitCallback         = HAL_TIM_Base_MspInit;
        break;

      case HAL_TIM_BASE_MSPDEINIT_CB_ID :
        /* 传统的弱定义基本定时器 Msp DeInit 回调函数 */
        htim->Base_MspDeInitCallback       = HAL_TIM_Base_MspDeInit;
        break;

      case HAL_TIM_IC_MSPINIT_CB_ID :
        /* 传统的弱定义输入捕获（IC）Msp Init 回调函数 */
        htim->IC_MspInitCallback           = HAL_TIM_IC_MspInit;
        break;

      case HAL_TIM_IC_MSPDEINIT_CB_ID :
        /* 传统的弱定义输入捕获（IC）Msp DeInit 回调函数 */
        htim->IC_MspDeInitCallback         = HAL_TIM_IC_MspDeInit;
        break;

      case HAL_TIM_OC_MSPINIT_CB_ID :
        /* 传统的弱定义输出比较（OC）Msp Init 回调函数 */
        htim->OC_MspInitCallback           = HAL_TIM_OC_MspInit;
        break;

      case HAL_TIM_OC_MSPDEINIT_CB_ID :
        /* 传统的弱定义输出比较（OC）Msp DeInit 回调函数 */
        htim->OC_MspDeInitCallback         = HAL_TIM_OC_MspDeInit;
        break;

      case HAL_TIM_PWM_MSPINIT_CB_ID :
        /* 传统的弱定义 PWM Msp Init 回调函数 */
        htim->PWM_MspInitCallback          = HAL_TIM_PWM_MspInit;
        break;

      case HAL_TIM_PWM_MSPDEINIT_CB_ID :
        /* 传统的弱定义 PWM Msp DeInit 回调函数 */
        htim->PWM_MspDeInitCallback        = HAL_TIM_PWM_MspDeInit;
        break;

      case HAL_TIM_ONE_PULSE_MSPINIT_CB_ID :
        /* 传统的弱定义单脉冲模式 Msp Init 回调函数 */
        htim->OnePulse_MspInitCallback     = HAL_TIM_OnePulse_MspInit;
        break;

      case HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID :
        /* 传统的弱定义单脉冲模式 Msp DeInit 回调函数 */
        htim->OnePulse_MspDeInitCallback   = HAL_TIM_OnePulse_MspDeInit;
        break;

      case HAL_TIM_ENCODER_MSPINIT_CB_ID :
        /* 传统的弱定义编码器模式 Msp Init 回调函数 */
        htim->Encoder_MspInitCallback      = HAL_TIM_Encoder_MspInit;
        break;

      case HAL_TIM_ENCODER_MSPDEINIT_CB_ID :
        /* 传统的弱定义编码器模式 Msp DeInit 回调函数 */
        htim->Encoder_MspDeInitCallback    = HAL_TIM_Encoder_MspDeInit;
        break;

      case HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID :
        /* 传统的弱定义霍尔传感器接口 Msp Init 回调函数 */
        htim->HallSensor_MspInitCallback   = HAL_TIMEx_HallSensor_MspInit;
        break;

      case HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID :
        /* 传统的弱定义霍尔传感器接口 Msp DeInit 回调函数 */
        htim->HallSensor_MspDeInitCallback = HAL_TIMEx_HallSensor_MspDeInit;
        break;

      default :
        /* 返回错误状态 */
        status = HAL_ERROR;
        break;
    }
  }
  else
  {
    /* 返回错误状态 */
    status = HAL_ERROR;
  }

  return status;
}
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group10 TIM Peripheral State functions
  *  @brief   TIM 外设状态函数
  *
@verbatim
  ==============================================================================
                        ##### 外设状态函数 #####
  ==============================================================================
    [..]
    本小节允许在运行时获取外设的状态以及数据流。

@endverbatim
  * @{
  */

/**
  * @brief  返回 TIM 基本定时器句柄状态。
  * @param  htim TIM 基本定时器句柄
  * @retval HAL 状态
  */
HAL_TIM_StateTypeDef HAL_TIM_Base_GetState(const TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @brief  返回 TIM 输出比较 (OC) 句柄状态。
  * @param  htim TIM 输出比较句柄
  * @retval HAL 状态
  */
HAL_TIM_StateTypeDef HAL_TIM_OC_GetState(const TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @brief  返回 TIM PWM 句柄状态。
  * @param  htim TIM 句柄
  * @retval HAL 状态
  */
HAL_TIM_StateTypeDef HAL_TIM_PWM_GetState(const TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @brief  返回 TIM 输入捕获 (IC) 句柄状态。
  * @param  htim TIM 输入捕获句柄
  * @retval HAL 状态
  */
HAL_TIM_StateTypeDef HAL_TIM_IC_GetState(const TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @brief  返回 TIM 单脉冲模式 (OPM) 句柄状态。
  * @param  htim TIM 单脉冲模式句柄
  * @retval HAL 状态
  */
HAL_TIM_StateTypeDef HAL_TIM_OnePulse_GetState(const TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @brief  返回 TIM 编码器模式句柄状态。
  * @param  htim TIM 编码器接口句柄
  * @retval HAL 状态
  */
HAL_TIM_StateTypeDef HAL_TIM_Encoder_GetState(const TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @brief  返回 TIM 编码器模式句柄状态。
  * @param  htim TIM 句柄
  * @retval 活动通道
  */
HAL_TIM_ActiveChannel HAL_TIM_GetActiveChannel(const TIM_HandleTypeDef *htim)
{
  return htim->Channel;
}

/**
  * @brief  返回 TIM 通道的实际状态。
  * @param  htim TIM 句柄
  * @param  Channel TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: TIM 通道 1
  *            @arg TIM_CHANNEL_2: TIM 通道 2
  *            @arg TIM_CHANNEL_3: TIM 通道 3
  *            @arg TIM_CHANNEL_4: TIM 通道 4
  *            @arg TIM_CHANNEL_5: TIM 通道 5
  *            @arg TIM_CHANNEL_6: TIM 通道 6
  * @retval TIM 通道状态
  */
HAL_TIM_ChannelStateTypeDef HAL_TIM_GetChannelState(const TIM_HandleTypeDef *htim,  uint32_t Channel)
{
  HAL_TIM_ChannelStateTypeDef channel_state;

  /* 检查参数 */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  channel_state = TIM_CHANNEL_STATE_GET(htim, Channel);

  return channel_state;
}

/**
  * @brief  返回 DMA 突发传输操作的实际状态。
  * @param  htim TIM 句柄
  * @retval DMA 突发传输状态
  */
HAL_TIM_DMABurstStateTypeDef HAL_TIM_DMABurstState(const TIM_HandleTypeDef *htim)
{
  /* 检查参数 */
  assert_param(IS_TIM_DMABURST_INSTANCE(htim->Instance));

  return htim->DMABurstState;
}

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup TIM_Private_Functions TIM 私有函数
  * @{
  */

/**
  * @brief  TIM DMA 错误回调函数
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
void TIM_DMAError(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_READY);
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;
    TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_4, HAL_TIM_CHANNEL_STATE_READY);
  }
  else
  {
    htim->State = HAL_TIM_STATE_READY;
  }

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  htim->ErrorCallback(htim);
#else
  HAL_TIM_ErrorCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
}

/**
  * @brief  TIM DMA 延迟脉冲完成回调函数。
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
static void TIM_DMADelayPulseCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;

    if (hdma->Init.Mode == DMA_NORMAL)
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
    }
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;

    if (hdma->Init.Mode == DMA_NORMAL)
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
    }
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;

    if (hdma->Init.Mode == DMA_NORMAL)
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_READY);
    }
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;

    if (hdma->Init.Mode == DMA_NORMAL)
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_4, HAL_TIM_CHANNEL_STATE_READY);
    }
  }
  else
  {
    /* 无需操作 */
  }

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  htim->PWM_PulseFinishedCallback(htim);
#else
  HAL_TIM_PWM_PulseFinishedCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
}

/**
  * @brief  TIM DMA 延迟脉冲半传输完成回调函数。
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
void TIM_DMADelayPulseHalfCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;
  }
  else
  {
    /* 无需操作 */
  }

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  htim->PWM_PulseFinishedHalfCpltCallback(htim);
#else
  HAL_TIM_PWM_PulseFinishedHalfCpltCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
}

/**
  * @brief  TIM DMA 捕获完成回调函数。
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
void TIM_DMACaptureCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;

    if (hdma->Init.Mode == DMA_NORMAL)
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
    }
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;

    if (hdma->Init.Mode == DMA_NORMAL)
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
    }
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;

    if (hdma->Init.Mode == DMA_NORMAL)
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_READY);
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_READY);
    }
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;

    if (hdma->Init.Mode == DMA_NORMAL)
    {
      TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_4, HAL_TIM_CHANNEL_STATE_READY);
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_4, HAL_TIM_CHANNEL_STATE_READY);
    }
  }
  else
  {
    /* 无需操作 */
  }

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  htim->IC_CaptureCallback(htim);
#else
  HAL_TIM_IC_CaptureCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
}

/**
  * @brief  TIM DMA 输入捕获过半完成回调函数。
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
void TIM_DMACaptureHalfCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;
  }
  else
  {
    /* 无操作 */
  }

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  htim->IC_CaptureHalfCpltCallback(htim);
#else
  HAL_TIM_IC_CaptureHalfCpltCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
}

/**
  * @brief  TIM DMA 周期流逝（更新）完成回调函数。
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
static void TIM_DMAPeriodElapsedCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  if (htim->hdma[TIM_DMA_ID_UPDATE]->Init.Mode == DMA_NORMAL)
  {
    htim->State = HAL_TIM_STATE_READY;
  }

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  htim->PeriodElapsedCallback(htim);
#else
  HAL_TIM_PeriodElapsedCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}

/**
  * @brief  TIM DMA 周期流逝（更新）过半完成回调函数。
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
static void TIM_DMAPeriodElapsedHalfCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  htim->PeriodElapsedHalfCpltCallback(htim);
#else
  HAL_TIM_PeriodElapsedHalfCpltCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}

/**
  * @brief  TIM DMA 触发回调函数。
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
static void TIM_DMATriggerCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  if (htim->hdma[TIM_DMA_ID_TRIGGER]->Init.Mode == DMA_NORMAL)
  {
    htim->State = HAL_TIM_STATE_READY;
  }

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  htim->TriggerCallback(htim);
#else
  HAL_TIM_TriggerCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}

/**
  * @brief  TIM DMA 触发过半完成回调函数。
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
static void TIM_DMATriggerHalfCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  htim->TriggerHalfCpltCallback(htim);
#else
  HAL_TIM_TriggerHalfCpltCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}

/**
  * @brief  时基配置
  * @param  TIMx TIM 外设
  * @param  Structure TIM 基本配置结构体
  * @retval 无
  */
void TIM_Base_SetConfig(TIM_TypeDef *TIMx, const TIM_Base_InitTypeDef *Structure)
{
  uint32_t tmpcr1;
  tmpcr1 = TIMx->CR1;

  /* 设置 TIM 时基单元参数 ----------------------------------------------------*/
  if (IS_TIM_COUNTER_MODE_SELECT_INSTANCE(TIMx))
  {
    /* 选择计数模式 */
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    tmpcr1 |= Structure->CounterMode;
  }

  if (IS_TIM_CLOCK_DIVISION_INSTANCE(TIMx))
  {
    /* 设置时钟分频 */
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= (uint32_t)Structure->ClockDivision;
  }

  /* Set the auto-reload preload */
  MODIFY_REG(tmpcr1, TIM_CR1_ARPE, Structure->AutoReloadPreload);

  /* Set the Autoreload value */
  TIMx->ARR = (uint32_t)Structure->Period ;

  /* Set the Prescaler value */
  TIMx->PSC = Structure->Prescaler;

  if (IS_TIM_REPETITION_COUNTER_INSTANCE(TIMx))
  {
    /* Set the Repetition Counter value */
    TIMx->RCR = Structure->RepetitionCounter;
  }

  /* Disable Update Event (UEV) with Update Generation (UG)
     by changing Update Request Source (URS) to avoid Update flag (UIF) */
  SET_BIT(TIMx->CR1, TIM_CR1_URS);

  /* Generate an update event to reload the Prescaler
     and the repetition counter (only for advanced timer) value immediately */
  TIMx->EGR = TIM_EGR_UG;

  TIMx->CR1 = tmpcr1;
}

/**
  * @brief  Timer Output Compare 1 configuration
  * @param  TIMx to select the TIM peripheral
  * @param  OC_Config The output configuration structure
  * @retval None
  */
static void TIM_OC1_SetConfig(TIM_TypeDef *TIMx, const TIM_OC_InitTypeDef *OC_Config)
{
  uint32_t tmpccmrx;
  uint32_t tmpccer;
  uint32_t tmpcr2;

  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;

  /* Disable the Channel 1: Reset the CC1E Bit */
  TIMx->CCER &= ~TIM_CCER_CC1E;

  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;

  /* Get the TIMx CCMR1 register value */
  tmpccmrx = TIMx->CCMR1;

  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR1_OC1M;
  tmpccmrx &= ~TIM_CCMR1_CC1S;
  /* Select the Output Compare Mode */
  tmpccmrx |= OC_Config->OCMode;

  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC1P;
  /* Set the Output Compare Polarity */
  tmpccer |= OC_Config->OCPolarity;

  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_1))
  {
    /* 检查参数 */
    assert_param(IS_TIM_OCN_POLARITY(OC_Config->OCNPolarity));

    /* 禁用通道 1N：复位 CC1NE 位 */
    TIMx->CCER &= ~TIM_CCER_CC1NE;

    /* 复位互补输出极性电平 */
    tmpccer &= ~TIM_CCER_CC1NP;
    /* 设置互补输出极性 */
    tmpccer |= OC_Config->OCNPolarity;
  }

  if (IS_TIM_BREAK_INSTANCE(TIMx))
  {
    /* 检查参数 */
    assert_param(IS_TIM_OCNIDLE_STATE(OC_Config->OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(OC_Config->OCIdleState));

    /* 复位输出比较及互补输出比较空闲状态 */
    tmpcr2 &= ~TIM_CR2_OIS1;
    tmpcr2 &= ~TIM_CR2_OIS1N;
    /* 设置输出空闲状态 */
    tmpcr2 |= OC_Config->OCIdleState;
    /* 设置互补输出空闲状态 */
    tmpcr2 |= OC_Config->OCNIdleState;
  }

  /* 写入 TIMx CR2 寄存器 */
  TIMx->CR2 = tmpcr2;

  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmrx;

  /* 设置捕获比较寄存器的值 */
  TIMx->CCR1 = OC_Config->Pulse;

  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  定时器输出比较 2 配置
  * @param  TIMx 选择 TIM 外设
  * @param  OC_Config 输出配置结构体
  * @retval 无
  */
void TIM_OC2_SetConfig(TIM_TypeDef *TIMx, const TIM_OC_InitTypeDef *OC_Config)
{
  uint32_t tmpccmrx;
  uint32_t tmpccer;
  uint32_t tmpcr2;

  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;

  /* 禁用通道 2：复位 CC2E 位 */
  TIMx->CCER &= ~TIM_CCER_CC2E;

  /* 获取 TIMx CR2 寄存器的值 */
  tmpcr2 =  TIMx->CR2;

  /* 获取 TIMx CCMR1 寄存器的值 */
  tmpccmrx = TIMx->CCMR1;

  /* 复位输出比较模式和捕获/比较选择位 */
  tmpccmrx &= ~TIM_CCMR1_OC2M;
  tmpccmrx &= ~TIM_CCMR1_CC2S;

  /* 选择输出比较模式 */
  tmpccmrx |= (OC_Config->OCMode << 8U);

  /* 复位输出极性电平 */
  tmpccer &= ~TIM_CCER_CC2P;
  /* 设置输出比较极性 */
  tmpccer |= (OC_Config->OCPolarity << 4U);

  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_2))
  {
    assert_param(IS_TIM_OCN_POLARITY(OC_Config->OCNPolarity));

    /* 禁用通道 2N：复位 CC2NE 位 */
    TIMx->CCER &= ~TIM_CCER_CC2NE;

    /* 复位互补输出 (N) 极性电平 */
    tmpccer &= ~TIM_CCER_CC2NP;
    /* 设置互补输出 (N) 极性 */
    tmpccer |= (OC_Config->OCNPolarity << 4U);
  }

  if (IS_TIM_BREAK_INSTANCE(TIMx))
  {
    /* 检查参数 */
    assert_param(IS_TIM_OCNIDLE_STATE(OC_Config->OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(OC_Config->OCIdleState));

    /* 复位输出比较及互补输出比较 (N) 空闲状态 */
    tmpcr2 &= ~TIM_CR2_OIS2;
    tmpcr2 &= ~TIM_CR2_OIS2N;
    /* 设置输出空闲状态 */
    tmpcr2 |= (OC_Config->OCIdleState << 2U);
    /* 设置互补输出 (N) 空闲状态 */
    tmpcr2 |= (OC_Config->OCNIdleState << 2U);
  }

  /* 写入 TIMx CR2 寄存器 */
  TIMx->CR2 = tmpcr2;

  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmrx;

  /* 设置捕获/比较寄存器的值 */
  TIMx->CCR2 = OC_Config->Pulse;

  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  定时器输出比较 3 配置
  * @param  TIMx 用于选择 TIM 外设
  * @param  OC_Config 输出配置结构体
  * @retval 无
  */
static void TIM_OC3_SetConfig(TIM_TypeDef *TIMx, const TIM_OC_InitTypeDef *OC_Config)
{
  uint32_t tmpccmrx;
  uint32_t tmpccer;
  uint32_t tmpcr2;

  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;

  /* 禁用通道 3：复位 CC2E 位 */
  TIMx->CCER &= ~TIM_CCER_CC3E;

  /* 获取 TIMx CR2 寄存器的值 */
  tmpcr2 =  TIMx->CR2;

  /* 获取 TIMx CCMR2 寄存器的值 */
  tmpccmrx = TIMx->CCMR2;

  /* 复位输出比较模式及输入捕获/输出比较选择位 */
  tmpccmrx &= ~TIM_CCMR2_OC3M;
  tmpccmrx &= ~TIM_CCMR2_CC3S;
  /* 选择输出比较模式 */
  tmpccmrx |= OC_Config->OCMode;

  /* 复位输出极性有效电平 */
  tmpccer &= ~TIM_CCER_CC3P;
  /* 设置输出比较极性 */
  tmpccer |= (OC_Config->OCPolarity << 8U);

  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_3))
  {
    assert_param(IS_TIM_OCN_POLARITY(OC_Config->OCNPolarity));

    /* 禁用通道 3N：复位 CC3NE 位 */
    TIMx->CCER &= ~TIM_CCER_CC3NE;

    /* 复位互补输出 N 极性有效电平 */
    tmpccer &= ~TIM_CCER_CC3NP;
    /* 设置互补输出 N 极性 */
    tmpccer |= (OC_Config->OCNPolarity << 8U);
  }

  if (IS_TIM_BREAK_INSTANCE(TIMx))
  {
    /* 检查参数有效性 */
    assert_param(IS_TIM_OCNIDLE_STATE(OC_Config->OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(OC_Config->OCIdleState));

    /* Reset the Output Compare and Output Compare N IDLE State */
    tmpcr2 &= ~TIM_CR2_OIS3;
    tmpcr2 &= ~TIM_CR2_OIS3N;
    /* Set the Output Idle state */
    tmpcr2 |= (OC_Config->OCIdleState << 4U);
    /* Set the Output N Idle state */
    tmpcr2 |= (OC_Config->OCNIdleState << 4U);
  }

  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;

  /* Write to TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmrx;

  /* Set the Capture Compare Register value */
  TIMx->CCR3 = OC_Config->Pulse;

  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Timer Output Compare 4 configuration
  * @param  TIMx to select the TIM peripheral
  * @param  OC_Config The output configuration structure
  * @retval None
  */
static void TIM_OC4_SetConfig(TIM_TypeDef *TIMx, const TIM_OC_InitTypeDef *OC_Config)
{
  uint32_t tmpccmrx;
  uint32_t tmpccer;
  uint32_t tmpcr2;

  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;

  /* Disable the Channel 4: Reset the CC4E Bit */
  TIMx->CCER &= ~TIM_CCER_CC4E;

  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;

  /* Get the TIMx CCMR2 register value */
  tmpccmrx = TIMx->CCMR2;

  /* Reset the Output Compare mode and Capture/Compare selection Bits */
  tmpccmrx &= ~TIM_CCMR2_OC4M;
  tmpccmrx &= ~TIM_CCMR2_CC4S;

  /* Select the Output Compare Mode */
  tmpccmrx |= (OC_Config->OCMode << 8U);

  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC4P;
  /* 设置输出比较极性 */
  tmpccer |= (OC_Config->OCPolarity << 12U);

  if (IS_TIM_BREAK_INSTANCE(TIMx))
  {
    /* 检查参数 */
    assert_param(IS_TIM_OCIDLE_STATE(OC_Config->OCIdleState));

    /* 复位输出比较空闲状态 */
    tmpcr2 &= ~TIM_CR2_OIS4;

    /* 设置输出空闲状态 */
    tmpcr2 |= (OC_Config->OCIdleState << 6U);
  }

  /* 写入 TIMx CR2 寄存器 */
  TIMx->CR2 = tmpcr2;

  /* 写入 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmrx;

  /* 设置捕获比较寄存器的值 */
  TIMx->CCR4 = OC_Config->Pulse;

  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  从定时器配置函数
  * @param  htim TIM 句柄
  * @param  sSlaveConfig 从定时器配置结构体
  * @retval 无
  */
static HAL_StatusTypeDef TIM_SlaveTimer_SetConfig(TIM_HandleTypeDef *htim,
                                                  const TIM_SlaveConfigTypeDef *sSlaveConfig)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;
  uint32_t tmpccmr1;
  uint32_t tmpccer;

  /* 获取 TIMx SMCR 寄存器的值 */
  tmpsmcr = htim->Instance->SMCR;

  /* 复位触发选择位 */
  tmpsmcr &= ~TIM_SMCR_TS;
  /* 设置输入触发源 */
  tmpsmcr |= sSlaveConfig->InputTrigger;

  /* 复位从模式选择位 */
  tmpsmcr &= ~TIM_SMCR_SMS;
  /* 设置从模式 */
  tmpsmcr |= sSlaveConfig->SlaveMode;

  /* 写入 TIMx SMCR 寄存器 */
  htim->Instance->SMCR = tmpsmcr;

  /* 配置触发器预分频器、滤波器以及极性 */
  switch (sSlaveConfig->InputTrigger)
  {
    case TIM_TS_ETRF:
    {
      /* 检查参数有效性 */
      assert_param(IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE(htim->Instance));
      assert_param(IS_TIM_TRIGGERPRESCALER(sSlaveConfig->TriggerPrescaler));
      assert_param(IS_TIM_TRIGGERPOLARITY(sSlaveConfig->TriggerPolarity));
      assert_param(IS_TIM_TRIGGERFILTER(sSlaveConfig->TriggerFilter));
      /* 配置 ETR 触发源 */
      TIM_ETR_SetConfig(htim->Instance,
                        sSlaveConfig->TriggerPrescaler,
                        sSlaveConfig->TriggerPolarity,
                        sSlaveConfig->TriggerFilter);
      break;
    }

    case TIM_TS_TI1F_ED:
    {
      /* 检查参数有效性 */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
      assert_param(IS_TIM_TRIGGERFILTER(sSlaveConfig->TriggerFilter));

      if (sSlaveConfig->SlaveMode == TIM_SLAVEMODE_GATED)
      {
        return HAL_ERROR;
      }

      /* 禁用通道 1：复位 CC1E 位 */
      tmpccer = htim->Instance->CCER;
      htim->Instance->CCER &= ~TIM_CCER_CC1E;
      tmpccmr1 = htim->Instance->CCMR1;

      /* 设置滤波器 */
      tmpccmr1 &= ~TIM_CCMR1_IC1F;
      tmpccmr1 |= ((sSlaveConfig->TriggerFilter) << 4U);

      /* 写入 TIMx CCMR1 和 CCER 寄存器 */
      htim->Instance->CCMR1 = tmpccmr1;
      htim->Instance->CCER = tmpccer;
      break;
    }

    case TIM_TS_TI1FP1:
    {
      /* 检查参数有效性 */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
      assert_param(IS_TIM_TRIGGERPOLARITY(sSlaveConfig->TriggerPolarity));
      assert_param(IS_TIM_TRIGGERFILTER(sSlaveConfig->TriggerFilter));

      /* 配置 TI1 滤波器和极性 */
      TIM_TI1_ConfigInputStage(htim->Instance,
                               sSlaveConfig->TriggerPolarity,
                               sSlaveConfig->TriggerFilter);
      break;
    }

    case TIM_TS_TI2FP2:
    {
      /* 检查参数有效性 */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
      assert_param(IS_TIM_TRIGGERPOLARITY(sSlaveConfig->TriggerPolarity));
      assert_param(IS_TIM_TRIGGERFILTER(sSlaveConfig->TriggerFilter));

      /* 配置 TI2 滤波器和极性 */
      TIM_TI2_ConfigInputStage(htim->Instance,
                               sSlaveConfig->TriggerPolarity,
                               sSlaveConfig->TriggerFilter);
      break;
    }

    case TIM_TS_ITR0:
    case TIM_TS_ITR1:
    case TIM_TS_ITR2:
    case TIM_TS_ITR3:
    {
      /* 检查参数有效性 */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  return status;
}

/**
  * @brief  将 TI1 配置为输入。
  * @param  TIMx 用于选择 TIM 外设。
  * @param  TIM_ICPolarity 输入极性。
  *          该参数可以是以下值之一：
  *            @arg TIM_ICPOLARITY_RISING
  *            @arg TIM_ICPOLARITY_FALLING
  *            @arg TIM_ICPOLARITY_BOTHEDGE
  * @param  TIM_ICSelection 指定所使用的输入源。
  *          该参数可以是以下值之一：
  *            @arg TIM_ICSELECTION_DIRECTTI: 选择 TIM 输入 1 连接至 IC1。
  *            @arg TIM_ICSELECTION_INDIRECTTI: 选择 TIM 输入 1 连接至 IC2。
  *            @arg TIM_ICSELECTION_TRC: 选择 TIM 输入 1 连接至 TRC。
  * @param  TIM_ICFilter 指定输入捕获滤波器。
  *          该参数必须是介于 0x00 和 0x0F 之间的值。
  * @retval 无
  * @note 在 INDIRECT 模式下不使用 TIM_ICFilter 和 TIM_ICPolarity，
  *       因为使用 TI2FP1（通道 2 路径上）作为输入信号。
  *       因此必须保护 CCMR1 免受未初始化的滤波器和极性值的影响。
  */
void TIM_TI1_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr1;
  uint32_t tmpccer;

  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;

  /* 禁用通道 1：复位 CC1E 位 */
  TIMx->CCER &= ~TIM_CCER_CC1E;
  /* 禁用通道 1N：复位 CC1NE 位 */
  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_1))
  {
    TIMx->CCER &= ~TIM_CCER_CC1NE;
  }

  /* 获取 TIMx CCMR1 寄存器的值 */
  tmpccmr1 = TIMx->CCMR1;

  /* 选择输入引脚 */
  if (IS_TIM_CC2_INSTANCE(TIMx) != RESET)
  {
    tmpccmr1 &= ~TIM_CCMR1_CC1S;
    tmpccmr1 |= TIM_ICSelection;
  }
  else
  {
    tmpccmr1 |= TIM_CCMR1_CC1S_0;
  }

  /* 设置滤波器 */
  tmpccmr1 &= ~TIM_CCMR1_IC1F;
  tmpccmr1 |= ((TIM_ICFilter << 4U) & TIM_CCMR1_IC1F);

  /* 选择极性并置位 CC1E 位 */
  tmpccer &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
  tmpccer |= (TIM_ICPolarity & (TIM_CCER_CC1P | TIM_CCER_CC1NP));

  /* 写入 TIMx CCMR1 和 CCER 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  配置 TI1 的极性和滤波器。
  * @param  TIMx 用于选择 TIM 外设。
  * @param  TIM_ICPolarity 输入极性。
  *          该参数可以是以下值之一：
  *            @arg TIM_ICPOLARITY_RISING
  *            @arg TIM_ICPOLARITY_FALLING
  *            @arg TIM_ICPOLARITY_BOTHEDGE
  * @param  TIM_ICFilter 指定输入捕获滤波器。
  *          该参数的取值范围必须介于 0x00 和 0x0F 之间。
  * @retval 无
  */
static void TIM_TI1_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr1;
  uint32_t tmpccer;

  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;

  /* 禁用通道 1：复位 CC1E 位 */
  TIMx->CCER &= ~TIM_CCER_CC1E;
  /* 禁用通道 1N：复位 CC1NE 位 */
  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_1))
  {
    TIMx->CCER &= ~TIM_CCER_CC1NE;
  }

  /* 获取 TIMx CCMR1 寄存器的值 */
  tmpccmr1 = TIMx->CCMR1;

  /* 设置滤波器 */
  tmpccmr1 &= ~TIM_CCMR1_IC1F;
  tmpccmr1 |= (TIM_ICFilter << 4U);

  /* 选择极性并置位 CC1E 位 */
  tmpccer &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
  tmpccer |= TIM_ICPolarity;

  /* 写入 TIMx CCMR1 和 CCER 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  将 TI2 配置为输入。
  * @param  TIMx 用于选择 TIM 外设
  * @param  TIM_ICPolarity 输入极性。
  *          该参数可以是以下值之一：
  *            @arg TIM_ICPOLARITY_RISING
  *            @arg TIM_ICPOLARITY_FALLING
  *            @arg TIM_ICPOLARITY_BOTHEDGE
  * @param  TIM_ICSelection 指定所使用的输入源。
  *          该参数可以是以下值之一：
  *            @arg TIM_ICSELECTION_DIRECTTI: 选择 TIM 输入 2 连接到 IC2。
  *            @arg TIM_ICSELECTION_INDIRECTTI: 选择 TIM 输入 2 连接到 IC1。
  *            @arg TIM_ICSELECTION_TRC: 选择 TIM 输入 2 连接到 TRC。
  * @param  TIM_ICFilter 指定输入捕获滤波器。
  *          该参数的取值范围必须介于 0x00 和 0x0F 之间。
  * @retval 无
  * @note 在间接（INDIRECT）模式下不使用 TIM_ICFilter 和 TIM_ICPolarity，因为此时使用
  *       TI1FP2（位于通道 1 路径上）作为输入信号。因此必须对 CCMR1 进行保护，
  *       以防止出现未初始化的滤波器和极性值。
  */
static void TIM_TI2_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                              uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr1;
  uint32_t tmpccer;

  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;

  /* 禁用通道 2：复位 CC2E 位 */
  TIMx->CCER &= ~TIM_CCER_CC2E;
  /* 禁用互补通道 2N：复位 CC2NE 位 */
  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_2))
  {
    TIMx->CCER &= ~TIM_CCER_CC2NE;
  }

  /* 获取 TIMx CCMR1 寄存器的值 */
  tmpccmr1 = TIMx->CCMR1;

  /* 选择输入端 */
  tmpccmr1 &= ~TIM_CCMR1_CC2S;
  tmpccmr1 |= (TIM_ICSelection << 8U);

  /* 设置滤波器 */
  tmpccmr1 &= ~TIM_CCMR1_IC2F;
  tmpccmr1 |= ((TIM_ICFilter << 12U) & TIM_CCMR1_IC2F);

  /* 选择极性并置位 CC2E 位 */
  tmpccer &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
  tmpccer |= ((TIM_ICPolarity << 4U) & (TIM_CCER_CC2P | TIM_CCER_CC2NP));

  /* 写入 TIMx CCMR1 和 CCER 寄存器 */
  TIMx->CCMR1 = tmpccmr1 ;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  配置 TI2 的极性和滤波器。
  * @param  TIMx 用于选择 TIM 外设。
  * @param  TIM_ICPolarity 输入极性。
  *          该参数可以是以下值之一：
  *            @arg TIM_ICPOLARITY_RISING
  *            @arg TIM_ICPOLARITY_FALLING
  *            @arg TIM_ICPOLARITY_BOTHEDGE
  * @param  TIM_ICFilter 指定输入捕获滤波器。
  *          该参数取值必须介于 0x00 和 0x0F 之间。
  * @retval 无
  */
static void TIM_TI2_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr1;
  uint32_t tmpccer;

  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;

  /* 禁用通道 2：复位 CC2E 位 */
  TIMx->CCER &= ~TIM_CCER_CC2E;
  /* 禁用互补通道 2N：复位 CC2NE 位 */
  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_2))
  {
    TIMx->CCER &= ~TIM_CCER_CC2NE;
  }

  /* 获取 TIMx CCMR1 寄存器的值 */
  tmpccmr1 = TIMx->CCMR1;

  /* 设置滤波器 */
  tmpccmr1 &= ~TIM_CCMR1_IC2F;
  tmpccmr1 |= (TIM_ICFilter << 12U);

  /* 选择极性并置位 CC2E 位 */
  tmpccer &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
  tmpccer |= (TIM_ICPolarity << 4U);

  /* 写入 TIMx CCMR1 和 CCER 寄存器 */
  TIMx->CCMR1 = tmpccmr1 ;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  将 TI3 配置为输入。
  * @param  TIMx 用于选择 TIM 外设
  * @param  TIM_ICPolarity 输入极性。
  *          该参数可以是以下值之一：
  *            @arg TIM_ICPOLARITY_RISING
  *            @arg TIM_ICPOLARITY_FALLING
  * @param  TIM_ICSelection 指定所使用的输入源。
  *          该参数可以是以下值之一：
  *            @arg TIM_ICSELECTION_DIRECTTI: 选择 TIM 输入 3 连接到 IC3。
  *            @arg TIM_ICSELECTION_INDIRECTTI: 选择 TIM 输入 3 连接到 IC4。
  *            @arg TIM_ICSELECTION_TRC: 选择 TIM 输入 3 连接到 TRC。
  * @param  TIM_ICFilter 指定输入捕获滤波器。
  *          该参数必须是介于 0x00 和 0x0F 之间的值。
  * @retval 无
  * @note 在 INDIRECT（间接）模式下不使用 TIM_ICFilter 和 TIM_ICPolarity，因为使用
  *       TI3FP4（在通道 1 路径上）作为输入信号。因此必须保护 CCMR2
  *       免受未初始化的滤波器和极性值的影响。
  */
static void TIM_TI3_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                              uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr2;
  uint32_t tmpccer;

  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;

  /* 禁用通道 3：复位 CC3E 位 */
  TIMx->CCER &= ~TIM_CCER_CC3E;
  /* 禁用通道 3N：复位 CC3NE 位 */
  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_3))
  {
    TIMx->CCER &= ~TIM_CCER_CC3NE;
  }

  /* 获取 TIMx CCMR2 寄存器的值 */
  tmpccmr2 = TIMx->CCMR2;

  /* 选择输入源 */
  tmpccmr2 &= ~TIM_CCMR2_CC3S;
  tmpccmr2 |= TIM_ICSelection;

  /* 设置滤波器 */
  tmpccmr2 &= ~TIM_CCMR2_IC3F;
  tmpccmr2 |= ((TIM_ICFilter << 4U) & TIM_CCMR2_IC3F);

  /* 选择极性并置位 CC3E 位 */
  tmpccer &= ~(TIM_CCER_CC3P);
  tmpccer |= ((TIM_ICPolarity << 8U) & TIM_CCER_CC3P);

  /* 写入 TIMx CCMR2 和 CCER 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  将 TI4 配置为输入。
  * @param  TIMx 用于选择 TIM 外设
  * @param  TIM_ICPolarity 输入极性。
  *          该参数可以是以下值之一：
  *            @arg TIM_ICPOLARITY_RISING
  *            @arg TIM_ICPOLARITY_FALLING
  * @param  TIM_ICSelection 指定所使用的输入源。
  *          该参数可以是以下值之一：
  *            @arg TIM_ICSELECTION_DIRECTTI: 选择 TIM 输入 4 连接到 IC4。
  *            @arg TIM_ICSELECTION_INDIRECTTI: 选择 TIM 输入 4 连接到 IC3。
  *            @arg TIM_ICSELECTION_TRC: 选择 TIM 输入 4 连接到 TRC。
  * @param  TIM_ICFilter 指定输入捕获滤波器。
  *          该参数必须是介于 0x00 和 0x0F 之间的值。
  * @note 在 INDIRECT（间接）模式下不使用 TIM_ICFilter 和 TIM_ICPolarity，因为使用
  *       TI4FP3（在通道 1 路径上）作为输入信号。因此必须保护 CCMR2
  *       免受未初始化的滤波器和极性值的影响。
  * @retval 无
  */
static void TIM_TI4_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                              uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr2;
  uint32_t tmpccer;

  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;

  /* 禁用通道 4：复位 CC4E 位 */
  TIMx->CCER &= ~TIM_CCER_CC4E;

  /* 获取 TIMx CCMR2 寄存器的值 */
  tmpccmr2 = TIMx->CCMR2;

  /* 选择输入源 */
  tmpccmr2 &= ~TIM_CCMR2_CC4S;
  tmpccmr2 |= (TIM_ICSelection << 8U);

  /* 设置滤波器 */
  tmpccmr2 &= ~TIM_CCMR2_IC4F;
  tmpccmr2 |= ((TIM_ICFilter << 12U) & TIM_CCMR2_IC4F);

  /* 选择极性并置位 CC4E 位 */
  tmpccer &= ~(TIM_CCER_CC4P);
  tmpccer |= ((TIM_ICPolarity << 12U) & TIM_CCER_CC4P);

  /* 写入 TIMx CCMR2 和 CCER 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer ;
}

/**
  * @brief  选择输入触发源
  * @param  TIMx 用于选择 TIM 外设
  * @param  InputTriggerSource 输入触发源。
  *          该参数可以是以下值之一：
  *            @arg TIM_TS_ITR0: 内部触发 0
  *            @arg TIM_TS_ITR1: 内部触发 1
  *            @arg TIM_TS_ITR2: 内部触发 2
  *            @arg TIM_TS_ITR3: 内部触发 3
  *            @arg TIM_TS_TI1F_ED: TI1 边沿检测器
  *            @arg TIM_TS_TI1FP1: 滤波后的定时器输入 1
  *            @arg TIM_TS_TI2FP2: 滤波后的定时器输入 2
  *            @arg TIM_TS_ETRF: 外部触发输入
  * @retval 无
  */
static void TIM_ITRx_SetConfig(TIM_TypeDef *TIMx, uint32_t InputTriggerSource)
{
  uint32_t tmpsmcr;

  /* 获取 TIMx SMCR 寄存器的值 */
  tmpsmcr = TIMx->SMCR;
  /* 复位 TS 位 */
  tmpsmcr &= ~TIM_SMCR_TS;
  /* 设置输入触发源和从模式 */
  tmpsmcr |= (InputTriggerSource | TIM_SLAVEMODE_EXTERNAL1);
  /* 写入 TIMx SMCR 寄存器 */
  TIMx->SMCR = tmpsmcr;
}
/**
  * @brief  配置 TIMx 外部触发 (ETR)。
  * @param  TIMx 用于选择 TIM 外设
  * @param  TIM_ExtTRGPrescaler 外部触发预分频器。
  *          该参数可以是以下值之一：
  *            @arg TIM_ETRPRESCALER_DIV1: ETRP 预分频器关闭。
  *            @arg TIM_ETRPRESCALER_DIV2: ETRP 频率 2 分频。
  *            @arg TIM_ETRPRESCALER_DIV4: ETRP 频率 4 分频。
  *            @arg TIM_ETRPRESCALER_DIV8: ETRP 频率 8 分频。
  * @param  TIM_ExtTRGPolarity 外部触发极性。
  *          该参数可以是以下值之一：
  *            @arg TIM_ETRPOLARITY_INVERTED: 低电平有效或下降沿有效。
  *            @arg TIM_ETRPOLARITY_NONINVERTED: 高电平有效或上升沿有效。
  * @param  ExtTRGFilter 外部触发滤波器。
  *          该参数必须是介于 0x00 和 0x0F 之间的值
  * @retval 无
  */
void TIM_ETR_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ExtTRGPrescaler,
                       uint32_t TIM_ExtTRGPolarity, uint32_t ExtTRGFilter)
{
  uint32_t tmpsmcr;

  tmpsmcr = TIMx->SMCR;

  /* 复位 ETR 位 */
  tmpsmcr &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP);

  /* 设置预分频器、滤波值和极性 */
  tmpsmcr |= (uint32_t)(TIM_ExtTRGPrescaler | (TIM_ExtTRGPolarity | (ExtTRGFilter << 8U)));

  /* 写入 TIMx SMCR 寄存器 */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @brief  使能或失能 TIM 捕获/比较通道 x。
  * @param  TIMx 用于选择 TIM 外设
  * @param  Channel 指定 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选中 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选中 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选中 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选中 TIM 通道 4
  * @param  ChannelState 指定 TIM 通道 CCxE 位的新状态。
  *          该参数可以是：TIM_CCx_ENABLE 或 TIM_CCx_DISABLE。
  * @retval 无
  */
void TIM_CCxChannelCmd(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t ChannelState)
{
  uint32_t tmp;

  /* 检查参数 */
  assert_param(IS_TIM_CC1_INSTANCE(TIMx));
  assert_param(IS_TIM_CHANNELS(Channel));

  tmp = TIM_CCER_CC1E << (Channel & 0x1FU); /* 0x1FU = 最大位移 31 位 */

  /* Reset the CCxE Bit */
  TIMx->CCER &= ~tmp;

  /* Set or reset the CCxE Bit */
  TIMx->CCER |= (uint32_t)(ChannelState << (Channel & 0x1FU)); /* 0x1FU = 31 bits max shift */
}

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
/**
  * @brief  Reset interrupt callbacks to the legacy weak callbacks.
  * @param  htim pointer to a TIM_HandleTypeDef structure that contains
  *                the configuration information for TIM module.
  * @retval None
  */
void TIM_ResetCallback(TIM_HandleTypeDef *htim)
{
  /* Reset the TIM callback to the legacy weak callbacks */
  htim->PeriodElapsedCallback             = HAL_TIM_PeriodElapsedCallback;
  htim->PeriodElapsedHalfCpltCallback     = HAL_TIM_PeriodElapsedHalfCpltCallback;
  htim->TriggerCallback                   = HAL_TIM_TriggerCallback;
  htim->TriggerHalfCpltCallback           = HAL_TIM_TriggerHalfCpltCallback;
  htim->IC_CaptureCallback                = HAL_TIM_IC_CaptureCallback;
  htim->IC_CaptureHalfCpltCallback        = HAL_TIM_IC_CaptureHalfCpltCallback;
  htim->OC_DelayElapsedCallback           = HAL_TIM_OC_DelayElapsedCallback;
  htim->PWM_PulseFinishedCallback         = HAL_TIM_PWM_PulseFinishedCallback;
  htim->PWM_PulseFinishedHalfCpltCallback = HAL_TIM_PWM_PulseFinishedHalfCpltCallback;
  htim->ErrorCallback                     = HAL_TIM_ErrorCallback;
  htim->CommutationCallback               = HAL_TIMEx_CommutCallback;
  htim->CommutationHalfCpltCallback       = HAL_TIMEx_CommutHalfCpltCallback;
  htim->BreakCallback                     = HAL_TIMEx_BreakCallback;
}
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

/**
  * @}
  */

#endif /* HAL_TIM_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/**
  ******************************************************************************
  * @file    stm32f1xx_hal_tim_ex.c
  * @author  MCD 应用团队
  * @brief   TIM HAL 模块驱动程序。
  *          本文件提供用于管理定时器扩展（Timer Extended）外设以下功能的固件函数：
  *           + 定时器霍尔传感器接口初始化
  *           + 定时器霍尔传感器接口启动
  *           + 定时器互补信号刹车和死区时间配置
  *           + 定时器主从同步配置
  *           + 定时器重映射功能配置
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * 本软件根据可在该软件组件根目录下的 LICENSE 文件中找到的条款获得许可。
  * 如果本软件未随附 LICENSE 文件，则按“原样”提供。
  *
  ******************************************************************************
  @verbatim
  ==============================================================================
                      ##### 定时器扩展特性 #####
  ==============================================================================
  [..]
    定时器扩展特性包括：
    (#) 带可编程死区时间的互补输出，适用于：
        (++) 输出比较
        (++) PWM 生成（边沿对齐和中心对齐模式）
        (++) 单脉冲模式输出
    (#) 同步电路，用于使用外部信号控制定时器以及将多个定时器相互互联。
    (#) 刹车输入，用于将定时器输出信号置于复位状态或已知状态。
    (#) 支持用于定位目的的增量式（正交）编码器和霍尔传感器电路

            ##### 如何使用本驱动程序 #####
  ==============================================================================
    [..]
     (#) 根据所选特性，通过实现以下函数来初始化 TIM 底层资源：
           (++) 霍尔传感器输出：HAL_TIMEx_HallSensor_MspInit()

     (#) 初始化 TIM 底层资源：
        (##) 使用 __HAL_RCC_TIMx_CLK_ENABLE() 使能 TIM 接口时钟；
        (##) TIM 引脚配置
            (+++) 使用以下函数使能 TIM GPIO 的时钟：
              __HAL_RCC_GPIOx_CLK_ENABLE();
            (+++) 使用 HAL_GPIO_Init() 将这些 TIM 引脚配置为复用功能模式；

     (#) 如果需要，可以使用以下函数配置外部时钟（默认时钟是来自 APBx 的内部时钟）：
         HAL_TIM_ConfigClockSource，时钟配置应在任何启动函数之前完成。

     (#) 使用本驱动程序的初始化函数之一将 TIM 配置为所需的工作模式：
          (++) HAL_TIMEx_HallSensor_Init() 和 HAL_TIMEx_ConfigCommutEvent()：使用定时器
               霍尔传感器接口以及相应的换向事件（如果需要，还包括相应的中断和 DMA 请求）
               （注意：一个定时器用于与霍尔传感器接口相连，另一个定时器应用于使用换向事件）。

     (#) 使用以下启动函数之一使能 TIM 外设：
           (++) 互补输出比较：HAL_TIMEx_OCN_Start()、HAL_TIMEx_OCN_Start_DMA()、
                HAL_TIMEx_OCN_Start_IT()
           (++) 互补 PWM 生成：HAL_TIMEx_PWMN_Start()、HAL_TIMEx_PWMN_Start_DMA()、
                HAL_TIMEx_PWMN_Start_IT()
           (++) 互补单脉冲模式输出：HAL_TIMEx_OnePulseN_Start()、HAL_TIMEx_OnePulseN_Start_IT()
           (++) 霍尔传感器输出：HAL_TIMEx_HallSensor_Start()、HAL_TIMEx_HallSensor_Start_DMA()、
                HAL_TIMEx_HallSensor_Start_IT()。

  @endverbatim
  ******************************************************************************
  */

/* 头文件包含 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup TIMEx TIMEx
  * @brief TIM 扩展 HAL 模块驱动程序
  * @{
  */

#ifdef HAL_TIM_MODULE_ENABLED

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有宏 --------------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 私有函数原型 --------------------------------------------------------------*/
static void TIM_DMADelayPulseNCplt(DMA_HandleTypeDef *hdma);
static void TIM_DMAErrorCCxN(DMA_HandleTypeDef *hdma);
static void TIM_CCxNChannelCmd(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t ChannelNState);

/* 导出函数 ------------------------------------------------------------------*/
/** @defgroup TIMEx_Exported_Functions TIM 扩展导出函数
  * @{
  */

/** @defgroup TIMEx_Exported_Functions_Group1 扩展定时器霍尔传感器函数
  * @brief    定时器霍尔传感器函数
  *
@verbatim
  ==============================================================================
                      ##### 定时器霍尔传感器函数 #####
  ==============================================================================
  [..]
    本节提供的函数允许：
    (+) 初始化并配置 TIM 霍尔传感器。
    (+) 反初始化 TIM 霍尔传感器。
    (+) 启动霍尔传感器接口。
    (+) 停止霍尔传感器接口。
    (+) 启动霍尔传感器接口并使能中断。
    (+) 停止霍尔传感器接口并禁用中断。
    (+) 启动霍尔传感器接口并使能 DMA 传输。
    (+) 停止霍尔传感器接口并禁用 DMA 传输。

@endverbatim
  * @{
  */
/**
  * @brief  初始化 TIM 霍尔传感器接口并初始化关联的句柄。
  * @note   当定时器实例被初始化为霍尔传感器接口模式时，
  *         定时器通道 1 和通道 2 将被保留，不能用于
  *         其他用途。
  * @param  htim TIM 霍尔传感器接口句柄
  * @param  sConfig TIM 霍尔传感器配置结构体
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Init(TIM_HandleTypeDef *htim, const TIM_HallSensor_InitTypeDef *sConfig)
{
  TIM_OC_InitTypeDef OC_Config;

  /* 检查 TIM 句柄分配 */
  if (htim == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));
  assert_param(IS_TIM_IC_POLARITY(sConfig->IC1Polarity));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));
  assert_param(IS_TIM_IC_PRESCALER(sConfig->IC1Prescaler));
  assert_param(IS_TIM_IC_FILTER(sConfig->IC1Filter));

  if (htim->State == HAL_TIM_STATE_RESET)
  {
    /* 分配锁资源并进行初始化 */
    htim->Lock = HAL_UNLOCKED;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
    /* 将中断回调函数复位为传统的弱定义（weak）回调函数 */
    TIM_ResetCallback(htim);

    if (htim->HallSensor_MspInitCallback == NULL)
    {
      htim->HallSensor_MspInitCallback = HAL_TIMEx_HallSensor_MspInit;
    }
    /* 初始化底层硬件：GPIO、时钟、NVIC */
    htim->HallSensor_MspInitCallback(htim);
#else
    /* 初始化底层硬件：GPIO、时钟、NVIC 和 DMA */
    HAL_TIMEx_HallSensor_MspInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
  }

  /* 设置 TIM 状态 */
  htim->State = HAL_TIM_STATE_BUSY;

  /* 在编码器模式下配置时基单元 */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* 将通道 1 配置为输入通道，以连接霍尔传感器的三个输出 */
  TIM_TI1_SetConfig(htim->Instance, sConfig->IC1Polarity, TIM_ICSELECTION_TRC, sConfig->IC1Filter);

  /* 复位 IC1PSC 位 */
  htim->Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC;
  /* 设置 IC1PSC 值 */
  htim->Instance->CCMR1 |= sConfig->IC1Prescaler;

  /* 使能霍尔传感器接口（三个输入的异或功能） */
  htim->Instance->CR2 |= TIM_CR2_TI1S;

  /* 选择 TIM_TS_TI1F_ED 信号作为 TIM 的输入触发源 */
  htim->Instance->SMCR &= ~TIM_SMCR_TS;
  htim->Instance->SMCR |= TIM_TS_TI1F_ED;

  /* 使用 TIM_TS_TI1F_ED 信号在每次检测到边沿时复位 TIM 计数器 */
  htim->Instance->SMCR &= ~TIM_SMCR_SMS;
  htim->Instance->SMCR |= TIM_SLAVEMODE_RESET;

  /* 将通道 2 配置为 PWM 模式 2，并设置所需的换相延迟（Commutation_Delay） */
  OC_Config.OCFastMode = TIM_OCFAST_DISABLE;
  OC_Config.OCIdleState = TIM_OCIDLESTATE_RESET;
  OC_Config.OCMode = TIM_OCMODE_PWM2;
  OC_Config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  OC_Config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  OC_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
  OC_Config.Pulse = sConfig->Commutation_Delay;

  TIM_OC2_SetConfig(htim->Instance, &OC_Config);

  /* 选择 OC2REF 作为 TRGO 上的触发输出：将 TIMx_CR2 寄存器中的 MMS 位写入 101 */
  htim->Instance->CR2 &= ~TIM_CR2_MMS;
  htim->Instance->CR2 |= TIM_TRGO_OC2REF;

  /* 初始化 DMA 突发操作状态 */
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
  * @brief  反初始化 TIM 霍尔传感器接口
  * @param  htim TIM 霍尔传感器接口句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_DeInit(TIM_HandleTypeDef *htim)
{
  /* 检查参数有效性 */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* 禁用 TIM 外设时钟 */
  __HAL_TIM_DISABLE(htim);

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  if (htim->HallSensor_MspDeInitCallback == NULL)
  {
    htim->HallSensor_MspDeInitCallback = HAL_TIMEx_HallSensor_MspDeInit;
  }
  /* 反初始化底层硬件 */
  htim->HallSensor_MspDeInitCallback(htim);
#else
  /* 反初始化底层硬件：GPIO、CLOCK、NVIC */
  HAL_TIMEx_HallSensor_MspDeInit(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  /* 改变 DMA 突发操作状态 */
  htim->DMABurstState = HAL_DMA_BURST_STATE_RESET;

  /* 改变 TIM 通道状态 */
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
  * @brief  初始化 TIM 霍尔传感器 MSP。
  * @param  htim TIM 霍尔传感器接口句柄
  * @retval 无
  */
__weak void HAL_TIMEx_HallSensor_MspInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要回调函数时，
            可以在用户文件中重新实现 HAL_TIMEx_HallSensor_MspInit
   */
}

/**
  * @brief  反初始化 TIM 霍尔传感器 MSP。
  * @param  htim TIM 霍尔传感器接口句柄
  * @retval 无
  */
__weak void HAL_TIMEx_HallSensor_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数，当需要回调函数时，
            可以在用户文件中重新实现 HAL_TIMEx_HallSensor_MspDeInit
   */
}

/**
  * @brief  启动 TIM 霍尔传感器接口。
  * @param  htim TIM 霍尔传感器接口句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start(TIM_HandleTypeDef *htim)
{
  uint32_t tmpsmcr;
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);

  /* 检查参数 */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));

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

  /* 使能输入捕获通道 1
  （在霍尔传感器接口中，可使用的三个通道为 TIM_CHANNEL_1、
  TIM_CHANNEL_2 和 TIM_CHANNEL_3） */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);

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
  * @brief  停止 TIM 霍尔传感器接口。
  * @param  htim TIM 霍尔传感器接口句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop(TIM_HandleTypeDef *htim)
{
  /* 检查参数 */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));

  /* 禁用输入捕获通道 1、2 和 3
  （在霍尔传感器接口中，可使用的三个通道为 TIM_CHANNEL_1、
  TIM_CHANNEL_2 和 TIM_CHANNEL_3） */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);

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
  * @brief  以中断模式启动 TIM 霍尔传感器接口。
  * @param  htim TIM 霍尔传感器接口句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start_IT(TIM_HandleTypeDef *htim)
{
  uint32_t tmpsmcr;
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);

  /* 检查参数有效性 */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));

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

  /* 使能捕获比较 1 中断事件 */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);

  /* 使能输入捕获通道 1
  （在霍尔传感器接口中，可使用的三个通道分别为 TIM_CHANNEL_1、
  TIM_CHANNEL_2 和 TIM_CHANNEL_3） */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);

  /* 使能外设，但在触发模式下，外设会随触发信号自动使能 */
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
  * @brief  以中断模式停止 TIM 霍尔传感器接口。
  * @param  htim TIM 霍尔传感器接口句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop_IT(TIM_HandleTypeDef *htim)
{
  /* 检查参数有效性 */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));

  /* 禁用输入捕获通道 1
  （在霍尔传感器接口中，可使用的三个通道分别为 TIM_CHANNEL_1、
  TIM_CHANNEL_2 和 TIM_CHANNEL_3） */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);

  /* 禁用捕获比较中断事件 */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);

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
  * @brief  以 DMA 模式启动 TIM 霍尔传感器接口。
  * @param  htim TIM 霍尔传感器接口句柄
  * @param  pData 目标缓冲区地址。
  * @param  Length 从 TIM 外设传输到内存的数据长度。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start_DMA(TIM_HandleTypeDef *htim, uint32_t *pData, uint16_t Length)
{
  uint32_t tmpsmcr;
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);

  /* 检查参数 */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));

  /* 设置 TIM 通道状态 */
  if ((channel_1_state == HAL_TIM_CHANNEL_STATE_BUSY)
      || (complementary_channel_1_state == HAL_TIM_CHANNEL_STATE_BUSY))
  {
    return HAL_BUSY;
  }
  else if ((channel_1_state == HAL_TIM_CHANNEL_STATE_READY)
           && (complementary_channel_1_state == HAL_TIM_CHANNEL_STATE_READY))
  {
    if ((pData == NULL) || (Length == 0U))
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

  /* 使能输入捕获通道 1
  （在霍尔传感器接口中，可使用的三个通道分别为 TIM_CHANNEL_1、
  TIM_CHANNEL_2 和 TIM_CHANNEL_3） */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);

  /* 设置 DMA 输入捕获 1 回调函数 */
  htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;
  htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMACaptureHalfCplt;
  /* 设置 DMA 错误回调函数 */
  htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

  /* 使能捕获 1 对应的 DMA 通道 */
  if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->CCR1, (uint32_t)pData, Length) != HAL_OK)
  {
    /* 返回错误状态 */
    return HAL_ERROR;
  }
  /* 使能捕获/比较 1 中断 */
  __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);

  /* 使能外设，触发模式除外（触发模式下会随触发信号自动使能） */
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
  * @brief  停止 DMA 模式下的 TIM 霍尔传感器接口。
  * @param  htim TIM 霍尔传感器接口句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop_DMA(TIM_HandleTypeDef *htim)
{
  /* 检查参数 */
  assert_param(IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(htim->Instance));

  /* 禁用输入捕获通道 1
  (在霍尔传感器接口中，可使用的三个通道分别为 TIM_CHANNEL_1、
  TIM_CHANNEL_2 和 TIM_CHANNEL_3) */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);


  /* 禁用捕获比较中断 1 事件 */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);

  (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 通道状态 */
  TIM_CHANNEL_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
  TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group2 扩展定时器互补输出比较函数
  *  @brief   定时器互补输出比较函数
  *
@verbatim
  ==============================================================================
              ##### 定时器互补输出比较函数 #####
  ==============================================================================
  [..]
    本节提供的函数允许实现以下操作：
    (+) 启动互补输出比较/PWM。
    (+) 停止互补输出比较/PWM。
    (+) 启动互补输出比较/PWM 并使能中断。
    (+) 停止互补输出比较/PWM 并禁用中断。
    (+) 启动互补输出比较/PWM 并使能 DMA 传输。
    (+) 停止互补输出比较/PWM 并禁用 DMA 传输。

@endverbatim
  * @{
  */

/**
  * @brief  启动互补输出上的 TIM 输出比较信号生成。
  * @param  htim TIM 输出比较句柄
  * @param  Channel 要使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  uint32_t tmpsmcr;

  /* 检查参数有效性 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  /* 检查 TIM 互补通道状态 */
  if (TIM_CHANNEL_N_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 互补通道状态 */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);

  /* 使能捕获比较通道 N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);

  /* 使能主输出 */
  __HAL_TIM_MOE_ENABLE(htim);

  /* 使能外设，但在触发模式下除外（触发模式下会随触发自动使能） */
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
  * @brief  停止互补输出上的 TIM 输出比较信号生成。
  * @param  htim TIM 句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* 检查参数 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  /* 禁用捕获/比较通道 N */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);

  /* 禁用主输出 */
  __HAL_TIM_MOE_DISABLE(htim);

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 互补通道状态 */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  以中断模式启动互补输出上的 TIM 输出比较信号生成。
  * @param  htim TIM OC 句柄
  * @param  Channel 要启用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  /* 检查 TIM 互补通道状态 */
  if (TIM_CHANNEL_N_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 互补通道状态 */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 使能 TIM 输出比较中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 使能 TIM 输出比较中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 使能 TIM 输出比较中断 */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);
      break;
    }


    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 使能 TIM 刹车中断 */
    __HAL_TIM_ENABLE_IT(htim, TIM_IT_BREAK);

    /* 使能捕获/比较通道 N */
    TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);

    /* 使能主输出 */
    __HAL_TIM_MOE_ENABLE(htim);

    /* 使能外设（触发模式除外，该模式下外设将随触发信号自动使能） */
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
  * @brief  以中断模式停止互补输出引脚上的 TIM 输出比较信号生成。
  * @param  htim TIM 输出比较句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpccer;

  /* 检查参数有效性 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 禁用 TIM 输出比较中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 禁用 TIM 输出比较中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 禁用 TIM 输出比较中断 */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 禁用捕获/比较通道 N */
    TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);

    /* 禁用 TIM 刹车中断（仅当没有其他通道处于激活状态时） */
    tmpccer = htim->Instance->CCER;
    if ((tmpccer & TIM_CCER_CCxNE_MASK) == (uint32_t)RESET)
    {
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_BREAK);
    }

    /* 禁用主输出 */
    __HAL_TIM_MOE_DISABLE(htim);

    /* 禁用外设 */
    __HAL_TIM_DISABLE(htim);

    /* 设置 TIM 互补通道状态 */
    TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  在互补输出通道上以 DMA 模式启动 TIM 输出比较信号生成。
  * @param  htim TIM 输出比较句柄
  * @param  Channel 要使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @param  pData 源缓冲区地址。
  * @param  Length 从内存传输到 TIM 外设的数据长度
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, const uint32_t *pData,
                                          uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  /* 设置 TIM 互补通道状态 */
  if (TIM_CHANNEL_N_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_BUSY)
  {
    return HAL_BUSY;
  }
  else if (TIM_CHANNEL_N_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_READY)
  {
    if ((pData == NULL) || (Length == 0U))
    {
      return HAL_ERROR;
    }
    else
    {
      TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);
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
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseNCplt;
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAErrorCCxN ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData, (uint32_t)&htim->Instance->CCR1,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 输出比较 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseNCplt;
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAErrorCCxN ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 输出比较 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseNCplt;
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAErrorCCxN ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)pData, (uint32_t)&htim->Instance->CCR3,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 输出比较 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 使能捕获/比较通道 N */
    TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);

    /* 使能主输出 */
    __HAL_TIM_MOE_ENABLE(htim);

    /* 使能外设，触发模式除外（触发模式下由触发信号自动使能） */
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
  * @brief  在互补输出上以 DMA 模式停止 TIM 输出比较信号生成。
  * @param  htim TIM 输出比较句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数有效性 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* 禁用 TIM 输出比较 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC1]);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 禁用 TIM 输出比较 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC2]);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 禁用 TIM 输出比较 DMA 请求 */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);
      (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_CC3]);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 禁用捕获/比较通道 N */
    TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);

    /* 禁用主输出 */
    __HAL_TIM_MOE_DISABLE(htim);

    /* 禁用外设 */
    __HAL_TIM_DISABLE(htim);

    /* 设置 TIM 互补通道状态 */
    TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group3 Extended Timer Complementary PWM functions
  * @brief    定时器互补 PWM 相关函数
  *
@verbatim
  ==============================================================================
                 ##### 定时器互补 PWM 相关函数 #####
  ==============================================================================
  [..]
    本节提供的函数用于实现：
    (+) 启动互补 PWM。
    (+) 停止互补 PWM。
    (+) 启动互补 PWM 并使能中断。
    (+) 停止互补 PWM 并禁用中断。
    (+) 启动互补 PWM 并使能 DMA 传输。
    (+) 停止互补 PWM 并禁用 DMA 传输。
@endverbatim
  * @{
  */

/**
  * @brief  在互补输出通道上启动 PWM 信号生成。
  * @param  htim TIM 句柄
  * @param  Channel 要使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  uint32_t tmpsmcr;

  /* 检查参数有效性 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  /* 检查 TIM 互补通道状态 */
  if (TIM_CHANNEL_N_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 互补通道状态 */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);

  /* 使能互补 PWM 输出 */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);

  /* 使能主输出 */
  __HAL_TIM_MOE_ENABLE(htim);

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
  * @brief  停止互补输出通道上的 PWM 信号生成。
  * @param  htim TIM 句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* 检查参数有效性 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  /* 禁用互补 PWM 输出 */
  TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);

  /* 禁用主输出 */
  __HAL_TIM_MOE_DISABLE(htim);

  /* 禁用外设 */
  __HAL_TIM_DISABLE(htim);

  /* 设置 TIM 互补通道状态 */
  TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  在互补输出上以中断模式启动 PWM 信号生成。
  * @param  htim TIM 句柄
  * @param  Channel 待禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选中 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选中 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选中 TIM 通道 3
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  /* 检查 TIM 互补通道状态 */
  if (TIM_CHANNEL_N_STATE_GET(htim, Channel) != HAL_TIM_CHANNEL_STATE_READY)
  {
    return HAL_ERROR;
  }

  /* 设置 TIM 互补通道状态 */
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

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 使能 TIM 刹车中断 */
    __HAL_TIM_ENABLE_IT(htim, TIM_IT_BREAK);

    /* 使能互补 PWM 输出 */
    TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);

    /* 使能主输出 */
    __HAL_TIM_MOE_ENABLE(htim);

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
  * @brief  在中断模式下停止互补输出上的 PWM 信号生成。
  * @param  htim TIM 句柄
  * @param  Channel 要禁用的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpccer;

  /* 检查参数 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

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

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 禁用互补 PWM 输出 */
    TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);

    /* 禁用 TIM 刹车中断（仅在没有其他通道处于活动状态时） */
    tmpccer = htim->Instance->CCER;
    if ((tmpccer & TIM_CCER_CCxNE_MASK) == (uint32_t)RESET)
    {
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_BREAK);
    }

    /* 禁用主输出 */
    __HAL_TIM_MOE_DISABLE(htim);

    /* 禁用外设 */
    __HAL_TIM_DISABLE(htim);

    /* 设置 TIM 互补通道状态 */
    TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  在 DMA 模式下启动互补输出上的 TIM PWM 信号生成
  * @param  htim TIM 句柄
  * @param  Channel 要使能的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @param  pData 源缓冲区地址。
  * @param  Length 从内存传输到 TIM 外设的数据长度
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, const uint32_t *pData,
                                           uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

  /* 设置 TIM 互补通道状态 */
  if (TIM_CHANNEL_N_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_BUSY)
  {
    return HAL_BUSY;
  }
  else if (TIM_CHANNEL_N_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_READY)
  {
    if ((pData == NULL) || (Length == 0U))
    {
      return HAL_ERROR;
    }
    else
    {
      TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);
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
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseNCplt;
      htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAErrorCCxN ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData, (uint32_t)&htim->Instance->CCR1,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 输入捕获/输出比较 1 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseNCplt;
      htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAErrorCCxN ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 输入捕获/输出比较 2 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
      break;
    }

    case TIM_CHANNEL_3:
    {
      /* 设置 DMA 比较回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseNCplt;
      htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback = TIM_DMADelayPulseHalfCplt;

      /* 设置 DMA 错误回调函数 */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAErrorCCxN ;

      /* 使能 DMA 通道 */
      if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)pData, (uint32_t)&htim->Instance->CCR3,
                           Length) != HAL_OK)
      {
        /* 返回错误状态 */
        return HAL_ERROR;
      }
      /* 使能 TIM 输入捕获/输出比较 3 DMA 请求 */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);
      break;
    }

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 使能互补 PWM 输出 */
    TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_ENABLE);

    /* 使能主输出 */
    __HAL_TIM_MOE_ENABLE(htim);

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
  * @brief  停止互补输出上以 DMA 模式生成的 TIM PWM 信号
  * @param  htim TIM 句柄
  * @param  Channel 要禁用的 TIM 通道
  *          此参数可取以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数有效性 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, Channel));

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

    default:
      status = HAL_ERROR;
      break;
  }

  if (status == HAL_OK)
  {
    /* 禁用互补 PWM 输出 */
    TIM_CCxNChannelCmd(htim->Instance, Channel, TIM_CCxN_DISABLE);

    /* 禁用主输出 */
    __HAL_TIM_MOE_DISABLE(htim);

    /* 禁用外设 */
    __HAL_TIM_DISABLE(htim);

    /* 设置 TIM 互补通道状态 */
    TIM_CHANNEL_N_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY);
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group4 Extended Timer Complementary One Pulse functions
  * @brief    定时器互补单脉冲功能
  *
@verbatim
  ==============================================================================
                ##### 定时器互补单脉冲功能 #####
  ==============================================================================
  [..]
    本节提供的函数用于：
    (+) 启动互补单脉冲生成。
    (+) 停止互补单脉冲。
    (+) 启动互补单脉冲并使能中断。
    (+) 停止互补单脉冲并禁用中断。

@endverbatim
  * @{
  */

/**
  * @brief  在互补输出通道上启动 TIM 单脉冲信号生成。
  * @note OutputChannel 必须与调用 @ref HAL_TIM_OnePulse_ConfigChannel() 时所选的脉冲输出通道匹配。
  * @param  htim TIM 单脉冲句柄
  * @param  OutputChannel 要使能的脉冲输出通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Start(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  uint32_t input_channel = (OutputChannel == TIM_CHANNEL_1) ? TIM_CHANNEL_2 : TIM_CHANNEL_1;
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);

  /* 检查参数 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, OutputChannel));

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

  /* 使能互补单脉冲输出通道和输入捕获通道 */
  TIM_CCxNChannelCmd(htim->Instance, OutputChannel, TIM_CCxN_ENABLE);
  TIM_CCxChannelCmd(htim->Instance, input_channel, TIM_CCx_ENABLE);

  /* 使能主输出 */
  __HAL_TIM_MOE_ENABLE(htim);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  在互补输出通道上停止 TIM 单脉冲信号生成。
  * @note OutputChannel 必须与调用 @ref HAL_TIM_OnePulse_ConfigChannel() 时所选的脉冲输出通道匹配。
  * @param  htim TIM 单脉冲句柄
  * @param  OutputChannel 要禁用的脉冲输出通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Stop(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  uint32_t input_channel = (OutputChannel == TIM_CHANNEL_1) ? TIM_CHANNEL_2 : TIM_CHANNEL_1;

  /* 检查参数 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, OutputChannel));

  /* 禁用互补单脉冲输出通道和输入捕获通道 */
  TIM_CCxNChannelCmd(htim->Instance, OutputChannel, TIM_CCxN_DISABLE);
  TIM_CCxChannelCmd(htim->Instance, input_channel, TIM_CCx_DISABLE);

  /* 禁用主输出 */
  __HAL_TIM_MOE_DISABLE(htim);

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
  * @brief  在互补通道上以中断模式启动 TIM 单脉冲信号生成。
  * @note OutputChannel 必须与调用 @ref HAL_TIM_OnePulse_ConfigChannel() 时选择的脉冲输出通道一致。
  * @param  htim TIM 单脉冲句柄
  * @param  OutputChannel 要使能的脉冲输出通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Start_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  uint32_t input_channel = (OutputChannel == TIM_CHANNEL_1) ? TIM_CHANNEL_2 : TIM_CHANNEL_1;
  HAL_TIM_ChannelStateTypeDef channel_1_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef channel_2_state = TIM_CHANNEL_STATE_GET(htim, TIM_CHANNEL_2);
  HAL_TIM_ChannelStateTypeDef complementary_channel_1_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_1);
  HAL_TIM_ChannelStateTypeDef complementary_channel_2_state = TIM_CHANNEL_N_STATE_GET(htim, TIM_CHANNEL_2);

  /* 检查参数 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, OutputChannel));

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

  /* 使能 TIM 捕获/比较 1 中断 */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);

  /* 使能 TIM 捕获/比较 2 中断 */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);

  /* 使能互补单脉冲输出通道和输入捕获通道 */
  TIM_CCxNChannelCmd(htim->Instance, OutputChannel, TIM_CCxN_ENABLE);
  TIM_CCxChannelCmd(htim->Instance, input_channel, TIM_CCx_ENABLE);

  /* 使能主输出 */
  __HAL_TIM_MOE_ENABLE(htim);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  在互补通道上以中断模式停止 TIM 单脉冲信号生成。
  * @note OutputChannel 必须与调用 @ref HAL_TIM_OnePulse_ConfigChannel() 时选择的脉冲输出通道一致。
  * @param  htim TIM 单脉冲句柄
  * @param  OutputChannel 要禁用的脉冲输出通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  uint32_t input_channel = (OutputChannel == TIM_CHANNEL_1) ? TIM_CHANNEL_2 : TIM_CHANNEL_1;

  /* 检查参数 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, OutputChannel));

  /* 禁用 TIM 捕获/比较 1 中断 */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);

  /* 禁用 TIM 捕获/比较 2 中断 */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);

  /* 禁用互补单脉冲输出通道和输入捕获通道 */
  TIM_CCxNChannelCmd(htim->Instance, OutputChannel, TIM_CCxN_DISABLE);
  TIM_CCxChannelCmd(htim->Instance, input_channel, TIM_CCx_DISABLE);

  /* 禁用主输出 */
  __HAL_TIM_MOE_DISABLE(htim);

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

/** @defgroup TIMEx_Exported_Functions_Group5 扩展外设控制函数
  * @brief    外设控制函数
  *
@verbatim
  ==============================================================================
                    ##### 外设控制函数 #####
  ==============================================================================
  [..]
    本节提供的函数支持：
      (+) 在使用霍尔传感器接口时配置换相事件。
      (+) 配置 OC 和 PWM 模式的输出通道。

      (+) 配置互补通道、刹车特性和死区时间。
      (+) 配置主从同步。
      (+) 配置定时器重映射功能。

@endverbatim
  * @{
  */

/**
  * @brief  配置 TIM 换相事件时序。
  * @note   若要使用换相事件，必须调用此函数，以便在每次定时器的 TRGI 输入检测到换相时更新配置。
  *         该特性的典型用法是配合另一个配置为霍尔传感器接口的定时器（接口定时器）使用，
  *         每当接口定时器的 TI1 输入端检测到换相时，该接口定时器就会在其 TRGO 输出端（连接到当前函数使用的定时器）生成换相信号。
  * @param  htim TIM 句柄
  * @param  InputTrigger 与霍尔传感器接口定时器相对应的内部触发源
  *          该参数可以是以下值之一：
  *            @arg TIM_TS_ITR0: 选择内部触发 0
  *            @arg TIM_TS_ITR1: 选择内部触发 1
  *            @arg TIM_TS_ITR2: 选择内部触发 2
  *            @arg TIM_TS_ITR3: 选择内部触发 3
  *            @arg TIM_TS_NONE: 无需触发源
  * @param  CommutationSource 换相事件源
  *          该参数可以是以下值之一：
  *            @arg TIM_COMMUTATION_TRGI: 换相源为接口定时器的 TRGI
  *            @arg TIM_COMMUTATION_SOFTWARE: 换相源由软件通过 COMG 位置位触发
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutEvent(TIM_HandleTypeDef *htim, uint32_t  InputTrigger,
                                              uint32_t  CommutationSource)
{
  /* 检查参数 */
  assert_param(IS_TIM_COMMUTATION_EVENT_INSTANCE(htim->Instance));
  assert_param(IS_TIM_INTERNAL_TRIGGEREVENT_SELECTION(InputTrigger));

  __HAL_LOCK(htim);

  if ((InputTrigger == TIM_TS_ITR0) || (InputTrigger == TIM_TS_ITR1) ||
      (InputTrigger == TIM_TS_ITR2) || (InputTrigger == TIM_TS_ITR3))
  {
    /* 选择输入触发源 */
    htim->Instance->SMCR &= ~TIM_SMCR_TS;
    htim->Instance->SMCR |= InputTrigger;
  }

  /* 选择捕获比较预装载功能 */
  htim->Instance->CR2 |= TIM_CR2_CCPC;
  /* 选择换相事件源 */
  htim->Instance->CR2 &= ~TIM_CR2_CCUS;
  htim->Instance->CR2 |= CommutationSource;

  /* 禁用换相中断 */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_COM);

  /* 禁用换相 DMA 请求 */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_COM);

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  配置 TIM 换相事件时序并使能中断。
  * @note   若要使用换相事件，必须调用此函数，以便在每次定时器的 TRGI 输入检测到换相时更新配置。
  *         该特性的典型用法是配合另一个配置为霍尔传感器接口的定时器（接口定时器）使用，
  *         每当接口定时器的 TI1 输入端检测到换相时，该接口定时器就会在其 TRGO 输出端（连接到当前函数使用的定时器）生成换相信号。
  * @param  htim TIM 句柄
  * @param  InputTrigger 与霍尔传感器接口定时器相对应的内部触发源
  *          该参数可以是以下值之一：
  *            @arg TIM_TS_ITR0: 选择内部触发 0
  *            @arg TIM_TS_ITR1: 选择内部触发 1
  *            @arg TIM_TS_ITR2: 选择内部触发 2
  *            @arg TIM_TS_ITR3: 选择内部触发 3
  *            @arg TIM_TS_NONE: 无需触发源
  * @param  CommutationSource 换相事件源
  *          该参数可以是以下值之一：
  *            @arg TIM_COMMUTATION_TRGI: 换相源为接口定时器的 TRGI
  *            @arg TIM_COMMUTATION_SOFTWARE: 换相源由软件通过 COMG 位置位触发
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutEvent_IT(TIM_HandleTypeDef *htim, uint32_t  InputTrigger,
                                                 uint32_t  CommutationSource)
{
  /* 检查参数 */
  assert_param(IS_TIM_COMMUTATION_EVENT_INSTANCE(htim->Instance));
  assert_param(IS_TIM_INTERNAL_TRIGGEREVENT_SELECTION(InputTrigger));

  __HAL_LOCK(htim);

  if ((InputTrigger == TIM_TS_ITR0) || (InputTrigger == TIM_TS_ITR1) ||
      (InputTrigger == TIM_TS_ITR2) || (InputTrigger == TIM_TS_ITR3))
  {
    /* 选择输入触发源 */
    htim->Instance->SMCR &= ~TIM_SMCR_TS;
    htim->Instance->SMCR |= InputTrigger;
  }

  /* 选择捕获/比较预装载特性 */
  htim->Instance->CR2 |= TIM_CR2_CCPC;
  /* 选择换相事件源 */
  htim->Instance->CR2 &= ~TIM_CR2_CCUS;
  htim->Instance->CR2 |= CommutationSource;

  /* 禁用换相 DMA 请求 */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_COM);

  /* 使能换相中断 */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_COM);

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  配置带有 DMA 的 TIM 换相事件序列。
  * @note  若要使用换相事件，必须调用此函数，以便在每次定时器的 TRGI 输入端检测到换相时更新配置。
  *        该特性的典型应用是配合另一个配置为霍尔传感器接口的定时器（接口定时器）使用，
  *        每当该接口定时器的 TI1 输入端检测到换相时，该接口定时器都会在其 TRGO 输出端（连接到本函数所使用的定时器）生成换相信号。
  * @note  用户应在自己的软件中配置 DMA，本函数中仅置位 COMDE 位
  * @param  htim TIM 句柄
  * @param  InputTrigger 对应于与霍尔传感器接口的定时器的内部触发源
  *          该参数可以是以下值之一：
  *            @arg TIM_TS_ITR0: 选择内部触发 0
  *            @arg TIM_TS_ITR1: 选择内部触发 1
  *            @arg TIM_TS_ITR2: 选择内部触发 2
  *            @arg TIM_TS_ITR3: 选择内部触发 3
  *            @arg TIM_TS_NONE: 无需触发源
  * @param  CommutationSource 换相事件源
  *          该参数可以是以下值之一：
  *            @arg TIM_COMMUTATION_TRGI: 换相源为接口定时器的 TRGI
  *            @arg TIM_COMMUTATION_SOFTWARE:  换相源由软件通过 COMG 位置位生成
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutEvent_DMA(TIM_HandleTypeDef *htim, uint32_t  InputTrigger,
                                                  uint32_t  CommutationSource)
{
  /* 检查参数 */
  assert_param(IS_TIM_COMMUTATION_EVENT_INSTANCE(htim->Instance));
  assert_param(IS_TIM_INTERNAL_TRIGGEREVENT_SELECTION(InputTrigger));

  __HAL_LOCK(htim);

  if ((InputTrigger == TIM_TS_ITR0) || (InputTrigger == TIM_TS_ITR1) ||
      (InputTrigger == TIM_TS_ITR2) || (InputTrigger == TIM_TS_ITR3))
  {
    /* 选择输入触发源 */
    htim->Instance->SMCR &= ~TIM_SMCR_TS;
    htim->Instance->SMCR |= InputTrigger;
  }

  /* 选择捕获/比较预装载特性 */
  htim->Instance->CR2 |= TIM_CR2_CCPC;
  /* 选择换相事件源 */
  htim->Instance->CR2 &= ~TIM_CR2_CCUS;
  htim->Instance->CR2 |= CommutationSource;

  /* 使能换相 DMA 请求 */
  /* 设置 DMA 换相完成回调函数 */
  htim->hdma[TIM_DMA_ID_COMMUTATION]->XferCpltCallback = TIMEx_DMACommutationCplt;
  htim->hdma[TIM_DMA_ID_COMMUTATION]->XferHalfCpltCallback = TIMEx_DMACommutationHalfCplt;
  /* 设置 DMA 错误回调函数 */
  htim->hdma[TIM_DMA_ID_COMMUTATION]->XferErrorCallback = TIM_DMAError;

  /* 禁用换相中断 */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_COM);

  /* 使能换相 DMA 请求 */
  __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_COM);

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  将 TIM 配置为主模式。
  * @param  htim TIM 句柄。
  * @param  sMasterConfig 指向 TIM_MasterConfigTypeDef 结构的指针，该结构包含所选的触发输出 (TRGO) 和主/从模式。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef *htim,
                                                        const TIM_MasterConfigTypeDef *sMasterConfig)
{
  uint32_t tmpcr2;
  uint32_t tmpsmcr;

  /* 检查参数 */
  assert_param(IS_TIM_MASTER_INSTANCE(htim->Instance));
  assert_param(IS_TIM_TRGO_SOURCE(sMasterConfig->MasterOutputTrigger));
  assert_param(IS_TIM_MSM_STATE(sMasterConfig->MasterSlaveMode));

  /* 检查输入状态 */
  __HAL_LOCK(htim);

  /* 更改句柄状态 */
  htim->State = HAL_TIM_STATE_BUSY;

  /* 获取 TIMx CR2 寄存器的值 */
  tmpcr2 = htim->Instance->CR2;

  /* 获取 TIMx SMCR 寄存器的值 */
  tmpsmcr = htim->Instance->SMCR;

  /* 复位 MMS 位 */
  tmpcr2 &= ~TIM_CR2_MMS;
  /* 选择 TRGO 源 */
  tmpcr2 |=  sMasterConfig->MasterOutputTrigger;

  /* 更新 TIMx CR2 寄存器 */
  htim->Instance->CR2 = tmpcr2;

  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
  {
    /* 复位 MSM 位 */
    tmpsmcr &= ~TIM_SMCR_MSM;
    /* 设置主模式 */
    tmpsmcr |= sMasterConfig->MasterSlaveMode;

    /* 更新 TIMx SMCR 寄存器 */
    htim->Instance->SMCR = tmpsmcr;
  }

  /* 更改 htim 状态 */
  htim->State = HAL_TIM_STATE_READY;

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  配置刹车功能、死区时间、锁定级别、OSSI/OSSR 状态
  *         以及 AOE（自动输出使能）。
  * @param  htim TIM 句柄
  * @param  sBreakDeadTimeConfig 指向 TIM_ConfigBreakDeadConfigTypeDef 结构的指针，
  *         该结构包含 TIM 外设的 BDTR 寄存器配置信息。
  * @note   当在刹车输入、刹车 2 输入或系统刹车输入上检测到有效电平时，可生成中断。
  *         刹车中断可以通过调用 @ref __HAL_TIM_ENABLE_IT 宏来使能。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_ConfigBreakDeadTime(TIM_HandleTypeDef *htim,
                                                const TIM_BreakDeadTimeConfigTypeDef *sBreakDeadTimeConfig)
{
  /* 保持该变量初始化为 0，因为它用于配置 BDTR 寄存器 */
  uint32_t tmpbdtr = 0U;

  /* 检查参数 */
  assert_param(IS_TIM_BREAK_INSTANCE(htim->Instance));
  assert_param(IS_TIM_OSSR_STATE(sBreakDeadTimeConfig->OffStateRunMode));
  assert_param(IS_TIM_OSSI_STATE(sBreakDeadTimeConfig->OffStateIDLEMode));
  assert_param(IS_TIM_LOCK_LEVEL(sBreakDeadTimeConfig->LockLevel));
  assert_param(IS_TIM_DEADTIME(sBreakDeadTimeConfig->DeadTime));
  assert_param(IS_TIM_BREAK_STATE(sBreakDeadTimeConfig->BreakState));
  assert_param(IS_TIM_BREAK_POLARITY(sBreakDeadTimeConfig->BreakPolarity));
  assert_param(IS_TIM_AUTOMATIC_OUTPUT_STATE(sBreakDeadTimeConfig->AutomaticOutput));

  /* 检查输入状态 */
  __HAL_LOCK(htim);

  /* 设置锁定级别、刹车使能位和极性、OSSR 状态、
     OSSI 状态、死区时间值以及自动输出使能位 */

  /* 设置 BDTR 位 */
  MODIFY_REG(tmpbdtr, TIM_BDTR_DTG, sBreakDeadTimeConfig->DeadTime);
  MODIFY_REG(tmpbdtr, TIM_BDTR_LOCK, sBreakDeadTimeConfig->LockLevel);
  MODIFY_REG(tmpbdtr, TIM_BDTR_OSSI, sBreakDeadTimeConfig->OffStateIDLEMode);
  MODIFY_REG(tmpbdtr, TIM_BDTR_OSSR, sBreakDeadTimeConfig->OffStateRunMode);
  MODIFY_REG(tmpbdtr, TIM_BDTR_BKE, sBreakDeadTimeConfig->BreakState);
  MODIFY_REG(tmpbdtr, TIM_BDTR_BKP, sBreakDeadTimeConfig->BreakPolarity);
  MODIFY_REG(tmpbdtr, TIM_BDTR_AOE, sBreakDeadTimeConfig->AutomaticOutput);


  /* 设置 TIMx_BDTR */
  htim->Instance->BDTR = tmpbdtr;

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  配置 TIMx 输入重映射功能。
  * @param  htim TIM 句柄。
  * @param  Remap 指定 TIM 重映射源。
  *
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_TIMEx_RemapConfig(TIM_HandleTypeDef *htim, uint32_t Remap)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);
  UNUSED(Remap);

  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group6 扩展回调函数
  * @brief    扩展回调函数
  *
@verbatim
  ==============================================================================
                    ##### 扩展回调函数 #####
  ==============================================================================
  [..]
    本节提供扩展 TIM 回调函数：
    (+) 定时器换相（Commutation）回调
    (+) 定时器刹车（Break）回调

@endverbatim
  * @{
  */

/**
  * @brief  非阻塞模式下的换相回调函数
  * @param  htim TIM 句柄
  * @retval 无
  */
__weak void HAL_TIMEx_CommutCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数；当需要该回调时，
            可以在用户文件中实现 HAL_TIMEx_CommutCallback
   */
}
/**
  * @brief  非阻塞模式下的换相半完成回调函数
  * @param  htim TIM 句柄
  * @retval 无
  */
__weak void HAL_TIMEx_CommutHalfCpltCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(htim);

  /* 注意：不应修改此函数；当需要该回调时，
            可以在用户文件中实现 HAL_TIMEx_CommutHalfCpltCallback
   */
}

/**
  * @brief  非阻塞模式下的刹车检测回调函数
  * @param  htim TIM 句柄
  * @retval 无
  */
__weak void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim)
{
  /* 防止未使用参数产生的编译警告 */
  UNUSED(htim);

  /* 注意：请勿修改此函数；当需要回调时，
            可在用户文件中实现 HAL_TIMEx_BreakCallback
   */
}
/**
  * @}
  */

/** @defgroup TIMEx_Exported_Functions_Group7 扩展外设状态函数
  * @brief    扩展外设状态函数
  *
@verbatim
  ==============================================================================
                ##### 扩展外设状态函数 #####
  ==============================================================================
  [..]
    本小节允许在运行时获取外设状态及数据流状态。

@endverbatim
  * @{
  */

/**
  * @brief  返回 TIM 霍尔传感器接口句柄的状态。
  * @param  htim TIM 霍尔传感器句柄
  * @retval HAL 状态
  */
HAL_TIM_StateTypeDef HAL_TIMEx_HallSensor_GetState(const TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @brief  返回 TIM 互补通道的实际状态。
  * @param  htim TIM 句柄
  * @param  ChannelN TIM 互补通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @retval TIM 互补通道状态
  */
HAL_TIM_ChannelStateTypeDef HAL_TIMEx_GetChannelNState(const TIM_HandleTypeDef *htim,  uint32_t ChannelN)
{
  HAL_TIM_ChannelStateTypeDef channel_state;

  /* 检查参数 */
  assert_param(IS_TIM_CCXN_INSTANCE(htim->Instance, ChannelN));

  channel_state = TIM_CHANNEL_N_STATE_GET(htim, ChannelN);

  return channel_state;
}
/**
  * @}
  */

/**
  * @}
  */

/* 私有函数 ---------------------------------------------------------*/
/** @defgroup TIMEx_Private_Functions TIM 扩展私有函数
  * @{
  */

/**
  * @brief  TIM DMA 换相回调函数。
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
void TIMEx_DMACommutationCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  /* 改变 htim 状态 */
  htim->State = HAL_TIM_STATE_READY;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  htim->CommutationCallback(htim);
#else
  HAL_TIMEx_CommutCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}

/**
  * @brief  TIM DMA 换相半传输完成回调函数。
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
void TIMEx_DMACommutationHalfCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  /* 改变 htim 状态 */
  htim->State = HAL_TIM_STATE_READY;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  htim->CommutationHalfCpltCallback(htim);
#else
  HAL_TIMEx_CommutHalfCpltCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}


/**
  * @brief  TIM DMA 延迟脉冲完成回调函数（互补通道）。
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
static void TIM_DMADelayPulseNCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;

    if (hdma->Init.Mode == DMA_NORMAL)
    {
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
    }
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;

    if (hdma->Init.Mode == DMA_NORMAL)
    {
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
    }
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;

    if (hdma->Init.Mode == DMA_NORMAL)
    {
      TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_READY);
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
  * @brief  TIM DMA 错误回调函数（互补通道）
  * @param  hdma 指向 DMA 句柄的指针。
  * @retval 无
  */
static void TIM_DMAErrorCCxN(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  if (hdma == htim->hdma[TIM_DMA_ID_CC1])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_1, HAL_TIM_CHANNEL_STATE_READY);
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_2, HAL_TIM_CHANNEL_STATE_READY);
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;
    TIM_CHANNEL_N_STATE_SET(htim, TIM_CHANNEL_3, HAL_TIM_CHANNEL_STATE_READY);
  }
  else
  {
    /* 无需操作 */
  }

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  htim->ErrorCallback(htim);
#else
  HAL_TIM_ErrorCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
}

/**
  * @brief  使能或禁用 TIM 捕获/比较通道 xN。
  * @param  TIMx 选择 TIM 外设
  * @param  Channel 指定 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选择 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选择 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选择 TIM 通道 3
  * @param  ChannelNState 指定 TIM 通道 CCxNE 位的最新状态。
  *          该参数可以是：TIM_CCxN_ENABLE 或 TIM_CCxN_Disable。
  * @retval 无
  */
static void TIM_CCxNChannelCmd(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t ChannelNState)
{
  uint32_t tmp;

  tmp = TIM_CCER_CC1NE << (Channel & 0xFU); /* 0xFU = 最大偏移 15 位 */

  /* 复位 CCxNE 位 */
  TIMx->CCER &=  ~tmp;

  /* 置位或复位 CCxNE 位 */
  TIMx->CCER |= (uint32_t)(ChannelNState << (Channel & 0xFU)); /* 0xFU = 最大偏移 15 位 */
}
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

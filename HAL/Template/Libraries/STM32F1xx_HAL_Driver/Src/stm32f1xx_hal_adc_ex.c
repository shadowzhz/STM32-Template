/**
  ******************************************************************************
  * @file    stm32f1xx_hal_adc_ex.c
  * @author  MCD Application Team
  * @brief   本文件提供固件函数，用于管理模数转换器 (ADC)
  *          外设的以下功能：
  *           + 外设控制功能
  *          其他函数（通用函数）可在文件
  *          "stm32f1xx_hal_adc.c" 中找到。
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * 本软件根据可在本软件组件根目录下的 LICENSE 文件中找到的条款进行许可。
  * 如果本软件未附带 LICENSE 文件，则按“原样 (AS-IS)”提供。
  *
  ******************************************************************************
  @verbatim
  [..] 
  (@) “ADC 外设特性”和“如何使用此驱动程序”章节
      可在通用函数文件 "stm32f1xx_hal_adc.c" 中找到。
  [..]
  @endverbatim
  */

/* 头文件包含 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup ADCEx ADCEx
  * @brief ADC 扩展 HAL 模块驱动程序
  * @{
  */

#ifdef HAL_ADC_MODULE_ENABLED

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/** @defgroup ADCEx_Private_Constants ADCEx 私有常量
  * @{
  */

  /* ADC 校准延时：                                                           */
  /* 启动校准前的硬件先决条件：ADC 必须已处于上电状态                         */
  /* 至少持续两个 ADC 时钟周期。                                              */
  /* 单位：ADC 时钟周期                                                       */
  #define ADC_PRECALIBRATION_DELAY_ADCCLOCKCYCLES       2U

  /* ADC 校准超时时间                                                         */
  /* 该值设定为高于最坏情况：低时钟频率、                                     */
  /* 最大预分频比。                                                           */
  /* 低频配置示例：时钟源为 0.1 MHz，ADC 时钟                                 */
  /* 预分频器为 4，采样时间为 12.5 个 ADC 时钟周期，分辨率为 12 位。    */
  /* 单位：ms                                                                 */
  #define ADC_CALIBRATION_TIMEOUT          10U

  /* 温度传感器稳定时间延迟。                                                 */
  /* 最大延迟为 10us（请参阅器件数据手册中的参数 tSTART）。                   */
  /* 单位：us                                                                 */
  #define ADC_TEMPSENSOR_DELAY_US         10U

/**
  * @}
  */

/* 私有宏 -------------------------------------------------------------------*/
/* 私有变量 -----------------------------------------------------------------*/
/* 私有函数原型 -------------------------------------------------------------*/
/* 私有函数 -----------------------------------------------------------------*/

/** @defgroup ADCEx_Exported_Functions ADCEx 导出函数
  * @{
  */

/** @defgroup ADCEx_Exported_Functions_Group1 扩展 IO 操作函数
 *  @brief    扩展输入和输出操作函数
 *
@verbatim    
 ===============================================================================
                      ##### IO 操作函数 #####
 ===============================================================================
    [..]  本节提供了实现以下功能的函数：
      (+) 启动注入组转换。
      (+) 停止注入组转换。
      (+) 轮询注入组转换完成状态。
      (+) 获取注入通道转换结果。
      (+) 启动注入组转换并使能中断。
      (+) 停止注入组转换并禁用中断。

      (+) 启动多重模式并使能 DMA 传输。
      (+) 停止多重模式并禁用 ADC DMA 传输。
      (+) 获取多重模式转换结果。

      (+) 执行单端或差分输入的 ADC 自校准。
      (+) 获取单端或差分输入的校准因子。
      (+) 设置单端或差分输入的校准因子。
      
@endverbatim
  * @{
  */

/**
  * @brief  执行 ADC 自动自校准
  *         校准前提条件：必须禁用 ADC（在调用 HAL_ADC_Start() 之前或调用
  *         HAL_ADC_Stop() 之后执行此函数）。
  *         在校准过程中，ADC 将被使能。该函数执行完成后，ADC 将保持使能状态。
  * @param  hadc: ADC 句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  uint32_t tickstart;
  __IO uint32_t wait_loop_index = 0U;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* 进程已锁定 */
  __HAL_LOCK(hadc);
    
   /* 1. 禁用 ADC 外设                                                          */
   tmp_hal_status = ADC_ConversionStop_Disable(hadc);
  
   /* 2. 开始校准前的校准前置延时。                                             */
   /*    - ADC 必须至少使能两个 ADC 时钟周期                                     */
   tmp_hal_status = ADC_Enable(hadc);

   /* 检查 ADC 是否已实际使能 */
   if (tmp_hal_status == HAL_OK)
   {
     /* 设置 ADC 状态 */
     ADC_STATE_CLR_SET(hadc->State,
                       HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                       HAL_ADC_STATE_BUSY_INTERNAL);
    
    /* 硬件前置条件：开始校准前的延时。                                          */
    /*  - 计算与 ADC 时钟周期对应的 CPU 时钟周期数。                              */
    /*  - 等待预期的 ADC 时钟周期延时 */
    wait_loop_index = ((SystemCoreClock
                        / HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_ADC))
                       * ADC_PRECALIBRATION_DELAY_ADCCLOCKCYCLES        );

    while(wait_loop_index != 0U)
    {
      wait_loop_index--;
    }

    /* 3. 复位 ADC 校准寄存器 */  
    SET_BIT(hadc->Instance->CR2, ADC_CR2_RSTCAL);
    
    tickstart = HAL_GetTick();  

    /* 等待校准复位完成 */
    while(HAL_IS_BIT_SET(hadc->Instance->CR2, ADC_CR2_RSTCAL))
    {
      if((HAL_GetTick() - tickstart) > ADC_CALIBRATION_TIMEOUT)
      {
        /* 新增检查项，以避免在发生任务抢占时误判为超时 */
        if(HAL_IS_BIT_SET(hadc->Instance->CR2, ADC_CR2_RSTCAL))
        {
          /* 将 ADC 状态机更新为错误状态 */
          ADC_STATE_CLR_SET(hadc->State,
                            HAL_ADC_STATE_BUSY_INTERNAL,
                            HAL_ADC_STATE_ERROR_INTERNAL);

          /* 进程解锁 */
          __HAL_UNLOCK(hadc);

          return HAL_ERROR;
        }
      }
    }
    
    /* 4. 启动 ADC 校准 */
    SET_BIT(hadc->Instance->CR2, ADC_CR2_CAL);
    
    tickstart = HAL_GetTick();  

    /* 等待校准完成 */
    while(HAL_IS_BIT_SET(hadc->Instance->CR2, ADC_CR2_CAL))
    {
      if((HAL_GetTick() - tickstart) > ADC_CALIBRATION_TIMEOUT)
      {
        /* 新增检查以避免发生抢占时出现误判超时 */
        if(HAL_IS_BIT_SET(hadc->Instance->CR2, ADC_CR2_CAL))
        {
          /* 将 ADC 状态机更新为错误状态 */
          ADC_STATE_CLR_SET(hadc->State,
                            HAL_ADC_STATE_BUSY_INTERNAL,
                            HAL_ADC_STATE_ERROR_INTERNAL);

          /* 解锁进程 */
          __HAL_UNLOCK(hadc);

          return HAL_ERROR;
        }
      }
    }
    
    /* 设置 ADC 状态 */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_BUSY_INTERNAL,
                      HAL_ADC_STATE_READY);
  }
  
  /* 解锁进程 */
  __HAL_UNLOCK(hadc);
  
  /* 返回函数状态 */
  return tmp_hal_status;
}

/**
  * @brief  使能 ADC，启动注入组转换。
  *         此函数中使能的中断：无。
  * @param  hadc: ADC 句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADCEx_InjectedStart(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* 检查参数有效性 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* 锁定进程 */
  __HAL_LOCK(hadc);
    
  /* 使能 ADC 外设 */
  tmp_hal_status = ADC_Enable(hadc);
  
  /* 若 ADC 已有效使能，则启动转换 */
  if (tmp_hal_status == HAL_OK)
  {
    /* 设置 ADC 状态                                                          */
    /* - 清除与注入组转换结果相关的状态位域                                   */
    /* - 设置与注入操作相关的状态位域                                         */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_READY | HAL_ADC_STATE_INJ_EOC,
                      HAL_ADC_STATE_INJ_BUSY);
    
    /* 独立模式或多重模式的情况（适用于具有多个 ADC 的器件）：               */
    /* 设置多重模式状态。                                                     */
    if (ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc))
    {
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
    }
    else
    {
      SET_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
    }
    
    /* 检查规则通道转换是否正在进行 */
    /* 注意：在该器件上，没有仅与注入组转换相关的 ADC 错误代码字段。           */
    /*       如果规则组的转换正在进行，则不会复位任何错误代码。                */
    /*                                                                         */
    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_REG_BUSY))
    {
      /* 复位所有 ADC 错误代码字段 */
      ADC_CLEAR_ERRORCODE(hadc);
    }
    
    /* 进程已解锁 */
    /* 在启动 ADC 转换前解锁：以防发生潜在的中断，                             */
    /* 从而允许处理流程进入 ADC IRQ 中断服务函数。                             */
    __HAL_UNLOCK(hadc);
    
    /* 清除注入组转换标志 */
    /* （以确保消除先前潜在 ADC 操作所遗留的未知状态）                         */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_JEOC);
        
    /* 使能注入组转换。                                                       */
    /* 如果选择了软件触发启动，转换将立即开始。                               */
    /* 如果选择了外部触发，转换将在下一个触发事件到达时开始。                 */
    /*                                                                         */
    /* 如果使能了注入通道自动转换，转换将在                                    */
    /* 下一次规则组转换完成后启动。                                            */
    /* 使能多重模式的情况（适用于具有多个 ADC 的器件）：如果 ADC 为            */
    /* 从机，则仅使能 ADC（不启动转换）。如果 ADC 为                          */
    /* 主机，则使能 ADC 并启动转换。                                          */
    if (HAL_IS_BIT_CLR(hadc->Instance->CR1, ADC_CR1_JAUTO))
    {
      if (ADC_IS_SOFTWARE_START_INJECTED(hadc)     &&
          ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc)  )
      {
        /* 通过软件触发启动注入组的 ADC 转换 */
        SET_BIT(hadc->Instance->CR2, (ADC_CR2_JSWSTART | ADC_CR2_JEXTTRIG));
      }
      else
      {
        /* 通过外部触发启动注入组的 ADC 转换 */
        SET_BIT(hadc->Instance->CR2, ADC_CR2_JEXTTRIG);
      }
    }
  }
  else
  {
    /* 进程解锁 */
    __HAL_UNLOCK(hadc);
  }
  
  /* 返回函数状态 */
  return tmp_hal_status;
}

/**
  * @brief  停止注入通道的转换。如果当前没有规则组转换在进行，
  *         则禁用 ADC 外设。
  * @note   如果必须禁用 ADC 且规则组上正在进行转换，
  *         必须使用函数 HAL_ADC_Stop 来同时停止
  *         注入组和规则组，并禁用 ADC。
  * @note   如果使能了注入组的自动注入模式，
  *         必须使用函数 HAL_ADC_Stop。
  * @note   在自动注入模式下，必须使用 HAL_ADC_Stop。
  * @param  hadc: ADC 句柄
  * @retval 无
  */
HAL_StatusTypeDef HAL_ADCEx_InjectedStop(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* 进程加锁 */
  __HAL_LOCK(hadc);
    
  /* 停止潜在的转换并禁用 ADC 外设                                           */
  /* 条件为：                                                                */
  /* - 另一个组（规则组）上没有转换计划                                      */
  /*   继续（注入组和规则组停止转换以及禁用 ADC 的操作是公用的）              */
  /*                                                                           */
  /* - 若处于自动注入模式，必须使用 HAL_ADC_Stop。                             */
  if(((hadc->State & HAL_ADC_STATE_REG_BUSY) == RESET)  &&
     HAL_IS_BIT_CLR(hadc->Instance->CR1, ADC_CR1_JAUTO)   )
  {
    /* 停止规则组和注入组上可能正在进行的转换 */
    /* 禁用 ADC 外设 */
    tmp_hal_status = ADC_ConversionStop_Disable(hadc);
    
    /* 检查 ADC 是否已有效禁用 */
    if (tmp_hal_status == HAL_OK)
    {
      /* 设置 ADC 状态 */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                        HAL_ADC_STATE_READY);
    }
  }
  else
  {
    /* 将 ADC 状态机更新为错误状态 */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
      
    tmp_hal_status = HAL_ERROR;
  }
  
  /* 进程解锁 */
  __HAL_UNLOCK(hadc);
  
  /* 返回函数状态 */
  return tmp_hal_status;
}

/**
  * @brief  等待注入组转换完成。
  * @param  hadc: ADC 句柄
  * @param  Timeout: 超时时间（毫秒）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADCEx_InjectedPollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout)
{
  uint32_t tickstart;

  /* 使能扫描模式且对每次转换进行轮询时，用于轮询的变量                        */
  /*                                                                           */
  __IO uint32_t Conversion_Timeout_CPU_cycles = 0U;
  uint32_t Conversion_Timeout_CPU_cycles_max = 0U;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* 获取超时时间 */
  tickstart = HAL_GetTick();  
     
  /* 轮询转换结束：区分单次转换与序列转换。                                  */
  /*                                                                          */
  /* 对于注入组，标志位 JEOC 仅在序列结束时置位，                             */
  /* 而不会在序列中的每次转换完成时都置位。                                   */
  /*  - 若注入组为单次转换（禁用扫描模式或                                    */
  /*    InjectedNbrOfConversion == 1），则使用标志位 JEOC 来判定              */
  /*    转换是否完成。                                                        */
  /*  - 若注入组为序列转换（使能扫描模式且                                    */
  /*    InjectedNbrOfConversion >= 2），则标志位 JEOC 仅在序列结束时置位。    */
  /*                                                                          */
  /*    若要轮询每次转换，最大转换时间是根据                                  */
  /*    ADC 转换时间（所选采样时间 + 12.5 个 ADC 时钟周期的转换时间）         */
  /*    以及 APB2/ADC 时钟分频器计算得出的（取决于配置，                      */
  /*    转换时间范围可为 28 到 32256 个 CPU 周期）。                          */
  /*    由于每次转换后不会置位标志位 JEOC，因此无法                           */
  /*    被置位。                                                              */
  if ((hadc->Instance->JSQR & ADC_JSQR_JL) == RESET)
  {
    /* 等待转换结束（End of Conversion）标志位置位 */
    while(HAL_IS_BIT_CLR(hadc->Instance->SR, ADC_FLAG_JEOC))
    {
      /* 检查是否禁用了超时（设置为无限等待） */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0U) || ((HAL_GetTick() - tickstart ) > Timeout))
        {
          /* 新增检查以避免在发生抢占时产生误判超时 */
          if(HAL_IS_BIT_CLR(hadc->Instance->SR, ADC_FLAG_JEOC))
          {
            /* 将 ADC 状态机更新为超时状态 */
            SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);

            /* 进程解锁 */
            __HAL_UNLOCK(hadc);

            return HAL_TIMEOUT;
          }
        }
      }
    }
  }
  else
  {
    /* 将轮询替换为等待最大转换时间 */
    /*  - 计算与 ADC 时钟周期以及所有通道上 ADC 最大转换周期对应的 CPU 时钟周期  */
    /*    数。                                                                */
    /*  - 等待预期的 ADC 时钟周期延迟                                         */
    Conversion_Timeout_CPU_cycles_max = ((SystemCoreClock
                                          / HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_ADC))
                                         * ADC_CONVCYCLES_MAX_RANGE(hadc)                 );
    
    while(Conversion_Timeout_CPU_cycles < Conversion_Timeout_CPU_cycles_max)
    {
      /* 检查是否禁用了超时（设置为无限等待） */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0)||((HAL_GetTick() - tickstart ) > Timeout))
        {
          /* 新增检查以避免在发生抢占时产生误判超时 */
          if(Conversion_Timeout_CPU_cycles < Conversion_Timeout_CPU_cycles_max)
          {
            /* 将 ADC 状态机更新为超时状态 */
            SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);

            /* 进程解锁 */
            __HAL_UNLOCK(hadc);

            return HAL_TIMEOUT;
          }
        }
      }
      Conversion_Timeout_CPU_cycles ++;
    }
  }

  /* 清除注入组转换标志位 */
  /* 注意：在 STM32F1 ADC 上，清除同时置位的规则转换标志位                   */
  /* 。                                                                       */
  __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_JSTRT | ADC_FLAG_JEOC | ADC_FLAG_EOC);
  
  /* 更新 ADC 状态机 */
  SET_BIT(hadc->State, HAL_ADC_STATE_INJ_EOC);
  
  /* 判断注入组是否将进行下一次转换                                           */
  /* （无论是通过外部触发还是通过规则组的自动注入转换）。                     */
  /*                                                                          */
  if(ADC_IS_SOFTWARE_START_INJECTED(hadc)                     || 
     (HAL_IS_BIT_CLR(hadc->Instance->CR1, ADC_CR1_JAUTO) &&     
     (ADC_IS_SOFTWARE_START_REGULAR(hadc)        &&
      (hadc->Init.ContinuousConvMode == DISABLE)   )        )   )
  {
    /* 设置 ADC 状态 */
    CLEAR_BIT(hadc->State, HAL_ADC_STATE_INJ_BUSY);   
    
    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_REG_BUSY))
    {
      SET_BIT(hadc->State, HAL_ADC_STATE_READY);
    }
  }
  
  /* 返回 ADC 状态 */
  return HAL_OK;
}

/**
  * @brief  使能 ADC，并在中断模式下启动注入组转换。
  *          - JEOC（注入组转换完成）
  *         这些中断中每个都有其专用的回调函数。
  * @param  hadc: ADC 句柄
  * @retval HAL 状态。
  */
HAL_StatusTypeDef HAL_ADCEx_InjectedStart_IT(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* 进程加锁 */
  __HAL_LOCK(hadc);
    
  /* 使能 ADC 外设 */
  tmp_hal_status = ADC_Enable(hadc);
  
  /* 如果 ADC 已实际使能，则启动转换 */
  if (tmp_hal_status == HAL_OK)
  {
    /* 设置 ADC 状态                                                          */
    /* - 清除与注入组转换结果相关的状态位域                                   */
    /* - 设置与注入组操作相关的状态位域                                       */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_READY | HAL_ADC_STATE_INJ_EOC,
                      HAL_ADC_STATE_INJ_BUSY);
    
    /* 独立模式或多重模式（适用于具有多个 ADC 的器件）的情况：                */
    /* 设置多重模式状态。                                                     */
    if (ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc))
    {
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
    }
    else
    {
      SET_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
    }
    
    /* 检查规则组转换是否正在进行 */
    /* 注意：在该器件上，没有仅与注入组转换相关的 ADC 错误代码字段。           */
    /*       如果规则组的转换正在进行，则不会复位任何错误代码。               */
    /*                                                                         */
    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_REG_BUSY))
    {
      /* 复位所有 ADC 错误代码字段 */
      ADC_CLEAR_ERRORCODE(hadc);
    }
    
    /* 进程已解锁 */
    /* 在启动 ADC 转换前解锁：以防发生中断时，将控制权让给 ADC 中断处理程序。 */
    /*                                                                         */
    __HAL_UNLOCK(hadc);
    
    /* 清除注入组转换标志 */
    /* （以确保消除先前 ADC 操作可能带来的未知状态）                         */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_JEOC);
    
    /* 使能注入通道转换结束中断 */
    __HAL_ADC_ENABLE_IT(hadc, ADC_IT_JEOC);
    
    /* 如果已选择软件触发，则启动注入组转换 */
    /* 且自动注入转换已禁用。                                                  */
    /* 如果选择了外部触发，转换将在下一个                                      */
    /* 触发事件到来时启动。                                                    */
    /* 如果启用了自动注入转换，转换将在                                        */
    /* 下一次规则组转换完成后启动。                                            */
    if (HAL_IS_BIT_CLR(hadc->Instance->CR1, ADC_CR1_JAUTO))
    {
      if (ADC_IS_SOFTWARE_START_INJECTED(hadc)     &&
          ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc)  )
      {
        /* 通过软件触发启动注入组的 ADC 转换 */
        SET_BIT(hadc->Instance->CR2, (ADC_CR2_JSWSTART | ADC_CR2_JEXTTRIG));
      }
      else
      {
        /* 通过外部触发启动注入组的 ADC 转换 */
        SET_BIT(hadc->Instance->CR2, ADC_CR2_JEXTTRIG);
      }
    }
  }
  else
  {
    /* 解锁进程 */
    __HAL_UNLOCK(hadc);
  }
  
  /* 返回函数状态 */
  return tmp_hal_status;
}

/**
  * @brief  停止注入通道转换，禁用转换结束中断。
  *         若当前没有正在进行的规则转换，则禁用 ADC 外设。
  * @note   如果必须禁用 ADC 且规则组转换正在进行，
  *         则必须使用函数 HAL_ADC_Stop 同时停止注入组和规则组，
  *         并禁用 ADC。
  * @note   如果注入组使能了自动注入模式，
  *         则必须使用函数 HAL_ADC_Stop。
  * @param  hadc: ADC 句柄
  * @retval 无
  */
HAL_StatusTypeDef HAL_ADCEx_InjectedStop_IT(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* 上锁进程 */
  __HAL_LOCK(hadc);
    
  /* 停止潜在的转换并禁用 ADC 外设                                           */
  /* 取决于以下条件：                                                        */
  /* - 另一个组（规则组）没有转换计划                                        */
  /*   继续（注入组和规则组停止转换以及 ADC 禁用的操作是通用的）                  */
  /*                                                                           */
  /* - 若处于自动注入模式，则必须使用 HAL_ADC_Stop。                           */ 
  if(((hadc->State & HAL_ADC_STATE_REG_BUSY) == RESET)  &&
     HAL_IS_BIT_CLR(hadc->Instance->CR1, ADC_CR1_JAUTO)   )
  {
    /* 停止规则组和注入组上可能正在进行的转换 */
    /* 禁用 ADC 外设 */
    tmp_hal_status = ADC_ConversionStop_Disable(hadc);
    
    /* 检查 ADC 是否已真正禁用 */
    if (tmp_hal_status == HAL_OK)
    {
      /* 禁用注入通道的 ADC 转换完成中断 */
      __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOC);
      
      /* 设置 ADC 状态 */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                        HAL_ADC_STATE_READY);
    }
  }
  else
  {
    /* 更新 ADC 状态机为错误状态 */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
      
    tmp_hal_status = HAL_ERROR;
  }
  
  /* 进程解锁 */
  __HAL_UNLOCK(hadc);
  
  /* 返回函数状态 */
  return tmp_hal_status;
}

#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
/**
  * @brief  使能 ADC，启动规则组转换并通过 DMA 传输结果。
  *         必须预先使用 HAL_ADCEx_MultiModeConfigChannel() 函数
  *         配置多重模式。
  *         此函数中使能的中断：
  *          - DMA 传输完成中断
  *          - DMA 过半传输中断
  *         这些中断各具有专用的回调函数。
  * @note:  在 STM32F1 系列器件上，从 ADC 的规则组必须配置
  *         转换触发源为 ADC_SOFTWARE_START。
  * @note:  从 ADC 可以预先使用单模式的 HAL_ADC_Start() 函数使能。
  * @param  hadc: 主 ADC 的 ADC 句柄（切勿使用从 ADC 的句柄）
  * @param  pData: 目标缓冲区地址。
  * @param  Length: 从 ADC 外设传输至内存的数据长度。
  * @retval 无
  */
HAL_StatusTypeDef HAL_ADCEx_MultiModeStart_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  ADC_HandleTypeDef tmphadcSlave={0};

  /* 检查参数 */
  assert_param(IS_ADC_MULTIMODE_MASTER_INSTANCE(hadc->Instance));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
  
  /* 进程加锁 */
  __HAL_LOCK(hadc);

  /* 设置与主 ADC 关联的从 ADC 的临时句柄                                     */
  ADC_MULTI_SLAVE(hadc, &tmphadcSlave);
  
  /* 在 STM32F1 系列器件上，从 ADC 规则组必须配置为                             */
  /* 转换触发源 ADC_SOFTWARE_START。                                           */
  /* 注意：从 ADC 的外部触发必须使能，该操作已在                                */
  /*       函数 "HAL_ADC_Init()" 中完成。                                      */
  if(!ADC_IS_SOFTWARE_START_REGULAR(&tmphadcSlave))  
  {
    /* 将 ADC 状态机更新为错误状态 */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
    
    /* 进程解锁 */
    __HAL_UNLOCK(hadc);
    
    return HAL_ERROR;
  }
  
  /* 使能 ADC 外设：主 ADC 与从 ADC（以防先前尚未使能）                         */
  /*                                                                          */
  tmp_hal_status = ADC_Enable(hadc);
  if (tmp_hal_status == HAL_OK)
  {
    tmp_hal_status = ADC_Enable(&tmphadcSlave);
  }
  
  /* 若多重模式下的所有 ADC 均已有效使能，则启动转换 */
  if (tmp_hal_status == HAL_OK)
  {
    /* 设置 ADC 状态（主 ADC）                                                 */
    /* - 清除与规则组转换结果相关的状态位域                                      */
    /* - 置位与规则组运行相关的状态位域                                          */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_MULTIMODE_SLAVE,
                      HAL_ADC_STATE_REG_BUSY);
      
    /* 若规则组的转换同时触发了注入组转换，                                      */
    /* 则更新 ADC 状态。                                                        */
    if (READ_BIT(hadc->Instance->CR1, ADC_CR1_JAUTO) != RESET)
    {
      ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);  
    }
    
    /* 进程解锁 */
    /* 在启动 ADC 转换前解锁：以防发生潜在的中断时，                             */
    /* 能够将处理流程交由 ADC 中断处理函数（ADC IRQ Handler）。                 */
    __HAL_UNLOCK(hadc);
    
    /* 将 ADC 错误码清零（设置为无错误） */
    ADC_CLEAR_ERRORCODE(hadc);
    
    
    /* 设置 DMA 传输完成回调函数 */
    hadc->DMA_Handle->XferCpltCallback = ADC_DMAConvCplt;
       
    /* 设置 DMA 半传输完成回调函数 */
    hadc->DMA_Handle->XferHalfCpltCallback = ADC_DMAHalfConvCplt;
    
    /* 设置 DMA 错误回调函数 */
    hadc->DMA_Handle->XferErrorCallback = ADC_DMAError;

    
    /* 处理 ADC 和 DMA 的启动：ADC 过载中断、DMA 启动、ADC                      */
    /* 启动（若为软件触发启动）：                                             */
    
    /* 清除规则组转换标志位和过载（overrun）标志位 */
    /* （以确保消除先前潜在 ADC 操作可能遗留的未知状态） */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC);
    
    /* 使能主 ADC 的 ADC DMA 模式 */
    SET_BIT(hadc->Instance->CR2, ADC_CR2_DMA);
    
    /* 启动 DMA 通道 */
    HAL_DMA_Start_IT(hadc->DMA_Handle, (uint32_t)&hadc->Instance->DR, (uint32_t)pData, Length);
    
    /* 若选择了软件触发，则启动规则组转换。                                   */
    /* 若选择了外部触发，转换将在下一次                                       */
    /* 触发事件到来时启动。                                                   */
    /* 注意：单次转换的备选触发方式可以是强制再次置位 ADON 位          */
    /*       "hadc->Instance->CR2 |= ADC_CR2_ADON;"                         */
    if (ADC_IS_SOFTWARE_START_REGULAR(hadc))
    {
      /* 通过软件触发启动规则组的 ADC 转换 */
      SET_BIT(hadc->Instance->CR2, (ADC_CR2_SWSTART | ADC_CR2_EXTTRIG));
    }
    else
    {
      /* 通过外部触发启动规则组的 ADC 转换 */
      SET_BIT(hadc->Instance->CR2, ADC_CR2_EXTTRIG);
    }
  }
  else
  {
    /* 进程解锁 */
    __HAL_UNLOCK(hadc);
  }
  
  /* 返回函数状态 */
  return tmp_hal_status;
}

/**
  * @brief  停止规则组的 ADC 转换（在自动注入模式下还包括注入通道），
  *         禁用 ADC DMA 传输，禁用 ADC 外设。
  * @note   执行此函数后多重模式仍保持使能。若要禁用多重模式
  *         （通过 HAL_ADCEx_MultiModeConfigChannel() 设置），必须使用
  *         HAL_ADC_Init() 或 HAL_ADC_ReInit() 重新初始化 ADC。
  * @note   若 DMA 配置为循环模式，必须在此函数之后调用 HAL_ADC_Stop_DMA
  *         函数（传入从 ADC 句柄），以正确禁用 DMA 通道。
  * @param  hadc: 主 ADC 的 ADC 句柄（切勿使用从 ADC 的句柄）
  * @retval None
  */
HAL_StatusTypeDef HAL_ADCEx_MultiModeStop_DMA(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  ADC_HandleTypeDef tmphadcSlave={0};
  
  /* 检查参数 */
  assert_param(IS_ADC_MULTIMODE_MASTER_INSTANCE(hadc->Instance));
  
  /* 进程加锁 */
  __HAL_LOCK(hadc);

  /* 停止规则组和注入组上可能正在进行的转换 */
  /* 禁用主 ADC 外设 */
  tmp_hal_status = ADC_ConversionStop_Disable(hadc);
  
  /* 检查 ADC 是否已有效禁用 */
  if(tmp_hal_status == HAL_OK)
  {
    /* 设置与主 ADC 关联的从 ADC 临时句柄                                   */
    ADC_MULTI_SLAVE(hadc, &tmphadcSlave);

    /* 禁用从 ADC 外设 */
    tmp_hal_status = ADC_ConversionStop_Disable(&tmphadcSlave);

    /* 检查 ADC 是否已有效禁用 */
    if(tmp_hal_status != HAL_OK)
    {
      /* 将 ADC 状态机更新为错误状态 */
      SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

      /* 进程解锁 */
      __HAL_UNLOCK(hadc);

      return HAL_ERROR;
    }

    /* 禁用 ADC DMA 模式 */
    CLEAR_BIT(hadc->Instance->CR2, ADC_CR2_DMA);
    
    /* 复位双重模式下 ADC DMA 连续请求的配置 */
    CLEAR_BIT(hadc->Instance->CR1, ADC_CR1_DUALMOD);
        
    /* 禁用 DMA 通道（用于 DMA 处于循环模式，或在 */
    /* DMA 传输正在进行时停止的情况）                                        */
    tmp_hal_status = HAL_DMA_Abort(hadc->DMA_Handle);

    /* 更改 ADC 状态（主 ADC） */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                      HAL_ADC_STATE_READY);
  }
  
  /* 进程解锁 */
  __HAL_UNLOCK(hadc);
  
  /* 返回函数状态 */
  return tmp_hal_status;
}
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */

/**
  * @brief  获取 ADC 注入组转换结果。
  * @note   读取寄存器 JDRx 会自动清除 ADC 标志位 JEOC
  *         （ADC 注入组单次转换结束）。
  * @note   此函数不会清除 ADC 标志位 JEOS
  *         （ADC 注入组序列转换结束）
  *         标志位 JEOS 置位的时机：
  *          - 如果序列发生器仅由 1 个序列级别（rank）组成，标志位 JEOS 等效于
  *            标志位 JEOC。
  *          - 如果序列发生器由多个序列级别组成，在扫描序列
  *            进行期间仅置位标志位 JEOC，在扫描序列结束时
  *            标志位 JEOC 和 EOS 会同时置位。
  *         此函数不得清除标志位 JEOS，因为
  *         这会导致与低功耗特性不兼容
  *         （低功耗自动等待特性，并非所有 STM32 系列都支持）。
  *         若要清除此标志位，可使用以下函数：
  *         中断编程模型下：@ref HAL_ADC_IRQHandler()；轮询编程
  *         模型下：@ref HAL_ADCEx_InjectedPollForConversion()
  *         或 @ref __HAL_ADC_CLEAR_FLAG(&hadc, ADC_FLAG_JEOS)。
  * @param  hadc: ADC 句柄
  * @param  InjectedRank: 转换的 ADC 注入序列级别。
  *          该参数可以是以下值之一：
  *            @arg ADC_INJECTED_RANK_1: 选中注入通道 1
  *            @arg ADC_INJECTED_RANK_2: 选中注入通道 2
  *            @arg ADC_INJECTED_RANK_3: 选中注入通道 3
  *            @arg ADC_INJECTED_RANK_4: 选中注入通道 4
  * @retval ADC 注入组转换数据
  */
uint32_t HAL_ADCEx_InjectedGetValue(ADC_HandleTypeDef* hadc, uint32_t InjectedRank)
{
  uint32_t tmp_jdr = 0U;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_INJECTED_RANK(InjectedRank));
  
  /* 获取 ADC 转换值 */ 
  switch(InjectedRank)
  {  
    case ADC_INJECTED_RANK_4: 
      tmp_jdr = hadc->Instance->JDR4;
      break;
    case ADC_INJECTED_RANK_3: 
      tmp_jdr = hadc->Instance->JDR3;
      break;
    case ADC_INJECTED_RANK_2: 
      tmp_jdr = hadc->Instance->JDR2;
      break;
    case ADC_INJECTED_RANK_1:
    default:
      tmp_jdr = hadc->Instance->JDR1;
      break;
  }
  
  /* 返回 ADC 转换值 */ 
  return tmp_jdr;
}

#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
/**
  * @brief  在选定的多重模式下，返回最新的主从 ADC 规则转换结果数据。
  * @param  hadc: 主 ADC 的 ADC 句柄（不得使用从 ADC 的句柄）
  * @retval 转换后的数据值。
  */
uint32_t HAL_ADCEx_MultiModeGetValue(ADC_HandleTypeDef* hadc)
{
  uint32_t tmpDR = 0U;
  
  /* 检查参数 */
  assert_param(IS_ADC_MULTIMODE_MASTER_INSTANCE(hadc->Instance));
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* 注意：此处无需软件清除 EOC 标志，因为在读取数据寄存器 DR 时      */
  /*       硬件会自动将其清除。                                       */
  
  /* 在 STM32F1 系列器件上，仅当使能 ADC1 的 DMA 模式时，ADC1 数据寄存器 DR */
  /* 才会包含 ADC2 的转换结果。                                             */
  tmpDR = hadc->Instance->DR;

  if (HAL_IS_BIT_CLR(ADC1->CR2, ADC_CR2_DMA))
  {
    tmpDR |= (ADC2->DR << 16U);
  }
    
  /* 返回 ADC 转换值 */ 
  return tmpDR;
}
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */

/**
  * @brief  非阻塞模式下的注入组转换完成回调函数
  * @param  hadc: ADC 句柄
  * @retval 无
  */
__weak void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* 防止出现未引用的参数编译警告 */
  UNUSED(hadc);
  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_ADCEx_InjectedConvCpltCallback
  */
}

/**
  * @}
  */

/** @defgroup ADCEx_Exported_Functions_Group2 扩展外设控制函数
  * @brief    扩展外设控制函数
  *
@verbatim   
 ===============================================================================
             ##### 外设控制函数 #####
 ===============================================================================  
    [..]  本节提供的函数可用于：
      (+) 配置注入组通道
      (+) 配置双重/多重模式

@endverbatim
  * @{
  */

/**
  * @brief  配置 ADC 注入组以及关联到注入组的选定通道。
  * @note   支持动态更新参数：
  *         该函数初始化注入组，后续调用此函数可在不复位 ADC 的情况下，
  *         动态重新配置结构体 "ADC_InjectionConfTypeDef" 中的部分参数。
  *         这些参数的设置受 ADC 状态限制：
  *         必须在 ADC 未处于转换状态时调用此函数。
  * @param  hadc: ADC 句柄
  * @param  sConfigInjected: ADC 注入组及注入通道配置结构体。
  * @retval 无
  */
HAL_StatusTypeDef HAL_ADCEx_InjectedConfigChannel(ADC_HandleTypeDef* hadc, ADC_InjectionConfTypeDef* sConfigInjected)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  __IO uint32_t wait_loop_index = 0U;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CHANNEL(sConfigInjected->InjectedChannel));
  assert_param(IS_ADC_SAMPLE_TIME(sConfigInjected->InjectedSamplingTime));
  assert_param(IS_FUNCTIONAL_STATE(sConfigInjected->AutoInjectedConv));
  assert_param(IS_ADC_EXTTRIGINJEC(sConfigInjected->ExternalTrigInjecConv));
  assert_param(IS_ADC_RANGE(sConfigInjected->InjectedOffset));
  
  if(hadc->Init.ScanConvMode != ADC_SCAN_DISABLE)
  {
    assert_param(IS_ADC_INJECTED_RANK(sConfigInjected->InjectedRank));
    assert_param(IS_ADC_INJECTED_NB_CONV(sConfigInjected->InjectedNbrOfConversion));
    assert_param(IS_FUNCTIONAL_STATE(sConfigInjected->InjectedDiscontinuousConvMode));
  }
  
  /* 进程已锁定 */
  __HAL_LOCK(hadc);
  
  /* 注入组序列器的配置：                                                     */
  /* - 若扫描模式被禁用，注入通道序列长度设置为                               */
  /*   0x00：转换 1 个通道（规则排序 1 上的通道）                             */
  /*   参数 "InjectedNbrOfConversion" 将被忽略。                              */
  /*   注意：该器件在硬件上支持扫描模式，若将其                               */
  /*   禁用，会自动忽略转换通道数。无论如何，转换                             */
  /*   通道数仍被强制设为 0x00，以便在所有 STM32 器件间保持一致。              */
  /* - 若扫描模式被使能，注入通道序列长度设置为                               */
  /*   参数 "InjectedNbrOfConversion"。                                       */
  if (hadc->Init.ScanConvMode == ADC_SCAN_DISABLE)
  {
    if (sConfigInjected->InjectedRank == ADC_INJECTED_RANK_1)
    {
      /* 清除所有注入排序旧的 SQx 位 */
      MODIFY_REG(hadc->Instance->JSQR                             ,
                 ADC_JSQR_JL   |
                 ADC_JSQR_JSQ4 |
                 ADC_JSQR_JSQ3 |
                 ADC_JSQR_JSQ2 |
                 ADC_JSQR_JSQ1                                    ,
                 ADC_JSQR_RK_JL(sConfigInjected->InjectedChannel,
                                  ADC_INJECTED_RANK_1,
                                  0x01U));
    }
    /* 若本意欲配置除 rank1 以外的其他注入排序，但由于                         */
    /* ScanConvMode 被禁用而无法配置，则报告错误。                             */
    else
    {
      /* 更新 ADC 状态机为错误状态 */
      SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
      
      tmp_hal_status = HAL_ERROR;
    }
  }
  else
  {
    /* 由于注入通道排序转换顺序取决于注入通道总数                             */
    /* 注入转换，所选排序（rank）必须小于或等于要更新的注入转换总数。 */
    /*                                                                           */
    if (sConfigInjected->InjectedRank <= sConfigInjected->InjectedNbrOfConversion)
    {
      /* 清除所选排序的原 SQx 位 */
      /* 设置所选排序的 SQx 位 */
      MODIFY_REG(hadc->Instance->JSQR                                         ,
                 
                 ADC_JSQR_JL                                               |
                 ADC_JSQR_RK_JL(ADC_JSQR_JSQ1,                         
                                  sConfigInjected->InjectedRank,         
                                  sConfigInjected->InjectedNbrOfConversion)   ,
                 
                 ADC_JSQR_JL_SHIFT(sConfigInjected->InjectedNbrOfConversion) |
                 ADC_JSQR_RK_JL(sConfigInjected->InjectedChannel,      
                                  sConfigInjected->InjectedRank,         
                                  sConfigInjected->InjectedNbrOfConversion)    );
    }
    else
    {
      /* 清除所选排序的原 SQx 位 */
      MODIFY_REG(hadc->Instance->JSQR                                       ,
                 
                 ADC_JSQR_JL                                               |
                 ADC_JSQR_RK_JL(ADC_JSQR_JSQ1,                         
                                  sConfigInjected->InjectedRank,         
                                  sConfigInjected->InjectedNbrOfConversion) ,
                 
                 0x00000000U);
    }
  } 
    
  /* 注入组配置                                                               */
  /* 参数更新取决于 ADC 状态：                                                */
  /* 仅在 ADC 禁用时才可更新的参数：                                          */
  /*  - 启动转换的外部触发                                                    */
  /* 参数更新不受 ADC 状态限制：                                              */
  /*  - 自动注入转换                                                          */
  /*  - 注入间断模式                                                          */
  /* 注意：如果 ADC 已使能，修改 CR2 寄存器时应注意避免向 ADON 位写入 1，     */
  /*       从而触发非预期的转换。                                             */
  if (ADC_IS_ENABLE(hadc) == RESET)
  {    
    MODIFY_REG(hadc->Instance->CR2                                           ,
               ADC_CR2_JEXTSEL |
               ADC_CR2_ADON                                                  ,
               ADC_CFGR_JEXTSEL(hadc, sConfigInjected->ExternalTrigInjecConv) );
  }
  
  
  /* 注入组配置                                                               */
  /*  - 自动注入转换                                                         */
  /*  - 注入间断模式                                                         */
  
    /* 仅当注入组外部触发被禁用时，                                            */
    /* 才能使能自动注入转换。                                                  */
    if (sConfigInjected->AutoInjectedConv == ENABLE)
    {
      if (sConfigInjected->ExternalTrigInjecConv == ADC_INJECTED_SOFTWARE_START)
      {
        SET_BIT(hadc->Instance->CR1, ADC_CR1_JAUTO);
      }
      else
      {
        /* 更新 ADC 状态机为错误状态 */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
        
        tmp_hal_status = HAL_ERROR;
      }
    }
    
    /* 仅当自动注入模式被禁用时，                                              */
    /* 才能使能注入间断模式。                                                  */  
    if (sConfigInjected->InjectedDiscontinuousConvMode == ENABLE)
    {
      if (sConfigInjected->AutoInjectedConv == DISABLE)
      {
        SET_BIT(hadc->Instance->CR1, ADC_CR1_JDISCEN);
      } 
      else
      {
        /* 更新 ADC 状态机为错误状态 */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
        
        tmp_hal_status = HAL_ERROR;
      }
    }


  /* InjectedChannel 采样时间配置 */
  /* 适用于通道 10 到 17 */
  if (sConfigInjected->InjectedChannel >= ADC_CHANNEL_10)
  {
    MODIFY_REG(hadc->Instance->SMPR1                                                             ,
               ADC_SMPR1(ADC_SMPR1_SMP10, sConfigInjected->InjectedChannel)                      ,
               ADC_SMPR1(sConfigInjected->InjectedSamplingTime, sConfigInjected->InjectedChannel) );
  }
  else /* 适用于通道 0 到 9 */
  {
    MODIFY_REG(hadc->Instance->SMPR2                                                             ,
               ADC_SMPR2(ADC_SMPR2_SMP0, sConfigInjected->InjectedChannel)                       ,
               ADC_SMPR2(sConfigInjected->InjectedSamplingTime, sConfigInjected->InjectedChannel) );
  }
  
  /* 如果选择了 ADC1 InjectedChannel_16 或 InjectedChannel_17，使能内部温度传感器 */
  /* 和 VREFINT 测量通道。                                                    */
  if ((sConfigInjected->InjectedChannel == ADC_CHANNEL_TEMPSENSOR) ||
      (sConfigInjected->InjectedChannel == ADC_CHANNEL_VREFINT)      )
  {
    SET_BIT(hadc->Instance->CR2, ADC_CR2_TSVREFE);
  }
  
  
  /* 配置偏移量：使能/禁用偏移、InjectedChannel、偏移值 */
  switch(sConfigInjected->InjectedRank)
  {
    case 1:
      /* 设置注入通道 1 的偏移量 */
      MODIFY_REG(hadc->Instance->JOFR1,
                 ADC_JOFR1_JOFFSET1,
                 sConfigInjected->InjectedOffset);
      break;
    case 2:
      /* 设置注入通道 2 偏移量 */
      MODIFY_REG(hadc->Instance->JOFR2,
                 ADC_JOFR2_JOFFSET2,
                 sConfigInjected->InjectedOffset);
      break;
    case 3:
      /* 设置注入通道 3 偏移量 */
      MODIFY_REG(hadc->Instance->JOFR3,
                 ADC_JOFR3_JOFFSET3,
                 sConfigInjected->InjectedOffset);
      break;
    case 4:
    default:
      MODIFY_REG(hadc->Instance->JOFR4,
                 ADC_JOFR4_JOFFSET4,
                 sConfigInjected->InjectedOffset);
      break;
  }
  
  /* 若选择了 ADC1 通道 16 或通道 17，则使能温度传感器 */
  /* 和 VREFINT 测量通路。 */
  if ((sConfigInjected->InjectedChannel == ADC_CHANNEL_TEMPSENSOR) ||
      (sConfigInjected->InjectedChannel == ADC_CHANNEL_VREFINT)      )
  {
    /* 对于具有多个 ADC 的 STM32F1 器件：仅 ADC1 可以访问内部 */
    /* 测量通道（VrefInt/TempSensor）。若尝试在其他 ADC 实例上 */
    /* 配置这些通道，将报告错误。 */
    if (hadc->Instance == ADC1)
    {
      if (READ_BIT(hadc->Instance->CR2, ADC_CR2_TSVREFE) == RESET)
      {
        SET_BIT(hadc->Instance->CR2, ADC_CR2_TSVREFE);
        
        if ((sConfigInjected->InjectedChannel == ADC_CHANNEL_TEMPSENSOR))
        {
          /* 延时以等待温度传感器稳定时间 */
          /* 计算需要等待的 CPU 周期数 */
          wait_loop_index = (ADC_TEMPSENSOR_DELAY_US * (SystemCoreClock / 1000000U));
          while(wait_loop_index != 0U)
          {
            wait_loop_index--;
          }
        }
      }
    }
    else
    {
      /* 将 ADC 状态机更新为错误状态 */
      SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
      
      tmp_hal_status = HAL_ERROR;
    }
  }
  
  /* 进程解锁 */
  __HAL_UNLOCK(hadc);
  
  /* 返回函数状态 */
  return tmp_hal_status;
}

#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
/**
  * @brief  使能 ADC 多重模式并配置多重模式参数
  * @note   支持动态更新参数：
  *         该函数用于初始化多重模式参数，随后调用该函数可用于
  *         在运行过程中动态重配置结构体 "ADC_MultiModeTypeDef" 的部分参数，
  *         而无需复位各 ADC（共用组中的两个 ADC）。
  *         这些参数的设置受 ADC 状态约束。
  *         有关参数约束条件，请参阅结构体 "ADC_MultiModeTypeDef" 的注释说明。
  * @note   若要从多重模式切回单模式，必须对 ADC 进行复位
  *         （使用函数 HAL_ADC_Init() ）。
  * @param  hadc: ADC 句柄
  * @param  multimode: ADC 多重模式配置结构体
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADCEx_MultiModeConfigChannel(ADC_HandleTypeDef* hadc, ADC_MultiModeTypeDef* multimode)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  ADC_HandleTypeDef tmphadcSlave={0};
  
  /* 检查参数有效性 */
  assert_param(IS_ADC_MULTIMODE_MASTER_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_MODE(multimode->Mode));
  
  /* 进程上锁 */
  __HAL_LOCK(hadc);
  
  /* 设置与主 ADC 关联的从 ADC 的临时句柄     */
  ADC_MULTI_SLAVE(hadc, &tmphadcSlave);
  
  /* 参数更新取决于 ADC 状态：                              */
  /* 在 ADC 禁用或启用但规则组没有正在进行的转换时，可更新的参数： */
  /*                                    */
  /*  - 主 ADC 和从 ADC 的 DMA 配置                            */
  /* 仅在 ADC 禁用时可更新的参数：                */
  /*  - 双重/多重模式选择                                              */
  /* 为优化代码，当共用组的两个 ADC 都处于禁用状态时，可以配置所有多重模式设置。 */
  /*                                 */
  if ((ADC_IS_ENABLE(hadc) == RESET)                     &&
      (ADC_IS_ENABLE(&tmphadcSlave) == RESET)            &&
      (IS_ADC_MULTIMODE_MASTER_INSTANCE(hadc->Instance))   )
  {
    MODIFY_REG(hadc->Instance->CR1,
               ADC_CR1_DUALMOD    ,
               multimode->Mode     );
  }
  /* 如果共享同一共用组的其中一个 ADC 已使能，则不能对多重模式结构的任何参数进行更新。 */
  /*          */
  else
  {
    /* 将 ADC 状态机更新为错误状态 */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
    
    tmp_hal_status = HAL_ERROR;
  }
    
    
  /* 进程已解锁 */
  __HAL_UNLOCK(hadc);
  
  /* 返回函数状态 */
  return tmp_hal_status;
} 
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */
/**
  * @}
  */  

/**
  * @}
  */

#endif /* HAL_ADC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

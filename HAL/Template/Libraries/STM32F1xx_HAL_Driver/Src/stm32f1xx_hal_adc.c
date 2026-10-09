/**
  ******************************************************************************
  * @file    stm32f1xx_hal_adc.c
  * @author  MCD Application Team
  * @brief   本文件提供固件函数，用于管理模数转换器 (ADC)
  *          外设的以下功能：
  *           + 初始化和反初始化函数
  *           + 外设控制函数
  *           + 外设状态函数
  *          其他函数（扩展函数）可在文件
  *          "stm32f1xx_hal_adc_ex.c" 中找到。
  *
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
                     ##### ADC 外设特性 #####
  ==============================================================================
  [..]
  (+) 12 位分辨率

  (+) 规则转换结束、注入转换结束，以及发生模拟看门狗或溢出事件时生成中断。
  
  (+) 单次和连续转换模式。
  
  (+) 用于顺序转换多个通道的扫描模式。
  
  (+) 具有内置数据一致性保证的数据对齐。
  
  (+) 可编程采样时间（按通道配置）
  
  (+) 规则组和注入组的 ADC 转换。

  (+) 规则组和注入组均支持外部触发（定时器或 EXTI）。

  (+) 生成 DMA 请求用于传输规则组的转换数据。

  (+) 多重双重模式（适用于具有 2 个或更多 ADC 的器件）。
  
  (+) 多重双重模式下可配置的 DMA 数据存储（适用于具有
      2 个或更多 ADC 的器件）。
  
  (+) 双重交错模式下转换之间可配置的延迟（适用于具有
      2 个或更多 ADC 的器件）。
  
  (+) ADC 校准

  (+) ADC 供电要求：全速运行时为 2.4 V 至 3.6 V，低速运行时可低至 1.8 V。
  
  (+) ADC 输入范围：从 Vref-（连接到 Vssa）到 Vref+（连接到
      Vdda 或外部参考电压）。


                     ##### 如何使用本驱动程序 #####
  ==============================================================================
    [..]

     *** ADC 顶层相关参数配置 ***
     ============================================================
     [..]

    (#) 使能 ADC 接口
      (++) 作为前提条件，必须在 RCC 顶层配置 ADC 时钟。
           注意：在 STM32F1 上，ADC 最大时钟频率为 14MHz（请参阅
                 器件数据手册）。
                 因此，必须根据 ADC 时钟源频率配置 ADC 时钟预分频器，
                 以使其保持在此最大频率以下。
        (++) 必须进行一项时钟设置：
             ADC 时钟（核心时钟，也可能是转换时钟）。
             (+++) 示例：
                   在 HAL_ADC_MspInit() 中（推荐的代码位置）或与其他
                   器件时钟参数配置一起：
               (+++) RCC_PeriphCLKInitTypeDef  PeriphClkInit;
               (+++) __ADC1_CLK_ENABLE();
               (+++) PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
               (+++) PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
               (+++) HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

    (#) ADC 引脚配置
         (++) 使用宏 __HAL_RCC_GPIOx_CLK_ENABLE()
              使能 ADC GPIO 的时钟
         (++) 使用函数 HAL_GPIO_Init()
              将这些 ADC 引脚配置为模拟模式

    (#) （可选）在使用中断方式的 ADC 情况下：
         (++) 使用函数 HAL_NVIC_EnableIRQ(ADCx_IRQn)
              配置 ADC 的 NVIC
         (++) 将 ADC 中断处理函数 HAL_ADC_IRQHandler()
              插入到对应的 ADC 中断向量函数
              ADCx_IRQHandler() 中。

    (#) （可选）在使用 DMA 的情况下：
         (++) 使用函数 HAL_DMA_Init() 配置 DMA
              （DMA 通道、正常或循环模式等）。
         (++) 使用函数 HAL_NVIC_EnableIRQ(DMAx_Channelx_IRQn)
              配置 DMA 的 NVIC
         (++) 将 ADC 中断处理函数 HAL_ADC_IRQHandler()
              插入到对应的 DMA 中断向量函数
              DMAx_Channelx_IRQHandler() 中。

     *** ADC、规则/注入组、通道参数配置 ***
     ==========================================================================
     [..]

    (#) 使用函数 HAL_ADC_Init() 配置 ADC 参数（分辨率、数据对齐等）
        以及规则组参数（转换触发源、排序器等）。

    (#) 使用函数 HAL_ADC_ConfigChannel() 配置规则组的通道参数
        （通道号、在排序器中的通道排序等，配置到规则组中）。

    (#) （可选）使用函数 HAL_ADCEx_InjectedConfigChannel() 配置注入组参数
        （注入组的转换触发源、排序器等）
        以及注入组的通道参数（通道号、在排序器中的通道排序等，
        配置到注入组中）。

    (#) （可选）使用函数 HAL_ADC_AnalogWDGConfig() 配置模拟看门狗参数
        （监视的通道、阈值等）。

    (#) （可选）对于具有多个 ADC 实例的器件：使用函数
        HAL_ADCEx_MultiModeConfigChannel() 配置多重模式参数。

     *** 执行 ADC 转换 ***
     ====================================
     [..]

    (#) （可选）使用函数 HAL_ADCEx_Calibration_Start() 执行 ADC 自动校准，
        以提高转换精度。

    (#) ADC 驱动程序可在三种模式中使用：轮询、中断、DMA 传输。

        (++) 轮询方式的 ADC 转换：
          (+++) 使用函数 HAL_ADC_Start() 启动 ADC 外设并开始转换
          (+++) 使用函数 HAL_ADC_PollForConversion() 等待 ADC 转换完成
                （或者注入组使用：HAL_ADCEx_InjectedPollForConversion() ）
          (+++) 使用函数 HAL_ADC_GetValue() 获取转换结果
                （或者注入组使用：HAL_ADCEx_InjectedGetValue() ）
          (+++) 使用函数 HAL_ADC_Stop() 停止转换并禁用 ADC 外设

        (++) 中断方式的 ADC 转换：
          (+++) 使用函数 HAL_ADC_Start_IT() 启动 ADC 外设并开始转换
          (+++) 通过调用函数 HAL_ADC_ConvCpltCallback() 等待 ADC 转换完成
                （该函数必须在用户程序中实现）
                （或者注入组使用：HAL_ADCEx_InjectedConvCpltCallback() ）
          (+++) 使用函数 HAL_ADC_GetValue() 获取转换结果
                （或者注入组使用：HAL_ADCEx_InjectedGetValue() ）
          (+++) 使用函数 HAL_ADC_Stop_IT() 停止转换并禁用 ADC 外设

        (++) DMA 传输方式的 ADC 转换：
          (+++) 使用函数 HAL_ADC_Start_DMA() 启动 ADC 外设并开始转换
          (+++) 通过调用函数 HAL_ADC_ConvCpltCallback() 或 HAL_ADC_ConvHalfCpltCallback()
                等待 ADC 转换完成（这些函数必须在用户程序中实现）
          (+++) 转换结果由 DMA 自动传输到目标变量地址。
          (+++) 使用函数 HAL_ADC_Stop_DMA() 停止转换并禁用 ADC 外设

        (++) 针对具有多个 ADC 的器件：采用 DMA 传输的 ADC 多重模式转换：
          (+++) 使用函数 HAL_ADC_Start() 启动 ADC 外设（从机）并开始转换
          (+++) 使用函数 HAL_ADCEx_MultiModeStart_DMA() 启动 ADC 外设（主机）并开始转换
          (+++) 通过调用函数 HAL_ADC_ConvCpltCallback() 或 HAL_ADC_ConvHalfCpltCallback()
                等待 ADC 转换完成（这些函数必须在用户程序中实现）
          (+++) 转换结果由 DMA 自动传输到目标变量地址。
          (+++) 使用函数 HAL_ADCEx_MultiModeStop_DMA() 停止转换并禁用 ADC 外设（主机）
          (+++) 使用函数 HAL_ADC_Stop_IT() 停止转换并禁用 ADC 外设（从机）

     [..]

    (@) 回调函数必须在用户程序中实现：
      (+@) HAL_ADC_ErrorCallback()
      (+@) HAL_ADC_LevelOutOfWindowCallback()（模拟看门狗回调）
      (+@) HAL_ADC_ConvCpltCallback()
      (+@) HAL_ADC_ConvHalfCpltCallback
      (+@) HAL_ADCEx_InjectedConvCpltCallback()

     *** ADC 的反初始化 ***
     ============================================================
     [..]

    (#) 禁用 ADC 接口
      (++) 可以在 RCC 顶层对 ADC 时钟进行硬件复位和禁用。
        (++) 使用宏 __ADCx_FORCE_RESET()、__ADCx_RELEASE_RESET()
             硬件复位 ADC 外设。
        (++) 使用与配置步骤等效的宏/函数
             禁用 ADC 时钟。
             (+++) 示例：
                   在 HAL_ADC_MspDeInit() 中（推荐的代码位置）或与其他
                   器件时钟参数配置一起：
               (+++) PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC
               (+++) PeriphClkInit.AdcClockSelection = RCC_ADCPLLCLK2_OFF
               (+++) HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit)

    (#) ADC 引脚配置
         (++) 使用宏 __HAL_RCC_GPIOx_CLK_DISABLE()
              禁用 ADC GPIO 的时钟

    (#) （可选）在使用中断方式的 ADC 情况下：
         (++) 使用函数 HAL_NVIC_EnableIRQ(ADCx_IRQn)
              禁用 ADC 的 NVIC

    (#) （可选）在使用 DMA 的情况下：
         (++) 使用函数 HAL_DMA_Init()
              反初始化 DMA。
         (++) 使用函数 HAL_NVIC_EnableIRQ(DMAx_Channelx_IRQn)
              禁用 DMA 的 NVIC

    [..]
    
    *** 回调函数注册 ***
    =============================================
    [..]

     当编译宏 USE_HAL_ADC_REGISTER_CALLBACKS 置为 1 时，
     允许用户动态配置驱动程序的回调函数。
     使用函数 HAL_ADC_RegisterCallback()
     注册中断回调函数。
    [..]

     函数 HAL_ADC_RegisterCallback() 允许注册以下回调函数：
       (+) ConvCpltCallback               : ADC 转换完成回调函数
       (+) ConvHalfCpltCallback           : ADC 转换 DMA 半传输完成回调函数
       (+) LevelOutOfWindowCallback       : ADC 模拟看门狗 1 回调函数
       (+) ErrorCallback                  : ADC 错误回调函数
       (+) InjectedConvCpltCallback       : ADC 注入组转换完成回调函数
       (+) MspInitCallback                : ADC Msp 初始化回调函数
       (+) MspDeInitCallback              : ADC Msp 反初始化回调函数
     该函数接收 HAL 外设句柄、回调函数 ID
     以及指向用户回调函数的指针作为参数。
    [..]

     使用函数 HAL_ADC_UnRegisterCallback 将回调函数重置为默认的
     弱定义 (weak) 函数。
    [..]

     HAL_ADC_UnRegisterCallback 接收 HAL 外设句柄
     和回调函数 ID 作为参数。
     该函数允许重置以下回调函数：
       (+) ConvCpltCallback               : ADC 转换完成回调函数
       (+) ConvHalfCpltCallback           : ADC 转换 DMA 半传输完成回调函数
       (+) LevelOutOfWindowCallback       : ADC 模拟看门狗 1 回调函数
       (+) ErrorCallback                  : ADC 错误回调函数
       (+) InjectedConvCpltCallback       : ADC 注入组转换完成回调函数
       (+) MspInitCallback                : ADC Msp 初始化回调函数
       (+) MspDeInitCallback              : ADC Msp 反初始化回调函数
     [..]

     默认情况下，在调用 HAL_ADC_Init() 之后且当状态为 HAL_ADC_STATE_RESET 时，
     所有回调函数均被设置为对应的弱定义函数：
     例如 HAL_ADC_ConvCpltCallback()、HAL_ADC_ErrorCallback()。
     但 MspInit 和 MspDeInit 函数除外，仅当这些回调函数为空（事先未注册）时，
     它们才会在 HAL_ADC_Init()/HAL_ADC_DeInit() 中被重置为原有的弱定义函数。
    [..]

     如果 MspInit 或 MspDeInit 不为空，则无论何种状态，
     HAL_ADC_Init()/HAL_ADC_DeInit() 都会保留并使用用户（事先注册的）MspInit/MspDeInit 回调函数。
     [..]

     回调函数仅能在 HAL_ADC_STATE_READY 状态下注册/注销。
     MspInit/MspDeInit 函数除外，它们可以在 HAL_ADC_STATE_READY 或 HAL_ADC_STATE_RESET 状态下
     进行注册/注销，因此已注册的（用户）MspInit/DeInit 回调函数可在 Init/DeInit 期间使用。
    [..]

     因此，用户在调用 HAL_ADC_DeInit()
     或 HAL_ADC_Init() 函数之前，首先使用 HAL_ADC_RegisterCallback()
     注册 MspInit/MspDeInit 用户回调函数。
     [..]

     当编译宏 USE_HAL_ADC_REGISTER_CALLBACKS 置为 0 或
     未定义时，回调函数注册功能不可用，所有回调函数
     均被设置为对应的弱定义函数。
  
  @endverbatim
  */

/* 头文件 --------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup ADC ADC
  * @brief ADC HAL 模块驱动
  * @{
  */

#ifdef HAL_ADC_MODULE_ENABLED

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/** @defgroup ADC_Private_Constants ADC 私有常量
  * @{
  */

  /* ADC 使能和禁用建立时间的超时值。                                         */
  /* 定义的值高于最坏情况：低时钟频率、                                       */
  /* 最大预分频比。                                                           */
  /* 低频配置示例：时钟源为 0.1 MHz，ADC 时钟                                 */
  /* 预分频为 4，采样时间为 12.5 个 ADC 时钟周期，分辨率为 12 位。            */
  /* 单位：毫秒 (ms)                                                          */
  #define ADC_ENABLE_TIMEOUT              2U
  #define ADC_DISABLE_TIMEOUT             2U

  /* ADC 稳定时间的延迟。                                                     */
  /* 最大延迟为 1us（请参阅器件数据手册，参数 tSTAB）。                       */
  /* 单位：us                                                                 */
  #define ADC_STAB_DELAY_US               1U

  /* 温度传感器稳定时间延迟。                                                 */
  /* 最大延迟为 10us（请参阅器件数据手册中的参数 tSTART）。                   */
  /* 单位：us                                                                 */
  #define ADC_TEMPSENSOR_DELAY_US         10U

/**
  * @}
  */

/* 私有宏 --------------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 私有函数原型 --------------------------------------------------------------*/
/** @defgroup ADC_Private_Functions ADC 私有函数
  * @{
  */
/**
  * @}
  */

/* 导出函数 ------------------------------------------------------------------*/

/** @defgroup ADC_Exported_Functions ADC 导出函数
  * @{
  */

/** @defgroup ADC_Exported_Functions_Group1 初始化和反初始化函数 
  * @brief    初始化和配置函数
  *
@verbatim    
 ===============================================================================
              ##### 初始化和反初始化函数 #####
 ===============================================================================
    [..]  本节提供的函数用于：
      (+) 初始化并配置 ADC。
      (+) 反初始化 ADC。

@endverbatim
  * @{
  */

/**
  * @brief  根据 "ADC_InitTypeDef" 结构体中指定的参数，
  *         初始化 ADC 外设和规则组。
  * @note   作为前提条件，必须在 RCC 顶层配置 ADC 时钟
  *         （时钟源 APB2）。
  *         请参考下方注释的代码示例，可将其复制并取消注释后
  *         放入 HAL_ADC_MspInit() 中。
  * @note   支持动态更新参数：
  *         仅当处于 ADC 复位状态时，此函数才会初始化 ADC MSP (HAL_ADC_MspInit())。
  *         后续调用此函数可用于动态重新配置 ADC_InitTypeDef 
  *         结构体的部分参数，而无需修改 MSP 配置。如果需要再次修改 
  *         ADC MSP，必须在调用 HAL_ADC_Init() 之前先调用 HAL_ADC_DeInit()。
  *         这些参数的设置取决于 ADC 的状态。
  *         有关参数约束，请参阅结构体 "ADC_InitTypeDef" 的注释。
  * @note   此函数在两个作用域内配置 ADC：整个 ADC 作用域和规则组作用域。
  *         有关参数详情，请参阅结构体 "ADC_InitTypeDef" 的注释。
  * @param  hadc: ADC 句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  uint32_t tmp_cr1 = 0U;
  uint32_t tmp_cr2 = 0U;
  uint32_t tmp_sqr1 = 0U;
  
  /* 检查 ADC 句柄 */
  if(hadc == NULL)
  {
    return HAL_ERROR;
  }
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_DATA_ALIGN(hadc->Init.DataAlign));
  assert_param(IS_ADC_SCAN_MODE(hadc->Init.ScanConvMode));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
  assert_param(IS_ADC_EXTTRIG(hadc->Init.ExternalTrigConv));
  
  if(hadc->Init.ScanConvMode != ADC_SCAN_DISABLE)
  {
    assert_param(IS_ADC_REGULAR_NB_CONV(hadc->Init.NbrOfConversion));
    assert_param(IS_FUNCTIONAL_STATE(hadc->Init.DiscontinuousConvMode));
    if(hadc->Init.DiscontinuousConvMode != DISABLE)
    {
      assert_param(IS_ADC_REGULAR_DISCONT_NUMBER(hadc->Init.NbrOfDiscConversion));
    }
  }
  
  /* 作为前置条件，必须在 HAL_ADC_MspInit() 中将 ADC 时钟配置至    */
  /* RCC 顶层。                                                               */
  /* 有关时钟使能过程的更多详细信息，请参阅                                   */
  /* 本文件头部。                                                             */

  /* 仅当 ADC 处于复位状态时执行的操作：                                      */
  /* - 初始化 ADC MSP                                                         */
  if (hadc->State == HAL_ADC_STATE_RESET)
  {
    /* 初始化 ADC 错误代码 */
    ADC_CLEAR_ERRORCODE(hadc);
    
    /* 分配锁资源并进行初始化 */
    hadc->Lock = HAL_UNLOCKED;
    
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    /* 初始化 ADC 回调函数设置 */
    hadc->ConvCpltCallback              = HAL_ADC_ConvCpltCallback;                 /* 传统的弱回调函数 */
    hadc->ConvHalfCpltCallback          = HAL_ADC_ConvHalfCpltCallback;             /* 传统的弱回调函数 */
    hadc->LevelOutOfWindowCallback      = HAL_ADC_LevelOutOfWindowCallback;         /* 传统的弱回调函数 */
    hadc->ErrorCallback                 = HAL_ADC_ErrorCallback;                    /* 传统的弱回调函数 */
    hadc->InjectedConvCpltCallback      = HAL_ADCEx_InjectedConvCpltCallback;       /* 传统的弱回调函数 */
    
    if (hadc->MspInitCallback == NULL)
    {
      hadc->MspInitCallback = HAL_ADC_MspInit; /* 传统的弱定义 MspInit */
    }
    
    /* 初始化底层硬件 */
    hadc->MspInitCallback(hadc);
#else
    /* 初始化底层硬件 */
    HAL_ADC_MspInit(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
  }
  
  /* 停止规则组和注入组上可能正在进行的转换 */
  /* 禁用 ADC 外设 */
  /* 注意：在 ADC 已经使能的情况下，采取预防措施，防止在修改 CR2 寄存器时     */
  /*       因向 ADON 位写入 1 而启动意外的转换。                              */
  /*                                                                          */
  tmp_hal_status = ADC_ConversionStop_Disable(hadc);
  
  
  /* 若上述前置准备操作已正确完成，则配置 ADC 参数。                          */ 
  /*                                                                          */
  if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL) &&
      (tmp_hal_status == HAL_OK)                                  )
  {
    /* 设置 ADC 状态 */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                      HAL_ADC_STATE_BUSY_INTERNAL);
    
    /* 设置 ADC 参数 */
    
    /* ADC 配置：                                                             */
    /*  - 数据对齐方式                                                        */
    /*  - 启动转换的外部触发源                                                */
    /*  - 外部触发极性（始终置 1，因为所有触发源均需要此设置：                 */
    /*    包括外部触发以及软件启动）                                          */
    /*  - 连续转换模式                                                        */
    /* 注意：外部触发极性位（ADC_CR2_EXTTRIG）在                              */
    /*       HAL_ADC_Start_xxx 系列函数中配置，因为若在此函数中配置，         */
    /*       使能 ADC 后注入组转换同时也会触发规则组转换。                    */
    /*                                                                        */
    tmp_cr2 |= (hadc->Init.DataAlign                                          |
                ADC_CFGR_EXTSEL(hadc, hadc->Init.ExternalTrigConv)            |
                ADC_CR2_CONTINUOUS((uint32_t)hadc->Init.ContinuousConvMode)   );

    /* ADC 配置：                                                             */
    /*  - 扫描模式                                                            */
    /*  - 间断模式使能/禁用                                                   */
    /*  - 间断模式转换通道数                                                  */
    tmp_cr1 |= (ADC_CR1_SCAN_SET(hadc->Init.ScanConvMode));
    
    /* 仅在连续转换模式被禁用的情况下使能间断模式 */
    /* 注意：若参数 "Init.ScanConvMode" 配置为禁用，即便间断模式参数依旧被配置，*/
    /*       也不会对 ADC 硬件产生任何影响。                                  */
    if (hadc->Init.DiscontinuousConvMode == ENABLE)
    {
      if (hadc->Init.ContinuousConvMode == DISABLE)
      {
        /* 使能所选 ADC 规则组间断模式 */
        /* 设置间断模式下的转换通道数 */
        SET_BIT(tmp_cr1, ADC_CR1_DISCEN                                            |
                         ADC_CR1_DISCONTINUOUS_NUM(hadc->Init.NbrOfDiscConversion)  );
      }
      else
      {
        /* ADC 规则组的连续模式和序列器间断模式设置 */
        /* 不能同时使能。                                */
        
        /* 将 ADC 状态机更新为错误状态 */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
        
        /* 将 ADC 错误代码设置为 ADC IP 内部错误 */
        SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
      }
    }
    
    /* 将先前的设置更新至 ADC 配置寄存器 CR1 */
      MODIFY_REG(hadc->Instance->CR1,
                 ADC_CR1_SCAN    |
                 ADC_CR1_DISCEN  |
                 ADC_CR1_DISCNUM    ,
                 tmp_cr1             );
    
    /* 将先前的设置更新至 ADC 配置寄存器 CR2 */
      MODIFY_REG(hadc->Instance->CR2,
                 ADC_CR2_ALIGN   |
                 ADC_CR2_EXTSEL  |
                 ADC_CR2_EXTTRIG |
                 ADC_CR2_CONT       ,
                 tmp_cr2             );

    /* 规则组序列器配置：                                                    */
    /* - 如果扫描模式已禁用，规则通道序列长度将设置为                        */
    /*   0x00：转换 1 个通道（规则通道排名 Rank 1 对应的通道）                */
    /*   参数 "NbrOfConversion" 将被忽略。                                   */
    /*   注意：该器件在硬件上支持扫描模式，如果将其                            */
    /*   禁用，会自动忽略转换通道数。无论如何，转换                           */
    /*   通道数强制设置为 0x00，以保持所有 STM32 器件的统一性。               */
    /* - 如果使能了扫描模式，规则通道序列长度将设置为                             */
    /*   参数 "NbrOfConversion"                                                */
    if (ADC_CR1_SCAN_SET(hadc->Init.ScanConvMode) == ADC_SCAN_ENABLE)
    {
      tmp_sqr1 = ADC_SQR1_L_SHIFT(hadc->Init.NbrOfConversion);
    }
      
    MODIFY_REG(hadc->Instance->SQR1,
               ADC_SQR1_L          ,
               tmp_sqr1             );
    
    /* 回读检查 ADC 寄存器是否已被有效配置，以                                  */
    /* 确保 ADC 内核 IP 时钟不存在潜在问题。                                   */
    /* 通过寄存器 CR2 进行检查（排除在其他函数中设置的位：                       */
    /* 执行控制位 (ADON, JSWSTART, SWSTART)、规则组位                           */
    /* (DMA)、注入组位 (JEXTTRIG 和 JEXTSEL)、内部通道                         */
    /* 测量路径位 (TSVREFE)）。                                                */
    if (READ_BIT(hadc->Instance->CR2, ~(ADC_CR2_ADON | ADC_CR2_DMA |
                                        ADC_CR2_SWSTART | ADC_CR2_JSWSTART |
                                        ADC_CR2_JEXTTRIG | ADC_CR2_JEXTSEL |
                                        ADC_CR2_TSVREFE                     ))
         == tmp_cr2)
    {
      /* 将 ADC 错误代码设置为无错误 */
      ADC_CLEAR_ERRORCODE(hadc);
      
      /* 设置 ADC 状态 */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_BUSY_INTERNAL,
                        HAL_ADC_STATE_READY);
    }
    else
    {
      /* 将 ADC 状态机更新为错误状态 */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_BUSY_INTERNAL,
                        HAL_ADC_STATE_ERROR_INTERNAL);
      
      /* 将 ADC 错误代码设置为 ADC IP 内部错误 */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
      
      tmp_hal_status = HAL_ERROR;
    }
  
  }
  else
  {
    /* 将 ADC 状态机更新为错误状态 */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
        
    tmp_hal_status = HAL_ERROR;
  }
  
  /* 返回函数状态 */
  return tmp_hal_status;
}

/**
  * @brief  将 ADC 外设寄存器反初始化为其默认复位值，
  *         同时反初始化 ADC MSP。
  *         如有需要，可将示例代码复制并取消注释到
  *         函数 HAL_ADC_MspDeInit() 中。
  * @param  hadc: ADC 句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* 检查 ADC 句柄 */
  if(hadc == NULL)
  {
     return HAL_ERROR;
  }
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* 设置 ADC 状态 */
  SET_BIT(hadc->State, HAL_ADC_STATE_BUSY_INTERNAL);
  
  /* 停止规则组和注入组上可能正在进行的转换 */
  /* 禁用 ADC 外设 */
  tmp_hal_status = ADC_ConversionStop_Disable(hadc);
  
  
  /* 若前序准备操作已正确完成，                                               */ 
  /* 则配置 ADC 参数。                                                         */
  if (tmp_hal_status == HAL_OK)
  {
    /* ========== 复位 ADC 寄存器 ========== */




    /* 复位寄存器 SR */
    __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_AWD | ADC_FLAG_JEOC | ADC_FLAG_EOC |
                                ADC_FLAG_JSTRT | ADC_FLAG_STRT));
                         
    /* 复位寄存器 CR1 */
    CLEAR_BIT(hadc->Instance->CR1, (ADC_CR1_AWDEN   | ADC_CR1_JAWDEN | ADC_CR1_DISCNUM | 
                                    ADC_CR1_JDISCEN | ADC_CR1_DISCEN | ADC_CR1_JAUTO   | 
                                    ADC_CR1_AWDSGL  | ADC_CR1_SCAN   | ADC_CR1_JEOCIE  |   
                                    ADC_CR1_AWDIE   | ADC_CR1_EOCIE  | ADC_CR1_AWDCH    ));
    
    /* 复位寄存器 CR2 */
    CLEAR_BIT(hadc->Instance->CR2, (ADC_CR2_TSVREFE | ADC_CR2_SWSTART | ADC_CR2_JSWSTART | 
                                    ADC_CR2_EXTTRIG | ADC_CR2_EXTSEL  | ADC_CR2_JEXTTRIG |  
                                    ADC_CR2_JEXTSEL | ADC_CR2_ALIGN   | ADC_CR2_DMA      |        
                                    ADC_CR2_RSTCAL  | ADC_CR2_CAL     | ADC_CR2_CONT     |          
                                    ADC_CR2_ADON                                          ));
    
    /* 复位寄存器 SMPR1 */
    CLEAR_BIT(hadc->Instance->SMPR1, (ADC_SMPR1_SMP17 | ADC_SMPR1_SMP16 | ADC_SMPR1_SMP15 | 
                                      ADC_SMPR1_SMP14 | ADC_SMPR1_SMP13 | ADC_SMPR1_SMP12 | 
                                      ADC_SMPR1_SMP11 | ADC_SMPR1_SMP10                    ));
    
    /* 复位寄存器 SMPR2 */
    CLEAR_BIT(hadc->Instance->SMPR2, (ADC_SMPR2_SMP9 | ADC_SMPR2_SMP8 | ADC_SMPR2_SMP7 | 
                                      ADC_SMPR2_SMP6 | ADC_SMPR2_SMP5 | ADC_SMPR2_SMP4 | 
                                      ADC_SMPR2_SMP3 | ADC_SMPR2_SMP2 | ADC_SMPR2_SMP1 | 
                                      ADC_SMPR2_SMP0                                    ));

    /* 复位寄存器 JOFR1 */
    CLEAR_BIT(hadc->Instance->JOFR1, ADC_JOFR1_JOFFSET1);
    /* 复位寄存器 JOFR2 */
    CLEAR_BIT(hadc->Instance->JOFR2, ADC_JOFR2_JOFFSET2);
    /* 复位寄存器 JOFR3 */
    CLEAR_BIT(hadc->Instance->JOFR3, ADC_JOFR3_JOFFSET3);
    /* 复位寄存器 JOFR4 */
    CLEAR_BIT(hadc->Instance->JOFR4, ADC_JOFR4_JOFFSET4);
    
    /* 复位寄存器 HTR */
    CLEAR_BIT(hadc->Instance->HTR, ADC_HTR_HT);
    /* 复位寄存器 LTR */
    CLEAR_BIT(hadc->Instance->LTR, ADC_LTR_LT);
    
    /* 复位寄存器 SQR1 */
    CLEAR_BIT(hadc->Instance->SQR1, ADC_SQR1_L    |
                                    ADC_SQR1_SQ16 | ADC_SQR1_SQ15 | 
                                    ADC_SQR1_SQ14 | ADC_SQR1_SQ13  );
    
    /* 复位寄存器 SQR1 */
    CLEAR_BIT(hadc->Instance->SQR1, ADC_SQR1_L    |
                                    ADC_SQR1_SQ16 | ADC_SQR1_SQ15 | 
                                    ADC_SQR1_SQ14 | ADC_SQR1_SQ13  );
    
    /* 复位寄存器 SQR2 */
    CLEAR_BIT(hadc->Instance->SQR2, ADC_SQR2_SQ12 | ADC_SQR2_SQ11 | ADC_SQR2_SQ10 | 
                                    ADC_SQR2_SQ9  | ADC_SQR2_SQ8  | ADC_SQR2_SQ7   );
    
    /* 复位寄存器 SQR3 */
    CLEAR_BIT(hadc->Instance->SQR3, ADC_SQR3_SQ6 | ADC_SQR3_SQ5 | ADC_SQR3_SQ4 | 
                                    ADC_SQR3_SQ3 | ADC_SQR3_SQ2 | ADC_SQR3_SQ1  );
    
    /* 复位寄存器 JSQR */
    CLEAR_BIT(hadc->Instance->JSQR, ADC_JSQR_JL |
                                    ADC_JSQR_JSQ4 | ADC_JSQR_JSQ3 | 
                                    ADC_JSQR_JSQ2 | ADC_JSQR_JSQ1  );
    
    /* 复位寄存器 JSQR */
    CLEAR_BIT(hadc->Instance->JSQR, ADC_JSQR_JL |
                                    ADC_JSQR_JSQ4 | ADC_JSQR_JSQ3 | 
                                    ADC_JSQR_JSQ2 | ADC_JSQR_JSQ1  );
    
    /* 复位寄存器 DR */
    /* 位访问模式为只读，无法直接复位 */
    
    /* 复位寄存器 JDR1、JDR2、JDR3、JDR4 */
    /* 位访问模式为只读，无法直接复位 */
    
    /* ========== 硬件复位 ADC 外设 ========== */
    /* 对整个 ADC 外设执行全局复位：ADC 状态被强制恢复到类似器件上电后的状态。*/
    /* 强制恢复到类似器件上电后的状态。                                       */
    /* 如有需要，复制粘贴并取消注释以下复位代码至                             */
    /* 函数 "void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)" 中：              */
    /*                                                                        */
    /*  __HAL_RCC_ADC1_FORCE_RESET()                                          */
    /*  __HAL_RCC_ADC1_RELEASE_RESET()                                        */
    
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    if (hadc->MspDeInitCallback == NULL)
    {
      hadc->MspDeInitCallback = HAL_ADC_MspDeInit; /* 遗留的弱定义 MspDeInit */
    }
    
    /* 去初始化底层硬件 */
    hadc->MspDeInitCallback(hadc);
#else
    /* 去初始化底层硬件 */
    HAL_ADC_MspDeInit(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
    
    /* 将 ADC 错误代码设置为无错误 */
    ADC_CLEAR_ERRORCODE(hadc);
    
    /* 设置 ADC 状态 */
    hadc->State = HAL_ADC_STATE_RESET; 
  
  }
  
  /* 进程已解锁 */
  __HAL_UNLOCK(hadc);
  
  /* 返回函数状态 */
  return tmp_hal_status;
}

/**
  * @brief  初始化 ADC MSP。
  * @param  hadc: ADC 句柄
  * @retval 无
  */
__weak void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hadc);
  /* 注意：不应修改此函数。当需要使用该回调时，
            必须在用户文件中实现 HAL_ADC_MspInit 函数。
   */ 
}

/**
  * @brief  反初始化 ADC MSP。
  * @param  hadc: ADC 句柄
  * @retval 无
  */
__weak void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hadc);
  /* 注意：不应修改此函数。当需要使用该回调时，
            必须在用户文件中实现 HAL_ADC_MspDeInit 函数。
   */ 
}

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
/**
  * @brief  注册用户 ADC 回调函数
  *         用于替换预定义的弱函数（weak）回调
  * @param  hadc 指向包含指定 ADC 配置信息的 ADC_HandleTypeDef 结构的指针。
  * @param  CallbackID 要注册的回调函数 ID
  *         该参数可以是以下值之一：
  *          @arg @ref HAL_ADC_CONVERSION_COMPLETE_CB_ID      ADC 转换完成回调 ID
  *          @arg @ref HAL_ADC_CONVERSION_HALF_CB_ID          ADC 转换过半回调 ID
  *          @arg @ref HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID    ADC 模拟看门狗 1 回调 ID
  *          @arg @ref HAL_ADC_ERROR_CB_ID                    ADC 错误回调 ID
  *          @arg @ref HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID  ADC 注入组转换完成回调 ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID                  ADC Msp Init 回调 ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID                ADC Msp DeInit 回调 ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID MspInit 回调 ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID MspDeInit 回调 ID
  * @param  pCallback 指向回调函数的指针
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADC_RegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID, pADC_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  if (pCallback == NULL)
  {
    /* 更新错误代码 */
    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }
  
  if ((hadc->State & HAL_ADC_STATE_READY) != 0)
  {
    switch (CallbackID)
    {
      case HAL_ADC_CONVERSION_COMPLETE_CB_ID :
        hadc->ConvCpltCallback = pCallback;
        break;
      
      case HAL_ADC_CONVERSION_HALF_CB_ID :
        hadc->ConvHalfCpltCallback = pCallback;
        break;
      
      case HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID :
        hadc->LevelOutOfWindowCallback = pCallback;
        break;
      
      case HAL_ADC_ERROR_CB_ID :
        hadc->ErrorCallback = pCallback;
        break;
      
      case HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID :
        hadc->InjectedConvCpltCallback = pCallback;
        break;
      
      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = pCallback;
        break;
      
      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = pCallback;
        break;
      
      default :
        /* 更新错误代码 */
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status = HAL_ERROR;
        break;
    }
  }
  else if (HAL_ADC_STATE_RESET == hadc->State)
  {
    switch (CallbackID)
    {
      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = pCallback;
        break;
      
      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = pCallback;
        break;
      
      default :
        /* 更新错误代码 */
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
      
        /* 返回错误状态 */
        status = HAL_ERROR;
        break;
    }
  }
  else
  {
    /* 更新错误代码 */
    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
    
    /* 返回错误状态 */
    status =  HAL_ERROR;
  }
  
  return status;
}

/**
  * @brief  注销 ADC 回调函数
  *         ADC 回调函数将重定向到预定义的弱函数（weak）回调
  * @param  hadc 指向包含指定 ADC 配置信息的 ADC_HandleTypeDef 结构的指针。
  * @param  CallbackID 要注销的回调函数 ID
  *         该参数可以是以下值之一：
  *          @arg @ref HAL_ADC_CONVERSION_COMPLETE_CB_ID      ADC 转换完成回调 ID
  *          @arg @ref HAL_ADC_CONVERSION_HALF_CB_ID          ADC 转换过半回调 ID
  *          @arg @ref HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID    ADC 模拟看门狗 1 回调 ID
  *          @arg @ref HAL_ADC_ERROR_CB_ID                    ADC 错误回调 ID
  *          @arg @ref HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID  ADC 注入组转换完成回调 ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID                  ADC Msp Init 回调 ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID                ADC Msp DeInit 回调 ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID MspInit 回调 ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID MspDeInit 回调 ID
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADC_UnRegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  if ((hadc->State & HAL_ADC_STATE_READY) != 0)
  {
    switch (CallbackID)
    {
      case HAL_ADC_CONVERSION_COMPLETE_CB_ID :
        hadc->ConvCpltCallback = HAL_ADC_ConvCpltCallback;
        break;
      
      case HAL_ADC_CONVERSION_HALF_CB_ID :
        hadc->ConvHalfCpltCallback = HAL_ADC_ConvHalfCpltCallback;
        break;
      
      case HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID :
        hadc->LevelOutOfWindowCallback = HAL_ADC_LevelOutOfWindowCallback;
        break;
      
      case HAL_ADC_ERROR_CB_ID :
        hadc->ErrorCallback = HAL_ADC_ErrorCallback;
        break;
      
      case HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID :
        hadc->InjectedConvCpltCallback = HAL_ADCEx_InjectedConvCpltCallback;
        break;
      
      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = HAL_ADC_MspInit; /* 传统的弱定义 MspInit             */
        break;
      
      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = HAL_ADC_MspDeInit; /* 传统的弱定义 MspDeInit           */
        break;
      
      default :
        /* 更新错误代码 */
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
        
        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (HAL_ADC_STATE_RESET == hadc->State)
  {
    switch (CallbackID)
    {
      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = HAL_ADC_MspInit;                   /* 传统的弱定义 MspInit             */
        break;
        
      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = HAL_ADC_MspDeInit;               /* 传统的弱定义 MspDeInit           */
        break;
        
      default :
        /* 更新错误代码 */
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
        
        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* 更新错误代码 */
    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
    
    /* 返回错误状态 */
    status =  HAL_ERROR;
  }
  
  return status;
}

#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup ADC_Exported_Functions_Group2 IO 操作函数
 *  @brief    输入和输出操作函数
 *
@verbatim   
 ===============================================================================
                      ##### IO 操作函数 #####
 ===============================================================================
    [..]  本节提供的函数允许执行以下操作：
      (+) 启动规则组转换。
      (+) 停止规则组转换。
      (+) 轮询规则组转换完成状态。
      (+) 轮询转换事件。
      (+) 获取规则通道转换结果。
      (+) 启动规则组转换并使能中断。
      (+) 停止规则组转换并禁用中断。
      (+) 处理 ADC 中断请求。
      (+) 启动规则组转换并使能 DMA 传输。
      (+) 停止规则组转换并禁用 ADC DMA 传输。
@endverbatim
  * @{
  */

/**
  * @brief  使能 ADC，并开始规则组转换。
  *         在此函数中使能的中断：无。
  * @param  hadc: ADC 句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* 进程已锁定 */
  __HAL_LOCK(hadc);
   
  /* 使能 ADC 外设 */
  tmp_hal_status = ADC_Enable(hadc);
  
  /* 若 ADC 已有效使能，则启动转换 */
  if (tmp_hal_status == HAL_OK)
  {
    /* 设置 ADC 状态                                                          */
    /* - 清除与规则组转换结果相关的状态位域                                   */
    /* - 设置与规则转换操作相关的状态位域                                     */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC,
                      HAL_ADC_STATE_REG_BUSY);
    
    /* 设置注入组状态（来自自动注入）以及多重模式状态                         */
    /* 适用于多重模式的所有情况：独立模式、多重模式 ADC 主机                  */
    /* 或多重模式 ADC 从机（适用于具有多个 ADC 的器件）：                     */
    if (ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc))
    {
      /* 设置 ADC 状态（ADC 独立模式或主模式） */
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
      
      /* 如果规则组的转换同时触发了注入组，                                    */
      /* 则更新 ADC 状态。                                                    */
      if (READ_BIT(hadc->Instance->CR1, ADC_CR1_JAUTO) != RESET)
      {
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);  
      }
    }
    else
    {
      /* 设置 ADC 状态（ADC 从模式） */
      SET_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
      
      /* 如果规则组的转换同时触发了注入组，                                    */
      /* 则更新 ADC 状态。                                                    */
      if (ADC_MULTIMODE_AUTO_INJECTED(hadc))
      {
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
      }
    }
    
    /* 状态机更新：检查注入转换是否正在进行 */
    if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
    {
      /* 复位与规则组转换相关的 ADC 错误代码字段 */
      CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));         
    }
    else
    {
      /* 复位 ADC 所有错误代码字段 */
      ADC_CLEAR_ERRORCODE(hadc);
    }
    
    /* 进程解锁 */
    /* 在启动 ADC 转换前解锁：以防发生潜在的中断，                             */
    /* 从而允许进程交由 ADC 中断处理函数（IRQ Handler）处理。                  */
    __HAL_UNLOCK(hadc);
  
    /* 清除规则组转换标志位 */
    /* （以确保不存在潜在的先前 ADC 操作所导致的未知状态） */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC);
    
    /* 使能规则组转换。                                                      */
    /* 如果选择了软件触发启动，转换将立即开始。                                */
    /* 如果选择了外部触发，转换将在下一次                                      */
    /* 触发事件到来时开始。                                                   */
    /* 使能多重模式的情况：                                                   */ 
    /*  - 如果 ADC 为从机，则仅使能 ADC（不启动转换）。                        */
    /*  - 如果 ADC 为主机，则使能 ADC 并启动转换。                            */
    /* 若 ADC 为主模式（Master），则使能 ADC 并启动转换。                     */
    /* 注意：单次转换的备选触发方式可以是强制再次置位                           */
    /*       ADON 位 "hadc->Instance->CR2 |= ADC_CR2_ADON;"                   */
    if (ADC_IS_SOFTWARE_START_REGULAR(hadc)      &&
        ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc)  )
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
  *         并禁用 ADC 外设。
  * @note:  禁用 ADC 外设会强制停止注入组上可能正在进行的转换。
  *         如果正在使用注入组，应预先使用 HAL_ADCEx_InjectedStop 函数将其停止。
  * @param  hadc: ADC 句柄
  * @retval HAL 状态。
  */
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
     
  /* 进程锁定 */
  __HAL_LOCK(hadc);
  
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
  
  /* 进程解锁 */
  __HAL_UNLOCK(hadc);
  
  /* 返回函数状态 */
  return tmp_hal_status;
}

/**
  * @brief  等待规则组转换完成。
  * @note   此函数不能在特定配置下使用：ADC 配置为 
  *         DMA 模式。
  *         在此情况下，DMA 会复位标志位 EOC，无法对每次转换
  *         进行轮询。
  * @note   在 STM32F1 系列器件上，当使能序列发生器（配置了
  *         多个 rank 通道）时存在限制：无法对序列内的每次
  *         转换进行轮询。在此情况下，轮询将被替换为
  *         等待最大转换时间。
  * @param  hadc: ADC 句柄
  * @param  Timeout: 超时时间（毫秒）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout)
{
  uint32_t tickstart = 0U;
  
  /* 用于在使能扫描模式并对每次转换进行轮询时的变量                          */
  /* 转换。                                                                  */
  __IO uint32_t Conversion_Timeout_CPU_cycles = 0U;
  uint32_t Conversion_Timeout_CPU_cycles_max = 0U;
 
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* 获取嘀嗒计数值 */
  tickstart = HAL_GetTick();
  
  /* 校验 ADC 配置是否兼容对每次转换进行                                     */
  /* 轮询：                                                                  */
  /* 特殊情况是 ADC 配置为 DMA 模式                                          */
  if (HAL_IS_BIT_SET(hadc->Instance->CR2, ADC_CR2_DMA))
  {
    /* 将 ADC 状态机更新为错误状态 */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
    
    /* 解锁进程 */
    __HAL_UNLOCK(hadc);
    
    return HAL_ERROR;
  }
  
  /* 轮询转换结束：区分单次转换还是序列                                       */
  /* 转换。                                                                  */
  /*  - 若为规则组单次转换（扫描模式禁用或使能且                              */
  /*    NbrOfConversion = 1），则使用标志位 EOC 来确定                        */
  /*    转换完成。                                                            */
  /*  - 若为规则组序列转换（启用扫描模式且                                   */
  /*    NbrOfConversion >= 2），则仅在序列结束时置位 EOC 标志。               */
  /*                                                                          */
  /*    若要轮询每次转换，最大转换时间由 ADC 转换时间（所选采样时间 +         */
  /*    12.5 个 ADC 时钟周期的转换时间）以及 APB2/ADC 时钟预分频器计算得出    */
  /*    （根据具体配置，转换时间范围可为 28 到 32256 个 CPU 周期）。           */
  /*    由于每次转换后不会置位 EOC 标志，因此无法设置超时状态。                 */
  /*                                                                          */
  /*                                                                          */
  if (HAL_IS_BIT_CLR(hadc->Instance->CR1, ADC_CR1_SCAN) &&
      HAL_IS_BIT_CLR(hadc->Instance->SQR1, ADC_SQR1_L)    )
  {
    /* 等待转换结束标志位置位 */
    while(HAL_IS_BIT_CLR(hadc->Instance->SR, ADC_FLAG_EOC))
    {
      /* 检查是否禁用了超时机制（设置为无限等待） */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0U) || ((HAL_GetTick() - tickstart ) > Timeout))
        {
          /* 进行重新检查，以避免在发生中断抢占时产生误超时判断 */
          if(HAL_IS_BIT_CLR(hadc->Instance->SR, ADC_FLAG_EOC))
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
    /*  - 计算与 ADC 时钟周期对应的 CPU 时钟周期数                          */
    /*    以及所有通道上的 ADC 最大转换周期数。                               */
    /*  - 等待预期的 ADC 时钟周期延时                                        */
    Conversion_Timeout_CPU_cycles_max = ((SystemCoreClock
                                          / HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_ADC))
                                         * ADC_CONVCYCLES_MAX_RANGE(hadc)                 );
    
    while(Conversion_Timeout_CPU_cycles < Conversion_Timeout_CPU_cycles_max)
    {
      /* 检查超时是否已禁用（设置为无限等待） */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout))
        {
          /* 进行新的检查以避免在发生抢占时出现误判超时 */
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
  
  /* 清除规则组转换标志 */
  __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_STRT | ADC_FLAG_EOC);
  
  /* 更新 ADC 状态机 */
  SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);
  
  /* 判断规则组是否即将进行下一次转换                                         */
  /* （由外部触发、连续模式或正在进行的扫描序列引起）。                       */
  /* 注意：在 STM32F1 系列器件上，如果使能了序列转换器                        */
  /*       （选择了多个通道序列），则在整个序列结束时                         */
  /*       才会置位转换结束标志。                                            */
  if(ADC_IS_SOFTWARE_START_REGULAR(hadc)        && 
     (hadc->Init.ContinuousConvMode == DISABLE)   )
  {   
    /* 设置 ADC 状态 */
    CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);   

    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_INJ_BUSY))
    { 
      SET_BIT(hadc->State, HAL_ADC_STATE_READY);
    }
  }
  
  /* 返回 ADC 状态 */
  return HAL_OK;
}

/**
  * @brief  轮询转换事件。
  * @param  hadc: ADC 句柄
  * @param  EventType: ADC 事件类型。
  *          该参数可以是以下值之一：
  *            @arg ADC_AWD_EVENT: ADC 模拟看门狗事件。
  * @param  Timeout: 超时时间（毫秒）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADC_PollForEvent(ADC_HandleTypeDef* hadc, uint32_t EventType, uint32_t Timeout)
{
  uint32_t tickstart = 0U; 

  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_EVENT_TYPE(EventType));
  
  /* 获取嘀嗒计数值 */
  tickstart = HAL_GetTick();
  
  /* 检查选定的事件标志 */
  while(__HAL_ADC_GET_FLAG(hadc, EventType) == RESET)
  {
    /* 检查超时是否被禁用（设置为无限等待） */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) || ((HAL_GetTick() - tickstart ) > Timeout))
      {
        /* 重新检查以避免在发生抢占时产生误超时判断 */
        if(__HAL_ADC_GET_FLAG(hadc, EventType) == RESET)
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
  
  /* 模拟看门狗（电平超出窗口范围）事件 */
  /* 设置 ADC 状态 */
  SET_BIT(hadc->State, HAL_ADC_STATE_AWD1);
    
  /* 清除 ADC 模拟看门狗标志 */
  __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD);
  
  /* 返回 ADC 状态 */
  return HAL_OK;
}

/**
  * @brief  使能 ADC，并以中断方式启动规则通道组的转换。
  *         该函数中使能的中断：
  *          - EOC（规则通道组转换结束）
  *         这些中断中每一个都具有其专用的回调函数。
  * @param  hadc: ADC 句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* 进程已锁定 */
  __HAL_LOCK(hadc);
    
  /* 使能 ADC 外设 */
  tmp_hal_status = ADC_Enable(hadc);
  
  /* 若 ADC 已有效使能，则启动转换 */
  if (tmp_hal_status == HAL_OK)
  {
    /* 设置 ADC 状态                                                          */
    /* - 清除与规则组转换结果相关的状态位域                                   */
    /* - 设置与规则组操作相关的状态位域                                       */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
                      HAL_ADC_STATE_REG_BUSY);
    
    /* 设置注入组状态（来自自动注入）以及多重模式状态                         */
    /* 适用于所有多重模式情况：独立模式、多重模式 ADC 主机                   */
    /* 或多重模式 ADC 从机（适用于具有多个 ADC 的器件）：                     */
    if (ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc))
    {
      /* 设置 ADC 状态（ADC 独立模式或主机） */
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
      
      /* 如果规则组转换同时触发了注入组，                                    */
      /* 则更新 ADC 状态。                                                    */
      if (READ_BIT(hadc->Instance->CR1, ADC_CR1_JAUTO) != RESET)
      {
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);  
      }
    }
    else
    {
      /* 设置 ADC 状态（ADC 从机） */
      SET_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
      
      /* 如果规则组转换同时触发了注入组，                                    */
      /* 更新 ADC 状态。                                                        */
      if (ADC_MULTIMODE_AUTO_INJECTED(hadc))
      {
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
      }
    }
    
    /* 状态机更新：检查注入组转换是否正在进行 */
    if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
    {
      /* 复位与规则组转换相关的 ADC 错误代码字段 */
      CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));         
    }
    else
    {
      /* 复位所有 ADC 错误代码字段 */
      ADC_CLEAR_ERRORCODE(hadc);
    }
    
    /* 进程已解锁 */
    /* 在启动 ADC 转换前解锁：以防发生潜在的中断，                             */
    /* 使处理流程能够进入 ADC IRQ 中断服务程序。                               */
    __HAL_UNLOCK(hadc);
    
    /* 清除规则组转换标志和过载 (overrun) 标志 */
    /* (确保不存在因之前潜在的 ADC 操作导致的未知状态) */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC);
    
    /* 使能规则组转换结束中断 */
    __HAL_ADC_ENABLE_IT(hadc, ADC_IT_EOC);
    
    /* 使能规则组转换。                                                        */
    /* 如果选择了软件触发启动，转换将立即开始。                                */
    /* 如果选择了外部触发，转换将在下一次                                      */
    /* 触发事件到来时开始。                                                    */
    /* 使能多重模式的情况：                                                    */ 
    /*  - 若 ADC 为从模式，仅使能 ADC（不启动转换）。   */
    /*  - 若 ADC 为主模式，使能 ADC 并启动转换。         */
    if (ADC_IS_SOFTWARE_START_REGULAR(hadc)      &&
        ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc)  )
    {
      /* 通过软件触发启动规则组 ADC 转换 */
      SET_BIT(hadc->Instance->CR2, (ADC_CR2_SWSTART | ADC_CR2_EXTTRIG));
    }
    else
    {
      /* 通过外部触发启动规则组 ADC 转换 */
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
  * @brief  停止规则组 ADC 转换（若处于自动注入模式则同时停止注入组），
  *         禁用转换结束中断，禁用 ADC 外设。
  * @param  hadc: ADC 句柄
  * @retval None
  */
HAL_StatusTypeDef HAL_ADC_Stop_IT(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* 检查参数有效性 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
     
  /* 进程上锁 */
  __HAL_LOCK(hadc);
  
  /* 停止规则组和注入组上可能正在进行的转换 */
  /* 禁用 ADC 外设 */
  tmp_hal_status = ADC_ConversionStop_Disable(hadc);
  
  /* 检查 ADC 是否已切实禁用 */
  if (tmp_hal_status == HAL_OK)
  {
    /* 禁用规则组 ADC 转换结束中断 */
    __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC);
    
    /* 设置 ADC 状态 */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                      HAL_ADC_STATE_READY);
  }
  
  /* 进程解锁 */
  __HAL_UNLOCK(hadc);
  
  /* 返回函数状态 */
  return tmp_hal_status;
}

/**
  * @brief  使能 ADC，启动规则组转换并通过 DMA 传输转换结果。
  *         该函数中使能的中断包括：
  *          - DMA 传输完成中断
  *          - DMA 半传输中断
  *         每个中断都有其专用的回调函数。
  * @note   对于具有多个 ADC 的器件：此函数仅适用于单 ADC 模式。
  *         对于多重模式，请使用专用的 MultimodeStart 函数。
  * @note   在 STM32F1 系列器件上，仅 ADC1 和 ADC3（ADC 的具体可用性取决于
  *         器件型号）具有 DMA 功能。
  *         在双 ADC 模式下，ADC2 的转换数据可通过 ADC1（多重模式下的主 ADC）
  *         的 DMA 进行传输。
  *         在具有 2 个 ADC 实例的器件上使用 ADC1 搭配 DMA 时：ADC1 转换数据寄存器 DR
  *         既包含 ADC1 的转换结果（ADC1 寄存器 DR 的位 0 至 11），另外还包含
  *         ADC2 最近一次的转换结果（ADC1 寄存器 DR 的位 16 至 27）。因此，
  *         若要使 DMA 仅传输 ADC1 的转换结果，DMA 的传输数据宽度必须
  *         配置为：半字（half word）。
  * @param  hadc: ADC 句柄
  * @param  pData: 目标缓冲区地址。
  * @param  Length: 从 ADC 外设传输至内存的数据长度。
  * @retval 无
  */
HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* 检查参数有效性 */
  assert_param(IS_ADC_DMA_CAPABILITY_INSTANCE(hadc->Instance));
    
  /* 校验多重模式是否已禁用（适用于具有多个 ADC 的器件）                       */
  /* 若多重模式已使能，则必须使用专用的多重模式转换启动 DMA                   */
  /* 函数。                                                                   */
  if(ADC_MULTIMODE_IS_ENABLE(hadc) == RESET)
  {
    /* 进程加锁 */
    __HAL_LOCK(hadc);
    
    /* 使能 ADC 外设 */
    tmp_hal_status = ADC_Enable(hadc);
    
    /* 若 ADC 已成功使能，则启动转换 */
    if (tmp_hal_status == HAL_OK)
    {
      /* 设置 ADC 状态                                                        */
      /* - 清除与规则组转换结果相关的状态位域                                 */
      /* - 设置与规则组操作相关的状态位域                                     */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
                        HAL_ADC_STATE_REG_BUSY);
    
    /* 设置注入组状态（来自自动注入）以及多重模式状态                         */
    /* 适用于多重模式的所有情况：独立模式、多重模式 ADC 主机                  */
    /* 或多重模式 ADC 从机（适用于具有多个 ADC 的器件）：                     */
    if (ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc))
    {
      /* 设置 ADC 状态（ADC 独立模式或主模式） */
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
      
      /* 如果规则组转换同时触发了注入组转换，                                  */
      /* 则更新 ADC 状态。                                                    */
      if (READ_BIT(hadc->Instance->CR1, ADC_CR1_JAUTO) != RESET)
      {
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);  
      }
    }
    else
    {
      /* 设置 ADC 状态（ADC 从模式） */
      SET_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
      
      /* 如果规则组转换同时触发了注入组转换，                                  */
      /* 则更新 ADC 状态。                                                    */
      if (ADC_MULTIMODE_AUTO_INJECTED(hadc))
      {
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
      }
    }
      
      /* 状态机更新：检查注入转换是否正在进行 */
      if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
      {
        /* 重置 ADC 中与规则组转换相关的错误码字段 */
        CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));         
      }
      else
      {
        /* 重置 ADC 所有错误码字段 */
        ADC_CLEAR_ERRORCODE(hadc);
      }
      
      /* 进程已解锁 */
      /* 在启动 ADC 转换前解锁：以防发生中断时，                               */
      /* 允许处理流程交由 ADC IRQ 处理函数继续执行。                          */
      __HAL_UNLOCK(hadc);
      
      /* 设置 DMA 传输完成回调函数 */
      hadc->DMA_Handle->XferCpltCallback = ADC_DMAConvCplt;

      /* 设置 DMA 半传输完成回调函数 */
      hadc->DMA_Handle->XferHalfCpltCallback = ADC_DMAHalfConvCplt;
      
      /* 设置 DMA 错误回调函数 */
      hadc->DMA_Handle->XferErrorCallback = ADC_DMAError;

      
      /* 管理 ADC 和 DMA 启动：ADC 过载中断、DMA 启动、ADC                   */
      /* 启动（若为软件触发启动）：                                           */
      
      /* 清除规则组转换标志和过载标志 */
      /* （以确保不存在可能来自先前 ADC 操作的未知状态                        */
      /* ）                                                                   */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC);
      
      /* 使能 ADC DMA 模式 */
      SET_BIT(hadc->Instance->CR2, ADC_CR2_DMA);
      
      /* 启动 DMA 通道 */
      HAL_DMA_Start_IT(hadc->DMA_Handle, (uint32_t)&hadc->Instance->DR, (uint32_t)pData, Length);
      
      /* 使能规则组转换。                                                     */
      /* 若选择了软件触发启动，转换将立即开始。                               */
      /* 若选择了外部触发，转换将在下一个                                     */
      /* 触发事件到来时开始。                                                 */
      if (ADC_IS_SOFTWARE_START_REGULAR(hadc))
      {
        /* 软件触发启动规则组 ADC 转换 */
        SET_BIT(hadc->Instance->CR2, (ADC_CR2_SWSTART | ADC_CR2_EXTTRIG));
      }
      else
      {
        /* 外部触发启动规则组 ADC 转换 */
        SET_BIT(hadc->Instance->CR2, ADC_CR2_EXTTRIG);
      }
    }
    else
    {
      /* 进程解锁 */
      __HAL_UNLOCK(hadc);
    }
  }
  else
  {
    tmp_hal_status = HAL_ERROR;
  }
  
  /* 返回函数状态 */
  return tmp_hal_status;
}

/**
  * @brief  停止规则组的 ADC 转换（在自动注入模式下还包括注入组），
  *         禁用 ADC DMA 传输，禁用 ADC 外设。
  * @note:  禁用 ADC 外设会强制停止注入组上可能正在进行的转换。
  *         如果正在使用注入组，应预先使用 HAL_ADCEx_InjectedStop 函数将其停止。
  * @note   对于具有多个 ADC 的器件：此函数仅适用于单 ADC 模式。
  *         对于多重模式，请使用专用的 MultimodeStop 函数。
  * @note   在 STM32F1 系列器件上，仅 ADC1 和 ADC3 具有 DMA 功能
  *         （ADC 的可用性取决于具体器件型号）。
  * @param  hadc: ADC 句柄
  * @retval HAL 状态。
  */
HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* 检查参数 */
  assert_param(IS_ADC_DMA_CAPABILITY_INSTANCE(hadc->Instance));
     
  /* 进程锁定 */
  __HAL_LOCK(hadc);
  
  /* 停止规则组和注入组上可能正在进行的转换 */
  /* 禁用 ADC 外设 */
  tmp_hal_status = ADC_ConversionStop_Disable(hadc);
  
  /* 检查 ADC 是否已有效禁用 */
  if (tmp_hal_status == HAL_OK)
  {
    /* 禁用 ADC DMA 模式 */
    CLEAR_BIT(hadc->Instance->CR2, ADC_CR2_DMA);
    
    /* 禁用 DMA 通道（适用于 DMA 循环模式或在 DMA */
    /* 传输正在进行时停止的情况）                       */
    if (hadc->DMA_Handle->State == HAL_DMA_STATE_BUSY)
    {
      tmp_hal_status = HAL_DMA_Abort(hadc->DMA_Handle);
      
      /* 检查 DMA 通道是否已有效禁用 */
      if (tmp_hal_status == HAL_OK)
      {
        /* 设置 ADC 状态 */
        ADC_STATE_CLR_SET(hadc->State,
                          HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                          HAL_ADC_STATE_READY);
      }
      else
      {
        /* 将 ADC 状态机更新为错误状态 */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_DMA);
      }
    }
  }
  
  /* 进程解锁 */
  __HAL_UNLOCK(hadc);
    
  /* 返回函数状态 */
  return tmp_hal_status;
}

/**
  * @brief  获取 ADC 规则组转换结果。
  * @note   读取寄存器 DR 会自动清除 ADC 标志位 EOC
  *         （ADC 规则组单次转换结束）。
  * @note   此函数不会清除 ADC 标志位 EOS
  *         （ADC 规则组序列转换结束）。
  *         EOS 标志位置位的时机：
  *          - 如果序列器仅包含 1 个通道（rank），则标志位 EOS 等效于标志位 EOC。
  *          - 如果序列器包含多个通道（rank），在扫描序列执行期间仅置位标志位 EOC，
  *            在扫描序列结束时，标志位 EOC 和 EOS 均会被置位。
  *         若要清除此标志位，可使用以下方法：
  *         在中断编程模式下：@ref HAL_ADC_IRQHandler()，在轮询编程
  *         模式下：@ref HAL_ADC_PollForConversion()
  *         或 @ref __HAL_ADC_CLEAR_FLAG(&hadc, ADC_FLAG_EOS)。
  * @param  hadc: ADC 句柄
  * @retval ADC 规则组转换数据
  */
uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* 注意：此处未通过软件清除 EOC 标志，因为在读取 DR 寄存器时，                 */
  /*       硬件会自动将其清除。                                                */
  
  /* 返回 ADC 转换值 */ 
  return hadc->Instance->DR;
}

/**
  * @brief  处理 ADC 中断请求  
  * @param  hadc: ADC 句柄
  * @retval 无
  */
void HAL_ADC_IRQHandler(ADC_HandleTypeDef* hadc)
{
  uint32_t tmp_sr = hadc->Instance->SR;
  uint32_t tmp_cr1 = hadc->Instance->CR1;

  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
  assert_param(IS_ADC_REGULAR_NB_CONV(hadc->Init.NbrOfConversion));
  
  
  /* ========== 检查规则组转换结束 (End of Conversion) 标志 ========== */
  if((tmp_cr1 & ADC_IT_EOC) == ADC_IT_EOC)
  {
    if((tmp_sr & ADC_FLAG_EOC) == ADC_FLAG_EOC)
    {
      /* 若未处于错误状态，则根据转换状态更新状态机 */
      if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL))
      {
        /* 设置 ADC 状态 */
        SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC); 
      }
      
      /* 判断规则组是否即将进行下一次转换                                     */
      /* （可能由外部触发、连续转换模式或正在进行的扫描序列引起）。          */
      /* 注意：在 STM32F1 系列器件上，若使能了序列发生器                      */
      /*       （选择了多个通道序列），则转换结束标志将在                     */
      /*       整个序列转换结束时置位。                                        */
      if(ADC_IS_SOFTWARE_START_REGULAR(hadc)        && 
         (hadc->Init.ContinuousConvMode == DISABLE)   )
      {
        /* 禁用规则组的 ADC 转换结束中断 */
        __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC);
        
        /* 设置 ADC 状态 */
        CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);   
        
        if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_INJ_BUSY))
        {
          SET_BIT(hadc->State, HAL_ADC_STATE_READY);
        }
      }

      /* 转换完成回调函数 */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->ConvCpltCallback(hadc);
#else
      HAL_ADC_ConvCpltCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
      
      /* 清除规则组转换标志 */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_STRT | ADC_FLAG_EOC);
    }
  }
  
  /* ========== 检查注入组转换结束标志 ========== */
  if((tmp_cr1 & ADC_IT_JEOC) == ADC_IT_JEOC)
  {
    if((tmp_sr & ADC_FLAG_JEOC) == ADC_FLAG_JEOC)
    {
      /* 若未处于错误状态，根据转换状态更新状态机 */
      if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL))
      {
        /* 设置 ADC 状态 */
        SET_BIT(hadc->State, HAL_ADC_STATE_INJ_EOC);
      }

      /* 判断注入组后续是否仍有转换：                                         */
      /* 通过外部触发、正在进行的扫描序列，或通过规则组自动注入                */
      /* 转换触发（与上述规则组中断禁用的条件相同）。                         */
      /*                                                                      */
      /* 注意：在 STM32F1 系列器件上，如果启用了序列发生器                    */
      /*       （选择了多个通道序列），转换结束标志仅在                       */
      /*       序列结束时置位。                                               */
      if(ADC_IS_SOFTWARE_START_INJECTED(hadc)                     || 
         (HAL_IS_BIT_CLR(hadc->Instance->CR1, ADC_CR1_JAUTO) &&     
         (ADC_IS_SOFTWARE_START_REGULAR(hadc)        &&
          (hadc->Init.ContinuousConvMode == DISABLE)   )        )   )
      {
        /* 禁用注入组 ADC 转换结束中断 */
        __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOC);
        
        /* 设置 ADC 状态 */
        CLEAR_BIT(hadc->State, HAL_ADC_STATE_INJ_BUSY);   

        if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_REG_BUSY))
        { 
          SET_BIT(hadc->State, HAL_ADC_STATE_READY);
        }
      }

      /* 转换完成回调函数 */ 
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->InjectedConvCpltCallback(hadc);
#else
      HAL_ADCEx_InjectedConvCpltCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
      
      /* 清除注入组转换标志 */
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_JSTRT | ADC_FLAG_JEOC));
    }
  }
   
  /* ========== 检查模拟看门狗标志 ========== */
  if((tmp_cr1 & ADC_IT_AWD) == ADC_IT_AWD)
  {
    if((tmp_sr & ADC_FLAG_AWD) == ADC_FLAG_AWD)
    {
      /* 设置 ADC 状态 */
      SET_BIT(hadc->State, HAL_ADC_STATE_AWD1);
      
      /* 电平超出窗口范围回调函数 */ 
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->LevelOutOfWindowCallback(hadc);
#else
      HAL_ADC_LevelOutOfWindowCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
      
      /* 清除 ADC 模拟看门狗标志 */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD);
    }
  }
  
}

/**
  * @brief  非阻塞模式下的转换完成回调函数
  * @param  hadc: ADC 句柄
  * @retval 无
  */
__weak void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* 防止未使用参数产生的编译警告 */
  UNUSED(hadc);
  /* NOTE : 不应修改此函数。当需要该回调函数时，
            应在用户文件中实现 HAL_ADC_ConvCpltCallback 函数。
   */
}

/**
  * @brief  非阻塞模式下的转换 DMA 半传输完成回调函数
  * @param  hadc: ADC 句柄
  * @retval 无
  */
__weak void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* 防止未使用参数产生的编译警告 */
  UNUSED(hadc);
  /* NOTE : 不应修改此函数。当需要该回调函数时，
            应在用户文件中实现 HAL_ADC_ConvHalfCpltCallback 函数。
  */
}

/**
  * @brief  非阻塞模式下的模拟看门狗回调函数。
  * @param  hadc: ADC 句柄
  * @retval 无
  */
__weak void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(hadc);
  /* 注意：不应修改此函数。当需要使用该回调时，
            必须在用户文件中实现 HAL_ADC_LevelOutOfWindowCallback 函数。
  */
}

/**
  * @brief  非阻塞模式下的 ADC 错误回调函数
  *        （中断方式的 ADC 转换或通过 DMA 传输）
  * @param  hadc: ADC 句柄
  * @retval 无
  */
__weak void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(hadc);
  /* 注意：不应修改此函数。当需要使用该回调时，
            必须在用户文件中实现 HAL_ADC_ErrorCallback 函数。
  */
}


/**
  * @}
  */

/** @defgroup ADC_Exported_Functions_Group3 外设控制函数
 *  @brief    外设控制函数
 *
@verbatim   
 ===============================================================================
             ##### 外设控制函数 #####
 ===============================================================================  
    [..]  本节提供的函数可实现：
      (+) 配置规则通道组中的通道
      (+) 配置模拟看门狗
      
@endverbatim
  * @{
  */

/**
  * @brief  配置所选通道并将其关联到规则通道组。
  * @note   使用内部测量通道的情况：
  *         Vbat/VrefInt/TempSensor。
  *         这些内部通路可以通过函数 HAL_ADC_DeInit() 禁用。
  * @note   支持动态更新参数：
  *         此函数用于在规则通道组中初始化通道，后续调用此函数可用于在不复位 ADC 
  *         的情况下动态重新配置 "ADC_ChannelConfTypeDef" 结构体的部分参数。
  *         这些参数的设置取决于 ADC 的当前状态。
  *         有关参数约束条件，请参见 "ADC_ChannelConfTypeDef" 结构体的注释说明。
  * @param  hadc: ADC 句柄
  * @param  sConfig: 规则组 ADC 通道配置结构体。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig)
{ 
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  __IO uint32_t wait_loop_index = 0U;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CHANNEL(sConfig->Channel));
  assert_param(IS_ADC_REGULAR_RANK(sConfig->Rank));
  assert_param(IS_ADC_SAMPLE_TIME(sConfig->SamplingTime));
  
  /* 进程锁 */
  __HAL_LOCK(hadc);
  
  
  /* 规则序列配置 */
  /* 对应 Rank 1 到 6 */
  if (sConfig->Rank < 7U)
  {
    MODIFY_REG(hadc->Instance->SQR3                        ,
               ADC_SQR3_RK(ADC_SQR3_SQ1, sConfig->Rank)    ,
               ADC_SQR3_RK(sConfig->Channel, sConfig->Rank) );
  }
  /* 对应 Rank 7 到 12 */
  else if (sConfig->Rank < 13U)
  {
    MODIFY_REG(hadc->Instance->SQR2                        ,
               ADC_SQR2_RK(ADC_SQR2_SQ7, sConfig->Rank)    ,
               ADC_SQR2_RK(sConfig->Channel, sConfig->Rank) );
  }
  /* 对应 Rank 13 到 16 */
  else
  {
    MODIFY_REG(hadc->Instance->SQR1                        ,
               ADC_SQR1_RK(ADC_SQR1_SQ13, sConfig->Rank)   ,
               ADC_SQR1_RK(sConfig->Channel, sConfig->Rank) );
  }
  
  
  /* 通道采样时间配置 */
  /* 针对通道 10 到 17 */
  if (sConfig->Channel >= ADC_CHANNEL_10)
  {
    MODIFY_REG(hadc->Instance->SMPR1                             ,
               ADC_SMPR1(ADC_SMPR1_SMP10, sConfig->Channel)      ,
               ADC_SMPR1(sConfig->SamplingTime, sConfig->Channel) );
  }
  else /* 针对通道 0 到 9 */
  {
    MODIFY_REG(hadc->Instance->SMPR2                             ,
               ADC_SMPR2(ADC_SMPR2_SMP0, sConfig->Channel)       ,
               ADC_SMPR2(sConfig->SamplingTime, sConfig->Channel) );
  }
  
  /* 若选择了 ADC1 的 Channel_16 或 Channel_17，则使能温度传感器  */
  /* 和 VREFINT 测量通道。                                            */
  if ((sConfig->Channel == ADC_CHANNEL_TEMPSENSOR) ||
      (sConfig->Channel == ADC_CHANNEL_VREFINT)      )
  {
    /* 对于具有多个 ADC 的 STM32F1 器件：仅 ADC1 可访问内部    */
    /* 测量通道（VrefInt/TempSensor）。若尝试在其他 ADC 实例上   */
    /* 配置这些通道，将报告错误。                               */
    if (hadc->Instance == ADC1)
    {
      if (READ_BIT(hadc->Instance->CR2, ADC_CR2_TSVREFE) == RESET)
      {
        SET_BIT(hadc->Instance->CR2, ADC_CR2_TSVREFE);
        
        if (sConfig->Channel == ADC_CHANNEL_TEMPSENSOR)
        {
          /* 延时以等待温度传感器稳定 */
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

/**
  * @brief  配置模拟看门狗。
  * @note   在 ADC 转换进行期间，可以修改模拟看门狗阈值。
  *         在此情况下，必须考虑一些约束条件：
  *         所设置的阈值将在下一次 ADC EOC（单次转换结束）
  *         开始生效。
  *         考虑到由于总线活动可能导致寄存器写入延迟，
  *         这可能会对新设置阈值的实际生效时机
  *         带来不确定性。
  * @param  hadc: ADC 句柄
  * @param  AnalogWDGConfig: ADC 模拟看门狗配置结构体
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_ADC_AnalogWDGConfig(ADC_HandleTypeDef* hadc, ADC_AnalogWDGConfTypeDef* AnalogWDGConfig)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_ANALOG_WATCHDOG_MODE(AnalogWDGConfig->WatchdogMode));
  assert_param(IS_FUNCTIONAL_STATE(AnalogWDGConfig->ITMode));
  assert_param(IS_ADC_RANGE(AnalogWDGConfig->HighThreshold));
  assert_param(IS_ADC_RANGE(AnalogWDGConfig->LowThreshold));
  
  if((AnalogWDGConfig->WatchdogMode == ADC_ANALOGWATCHDOG_SINGLE_REG)     ||
     (AnalogWDGConfig->WatchdogMode == ADC_ANALOGWATCHDOG_SINGLE_INJEC)   ||
     (AnalogWDGConfig->WatchdogMode == ADC_ANALOGWATCHDOG_SINGLE_REGINJEC)  )
  {
    assert_param(IS_ADC_CHANNEL(AnalogWDGConfig->Channel));
  }
  
  /* 进程已锁定 */
  __HAL_LOCK(hadc);
  
  /* 模拟看门狗配置 */

  /* 配置 ADC 模拟看门狗中断 */
  if(AnalogWDGConfig->ITMode == ENABLE)
  {
    /* 使能 ADC 模拟看门狗中断 */
    __HAL_ADC_ENABLE_IT(hadc, ADC_IT_AWD);
  }
  else
  {
    /* 禁用 ADC 模拟看门狗中断 */
    __HAL_ADC_DISABLE_IT(hadc, ADC_IT_AWD);
  }
  
  /* 模拟看门狗配置：                                                          */
  /*  - 设置模拟看门狗使能模式：规则组和/或注入组，                           */
  /*    单通道或全部通道。                                                    */
  /*  - 设置模拟看门狗监控通道（若看门狗模式为                                */
  /*    "全部通道"，即 ADC_CFGR_AWD1SGL=0，则不使用该参数）。                 */
  MODIFY_REG(hadc->Instance->CR1            ,
             ADC_CR1_AWDSGL |
             ADC_CR1_JAWDEN |
             ADC_CR1_AWDEN  |
             ADC_CR1_AWDCH                  ,
             AnalogWDGConfig->WatchdogMode |
             AnalogWDGConfig->Channel        );
  
  /* 设置高阈值 */
  WRITE_REG(hadc->Instance->HTR, AnalogWDGConfig->HighThreshold);
  
  /* 设置低阈值 */
  WRITE_REG(hadc->Instance->LTR, AnalogWDGConfig->LowThreshold);

  /* 进程已解锁 */
  __HAL_UNLOCK(hadc);
  
  /* 返回函数状态 */
  return HAL_OK;
}


/**
  * @}
  */


/** @defgroup ADC_Exported_Functions_Group4 外设状态函数
 *  @brief    外设状态函数
 *
@verbatim
 ===============================================================================
            ##### 外设状态与错误处理函数 #####
 ===============================================================================  
    [..]
    本小节提供的函数用于在运行时获取外设的状态：
      (+) 检查 ADC 状态
      (+) 检查 ADC 错误代码

@endverbatim
  * @{
  */

/**
  * @brief  返回 ADC 状态
  * @param  hadc: ADC 句柄
  * @retval HAL 状态
  */
uint32_t HAL_ADC_GetState(ADC_HandleTypeDef* hadc)
{
  /* 返回 ADC 状态 */
  return hadc->State;
}

/**
  * @brief  返回 ADC 错误代码
  * @param  hadc: ADC 句柄
  * @retval ADC 错误代码
  */
uint32_t HAL_ADC_GetError(ADC_HandleTypeDef *hadc)
{
  return hadc->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup ADC_Private_Functions ADC 私有函数
  * @{
  */

/**
  * @brief  使能指定的 ADC。
  * @note   使用此函数的前提条件：ADC 必须处于禁用状态，
  *         且内部调压器必须已使能（在 HAL_ADC_Init() 中完成）。
  * @param  hadc: ADC 句柄
  * @retval HAL 状态。
  */
HAL_StatusTypeDef ADC_Enable(ADC_HandleTypeDef* hadc)
{
  uint32_t tickstart = 0U;
  __IO uint32_t wait_loop_index = 0U;
  
  /* 使能 ADC 并等待 ADC 就绪（适用于 ADC 处于禁用状态或                      */
  /* 使能阶段尚未完成：ADC 就绪标志尚未置位的情况）。                          */
  /* 实现了超时机制，以防 ADC 无法使能时系统卡死（可能的原因：                */
  /* ADC 时钟未运行等）。                                                    */
  if (ADC_IS_ENABLE(hadc) == RESET)
  {
    /* 使能外设 */
    __HAL_ADC_ENABLE(hadc);
    
    /* 延时以等待 ADC 稳定 */
    /* 计算需要等待的 CPU 周期数 */
    wait_loop_index = (ADC_STAB_DELAY_US * (SystemCoreClock / 1000000U));
    while(wait_loop_index != 0U)
    {
      wait_loop_index--;
    }
    
    /* 获取滴答计数值 */
    tickstart = HAL_GetTick();

    /* 等待 ADC 实际使能 */
    while(ADC_IS_ENABLE(hadc) == RESET)
    {
      if((HAL_GetTick() - tickstart) > ADC_ENABLE_TIMEOUT)
      {
        /* 重新检查以避免在发生任务抢占时误判超时 */
        if(ADC_IS_ENABLE(hadc) == RESET)
        {
          /* 将 ADC 状态机更新为错误状态 */
          SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

          /* 将 ADC 错误码设置为 ADC IP 内部错误 */
          SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);

          /* 进程解锁 */
          __HAL_UNLOCK(hadc);

          return HAL_ERROR;
        }
      }
    }
  }
   
  /* 返回 HAL 状态 */
  return HAL_OK;
}

/**
  * @brief  停止 ADC 转换并禁用所选的 ADC
  * @note   使用此前置条件：必须先停止 ADC 转换才能禁用 ADC。
  * @param  hadc: ADC 句柄
  * @retval HAL 状态。
  */
HAL_StatusTypeDef ADC_ConversionStop_Disable(ADC_HandleTypeDef* hadc)
{
  uint32_t tickstart = 0U;
  
  /* 校验 ADC 是否尚未被禁用 */
  if (ADC_IS_ENABLE(hadc) != RESET)
  {
    /* 禁用 ADC 外设 */
    __HAL_ADC_DISABLE(hadc);
     
    /* 获取滴答计数值 */
    tickstart = HAL_GetTick();
    
    /* 等待 ADC 实际禁用 */
    while(ADC_IS_ENABLE(hadc) != RESET)
    {
      if((HAL_GetTick() - tickstart) > ADC_DISABLE_TIMEOUT)
      {
        /* 重新检查以避免在发生任务抢占时误判超时 */
        if(ADC_IS_ENABLE(hadc) != RESET)
        {
          /* 将 ADC 状态机更新为错误状态 */
          SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

          /* 将 ADC 错误码设置为 ADC IP 内部错误 */
          SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);

          return HAL_ERROR;
        }
      }
    }
  }
  
  /* 返回 HAL 状态 */
  return HAL_OK;
}

/**
  * @brief  DMA 传输完成回调函数。
  * @param  hdma: 指向 DMA 句柄的指针。
  * @retval 无
  */
void ADC_DMAConvCplt(DMA_HandleTypeDef *hdma)
{
  /* 获取当前 DMA 句柄对应的 ADC 句柄 */
  ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
 
  /* 若未处于错误状态，则根据转换状态更新状态机 */
  if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL | HAL_ADC_STATE_ERROR_DMA))
  {
    /* 更新 ADC 状态机 */
    SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);
    
    /* 判断规则组后续是否还有转换：                                          */
    /* 由外部触发、连续模式或正在进行的扫描序列引起。                         */
    /* 注意：在 STM32F1 系列器件上，如果使能了序列发生器                      */
    /*       （选择了多个通道转换顺序），则转换结束标志将在                   */
    /*       整个序列转换结束时置位。                                         */
    if(ADC_IS_SOFTWARE_START_REGULAR(hadc)        && 
       (hadc->Init.ContinuousConvMode == DISABLE)   )
    {
      /* 设置 ADC 状态 */
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);   
      
      if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_INJ_BUSY))
      {
        SET_BIT(hadc->State, HAL_ADC_STATE_READY);
      }
    }
    
    /* 转换完成回调函数 */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    hadc->ConvCpltCallback(hadc);
#else
    HAL_ADC_ConvCpltCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
  }
  else
  {
    /* 调用 DMA 错误回调函数 */
    hadc->DMA_Handle->XferErrorCallback(hdma);
  }
}

/**
  * @brief  DMA 半传输完成回调函数。
  * @param  hdma: 指向 DMA 句柄的指针。
  * @retval 无
  */
void ADC_DMAHalfConvCplt(DMA_HandleTypeDef *hdma)   
{
  /* 获取当前 DMA 句柄对应的 ADC 句柄 */
  ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
  /* 半传输完成转换回调函数 */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    hadc->ConvHalfCpltCallback(hadc);
#else
  HAL_ADC_ConvHalfCpltCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA 错误回调函数
  * @param  hdma: 指向 DMA 句柄的指针。
  * @retval 无
  */
void ADC_DMAError(DMA_HandleTypeDef *hdma)   
{
  /* 获取当前 DMA 句柄对应的 ADC 句柄 */
  ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
  /* 设置 ADC 状态 */
  SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_DMA);
  
  /* 将 ADC 错误码设置为 DMA 错误 */
  SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_DMA);
  
  /* 错误回调函数 */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
  hadc->ErrorCallback(hadc);
#else
  HAL_ADC_ErrorCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
}

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

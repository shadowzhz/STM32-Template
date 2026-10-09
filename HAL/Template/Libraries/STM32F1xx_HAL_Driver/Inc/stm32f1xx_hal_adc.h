/**
  ******************************************************************************
  * @file    stm32f1xx_hal_adc.h
  * @author  MCD 应用团队
  * @brief   包含 ADC HAL 库函数原型的头文件。
  ******************************************************************************
  * @attention
  *
  *
  * Copyright (c) 2016 STMicroelectronics.
  * 保留所有权利。
  *
  * 本软件根据可在该软件组件根目录的 LICENSE 文件中找到的条款获得许可。
  * 如果本软件未附带 LICENSE 文件，则按“原样”提供。
  *
  ******************************************************************************
  */

/* 防止递归包含的宏定义 -----------------------------------------------------*/
#ifndef __STM32F1xx_HAL_ADC_H
#define __STM32F1xx_HAL_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 包含文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"  
/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup ADC
  * @{
  */ 

/* 导出类型 ------------------------------------------------------------------*/ 
/** @defgroup ADC_Exported_Types ADC 导出类型
  * @{
  */

/** 
  * @brief  ADC 及规则组初始化的结构体定义
  * @note   该结构体的参数分布在 2 个作用域中：
  *          - 整个 ADC 作用域（影响规则组和注入组）：DataAlign，ScanConvMode。
  *          - 规则组作用域：ContinuousConvMode，NbrOfConversion，DiscontinuousConvMode，NbrOfDiscConversion，ExternalTrigConvEdge，ExternalTrigConv。
  * @note   使用函数 HAL_ADC_Init() 配置这些参数取决于 ADC 的状态。
  *         ADC 必须处于禁用状态，或者处于使能但规则组未在进行转换的状态。
  */
typedef struct
{
  uint32_t DataAlign;                        /*!< 指定 ADC 数据对齐方式为右对齐（MSB 位于寄存器第 11 位，LSB 位于寄存器第 0 位）（默认设置）
                                                  或左对齐（若为规则组：MSB 位于寄存器第 15 位，LSB 位于寄存器第 4 位；若为注入组（由于应用偏移量后可能为负值，MSB 保留为有符号值）：MSB 位于寄存器第 14 位，LSB 位于寄存器第 3 位）。
                                                  此参数可以取 @ref ADC_Data_align 的值 */
  uint32_t ScanConvMode;                     /*!< 配置规则组和注入组的序列器。
                                                  此参数可与参数 'DiscontinuousConvMode' 关联使用，以将主序列细分为连续的几个子序列。
                                                  若禁用：转换以单次模式执行（仅转换一个通道，即 rank 1 中定义的通道）。
                                                               参数 'NbrOfConversion' 和 'InjectedNbrOfConversion' 被忽略（相当于设置为 1）。
                                                  若使能：转换以扫描序列模式执行（多个 rank 由 'NbrOfConversion'/'InjectedNbrOfConversion' 以及各个通道 rank 定义）。
                                                               扫描方向为递增：从 rank 1 到 rank 'n'。
                                                  此参数可以取 @ref ADC_Scan_mode 的值
                                                  注意：对于规则组，若使能此参数，转换应通过轮询方式（HAL_ADC_Start 配合间断模式且 NbrOfDiscConversion=1）
                                                        或通过 DMA 方式（HAL_ADC_Start_DMA）执行，但不能通过中断方式（HAL_ADC_Start_IT）执行：在扫描模式下，中断仅在
                                                        序列的最后一次转换完成时触发。之前所有的转换结果都会被最后一次的结果覆盖。
                                                        注入组在扫描模式下没有此限制：每个 rank 都有其独立的结果寄存器，数据不会被覆盖。 */
  FunctionalState ContinuousConvMode;         /*!< 指定在所选触发事件发生后（软件启动或外部触发），
                                                  规则组转换是以单次模式（转换一次）还是连续模式执行。
                                                  此参数可设置为 ENABLE 或 DISABLE。 */
  uint32_t NbrOfConversion;                  /*!< 指定规则组序列器中将要转换的 rank 数量。
                                                  若要使用规则组序列器并转换多个 rank，必须使能参数 'ScanConvMode'。
                                                  此参数必须为介于 Min_Data = 1 和 Max_Data = 16 之间的数值。 */
  FunctionalState  DiscontinuousConvMode;    /*!< 指定规则组的转换序列是以完整序列模式还是间断序列模式执行（主序列被细分为连续的几个子序列）。
                                                  间断模式仅在使能序列器时（参数 'ScanConvMode'）使用。若禁用序列器，此参数将被忽略。
                                                  仅在禁用连续模式时才能使能间断模式。若使能了连续模式，该参数设置将被忽略。
                                                  此参数可设置为 ENABLE 或 DISABLE。 */
  uint32_t NbrOfDiscConversion;              /*!< 指定规则组主序列（参数 NbrOfConversion）细分后的间断转换数量。
                                                  若参数 'DiscontinuousConvMode' 被禁用，则此参数被忽略。
                                                  此参数必须为介于 Min_Data = 1 和 Max_Data = 8 之间的数值。 */
  uint32_t ExternalTrigConv;                 /*!< 选择用于触发规则组启动转换的外部事件。
                                                  若设置为 ADC_SOFTWARE_START，则禁用外部触发。
                                                  若设置为外部触发源，则在事件上升沿触发。
                                                  此参数可以取 @ref ADC_External_trigger_source_Regular 的值 */
}ADC_InitTypeDef;

/** 
  * @brief  ADC 规则组通道结构体定义   
  * @note   使用函数 HAL_ADC_ConfigChannel() 配置这些参数取决于 ADC 的状态。
  *         ADC 可以处于禁用状态，或处于使能但规则组未进行转换的状态。
  */ 
typedef struct 
{
  uint32_t Channel;                /*!< 指定要配置到 ADC 规则组的通道。
                                        该参数可以是 @ref ADC_channels 中的一个值
                                        注意：根据器件不同，某些通道可能在封装引脚上不可用。有关通道的可用性，请参阅器件数据手册。
                                        注意：在具有多个 ADC 的 STM32F1 器件上：仅 ADC1 可以访问内部测量通道（VrefInt/TempSensor） 
                                        注意：在 STM32F10xx8 和 STM32F10xxB 器件上：当 ADC 在注入触发下进行转换时，PA0 引脚上（ADC 输入通道 0）可能会产生低幅度电压毛刺。
                                              建议合理分配模拟通道，将通道 0 配置为注入通道。
                                              更多详细信息请参阅这些器件的勘误手册（errata sheet）。 */
  uint32_t Rank;                   /*!< 指定规则组序列器中的排序（Rank） 
                                        该参数可以是 @ref ADC_regular_rank 中的一个值
                                        注意：如果需要禁用通道或更改转换序列器的顺序，可以通过新的通道设置覆盖包含先前通道设置的 Rank（或者可以调整转换次数参数） */
  uint32_t SamplingTime;           /*!< 为所选通道设置的采样时间值。
                                        单位：ADC 时钟周期
                                        转换时间是采样时间与处理时间之和（12 位 ADC 分辨率下处理时间为 12.5 个 ADC 时钟周期）。
                                        该参数可以是 @ref ADC_sampling_times 中的一个值
                                        注意：此参数会更新通道的参数属性，该通道可用于规则组和/或注入组。
                                                 如果该通道先前已在另一个组（规则组/注入组）中配置过，它将被更新为最新的设置。
                                        注意：如果使用内部测量通道（VrefInt/TempSensor），
                                              必须遵守采样时间约束（采样时间可根据 ADC 时钟频率和采样时间设置进行调整）
                                              有关时序值请参阅器件数据手册中的参数 TS_vrefint、TS_temp（大致范围：最小 5us 至 17.1us）。 */
}ADC_ChannelConfTypeDef;

/**
  * @brief  ADC 模拟看门狗配置定义
  * @note   使用函数配置这些参数取决于 ADC 的状态。
  *         ADC 状态可以处于禁用状态，或处于使能但规则组和注入组均未进行转换的状态。
  */
typedef struct
{
  uint32_t WatchdogMode;      /*!< 配置 ADC 模拟看门狗模式：单通道/所有通道，规则组/注入组。
                                   该参数可以是 @ref ADC_analog_watchdog_mode 中的一个值。 */
  uint32_t Channel;           /*!< 选择由模拟看门狗监视的 ADC 通道。
                                   仅当看门狗模式配置为单通道时（参数 WatchdogMode），该参数才有效
                                   该参数可以是 @ref ADC_channels 中的一个值。 */
  FunctionalState  ITMode;    /*!< 指定模拟看门狗是配置为中断模式还是轮询模式。
                                   该参数可设置为 ENABLE 或 DISABLE */
  uint32_t HighThreshold;     /*!< 配置 ADC 模拟看门狗高阈值。
                                   该参数必须是介于 Min_Data = 0x000 和 Max_Data = 0xFFF 之间的数值。 */
  uint32_t LowThreshold;      /*!< 配置 ADC 模拟看门狗高阈值。
                                   该参数必须是介于 Min_Data = 0x000 和 Max_Data = 0xFFF 之间的数值。 */
  uint32_t WatchdogNumber;    /*!< 保留供将来使用，可设置为 0 */
}ADC_AnalogWDGConfTypeDef;

/** 
  * @brief  HAL ADC 状态机：ADC 状态定义（位域）
  */ 
/* ADC 全局作用域状态 */
#define HAL_ADC_STATE_RESET             0x00000000U    /*!< ADC 尚未初始化或已被禁用 */
#define HAL_ADC_STATE_READY             0x00000001U    /*!< ADC 外设就绪可供使用 */
#define HAL_ADC_STATE_BUSY_INTERNAL     0x00000002U    /*!< ADC 正在进行内部处理（初始化、校准） */
#define HAL_ADC_STATE_TIMEOUT           0x00000004U    /*!< 发生超时 */

/* ADC 错误状态 */
#define HAL_ADC_STATE_ERROR_INTERNAL    0x00000010U    /*!< 发生内部错误 */
#define HAL_ADC_STATE_ERROR_CONFIG      0x00000020U    /*!< 发生配置错误 */
#define HAL_ADC_STATE_ERROR_DMA         0x00000040U    /*!< 发生 DMA 错误 */

/* ADC 规则组状态 */
#define HAL_ADC_STATE_REG_BUSY          0x00000100U    /*!< 规则组转换正在进行或可能发生（通过连续模式、
                                                           外部触发、低功耗自动上电、双/多重 ADC 主模式控制等方式） */
#define HAL_ADC_STATE_REG_EOC           0x00000200U    /*!< 规则组转换数据可用 */
#define HAL_ADC_STATE_REG_OVR           0x00000400U    /*!< STM32F1 系列器件不支持：发生数据溢出（Overrun） */
#define HAL_ADC_STATE_REG_EOSMP         0x00000800U    /*!< STM32F1 系列器件不支持：采样结束（End Of Sampling）标志置位 */

/* ADC 注入组状态 */
#define HAL_ADC_STATE_INJ_BUSY          0x00001000U    /*!< 注入组转换正在进行或可能发生（通过自动注入模式、
                                                           外部触发、低功耗自动上电、双/多重 ADC 主模式控制等方式） */
#define HAL_ADC_STATE_INJ_EOC           0x00002000U    /*!< 注入组转换数据可用 */
#define HAL_ADC_STATE_INJ_JQOVF         0x00004000U    /*!< STM32F1 系列器件不支持：发生注入队列溢出 */

/* ADC 模拟看门狗状态 */
#define HAL_ADC_STATE_AWD1              0x00010000U    /*!< 模拟看门狗 1 发生超出窗口事件 */
#define HAL_ADC_STATE_AWD2              0x00020000U    /*!< STM32F1 器件不支持：模拟看门狗 2 发生超出窗口事件 */
#define HAL_ADC_STATE_AWD3              0x00040000U    /*!< STM32F1 器件不支持：模拟看门狗 3 发生超出窗口事件 */

/* ADC 多重模式状态 */
#define HAL_ADC_STATE_MULTIMODE_SLAVE   0x00100000U    /*!< ADC 处于多重模式从机状态，受另一个 ADC 主机控制 ( */


/**
  * @brief  ADC 句柄结构体定义  
  */ 
typedef struct __ADC_HandleTypeDef
{
  ADC_TypeDef                   *Instance;              /*!< 寄存器基地址 */

  ADC_InitTypeDef               Init;                   /*!< ADC 必要配置参数 */

  DMA_HandleTypeDef             *DMA_Handle;            /*!< 指向 DMA 句柄的指针 */

  HAL_LockTypeDef               Lock;                   /*!< ADC 锁对象 */
  
  __IO uint32_t                 State;                  /*!< ADC 通信状态（ADC 状态位图） */

  __IO uint32_t                 ErrorCode;              /*!< ADC 错误代码 */

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
  void (* ConvCpltCallback)(struct __ADC_HandleTypeDef *hadc);              /*!< ADC 转换完成回调函数 */
  void (* ConvHalfCpltCallback)(struct __ADC_HandleTypeDef *hadc);          /*!< ADC 转换 DMA 半传输完成回调函数 */
  void (* LevelOutOfWindowCallback)(struct __ADC_HandleTypeDef *hadc);      /*!< ADC 模拟看门狗 1 回调函数 */
  void (* ErrorCallback)(struct __ADC_HandleTypeDef *hadc);                 /*!< ADC 错误回调函数 */
  void (* InjectedConvCpltCallback)(struct __ADC_HandleTypeDef *hadc);      /*!< ADC 注入组转换完成回调函数 */       /*!< ADC 采样结束回调函数 */
  void (* MspInitCallback)(struct __ADC_HandleTypeDef *hadc);               /*!< ADC MSP 初始化回调函数 */
  void (* MspDeInitCallback)(struct __ADC_HandleTypeDef *hadc);             /*!< ADC MSP 反初始化回调函数 */
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
}ADC_HandleTypeDef;


#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL ADC 回调函数 ID 枚举定义
  */
typedef enum
{
  HAL_ADC_CONVERSION_COMPLETE_CB_ID     = 0x00U,  /*!< ADC 转换完成回调函数 ID */
  HAL_ADC_CONVERSION_HALF_CB_ID         = 0x01U,  /*!< ADC 转换 DMA 半传输完成回调函数 ID */
  HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID   = 0x02U,  /*!< ADC 模拟看门狗 1 回调函数 ID */
  HAL_ADC_ERROR_CB_ID                   = 0x03U,  /*!< ADC 错误回调函数 ID */
  HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID = 0x04U,  /*!< ADC 注入组转换完成回调函数 ID */
  HAL_ADC_MSPINIT_CB_ID                 = 0x09U,  /*!< ADC MSP 初始化回调函数 ID          */
  HAL_ADC_MSPDEINIT_CB_ID               = 0x0AU   /*!< ADC MSP 反初始化回调函数 ID        */
} HAL_ADC_CallbackIDTypeDef;

/**
  * @brief  HAL ADC 回调函数指针定义
  */
typedef  void (*pADC_CallbackTypeDef)(ADC_HandleTypeDef *hadc); /*!< 指向 ADC 回调函数的指针 */

#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

/**
  * @}
  */



/* 导出常量 --------------------------------------------------------*/

/** @defgroup ADC_Exported_Constants ADC 导出常量
  * @{
  */

/** @defgroup ADC_Error_Code ADC 错误代码
  * @{
  */
#define HAL_ADC_ERROR_NONE                0x00U   /*!< 无错误                                                */
#define HAL_ADC_ERROR_INTERNAL            0x01U   /*!< ADC IP 内部错误：时钟异常、
                                                       使能/失能故障或处于错误状态                           */
#define HAL_ADC_ERROR_OVR                 0x02U   /*!< 数据溢出错误                                          */
#define HAL_ADC_ERROR_DMA                 0x04U   /*!< DMA 传输错误                                          */

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
#define HAL_ADC_ERROR_INVALID_CALLBACK  (0x10U)   /*!< 无效回调错误 */
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
/**
  * @}
  */


/** @defgroup ADC_Data_align ADC 数据对齐方式
  * @{
  */
#define ADC_DATAALIGN_RIGHT      0x00000000U
#define ADC_DATAALIGN_LEFT       ((uint32_t)ADC_CR2_ALIGN)
/**
  * @}
  */

/** @defgroup ADC_Scan_mode ADC 扫描模式
  * @{
  */
/* 注意：扫描模式的取值并非二选一的 ENABLE/DISABLE，这是为了与具有包含额外选项的  */
/*       序列器的其他 STM32 系列器件保持兼容。                                */
/*                                                                            */
#define ADC_SCAN_DISABLE         0x00000000U
#define ADC_SCAN_ENABLE          ((uint32_t)ADC_CR1_SCAN)
/**
  * @}
  */

/** @defgroup ADC_External_trigger_edge_Regular 规则组 ADC 外部触发使能
  * @{
  */
#define ADC_EXTERNALTRIGCONVEDGE_NONE           0x00000000U
#define ADC_EXTERNALTRIGCONVEDGE_RISING         ((uint32_t)ADC_CR2_EXTTRIG)
/**
  * @}
  */

/** @defgroup ADC_channels ADC 通道
  * @{
  */
/* 注意：视具体器件而定，某些通道可能未引出至封装引脚上。                    */
/*       有关通道的可用性，请参阅器件数据手册。                               */
#define ADC_CHANNEL_0                       0x00000000U
#define ADC_CHANNEL_1           ((uint32_t)(                                                                    ADC_SQR3_SQ1_0))
#define ADC_CHANNEL_2           ((uint32_t)(                                                   ADC_SQR3_SQ1_1                 ))
#define ADC_CHANNEL_3           ((uint32_t)(                                                   ADC_SQR3_SQ1_1 | ADC_SQR3_SQ1_0))
#define ADC_CHANNEL_4           ((uint32_t)(                                  ADC_SQR3_SQ1_2                                  ))
#define ADC_CHANNEL_5           ((uint32_t)(                                  ADC_SQR3_SQ1_2                  | ADC_SQR3_SQ1_0))
#define ADC_CHANNEL_6           ((uint32_t)(                                  ADC_SQR3_SQ1_2 | ADC_SQR3_SQ1_1                 ))
#define ADC_CHANNEL_7           ((uint32_t)(                                  ADC_SQR3_SQ1_2 | ADC_SQR3_SQ1_1 | ADC_SQR3_SQ1_0))
#define ADC_CHANNEL_8           ((uint32_t)(                 ADC_SQR3_SQ1_3                                                   ))
#define ADC_CHANNEL_9           ((uint32_t)(                 ADC_SQR3_SQ1_3                                   | ADC_SQR3_SQ1_0))
#define ADC_CHANNEL_10          ((uint32_t)(                 ADC_SQR3_SQ1_3                  | ADC_SQR3_SQ1_1                 ))
#define ADC_CHANNEL_11          ((uint32_t)(                 ADC_SQR3_SQ1_3                  | ADC_SQR3_SQ1_1 | ADC_SQR3_SQ1_0))
#define ADC_CHANNEL_12          ((uint32_t)(                 ADC_SQR3_SQ1_3 | ADC_SQR3_SQ1_2                                  ))
#define ADC_CHANNEL_13          ((uint32_t)(                 ADC_SQR3_SQ1_3 | ADC_SQR3_SQ1_2                  | ADC_SQR3_SQ1_0))
#define ADC_CHANNEL_14          ((uint32_t)(                 ADC_SQR3_SQ1_3 | ADC_SQR3_SQ1_2 | ADC_SQR3_SQ1_1                 ))
#define ADC_CHANNEL_15          ((uint32_t)(                 ADC_SQR3_SQ1_3 | ADC_SQR3_SQ1_2 | ADC_SQR3_SQ1_1 | ADC_SQR3_SQ1_0))
#define ADC_CHANNEL_16          ((uint32_t)(ADC_SQR3_SQ1_4                                                                    ))
#define ADC_CHANNEL_17          ((uint32_t)(ADC_SQR3_SQ1_4                                                    | ADC_SQR3_SQ1_0))

#define ADC_CHANNEL_TEMPSENSOR  ADC_CHANNEL_16  /* ADC 内部通道（未连接到器件引脚） */
#define ADC_CHANNEL_VREFINT     ADC_CHANNEL_17  /* ADC 内部通道（未连接到器件引脚） */
/**
  * @}
  */

/** @defgroup ADC_sampling_times ADC 采样时间
  * @{
  */
#define ADC_SAMPLETIME_1CYCLE_5                   0x00000000U                                              /*!< 采样时间 1.5 个 ADC 时钟周期 */
#define ADC_SAMPLETIME_7CYCLES_5      ((uint32_t)(                                      ADC_SMPR2_SMP0_0)) /*!< 采样时间 7.5 个 ADC 时钟周期 */
#define ADC_SAMPLETIME_13CYCLES_5     ((uint32_t)(                   ADC_SMPR2_SMP0_1                   )) /*!< 采样时间 13.5 个 ADC 时钟周期 */
#define ADC_SAMPLETIME_28CYCLES_5     ((uint32_t)(                   ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_0)) /*!< 采样时间 28.5 个 ADC 时钟周期 */
#define ADC_SAMPLETIME_41CYCLES_5     ((uint32_t)(ADC_SMPR2_SMP0_2                                      )) /*!< 采样时间 41.5 个 ADC 时钟周期 */
#define ADC_SAMPLETIME_55CYCLES_5     ((uint32_t)(ADC_SMPR2_SMP0_2                    | ADC_SMPR2_SMP0_0)) /*!< 采样时间 55.5 个 ADC 时钟周期 */
#define ADC_SAMPLETIME_71CYCLES_5     ((uint32_t)(ADC_SMPR2_SMP0_2 | ADC_SMPR2_SMP0_1                   )) /*!< 采样时间 71.5 个 ADC 时钟周期 */
#define ADC_SAMPLETIME_239CYCLES_5    ((uint32_t)(ADC_SMPR2_SMP0_2 | ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_0)) /*!< 采样时间 239.5 个 ADC 时钟周期 */
/**
  * @}
  */

/** @defgroup ADC_regular_rank 规则组中的 ADC 转换顺序
  * @{
  */
#define ADC_REGULAR_RANK_1                 0x00000001U
#define ADC_REGULAR_RANK_2                 0x00000002U
#define ADC_REGULAR_RANK_3                 0x00000003U
#define ADC_REGULAR_RANK_4                 0x00000004U
#define ADC_REGULAR_RANK_5                 0x00000005U
#define ADC_REGULAR_RANK_6                 0x00000006U
#define ADC_REGULAR_RANK_7                 0x00000007U
#define ADC_REGULAR_RANK_8                 0x00000008U
#define ADC_REGULAR_RANK_9                 0x00000009U
#define ADC_REGULAR_RANK_10                0x0000000AU
#define ADC_REGULAR_RANK_11                0x0000000BU
#define ADC_REGULAR_RANK_12                0x0000000CU
#define ADC_REGULAR_RANK_13                0x0000000DU
#define ADC_REGULAR_RANK_14                0x0000000EU
#define ADC_REGULAR_RANK_15                0x0000000FU
#define ADC_REGULAR_RANK_16                0x00000010U
/**
  * @}
  */

/** @defgroup ADC_analog_watchdog_mode ADC 模拟看门狗模式
  * @{
  */
#define ADC_ANALOGWATCHDOG_NONE                             0x00000000U
#define ADC_ANALOGWATCHDOG_SINGLE_REG           ((uint32_t)(ADC_CR1_AWDSGL | ADC_CR1_AWDEN))
#define ADC_ANALOGWATCHDOG_SINGLE_INJEC         ((uint32_t)(ADC_CR1_AWDSGL | ADC_CR1_JAWDEN))
#define ADC_ANALOGWATCHDOG_SINGLE_REGINJEC      ((uint32_t)(ADC_CR1_AWDSGL | ADC_CR1_AWDEN | ADC_CR1_JAWDEN))
#define ADC_ANALOGWATCHDOG_ALL_REG              ((uint32_t)ADC_CR1_AWDEN)
#define ADC_ANALOGWATCHDOG_ALL_INJEC            ((uint32_t)ADC_CR1_JAWDEN)
#define ADC_ANALOGWATCHDOG_ALL_REGINJEC         ((uint32_t)(ADC_CR1_AWDEN | ADC_CR1_JAWDEN))
/**
  * @}
  */

/** @defgroup ADC_conversion_group ADC 转换组
  * @{
  */
#define ADC_REGULAR_GROUP             ((uint32_t)(ADC_FLAG_EOC))
#define ADC_INJECTED_GROUP            ((uint32_t)(ADC_FLAG_JEOC))
#define ADC_REGULAR_INJECTED_GROUP    ((uint32_t)(ADC_FLAG_EOC | ADC_FLAG_JEOC))
/**
  * @}
  */

/** @defgroup ADC_Event_type ADC 事件类型
  * @{
  */
#define ADC_AWD_EVENT               ((uint32_t)ADC_FLAG_AWD)   /*!< ADC 模拟看门狗事件 */

#define ADC_AWD1_EVENT              ADC_AWD_EVENT              /*!< ADC 模拟看门狗 1 事件：备用命名，用于兼容具有多个模拟看门狗的其他 STM32 设备 */
/**
  * @}
  */

/** @defgroup ADC_interrupts_definition ADC 中断定义
  * @{
  */
#define ADC_IT_EOC           ADC_CR1_EOCIE        /*!< ADC 规则组转换结束中断源 */
#define ADC_IT_JEOC          ADC_CR1_JEOCIE       /*!< ADC 注入组转换结束中断源 */
#define ADC_IT_AWD           ADC_CR1_AWDIE        /*!< ADC 模拟看门狗中断源 */
/**
  * @}
  */

/** @defgroup ADC_flags_definition ADC 标志位定义
  * @{
  */
#define ADC_FLAG_STRT          ADC_SR_STRT     /*!< ADC 规则组启动标志位 */
#define ADC_FLAG_JSTRT         ADC_SR_JSTRT    /*!< ADC 注入组启动标志位 */
#define ADC_FLAG_EOC           ADC_SR_EOC      /*!< ADC 规则组转换结束标志位 */
#define ADC_FLAG_JEOC          ADC_SR_JEOC     /*!< ADC 注入组转换结束标志位 */
#define ADC_FLAG_AWD           ADC_SR_AWD      /*!< ADC 模拟看门狗标志位 */
/**
  * @}
  */


/**
  * @}
  */ 

/* 私有常量 ------------------------------------------------------------------*/

/** @addtogroup ADC_Private_Constants ADC 私有常量
  * @{
  */

/** @defgroup ADC_conversion_cycles ADC 转换周期数
  * @{
  */
/* ADC 转换周期（单位：ADC 时钟周期）                                      */
/* （所选采样时间 + 12.5 个 ADC 时钟周期的转换时间，                          */
/* 分辨率为 12 位）                                                         */
#define ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_1CYCLE5                  14U
#define ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_7CYCLES5                 20U
#define ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_13CYCLES5                26U
#define ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_28CYCLES5                41U
#define ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_41CYCLES5                54U
#define ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_55CYCLES5                68U
#define ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_71CYCLES5                84U
#define ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_239CYCLES5              252U
/**
  * @}
  */

/** @defgroup ADC_sampling_times_all_channels ADC 所有通道采样时间
  * @{
  */
#define ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT2                                          \
     (ADC_SMPR2_SMP9_2 | ADC_SMPR2_SMP8_2 | ADC_SMPR2_SMP7_2 | ADC_SMPR2_SMP6_2 |     \
      ADC_SMPR2_SMP5_2 | ADC_SMPR2_SMP4_2 | ADC_SMPR2_SMP3_2 | ADC_SMPR2_SMP2_2 |     \
      ADC_SMPR2_SMP1_2 | ADC_SMPR2_SMP0_2)
#define ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT2                                          \
     (ADC_SMPR1_SMP17_2 | ADC_SMPR1_SMP16_2 | ADC_SMPR1_SMP15_2 | ADC_SMPR1_SMP14_2 | \
      ADC_SMPR1_SMP13_2 | ADC_SMPR1_SMP12_2 | ADC_SMPR1_SMP11_2 | ADC_SMPR1_SMP10_2 )

#define ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT1                                          \
     (ADC_SMPR2_SMP9_1 | ADC_SMPR2_SMP8_1 | ADC_SMPR2_SMP7_1 | ADC_SMPR2_SMP6_1 |     \
      ADC_SMPR2_SMP5_1 | ADC_SMPR2_SMP4_1 | ADC_SMPR2_SMP3_1 | ADC_SMPR2_SMP2_1 |     \
      ADC_SMPR2_SMP1_1 | ADC_SMPR2_SMP0_1)
#define ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT1                                          \
     (ADC_SMPR1_SMP17_1 | ADC_SMPR1_SMP16_1 | ADC_SMPR1_SMP15_1 | ADC_SMPR1_SMP14_1 | \
      ADC_SMPR1_SMP13_1 | ADC_SMPR1_SMP12_1 | ADC_SMPR1_SMP11_1 | ADC_SMPR1_SMP10_1 )

#define ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT0                                          \
     (ADC_SMPR2_SMP9_0 | ADC_SMPR2_SMP8_0 | ADC_SMPR2_SMP7_0 | ADC_SMPR2_SMP6_0 |     \
      ADC_SMPR2_SMP5_0 | ADC_SMPR2_SMP4_0 | ADC_SMPR2_SMP3_0 | ADC_SMPR2_SMP2_0 |     \
      ADC_SMPR2_SMP1_0 | ADC_SMPR2_SMP0_0)
#define ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT0                                          \
     (ADC_SMPR1_SMP17_0 | ADC_SMPR1_SMP16_0 | ADC_SMPR1_SMP15_0 | ADC_SMPR1_SMP14_0 | \
      ADC_SMPR1_SMP13_0 | ADC_SMPR1_SMP12_0 | ADC_SMPR1_SMP11_0 | ADC_SMPR1_SMP10_0 )

#define ADC_SAMPLETIME_1CYCLE5_SMPR2ALLCHANNELS    0x00000000U
#define ADC_SAMPLETIME_7CYCLES5_SMPR2ALLCHANNELS   (ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT0)
#define ADC_SAMPLETIME_13CYCLES5_SMPR2ALLCHANNELS  (ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT1)
#define ADC_SAMPLETIME_28CYCLES5_SMPR2ALLCHANNELS  (ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT1 | ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT0)
#define ADC_SAMPLETIME_41CYCLES5_SMPR2ALLCHANNELS  (ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT2)
#define ADC_SAMPLETIME_55CYCLES5_SMPR2ALLCHANNELS  (ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT2 | ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT0)
#define ADC_SAMPLETIME_71CYCLES5_SMPR2ALLCHANNELS  (ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT2 | ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT1)
#define ADC_SAMPLETIME_239CYCLES5_SMPR2ALLCHANNELS (ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT2 | ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT1 | ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT0)

#define ADC_SAMPLETIME_1CYCLE5_SMPR1ALLCHANNELS    0x00000000U
#define ADC_SAMPLETIME_7CYCLES5_SMPR1ALLCHANNELS   (ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT0)
#define ADC_SAMPLETIME_13CYCLES5_SMPR1ALLCHANNELS  (ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT1)
#define ADC_SAMPLETIME_28CYCLES5_SMPR1ALLCHANNELS  (ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT1 | ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT0)
#define ADC_SAMPLETIME_41CYCLES5_SMPR1ALLCHANNELS  (ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT2)
#define ADC_SAMPLETIME_55CYCLES5_SMPR1ALLCHANNELS  (ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT2 | ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT0)
#define ADC_SAMPLETIME_71CYCLES5_SMPR1ALLCHANNELS  (ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT2 | ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT1)
#define ADC_SAMPLETIME_239CYCLES5_SMPR1ALLCHANNELS (ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT2 | ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT1 | ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT0)
/**
  * @}
  */

/* 所有转换后标志位的组合：EOC/EOS, JEOC/JEOS, OVR, AWDx */
#define ADC_FLAG_POSTCONV_ALL   (ADC_FLAG_EOC | ADC_FLAG_JEOC | ADC_FLAG_AWD )

/**
  * @}
  */


/* 导出宏 --------------------------------------------------------------------*/

/** @defgroup ADC_Exported_Macros ADC 导出宏
  * @{
  */
/* 供 HAL 驱动内部使用的宏，也可以用于 */
/* 最终用户代码中。                                                           */    

/**
  * @brief 使能 ADC 外设
  * @note 使能 ADC 需要一段延迟以满足 ADC 稳定时间
  *       （请参考器件数据手册中的参数 tSTAB）
  * @note 在 STM32F1 上，如果 ADC 已经使能，该宏将软件触发规则组
  *       转换启动。
  * @param __HANDLE__: ADC 句柄
  * @retval 无
  */
#define __HAL_ADC_ENABLE(__HANDLE__)                                           \
  (SET_BIT((__HANDLE__)->Instance->CR2, (ADC_CR2_ADON)))
    
/**
  * @brief 禁用 ADC 外设
  * @param __HANDLE__: ADC 句柄
  * @retval 无
  */
#define __HAL_ADC_DISABLE(__HANDLE__)                                          \
  (CLEAR_BIT((__HANDLE__)->Instance->CR2, (ADC_CR2_ADON)))
    
/** @brief 使能 ADC 转换结束中断。
  * @param __HANDLE__: ADC 句柄
  * @param __INTERRUPT__: ADC 中断
  *          该参数可以是以下值的任意组合：
  *            @arg ADC_IT_EOC: ADC 规则通道转换结束中断源
  *            @arg ADC_IT_JEOC: ADC 注入通道转换结束中断源
  *            @arg ADC_IT_AWD: ADC 模拟看门狗中断源
  * @retval 无
  */
#define __HAL_ADC_ENABLE_IT(__HANDLE__, __INTERRUPT__)                         \
  (SET_BIT((__HANDLE__)->Instance->CR1, (__INTERRUPT__)))
    
/** @brief 禁用 ADC 转换结束中断。
  * @param __HANDLE__: ADC 句柄
  * @param __INTERRUPT__: ADC 中断
  *          该参数可以是以下值的任意组合：
  *            @arg ADC_IT_EOC: ADC 规则通道转换结束中断源
  *            @arg ADC_IT_JEOC: ADC 注入通道转换结束中断源
  *            @arg ADC_IT_AWD: ADC 模拟看门狗中断源
  * @retval 无
  */
#define __HAL_ADC_DISABLE_IT(__HANDLE__, __INTERRUPT__)                        \
  (CLEAR_BIT((__HANDLE__)->Instance->CR1, (__INTERRUPT__)))

/** @brief  检查指定的 ADC 中断源是使能还是禁用。
  * @param __HANDLE__: ADC 句柄
  * @param __INTERRUPT__: 要检查的 ADC 中断源
  *          该参数可以是以下值的任意组合：
  *            @arg ADC_IT_EOC: ADC 规则通道转换结束中断源
  *            @arg ADC_IT_JEOC: ADC 注入通道转换结束中断源
  *            @arg ADC_IT_AWD: ADC 模拟看门狗中断源
  * @retval 无
  */
#define __HAL_ADC_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)                     \
  (((__HANDLE__)->Instance->CR1 & (__INTERRUPT__)) == (__INTERRUPT__))

/** @brief 获取所选 ADC 的标志位状态。
  * @param __HANDLE__: ADC 句柄
  * @param __FLAG__: ADC 标志位
  *          该参数可以是以下值的任意组合：
  *            @arg ADC_FLAG_STRT: ADC 规则组启动标志位
  *            @arg ADC_FLAG_JSTRT: ADC 注入组启动标志位
  *            @arg ADC_FLAG_EOC: ADC 规则通道转换结束标志位
  *            @arg ADC_FLAG_JEOC: ADC 注入通道转换结束标志位
  *            @arg ADC_FLAG_AWD: ADC 模拟看门狗标志位
  * @retval 无
  */
#define __HAL_ADC_GET_FLAG(__HANDLE__, __FLAG__)                               \
  ((((__HANDLE__)->Instance->SR) & (__FLAG__)) == (__FLAG__))
    
/** @brief 清除 ADC 的挂起标志位
  * @param __HANDLE__: ADC 句柄
  * @param __FLAG__: ADC 标志位
  *          该参数可以是以下值的任意组合：
  *            @arg ADC_FLAG_STRT: ADC 规则组启动标志位
  *            @arg ADC_FLAG_JSTRT: ADC 注入组启动标志位
  *            @arg ADC_FLAG_EOC: ADC 规则通道转换结束标志位
  *            @arg ADC_FLAG_JEOC: ADC 注入通道转换结束标志位
  *            @arg ADC_FLAG_AWD: ADC 模拟看门狗标志位
  * @retval 无
  */
#define __HAL_ADC_CLEAR_FLAG(__HANDLE__, __FLAG__)                             \
  (WRITE_REG((__HANDLE__)->Instance->SR, ~(__FLAG__)))

/** @brief  复位 ADC 句柄状态
  * @param  __HANDLE__: ADC 句柄
  * @retval 无
  */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
#define __HAL_ADC_RESET_HANDLE_STATE(__HANDLE__)                               \
  do{                                                                          \
     (__HANDLE__)->State = HAL_ADC_STATE_RESET;                                \
     (__HANDLE__)->MspInitCallback = NULL;                                     \
     (__HANDLE__)->MspDeInitCallback = NULL;                                   \
    } while(0)
#else
#define __HAL_ADC_RESET_HANDLE_STATE(__HANDLE__)                               \
  ((__HANDLE__)->State = HAL_ADC_STATE_RESET)
#endif

/**
  * @}
  */

/* 私有宏 -------------------------------------------------------------------*/

/** @defgroup ADC_Private_Macros ADC 私有宏
  * @{
  */
/* 仅供 HAL 驱动内部使用的宏，不适用于                                         */
/* 最终用户代码。                                                             */

/**
  * @brief 校验 ADC 状态：使能或禁用
  * @param __HANDLE__: ADC 句柄
  * @retval SET（ADC 已使能）或 RESET（ADC 已禁用）
  */
#define ADC_IS_ENABLE(__HANDLE__)                                              \
  ((( ((__HANDLE__)->Instance->CR2 & ADC_CR2_ADON) == ADC_CR2_ADON )           \
   ) ? SET : RESET)

/**
  * @brief 测试规则组的转换触发源是软件启动还是外部触发。
  * @param __HANDLE__: ADC 句柄
  * @retval SET（软件启动）或 RESET（外部触发）
  */
#define ADC_IS_SOFTWARE_START_REGULAR(__HANDLE__)                              \
  (READ_BIT((__HANDLE__)->Instance->CR2, ADC_CR2_EXTSEL) == ADC_SOFTWARE_START)

/**
  * @brief 测试注入组的转换触发源是软件启动还是外部触发。
  * @param __HANDLE__: ADC 句柄
  * @retval SET（软件启动）或 RESET（外部触发）
  */
#define ADC_IS_SOFTWARE_START_INJECTED(__HANDLE__)                             \
  (READ_BIT((__HANDLE__)->Instance->CR2, ADC_CR2_JEXTSEL) == ADC_INJECTED_SOFTWARE_START)

/**
  * @brief 同时清除并设置句柄 State 的特定位
  * @note: ADC_STATE_CLR_SET() 宏仅是通用宏 MODIFY_REG() 的别名，
  *        第一个参数是 ADC 句柄 State，第二个参数是待清除的位域，
  *        第三个也是最后一个参数是待设置的位域。
  * @retval 无
  */
#define ADC_STATE_CLR_SET MODIFY_REG

/**
  * @brief 清除 ADC 错误代码（将其设置为错误代码："无错误"）
  * @param __HANDLE__: ADC 句柄
  * @retval 无
  */
#define ADC_CLEAR_ERRORCODE(__HANDLE__)                                        \
  ((__HANDLE__)->ErrorCode = HAL_ADC_ERROR_NONE)

/**
  * @brief 设置规则通道序列长度对应的 ADC 转换次数。
  * @param _NbrOfConversion_: 规则通道序列长度
  * @retval 无
  */
#define ADC_SQR1_L_SHIFT(_NbrOfConversion_)                                    \
  (((_NbrOfConversion_) - (uint8_t)1) << ADC_SQR1_L_Pos)

/**
  * @brief 设置通道号在 10 到 18 之间的 ADC 采样时间。
  * @param _SAMPLETIME_: 采样时间参数。
  * @param _CHANNELNB_: 通道号。  
  * @retval 无
  */
#define ADC_SMPR1(_SAMPLETIME_, _CHANNELNB_)                                   \
  ((_SAMPLETIME_) << (ADC_SMPR1_SMP11_Pos * ((_CHANNELNB_) - 10)))

/**
  * @brief 设置通道号在 0 到 9 之间的 ADC 采样时间。
  * @param _SAMPLETIME_: 采样时间参数。
  * @param _CHANNELNB_: 通道号。  
  * @retval 无
  */
#define ADC_SMPR2(_SAMPLETIME_, _CHANNELNB_)                                   \
  ((_SAMPLETIME_) << (ADC_SMPR2_SMP1_Pos * (_CHANNELNB_)))

/**
  * @brief 设置所选规则通道的转换顺序（Rank），适用于 Rank 1 到 6。
  * @param _CHANNELNB_: 通道号。
  * @param _RANKNB_: 顺序编号（Rank）。    
  * @retval 无
  */
#define ADC_SQR3_RK(_CHANNELNB_, _RANKNB_)                                     \
  ((_CHANNELNB_) << (ADC_SQR3_SQ2_Pos * ((_RANKNB_) - 1)))

/**
  * @brief 设置所选规则通道的转换顺序（Rank），适用于 Rank 7 到 12。
  * @param _CHANNELNB_: 通道号。
  * @param _RANKNB_: 顺序编号（Rank）。    
  * @retval 无
  */
#define ADC_SQR2_RK(_CHANNELNB_, _RANKNB_)                                     \
  ((_CHANNELNB_) << (ADC_SQR2_SQ8_Pos * ((_RANKNB_) - 7)))

/**
  * @brief 设置所选规则通道的转换顺序（Rank），适用于 Rank 13 到 16。
  * @param _CHANNELNB_: 通道号。
  * @param _RANKNB_: 顺序编号（Rank）。    
  * @retval 无
  */
#define ADC_SQR1_RK(_CHANNELNB_, _RANKNB_)                                     \
  ((_CHANNELNB_) << (ADC_SQR1_SQ14_Pos * ((_RANKNB_) - 13)))

/**
  * @brief 设置注入序列长度。
  * @param _JSQR_JL_: 序列长度。
  * @retval 无
  */
#define ADC_JSQR_JL_SHIFT(_JSQR_JL_)                                           \
  (((_JSQR_JL_) -1) << ADC_JSQR_JL_Pos)

/**
  * @brief 设置所选注入通道的转换顺序（Rank）
  *        注意：在 STM32F1 系列器件上，JSQR 寄存器中的通道转换顺序位置
  *              取决于注入序列发生器中选定的转换顺序总数
  *              （转换顺序序列从 4-JL 开始）
  * @param _CHANNELNB_: 通道号。
  * @param _RANKNB_: 顺序编号（Rank）。
  * @param _JSQR_JL_: 序列长度。
  * @retval 无
  */
#define ADC_JSQR_RK_JL(_CHANNELNB_, _RANKNB_, _JSQR_JL_)                       \
  ((_CHANNELNB_) << (ADC_JSQR_JSQ2_Pos * ((4 - ((_JSQR_JL_) - (_RANKNB_))) - 1)))

/**
  * @brief 使能 ADC 连续转换模式。
  * @param _CONTINUOUS_MODE_: 连续模式。
  * @retval 无
  */
#define ADC_CR2_CONTINUOUS(_CONTINUOUS_MODE_)                                  \
  ((_CONTINUOUS_MODE_) << ADC_CR2_CONT_Pos)

/**
  * @brief 配置规则组通道间断转换的通道数。
  * @param _NBR_DISCONTINUOUS_CONV_: 间断转换的通道数。
  * @retval 无
  */
#define ADC_CR1_DISCONTINUOUS_NUM(_NBR_DISCONTINUOUS_CONV_)                    \
  (((_NBR_DISCONTINUOUS_CONV_) - 1) << ADC_CR1_DISCNUM_Pos)

/**
  * @brief 使能 ADC 扫描模式以通过序列发生器转换多个通道序列。
  * @param _SCAN_MODE_: 扫描转换模式。
  * @retval 无
  */
/* 注意：出于兼容旧版本考虑，扫描模式与 ENABLE 进行比较，该参数               */
/*       等效于 ADC_SCAN_ENABLE。                                             */
#define ADC_CR1_SCAN_SET(_SCAN_MODE_)                                          \
  (( ((_SCAN_MODE_) == ADC_SCAN_ENABLE) || ((_SCAN_MODE_) == ENABLE)           \
   )? (ADC_SCAN_ENABLE) : (ADC_SCAN_DISABLE)                                   \
  )

/**
  * @brief 获取所有通道上的最大 ADC 转换周期数。
  * 返回所选的采样时间 + 转换时间（12.5 个 ADC 时钟周期）
  * 采样时间在 4 个区间内进行近似取值，返回最高值：
  *   低于 7.5 个周期 {1.5 个周期；7.5 个周期}，
  *   在 13.5 个周期至 28.5 个周期之间 {13.5 个周期；28.5 个周期}
  *   在 41.5 个周期至 71.5 个周期之间 {41.5 个周期；55.5 个周期；71.5 个周期}
  *   等于 239.5 个周期
  * 单位：ADC 时钟周期
  * @param __HANDLE__: ADC 句柄
  * @retval 所有通道上的 ADC 转换周期数
  */   
#define ADC_CONVCYCLES_MAX_RANGE(__HANDLE__)                                                                     \
    (( (((__HANDLE__)->Instance->SMPR2 & ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT2) == RESET)  &&                     \
       (((__HANDLE__)->Instance->SMPR1 & ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT2) == RESET) ) ?                     \
                                                                                                                 \
          (( (((__HANDLE__)->Instance->SMPR2 & ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT1) == RESET)  &&               \
             (((__HANDLE__)->Instance->SMPR1 & ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT1) == RESET) ) ?               \
               ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_7CYCLES5 : ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_28CYCLES5)   \
          :                                                                                                      \
          ((((((__HANDLE__)->Instance->SMPR2 & ADC_SAMPLETIME_ALLCHANNELS_SMPR2BIT1) == RESET)  &&               \
             (((__HANDLE__)->Instance->SMPR1 & ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT1) == RESET)) ||               \
            ((((__HANDLE__)->Instance->SMPR2 & ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT0) == RESET)  &&               \
             (((__HANDLE__)->Instance->SMPR1 & ADC_SAMPLETIME_ALLCHANNELS_SMPR1BIT0) == RESET))) ?               \
               ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_71CYCLES5 : ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_239CYCLES5) \
     )

#define IS_ADC_DATA_ALIGN(ALIGN) (((ALIGN) == ADC_DATAALIGN_RIGHT) || \
                                  ((ALIGN) == ADC_DATAALIGN_LEFT)    )

#define IS_ADC_SCAN_MODE(SCAN_MODE) (((SCAN_MODE) == ADC_SCAN_DISABLE) || \
                                     ((SCAN_MODE) == ADC_SCAN_ENABLE)    )

#define IS_ADC_EXTTRIG_EDGE(EDGE) (((EDGE) == ADC_EXTERNALTRIGCONVEDGE_NONE)  || \
                                   ((EDGE) == ADC_EXTERNALTRIGCONVEDGE_RISING)  )

#define IS_ADC_CHANNEL(CHANNEL) (((CHANNEL) == ADC_CHANNEL_0)           || \
                                 ((CHANNEL) == ADC_CHANNEL_1)           || \
                                 ((CHANNEL) == ADC_CHANNEL_2)           || \
                                 ((CHANNEL) == ADC_CHANNEL_3)           || \
                                 ((CHANNEL) == ADC_CHANNEL_4)           || \
                                 ((CHANNEL) == ADC_CHANNEL_5)           || \
                                 ((CHANNEL) == ADC_CHANNEL_6)           || \
                                 ((CHANNEL) == ADC_CHANNEL_7)           || \
                                 ((CHANNEL) == ADC_CHANNEL_8)           || \
                                 ((CHANNEL) == ADC_CHANNEL_9)           || \
                                 ((CHANNEL) == ADC_CHANNEL_10)          || \
                                 ((CHANNEL) == ADC_CHANNEL_11)          || \
                                 ((CHANNEL) == ADC_CHANNEL_12)          || \
                                 ((CHANNEL) == ADC_CHANNEL_13)          || \
                                 ((CHANNEL) == ADC_CHANNEL_14)          || \
                                 ((CHANNEL) == ADC_CHANNEL_15)          || \
                                 ((CHANNEL) == ADC_CHANNEL_16)          || \
                                 ((CHANNEL) == ADC_CHANNEL_17)            )

#define IS_ADC_SAMPLE_TIME(TIME) (((TIME) == ADC_SAMPLETIME_1CYCLE_5)    || \
                                  ((TIME) == ADC_SAMPLETIME_7CYCLES_5)   || \
                                  ((TIME) == ADC_SAMPLETIME_13CYCLES_5)  || \
                                  ((TIME) == ADC_SAMPLETIME_28CYCLES_5)  || \
                                  ((TIME) == ADC_SAMPLETIME_41CYCLES_5)  || \
                                  ((TIME) == ADC_SAMPLETIME_55CYCLES_5)  || \
                                  ((TIME) == ADC_SAMPLETIME_71CYCLES_5)  || \
                                  ((TIME) == ADC_SAMPLETIME_239CYCLES_5)   )

#define IS_ADC_REGULAR_RANK(CHANNEL) (((CHANNEL) == ADC_REGULAR_RANK_1 ) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_2 ) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_3 ) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_4 ) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_5 ) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_6 ) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_7 ) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_8 ) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_9 ) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_10) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_11) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_12) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_13) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_14) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_15) || \
                                      ((CHANNEL) == ADC_REGULAR_RANK_16)   )

#define IS_ADC_ANALOG_WATCHDOG_MODE(WATCHDOG) (((WATCHDOG) == ADC_ANALOGWATCHDOG_NONE)             || \
                                               ((WATCHDOG) == ADC_ANALOGWATCHDOG_SINGLE_REG)       || \
                                               ((WATCHDOG) == ADC_ANALOGWATCHDOG_SINGLE_INJEC)     || \
                                               ((WATCHDOG) == ADC_ANALOGWATCHDOG_SINGLE_REGINJEC)  || \
                                               ((WATCHDOG) == ADC_ANALOGWATCHDOG_ALL_REG)          || \
                                               ((WATCHDOG) == ADC_ANALOGWATCHDOG_ALL_INJEC)        || \
                                               ((WATCHDOG) == ADC_ANALOGWATCHDOG_ALL_REGINJEC)       )

#define IS_ADC_CONVERSION_GROUP(CONVERSION) (((CONVERSION) == ADC_REGULAR_GROUP)         || \
                                             ((CONVERSION) == ADC_INJECTED_GROUP)        || \
                                             ((CONVERSION) == ADC_REGULAR_INJECTED_GROUP)  )

#define IS_ADC_EVENT_TYPE(EVENT) ((EVENT) == ADC_AWD_EVENT)


/** @defgroup ADC_range_verification ADC 范围校验
  * 适用于唯一的 ADC 分辨率：12 位
  * @{
  */
#define IS_ADC_RANGE(ADC_VALUE) ((ADC_VALUE) <= 0x0FFFU)
/**
  * @}
  */

/** @defgroup ADC_regular_nb_conv_verification ADC 规则通道转换次数校验
  * @{
  */
#define IS_ADC_REGULAR_NB_CONV(LENGTH) (((LENGTH) >= 1U) && ((LENGTH) <= 16U))
/**
  * @}
  */

/** @defgroup ADC_regular_discontinuous_mode_number_verification ADC 规则组间断模式转换数目校验
  * @{
  */
#define IS_ADC_REGULAR_DISCONT_NUMBER(NUMBER) (((NUMBER) >= 1U) && ((NUMBER) <= 8U))
/**
  * @}
  */
      
/**
  * @}
  */
    
/* 包含 ADC HAL 扩展模块 */
#include "stm32f1xx_hal_adc_ex.h"

/* 导出函数 ------------------------------------------------------------------*/
/** @addtogroup ADC_Exported_Functions
  * @{
  */

/** @addtogroup ADC_Exported_Functions_Group1
  * @{
  */


/* 初始化和去初始化函数  **********************************/
HAL_StatusTypeDef       HAL_ADC_Init(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef       HAL_ADC_DeInit(ADC_HandleTypeDef *hadc);
void                    HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void                    HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
/* 回调注册/注销函数  ***********************************/
HAL_StatusTypeDef HAL_ADC_RegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID, pADC_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_ADC_UnRegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

/**
  * @}
  */

/* IO 操作函数  *****************************************************/

/** @addtogroup ADC_Exported_Functions_Group2
  * @{
  */


/* 阻塞模式：轮询 */
HAL_StatusTypeDef       HAL_ADC_Start(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef       HAL_ADC_Stop(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef       HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);
HAL_StatusTypeDef       HAL_ADC_PollForEvent(ADC_HandleTypeDef* hadc, uint32_t EventType, uint32_t Timeout);

/* 非阻塞模式：中断 */
HAL_StatusTypeDef       HAL_ADC_Start_IT(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef       HAL_ADC_Stop_IT(ADC_HandleTypeDef* hadc);

/* 非阻塞模式：DMA */
HAL_StatusTypeDef       HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);
HAL_StatusTypeDef       HAL_ADC_Stop_DMA(ADC_HandleTypeDef* hadc);

/* ADC 获取转换值，用于轮询或中断模式 */
uint32_t                HAL_ADC_GetValue(ADC_HandleTypeDef* hadc);

/* 用于非阻塞模式（中断和 DMA）的 ADC 中断处理程序 (IRQHandler) 及回调函数 */
void                    HAL_ADC_IRQHandler(ADC_HandleTypeDef* hadc);
void                    HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void                    HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void                    HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc);
void                    HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc);
/**
  * @}
  */


/* 外设控制函数 ***********************************************/
/** @addtogroup ADC_Exported_Functions_Group3
  * @{
  */
HAL_StatusTypeDef       HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig);
HAL_StatusTypeDef       HAL_ADC_AnalogWDGConfig(ADC_HandleTypeDef* hadc, ADC_AnalogWDGConfTypeDef* AnalogWDGConfig);
/**
  * @}
  */


/* 外设状态函数 *************************************************/
/** @addtogroup ADC_Exported_Functions_Group4
  * @{
  */
uint32_t                HAL_ADC_GetState(ADC_HandleTypeDef* hadc);
uint32_t                HAL_ADC_GetError(ADC_HandleTypeDef *hadc);
/**
  * @}
  */


/**
  * @}
  */


/* HAL 驱动内部函数 **********************************************/
/** @addtogroup ADC_Private_Functions
  * @{
  */
HAL_StatusTypeDef ADC_Enable(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef ADC_ConversionStop_Disable(ADC_HandleTypeDef* hadc);
void              ADC_StabilizationTime(uint32_t DelayUs);
void              ADC_DMAConvCplt(DMA_HandleTypeDef *hdma);
void              ADC_DMAHalfConvCplt(DMA_HandleTypeDef *hdma);
void              ADC_DMAError(DMA_HandleTypeDef *hdma);
/**
  * @}
  */ 


/**
  * @}
  */ 

/**
  * @}
  */

#ifdef __cplusplus
}
#endif


#endif /* __STM32F1xx_HAL_ADC_H */

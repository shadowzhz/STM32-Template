/**
  ******************************************************************************
  * @file    stm32f1xx_hal_adc_ex.h
  * @author  MCD 应用团队
  * @brief   ADC HAL 扩展模块的头文件。
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * 保留所有权利。
  *
  * 本软件根据可在该软件组件根目录下的 LICENSE 文件中找到的条款获得许可。
  * 如果本软件未随附 LICENSE 文件，则按“原样”提供。
  *
  ******************************************************************************
  */

/* 防止递归包含的宏定义 -----------------------------------------------------*/
#ifndef __STM32F1xx_HAL_ADC_EX_H
#define __STM32F1xx_HAL_ADC_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"  

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup ADCEx
  * @{
  */ 

/* 导出的类型 ------------------------------------------------------------*/ 
/** @defgroup ADCEx_Exported_Types ADCEx 导出的类型
  * @{
  */

/** 
  * @brief  ADC 注入通道配置结构体定义
  * @note   该结构体的参数分布在 2 个作用域中：
  *          - 通道级作用域：InjectedChannel、InjectedRank、InjectedSamplingTime、InjectedOffset
  *          - 注入组级作用域（影响注入组的所有通道）：InjectedNbrOfConversion、InjectedDiscontinuousConvMode、
  *            AutoInjectedConv、ExternalTrigInjecConvEdge、ExternalTrigInjecConv。
  * @note   使用函数 HAL_ADCEx_InjectedConfigChannel() 配置这些参数取决于 ADC 状态。
  *         ADC 状态可以是：
  *          - 针对所有参数：ADC 处于禁用状态（这是修改参数 'ExternalTrigInjecConv' 的唯一可用 ADC 状态）
  *          - 针对除参数 'ExternalTrigInjecConv' 之外的所有参数：ADC 处于启用状态，且注入组没有正在进行的转换。
  */
typedef struct 
{
  uint32_t InjectedChannel;                       /*!< 选择要配置的 ADC 通道
                                                       该参数可以是 @ref ADC_channels 中的一个值
                                                       注意：根据器件型号不同，某些通道可能在封装引脚上不可用。通道可用性请参阅器件数据手册。
                                                       注意：在具有多个 ADC 的 STM32F1 器件上：仅 ADC1 可以访问内部测量通道（VrefInt/TempSensor）
                                                       注意：在 STM32F10xx8 和 STM32F10xxB 器件上：当 ADC 通过注入触发进行转换时，PA0 引脚上（ADC 输入通道 0）可能会产生低幅度电压毛刺。
                                                             建议合理分配模拟通道，将通道 0 配置为注入通道。
                                                             有关详细信息，请参阅这些器件的勘误手册（Errata Sheet）。 */
  uint32_t InjectedRank;                          /*!< 注入组序列发生器中的排序（Rank）
                                                       该参数必须是 @ref ADCEx_injected_rank 中的一个值
                                                       注意：如果需要禁用通道或更改转换序列顺序，包含先前通道设置的排序位置可以被新通道设置覆盖（或者可以调整转换通道数目参数） */
  uint32_t InjectedSamplingTime;                  /*!< 为所选通道设置的采样时间值。
                                                       单位：ADC 时钟周期
                                                       转换时间为采样时间与处理时间之和（在 12 位 ADC 分辨率下为 12.5 个 ADC 时钟周期）。
                                                       该参数可以是 @ref ADC_sampling_times 中的一个值
                                                       注意：此参数会更新该通道的参数属性，该属性可用于规则组和/或注入组。
                                                                如果同一通道之前已在另一组（规则/注入）中配置，它将被更新为最新的设置。
                                                       注意：在使用内部测量通道（VrefInt/TempSensor）的情况下，
                                                             必须遵守采样时间约束（采样时间可根据 ADC 时钟频率和采样时间设置进行调整）
                                                             有关时间参数值（参数 TS_vrefint、TS_temp，大致范围：最小 5us 至 17.1us），请参阅器件数据手册。 */
  uint32_t InjectedOffset;                        /*!< 定义要从原始转换数据中减去的偏移量（仅适用于注入组中设置的通道）。
                                                       偏移值必须为正数。
                                                       根据所选的 ADC 分辨率（12、10、8 或 6 位），
                                                       该参数必须分别介于 Min_Data = 0x000 与 Max_Data = 0xFFF、0x3FF、0xFF 或 0x3F 之间。 */
  uint32_t InjectedNbrOfConversion;               /*!< 指定将在注入组序列发生器中进行转换的排序（Rank）数量。
                                                       要使用注入组序列发生器并转换多个排序，必须启用参数 'ScanConvMode'。
                                                       该参数必须是介于 Min_Data = 1 与 Max_Data = 4 之间的数值。
                                                       注意：此设置会影响整个注入组。因此，调用 HAL_ADCEx_InjectedConfigChannel() 
                                                                配置注入组上的通道可能会影响先前设置的其他通道的配置。 */
  FunctionalState InjectedDiscontinuousConvMode;  /*!< 指定注入组的转换序列是以完整序列模式还是间断模式（主序列被细分为连续的几个部分）执行。
                                                       仅在序列发生器启用时（参数 'ScanConvMode'）才使用间断模式。如果序列发生器被禁用，则忽略此参数。
                                                       只有在连续模式被禁用时才能启用间断模式。如果启用了连续模式，则忽略此参数设置。
                                                       该参数可设置为 ENABLE 或 DISABLE。
                                                       注意：对于注入组，间断模式的排序步进固定为逐个执行。
                                                       注意：此设置会影响整个注入组。因此，调用 HAL_ADCEx_InjectedConfigChannel() 
                                                                配置注入组上的通道可能会影响先前设置的其他通道的配置。 */
  FunctionalState AutoInjectedConv;               /*!< 启用或禁用所选 ADC 在规则转换后自动执行注入组转换（JAUTO）
                                                       该参数可设置为 ENABLE 或 DISABLE。
                                                       注意：要使用自动注入转换，必须禁用间断模式（'DiscontinuousConvMode' 和 'InjectedDiscontinuousConvMode' 设置为 DISABLE）
                                                       注意：要使用自动注入转换，必须禁用注入组外部触发（'ExternalTrigInjecConv' 设置为 ADC_SOFTWARE_START）
                                                       注意：如果规则组使用 DMA：若 DMA 配置为正常模式（单次传输），JAUTO 将在 DMA 传输完成时停止。
                                                             要保持 JAUTO 始终启用，必须将 DMA 配置为循环模式。
                                                       注意：此设置会影响整个注入组。因此，调用 HAL_ADCEx_InjectedConfigChannel() 
                                                                配置注入组上的通道可能会影响先前设置的其他通道的配置。 */
  uint32_t ExternalTrigInjecConv;                 /*!< 选择用于触发注入组转换启动的外部事件。
                                                       如果设置为 ADC_INJECTED_SOFTWARE_START，则禁用外部触发。
                                                       如果设置为外部触发源，则在事件上升沿触发。
                                                       该参数可以是 @ref ADCEx_External_trigger_source_Injected 中的一个值。
                                                       注意：该参数必须在 ADC 禁用时修改（ADC 启动转换之前或停止转换之后）。
                                                             如果 ADC 已使能，该参数设置将被忽略且不报错（因为在运行中更新其他参数时这可能是预期行为）。
                                                       注意：此设置会影响整个注入组。因此，调用 HAL_ADCEx_InjectedConfigChannel() 配置注入组中的某个通道
                                                             可能会影响先前已设置的其他通道的配置。 */
}ADC_InjectionConfTypeDef;

#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
/** 
  * @brief  ADC 多重模式结构体定义
  * @note   使用函数 HAL_ADCEx_MultiModeConfigChannel() 设置这些参数取决于 ADC 的状态（共用组的两个 ADC）。
  *         共用组的 ADC 状态必须为：已禁用。
  */
typedef struct
{
  uint32_t Mode;              /*!< 配置 ADC 工作在独立模式或多重模式。
                                   该参数可以是 @ref ADCEx_Common_mode 中的一个值。
                                   注意：在双重模式下，更改通道配置会引发重启，可能导致失去同步。建议在进行任何配置更改前禁用双重模式。
                                   注意：如果使用同步模式：ADC1 和 ADC2 同时采样的两个通道必须配置完全相同的采样时间。
                                   注意：如果使用交替模式：为避免转换重叠，快速交替模式允许的最大采样时间为 7 个 ADC 时钟周期，慢速交替模式为 14 个 ADC 时钟周期。
                                   注意：部分多重模式参数在 STM32F1 上是固定的，但在其他具有多个 ADC 的 STM32 器件上可配置（多重模式配置结构体可能包含额外参数）。
                                         等效关系如下：
                                           - 参数 'DMAAccessMode'：在 STM32F1 上，该参数固定为 1 个 DMA 通道（两个 ADC 共用一个 DMA 通道，即 ADC 主机 DMA）。在其他具有多个 ADC 的 STM32 器件上，等效于参数 'ADC_DMAACCESSMODE_12_10_BITS'。
                                           - 参数 'TwoSamplingDelay'：在 STM32F1 上，根据选择的快速或慢速交替模式，该参数固定为 7 或 14 个 ADC 时钟周期。在其他具有多个 ADC 的 STM32 器件上，等效于参数 'ADC_TWOSAMPLINGDELAY_7CYCLES'（针对快速交替模式）。 */

  
}ADC_MultiModeTypeDef;                                                          
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */

/**
  * @}
  */


/* 导出常量 ------------------------------------------------------------------*/
   
/** @defgroup ADCEx_Exported_Constants ADCEx 导出常量
  * @{
  */

/** @defgroup ADCEx_injected_rank ADCEx 注入组排序
  * @{
  */
#define ADC_INJECTED_RANK_1                           0x00000001U
#define ADC_INJECTED_RANK_2                           0x00000002U
#define ADC_INJECTED_RANK_3                           0x00000003U
#define ADC_INJECTED_RANK_4                           0x00000004U
/**
  * @}
  */

/** @defgroup ADCEx_External_trigger_edge_Injected ADCEx 注入组外部触发使能
  * @{
  */
#define ADC_EXTERNALTRIGINJECCONV_EDGE_NONE           0x00000000U
#define ADC_EXTERNALTRIGINJECCONV_EDGE_RISING         ((uint32_t)ADC_CR2_JEXTTRIG)
/**
  * @}
  */
    
/** @defgroup ADC_External_trigger_source_Regular ADC 规则组外部触发源选择
  * @{
  */
/*!< 具有通用触发名称的外部触发列表，与                                       */
/* 目标 ADC 无关，按触发名称排序：                                            */

/*!< 仅适用于 ADC1 和 ADC2 的规则组外部触发 */
#define ADC_EXTERNALTRIGCONV_T1_CC1         ADC1_2_EXTERNALTRIG_T1_CC1
#define ADC_EXTERNALTRIGCONV_T1_CC2         ADC1_2_EXTERNALTRIG_T1_CC2
#define ADC_EXTERNALTRIGCONV_T2_CC2         ADC1_2_EXTERNALTRIG_T2_CC2
#define ADC_EXTERNALTRIGCONV_T3_TRGO        ADC1_2_EXTERNALTRIG_T3_TRGO
#define ADC_EXTERNALTRIGCONV_T4_CC4         ADC1_2_EXTERNALTRIG_T4_CC4
#define ADC_EXTERNALTRIGCONV_EXT_IT11       ADC1_2_EXTERNALTRIG_EXT_IT11

#if defined (STM32F103xE) || defined (STM32F103xG)
/*!< 仅适用于 ADC3 的规则组外部触发源 */
#define ADC_EXTERNALTRIGCONV_T2_CC3         ADC3_EXTERNALTRIG_T2_CC3
#define ADC_EXTERNALTRIGCONV_T3_CC1         ADC3_EXTERNALTRIG_T3_CC1
#define ADC_EXTERNALTRIGCONV_T5_CC1         ADC3_EXTERNALTRIG_T5_CC1
#define ADC_EXTERNALTRIGCONV_T5_CC3         ADC3_EXTERNALTRIG_T5_CC3
#define ADC_EXTERNALTRIGCONV_T8_CC1         ADC3_EXTERNALTRIG_T8_CC1
#endif /* STM32F103xE || defined STM32F103xG */

/*!< 适用于所有 ADC 实例的规则组外部触发源 */
#define ADC_EXTERNALTRIGCONV_T1_CC3         ADC1_2_3_EXTERNALTRIG_T1_CC3

#if defined (STM32F101xE) || defined (STM32F103xE) || defined (STM32F103xG) || defined (STM32F105xC) || defined (STM32F107xC)
/*!< 注意：TIM8_TRGO 仅在超高密度（high-density）和                            */
/*         超高密度（XL-density）器件的 ADC1 与 ADC2 上可用。                    */
/*         若要在 ADC1 或 ADC2 上使用该触发源，必须通过以下宏将触发源从          */
/*         EXTI 线 11 重映射到 TIM8_TRGO：                                    */
/*           __HAL_AFIO_REMAP_ADC1_ETRGREG_ENABLE()                           */
/*           __HAL_AFIO_REMAP_ADC2_ETRGREG_ENABLE()                           */

/* 内部常量值管理说明：若 TIM8_TRGO 可用，                                    */
/* 其宏定义默认设置为 ADC1 和 ADC2 对应的值；如果选用了 ADC3，                */
/* HAL ADC 驱动程序会将其更改为适用于 ADC3 的值。                             */
#define ADC_EXTERNALTRIGCONV_T8_TRGO        ADC1_2_EXTERNALTRIG_T8_TRGO
#endif /* STM32F101xE || STM32F103xE || STM32F103xG || STM32F105xC || STM32F107xC */

#define ADC_SOFTWARE_START                  ADC1_2_3_SWSTART
/**
  * @}
  */

/** @defgroup ADCEx_External_trigger_source_Injected ADCEx 注入组外部触发源选择
  * @{
  */
/*!< 通用触发名称的外部触发列表，独立于                                        */
/* ADC 目标，按触发名称排序：                                                 */

/*!< 仅适用于 ADC1 和 ADC2 的注入组外部触发源 */
#define ADC_EXTERNALTRIGINJECCONV_T2_TRGO        ADC1_2_EXTERNALTRIGINJEC_T2_TRGO
#define ADC_EXTERNALTRIGINJECCONV_T2_CC1         ADC1_2_EXTERNALTRIGINJEC_T2_CC1
#define ADC_EXTERNALTRIGINJECCONV_T3_CC4         ADC1_2_EXTERNALTRIGINJEC_T3_CC4
#define ADC_EXTERNALTRIGINJECCONV_T4_TRGO        ADC1_2_EXTERNALTRIGINJEC_T4_TRGO 
#define ADC_EXTERNALTRIGINJECCONV_EXT_IT15       ADC1_2_EXTERNALTRIGINJEC_EXT_IT15

#if defined (STM32F103xE) || defined (STM32F103xG)
/*!< 仅适用于 ADC3 的注入组外部触发源 */
#define ADC_EXTERNALTRIGINJECCONV_T4_CC3         ADC3_EXTERNALTRIGINJEC_T4_CC3
#define ADC_EXTERNALTRIGINJECCONV_T8_CC2         ADC3_EXTERNALTRIGINJEC_T8_CC2
#define ADC_EXTERNALTRIGINJECCONV_T5_TRGO        ADC3_EXTERNALTRIGINJEC_T5_TRGO
#define ADC_EXTERNALTRIGINJECCONV_T5_CC4         ADC3_EXTERNALTRIGINJEC_T5_CC4
#endif /* STM32F103xE || defined STM32F103xG */

/*!< 适用于所有 ADC 实例的注入组外部触发源 */
#define ADC_EXTERNALTRIGINJECCONV_T1_CC4         ADC1_2_3_EXTERNALTRIGINJEC_T1_CC4
#define ADC_EXTERNALTRIGINJECCONV_T1_TRGO        ADC1_2_3_EXTERNALTRIGINJEC_T1_TRGO

#if defined (STM32F101xE) || defined (STM32F103xE) || defined (STM32F103xG) || defined (STM32F105xC) || defined (STM32F107xC)
/*!< 注意：TIM8_CC4 仅在高容量（high-density）和                              */
/*         超高容量（XL-density）器件的 ADC1 及 ADC2 上可用。                  */
/*         若要在 ADC1 或 ADC2 上使用该触发源，必须使用以下宏将触发源从         */
/*         EXTI 线 11 重映射到 TIM8_CC4：                                     */
/*           __HAL_AFIO_REMAP_ADC1_ETRGINJ_ENABLE()                           */
/*           __HAL_AFIO_REMAP_ADC2_ETRGINJ_ENABLE()                           */

/* 内部常量值管理说明：若 TIM8_CC4 可用，                                     */
/* 其定义默认设置为 ADC1&ADC2 的对应值；若选择了 ADC3，                        */
/* 则 HAL ADC 驱动程序会将其更改为 ADC3 的对应值。                            */
#define ADC_EXTERNALTRIGINJECCONV_T8_CC4         ADC1_2_EXTERNALTRIGINJEC_T8_CC4
#endif /* STM32F101xE || STM32F103xE || STM32F103xG || STM32F105xC || STM32F107xC */

#define ADC_INJECTED_SOFTWARE_START              ADC1_2_3_JSWSTART
/**
  * @}
  */

#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
/** @defgroup ADCEx_Common_mode ADC 扩展双重 ADC 模式
  * @{
  */
#define ADC_MODE_INDEPENDENT                              0x00000000U                                                                     /*!< 禁用 ADC 双重模式（ADC 独立模式） */
#define ADC_DUALMODE_REGSIMULT_INJECSIMULT    ((uint32_t)(                                                            ADC_CR1_DUALMOD_0)) /*!< 启用 ADC 双重模式：规则同步 + 注入同步组合模式，作用于规则组和注入组 */
#define ADC_DUALMODE_REGSIMULT_ALTERTRIG      ((uint32_t)(                                        ADC_CR1_DUALMOD_1                    )) /*!< 启用 ADC 双重模式：规则同步 + 交替触发组合模式，作用于规则组和注入组 */
#define ADC_DUALMODE_INJECSIMULT_INTERLFAST   ((uint32_t)(                                        ADC_CR1_DUALMOD_1 | ADC_CR1_DUALMOD_0)) /*!< 启用 ADC 双重模式：注入同步 + 快速交叉组合模式，作用于规则组和注入组（ADC 采样阶段之间的延迟：7 个 ADC 时钟周期（等同于在其他 STM32 设备上将参数 "TwoSamplingDelay" 设置为 "ADC_TWOSAMPLINGDELAY_7CYCLES"）） */
#define ADC_DUALMODE_INJECSIMULT_INTERLSLOW   ((uint32_t)(                    ADC_CR1_DUALMOD_2                                        )) /*!< 启用 ADC 双重模式：注入同步 + 慢速交叉组合模式，作用于规则组和注入组（ADC 采样阶段之间的延迟：14 个 ADC 时钟周期（等同于在其他 STM32 设备上将参数 "TwoSamplingDelay" 设置为 "ADC_TWOSAMPLINGDELAY_7CYCLES"）） */
#define ADC_DUALMODE_INJECSIMULT              ((uint32_t)(                    ADC_CR1_DUALMOD_2 |                     ADC_CR1_DUALMOD_0)) /*!< 启用 ADC 双重模式：注入同步模式，作用于注入组 */
#define ADC_DUALMODE_REGSIMULT                ((uint32_t)(                    ADC_CR1_DUALMOD_2 | ADC_CR1_DUALMOD_1                    )) /*!< 启用 ADC 双重模式：规则同步模式，作用于规则组 */
#define ADC_DUALMODE_INTERLFAST               ((uint32_t)(                    ADC_CR1_DUALMOD_2 | ADC_CR1_DUALMOD_1 | ADC_CR1_DUALMOD_0)) /*!< 启用 ADC 双重模式：快速交叉模式，作用于规则组（ADC 采样阶段之间的延迟：7 个 ADC 时钟周期（等同于在其他 STM32 设备上将参数 "TwoSamplingDelay" 设置为 "ADC_TWOSAMPLINGDELAY_7CYCLES"）） */
#define ADC_DUALMODE_INTERLSLOW               ((uint32_t)(ADC_CR1_DUALMOD_3                                                            )) /*!< 启用 ADC 双重模式：慢速交叉模式，作用于规则组（ADC 采样阶段之间的延迟：14 个 ADC 时钟周期（等同于在其他 STM32 设备上将参数 "TwoSamplingDelay" 设置为 "ADC_TWOSAMPLINGDELAY_7CYCLES"）） */
#define ADC_DUALMODE_ALTERTRIG                ((uint32_t)(ADC_CR1_DUALMOD_3 |                                         ADC_CR1_DUALMOD_0)) /*!< 启用 ADC 双重模式：交替触发模式，作用于注入组 */
/**
  * @}
  */
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */

/**
  * @}
  */


/* 私有常量 ---------------------------------------------------------*/

/** @addtogroup ADCEx_Private_Constants ADCEx 私有常量
  * @{
  */

/** @defgroup ADCEx_Internal_HAL_driver_Ext_trig_src_Regular ADC 扩展内部 HAL 驱动规则组触发源选择
  * @{
  */
/* ADC1、ADC2、ADC3 规则组外部触发源列表（如果所选器件包含                   */
/* 对应的 ADC 实例）。                                                        */
/*（供 HAL 驱动内部使用。请勿在 HAL 结构体参数中使用）                        */

/* ADC1 和 ADC2 规则组外部触发源（如果对应 ADCx 可用） */
#define ADC1_2_EXTERNALTRIG_T1_CC1                       0x00000000U
#define ADC1_2_EXTERNALTRIG_T1_CC2           ((uint32_t)(                                      ADC_CR2_EXTSEL_0))
#define ADC1_2_EXTERNALTRIG_T2_CC2           ((uint32_t)(                   ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_0))
#define ADC1_2_EXTERNALTRIG_T3_TRGO          ((uint32_t)(ADC_CR2_EXTSEL_2                                      ))
#define ADC1_2_EXTERNALTRIG_T4_CC4           ((uint32_t)(ADC_CR2_EXTSEL_2 |                    ADC_CR2_EXTSEL_0))
#define ADC1_2_EXTERNALTRIG_EXT_IT11         ((uint32_t)(ADC_CR2_EXTSEL_2 | ADC_CR2_EXTSEL_1                   ))
#if defined (STM32F101xE) || defined (STM32F103xE) || defined (STM32F103xG)
/* 注意：TIM8_TRGO 仅在超高密度（XL-density）和高密度（high-density）器件的    */
/* ADC1 和 ADC2 上可用。                                                      */
#define ADC1_2_EXTERNALTRIG_T8_TRGO          ADC1_2_EXTERNALTRIG_EXT_IT11
#endif

#if defined (STM32F103xE) || defined (STM32F103xG)
/* ADC3 规则组外部触发源 */
#define ADC3_EXTERNALTRIG_T3_CC1             ADC1_2_EXTERNALTRIG_T1_CC1
#define ADC3_EXTERNALTRIG_T2_CC3             ADC1_2_EXTERNALTRIG_T1_CC2
#define ADC3_EXTERNALTRIG_T8_CC1             ADC1_2_EXTERNALTRIG_T2_CC2
#define ADC3_EXTERNALTRIG_T8_TRGO            ADC1_2_EXTERNALTRIG_T3_TRGO
#define ADC3_EXTERNALTRIG_T5_CC1             ADC1_2_EXTERNALTRIG_T4_CC4
#define ADC3_EXTERNALTRIG_T5_CC3             ADC1_2_EXTERNALTRIG_EXT_IT11
#endif

/* ADC1、ADC2 和 ADC3 规则组外部触发源（如果对应 ADCx 可用） */
#define ADC1_2_3_EXTERNALTRIG_T1_CC3         ((uint32_t)(                   ADC_CR2_EXTSEL_1                   ))
#define ADC1_2_3_SWSTART                     ((uint32_t)(ADC_CR2_EXTSEL_2 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_0))
/**
  * @}
  */

/** @defgroup ADCEx_Internal_HAL_driver_Ext_trig_src_Injected ADC 扩展内部 HAL 驱动注入组触发源选择
  * @{
  */
/* ADC1、ADC2、ADC3 注入组外部触发源列表（如果所选器件包含                   */
/* 实例在所选器件上可用）。                                                   */
/* （仅供 HAL 驱动内部使用。请勿在 HAL 结构体参数中使用）                    */

/* ADC1 和 ADC2 的注入组外部触发源（如果 ADCx 可用） */
#define ADC1_2_EXTERNALTRIGINJEC_T2_TRGO          ((uint32_t)(                    ADC_CR2_JEXTSEL_1                    ))
#define ADC1_2_EXTERNALTRIGINJEC_T2_CC1           ((uint32_t)(                    ADC_CR2_JEXTSEL_1 | ADC_CR2_JEXTSEL_0))
#define ADC1_2_EXTERNALTRIGINJEC_T3_CC4           ((uint32_t)(ADC_CR2_JEXTSEL_2                                        ))
#define ADC1_2_EXTERNALTRIGINJEC_T4_TRGO          ((uint32_t)(ADC_CR2_JEXTSEL_2 |                     ADC_CR2_JEXTSEL_0))
#define ADC1_2_EXTERNALTRIGINJEC_EXT_IT15         ((uint32_t)(ADC_CR2_JEXTSEL_2 | ADC_CR2_JEXTSEL_1                    ))
#if defined (STM32F101xE) || defined (STM32F103xE) || defined (STM32F103xG)
/* 注意：TIM8_CC4 仅在大容量（High-density）和超大容量                        */
/* （XL-density）器件的 ADC1 和 ADC2 上可用。                                 */
#define ADC1_2_EXTERNALTRIGINJEC_T8_CC4           ADC1_2_EXTERNALTRIGINJEC_EXT_IT15
#endif

#if defined (STM32F103xE) || defined (STM32F103xG)
/* ADC3 的注入组外部触发源 */
#define ADC3_EXTERNALTRIGINJEC_T4_CC3             ADC1_2_EXTERNALTRIGINJEC_T2_TRGO
#define ADC3_EXTERNALTRIGINJEC_T8_CC2             ADC1_2_EXTERNALTRIGINJEC_T2_CC1
#define ADC3_EXTERNALTRIGINJEC_T8_CC4             ADC1_2_EXTERNALTRIGINJEC_T3_CC4
#define ADC3_EXTERNALTRIGINJEC_T5_TRGO            ADC1_2_EXTERNALTRIGINJEC_T4_TRGO
#define ADC3_EXTERNALTRIGINJEC_T5_CC4             ADC1_2_EXTERNALTRIGINJEC_EXT_IT15
#endif /* STM32F103xE || defined STM32F103xG */

/* ADC1、ADC2 和 ADC3 的注入组外部触发源（如果 ADCx 可用） */
#define ADC1_2_3_EXTERNALTRIGINJEC_T1_TRGO                    0x00000000U
#define ADC1_2_3_EXTERNALTRIGINJEC_T1_CC4         ((uint32_t)(                                        ADC_CR2_JEXTSEL_0))
#define ADC1_2_3_JSWSTART                         ((uint32_t)(ADC_CR2_JEXTSEL_2 | ADC_CR2_JEXTSEL_1 | ADC_CR2_JEXTSEL_0))
/**
  * @}
  */

/**
  * @}
  */


/* 导出宏 --------------------------------------------------------------------*/

/* 私有宏 --------------------------------------------------------------------*/

/** @defgroup ADCEx_Private_Macro ADCEx 私有宏
  * @{
  */
/* 保留供 HAL 驱动内部使用的宏，不适用于最终用户的                            */
/* 代码中。                                                                   */

    
/**
  * @brief 适用于具有 3 个 ADC 的器件：根据 ADC 所属的通用组（ADC1&ADC2 或 ADC3），
  *        定义规则组的外部触发源（某些具有相同源的触发器在写入 ADC CR2 寄存器的 
  *        EXTSEL 位时具有不同的值）。
  *        对于具有 2 个或更少 ADC 的器件：此宏不作任何修改。
  * @param __HANDLE__: ADC 句柄
  * @param __EXT_TRIG_CONV__: 为规则组选择的外部触发源。
  * @retval 待写入 CR2 寄存器 EXTSEL 位的外部触发值
  */
#if defined (STM32F103xE) || defined (STM32F103xG)
#define ADC_CFGR_EXTSEL(__HANDLE__, __EXT_TRIG_CONV__)                         \
 (( (((__HANDLE__)->Instance) == ADC3)                                         \
  )?                                                                           \
   ( ( (__EXT_TRIG_CONV__) == ADC_EXTERNALTRIGCONV_T8_TRGO                     \
     )?                                                                        \
      (ADC3_EXTERNALTRIG_T8_TRGO)                                              \
      :                                                                        \
      (__EXT_TRIG_CONV__)                                                      \
   )                                                                           \
   :                                                                           \
   (__EXT_TRIG_CONV__)                                                         \
 )
#else
#define ADC_CFGR_EXTSEL(__HANDLE__, __EXT_TRIG_CONV__)                         \
  (__EXT_TRIG_CONV__)
#endif /* STM32F103xE || STM32F103xG */

/**
  * @brief 适用于具有 3 个 ADC 的器件：根据 ADC 所属的通用组（ADC1&ADC2 或 ADC3），
  *        定义注入组的外部触发源（某些具有相同源的触发器在写入 ADC CR2 寄存器的 
  *        JEXTSEL 位时具有不同的值）。
  *        对于具有 2 个或更少 ADC 的器件：此宏不作任何修改。
  * @param __HANDLE__: ADC 句柄
  * @param __EXT_TRIG_INJECTCONV__: 为注入组选择的外部触发源。
  * @retval 待写入 CR2 寄存器 JEXTSEL 位的外部触发值
  */
#if defined (STM32F103xE) || defined (STM32F103xG)
#define ADC_CFGR_JEXTSEL(__HANDLE__, __EXT_TRIG_INJECTCONV__)                  \
 (( (((__HANDLE__)->Instance) == ADC3)                                         \
  )?                                                                           \
   ( ( (__EXT_TRIG_INJECTCONV__) == ADC_EXTERNALTRIGINJECCONV_T8_CC4           \
     )?                                                                        \
      (ADC3_EXTERNALTRIGINJEC_T8_CC4)                                          \
      :                                                                        \
      (__EXT_TRIG_INJECTCONV__)                                                \
   )                                                                           \
   :                                                                           \
   (__EXT_TRIG_INJECTCONV__)                                                   \
 )
#else
#define ADC_CFGR_JEXTSEL(__HANDLE__, __EXT_TRIG_INJECTCONV__)                  \
   (__EXT_TRIG_INJECTCONV__)
#endif /* STM32F103xE || STM32F103xG */


/**
  * @brief 校验所选 ADC 是否使能了双重/多重模式（多模式 ADC 主机或 ADC 从机）（适用于具有多个 ADC 的器件）
  * @param __HANDLE__: ADC 句柄
  * @retval 多模式状态：若禁用多模式则为 RESET，若使能多模式则为其他值
  */
#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
#define ADC_MULTIMODE_IS_ENABLE(__HANDLE__)                                    \
 (( (((__HANDLE__)->Instance) == ADC1) || (((__HANDLE__)->Instance) == ADC2)   \
  )?                                                                           \
   (ADC1->CR1 & ADC_CR1_DUALMOD)                                               \
   :                                                                           \
   (RESET)                                                                     \
 )
#else
#define ADC_MULTIMODE_IS_ENABLE(__HANDLE__)                                    \
  (RESET)
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */

/**
  * @brief 校验 ADC 启动转换的条件：ADC 必须处于非多模式下，或者处于多模式且具有 ADC 主机句柄（适用于具有多个 ADC 的器件）
  * @param __HANDLE__: ADC 句柄
  * @retval 无
  */
#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
#define ADC_NONMULTIMODE_OR_MULTIMODEMASTER(__HANDLE__)                        \
  (( (((__HANDLE__)->Instance) == ADC2)                                        \
   )?                                                                          \
    ((ADC1->CR1 & ADC_CR1_DUALMOD) == RESET)                                   \
    :                                                                          \
    (!RESET)                                                                   \
  )
#else
#define ADC_NONMULTIMODE_OR_MULTIMODEMASTER(__HANDLE__)                        \
  (!RESET)
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */

/**
  * @brief 检查 ADC 多模式设置：在多模式情况下，检查所选 ADC 的 ADC 主机是否启用了自动注入功能（适用于具有多个 ADC 的器件）
  * @param __HANDLE__: ADC 句柄
  * @retval 无
  */
#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
#define ADC_MULTIMODE_AUTO_INJECTED(__HANDLE__)                                \
  (( (((__HANDLE__)->Instance) == ADC1) || (((__HANDLE__)->Instance) == ADC2)  \
   )?                                                                          \
    (ADC1->CR1 & ADC_CR1_JAUTO)                                                \
    :                                                                          \
    (RESET)                                                                    \
  )
#else
#define ADC_MULTIMODE_AUTO_INJECTED(__HANDLE__)                                \
  (RESET)
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */

#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
/**
  * @brief 设置共享通用多模式配置的另一个 ADC 的句柄
  * @param __HANDLE__: ADC 句柄
  * @param __HANDLE_OTHER_ADC__: 另一个 ADC 句柄
  * @retval 无
  */
#define ADC_COMMON_ADC_OTHER(__HANDLE__, __HANDLE_OTHER_ADC__)                 \
  ((__HANDLE_OTHER_ADC__)->Instance = ADC2)

/**
  * @brief 设置与 ADC 主机关联的 ADC 从机句柄
  * 在 STM32F1 器件上，ADC 从机始终为 ADC2（在其他 STM32 器件上可能有所不同）
  * @param __HANDLE_MASTER__: ADC 主机句柄
  * @param __HANDLE_SLAVE__: ADC 从机句柄
  * @retval 无
  */
#define ADC_MULTI_SLAVE(__HANDLE_MASTER__, __HANDLE_SLAVE__)                   \
  ((__HANDLE_SLAVE__)->Instance = ADC2)
       
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */

#define IS_ADC_INJECTED_RANK(CHANNEL) (((CHANNEL) == ADC_INJECTED_RANK_1) || \
                                       ((CHANNEL) == ADC_INJECTED_RANK_2) || \
                                       ((CHANNEL) == ADC_INJECTED_RANK_3) || \
                                       ((CHANNEL) == ADC_INJECTED_RANK_4))

#define IS_ADC_EXTTRIGINJEC_EDGE(EDGE) (((EDGE) == ADC_EXTERNALTRIGINJECCONV_EDGE_NONE)  || \
                                        ((EDGE) == ADC_EXTERNALTRIGINJECCONV_EDGE_RISING))

/** @defgroup ADCEx_injected_nb_conv_verification ADCEx 注入通道转换数目校验
  * @{
  */
#define IS_ADC_INJECTED_NB_CONV(LENGTH)  (((LENGTH) >= 1U) && ((LENGTH) <= 4U))
/**
  * @}
  */

#if defined (STM32F100xB) || defined (STM32F100xE) || defined (STM32F101x6) || defined (STM32F101xB) || defined (STM32F102x6) || defined (STM32F102xB) || defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC)
#define IS_ADC_EXTTRIG(REGTRIG) (((REGTRIG) == ADC_EXTERNALTRIGCONV_T1_CC1)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T1_CC2)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T2_CC2)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T3_TRGO)   || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T4_CC4)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_EXT_IT11)  || \
                                 ((REGTRIG) == ADC_SOFTWARE_START))
#endif
#if defined (STM32F101xE)
#define IS_ADC_EXTTRIG(REGTRIG) (((REGTRIG) == ADC_EXTERNALTRIGCONV_T1_CC1)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T1_CC2)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T2_CC2)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T3_TRGO)   || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T4_CC4)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_EXT_IT11)  || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T8_TRGO)   || \
                                 ((REGTRIG) == ADC_SOFTWARE_START))
#endif
#if defined (STM32F101xG)
#define IS_ADC_EXTTRIG(REGTRIG) (((REGTRIG) == ADC_EXTERNALTRIGCONV_T1_CC1)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T1_CC2)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T2_CC2)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T3_TRGO)   || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T4_CC4)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_EXT_IT11)  || \
                                 ((REGTRIG) == ADC_SOFTWARE_START))
#endif
#if defined (STM32F103xE) || defined (STM32F103xG)
#define IS_ADC_EXTTRIG(REGTRIG) (((REGTRIG) == ADC_EXTERNALTRIGCONV_T1_CC1)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T1_CC2)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T2_CC2)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T3_TRGO)   || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T4_CC4)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_EXT_IT11)  || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T3_CC1)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T2_CC3)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T8_CC1)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T5_CC1)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T5_CC3)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T1_CC3)    || \
                                 ((REGTRIG) == ADC_EXTERNALTRIGCONV_T8_TRGO)   || \
                                 ((REGTRIG) == ADC_SOFTWARE_START))
#endif

#if defined (STM32F100xB) || defined (STM32F100xE) || defined (STM32F101x6) || defined (STM32F101xB) || defined (STM32F102x6) || defined (STM32F102xB) || defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC)
#define IS_ADC_EXTTRIGINJEC(REGTRIG) (((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T2_TRGO)  || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T2_CC1)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T3_CC4)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T4_TRGO)  || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_EXT_IT15) || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T1_CC4)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T1_TRGO)  || \
                                      ((REGTRIG) == ADC_INJECTED_SOFTWARE_START))
#endif
#if defined (STM32F101xE)
#define IS_ADC_EXTTRIGINJEC(REGTRIG) (((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T2_TRGO)  || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T2_CC1)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T3_CC4)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T4_TRGO)  || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_EXT_IT15) || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T1_CC4)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T1_TRGO)  || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T8_CC4)   || \
                                      ((REGTRIG) == ADC_INJECTED_SOFTWARE_START))
#endif
#if defined (STM32F101xG)
#define IS_ADC_EXTTRIGINJEC(REGTRIG) (((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T2_TRGO)  || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T2_CC1)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T3_CC4)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T4_TRGO)  || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_EXT_IT15) || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T1_CC4)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T1_TRGO)  || \
                                      ((REGTRIG) == ADC_INJECTED_SOFTWARE_START))
#endif
#if defined (STM32F103xE) || defined (STM32F103xG)
#define IS_ADC_EXTTRIGINJEC(REGTRIG) (((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T2_TRGO)  || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T2_CC1)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T3_CC4)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T4_TRGO)  || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T5_CC4)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_EXT_IT15) || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T4_CC3)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T8_CC2)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T5_TRGO)  || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T5_CC4)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T1_CC4)   || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T1_TRGO)  || \
                                      ((REGTRIG) == ADC_EXTERNALTRIGINJECCONV_T8_CC4)   || \
                                      ((REGTRIG) == ADC_INJECTED_SOFTWARE_START))
#endif

#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
#define IS_ADC_MODE(MODE) (((MODE) == ADC_MODE_INDEPENDENT)                || \
                           ((MODE) == ADC_DUALMODE_REGSIMULT_INJECSIMULT)  || \
                           ((MODE) == ADC_DUALMODE_REGSIMULT_ALTERTRIG)    || \
                           ((MODE) == ADC_DUALMODE_INJECSIMULT_INTERLFAST) || \
                           ((MODE) == ADC_DUALMODE_INJECSIMULT_INTERLSLOW) || \
                           ((MODE) == ADC_DUALMODE_INJECSIMULT)            || \
                           ((MODE) == ADC_DUALMODE_REGSIMULT)              || \
                           ((MODE) == ADC_DUALMODE_INTERLFAST)             || \
                           ((MODE) == ADC_DUALMODE_INTERLSLOW)             || \
                           ((MODE) == ADC_DUALMODE_ALTERTRIG) )
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */

/**
  * @}
  */      
   
    

    
    
   
/* 导出函数 (Exported functions) --------------------------------------------------------*/
/** @addtogroup ADCEx_Exported_Functions
  * @{
  */

/* IO 操作函数 *****************************************************/
/** @addtogroup ADCEx_Exported_Functions_Group1
  * @{
  */

/* ADC 校准 */
HAL_StatusTypeDef       HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef* hadc);

/* 阻塞模式：轮询 */
HAL_StatusTypeDef       HAL_ADCEx_InjectedStart(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef       HAL_ADCEx_InjectedStop(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef       HAL_ADCEx_InjectedPollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);

/* 非阻塞模式：中断 */
HAL_StatusTypeDef       HAL_ADCEx_InjectedStart_IT(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef       HAL_ADCEx_InjectedStop_IT(ADC_HandleTypeDef* hadc);

#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
/* ADC 双重/多重模式 */
HAL_StatusTypeDef       HAL_ADCEx_MultiModeStart_DMA(ADC_HandleTypeDef *hadc, uint32_t *pData, uint32_t Length);
HAL_StatusTypeDef       HAL_ADCEx_MultiModeStop_DMA(ADC_HandleTypeDef *hadc); 
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */

/* 获取 ADC 转换值（适用于轮询或中断模式） */
uint32_t                HAL_ADCEx_InjectedGetValue(ADC_HandleTypeDef* hadc, uint32_t InjectedRank);
#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
uint32_t                HAL_ADCEx_MultiModeGetValue(ADC_HandleTypeDef *hadc);
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */

/* 非阻塞模式（中断）下使用的 ADC IRQHandler 和回调函数 */
void                    HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc);
/**
  * @}
  */


/* 外设控制函数 ***********************************************/
/** @addtogroup ADCEx_Exported_Functions_Group2
  * @{
  */
HAL_StatusTypeDef       HAL_ADCEx_InjectedConfigChannel(ADC_HandleTypeDef* hadc,ADC_InjectionConfTypeDef* sConfigInjected);
#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
HAL_StatusTypeDef       HAL_ADCEx_MultiModeConfigChannel(ADC_HandleTypeDef *hadc, ADC_MultiModeTypeDef *multimode);
#endif /* defined STM32F103x6 || defined STM32F103xB || defined STM32F105xC || defined STM32F107xC || defined STM32F103xE || defined STM32F103xG */
/**
  * @}
  */


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

#endif /* __STM32F1xx_HAL_ADC_EX_H */

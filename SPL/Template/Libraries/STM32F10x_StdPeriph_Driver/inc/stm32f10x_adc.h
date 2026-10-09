/**
  ******************************************************************************
  * @file    stm32f10x_adc.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 ADC 固件库的全部函数原型。
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
#ifndef __STM32F10x_ADC_H
#define __STM32F10x_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup ADC   ADC 模数转换
  * @{
  */

/** @defgroup ADC_Exported_Types   ADC 导出类型
  * @{
  */

/**
  * @brief  ADC 初始化结构体定义
  */

typedef struct
{
  uint32_t ADC_Mode;                      /*!< 配置 ADC 工作在独立模式还是双重模式。
                                               该参数可取 @ref ADC_mode 中的值 */

  FunctionalState ADC_ScanConvMode;       /*!< 指定转换是在扫描（多通道）模式还是单次（单通道）模式下进行。
                                               该参数可设为 ENABLE 或 DISABLE */

  FunctionalState ADC_ContinuousConvMode; /*!< 指定转换是在连续模式还是单次模式下进行。
                                               该参数可设为 ENABLE 或 DISABLE。 */

  uint32_t ADC_ExternalTrigConv;          /*!< 定义用于启动规则通道模数转换的外部触发源。
                                               该参数可取 @ref
                                               ADC_external_trigger_sources_for_regular_channels_conversion 中的值 */

  uint32_t ADC_DataAlign;                 /*!< 指定 ADC 数据是左对齐还是右对齐。
                                               该参数可取 @ref ADC_data_align 中的值 */

  uint8_t ADC_NbrOfChannel;               /*!< 指定规则通道组将使用序列器转换的 ADC 通道数量。
                                               该参数取值范围为 1 到 16。 */
}ADC_InitTypeDef;
/**
  * @}
  */

/** @defgroup ADC_Exported_Constants   ADC 导出常量
  * @{
  */

#define IS_ADC_ALL_PERIPH(PERIPH) (((PERIPH) == ADC1) || \
                                   ((PERIPH) == ADC2) || \
                                   ((PERIPH) == ADC3))

#define IS_ADC_DMA_PERIPH(PERIPH) (((PERIPH) == ADC1) || \
                                   ((PERIPH) == ADC3))

/** @defgroup ADC_mode   ADC 工作模式
  * @{
  */

#define ADC_Mode_Independent                       ((uint32_t)0x00000000)
#define ADC_Mode_RegInjecSimult                    ((uint32_t)0x00010000)
#define ADC_Mode_RegSimult_AlterTrig               ((uint32_t)0x00020000)
#define ADC_Mode_InjecSimult_FastInterl            ((uint32_t)0x00030000)
#define ADC_Mode_InjecSimult_SlowInterl            ((uint32_t)0x00040000)
#define ADC_Mode_InjecSimult                       ((uint32_t)0x00050000)
#define ADC_Mode_RegSimult                         ((uint32_t)0x00060000)
#define ADC_Mode_FastInterl                        ((uint32_t)0x00070000)
#define ADC_Mode_SlowInterl                        ((uint32_t)0x00080000)
#define ADC_Mode_AlterTrig                         ((uint32_t)0x00090000)

#define IS_ADC_MODE(MODE) (((MODE) == ADC_Mode_Independent) || \
                           ((MODE) == ADC_Mode_RegInjecSimult) || \
                           ((MODE) == ADC_Mode_RegSimult_AlterTrig) || \
                           ((MODE) == ADC_Mode_InjecSimult_FastInterl) || \
                           ((MODE) == ADC_Mode_InjecSimult_SlowInterl) || \
                           ((MODE) == ADC_Mode_InjecSimult) || \
                           ((MODE) == ADC_Mode_RegSimult) || \
                           ((MODE) == ADC_Mode_FastInterl) || \
                           ((MODE) == ADC_Mode_SlowInterl) || \
                           ((MODE) == ADC_Mode_AlterTrig))
/**
  * @}
  */

/** @defgroup ADC_external_trigger_sources_for_regular_channels_conversion
  *           规则通道转换的外部触发源
  * @{
  */

#define ADC_ExternalTrigConv_T1_CC1                ((uint32_t)0x00000000) /*!< 适用于 ADC1 与 ADC2 */
#define ADC_ExternalTrigConv_T1_CC2                ((uint32_t)0x00020000) /*!< 适用于 ADC1 与 ADC2 */
#define ADC_ExternalTrigConv_T2_CC2                ((uint32_t)0x00060000) /*!< 适用于 ADC1 与 ADC2 */
#define ADC_ExternalTrigConv_T3_TRGO               ((uint32_t)0x00080000) /*!< 适用于 ADC1 与 ADC2 */
#define ADC_ExternalTrigConv_T4_CC4                ((uint32_t)0x000A0000) /*!< 适用于 ADC1 与 ADC2 */
#define ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO    ((uint32_t)0x000C0000) /*!< 适用于 ADC1 与 ADC2 */

#define ADC_ExternalTrigConv_T1_CC3                ((uint32_t)0x00040000) /*!< 适用于 ADC1、ADC2 与 ADC3 */
#define ADC_ExternalTrigConv_None                  ((uint32_t)0x000E0000) /*!< 适用于 ADC1、ADC2 与 ADC3 */

#define ADC_ExternalTrigConv_T3_CC1                ((uint32_t)0x00000000) /*!< 仅适用于 ADC3 */
#define ADC_ExternalTrigConv_T2_CC3                ((uint32_t)0x00020000) /*!< 仅适用于 ADC3 */
#define ADC_ExternalTrigConv_T8_CC1                ((uint32_t)0x00060000) /*!< 仅适用于 ADC3 */
#define ADC_ExternalTrigConv_T8_TRGO               ((uint32_t)0x00080000) /*!< 仅适用于 ADC3 */
#define ADC_ExternalTrigConv_T5_CC1                ((uint32_t)0x000A0000) /*!< 仅适用于 ADC3 */
#define ADC_ExternalTrigConv_T5_CC3                ((uint32_t)0x000C0000) /*!< 仅适用于 ADC3 */

#define IS_ADC_EXT_TRIG(REGTRIG) (((REGTRIG) == ADC_ExternalTrigConv_T1_CC1) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T1_CC2) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T1_CC3) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T2_CC2) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T3_TRGO) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T4_CC4) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_None) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T3_CC1) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T2_CC3) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T8_CC1) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T8_TRGO) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T5_CC1) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T5_CC3))
/**
  * @}
  */

/** @defgroup ADC_data_align   ADC 数据对齐方式
  * @{
  */

#define ADC_DataAlign_Right                        ((uint32_t)0x00000000)
#define ADC_DataAlign_Left                         ((uint32_t)0x00000800)
#define IS_ADC_DATA_ALIGN(ALIGN) (((ALIGN) == ADC_DataAlign_Right) || \
                                  ((ALIGN) == ADC_DataAlign_Left))
/**
  * @}
  */

/** @defgroup ADC_channels   ADC 通道
  * @{
  */

#define ADC_Channel_0                               ((uint8_t)0x00)
#define ADC_Channel_1                               ((uint8_t)0x01)
#define ADC_Channel_2                               ((uint8_t)0x02)
#define ADC_Channel_3                               ((uint8_t)0x03)
#define ADC_Channel_4                               ((uint8_t)0x04)
#define ADC_Channel_5                               ((uint8_t)0x05)
#define ADC_Channel_6                               ((uint8_t)0x06)
#define ADC_Channel_7                               ((uint8_t)0x07)
#define ADC_Channel_8                               ((uint8_t)0x08)
#define ADC_Channel_9                               ((uint8_t)0x09)
#define ADC_Channel_10                              ((uint8_t)0x0A)
#define ADC_Channel_11                              ((uint8_t)0x0B)
#define ADC_Channel_12                              ((uint8_t)0x0C)
#define ADC_Channel_13                              ((uint8_t)0x0D)
#define ADC_Channel_14                              ((uint8_t)0x0E)
#define ADC_Channel_15                              ((uint8_t)0x0F)
#define ADC_Channel_16                              ((uint8_t)0x10)
#define ADC_Channel_17                              ((uint8_t)0x11)

#define ADC_Channel_TempSensor                      ((uint8_t)ADC_Channel_16)
#define ADC_Channel_Vrefint                         ((uint8_t)ADC_Channel_17)

#define IS_ADC_CHANNEL(CHANNEL) (((CHANNEL) == ADC_Channel_0) || ((CHANNEL) == ADC_Channel_1) || \
                                 ((CHANNEL) == ADC_Channel_2) || ((CHANNEL) == ADC_Channel_3) || \
                                 ((CHANNEL) == ADC_Channel_4) || ((CHANNEL) == ADC_Channel_5) || \
                                 ((CHANNEL) == ADC_Channel_6) || ((CHANNEL) == ADC_Channel_7) || \
                                 ((CHANNEL) == ADC_Channel_8) || ((CHANNEL) == ADC_Channel_9) || \
                                 ((CHANNEL) == ADC_Channel_10) || ((CHANNEL) == ADC_Channel_11) || \
                                 ((CHANNEL) == ADC_Channel_12) || ((CHANNEL) == ADC_Channel_13) || \
                                 ((CHANNEL) == ADC_Channel_14) || ((CHANNEL) == ADC_Channel_15) || \
                                 ((CHANNEL) == ADC_Channel_16) || ((CHANNEL) == ADC_Channel_17))
/**
  * @}
  */

/** @defgroup ADC_sampling_time   ADC 采样时间
  * @{
  */

#define ADC_SampleTime_1Cycles5                    ((uint8_t)0x00)
#define ADC_SampleTime_7Cycles5                    ((uint8_t)0x01)
#define ADC_SampleTime_13Cycles5                   ((uint8_t)0x02)
#define ADC_SampleTime_28Cycles5                   ((uint8_t)0x03)
#define ADC_SampleTime_41Cycles5                   ((uint8_t)0x04)
#define ADC_SampleTime_55Cycles5                   ((uint8_t)0x05)
#define ADC_SampleTime_71Cycles5                   ((uint8_t)0x06)
#define ADC_SampleTime_239Cycles5                  ((uint8_t)0x07)
#define IS_ADC_SAMPLE_TIME(TIME) (((TIME) == ADC_SampleTime_1Cycles5) || \
                                  ((TIME) == ADC_SampleTime_7Cycles5) || \
                                  ((TIME) == ADC_SampleTime_13Cycles5) || \
                                  ((TIME) == ADC_SampleTime_28Cycles5) || \
                                  ((TIME) == ADC_SampleTime_41Cycles5) || \
                                  ((TIME) == ADC_SampleTime_55Cycles5) || \
                                  ((TIME) == ADC_SampleTime_71Cycles5) || \
                                  ((TIME) == ADC_SampleTime_239Cycles5))
/**
  * @}
  */

/** @defgroup ADC_external_trigger_sources_for_injected_channels_conversion
  *           注入通道转换的外部触发源
  * @{
  */

#define ADC_ExternalTrigInjecConv_T2_TRGO           ((uint32_t)0x00002000) /*!< 适用于 ADC1 与 ADC2 */
#define ADC_ExternalTrigInjecConv_T2_CC1            ((uint32_t)0x00003000) /*!< 适用于 ADC1 与 ADC2 */
#define ADC_ExternalTrigInjecConv_T3_CC4            ((uint32_t)0x00004000) /*!< 适用于 ADC1 与 ADC2 */
#define ADC_ExternalTrigInjecConv_T4_TRGO           ((uint32_t)0x00005000) /*!< 适用于 ADC1 与 ADC2 */
#define ADC_ExternalTrigInjecConv_Ext_IT15_TIM8_CC4 ((uint32_t)0x00006000) /*!< 适用于 ADC1 与 ADC2 */

#define ADC_ExternalTrigInjecConv_T1_TRGO           ((uint32_t)0x00000000) /*!< 适用于 ADC1、ADC2 与 ADC3 */
#define ADC_ExternalTrigInjecConv_T1_CC4            ((uint32_t)0x00001000) /*!< 适用于 ADC1、ADC2 与 ADC3 */
#define ADC_ExternalTrigInjecConv_None              ((uint32_t)0x00007000) /*!< 适用于 ADC1、ADC2 与 ADC3 */

#define ADC_ExternalTrigInjecConv_T4_CC3            ((uint32_t)0x00002000) /*!< 仅适用于 ADC3 */
#define ADC_ExternalTrigInjecConv_T8_CC2            ((uint32_t)0x00003000) /*!< 仅适用于 ADC3 */
#define ADC_ExternalTrigInjecConv_T8_CC4            ((uint32_t)0x00004000) /*!< 仅适用于 ADC3 */
#define ADC_ExternalTrigInjecConv_T5_TRGO           ((uint32_t)0x00005000) /*!< 仅适用于 ADC3 */
#define ADC_ExternalTrigInjecConv_T5_CC4            ((uint32_t)0x00006000) /*!< 仅适用于 ADC3 */

#define IS_ADC_EXT_INJEC_TRIG(INJTRIG) (((INJTRIG) == ADC_ExternalTrigInjecConv_T1_TRGO) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T1_CC4) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T2_TRGO) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T2_CC1) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T3_CC4) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T4_TRGO) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_Ext_IT15_TIM8_CC4) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_None) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T4_CC3) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T8_CC2) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T8_CC4) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T5_TRGO) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T5_CC4))
/**
  * @}
  */

/** @defgroup ADC_injected_channel_selection   ADC 注入通道选择
  * @{
  */

#define ADC_InjectedChannel_1                       ((uint8_t)0x14)
#define ADC_InjectedChannel_2                       ((uint8_t)0x18)
#define ADC_InjectedChannel_3                       ((uint8_t)0x1C)
#define ADC_InjectedChannel_4                       ((uint8_t)0x20)
#define IS_ADC_INJECTED_CHANNEL(CHANNEL) (((CHANNEL) == ADC_InjectedChannel_1) || \
                                          ((CHANNEL) == ADC_InjectedChannel_2) || \
                                          ((CHANNEL) == ADC_InjectedChannel_3) || \
                                          ((CHANNEL) == ADC_InjectedChannel_4))
/**
  * @}
  */

/** @defgroup ADC_analog_watchdog_selection   ADC 模拟看门狗选择
  * @{
  */

#define ADC_AnalogWatchdog_SingleRegEnable         ((uint32_t)0x00800200)
#define ADC_AnalogWatchdog_SingleInjecEnable       ((uint32_t)0x00400200)
#define ADC_AnalogWatchdog_SingleRegOrInjecEnable  ((uint32_t)0x00C00200)
#define ADC_AnalogWatchdog_AllRegEnable            ((uint32_t)0x00800000)
#define ADC_AnalogWatchdog_AllInjecEnable          ((uint32_t)0x00400000)
#define ADC_AnalogWatchdog_AllRegAllInjecEnable    ((uint32_t)0x00C00000)
#define ADC_AnalogWatchdog_None                    ((uint32_t)0x00000000)

#define IS_ADC_ANALOG_WATCHDOG(WATCHDOG) (((WATCHDOG) == ADC_AnalogWatchdog_SingleRegEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_SingleInjecEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_SingleRegOrInjecEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_AllRegEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_AllInjecEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_AllRegAllInjecEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_None))
/**
  * @}
  */

/** @defgroup ADC_interrupts_definition   ADC 中断定义
  * @{
  */

#define ADC_IT_EOC                                 ((uint16_t)0x0220)
#define ADC_IT_AWD                                 ((uint16_t)0x0140)
#define ADC_IT_JEOC                                ((uint16_t)0x0480)

#define IS_ADC_IT(IT) ((((IT) & (uint16_t)0xF81F) == 0x00) && ((IT) != 0x00))

#define IS_ADC_GET_IT(IT) (((IT) == ADC_IT_EOC) || ((IT) == ADC_IT_AWD) || \
                           ((IT) == ADC_IT_JEOC))
/**
  * @}
  */

/** @defgroup ADC_flags_definition   ADC 标志定义
  * @{
  */

#define ADC_FLAG_AWD                               ((uint8_t)0x01)
#define ADC_FLAG_EOC                               ((uint8_t)0x02)
#define ADC_FLAG_JEOC                              ((uint8_t)0x04)
#define ADC_FLAG_JSTRT                             ((uint8_t)0x08)
#define ADC_FLAG_STRT                              ((uint8_t)0x10)
#define IS_ADC_CLEAR_FLAG(FLAG) ((((FLAG) & (uint8_t)0xE0) == 0x00) && ((FLAG) != 0x00))
#define IS_ADC_GET_FLAG(FLAG) (((FLAG) == ADC_FLAG_AWD) || ((FLAG) == ADC_FLAG_EOC) || \
                               ((FLAG) == ADC_FLAG_JEOC) || ((FLAG)== ADC_FLAG_JSTRT) || \
                               ((FLAG) == ADC_FLAG_STRT))
/**
  * @}
  */

/** @defgroup ADC_thresholds   ADC 阈值
  * @{
  */

#define IS_ADC_THRESHOLD(THRESHOLD) ((THRESHOLD) <= 0xFFF)

/**
  * @}
  */

/** @defgroup ADC_injected_offset   ADC 注入通道偏移量
  * @{
  */

#define IS_ADC_OFFSET(OFFSET) ((OFFSET) <= 0xFFF)

/**
  * @}
  */

/** @defgroup ADC_injected_length   ADC 注入序列长度
  * @{
  */

#define IS_ADC_INJECTED_LENGTH(LENGTH) (((LENGTH) >= 0x1) && ((LENGTH) <= 0x4))

/**
  * @}
  */

/** @defgroup ADC_injected_rank   ADC 注入通道排序位置
  * @{
  */

#define IS_ADC_INJECTED_RANK(RANK) (((RANK) >= 0x1) && ((RANK) <= 0x4))

/**
  * @}
  */ 


/** @defgroup ADC_regular_length   ADC 规则序列长度
  * @{
  */

#define IS_ADC_REGULAR_LENGTH(LENGTH) (((LENGTH) >= 0x1) && ((LENGTH) <= 0x10))
/**
  * @}
  */

/** @defgroup ADC_regular_rank   ADC 规则通道排序位置
  * @{
  */

#define IS_ADC_REGULAR_RANK(RANK) (((RANK) >= 0x1) && ((RANK) <= 0x10))

/**
  * @}
  */

/** @defgroup ADC_regular_discontinuous_mode_number   ADC 规则组间断模式通道数
  * @{
  */

#define IS_ADC_REGULAR_DISC_NUMBER(NUMBER) (((NUMBER) >= 0x1) && ((NUMBER) <= 0x8))

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup ADC_Exported_Macros   ADC 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup ADC_Exported_Functions   ADC 导出函数
  * @{
  */

void ADC_DeInit(ADC_TypeDef* ADCx);
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState);
void ADC_ResetCalibration(ADC_TypeDef* ADCx);
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx);
void ADC_StartCalibration(ADC_TypeDef* ADCx);
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx);
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx);
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, uint8_t Number);
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_ExternalTrigConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);
uint32_t ADC_GetDualModeConversionValue(void);
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConv);
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx);
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, uint8_t Length);
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset);
uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel);
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog);
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold, uint16_t LowThreshold);
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel);
void ADC_TempSensorVrefintCmd(FunctionalState NewState);
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, uint16_t ADC_IT);
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint16_t ADC_IT);

#ifdef __cplusplus
}
#endif

#endif /*__STM32F10x_ADC_H */

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

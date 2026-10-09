/**
  ******************************************************************************
  * @file    stm32f10x_dac.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 DAC 固件库的所有函数原型。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供参考，旨在为客户提供有关其产品的编码信息，以便客户节省时间。
  * 因此，对于因本固件的内容和/或客户将本文所含编码信息用于其产品
  * 而产生的任何索赔所导致的任何直接、间接或后果性损害，
  * 意法半导体概不承担责任。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 定义以下宏，以防止本头文件被递归包含 -------------------------------------*/
#ifndef __STM32F10x_DAC_H
#define __STM32F10x_DAC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup DAC   DAC 驱动模块
  * @{
  */

/** @defgroup DAC_Exported_Types   DAC 导出类型
  * @{
  */

/**
  * @brief  DAC 初始化结构体定义
  */

typedef struct
{
  uint32_t DAC_Trigger;                      /*!< 指定所选 DAC 通道的外部触发。
                                                  该参数可以是 @ref DAC_trigger_selection 的值 */

  uint32_t DAC_WaveGeneration;               /*!< 指定是否生成 DAC 通道噪声波或三角波，
                                                  或者不生成任何波形。
                                                  该参数可以是 @ref DAC_wave_generation 的值 */

  uint32_t DAC_LFSRUnmask_TriangleAmplitude; /*!< 指定 DAC 通道噪声波生成的 LFSR 掩码，
                                                  或 DAC 通道最大幅值三角波生成。
                                                  该参数可以是 @ref DAC_lfsrunmask_triangleamplitude 的值 */

  uint32_t DAC_OutputBuffer;                 /*!< 指定 DAC 通道输出缓冲是使能还是关闭。
                                                  该参数可以是 @ref DAC_output_buffer 的值 */
}DAC_InitTypeDef;

/**
  * @}
  */

/** @defgroup DAC_Exported_Constants   DAC 导出常量
  * @{
  */

/** @defgroup DAC_trigger_selection   DAC 触发选择
  * @{
  */

#define DAC_Trigger_None                   ((uint32_t)0x00000000) /*!< 一旦载入 DAC1_DHRxxxx 寄存器，转换便自动开始，
                                                                       而非由外部触发 */
#define DAC_Trigger_T6_TRGO                ((uint32_t)0x00000004) /*!< 选择 TIM6 TRGO 作为 DAC 通道的外部转换触发 */
#define DAC_Trigger_T8_TRGO                ((uint32_t)0x0000000C) /*!< 选择 TIM8 TRGO 作为 DAC 通道的外部转换触发
                                                                       仅适用于高密度器件 */
#define DAC_Trigger_T3_TRGO                ((uint32_t)0x0000000C) /*!< 选择 TIM8 TRGO 作为 DAC 通道的外部转换触发
                                                                       仅适用于互联型、中密度和小容量价值型器件 */
#define DAC_Trigger_T7_TRGO                ((uint32_t)0x00000014) /*!< 选择 TIM7 TRGO 作为 DAC 通道的外部转换触发 */
#define DAC_Trigger_T5_TRGO                ((uint32_t)0x0000001C) /*!< 选择 TIM5 TRGO 作为 DAC 通道的外部转换触发 */
#define DAC_Trigger_T15_TRGO               ((uint32_t)0x0000001C) /*!< 选择 TIM15 TRGO 作为 DAC 通道的外部转换触发
                                                                       仅适用于中密度和小容量价值型器件 */
#define DAC_Trigger_T2_TRGO                ((uint32_t)0x00000024) /*!< 选择 TIM2 TRGO 作为 DAC 通道的外部转换触发 */
#define DAC_Trigger_T4_TRGO                ((uint32_t)0x0000002C) /*!< 选择 TIM4 TRGO 作为 DAC 通道的外部转换触发 */
#define DAC_Trigger_Ext_IT9                ((uint32_t)0x00000034) /*!< 选择 EXTI 线 9 事件作为 DAC 通道的外部转换触发 */
#define DAC_Trigger_Software               ((uint32_t)0x0000003C) /*!< DAC 通道由软件触发启动转换 */

#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_Trigger_None) || \
                                 ((TRIGGER) == DAC_Trigger_T6_TRGO) || \
                                 ((TRIGGER) == DAC_Trigger_T8_TRGO) || \
                                 ((TRIGGER) == DAC_Trigger_T7_TRGO) || \
                                 ((TRIGGER) == DAC_Trigger_T5_TRGO) || \
                                 ((TRIGGER) == DAC_Trigger_T2_TRGO) || \
                                 ((TRIGGER) == DAC_Trigger_T4_TRGO) || \
                                 ((TRIGGER) == DAC_Trigger_Ext_IT9) || \
                                 ((TRIGGER) == DAC_Trigger_Software))

/**
  * @}
  */

/** @defgroup DAC_wave_generation   DAC 波形生成
  * @{
  */

#define DAC_WaveGeneration_None            ((uint32_t)0x00000000)
#define DAC_WaveGeneration_Noise           ((uint32_t)0x00000040)
#define DAC_WaveGeneration_Triangle        ((uint32_t)0x00000080)
#define IS_DAC_GENERATE_WAVE(WAVE) (((WAVE) == DAC_WaveGeneration_None) || \
                                    ((WAVE) == DAC_WaveGeneration_Noise) || \
                                    ((WAVE) == DAC_WaveGeneration_Triangle))
/**
  * @}
  */

/** @defgroup DAC_lfsrunmask_triangleamplitude   DAC LFSR 掩码与三角波幅值
  * @{
  */

#define DAC_LFSRUnmask_Bit0                ((uint32_t)0x00000000) /*!< 不屏蔽 DAC 通道 LFSR bit0 以生成噪声波 */
#define DAC_LFSRUnmask_Bits1_0             ((uint32_t)0x00000100) /*!< 不屏蔽 DAC 通道 LFSR bit[1:0] 以生成噪声波 */
#define DAC_LFSRUnmask_Bits2_0             ((uint32_t)0x00000200) /*!< 不屏蔽 DAC 通道 LFSR bit[2:0] 以生成噪声波 */
#define DAC_LFSRUnmask_Bits3_0             ((uint32_t)0x00000300) /*!< 不屏蔽 DAC 通道 LFSR bit[3:0] 以生成噪声波 */
#define DAC_LFSRUnmask_Bits4_0             ((uint32_t)0x00000400) /*!< 不屏蔽 DAC 通道 LFSR bit[4:0] 以生成噪声波 */
#define DAC_LFSRUnmask_Bits5_0             ((uint32_t)0x00000500) /*!< 不屏蔽 DAC 通道 LFSR bit[5:0] 以生成噪声波 */
#define DAC_LFSRUnmask_Bits6_0             ((uint32_t)0x00000600) /*!< 不屏蔽 DAC 通道 LFSR bit[6:0] 以生成噪声波 */
#define DAC_LFSRUnmask_Bits7_0             ((uint32_t)0x00000700) /*!< 不屏蔽 DAC 通道 LFSR bit[7:0] 以生成噪声波 */
#define DAC_LFSRUnmask_Bits8_0             ((uint32_t)0x00000800) /*!< 不屏蔽 DAC 通道 LFSR bit[8:0] 以生成噪声波 */
#define DAC_LFSRUnmask_Bits9_0             ((uint32_t)0x00000900) /*!< 不屏蔽 DAC 通道 LFSR bit[9:0] 以生成噪声波 */
#define DAC_LFSRUnmask_Bits10_0            ((uint32_t)0x00000A00) /*!< 不屏蔽 DAC 通道 LFSR bit[10:0] 以生成噪声波 */
#define DAC_LFSRUnmask_Bits11_0            ((uint32_t)0x00000B00) /*!< 不屏蔽 DAC 通道 LFSR bit[11:0] 以生成噪声波 */
#define DAC_TriangleAmplitude_1            ((uint32_t)0x00000000) /*!< 选择最大三角波幅值为 1 */
#define DAC_TriangleAmplitude_3            ((uint32_t)0x00000100) /*!< 选择最大三角波幅值为 3 */
#define DAC_TriangleAmplitude_7            ((uint32_t)0x00000200) /*!< 选择最大三角波幅值为 7 */
#define DAC_TriangleAmplitude_15           ((uint32_t)0x00000300) /*!< 选择最大三角波幅值为 15 */
#define DAC_TriangleAmplitude_31           ((uint32_t)0x00000400) /*!< 选择最大三角波幅值为 31 */
#define DAC_TriangleAmplitude_63           ((uint32_t)0x00000500) /*!< 选择最大三角波幅值为 63 */
#define DAC_TriangleAmplitude_127          ((uint32_t)0x00000600) /*!< 选择最大三角波幅值为 127 */
#define DAC_TriangleAmplitude_255          ((uint32_t)0x00000700) /*!< 选择最大三角波幅值为 255 */
#define DAC_TriangleAmplitude_511          ((uint32_t)0x00000800) /*!< 选择最大三角波幅值为 511 */
#define DAC_TriangleAmplitude_1023         ((uint32_t)0x00000900) /*!< 选择最大三角波幅值为 1023 */
#define DAC_TriangleAmplitude_2047         ((uint32_t)0x00000A00) /*!< 选择最大三角波幅值为 2047 */
#define DAC_TriangleAmplitude_4095         ((uint32_t)0x00000B00) /*!< 选择最大三角波幅值为 4095 */

#define IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(VALUE) (((VALUE) == DAC_LFSRUnmask_Bit0) || \
                                                      ((VALUE) == DAC_LFSRUnmask_Bits1_0) || \
                                                      ((VALUE) == DAC_LFSRUnmask_Bits2_0) || \
                                                      ((VALUE) == DAC_LFSRUnmask_Bits3_0) || \
                                                      ((VALUE) == DAC_LFSRUnmask_Bits4_0) || \
                                                      ((VALUE) == DAC_LFSRUnmask_Bits5_0) || \
                                                      ((VALUE) == DAC_LFSRUnmask_Bits6_0) || \
                                                      ((VALUE) == DAC_LFSRUnmask_Bits7_0) || \
                                                      ((VALUE) == DAC_LFSRUnmask_Bits8_0) || \
                                                      ((VALUE) == DAC_LFSRUnmask_Bits9_0) || \
                                                      ((VALUE) == DAC_LFSRUnmask_Bits10_0) || \
                                                      ((VALUE) == DAC_LFSRUnmask_Bits11_0) || \
                                                      ((VALUE) == DAC_TriangleAmplitude_1) || \
                                                      ((VALUE) == DAC_TriangleAmplitude_3) || \
                                                      ((VALUE) == DAC_TriangleAmplitude_7) || \
                                                      ((VALUE) == DAC_TriangleAmplitude_15) || \
                                                      ((VALUE) == DAC_TriangleAmplitude_31) || \
                                                      ((VALUE) == DAC_TriangleAmplitude_63) || \
                                                      ((VALUE) == DAC_TriangleAmplitude_127) || \
                                                      ((VALUE) == DAC_TriangleAmplitude_255) || \
                                                      ((VALUE) == DAC_TriangleAmplitude_511) || \
                                                      ((VALUE) == DAC_TriangleAmplitude_1023) || \
                                                      ((VALUE) == DAC_TriangleAmplitude_2047) || \
                                                      ((VALUE) == DAC_TriangleAmplitude_4095))
/**
  * @}
  */

/** @defgroup DAC_output_buffer   DAC 输出缓冲
  * @{
  */

#define DAC_OutputBuffer_Enable            ((uint32_t)0x00000000)
#define DAC_OutputBuffer_Disable           ((uint32_t)0x00000002)
#define IS_DAC_OUTPUT_BUFFER_STATE(STATE) (((STATE) == DAC_OutputBuffer_Enable) || \
                                           ((STATE) == DAC_OutputBuffer_Disable))
/**
  * @}
  */

/** @defgroup DAC_Channel_selection   DAC 通道选择
  * @{
  */

#define DAC_Channel_1                      ((uint32_t)0x00000000)
#define DAC_Channel_2                      ((uint32_t)0x00000010)
#define IS_DAC_CHANNEL(CHANNEL) (((CHANNEL) == DAC_Channel_1) || \
                                 ((CHANNEL) == DAC_Channel_2))
/**
  * @}
  */

/** @defgroup DAC_data_alignment   DAC 数据对齐
  * @{
  */

#define DAC_Align_12b_R                    ((uint32_t)0x00000000)
#define DAC_Align_12b_L                    ((uint32_t)0x00000004)
#define DAC_Align_8b_R                     ((uint32_t)0x00000008)
#define IS_DAC_ALIGN(ALIGN) (((ALIGN) == DAC_Align_12b_R) || \
                             ((ALIGN) == DAC_Align_12b_L) || \
                             ((ALIGN) == DAC_Align_8b_R))
/**
  * @}
  */

/** @defgroup DAC_wave_generation   DAC 波形生成
  * @{
  */

#define DAC_Wave_Noise                     ((uint32_t)0x00000040)
#define DAC_Wave_Triangle                  ((uint32_t)0x00000080)
#define IS_DAC_WAVE(WAVE) (((WAVE) == DAC_Wave_Noise) || \
                           ((WAVE) == DAC_Wave_Triangle))
/**
  * @}
  */

/** @defgroup DAC_data   DAC 数据
  * @{
  */

#define IS_DAC_DATA(DATA) ((DATA) <= 0xFFF0) 
/**
  * @}
  */
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL)  || defined (STM32F10X_HD_VL)
/** @defgroup DAC_interrupts_definition   DAC 中断定义
  * @{
  */ 
  
#define DAC_IT_DMAUDR                      ((uint32_t)0x00002000)  
#define IS_DAC_IT(IT) (((IT) == DAC_IT_DMAUDR)) 

/**
  * @}
  */ 

/** @defgroup DAC_flags_definition   DAC 标志定义
  * @{
  */ 
  
#define DAC_FLAG_DMAUDR                    ((uint32_t)0x00002000)  
#define IS_DAC_FLAG(FLAG) (((FLAG) == DAC_FLAG_DMAUDR))  

/**
  * @}
  */
#endif

/**
  * @}
  */

/** @defgroup DAC_Exported_Macros   DAC 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup DAC_Exported_Functions   DAC 导出函数
  * @{
  */

void DAC_DeInit(void);
void DAC_Init(uint32_t DAC_Channel, DAC_InitTypeDef* DAC_InitStruct);
void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct);
void DAC_Cmd(uint32_t DAC_Channel, FunctionalState NewState);
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
void DAC_ITConfig(uint32_t DAC_Channel, uint32_t DAC_IT, FunctionalState NewState);
#endif
void DAC_DMACmd(uint32_t DAC_Channel, FunctionalState NewState);
void DAC_SoftwareTriggerCmd(uint32_t DAC_Channel, FunctionalState NewState);
void DAC_DualSoftwareTriggerCmd(FunctionalState NewState);
void DAC_WaveGenerationCmd(uint32_t DAC_Channel, uint32_t DAC_Wave, FunctionalState NewState);
void DAC_SetChannel1Data(uint32_t DAC_Align, uint16_t Data);
void DAC_SetChannel2Data(uint32_t DAC_Align, uint16_t Data);
void DAC_SetDualChannelData(uint32_t DAC_Align, uint16_t Data2, uint16_t Data1);
uint16_t DAC_GetDataOutputValue(uint32_t DAC_Channel);
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL) 
FlagStatus DAC_GetFlagStatus(uint32_t DAC_Channel, uint32_t DAC_FLAG);
void DAC_ClearFlag(uint32_t DAC_Channel, uint32_t DAC_FLAG);
ITStatus DAC_GetITStatus(uint32_t DAC_Channel, uint32_t DAC_IT);
void DAC_ClearITPendingBit(uint32_t DAC_Channel, uint32_t DAC_IT);
#endif

#ifdef __cplusplus
}
#endif

#endif /*__STM32F10x_DAC_H */
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

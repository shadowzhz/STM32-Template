/**
  ******************************************************************************
  * @file    stm32f10x_dma.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 DMA 固件库的所有函数原型。
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
#ifndef __STM32F10x_DMA_H
#define __STM32F10x_DMA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup DMA   DMA 驱动模块
  * @{
  */

/** @defgroup DMA_Exported_Types   DMA 导出类型
  * @{
  */

/**
  * @brief  DMA 初始化结构体定义
  */

typedef struct
{
  uint32_t DMA_PeripheralBaseAddr; /*!< 指定 DMA 通道关联的外设数据寄存器基地址（如 (uint32_t)&USART1->DR 或 (uint32_t)&ADC1->DR）。 */

  uint32_t DMA_MemoryBaseAddr;     /*!< 指定 DMA 通道关联的内部 SRAM 存储器缓冲区首地址（如 (uint32_t)rx_buffer）。
                                        @warning 存储区数组的生命周期必须在传输期间持续有效！严禁传入局部临时变量指针，
                                                 必须定义为全局变量或 static 变量，否则函数退出后栈内存被覆盖将导致数据严重损毁！ */

  uint32_t DMA_DIR;                /*!< 指定数据传输方向（外设作为源还是目标）：
                                        - DMA_DIR_PeripheralDST: 内存 -> 外设（如串口发送 TX）
                                        - DMA_DIR_PeripheralSRC: 外设 -> 内存（如串口接收 RX、ADC 采集） */

  uint32_t DMA_BufferSize;         /*!< 指定所选通道单次传输的数据单元数目（0 ~ 65535）。
                                        数据单元宽度由 DMA_PeripheralDataSize 与 DMA_MemoryDataSize 共同决定。 */

  uint32_t DMA_PeripheralInc;      /*!< 指定外设地址寄存器在每次传输后是否自增：
                                        - DMA_PeripheralInc_Disable: 外设地址固定（最常用，固定读写单个 DR 寄存器）
                                        - DMA_PeripheralInc_Enable : 外设地址自增（仅在内存到内存拷贝或连续 FIFO 时使用） */

  uint32_t DMA_MemoryInc;          /*!< 指定内部存储器地址在每次传输后是否自增：
                                        - DMA_MemoryInc_Enable : 内存地址递增（最常用，顺序填充数组缓冲区）
                                        - DMA_MemoryInc_Disable: 内存地址固定（固定写入单一全局变量） */

  uint32_t DMA_PeripheralDataSize; /*!< 指定外设数据传输宽度：
                                        - DMA_PeripheralDataSize_Byte    : 8 位（字节，串口、I2C、8 位 SPI 寄存器）
                                        - DMA_PeripheralDataSize_HalfWord: 16 位（半字，ADC 转换结果、16 位定时器）
                                        - DMA_PeripheralDataSize_Word    : 32 位（字，32 位定时器或特定总线） */

  uint32_t DMA_MemoryDataSize;     /*!< 指定内存数据传输宽度：
                                        - DMA_MemoryDataSize_Byte    : 8 位（uint8_t 数组）
                                        - DMA_MemoryDataSize_HalfWord: 16 位（uint16_t 数组，如 ADC 采样缓存）
                                        - DMA_MemoryDataSize_Word    : 32 位（uint32_t 数组） */

  uint32_t DMA_Mode;               /*!< 指定 DMA 通道的工作模式：
                                        - DMA_Mode_Normal  : 正常单次模式（计数器递减到 0 后自动停止）
                                        - DMA_Mode_Circular: 循环缓冲模式（计数器到 0 后自动重装初值循环传输，常用于 ADC 多通道连续采集）
                                        @note 在存储器到存储器模式下不能使用循环缓冲模式。 */

  uint32_t DMA_Priority;           /*!< 指定 DMA 通道的软件仲裁优先级：
                                        - DMA_Priority_VeryHigh（最高）
                                        - DMA_Priority_High
                                        - DMA_Priority_Medium
                                        - DMA_Priority_Low（最低）
                                        @note 软件优先级相同时，硬件通道号越小优先级越高（通道 1 高于通道 2）。 */

  uint32_t DMA_M2M;                /*!< 指定是否启用纯存储器到存储器（SRAM -> SRAM）高速拷贝传输：
                                        - DMA_M2M_Disable: 禁用 M2M（由外设硬件请求信号触发传输，最常用）
                                        - DMA_M2M_Enable : 启用 M2M（使能后软件立即全速触发传输，仅 DMA1 部分通道支持） */
}DMA_InitTypeDef;

/**
  * @}
  */

/** @defgroup DMA_Exported_Constants   DMA 导出常量
  * @{
  */

#define IS_DMA_ALL_PERIPH(PERIPH) (((PERIPH) == DMA1_Channel1) || \
                                   ((PERIPH) == DMA1_Channel2) || \
                                   ((PERIPH) == DMA1_Channel3) || \
                                   ((PERIPH) == DMA1_Channel4) || \
                                   ((PERIPH) == DMA1_Channel5) || \
                                   ((PERIPH) == DMA1_Channel6) || \
                                   ((PERIPH) == DMA1_Channel7) || \
                                   ((PERIPH) == DMA2_Channel1) || \
                                   ((PERIPH) == DMA2_Channel2) || \
                                   ((PERIPH) == DMA2_Channel3) || \
                                   ((PERIPH) == DMA2_Channel4) || \
                                   ((PERIPH) == DMA2_Channel5))

/** @defgroup DMA_data_transfer_direction   DMA 数据传输方向
  * @{
  */

#define DMA_DIR_PeripheralDST              ((uint32_t)0x00000010)
#define DMA_DIR_PeripheralSRC              ((uint32_t)0x00000000)
#define IS_DMA_DIR(DIR) (((DIR) == DMA_DIR_PeripheralDST) || \
                         ((DIR) == DMA_DIR_PeripheralSRC))
/**
  * @}
  */

/** @defgroup DMA_peripheral_incremented_mode   DMA 外设递增模式
  * @{
  */

#define DMA_PeripheralInc_Enable           ((uint32_t)0x00000040)
#define DMA_PeripheralInc_Disable          ((uint32_t)0x00000000)
#define IS_DMA_PERIPHERAL_INC_STATE(STATE) (((STATE) == DMA_PeripheralInc_Enable) || \
                                            ((STATE) == DMA_PeripheralInc_Disable))
/**
  * @}
  */

/** @defgroup DMA_memory_incremented_mode   DMA 存储器递增模式
  * @{
  */

#define DMA_MemoryInc_Enable               ((uint32_t)0x00000080)
#define DMA_MemoryInc_Disable              ((uint32_t)0x00000000)
#define IS_DMA_MEMORY_INC_STATE(STATE) (((STATE) == DMA_MemoryInc_Enable) || \
                                        ((STATE) == DMA_MemoryInc_Disable))
/**
  * @}
  */

/** @defgroup DMA_peripheral_data_size   DMA 外设数据大小
  * @{
  */

#define DMA_PeripheralDataSize_Byte        ((uint32_t)0x00000000)
#define DMA_PeripheralDataSize_HalfWord    ((uint32_t)0x00000100)
#define DMA_PeripheralDataSize_Word        ((uint32_t)0x00000200)
#define IS_DMA_PERIPHERAL_DATA_SIZE(SIZE) (((SIZE) == DMA_PeripheralDataSize_Byte) || \
                                           ((SIZE) == DMA_PeripheralDataSize_HalfWord) || \
                                           ((SIZE) == DMA_PeripheralDataSize_Word))
/**
  * @}
  */

/** @defgroup DMA_memory_data_size   DMA 存储器数据大小
  * @{
  */

#define DMA_MemoryDataSize_Byte            ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord        ((uint32_t)0x00000400)
#define DMA_MemoryDataSize_Word            ((uint32_t)0x00000800)
#define IS_DMA_MEMORY_DATA_SIZE(SIZE) (((SIZE) == DMA_MemoryDataSize_Byte) || \
                                       ((SIZE) == DMA_MemoryDataSize_HalfWord) || \
                                       ((SIZE) == DMA_MemoryDataSize_Word))
/**
  * @}
  */

/** @defgroup DMA_circular_normal_mode   DMA 循环/正常模式
  * @{
  */

#define DMA_Mode_Circular                  ((uint32_t)0x00000020)
#define DMA_Mode_Normal                    ((uint32_t)0x00000000)
#define IS_DMA_MODE(MODE) (((MODE) == DMA_Mode_Circular) || ((MODE) == DMA_Mode_Normal))
/**
  * @}
  */

/** @defgroup DMA_priority_level   DMA 优先级
  * @{
  */

#define DMA_Priority_VeryHigh              ((uint32_t)0x00003000)
#define DMA_Priority_High                  ((uint32_t)0x00002000)
#define DMA_Priority_Medium                ((uint32_t)0x00001000)
#define DMA_Priority_Low                   ((uint32_t)0x00000000)
#define IS_DMA_PRIORITY(PRIORITY) (((PRIORITY) == DMA_Priority_VeryHigh) || \
                                   ((PRIORITY) == DMA_Priority_High) || \
                                   ((PRIORITY) == DMA_Priority_Medium) || \
                                   ((PRIORITY) == DMA_Priority_Low))
/**
  * @}
  */

/** @defgroup DMA_memory_to_memory   DMA 存储器到存储器
  * @{
  */

#define DMA_M2M_Enable                     ((uint32_t)0x00004000)
#define DMA_M2M_Disable                    ((uint32_t)0x00000000)
#define IS_DMA_M2M_STATE(STATE) (((STATE) == DMA_M2M_Enable) || ((STATE) == DMA_M2M_Disable))

/**
  * @}
  */

/** @defgroup DMA_interrupts_definition   DMA 中断定义
  * @{
  */

#define DMA_IT_TC                          ((uint32_t)0x00000002)
#define DMA_IT_HT                          ((uint32_t)0x00000004)
#define DMA_IT_TE                          ((uint32_t)0x00000008)
#define IS_DMA_CONFIG_IT(IT) ((((IT) & 0xFFFFFFF1) == 0x00) && ((IT) != 0x00))

#define DMA1_IT_GL1                        ((uint32_t)0x00000001)
#define DMA1_IT_TC1                        ((uint32_t)0x00000002)
#define DMA1_IT_HT1                        ((uint32_t)0x00000004)
#define DMA1_IT_TE1                        ((uint32_t)0x00000008)
#define DMA1_IT_GL2                        ((uint32_t)0x00000010)
#define DMA1_IT_TC2                        ((uint32_t)0x00000020)
#define DMA1_IT_HT2                        ((uint32_t)0x00000040)
#define DMA1_IT_TE2                        ((uint32_t)0x00000080)
#define DMA1_IT_GL3                        ((uint32_t)0x00000100)
#define DMA1_IT_TC3                        ((uint32_t)0x00000200)
#define DMA1_IT_HT3                        ((uint32_t)0x00000400)
#define DMA1_IT_TE3                        ((uint32_t)0x00000800)
#define DMA1_IT_GL4                        ((uint32_t)0x00001000)
#define DMA1_IT_TC4                        ((uint32_t)0x00002000)
#define DMA1_IT_HT4                        ((uint32_t)0x00004000)
#define DMA1_IT_TE4                        ((uint32_t)0x00008000)
#define DMA1_IT_GL5                        ((uint32_t)0x00010000)
#define DMA1_IT_TC5                        ((uint32_t)0x00020000)
#define DMA1_IT_HT5                        ((uint32_t)0x00040000)
#define DMA1_IT_TE5                        ((uint32_t)0x00080000)
#define DMA1_IT_GL6                        ((uint32_t)0x00100000)
#define DMA1_IT_TC6                        ((uint32_t)0x00200000)
#define DMA1_IT_HT6                        ((uint32_t)0x00400000)
#define DMA1_IT_TE6                        ((uint32_t)0x00800000)
#define DMA1_IT_GL7                        ((uint32_t)0x01000000)
#define DMA1_IT_TC7                        ((uint32_t)0x02000000)
#define DMA1_IT_HT7                        ((uint32_t)0x04000000)
#define DMA1_IT_TE7                        ((uint32_t)0x08000000)

#define DMA2_IT_GL1                        ((uint32_t)0x10000001)
#define DMA2_IT_TC1                        ((uint32_t)0x10000002)
#define DMA2_IT_HT1                        ((uint32_t)0x10000004)
#define DMA2_IT_TE1                        ((uint32_t)0x10000008)
#define DMA2_IT_GL2                        ((uint32_t)0x10000010)
#define DMA2_IT_TC2                        ((uint32_t)0x10000020)
#define DMA2_IT_HT2                        ((uint32_t)0x10000040)
#define DMA2_IT_TE2                        ((uint32_t)0x10000080)
#define DMA2_IT_GL3                        ((uint32_t)0x10000100)
#define DMA2_IT_TC3                        ((uint32_t)0x10000200)
#define DMA2_IT_HT3                        ((uint32_t)0x10000400)
#define DMA2_IT_TE3                        ((uint32_t)0x10000800)
#define DMA2_IT_GL4                        ((uint32_t)0x10001000)
#define DMA2_IT_TC4                        ((uint32_t)0x10002000)
#define DMA2_IT_HT4                        ((uint32_t)0x10004000)
#define DMA2_IT_TE4                        ((uint32_t)0x10008000)
#define DMA2_IT_GL5                        ((uint32_t)0x10010000)
#define DMA2_IT_TC5                        ((uint32_t)0x10020000)
#define DMA2_IT_HT5                        ((uint32_t)0x10040000)
#define DMA2_IT_TE5                        ((uint32_t)0x10080000)

#define IS_DMA_CLEAR_IT(IT) (((((IT) & 0xF0000000) == 0x00) || (((IT) & 0xEFF00000) == 0x00)) && ((IT) != 0x00))

#define IS_DMA_GET_IT(IT) (((IT) == DMA1_IT_GL1) || ((IT) == DMA1_IT_TC1) || \
                           ((IT) == DMA1_IT_HT1) || ((IT) == DMA1_IT_TE1) || \
                           ((IT) == DMA1_IT_GL2) || ((IT) == DMA1_IT_TC2) || \
                           ((IT) == DMA1_IT_HT2) || ((IT) == DMA1_IT_TE2) || \
                           ((IT) == DMA1_IT_GL3) || ((IT) == DMA1_IT_TC3) || \
                           ((IT) == DMA1_IT_HT3) || ((IT) == DMA1_IT_TE3) || \
                           ((IT) == DMA1_IT_GL4) || ((IT) == DMA1_IT_TC4) || \
                           ((IT) == DMA1_IT_HT4) || ((IT) == DMA1_IT_TE4) || \
                           ((IT) == DMA1_IT_GL5) || ((IT) == DMA1_IT_TC5) || \
                           ((IT) == DMA1_IT_HT5) || ((IT) == DMA1_IT_TE5) || \
                           ((IT) == DMA1_IT_GL6) || ((IT) == DMA1_IT_TC6) || \
                           ((IT) == DMA1_IT_HT6) || ((IT) == DMA1_IT_TE6) || \
                           ((IT) == DMA1_IT_GL7) || ((IT) == DMA1_IT_TC7) || \
                           ((IT) == DMA1_IT_HT7) || ((IT) == DMA1_IT_TE7) || \
                           ((IT) == DMA2_IT_GL1) || ((IT) == DMA2_IT_TC1) || \
                           ((IT) == DMA2_IT_HT1) || ((IT) == DMA2_IT_TE1) || \
                           ((IT) == DMA2_IT_GL2) || ((IT) == DMA2_IT_TC2) || \
                           ((IT) == DMA2_IT_HT2) || ((IT) == DMA2_IT_TE2) || \
                           ((IT) == DMA2_IT_GL3) || ((IT) == DMA2_IT_TC3) || \
                           ((IT) == DMA2_IT_HT3) || ((IT) == DMA2_IT_TE3) || \
                           ((IT) == DMA2_IT_GL4) || ((IT) == DMA2_IT_TC4) || \
                           ((IT) == DMA2_IT_HT4) || ((IT) == DMA2_IT_TE4) || \
                           ((IT) == DMA2_IT_GL5) || ((IT) == DMA2_IT_TC5) || \
                           ((IT) == DMA2_IT_HT5) || ((IT) == DMA2_IT_TE5))

/**
  * @}
  */

/** @defgroup DMA_flags_definition   DMA 标志定义
  * @{
  */
#define DMA1_FLAG_GL1                      ((uint32_t)0x00000001)
#define DMA1_FLAG_TC1                      ((uint32_t)0x00000002)
#define DMA1_FLAG_HT1                      ((uint32_t)0x00000004)
#define DMA1_FLAG_TE1                      ((uint32_t)0x00000008)
#define DMA1_FLAG_GL2                      ((uint32_t)0x00000010)
#define DMA1_FLAG_TC2                      ((uint32_t)0x00000020)
#define DMA1_FLAG_HT2                      ((uint32_t)0x00000040)
#define DMA1_FLAG_TE2                      ((uint32_t)0x00000080)
#define DMA1_FLAG_GL3                      ((uint32_t)0x00000100)
#define DMA1_FLAG_TC3                      ((uint32_t)0x00000200)
#define DMA1_FLAG_HT3                      ((uint32_t)0x00000400)
#define DMA1_FLAG_TE3                      ((uint32_t)0x00000800)
#define DMA1_FLAG_GL4                      ((uint32_t)0x00001000)
#define DMA1_FLAG_TC4                      ((uint32_t)0x00002000)
#define DMA1_FLAG_HT4                      ((uint32_t)0x00004000)
#define DMA1_FLAG_TE4                      ((uint32_t)0x00008000)
#define DMA1_FLAG_GL5                      ((uint32_t)0x00010000)
#define DMA1_FLAG_TC5                      ((uint32_t)0x00020000)
#define DMA1_FLAG_HT5                      ((uint32_t)0x00040000)
#define DMA1_FLAG_TE5                      ((uint32_t)0x00080000)
#define DMA1_FLAG_GL6                      ((uint32_t)0x00100000)
#define DMA1_FLAG_TC6                      ((uint32_t)0x00200000)
#define DMA1_FLAG_HT6                      ((uint32_t)0x00400000)
#define DMA1_FLAG_TE6                      ((uint32_t)0x00800000)
#define DMA1_FLAG_GL7                      ((uint32_t)0x01000000)
#define DMA1_FLAG_TC7                      ((uint32_t)0x02000000)
#define DMA1_FLAG_HT7                      ((uint32_t)0x04000000)
#define DMA1_FLAG_TE7                      ((uint32_t)0x08000000)

#define DMA2_FLAG_GL1                      ((uint32_t)0x10000001)
#define DMA2_FLAG_TC1                      ((uint32_t)0x10000002)
#define DMA2_FLAG_HT1                      ((uint32_t)0x10000004)
#define DMA2_FLAG_TE1                      ((uint32_t)0x10000008)
#define DMA2_FLAG_GL2                      ((uint32_t)0x10000010)
#define DMA2_FLAG_TC2                      ((uint32_t)0x10000020)
#define DMA2_FLAG_HT2                      ((uint32_t)0x10000040)
#define DMA2_FLAG_TE2                      ((uint32_t)0x10000080)
#define DMA2_FLAG_GL3                      ((uint32_t)0x10000100)
#define DMA2_FLAG_TC3                      ((uint32_t)0x10000200)
#define DMA2_FLAG_HT3                      ((uint32_t)0x10000400)
#define DMA2_FLAG_TE3                      ((uint32_t)0x10000800)
#define DMA2_FLAG_GL4                      ((uint32_t)0x10001000)
#define DMA2_FLAG_TC4                      ((uint32_t)0x10002000)
#define DMA2_FLAG_HT4                      ((uint32_t)0x10004000)
#define DMA2_FLAG_TE4                      ((uint32_t)0x10008000)
#define DMA2_FLAG_GL5                      ((uint32_t)0x10010000)
#define DMA2_FLAG_TC5                      ((uint32_t)0x10020000)
#define DMA2_FLAG_HT5                      ((uint32_t)0x10040000)
#define DMA2_FLAG_TE5                      ((uint32_t)0x10080000)

#define IS_DMA_CLEAR_FLAG(FLAG) (((((FLAG) & 0xF0000000) == 0x00) || (((FLAG) & 0xEFF00000) == 0x00)) && ((FLAG) != 0x00))

#define IS_DMA_GET_FLAG(FLAG) (((FLAG) == DMA1_FLAG_GL1) || ((FLAG) == DMA1_FLAG_TC1) || \
                               ((FLAG) == DMA1_FLAG_HT1) || ((FLAG) == DMA1_FLAG_TE1) || \
                               ((FLAG) == DMA1_FLAG_GL2) || ((FLAG) == DMA1_FLAG_TC2) || \
                               ((FLAG) == DMA1_FLAG_HT2) || ((FLAG) == DMA1_FLAG_TE2) || \
                               ((FLAG) == DMA1_FLAG_GL3) || ((FLAG) == DMA1_FLAG_TC3) || \
                               ((FLAG) == DMA1_FLAG_HT3) || ((FLAG) == DMA1_FLAG_TE3) || \
                               ((FLAG) == DMA1_FLAG_GL4) || ((FLAG) == DMA1_FLAG_TC4) || \
                               ((FLAG) == DMA1_FLAG_HT4) || ((FLAG) == DMA1_FLAG_TE4) || \
                               ((FLAG) == DMA1_FLAG_GL5) || ((FLAG) == DMA1_FLAG_TC5) || \
                               ((FLAG) == DMA1_FLAG_HT5) || ((FLAG) == DMA1_FLAG_TE5) || \
                               ((FLAG) == DMA1_FLAG_GL6) || ((FLAG) == DMA1_FLAG_TC6) || \
                               ((FLAG) == DMA1_FLAG_HT6) || ((FLAG) == DMA1_FLAG_TE6) || \
                               ((FLAG) == DMA1_FLAG_GL7) || ((FLAG) == DMA1_FLAG_TC7) || \
                               ((FLAG) == DMA1_FLAG_HT7) || ((FLAG) == DMA1_FLAG_TE7) || \
                               ((FLAG) == DMA2_FLAG_GL1) || ((FLAG) == DMA2_FLAG_TC1) || \
                               ((FLAG) == DMA2_FLAG_HT1) || ((FLAG) == DMA2_FLAG_TE1) || \
                               ((FLAG) == DMA2_FLAG_GL2) || ((FLAG) == DMA2_FLAG_TC2) || \
                               ((FLAG) == DMA2_FLAG_HT2) || ((FLAG) == DMA2_FLAG_TE2) || \
                               ((FLAG) == DMA2_FLAG_GL3) || ((FLAG) == DMA2_FLAG_TC3) || \
                               ((FLAG) == DMA2_FLAG_HT3) || ((FLAG) == DMA2_FLAG_TE3) || \
                               ((FLAG) == DMA2_FLAG_GL4) || ((FLAG) == DMA2_FLAG_TC4) || \
                               ((FLAG) == DMA2_FLAG_HT4) || ((FLAG) == DMA2_FLAG_TE4) || \
                               ((FLAG) == DMA2_FLAG_GL5) || ((FLAG) == DMA2_FLAG_TC5) || \
                               ((FLAG) == DMA2_FLAG_HT5) || ((FLAG) == DMA2_FLAG_TE5))
/**
  * @}
  */

/** @defgroup DMA_Buffer_Size   DMA 缓冲区大小
  * @{
  */

#define IS_DMA_BUFFER_SIZE(SIZE) (((SIZE) >= 0x1) && ((SIZE) < 0x10000))

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup DMA_Exported_Macros   DMA 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Exported_Functions   DMA 导出函数
  * @{
  */

void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx);

/**
  * @brief  根据 DMA_InitStruct 中的指定参数初始化指定的 DMA 通道
  * @param  DMAy_Channelx: 目标 DMA 通道（例如 DMA1_Channel1、DMA1_Channel4 等）
  * @param  DMA_InitStruct: 指向包含配置参数的 DMA_InitTypeDef 结构体指针
  * @warning 【外设时钟先决条件】
  *          在调用本函数之前，【必须先开启 DMA 的 AHB 外设时钟】！
  *          例如：RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  *          若时钟未使能，访问 DMA 寄存器将静默失败，DMA 完全无法启动！
  * @warning 【硬件通道映射约束】
  *          STM32F1 的每个 DMA 通道绑定的硬件外设是固定的（查阅《参考手册》DMA 请求映射表）：
  *          - ADC1 规则通道固定使用 DMA1_Channel1；
  *          - SPI1_RX/TX 固定使用 DMA1_Channel2/3；
  *          - USART1_TX/RX 固定使用 DMA1_Channel4/5；
  *          切勿配置错误的通道号，否则外设产生数据请求时 DMA 将无法响应。
  * @retval None
  */
void DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct);

void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);

/**
  * @brief  使能或失能指定的 DMA 通道
  * @param  DMAy_Channelx: 目标 DMA 通道（例如 DMA1_Channel4）
  * @param  NewState: 新状态（ENABLE 或 DISABLE）
  * @note   只有使能通道后，DMA 才能响应外设或内存的传输请求。
  * @retval None
  */
void DMA_Cmd(DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState);

void DMA_ITConfig(DMA_Channel_TypeDef* DMAy_Channelx, uint32_t DMA_IT, FunctionalState NewState);

/**
  * @brief  设置指定 DMA 通道的当前剩余传输数据单元数（重载 CNDTR 寄存器）
  * @param  DMAy_Channelx: 目标 DMA 通道（例如 DMA1_Channel4）
  * @param  DataNumber: 待传输的数据单元数（0 ~ 65535）
  * @warning 【运行中动态修改重载长度死穴 / 避坑警告】
  *          STM32 硬件规定：【仅在 DMA 通道处于禁用（DISABLE）状态时，CNDTR 寄存器才允许被写入】！
  *          若在 DMA_Cmd(..., ENABLE) 运行期间直接调用本函数，硬件将【静默丢弃写入】，长度绝不会更新！
  *          标准动态重装长度的标准三步操作时序：
  *          @code
  *          DMA_Cmd(DMA1_Channel4, DISABLE);                      // 1. 先关闭通道
  *          DMA_SetCurrDataCounter(DMA1_Channel4, new_length);    // 2. 写入新传输长度
  *          DMA_Cmd(DMA1_Channel4, ENABLE);                       // 3. 重新使能通道开始传输
  *          @endcode
  * @retval None
  */
void DMA_SetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx, uint16_t DataNumber);

/**
  * @brief  获取指定 DMA 通道当前剩余未传输的数据单元数（读取 CNDTR 寄存器）
  * @param  DMAy_Channelx: 目标 DMA 通道（例如 DMA1_Channel5）
  * @note   【串口不定长接收计算实际接收字节数】
  *         若总缓冲大小为 BUF_SIZE，在总线产生空闲中断（IDLE）时：
  *         实际接收字节数 = BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5);
  * @retval 当前通道剩余未完成的数据单元数
  */
uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx);

FlagStatus DMA_GetFlagStatus(uint32_t DMAy_FLAG);
void DMA_ClearFlag(uint32_t DMAy_FLAG);
ITStatus DMA_GetITStatus(uint32_t DMAy_IT);
void DMA_ClearITPendingBit(uint32_t DMAy_IT);

#ifdef __cplusplus
}
#endif

#endif /*__STM32F10x_DMA_H */
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

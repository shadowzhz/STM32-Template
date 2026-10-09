/**
  ******************************************************************************
  * @file    stm32f10x_dma.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供所有 DMA 固件函数。
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

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup DMA
  * @brief DMA 驱动模块
  * @{
  */ 

/** @defgroup DMA_Private_TypesDefinitions   DMA 私有类型定义
  * @{
  */ 
/**
  * @}
  */

/** @defgroup DMA_Private_Defines   DMA 私有宏定义
  * @{
  */


/* DMA1 通道 x 中断挂起位掩码 */
#define DMA1_Channel1_IT_Mask    ((uint32_t)(DMA_ISR_GIF1 | DMA_ISR_TCIF1 | DMA_ISR_HTIF1 | DMA_ISR_TEIF1))
#define DMA1_Channel2_IT_Mask    ((uint32_t)(DMA_ISR_GIF2 | DMA_ISR_TCIF2 | DMA_ISR_HTIF2 | DMA_ISR_TEIF2))
#define DMA1_Channel3_IT_Mask    ((uint32_t)(DMA_ISR_GIF3 | DMA_ISR_TCIF3 | DMA_ISR_HTIF3 | DMA_ISR_TEIF3))
#define DMA1_Channel4_IT_Mask    ((uint32_t)(DMA_ISR_GIF4 | DMA_ISR_TCIF4 | DMA_ISR_HTIF4 | DMA_ISR_TEIF4))
#define DMA1_Channel5_IT_Mask    ((uint32_t)(DMA_ISR_GIF5 | DMA_ISR_TCIF5 | DMA_ISR_HTIF5 | DMA_ISR_TEIF5))
#define DMA1_Channel6_IT_Mask    ((uint32_t)(DMA_ISR_GIF6 | DMA_ISR_TCIF6 | DMA_ISR_HTIF6 | DMA_ISR_TEIF6))
#define DMA1_Channel7_IT_Mask    ((uint32_t)(DMA_ISR_GIF7 | DMA_ISR_TCIF7 | DMA_ISR_HTIF7 | DMA_ISR_TEIF7))

/* DMA2 通道 x 中断挂起位掩码 */
#define DMA2_Channel1_IT_Mask    ((uint32_t)(DMA_ISR_GIF1 | DMA_ISR_TCIF1 | DMA_ISR_HTIF1 | DMA_ISR_TEIF1))
#define DMA2_Channel2_IT_Mask    ((uint32_t)(DMA_ISR_GIF2 | DMA_ISR_TCIF2 | DMA_ISR_HTIF2 | DMA_ISR_TEIF2))
#define DMA2_Channel3_IT_Mask    ((uint32_t)(DMA_ISR_GIF3 | DMA_ISR_TCIF3 | DMA_ISR_HTIF3 | DMA_ISR_TEIF3))
#define DMA2_Channel4_IT_Mask    ((uint32_t)(DMA_ISR_GIF4 | DMA_ISR_TCIF4 | DMA_ISR_HTIF4 | DMA_ISR_TEIF4))
#define DMA2_Channel5_IT_Mask    ((uint32_t)(DMA_ISR_GIF5 | DMA_ISR_TCIF5 | DMA_ISR_HTIF5 | DMA_ISR_TEIF5))

/* DMA2 标志掩码 */
#define FLAG_Mask                ((uint32_t)0x10000000)

/* DMA 寄存器掩码 */
#define CCR_CLEAR_Mask           ((uint32_t)0xFFFF800F)

/**
  * @}
  */

/** @defgroup DMA_Private_Macros   DMA 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Private_Variables   DMA 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Private_FunctionPrototypes   DMA 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Private_Functions   DMA 私有函数
  * @{
  */

/**
  * @brief  将 DMAy 通道 x 的寄存器反初始化为默认复位值。
  * @param  DMAy_Channelx: 其中 y 可取 1 或 2 以选择 DMA，
  *   x 对于 DMA1 可取 1 到 7，对于 DMA2 可取 1 到 5，以选择 DMA 通道。
  * @retval 无
  */
void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx)
{
  /* 检查参数 */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  
  /* 关闭所选 DMAy 通道 x */
  DMAy_Channelx->CCR &= (uint16_t)(~DMA_CCR1_EN);
  
  /* 复位 DMAy 通道 x 控制寄存器 */
  DMAy_Channelx->CCR  = 0;
  
  /* 复位 DMAy 通道 x 剩余字节寄存器 */
  DMAy_Channelx->CNDTR = 0;
  
  /* 复位 DMAy 通道 x 外设地址寄存器 */
  DMAy_Channelx->CPAR  = 0;
  
  /* 复位 DMAy 通道 x 存储器地址寄存器 */
  DMAy_Channelx->CMAR = 0;
  
  if (DMAy_Channelx == DMA1_Channel1)
  {
    /* 复位 DMA1 通道 1 的中断挂起位 */
    DMA1->IFCR |= DMA1_Channel1_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel2)
  {
    /* 复位 DMA1 通道 2 的中断挂起位 */
    DMA1->IFCR |= DMA1_Channel2_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel3)
  {
    /* 复位 DMA1 通道 3 的中断挂起位 */
    DMA1->IFCR |= DMA1_Channel3_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel4)
  {
    /* 复位 DMA1 通道 4 的中断挂起位 */
    DMA1->IFCR |= DMA1_Channel4_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel5)
  {
    /* 复位 DMA1 通道 5 的中断挂起位 */
    DMA1->IFCR |= DMA1_Channel5_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel6)
  {
    /* 复位 DMA1 通道 6 的中断挂起位 */
    DMA1->IFCR |= DMA1_Channel6_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel7)
  {
    /* 复位 DMA1 通道 7 的中断挂起位 */
    DMA1->IFCR |= DMA1_Channel7_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel1)
  {
    /* 复位 DMA2 通道 1 的中断挂起位 */
    DMA2->IFCR |= DMA2_Channel1_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel2)
  {
    /* 复位 DMA2 通道 2 的中断挂起位 */
    DMA2->IFCR |= DMA2_Channel2_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel3)
  {
    /* 复位 DMA2 通道 3 的中断挂起位 */
    DMA2->IFCR |= DMA2_Channel3_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel4)
  {
    /* 复位 DMA2 通道 4 的中断挂起位 */
    DMA2->IFCR |= DMA2_Channel4_IT_Mask;
  }
  else
  { 
    if (DMAy_Channelx == DMA2_Channel5)
    {
      /* 复位 DMA2 通道 5 的中断挂起位 */
      DMA2->IFCR |= DMA2_Channel5_IT_Mask;
    }
  }
}

/**
  * @brief  根据 DMA_InitStruct 中指定的参数初始化 DMAy 通道 x。
  * @param  DMAy_Channelx: 其中 y 可取 1 或 2 以选择 DMA，
  *   x 对于 DMA1 可取 1 到 7，对于 DMA2 可取 1 到 5，以选择 DMA 通道。
  * @param  DMA_InitStruct: 指向 DMA_InitTypeDef 结构的指针，
  *         该结构包含指定 DMA 通道的配置信息。
  * @retval 无
  */
void DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct)
{
  uint32_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_DIR(DMA_InitStruct->DMA_DIR));
  assert_param(IS_DMA_BUFFER_SIZE(DMA_InitStruct->DMA_BufferSize));
  assert_param(IS_DMA_PERIPHERAL_INC_STATE(DMA_InitStruct->DMA_PeripheralInc));
  assert_param(IS_DMA_MEMORY_INC_STATE(DMA_InitStruct->DMA_MemoryInc));   
  assert_param(IS_DMA_PERIPHERAL_DATA_SIZE(DMA_InitStruct->DMA_PeripheralDataSize));
  assert_param(IS_DMA_MEMORY_DATA_SIZE(DMA_InitStruct->DMA_MemoryDataSize));
  assert_param(IS_DMA_MODE(DMA_InitStruct->DMA_Mode));
  assert_param(IS_DMA_PRIORITY(DMA_InitStruct->DMA_Priority));
  assert_param(IS_DMA_M2M_STATE(DMA_InitStruct->DMA_M2M));

/*--------------------------- DMAy 通道 x CCR 配置 -----------------*/
  /* 获取 DMAy 通道 x CCR 值 */
  tmpreg = DMAy_Channelx->CCR;
  /* 清零 MEM2MEM、PL、MSIZE、PSIZE、MINC、PINC、CIRC 和 DIR 位 */
  tmpreg &= CCR_CLEAR_Mask;
  /* 配置 DMAy 通道 x：数据传输、数据大小、优先级和模式 */
  /* 根据 DMA_DIR 值置位 DIR 位 */
  /* 根据 DMA_Mode 值置位 CIRC 位 */
  /* 根据 DMA_PeripheralInc 值置位 PINC 位 */
  /* 根据 DMA_MemoryInc 值置位 MINC 位 */
  /* 根据 DMA_PeripheralDataSize 值置位 PSIZE 位 */
  /* 根据 DMA_MemoryDataSize 值置位 MSIZE 位 */
  /* 根据 DMA_Priority 值置位 PL 位 */
  /* 根据 DMA_M2M 值置位 MEM2MEM 位 */
  tmpreg |= DMA_InitStruct->DMA_DIR | DMA_InitStruct->DMA_Mode |
            DMA_InitStruct->DMA_PeripheralInc | DMA_InitStruct->DMA_MemoryInc |
            DMA_InitStruct->DMA_PeripheralDataSize | DMA_InitStruct->DMA_MemoryDataSize |
            DMA_InitStruct->DMA_Priority | DMA_InitStruct->DMA_M2M;

  /* 写入 DMAy 通道 x CCR */
  DMAy_Channelx->CCR = tmpreg;

/*--------------------------- DMAy 通道 x CNDTR 配置 ---------------*/
  /* 写入 DMAy 通道 x CNDTR */
  DMAy_Channelx->CNDTR = DMA_InitStruct->DMA_BufferSize;

/*--------------------------- DMAy 通道 x CPAR 配置 ----------------*/
  /* 写入 DMAy 通道 x CPAR */
  DMAy_Channelx->CPAR = DMA_InitStruct->DMA_PeripheralBaseAddr;

/*--------------------------- DMAy 通道 x CMAR 配置 ----------------*/
  /* 写入 DMAy 通道 x CMAR */
  DMAy_Channelx->CMAR = DMA_InitStruct->DMA_MemoryBaseAddr;
}

/**
  * @brief  将 DMA_InitStruct 的每个成员填充为默认值。
  * @param  DMA_InitStruct : 指向将被初始化的 DMA_InitTypeDef 结构的指针。
  * @retval 无
  */
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct)
{
/*-------------- 复位 DMA 初始化结构体参数值 ------------------*/
  /* 初始化 DMA_PeripheralBaseAddr 成员 */
  DMA_InitStruct->DMA_PeripheralBaseAddr = 0;
  /* 初始化 DMA_MemoryBaseAddr 成员 */
  DMA_InitStruct->DMA_MemoryBaseAddr = 0;
  /* 初始化 DMA_DIR 成员 */
  DMA_InitStruct->DMA_DIR = DMA_DIR_PeripheralSRC;
  /* 初始化 DMA_BufferSize 成员 */
  DMA_InitStruct->DMA_BufferSize = 0;
  /* 初始化 DMA_PeripheralInc 成员 */
  DMA_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  /* 初始化 DMA_MemoryInc 成员 */
  DMA_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Disable;
  /* 初始化 DMA_PeripheralDataSize 成员 */
  DMA_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  /* 初始化 DMA_MemoryDataSize 成员 */
  DMA_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  /* 初始化 DMA_Mode 成员 */
  DMA_InitStruct->DMA_Mode = DMA_Mode_Normal;
  /* 初始化 DMA_Priority 成员 */
  DMA_InitStruct->DMA_Priority = DMA_Priority_Low;
  /* 初始化 DMA_M2M 成员 */
  DMA_InitStruct->DMA_M2M = DMA_M2M_Disable;
}

/**
  * @brief  使能或关闭指定的 DMAy 通道 x。
  * @param  DMAy_Channelx: 其中 y 可取 1 或 2 以选择 DMA，
  *   x 对于 DMA1 可取 1 到 7，对于 DMA2 可取 1 到 5，以选择 DMA 通道。
  * @param  NewState: DMAy 通道 x 的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void DMA_Cmd(DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* 使能所选 DMAy 通道 x */
    DMAy_Channelx->CCR |= DMA_CCR1_EN;
  }
  else
  {
    /* 关闭所选 DMAy 通道 x */
    DMAy_Channelx->CCR &= (uint16_t)(~DMA_CCR1_EN);
  }
}

/**
  * @brief  使能或关闭指定的 DMAy 通道 x 中断。
  * @param  DMAy_Channelx: 其中 y 可取 1 或 2 以选择 DMA，
  *   x 对于 DMA1 可取 1 到 7，对于 DMA2 可取 1 到 5，以选择 DMA 通道。
  * @param  DMA_IT: 指定要使能或关闭的 DMA 中断源。
  *   该参数可以是以下值的任意组合：
  *     @arg DMA_IT_TC:  传输完成中断掩码
  *     @arg DMA_IT_HT:  半传输中断掩码
  *     @arg DMA_IT_TE:  传输错误中断掩码
  * @param  NewState: 指定 DMA 中断的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void DMA_ITConfig(DMA_Channel_TypeDef* DMAy_Channelx, uint32_t DMA_IT, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_CONFIG_IT(DMA_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选 DMA 中断 */
    DMAy_Channelx->CCR |= DMA_IT;
  }
  else
  {
    /* 关闭所选 DMA 中断 */
    DMAy_Channelx->CCR &= ~DMA_IT;
  }
}

/**
  * @brief  设置当前 DMAy 通道 x 传输中的数据单元数。
  * @param  DMAy_Channelx: 其中 y 可取 1 或 2 以选择 DMA，
  *         x 对于 DMA1 可取 1 到 7，对于 DMA2 可取 1 到 5，以选择 DMA 通道。
  * @param  DataNumber: 当前 DMAy 通道 x 传输中的数据单元数。
  * @note   本函数只能在 DMAy_Channelx 被关闭时使用。
  * @retval 无。
  */
void DMA_SetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx, uint16_t DataNumber)
{
  /* 检查参数 */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  
/*--------------------------- DMAy 通道 x CNDTR 配置 ---------------*/
  /* 写入 DMAy 通道 x CNDTR */
  DMAy_Channelx->CNDTR = DataNumber;  
}

/**
  * @brief  返回当前 DMAy 通道 x 传输中剩余的数据单元数。
  * @param  DMAy_Channelx: 其中 y 可取 1 或 2 以选择 DMA，
  *   x 对于 DMA1 可取 1 到 7，对于 DMA2 可取 1 到 5，以选择 DMA 通道。
  * @retval 当前 DMAy 通道 x 传输中剩余的数据单元数。
  */
uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx)
{
  /* 检查参数 */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  /* 返回 DMAy 通道 x 的剩余数据单元数 */
  return ((uint16_t)(DMAy_Channelx->CNDTR));
}

/**
  * @brief  检查指定的 DMAy 通道 x 标志是否被置位。
  * @param  DMAy_FLAG: 指定要检查的标志。
  *   该参数可取以下值之一：
  *     @arg DMA1_FLAG_GL1: DMA1 通道 1 全局标志。
  *     @arg DMA1_FLAG_TC1: DMA1 通道 1 传输完成标志。
  *     @arg DMA1_FLAG_HT1: DMA1 通道 1 半传输标志。
  *     @arg DMA1_FLAG_TE1: DMA1 通道 1 传输错误标志。
  *     @arg DMA1_FLAG_GL2: DMA1 通道 2 全局标志。
  *     @arg DMA1_FLAG_TC2: DMA1 通道 2 传输完成标志。
  *     @arg DMA1_FLAG_HT2: DMA1 通道 2 半传输标志。
  *     @arg DMA1_FLAG_TE2: DMA1 通道 2 传输错误标志。
  *     @arg DMA1_FLAG_GL3: DMA1 通道 3 全局标志。
  *     @arg DMA1_FLAG_TC3: DMA1 通道 3 传输完成标志。
  *     @arg DMA1_FLAG_HT3: DMA1 通道 3 半传输标志。
  *     @arg DMA1_FLAG_TE3: DMA1 通道 3 传输错误标志。
  *     @arg DMA1_FLAG_GL4: DMA1 通道 4 全局标志。
  *     @arg DMA1_FLAG_TC4: DMA1 通道 4 传输完成标志。
  *     @arg DMA1_FLAG_HT4: DMA1 通道 4 半传输标志。
  *     @arg DMA1_FLAG_TE4: DMA1 通道 4 传输错误标志。
  *     @arg DMA1_FLAG_GL5: DMA1 通道 5 全局标志。
  *     @arg DMA1_FLAG_TC5: DMA1 通道 5 传输完成标志。
  *     @arg DMA1_FLAG_HT5: DMA1 通道 5 半传输标志。
  *     @arg DMA1_FLAG_TE5: DMA1 通道 5 传输错误标志。
  *     @arg DMA1_FLAG_GL6: DMA1 通道 6 全局标志。
  *     @arg DMA1_FLAG_TC6: DMA1 通道 6 传输完成标志。
  *     @arg DMA1_FLAG_HT6: DMA1 通道 6 半传输标志。
  *     @arg DMA1_FLAG_TE6: DMA1 通道 6 传输错误标志。
  *     @arg DMA1_FLAG_GL7: DMA1 通道 7 全局标志。
  *     @arg DMA1_FLAG_TC7: DMA1 通道 7 传输完成标志。
  *     @arg DMA1_FLAG_HT7: DMA1 通道 7 半传输标志。
  *     @arg DMA1_FLAG_TE7: DMA1 通道 7 传输错误标志。
  *     @arg DMA2_FLAG_GL1: DMA2 通道 1 全局标志。
  *     @arg DMA2_FLAG_TC1: DMA2 通道 1 传输完成标志。
  *     @arg DMA2_FLAG_HT1: DMA2 通道 1 半传输标志。
  *     @arg DMA2_FLAG_TE1: DMA2 通道 1 传输错误标志。
  *     @arg DMA2_FLAG_GL2: DMA2 通道 2 全局标志。
  *     @arg DMA2_FLAG_TC2: DMA2 通道 2 传输完成标志。
  *     @arg DMA2_FLAG_HT2: DMA2 通道 2 半传输标志。
  *     @arg DMA2_FLAG_TE2: DMA2 通道 2 传输错误标志。
  *     @arg DMA2_FLAG_GL3: DMA2 通道 3 全局标志。
  *     @arg DMA2_FLAG_TC3: DMA2 通道 3 传输完成标志。
  *     @arg DMA2_FLAG_HT3: DMA2 通道 3 半传输标志。
  *     @arg DMA2_FLAG_TE3: DMA2 通道 3 传输错误标志。
  *     @arg DMA2_FLAG_GL4: DMA2 通道 4 全局标志。
  *     @arg DMA2_FLAG_TC4: DMA2 通道 4 传输完成标志。
  *     @arg DMA2_FLAG_HT4: DMA2 通道 4 半传输标志。
  *     @arg DMA2_FLAG_TE4: DMA2 通道 4 传输错误标志。
  *     @arg DMA2_FLAG_GL5: DMA2 通道 5 全局标志。
  *     @arg DMA2_FLAG_TC5: DMA2 通道 5 传输完成标志。
  *     @arg DMA2_FLAG_HT5: DMA2 通道 5 半传输标志。
  *     @arg DMA2_FLAG_TE5: DMA2 通道 5 传输错误标志。
  * @retval DMAy_FLAG 的新状态 (SET 或 RESET)。
  */
FlagStatus DMA_GetFlagStatus(uint32_t DMAy_FLAG)
{
  FlagStatus bitstatus = RESET;
  uint32_t tmpreg = 0;
  
  /* 检查参数 */
  assert_param(IS_DMA_GET_FLAG(DMAy_FLAG));

  /* 计算所使用的 DMAy */
  if ((DMAy_FLAG & FLAG_Mask) != (uint32_t)RESET)
  {
    /* 获取 DMA2 ISR 寄存器值 */
    tmpreg = DMA2->ISR ;
  }
  else
  {
    /* 获取 DMA1 ISR 寄存器值 */
    tmpreg = DMA1->ISR ;
  }

  /* 检查指定 DMAy 标志的状态 */
  if ((tmpreg & DMAy_FLAG) != (uint32_t)RESET)
  {
    /* DMAy_FLAG 已置位 */
    bitstatus = SET;
  }
  else
  {
    /* DMAy_FLAG 已清零 */
    bitstatus = RESET;
  }
  
  /* 返回 DMAy_FLAG 状态 */
  return  bitstatus;
}

/**
  * @brief  清除 DMAy 通道 x 的挂起标志。
  * @param  DMAy_FLAG: 指定要清除的标志。
  *   该参数可以是 (同一 DMA 的) 以下值的任意组合：
  *     @arg DMA1_FLAG_GL1: DMA1 通道 1 全局标志。
  *     @arg DMA1_FLAG_TC1: DMA1 通道 1 传输完成标志。
  *     @arg DMA1_FLAG_HT1: DMA1 通道 1 半传输标志。
  *     @arg DMA1_FLAG_TE1: DMA1 通道 1 传输错误标志。
  *     @arg DMA1_FLAG_GL2: DMA1 通道 2 全局标志。
  *     @arg DMA1_FLAG_TC2: DMA1 通道 2 传输完成标志。
  *     @arg DMA1_FLAG_HT2: DMA1 通道 2 半传输标志。
  *     @arg DMA1_FLAG_TE2: DMA1 通道 2 传输错误标志。
  *     @arg DMA1_FLAG_GL3: DMA1 通道 3 全局标志。
  *     @arg DMA1_FLAG_TC3: DMA1 通道 3 传输完成标志。
  *     @arg DMA1_FLAG_HT3: DMA1 通道 3 半传输标志。
  *     @arg DMA1_FLAG_TE3: DMA1 通道 3 传输错误标志。
  *     @arg DMA1_FLAG_GL4: DMA1 通道 4 全局标志。
  *     @arg DMA1_FLAG_TC4: DMA1 通道 4 传输完成标志。
  *     @arg DMA1_FLAG_HT4: DMA1 通道 4 半传输标志。
  *     @arg DMA1_FLAG_TE4: DMA1 通道 4 传输错误标志。
  *     @arg DMA1_FLAG_GL5: DMA1 通道 5 全局标志。
  *     @arg DMA1_FLAG_TC5: DMA1 通道 5 传输完成标志。
  *     @arg DMA1_FLAG_HT5: DMA1 通道 5 半传输标志。
  *     @arg DMA1_FLAG_TE5: DMA1 通道 5 传输错误标志。
  *     @arg DMA1_FLAG_GL6: DMA1 通道 6 全局标志。
  *     @arg DMA1_FLAG_TC6: DMA1 通道 6 传输完成标志。
  *     @arg DMA1_FLAG_HT6: DMA1 通道 6 半传输标志。
  *     @arg DMA1_FLAG_TE6: DMA1 通道 6 传输错误标志。
  *     @arg DMA1_FLAG_GL7: DMA1 通道 7 全局标志。
  *     @arg DMA1_FLAG_TC7: DMA1 通道 7 传输完成标志。
  *     @arg DMA1_FLAG_HT7: DMA1 通道 7 半传输标志。
  *     @arg DMA1_FLAG_TE7: DMA1 通道 7 传输错误标志。
  *     @arg DMA2_FLAG_GL1: DMA2 通道 1 全局标志。
  *     @arg DMA2_FLAG_TC1: DMA2 通道 1 传输完成标志。
  *     @arg DMA2_FLAG_HT1: DMA2 通道 1 半传输标志。
  *     @arg DMA2_FLAG_TE1: DMA2 通道 1 传输错误标志。
  *     @arg DMA2_FLAG_GL2: DMA2 通道 2 全局标志。
  *     @arg DMA2_FLAG_TC2: DMA2 通道 2 传输完成标志。
  *     @arg DMA2_FLAG_HT2: DMA2 通道 2 半传输标志。
  *     @arg DMA2_FLAG_TE2: DMA2 通道 2 传输错误标志。
  *     @arg DMA2_FLAG_GL3: DMA2 通道 3 全局标志。
  *     @arg DMA2_FLAG_TC3: DMA2 通道 3 传输完成标志。
  *     @arg DMA2_FLAG_HT3: DMA2 通道 3 半传输标志。
  *     @arg DMA2_FLAG_TE3: DMA2 通道 3 传输错误标志。
  *     @arg DMA2_FLAG_GL4: DMA2 通道 4 全局标志。
  *     @arg DMA2_FLAG_TC4: DMA2 通道 4 传输完成标志。
  *     @arg DMA2_FLAG_HT4: DMA2 通道 4 半传输标志。
  *     @arg DMA2_FLAG_TE4: DMA2 通道 4 传输错误标志。
  *     @arg DMA2_FLAG_GL5: DMA2 通道 5 全局标志。
  *     @arg DMA2_FLAG_TC5: DMA2 通道 5 传输完成标志。
  *     @arg DMA2_FLAG_HT5: DMA2 通道 5 半传输标志。
  *     @arg DMA2_FLAG_TE5: DMA2 通道 5 传输错误标志。
  * @retval 无
  */
void DMA_ClearFlag(uint32_t DMAy_FLAG)
{
  /* 检查参数 */
  assert_param(IS_DMA_CLEAR_FLAG(DMAy_FLAG));

  /* 计算所使用的 DMAy */
  if ((DMAy_FLAG & FLAG_Mask) != (uint32_t)RESET)
  {
    /* 清除所选 DMAy 标志 */
    DMA2->IFCR = DMAy_FLAG;
  }
  else
  {
    /* 清除所选 DMAy 标志 */
    DMA1->IFCR = DMAy_FLAG;
  }
}

/**
  * @brief  检查指定的 DMAy 通道 x 中断是否发生。
  * @param  DMAy_IT: 指定要检查的 DMAy 中断源。
  *   该参数可取以下值之一：
  *     @arg DMA1_IT_GL1: DMA1 通道 1 全局中断。
  *     @arg DMA1_IT_TC1: DMA1 通道 1 传输完成中断。
  *     @arg DMA1_IT_HT1: DMA1 通道 1 半传输中断。
  *     @arg DMA1_IT_TE1: DMA1 通道 1 传输错误中断。
  *     @arg DMA1_IT_GL2: DMA1 通道 2 全局中断。
  *     @arg DMA1_IT_TC2: DMA1 通道 2 传输完成中断。
  *     @arg DMA1_IT_HT2: DMA1 通道 2 半传输中断。
  *     @arg DMA1_IT_TE2: DMA1 通道 2 传输错误中断。
  *     @arg DMA1_IT_GL3: DMA1 通道 3 全局中断。
  *     @arg DMA1_IT_TC3: DMA1 通道 3 传输完成中断。
  *     @arg DMA1_IT_HT3: DMA1 通道 3 半传输中断。
  *     @arg DMA1_IT_TE3: DMA1 通道 3 传输错误中断。
  *     @arg DMA1_IT_GL4: DMA1 通道 4 全局中断。
  *     @arg DMA1_IT_TC4: DMA1 通道 4 传输完成中断。
  *     @arg DMA1_IT_HT4: DMA1 通道 4 半传输中断。
  *     @arg DMA1_IT_TE4: DMA1 通道 4 传输错误中断。
  *     @arg DMA1_IT_GL5: DMA1 通道 5 全局中断。
  *     @arg DMA1_IT_TC5: DMA1 通道 5 传输完成中断。
  *     @arg DMA1_IT_HT5: DMA1 通道 5 半传输中断。
  *     @arg DMA1_IT_TE5: DMA1 通道 5 传输错误中断。
  *     @arg DMA1_IT_GL6: DMA1 通道 6 全局中断。
  *     @arg DMA1_IT_TC6: DMA1 通道 6 传输完成中断。
  *     @arg DMA1_IT_HT6: DMA1 通道 6 半传输中断。
  *     @arg DMA1_IT_TE6: DMA1 通道 6 传输错误中断。
  *     @arg DMA1_IT_GL7: DMA1 通道 7 全局中断。
  *     @arg DMA1_IT_TC7: DMA1 通道 7 传输完成中断。
  *     @arg DMA1_IT_HT7: DMA1 通道 7 半传输中断。
  *     @arg DMA1_IT_TE7: DMA1 通道 7 传输错误中断。
  *     @arg DMA2_IT_GL1: DMA2 通道 1 全局中断。
  *     @arg DMA2_IT_TC1: DMA2 通道 1 传输完成中断。
  *     @arg DMA2_IT_HT1: DMA2 通道 1 半传输中断。
  *     @arg DMA2_IT_TE1: DMA2 通道 1 传输错误中断。
  *     @arg DMA2_IT_GL2: DMA2 通道 2 全局中断。
  *     @arg DMA2_IT_TC2: DMA2 通道 2 传输完成中断。
  *     @arg DMA2_IT_HT2: DMA2 通道 2 半传输中断。
  *     @arg DMA2_IT_TE2: DMA2 通道 2 传输错误中断。
  *     @arg DMA2_IT_GL3: DMA2 通道 3 全局中断。
  *     @arg DMA2_IT_TC3: DMA2 通道 3 传输完成中断。
  *     @arg DMA2_IT_HT3: DMA2 通道 3 半传输中断。
  *     @arg DMA2_IT_TE3: DMA2 通道 3 传输错误中断。
  *     @arg DMA2_IT_GL4: DMA2 通道 4 全局中断。
  *     @arg DMA2_IT_TC4: DMA2 通道 4 传输完成中断。
  *     @arg DMA2_IT_HT4: DMA2 通道 4 半传输中断。
  *     @arg DMA2_IT_TE4: DMA2 通道 4 传输错误中断。
  *     @arg DMA2_IT_GL5: DMA2 通道 5 全局中断。
  *     @arg DMA2_IT_TC5: DMA2 通道 5 传输完成中断。
  *     @arg DMA2_IT_HT5: DMA2 通道 5 半传输中断。
  *     @arg DMA2_IT_TE5: DMA2 通道 5 传输错误中断。
  * @retval DMAy_IT 的新状态 (SET 或 RESET)。
  */
ITStatus DMA_GetITStatus(uint32_t DMAy_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_DMA_GET_IT(DMAy_IT));

  /* 计算所使用的 DMA */
  if ((DMAy_IT & FLAG_Mask) != (uint32_t)RESET)
  {
    /* 获取 DMA2 ISR 寄存器值 */
    tmpreg = DMA2->ISR;
  }
  else
  {
    /* 获取 DMA1 ISR 寄存器值 */
    tmpreg = DMA1->ISR;
  }

  /* 检查指定 DMAy 中断的状态 */
  if ((tmpreg & DMAy_IT) != (uint32_t)RESET)
  {
    /* DMAy_IT 已置位 */
    bitstatus = SET;
  }
  else
  {
    /* DMAy_IT 已清零 */
    bitstatus = RESET;
  }
  /* 返回 DMA_IT 状态 */
  return  bitstatus;
}

/**
  * @brief  清除 DMAy 通道 x 的中断挂起位。
  * @param  DMAy_IT: 指定要清除的 DMAy 中断挂起位。
  *   该参数可以是 (同一 DMA 的) 以下值的任意组合：
  *     @arg DMA1_IT_GL1: DMA1 通道 1 全局中断。
  *     @arg DMA1_IT_TC1: DMA1 通道 1 传输完成中断。
  *     @arg DMA1_IT_HT1: DMA1 通道 1 半传输中断。
  *     @arg DMA1_IT_TE1: DMA1 通道 1 传输错误中断。
  *     @arg DMA1_IT_GL2: DMA1 通道 2 全局中断。
  *     @arg DMA1_IT_TC2: DMA1 通道 2 传输完成中断。
  *     @arg DMA1_IT_HT2: DMA1 通道 2 半传输中断。
  *     @arg DMA1_IT_TE2: DMA1 通道 2 传输错误中断。
  *     @arg DMA1_IT_GL3: DMA1 通道 3 全局中断。
  *     @arg DMA1_IT_TC3: DMA1 通道 3 传输完成中断。
  *     @arg DMA1_IT_HT3: DMA1 通道 3 半传输中断。
  *     @arg DMA1_IT_TE3: DMA1 通道 3 传输错误中断。
  *     @arg DMA1_IT_GL4: DMA1 通道 4 全局中断。
  *     @arg DMA1_IT_TC4: DMA1 通道 4 传输完成中断。
  *     @arg DMA1_IT_HT4: DMA1 通道 4 半传输中断。
  *     @arg DMA1_IT_TE4: DMA1 通道 4 传输错误中断。
  *     @arg DMA1_IT_GL5: DMA1 通道 5 全局中断。
  *     @arg DMA1_IT_TC5: DMA1 通道 5 传输完成中断。
  *     @arg DMA1_IT_HT5: DMA1 通道 5 半传输中断。
  *     @arg DMA1_IT_TE5: DMA1 通道 5 传输错误中断。
  *     @arg DMA1_IT_GL6: DMA1 通道 6 全局中断。
  *     @arg DMA1_IT_TC6: DMA1 通道 6 传输完成中断。
  *     @arg DMA1_IT_HT6: DMA1 通道 6 半传输中断。
  *     @arg DMA1_IT_TE6: DMA1 通道 6 传输错误中断。
  *     @arg DMA1_IT_GL7: DMA1 通道 7 全局中断。
  *     @arg DMA1_IT_TC7: DMA1 通道 7 传输完成中断。
  *     @arg DMA1_IT_HT7: DMA1 通道 7 半传输中断。
  *     @arg DMA1_IT_TE7: DMA1 通道 7 传输错误中断。
  *     @arg DMA2_IT_GL1: DMA2 通道 1 全局中断。
  *     @arg DMA2_IT_TC1: DMA2 通道 1 传输完成中断。
  *     @arg DMA2_IT_HT1: DMA2 通道 1 半传输中断。
  *     @arg DMA2_IT_TE1: DMA2 通道 1 传输错误中断。
  *     @arg DMA2_IT_GL2: DMA2 通道 2 全局中断。
  *     @arg DMA2_IT_TC2: DMA2 通道 2 传输完成中断。
  *     @arg DMA2_IT_HT2: DMA2 通道 2 半传输中断。
  *     @arg DMA2_IT_TE2: DMA2 通道 2 传输错误中断。
  *     @arg DMA2_IT_GL3: DMA2 通道 3 全局中断。
  *     @arg DMA2_IT_TC3: DMA2 通道 3 传输完成中断。
  *     @arg DMA2_IT_HT3: DMA2 通道 3 半传输中断。
  *     @arg DMA2_IT_TE3: DMA2 通道 3 传输错误中断。
  *     @arg DMA2_IT_GL4: DMA2 通道 4 全局中断。
  *     @arg DMA2_IT_TC4: DMA2 通道 4 传输完成中断。
  *     @arg DMA2_IT_HT4: DMA2 通道 4 半传输中断。
  *     @arg DMA2_IT_TE4: DMA2 通道 4 传输错误中断。
  *     @arg DMA2_IT_GL5: DMA2 通道 5 全局中断。
  *     @arg DMA2_IT_TC5: DMA2 通道 5 传输完成中断。
  *     @arg DMA2_IT_HT5: DMA2 通道 5 半传输中断。
  *     @arg DMA2_IT_TE5: DMA2 通道 5 传输错误中断。
  * @retval 无
  */
void DMA_ClearITPendingBit(uint32_t DMAy_IT)
{
  /* 检查参数 */
  assert_param(IS_DMA_CLEAR_IT(DMAy_IT));

  /* 计算所使用的 DMAy */
  if ((DMAy_IT & FLAG_Mask) != (uint32_t)RESET)
  {
    /* 清除所选 DMAy 中断挂起位 */
    DMA2->IFCR = DMAy_IT;
  }
  else
  {
    /* 清除所选 DMAy 中断挂起位 */
    DMA1->IFCR = DMAy_IT;
  }
}

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

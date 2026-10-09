/**
  ******************************************************************************
  * @file    stm32f1xx_hal_dma.c
  * @author  MCD Application Team
  * @brief   DMA HAL 模块驱动程序。
  *          本文件提供用于管理直接内存访问（DMA）外设以下功能的固件函数：
  *           + 初始化和反初始化函数
  *           + IO 操作函数
  *           + 外设状态与错误管理函数
  @verbatim
  ==============================================================================
                        ##### 如何使用此驱动程序 #####
  ==============================================================================
  [..]
   (#) 使能并配置连接到对应 DMA 通道的外设
       （内部 SRAM / FLASH 存储器除外：无需进行初始化）。
       有关外设与 DMA 请求之间的映射关系，请参阅参考手册。

   (#) 针对指定的通道，使用 HAL_DMA_Init() 函数配置以下所需参数：
       通道请求、传输方向、源与目标数据宽度、
       循环模式或普通模式、通道优先级、源与目标地址递增模式。

   (#) 使用 HAL_DMA_GetState() 函数获取 DMA 状态，并在检测到错误时使用 HAL_DMA_GetError() 获取错误码。
                    
   (#) 使用 HAL_DMA_Abort() 函数中止当前的传输。
                   
     -@-   在内存到内存传输模式下，不允许使用循环模式。
     *** 轮询模式 IO 操作 ***
     =================================
    [..]
          (+) 在配置好源地址、目标地址以及待传输的数据长度后，使用 HAL_DMA_Start() 启动 DMA 传输。
          (+) 使用 HAL_DMA_PollForTransfer() 轮询等待当前传输完成，在此情况下，用户可根据具体应用需求配置固定的超时时间。

     *** 中断模式 IO 操作 ***
     ===================================
    [..]
          (+) 使用 HAL_NVIC_SetPriority() 配置 DMA 中断优先级。
          (+) 使用 HAL_NVIC_EnableIRQ() 使能 DMA 中断处理程序。
          (+) 在配置好源地址、目标地址以及待传输的数据长度后，使用 HAL_DMA_Start_IT() 启动 DMA 传输。
              此时将配置并使能 DMA 中断。
          (+) 在 DMA_IRQHandler() 中断服务例程中调用 HAL_DMA_IRQHandler()。
          (+) 数据传输完成时将执行 HAL_DMA_IRQHandler() 函数，用户可以通过自定义函数指针 XferCpltCallback 和
              XferErrorCallback（即 DMA 句柄结构体的成员）来添加自己的回调函数。

     *** DMA HAL 驱动宏列表 ***
     ============================================= 
      [..]
       以下是 DMA HAL 驱动中最常用的宏列表：

       (+) __HAL_DMA_ENABLE: 使能指定的 DMA 通道。
       (+) __HAL_DMA_DISABLE: 禁用指定的 DMA 通道。
       (+) __HAL_DMA_GET_FLAG: 获取指定的 DMA 通道挂起标志位。
       (+) __HAL_DMA_CLEAR_FLAG: 清除指定的 DMA 通道挂起标志位。
       (+) __HAL_DMA_ENABLE_IT: 使能指定的 DMA 通道中断。
       (+) __HAL_DMA_DISABLE_IT: 禁用指定的 DMA 通道中断。
       (+) __HAL_DMA_GET_IT_SOURCE: 检查指定的 DMA 通道中断是否发生。

     [..] 
      (@) 您可以参考 DMA HAL 驱动头文件获取更多有用的宏。

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup DMA DMA
  * @brief DMA HAL 模块驱动
  * @{
  */

#ifdef HAL_DMA_MODULE_ENABLED

/* 私有类型定义 -----------------------------------------------------------*/
/* 私有宏定义 ------------------------------------------------------------*/
/* 私有宏 -----------------------------------------------------------------*/
/* 私有变量 -------------------------------------------------------------*/
/* 私有函数原型 -----------------------------------------------------------*/
/** @defgroup DMA_Private_Functions DMA 私有函数
  * @{
  */
static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
/**
  * @}
  */

/* 导出函数 ---------------------------------------------------------*/

/** @defgroup DMA_Exported_Functions DMA 导出函数
  * @{
  */

/** @defgroup DMA_Exported_Functions_Group1 初始化和反初始化函数
  *  @brief   初始化和反初始化函数 
  *
@verbatim
 ===============================================================================
             ##### 初始化和反初始化函数  #####
 ===============================================================================
    [..]
    本节提供的函数用于初始化 DMA 通道的源地址与目标地址、
    地址递增模式与数据宽度、传输方向、循环/普通模式选择、
    内存到内存模式选择以及通道优先级值。
    [..]
    HAL_DMA_Init() 函数遵循参考手册中描述的 DMA 配置流程。

@endverbatim
  * @{
  */

/**
  * @brief  根据 DMA_InitTypeDef 中指定的参数初始化 DMA，
  *         并初始化关联的句柄。
  * @param  hdma: 指向包含指定 DMA 通道配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma)
{
  uint32_t tmp = 0U;

  /* 检查 DMA 句柄分配 */
  if(hdma == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
  assert_param(IS_DMA_DIRECTION(hdma->Init.Direction));
  assert_param(IS_DMA_PERIPHERAL_INC_STATE(hdma->Init.PeriphInc));
  assert_param(IS_DMA_MEMORY_INC_STATE(hdma->Init.MemInc));
  assert_param(IS_DMA_PERIPHERAL_DATA_SIZE(hdma->Init.PeriphDataAlignment));
  assert_param(IS_DMA_MEMORY_DATA_SIZE(hdma->Init.MemDataAlignment));
  assert_param(IS_DMA_MODE(hdma->Init.Mode));
  assert_param(IS_DMA_PRIORITY(hdma->Init.Priority));

#if defined (DMA2)
  /* 计算通道索引 */
  if ((uint32_t)(hdma->Instance) < (uint32_t)(DMA2_Channel1))
  {
    /* DMA1 */
    hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA1_Channel1) / ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1)) << 2;
    hdma->DmaBaseAddress = DMA1;
  }
  else 
  {
    /* DMA2 */
    hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA2_Channel1) / ((uint32_t)DMA2_Channel2 - (uint32_t)DMA2_Channel1)) << 2;
    hdma->DmaBaseAddress = DMA2;
  }
#else
  /* DMA1 */
  hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA1_Channel1) / ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1)) << 2;
  hdma->DmaBaseAddress = DMA1;
#endif /* DMA2 */

  /* 更改 DMA 外设状态 */
  hdma->State = HAL_DMA_STATE_BUSY;

  /* 获取 CR 寄存器的值 */
  tmp = hdma->Instance->CCR;

  /* 清除 PL、MSIZE、PSIZE、MINC、PINC、CIRC 和 DIR 位 */
  tmp &= ((uint32_t)~(DMA_CCR_PL    | DMA_CCR_MSIZE  | DMA_CCR_PSIZE  | \
                      DMA_CCR_MINC  | DMA_CCR_PINC   | DMA_CCR_CIRC   | \
                      DMA_CCR_DIR));

  /* 准备 DMA 通道配置 */
  tmp |=  hdma->Init.Direction        |
          hdma->Init.PeriphInc           | hdma->Init.MemInc           |
          hdma->Init.PeriphDataAlignment | hdma->Init.MemDataAlignment |
          hdma->Init.Mode                | hdma->Init.Priority;

  /* 写入 DMA 通道 CR 寄存器 */
  hdma->Instance->CCR = tmp;

  /* 初始化错误代码 */
  hdma->ErrorCode = HAL_DMA_ERROR_NONE;

  /* 初始化 DMA 状态 */
  hdma->State = HAL_DMA_STATE_READY;
  /* 分配锁资源并进行初始化 */
  hdma->Lock = HAL_UNLOCKED;

  return HAL_OK;
}

/**
  * @brief  反初始化 DMA 外设。
  * @param  hdma: 指向包含指定 DMA 通道配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_DMA_DeInit(DMA_HandleTypeDef *hdma)
{
  /* 检查 DMA 句柄分配 */
  if(hdma == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));

  /* 禁用选定的 DMA Channelx */
  __HAL_DMA_DISABLE(hdma);

  /* 复位 DMA 通道控制寄存器 */
  hdma->Instance->CCR  = 0U;

  /* 复位 DMA 通道数据传输数量寄存器 */
  hdma->Instance->CNDTR = 0U;

  /* 复位 DMA 通道外设地址寄存器 */
  hdma->Instance->CPAR  = 0U;

  /* 复位 DMA 通道内存地址寄存器 */
  hdma->Instance->CMAR = 0U;

#if defined (DMA2)
  /* 计算通道索引 */
  if ((uint32_t)(hdma->Instance) < (uint32_t)(DMA2_Channel1))
  {
    /* DMA1 */
    hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA1_Channel1) / ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1)) << 2;
    hdma->DmaBaseAddress = DMA1;
  }
  else
  {
    /* DMA2 */
    hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA2_Channel1) / ((uint32_t)DMA2_Channel2 - (uint32_t)DMA2_Channel1)) << 2;
    hdma->DmaBaseAddress = DMA2;
  }
#else
  /* DMA1 */
  hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA1_Channel1) / ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1)) << 2;
  hdma->DmaBaseAddress = DMA1;
#endif /* DMA2 */

  /* 清除所有标志位 */
  hdma->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << (hdma->ChannelIndex));

  /* 清除所有回调函数 */
  hdma->XferCpltCallback = NULL;
  hdma->XferHalfCpltCallback = NULL;
  hdma->XferErrorCallback = NULL;
  hdma->XferAbortCallback = NULL;

  /* 复位错误代码 */
  hdma->ErrorCode = HAL_DMA_ERROR_NONE;

  /* 复位 DMA 状态 */
  hdma->State = HAL_DMA_STATE_RESET;

  /* 释放锁 */
  __HAL_UNLOCK(hdma);

  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup DMA_Exported_Functions_Group2 输入和输出操作函数
  *  @brief   输入和输出操作函数
  *
@verbatim
 ===============================================================================
                      #####  IO 操作函数  #####
 ===============================================================================
    [..]  本节提供的函数允许：
      (+) 配置源地址、目的地址和数据长度，并启动 DMA 传输
      (+) 配置源地址、目的地址和数据长度，并
          在开启中断的情况下启动 DMA 传输
      (+) 中止 DMA 传输
      (+) 轮询传输完成状态
      (+) 处理 DMA 中断请求

@endverbatim
  * @{
  */

/**
  * @brief  启动 DMA 传输。
  * @param  hdma: 指向 DMA_HandleTypeDef 结构体的指针，该结构体包含
  *               指定 DMA 通道的配置信息。
  * @param  SrcAddress: 源内存缓冲区地址
  * @param  DstAddress: 目的内存缓冲区地址
  * @param  DataLength: 从源传输到目的的数据长度
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_DMA_BUFFER_SIZE(DataLength));

  /* 进程加锁 */
  __HAL_LOCK(hdma);

  if(HAL_DMA_STATE_READY == hdma->State)
  {
    /* 改变 DMA 外设状态 */
    hdma->State = HAL_DMA_STATE_BUSY;
    hdma->ErrorCode = HAL_DMA_ERROR_NONE;
            
    /* 禁用外设 */
    __HAL_DMA_DISABLE(hdma);
    
    /* 配置源地址、目的地址和数据长度并清除标志位 */
    DMA_SetConfig(hdma, SrcAddress, DstAddress, DataLength);
    
    /* 使能外设 */
    __HAL_DMA_ENABLE(hdma);
  }
  else
  {
   /* 进程解锁 */
   __HAL_UNLOCK(hdma);  
   status = HAL_BUSY;
  }  
  return status;
}

/**
  * @brief  在开启中断的情况下启动 DMA 传输。
  * @param  hdma: 指向 DMA_HandleTypeDef 结构体的指针，该结构体包含
  *               指定 DMA 通道的配置信息。
  * @param  SrcAddress: 源内存缓冲区地址
  * @param  DstAddress: 目的内存缓冲区地址
  * @param  DataLength: 从源传输到目的的数据长度
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 检查参数 */
  assert_param(IS_DMA_BUFFER_SIZE(DataLength));

  /* 进程已上锁 */
  __HAL_LOCK(hdma);
  
  if(HAL_DMA_STATE_READY == hdma->State)
  {
    /* 改变 DMA 外设状态 */
    hdma->State = HAL_DMA_STATE_BUSY;
    hdma->ErrorCode = HAL_DMA_ERROR_NONE;
    
    /* 禁用外设 */
    __HAL_DMA_DISABLE(hdma);
    
    /* 配置源地址、目的地址和数据传输长度并清除标志位 */
    DMA_SetConfig(hdma, SrcAddress, DstAddress, DataLength);
    
    /* 使能传输完成中断 */
    /* 使能传输错误中断 */
    if(NULL != hdma->XferHalfCpltCallback)
    {
      /* 同时也使能半传输完成中断 */
      __HAL_DMA_ENABLE_IT(hdma, (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE));
    }
    else
    {
      __HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT);
      __HAL_DMA_ENABLE_IT(hdma, (DMA_IT_TC | DMA_IT_TE));
    }
    /* 使能外设 */
    __HAL_DMA_ENABLE(hdma);
  }
  else
  {      
    /* 进程已解锁 */
    __HAL_UNLOCK(hdma); 

    /* 保持 BUSY 状态 */
    status = HAL_BUSY;
  }    
  return status;
}

/**
  * @brief  终止 DMA 传输。
  * @param  hdma: 指向包含指定 DMA 通道配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  if(hdma->State != HAL_DMA_STATE_BUSY)
  {
    /* 当前无正在进行的传输 */
    hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;
    
    /* 进程已解锁 */
    __HAL_UNLOCK(hdma);
    
    return HAL_ERROR;
  }
  else

  {
    /* 禁用 DMA 中断 */
    __HAL_DMA_DISABLE_IT(hdma, (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE));
      
    /* 禁用通道 */
    __HAL_DMA_DISABLE(hdma);
      
    /* 清除所有标志位 */
    hdma->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << hdma->ChannelIndex);
  }
  /* 更改 DMA 状态 */
  hdma->State = HAL_DMA_STATE_READY;

  /* 进程解锁 */
  __HAL_UNLOCK(hdma);      
  
  return status; 
}

/**
  * @brief  以中断模式中止 DMA 传输。
  * @param  hdma  : 指向包含指定 DMA 通道配置信息的 DMA_HandleTypeDef 结构的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma)
{  
  HAL_StatusTypeDef status = HAL_OK;
  
  if(HAL_DMA_STATE_BUSY != hdma->State)
  {
    /* 无正在进行的传输 */
    hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;
        
    status = HAL_ERROR;
  }
  else
  { 
    /* 禁用 DMA 中断 */
    __HAL_DMA_DISABLE_IT(hdma, (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE));

    /* 禁用该通道 */
    __HAL_DMA_DISABLE(hdma);

    /* 清除所有标志位 */
    __HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_GI_FLAG_INDEX(hdma));

    /* 更改 DMA 状态 */
    hdma->State = HAL_DMA_STATE_READY;

    /* 进程解锁 */
    __HAL_UNLOCK(hdma);

    /* 调用用户中止回调函数 */
    if(hdma->XferAbortCallback != NULL)
    {
      hdma->XferAbortCallback(hdma);
    } 
  }
  return status;
}

/**
  * @brief  轮询等待传输完成。
  * @param  hdma:    指向包含指定 DMA 通道配置信息的 DMA_HandleTypeDef 结构的指针。
  * @param  CompleteLevel: 指定 DMA 传输完成级别。
  * @param  Timeout:       超时时间。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_DMA_PollForTransfer(DMA_HandleTypeDef *hdma, uint32_t CompleteLevel, uint32_t Timeout)
{
  uint32_t temp;
  uint32_t tickstart = 0U;

  if(HAL_DMA_STATE_BUSY != hdma->State)
  {
    /* 无正在进行的传输 */
    hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;
    __HAL_UNLOCK(hdma);
    return HAL_ERROR;
  }

  /* 循环模式下不支持轮询模式 */
  if (RESET != (hdma->Instance->CCR & DMA_CCR_CIRC))
  {
    hdma->ErrorCode = HAL_DMA_ERROR_NOT_SUPPORTED;
    return HAL_ERROR;
  }
  
  /* 获取对应级别的传输完成标志位 */
  if(CompleteLevel == HAL_DMA_FULL_TRANSFER)
  {
    /* 传输完成标志 */
    temp = __HAL_DMA_GET_TC_FLAG_INDEX(hdma);
  }
  else
  {
    /* 半传输完成标志 */
    temp = __HAL_DMA_GET_HT_FLAG_INDEX(hdma);
  }

  /* 获取滴答计数值 */
  tickstart = HAL_GetTick();

  while(__HAL_DMA_GET_FLAG(hdma, temp) == RESET)
  {
    if((__HAL_DMA_GET_FLAG(hdma, __HAL_DMA_GET_TE_FLAG_INDEX(hdma)) != RESET))
    {
      /* 当发生 DMA 传输错误时 */
      /* 硬件自动清零其 EN 位 */
      /* 清除所有标志位 */
      hdma->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << hdma->ChannelIndex);

      /* 更新错误代码 */
      SET_BIT(hdma->ErrorCode, HAL_DMA_ERROR_TE);

      /* 改变 DMA 状态 */
      hdma->State= HAL_DMA_STATE_READY;

      /* 进程解锁 */
      __HAL_UNLOCK(hdma);

      return HAL_ERROR;
    }
    /* 检查是否超时 */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout))
      {
        /* 更新错误代码 */
        SET_BIT(hdma->ErrorCode, HAL_DMA_ERROR_TIMEOUT);

        /* 改变 DMA 状态 */
        hdma->State = HAL_DMA_STATE_READY;

        /* 进程解锁 */
        __HAL_UNLOCK(hdma);

        return HAL_ERROR;
      }
    }
  }

  if(CompleteLevel == HAL_DMA_FULL_TRANSFER)
  {
    /* 清除传输完成标志 */
    __HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));

    /* 所选 Channelx 的 EN 位被清零（DMA 已禁用且
    所有传输均已完成） */
    hdma->State = HAL_DMA_STATE_READY;
  }
  else
  {
    /* 清除半传输完成标志 */
    __HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_HT_FLAG_INDEX(hdma));
  }
  
  /* 进程解锁 */
  __HAL_UNLOCK(hdma);

  return HAL_OK;
}

/**
  * @brief  处理 DMA 中断请求。
  * @param  hdma: 指向包含指定 DMA 通道配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma)
{
  uint32_t flag_it = hdma->DmaBaseAddress->ISR;
  uint32_t source_it = hdma->Instance->CCR;
  
  /* 半传输完成中断管理 ********************************************************/
  if (((flag_it & (DMA_FLAG_HT1 << hdma->ChannelIndex)) != RESET) && ((source_it & DMA_IT_HT) != RESET))
  {
    /* 若 DMA 模式非循环模式 (CIRCULAR)，则禁用半传输中断 */
    if((hdma->Instance->CCR & DMA_CCR_CIRC) == 0U)
    {
      /* 禁用半传输中断 */
      __HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT);
    }
    /* 清除半传输完成标志 */
    __HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_HT_FLAG_INDEX(hdma));

    /* DMA 外设状态在半传输时不更新，*/
    /* 而是在传输完成时更新 */

    if(hdma->XferHalfCpltCallback != NULL)
    {
      /* 半传输回调函数 */
      hdma->XferHalfCpltCallback(hdma);
    }
  }

  /* 传输完成中断管理 *********************************************************/
  else if (((flag_it & (DMA_FLAG_TC1 << hdma->ChannelIndex)) != RESET) && ((source_it & DMA_IT_TC) != RESET))
  {
    if((hdma->Instance->CCR & DMA_CCR_CIRC) == 0U)
    {
      /* 禁用传输完成和错误中断 */
      __HAL_DMA_DISABLE_IT(hdma, DMA_IT_TE | DMA_IT_TC);  

      /* 改变 DMA 状态 */
      hdma->State = HAL_DMA_STATE_READY;
    }
    /* 清除传输完成标志 */
      __HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));

    /* 进程解锁 */
    __HAL_UNLOCK(hdma);

    if(hdma->XferCpltCallback != NULL)
    {
      /* 传输完成回调函数 */
      hdma->XferCpltCallback(hdma);
    }
  }

  /* 传输错误中断管理 **************************************/
  else if (( RESET != (flag_it & (DMA_FLAG_TE1 << hdma->ChannelIndex))) && (RESET != (source_it & DMA_IT_TE)))
  {
    /* 当发生 DMA 传输错误时 */
    /* 硬件将自动清除其 EN 位 */
    /* 禁用所有 DMA 中断 */
    __HAL_DMA_DISABLE_IT(hdma, (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE));

    /* 清除所有标志位 */
    hdma->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << hdma->ChannelIndex);

    /* 更新错误代码 */
    hdma->ErrorCode = HAL_DMA_ERROR_TE;

    /* 更改 DMA 状态 */
    hdma->State = HAL_DMA_STATE_READY;

    /* 进程解锁 */
    __HAL_UNLOCK(hdma);

    if (hdma->XferErrorCallback != NULL)
    {
      /* 传输错误回调函数 */
      hdma->XferErrorCallback(hdma);
    }
  }
  return;
}

/**
  * @brief 注册回调函数
  * @param hdma: 指向包含指定 DMA 通道配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @param CallbackID: 用户回调函数标识符
  *                    为 HAL_DMA_CallbackIDTypeDef 枚举类型参数。
  * @param pCallback: 指向私有回调函数的指针，该函数以
  *                   DMA_HandleTypeDef 结构体指针作为参数。
  * @retval HAL 状态
  */                          
HAL_StatusTypeDef HAL_DMA_RegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID, void (* pCallback)( DMA_HandleTypeDef * _hdma))
{
  HAL_StatusTypeDef status = HAL_OK;
  
  /* 进程加锁 */
  __HAL_LOCK(hdma);
  
  if(HAL_DMA_STATE_READY == hdma->State)
  {
    switch (CallbackID)
    {
    case  HAL_DMA_XFER_CPLT_CB_ID:
      hdma->XferCpltCallback = pCallback;
      break;
      
    case  HAL_DMA_XFER_HALFCPLT_CB_ID:
      hdma->XferHalfCpltCallback = pCallback;
      break;         

    case  HAL_DMA_XFER_ERROR_CB_ID:
      hdma->XferErrorCallback = pCallback;
      break;         
      
    case  HAL_DMA_XFER_ABORT_CB_ID:
      hdma->XferAbortCallback = pCallback;
      break; 
      
    default:
      status = HAL_ERROR;
      break;                                                            
    }
  }
  else
  {
    status = HAL_ERROR;
  } 
  
  /* 释放锁 */
  __HAL_UNLOCK(hdma);
  
  return status;
}

/**
  * @brief 注销回调函数
  * @param hdma: 指向包含指定 DMA 通道配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @param CallbackID: 用户回调函数标识符
  *                    为 HAL_DMA_CallbackIDTypeDef 枚举类型参数。
  * @retval HAL 状态
  */              
HAL_StatusTypeDef HAL_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 进程加锁 */
  __HAL_LOCK(hdma);
  
  if(HAL_DMA_STATE_READY == hdma->State)
  {
    switch (CallbackID)
    {
    case  HAL_DMA_XFER_CPLT_CB_ID:
      hdma->XferCpltCallback = NULL;
      break;

    case  HAL_DMA_XFER_HALFCPLT_CB_ID:
      hdma->XferHalfCpltCallback = NULL;
      break;         

    case  HAL_DMA_XFER_ERROR_CB_ID:
      hdma->XferErrorCallback = NULL;
      break;         

    case  HAL_DMA_XFER_ABORT_CB_ID:
      hdma->XferAbortCallback = NULL;
      break; 

    case   HAL_DMA_XFER_ALL_CB_ID:
      hdma->XferCpltCallback = NULL;
      hdma->XferHalfCpltCallback = NULL;
      hdma->XferErrorCallback = NULL;
      hdma->XferAbortCallback = NULL;
      break; 

    default:
      status = HAL_ERROR;
      break;
    }
  }
  else
  {
    status = HAL_ERROR;
  } 
  
  /* 释放锁 */
  __HAL_UNLOCK(hdma);
  
  return status;
}
  
/**
  * @}
  */

/** @defgroup DMA_Exported_Functions_Group3 外设状态和错误处理函数
  *  @brief    外设状态和错误处理函数
  *
@verbatim
 ===============================================================================
            ##### 外设状态和错误处理函数 #####
 ===============================================================================  
    [..]
    本节提供的函数可用于：
      (+) 检查 DMA 状态
      (+) 获取错误代码

@endverbatim
  * @{
  */

/**
  * @brief  返回 DMA 句柄状态。
  * @param  hdma: 指向包含指定 DMA 通道配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_DMA_StateTypeDef HAL_DMA_GetState(DMA_HandleTypeDef *hdma)
{
  /* 返回 DMA 句柄状态 */
  return hdma->State;
}

/**
  * @brief  返回 DMA 错误代码。
  * @param  hdma : 指向包含指定 DMA 通道配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval DMA 错误代码
  */
uint32_t HAL_DMA_GetError(DMA_HandleTypeDef *hdma)
{
  return hdma->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup DMA_Private_Functions
  * @{
  */

/**
  * @brief  设置 DMA 传输参数。
  * @param  hdma:       指向包含指定 DMA 通道配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @param  SrcAddress: 源内存缓冲区地址
  * @param  DstAddress: 目的内存缓冲区地址
  * @param  DataLength: 从源传输到目的的数据长度
  * @retval HAL 状态
  */
static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
  /* 清除所有标志位 */
  hdma->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << hdma->ChannelIndex);

  /* 配置 DMA 通道数据长度 */
  hdma->Instance->CNDTR = DataLength;

  /* 内存到外设 */
  if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH)
  {
    /* 配置 DMA 通道目的地址 */
    hdma->Instance->CPAR = DstAddress;

    /* 配置 DMA 通道源地址 */
    hdma->Instance->CMAR = SrcAddress;
  }
  /* 外设到内存 */
  else
  {
    /* 配置 DMA 通道源地址 */
    hdma->Instance->CPAR = SrcAddress;

    /* 配置 DMA 通道目的地址 */
    hdma->Instance->CMAR = DstAddress;
  }
}

/**
  * @}
  */

#endif /* HAL_DMA_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */


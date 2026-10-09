/**
  ******************************************************************************
  * @file    stm32f1xx_hal_can.c
  * @author  MCD Application Team
  * @brief   本文件提供固件函数，用于管理控制器局域网（CAN）外设的以下功能：
  *           + 初始化与去初始化函数
  *           + I/O 操作函数
  *           + 外设控制函数
  *           + 外设状态与错误管理函数
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * 保留所有权利。
  *
  * 本软件根据可在该软件组件根目录下的 LICENSE 文件中找到的条款进行许可。
  * 若本软件未附带 LICENSE 文件，则按“原样（AS-IS）”提供。
  *
  ******************************************************************************
  @verbatim
  ==============================================================================
                                 ##### 用户注意事项 #####
  ==============================================================================
    [..]
      (#) 此 HAL CAN 驱动已废弃，其在 CAN 发送/接收（Tx/Rx）FIFO 管理方面存在一些限制。
          现已设计了带有全新 API 的另一版本 HAL CAN 驱动，以修复这些限制。

  ==============================================================================
                        ##### 如何使用本驱动 #####
  ==============================================================================
    [..]            
      (#) 使能 CAN 控制器接口时钟，对于 CAN1 使用 __HAL_RCC_CAN1_CLK_ENABLE()，
          对于 CAN2 使用 __HAL_RCC_CAN2_CLK_ENABLE()
      -@- 如果仅使用 CAN2，也必须使能 CAN1 时钟。
       
      (#) CAN 引脚配置
        (++) 使用以下函数使能 CAN GPIO 的时钟：
             __HAL_RCC_GPIOx_CLK_ENABLE();   
        (++) 使用以下函数 HAL_GPIO_Init() 连接并配置相关的 CAN 引脚；
              
      (#) 使用 HAL_CAN_Init() 函数初始化并配置 CAN。
                 
      (#) 使用 HAL_CAN_Transmit() 函数发送指定的 CAN 报文。

      (#) 或者使用 HAL_CAN_Transmit_IT() 函数发送指定的 CAN 报文。
           
      (#) 使用 HAL_CAN_Receive() 函数接收 CAN 报文。

      (#) 或者使用 HAL_CAN_Receive_IT() 函数接收 CAN 报文。

     *** 轮询模式 I/O 操作 ***
     =================================
     [..]    
       (+) 使用 HAL_CAN_Transmit() 启动 CAN 外设发送并等待此操作结束，
           在此阶段，用户可根据最终应用指定超时时间
       (+) 使用 HAL_CAN_Receive() 启动 CAN 外设接收并等待此操作结束，
           在此阶段，用户可根据最终应用指定超时时间
       
     *** 中断模式 I/O 操作 ***    
     ===================================
     [..]    
       (+) 使用 HAL_CAN_Transmit_IT() 启动 CAN 外设发送
       (+) 使用 HAL_CAN_Receive_IT() 启动 CAN 外设接收         
       (+) 在所使用的 CAN 中断服务函数中调用 HAL_CAN_IRQHandler()
       (+) 在 CAN 发送完成时将执行 HAL_CAN_TxCpltCallback() 函数，用户可通过
           自定义函数指针 HAL_CAN_TxCpltCallback 添加自己的代码
       (+) 发生 CAN 错误时将执行 HAL_CAN_ErrorCallback() 函数，用户可通过
           自定义函数指针 HAL_CAN_ErrorCallback 添加自己的代码
 
     *** CAN HAL 驱动宏列表 ***
     ============================================= 
     [..]
       以下是 CAN HAL 驱动中最常用的宏列表。
       
      (+) __HAL_CAN_ENABLE_IT: 使能指定的 CAN 中断
      (+) __HAL_CAN_DISABLE_IT: 禁用指定的 CAN 中断
      (+) __HAL_CAN_GET_IT_SOURCE: 检查指定的 CAN 中断源是否已使能或禁用
      (+) __HAL_CAN_CLEAR_FLAG: 清除 CAN 的挂起标志位
      (+) __HAL_CAN_GET_FLAG: 获取选定 CAN 标志位的状态
      
     [..] 
      (@) 更多实用的宏请参阅 CAN 传统（Legacy）HAL 驱动头文件
                
  @endverbatim
           
  ******************************************************************************
  */

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup CAN CAN
  * @brief CAN 驱动模块
  * @{
  */

#ifdef HAL_CAN_LEGACY_MODULE_ENABLED
  
#if defined(STM32F103x6) || defined(STM32F103xB) || defined(STM32F103xE) || \
    defined(STM32F103xG) || defined(STM32F105xC) || defined(STM32F107xC)
#ifdef HAL_CAN_MODULE_ENABLED
/* 在 stm32f1xx_hal_conf.h 文件中选择 HAL CAN 模块：
   (#) HAL_CAN_MODULE_ENABLED 用于修复 FIFO 限制的新 HAL CAN 驱动
   (#) HAL_CAN_LEGACY_MODULE_ENABLED 用于传统 HAL CAN 驱动 */
#error 'The HAL CAN driver cannot be used with its legacy, Please ensure to enable only one HAL CAN module at once in stm32f1xx_hal_conf.h file'
#endif /* HAL_CAN_MODULE_ENABLED */

#warning 'Legacy HAL CAN driver is enabled! It can be used with known limitations, refer to the release notes. However it is recommended to use rather the new HAL CAN driver'
  
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/** @defgroup CAN_Private_Constants CAN 私有常量
  * @{
  */
#define CAN_TIMEOUT_VALUE  10U
/**
  * @}
  */
/* 私有宏 --------------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 私有函数原型 --------------------------------------------------------------*/
/** @defgroup CAN_Private_Functions CAN 私有函数
  * @{
  */
static HAL_StatusTypeDef CAN_Receive_IT(CAN_HandleTypeDef* hcan, uint8_t FIFONumber);
static HAL_StatusTypeDef CAN_Transmit_IT(CAN_HandleTypeDef* hcan);
/**
  * @}
  */

/* 导出函数 --------------------------------------------------------*/
/** @defgroup CAN_Exported_Functions CAN 导出函数
  * @{
  */

/** @defgroup CAN_Exported_Functions_Group1 初始化和反初始化函数 
 *  @brief    初始化与配置函数 
 *
@verbatim    
  ==============================================================================
              ##### 初始化和反初始化函数 #####
  ==============================================================================
    [..]  本节提供的函数可实现：
      (+) 初始化并配置 CAN。
      (+) 反初始化 CAN。
         
@endverbatim
  * @{
  */
  
/**
  * @brief  根据 CAN_InitStruct 中指定的参数
  *         初始化 CAN 外设。
  * @param  hcan: 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_Init(CAN_HandleTypeDef* hcan)
{
  uint32_t status = CAN_INITSTATUS_FAILED;  /* 默认初始化状态 */
  uint32_t tickstart = 0U;
  uint32_t tmp_mcr = 0U;
  
  /* 检查 CAN 句柄 */
  if(hcan == NULL)
  {
     return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_CAN_ALL_INSTANCE(hcan->Instance));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.TTCM));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.ABOM));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.AWUM));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.NART));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.RFLM));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.TXFP));
  assert_param(IS_CAN_MODE(hcan->Init.Mode));
  assert_param(IS_CAN_SJW(hcan->Init.SJW));
  assert_param(IS_CAN_BS1(hcan->Init.BS1));
  assert_param(IS_CAN_BS2(hcan->Init.BS2));
  assert_param(IS_CAN_PRESCALER(hcan->Init.Prescaler));
  
  if(hcan->State == HAL_CAN_STATE_RESET)
  {
    /* 分配互斥锁资源并初始化 */
    hcan->Lock = HAL_UNLOCKED;
    /* 初始化底层硬件 */
    HAL_CAN_MspInit(hcan);
  }
  
  /* 初始化 CAN 状态 */
  hcan->State = HAL_CAN_STATE_BUSY;
  
  /* 退出睡眠模式 */
  CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_SLEEP);

  /* 请求进入初始化模式 */
  SET_BIT(hcan->Instance->MCR, CAN_MCR_INRQ);

  /* 获取超时时间 */
  tickstart = HAL_GetTick();   
  
  /* 等待应答确认 */
  while(HAL_IS_BIT_CLR(hcan->Instance->MSR, CAN_MSR_INAK))
  {
    if((HAL_GetTick()-tickstart) > CAN_TIMEOUT_VALUE)
    {
      hcan->State= HAL_CAN_STATE_TIMEOUT;
      /* 进程解锁 */
      __HAL_UNLOCK(hcan);
      return HAL_TIMEOUT;
    }
  }

  /* 检查应答 */
  if ((hcan->Instance->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)
  {
    /* 设置时间触发通信模式 */
    if (hcan->Init.TTCM == ENABLE)
    {
      SET_BIT(tmp_mcr, CAN_MCR_TTCM);
    }
    else
    {
      CLEAR_BIT(tmp_mcr, CAN_MCR_TTCM);
    }

    /* 设置自动离线管理 */
    if (hcan->Init.ABOM == ENABLE)
    {
      SET_BIT(tmp_mcr, CAN_MCR_ABOM);
    }
    else
    {
      CLEAR_BIT(tmp_mcr, CAN_MCR_ABOM);
    }

    /* 设置自动唤醒模式 */
    if (hcan->Init.AWUM == ENABLE)
    {
      SET_BIT(tmp_mcr, CAN_MCR_AWUM);
    }
    else
    {
      CLEAR_BIT(tmp_mcr, CAN_MCR_AWUM);
    }
    /* 设置禁止自动重传模式 */
    if (hcan->Init.NART == ENABLE)
    {
      SET_BIT(tmp_mcr, CAN_MCR_NART);
    }
    else
    {
      CLEAR_BIT(tmp_mcr, CAN_MCR_NART);
    }

    /* 设置接收 FIFO 锁定模式 */
    if (hcan->Init.RFLM == ENABLE)
    {
      SET_BIT(tmp_mcr, CAN_MCR_RFLM);
    }
    else
    {
      CLEAR_BIT(tmp_mcr, CAN_MCR_RFLM);
    }
    /* 设置发送 FIFO 优先级 */
    if (hcan->Init.TXFP == ENABLE)
    {
      SET_BIT(tmp_mcr, CAN_MCR_TXFP);
    }
    else
    {
      CLEAR_BIT(tmp_mcr, CAN_MCR_TXFP);
    }
    
    /* 更新 MCR 寄存器 */
    MODIFY_REG(hcan->Instance->MCR,
               CAN_MCR_TTCM |
               CAN_MCR_ABOM |
               CAN_MCR_AWUM |
               CAN_MCR_NART |
               CAN_MCR_RFLM |
               CAN_MCR_TXFP,
               tmp_mcr);
    
    /* 设置位时序寄存器 */
    WRITE_REG(hcan->Instance->BTR, (uint32_t)(hcan->Init.Mode           |
                                              hcan->Init.SJW            |
                                              hcan->Init.BS1            |
                                              hcan->Init.BS2            |
                                              (hcan->Init.Prescaler - 1U)));

    /* 请求退出初始化模式 */
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_INRQ);

    /* 获取超时时间 */
    tickstart = HAL_GetTick();   
   
    /* 等待应答 */
    while(HAL_IS_BIT_SET(hcan->Instance->MSR, CAN_MSR_INAK))
    {
      if((HAL_GetTick()-tickstart) > CAN_TIMEOUT_VALUE)
      {
        hcan->State= HAL_CAN_STATE_TIMEOUT;

        /* 进程解锁 */
        __HAL_UNLOCK(hcan);

        return HAL_TIMEOUT;
      }
    }

    /* 检查是否已应答 */
    if(HAL_IS_BIT_CLR(hcan->Instance->MSR, CAN_MSR_INAK))
    {
      status = CAN_INITSTATUS_SUCCESS;
    }
  }
 
  if(status == CAN_INITSTATUS_SUCCESS)
  {
    /* 将 CAN 错误码清零 (无错误) */
    hcan->ErrorCode = HAL_CAN_ERROR_NONE;
    
    /* 初始化 CAN 状态 */
    hcan->State = HAL_CAN_STATE_READY;
  
    /* 返回函数状态 */
    return HAL_OK;
  }
  else
  {
    /* 初始化 CAN 状态 */
    hcan->State = HAL_CAN_STATE_ERROR;

    /* 返回函数状态 */
    return HAL_ERROR;
  }
}

/**
  * @brief  根据 CAN_FilterInitStruct 中指定的参数配置 CAN 接收过滤器。
  * @param  hcan: 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构的指针。
  * @param  sFilterConfig: 指向包含过滤器配置信息的 CAN_FilterConfTypeDef 结构的指针。
  * @retval 无
  */
HAL_StatusTypeDef HAL_CAN_ConfigFilter(CAN_HandleTypeDef* hcan, CAN_FilterConfTypeDef* sFilterConfig)
{
  uint32_t filternbrbitpos = 0U;
  
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);

  /* 检查参数 */
  assert_param(IS_CAN_FILTER_NUMBER(sFilterConfig->FilterNumber));
  assert_param(IS_CAN_FILTER_MODE(sFilterConfig->FilterMode));
  assert_param(IS_CAN_FILTER_SCALE(sFilterConfig->FilterScale));
  assert_param(IS_CAN_FILTER_FIFO(sFilterConfig->FilterFIFOAssignment));
  assert_param(IS_FUNCTIONAL_STATE(sFilterConfig->FilterActivation));
  assert_param(IS_CAN_BANKNUMBER(sFilterConfig->BankNumber));
  
  filternbrbitpos = (1U) << sFilterConfig->FilterNumber;

  /* 过滤器初始化模式 */
  /* 选择从 CAN 过滤器的起始组 */
  MODIFY_REG(hcan->Instance->FMR                         ,
             CAN_FMR_CAN2SB                              ,
             CAN_FMR_FINIT                              |
             (uint32_t)(sFilterConfig->BankNumber << 8U)   );

  /* 禁用过滤器 */
  CLEAR_BIT(hcan->Instance->FA1R, filternbrbitpos);

  /* 过滤器位宽 */
  if (sFilterConfig->FilterScale == CAN_FILTERSCALE_16BIT)
  {
    /* 过滤器 16 位位宽 */
    CLEAR_BIT(hcan->Instance->FS1R, filternbrbitpos);

    /* 第一个 16 位标识符和第一个 16 位掩码 */
    /* 或第一个 16 位标识符和第二个 16 位标识符 */
    hcan->Instance->sFilterRegister[sFilterConfig->FilterNumber].FR1 = 
       ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdLow) << 16U) |
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdLow);

    /* 第二个 16 位标识符和第二个 16 位掩码 */
    /* 或第三个 16 位标识符和第四个 16 位标识符 */
    hcan->Instance->sFilterRegister[sFilterConfig->FilterNumber].FR2 = 
       ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdHigh) << 16U) |
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdHigh);
  }

  if (sFilterConfig->FilterScale == CAN_FILTERSCALE_32BIT)
  {
    /* 过滤器采用 32 位尺度 */
    SET_BIT(hcan->Instance->FS1R, filternbrbitpos);
    /* 32 位标识符或第一个 32 位标识符 */
    hcan->Instance->sFilterRegister[sFilterConfig->FilterNumber].FR1 = 
       ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdHigh) << 16U) |
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdLow);
    /* 32 位掩码或第二个 32 位标识符 */
    hcan->Instance->sFilterRegister[sFilterConfig->FilterNumber].FR2 = 
       ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdHigh) << 16U) |
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdLow);
  }

  /* 过滤器模式 */
  if (sFilterConfig->FilterMode == CAN_FILTERMODE_IDMASK)
  {
    /* 过滤器采用标识符/掩码模式 */
    CLEAR_BIT(hcan->Instance->FM1R, filternbrbitpos);
  }
  else /* CAN_FilterInitStruct->CAN_FilterMode == CAN_FilterMode_IdList */
  {
    /* 过滤器采用标识符列表模式 */
    SET_BIT(hcan->Instance->FM1R, filternbrbitpos);
  }

  /* 过滤器关联 FIFO 分配 */
  if (sFilterConfig->FilterFIFOAssignment == CAN_FILTER_FIFO0)
  {
    /* 为过滤器分配 FIFO 0 */
    CLEAR_BIT(hcan->Instance->FFA1R, filternbrbitpos);
  }
  else
  {
    /* 为过滤器分配 FIFO 1 */
    SET_BIT(hcan->Instance->FFA1R, filternbrbitpos);
  }
  
  /* 过滤器使能/激活 */
  if (sFilterConfig->FilterActivation == ENABLE)
  {
    SET_BIT(hcan->Instance->FA1R, filternbrbitpos);
  }

  /* 退出过滤器初始化模式 */
  CLEAR_BIT(hcan->Instance->FMR, ((uint32_t)CAN_FMR_FINIT));
  
  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  将 CANx 外设寄存器去初始化恢复为默认复位值。
  * @param  hcan: 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_DeInit(CAN_HandleTypeDef* hcan)
{
  /* 检查 CAN 句柄 */
  if(hcan == NULL)
  {
     return HAL_ERROR;
  }
  
  /* 检查参数 */
  assert_param(IS_CAN_ALL_INSTANCE(hcan->Instance));
  
  /* 更改 CAN 状态 */
  hcan->State = HAL_CAN_STATE_BUSY;
  
  /* 反初始化底层硬件 */
  HAL_CAN_MspDeInit(hcan);
  
  /* 更改 CAN 状态 */
  hcan->State = HAL_CAN_STATE_RESET;

  /* 释放锁 */
  __HAL_UNLOCK(hcan);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  初始化 CAN MSP。
  * @param  hcan: 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(hcan);
  /* 注意：不应修改此函数，当需要该回调函数时，
            可以在用户文件中实现 HAL_CAN_MspInit
   */ 
}

/**
  * @brief  反初始化 CAN MSP。
  * @param  hcan: 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(hcan);
  /* 注意：不应修改此函数，当需要该回调函数时，
            可以在用户文件中实现 HAL_CAN_MspDeInit
   */ 
}

/**
  * @}
  */

/** @defgroup CAN_Exported_Functions_Group2 输入与输出操作函数
  *  @brief    I/O 操作函数 
  *
@verbatim   
  ==============================================================================
                      ##### IO 操作函数 #####
  ==============================================================================
    [..]  本节提供的函数可实现：
      (+) 发送 CAN 报文帧。
      (+) 接收 CAN 报文帧。
      (+) 使 CAN 外设进入睡眠模式。
      (+) 将 CAN 外设从睡眠模式唤醒。
               
@endverbatim
  * @{
  */

/**
  * @brief  启动并发送一个 CAN 报文帧。
  * @param  hcan: 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @param  Timeout: 指定超时时间值
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_Transmit(CAN_HandleTypeDef* hcan, uint32_t Timeout)
{
  uint32_t transmitmailbox = CAN_TXSTATUS_NOMAILBOX;
  uint32_t tickstart = 0U;

  /* 检查参数 */
  assert_param(IS_CAN_IDTYPE(hcan->pTxMsg->IDE));
  assert_param(IS_CAN_RTR(hcan->pTxMsg->RTR));
  assert_param(IS_CAN_DLC(hcan->pTxMsg->DLC));

  if(((hcan->Instance->TSR&CAN_TSR_TME0) == CAN_TSR_TME0) || \
     ((hcan->Instance->TSR&CAN_TSR_TME1) == CAN_TSR_TME1) || \
     ((hcan->Instance->TSR&CAN_TSR_TME2) == CAN_TSR_TME2))
  {
    /* 进程加锁 */
    __HAL_LOCK(hcan);

    /* 更改 CAN 状态 */
    switch(hcan->State)
    {
      case(HAL_CAN_STATE_BUSY_RX0):
          hcan->State = HAL_CAN_STATE_BUSY_TX_RX0;
          break;
      case(HAL_CAN_STATE_BUSY_RX1):
          hcan->State = HAL_CAN_STATE_BUSY_TX_RX1;
          break;
      case(HAL_CAN_STATE_BUSY_RX0_RX1):
          hcan->State = HAL_CAN_STATE_BUSY_TX_RX0_RX1;
          break;
      default: /* HAL_CAN_STATE_READY */
          hcan->State = HAL_CAN_STATE_BUSY_TX;
          break;
    }

    /* 选择一个空闲的发送邮箱 */
    if (HAL_IS_BIT_SET(hcan->Instance->TSR, CAN_TSR_TME0))
    {
      transmitmailbox = CAN_TXMAILBOX_0;
    }
    else if (HAL_IS_BIT_SET(hcan->Instance->TSR, CAN_TSR_TME1))
    {
      transmitmailbox = CAN_TXMAILBOX_1;
    }
    else
    {
      transmitmailbox = CAN_TXMAILBOX_2;
    }

    /* 设置标识符 (ID) */
    hcan->Instance->sTxMailBox[transmitmailbox].TIR &= CAN_TI0R_TXRQ;
    if (hcan->pTxMsg->IDE == CAN_ID_STD)
    {
      assert_param(IS_CAN_STDID(hcan->pTxMsg->StdId));  
      hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((hcan->pTxMsg->StdId << CAN_TI0R_STID_Pos) |
                                                           hcan->pTxMsg->RTR);
    }
    else
    {
      assert_param(IS_CAN_EXTID(hcan->pTxMsg->ExtId));
      hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((hcan->pTxMsg->ExtId << CAN_TI0R_EXID_Pos) |
                                                           hcan->pTxMsg->IDE |
                                                           hcan->pTxMsg->RTR);
    }

    /* 设置数据长度代码 (DLC) */
    hcan->pTxMsg->DLC &= (uint8_t)0x0000000F;
    hcan->Instance->sTxMailBox[transmitmailbox].TDTR &= 0xFFFFFFF0U;
    hcan->Instance->sTxMailBox[transmitmailbox].TDTR |= hcan->pTxMsg->DLC;

    /* 设置数据段 */
    WRITE_REG(hcan->Instance->sTxMailBox[transmitmailbox].TDLR, ((uint32_t)hcan->pTxMsg->Data[3] << CAN_TDL0R_DATA3_Pos) | 
                                                                ((uint32_t)hcan->pTxMsg->Data[2] << CAN_TDL0R_DATA2_Pos) |
                                                                ((uint32_t)hcan->pTxMsg->Data[1] << CAN_TDL0R_DATA1_Pos) | 
                                                                ((uint32_t)hcan->pTxMsg->Data[0] << CAN_TDL0R_DATA0_Pos));
    WRITE_REG(hcan->Instance->sTxMailBox[transmitmailbox].TDHR, ((uint32_t)hcan->pTxMsg->Data[7] << CAN_TDL0R_DATA3_Pos) | 
                                                                ((uint32_t)hcan->pTxMsg->Data[6] << CAN_TDL0R_DATA2_Pos) |
                                                                ((uint32_t)hcan->pTxMsg->Data[5] << CAN_TDL0R_DATA1_Pos) |
                                                                ((uint32_t)hcan->pTxMsg->Data[4] << CAN_TDL0R_DATA0_Pos));
    /* 请求发送 */
    SET_BIT(hcan->Instance->sTxMailBox[transmitmailbox].TIR, CAN_TI0R_TXRQ);

    /* 获取嘀嗒计数值 (tick) */
    tickstart = HAL_GetTick();

    /* 检查发送完成标志 */
    while(!(__HAL_CAN_TRANSMIT_STATUS(hcan, transmitmailbox)))
    {
      /* 检查是否超时 */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0U) || ((HAL_GetTick()-tickstart) > Timeout))
        {
          hcan->State = HAL_CAN_STATE_TIMEOUT;

          /* 取消发送 */
          __HAL_CAN_CANCEL_TRANSMIT(hcan, transmitmailbox);

          /* 进程解锁 */
          __HAL_UNLOCK(hcan);
          return HAL_TIMEOUT;
        }
      }
    }
    /* 更改 CAN 状态 */
    switch(hcan->State)
    {
      case(HAL_CAN_STATE_BUSY_TX_RX0):
          hcan->State = HAL_CAN_STATE_BUSY_RX0;
          break;
      case(HAL_CAN_STATE_BUSY_TX_RX1):
          hcan->State = HAL_CAN_STATE_BUSY_RX1;
          break;
      case(HAL_CAN_STATE_BUSY_TX_RX0_RX1):
          hcan->State = HAL_CAN_STATE_BUSY_RX0_RX1;
          break;
      default: /* HAL_CAN_STATE_BUSY_TX */
          hcan->State = HAL_CAN_STATE_READY;
          break;
    }

    /* 进程解锁 */
    __HAL_UNLOCK(hcan);

    /* 返回函数状态 */
    return HAL_OK;
  }
  else
  {
    /* 更改 CAN 状态 */
    hcan->State = HAL_CAN_STATE_ERROR;

    /* 返回函数状态 */
    return HAL_ERROR;
  }
}

/**
  * @brief  启动并发送 CAN 帧报文。
  * @param  hcan: 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_Transmit_IT(CAN_HandleTypeDef* hcan)
{
  uint32_t transmitmailbox = CAN_TXSTATUS_NOMAILBOX;

  /* 检查参数 */
  assert_param(IS_CAN_IDTYPE(hcan->pTxMsg->IDE));
  assert_param(IS_CAN_RTR(hcan->pTxMsg->RTR));
  assert_param(IS_CAN_DLC(hcan->pTxMsg->DLC));
  
  if(((hcan->Instance->TSR&CAN_TSR_TME0) == CAN_TSR_TME0) || \
     ((hcan->Instance->TSR&CAN_TSR_TME1) == CAN_TSR_TME1) || \
     ((hcan->Instance->TSR&CAN_TSR_TME2) == CAN_TSR_TME2))
  {
    /* 进程上锁 */
    __HAL_LOCK(hcan);
    
    /* 选择一个空闲的发送邮箱 */
    if(HAL_IS_BIT_SET(hcan->Instance->TSR, CAN_TSR_TME0))
    {
      transmitmailbox = CAN_TXMAILBOX_0;
    }
    else if(HAL_IS_BIT_SET(hcan->Instance->TSR, CAN_TSR_TME1))
    {
      transmitmailbox = CAN_TXMAILBOX_1;
    }
    else
    {
      transmitmailbox = CAN_TXMAILBOX_2;
    }

    /* 设置标识符 (ID) */
    hcan->Instance->sTxMailBox[transmitmailbox].TIR &= CAN_TI0R_TXRQ;
    if(hcan->pTxMsg->IDE == CAN_ID_STD)
    {
      assert_param(IS_CAN_STDID(hcan->pTxMsg->StdId));
      hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((hcan->pTxMsg->StdId << CAN_TI0R_STID_Pos) | \
                                                           hcan->pTxMsg->RTR);
    }
    else
    {
      assert_param(IS_CAN_EXTID(hcan->pTxMsg->ExtId));
      hcan->Instance->sTxMailBox[transmitmailbox].TIR |= ((hcan->pTxMsg->ExtId << CAN_TI0R_EXID_Pos) | \
                                                           hcan->pTxMsg->IDE |
                                                           hcan->pTxMsg->RTR);
    }

    /* 设置数据长度代码 (DLC) */
    hcan->pTxMsg->DLC &= (uint8_t)0x0000000FU;
    hcan->Instance->sTxMailBox[transmitmailbox].TDTR &= 0xFFFFFFF0U;
    hcan->Instance->sTxMailBox[transmitmailbox].TDTR |= hcan->pTxMsg->DLC;

    /* 设置数据段 */
    WRITE_REG(hcan->Instance->sTxMailBox[transmitmailbox].TDLR, ((uint32_t)hcan->pTxMsg->Data[3U] << CAN_TDL0R_DATA3_Pos) |
                                                                ((uint32_t)hcan->pTxMsg->Data[2U] << CAN_TDL0R_DATA2_Pos) |
                                                                ((uint32_t)hcan->pTxMsg->Data[1U] << CAN_TDL0R_DATA1_Pos) |
                                                                ((uint32_t)hcan->pTxMsg->Data[0U] << CAN_TDL0R_DATA0_Pos));
    WRITE_REG(hcan->Instance->sTxMailBox[transmitmailbox].TDHR, ((uint32_t)hcan->pTxMsg->Data[7U] << CAN_TDL0R_DATA3_Pos) |
                                                                ((uint32_t)hcan->pTxMsg->Data[6U] << CAN_TDL0R_DATA2_Pos) |
                                                                ((uint32_t)hcan->pTxMsg->Data[5U] << CAN_TDL0R_DATA1_Pos) |
                                                                ((uint32_t)hcan->pTxMsg->Data[4U] << CAN_TDL0R_DATA0_Pos));

    /* 更改 CAN 状态 */
    switch(hcan->State)
    {
      case(HAL_CAN_STATE_BUSY_RX0):
          hcan->State = HAL_CAN_STATE_BUSY_TX_RX0;
          break;
      case(HAL_CAN_STATE_BUSY_RX1):
          hcan->State = HAL_CAN_STATE_BUSY_TX_RX1;
          break;
      case(HAL_CAN_STATE_BUSY_RX0_RX1):
          hcan->State = HAL_CAN_STATE_BUSY_TX_RX0_RX1;
          break;
      default: /* HAL_CAN_STATE_READY */
          hcan->State = HAL_CAN_STATE_BUSY_TX;
          break;
    }

    /* 将 CAN 错误代码设置为无错误 */
    hcan->ErrorCode = HAL_CAN_ERROR_NONE;

    /* 进程解锁 */
    __HAL_UNLOCK(hcan);

    /* 请求发送 */
    hcan->Instance->sTxMailBox[transmitmailbox].TIR |= CAN_TI0R_TXRQ;

    /* 使能中断：*/
    /*  - 使能错误警告中断 */
    /*  - 使能被动错误中断 */
    /*  - 使能总线关闭中断 */
    /*  - 使能上次错误代码中断 */
    /*  - 使能错误中断 */
    /*  - 使能发送邮箱空中断 */
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_EWG |
                              CAN_IT_EPV |
                              CAN_IT_BOF |
                              CAN_IT_LEC |
                              CAN_IT_ERR |
                              CAN_IT_TME  );
  }
  else
  {
    /* 改变 CAN 状态 */
    hcan->State = HAL_CAN_STATE_ERROR;

    /* 返回函数状态 */
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  接收一个正确的 CAN 帧。
  * @param  hcan: 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @param  FIFONumber: FIFO 编号值
  * @param  Timeout: 指定超时时间值
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_Receive(CAN_HandleTypeDef* hcan, uint8_t FIFONumber, uint32_t Timeout)
{
  uint32_t tickstart = 0U;
  CanRxMsgTypeDef* pRxMsg = NULL;

  /* 检查参数 */
  assert_param(IS_CAN_FIFO(FIFONumber));

  /* 检查 CAN 状态是否未处于 RX FIFO0 繁忙状态 */
  if ((FIFONumber == CAN_FIFO0) && ((hcan->State == HAL_CAN_STATE_BUSY_RX0) ||         \
                                    (hcan->State == HAL_CAN_STATE_BUSY_TX_RX0) ||      \
                                    (hcan->State == HAL_CAN_STATE_BUSY_RX0_RX1) ||     \
                                    (hcan->State == HAL_CAN_STATE_BUSY_TX_RX0_RX1)))
  {
    return HAL_BUSY;
  }

  /* 检查 CAN 状态对于 RX FIFO1 是否非忙 */
  if ((FIFONumber == CAN_FIFO1) && ((hcan->State == HAL_CAN_STATE_BUSY_RX1) ||         \
                                    (hcan->State == HAL_CAN_STATE_BUSY_TX_RX1) ||      \
                                    (hcan->State == HAL_CAN_STATE_BUSY_RX0_RX1) ||     \
                                    (hcan->State == HAL_CAN_STATE_BUSY_TX_RX0_RX1)))
  {
    return HAL_BUSY;
  }

  /* 进程已锁定 */
  __HAL_LOCK(hcan);

  /* 改变 CAN 状态 */
  if (FIFONumber == CAN_FIFO0)
  {
    switch(hcan->State)
    {
      case(HAL_CAN_STATE_BUSY_TX):
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0;
        break;
      case(HAL_CAN_STATE_BUSY_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_RX0_RX1;
        break;
      case(HAL_CAN_STATE_BUSY_TX_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0_RX1;
        break;
      default: /* HAL_CAN_STATE_READY */
        hcan->State = HAL_CAN_STATE_BUSY_RX0;
        break;
    }
  }
  else /* FIFONumber == CAN_FIFO1 */
  {
    switch(hcan->State)
    {
      case(HAL_CAN_STATE_BUSY_TX):
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX1;
        break;
      case(HAL_CAN_STATE_BUSY_RX0):
        hcan->State = HAL_CAN_STATE_BUSY_RX0_RX1;
        break;
      case(HAL_CAN_STATE_BUSY_TX_RX0):
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0_RX1;
        break;
      default: /* HAL_CAN_STATE_READY */
        hcan->State = HAL_CAN_STATE_BUSY_RX1;
        break;
    }
  }
  /* 获取嘀嗒计数值 */ 
  tickstart = HAL_GetTick();
  
  /* 检查待处理消息 */
  while(__HAL_CAN_MSG_PENDING(hcan, FIFONumber) == 0U)
  {
    /* 检查是否超时 */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) || ((HAL_GetTick()-tickstart) > Timeout))
      {
        hcan->State = HAL_CAN_STATE_TIMEOUT;
        /* 进程已解锁 */
        __HAL_UNLOCK(hcan);
        return HAL_TIMEOUT;
      }
    }
  }

  /* 设置 RxMsg 指针 */
  if(FIFONumber == CAN_FIFO0)
  {
    pRxMsg = hcan->pRxMsg;
  }
  else /* FIFONumber == CAN_FIFO1 */
  {
    pRxMsg = hcan->pRx1Msg;
  }

  /* 获取标识符（Id） */
  pRxMsg->IDE = (uint8_t)CAN_ID_EXT & hcan->Instance->sFIFOMailBox[FIFONumber].RIR;
  if (pRxMsg->IDE == CAN_ID_STD)
  {
    pRxMsg->StdId = 0x000007FFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 21U);
  }
  else
  {
    pRxMsg->ExtId = 0x1FFFFFFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 3U);
  }
  
  pRxMsg->RTR = (uint8_t)CAN_RTR_REMOTE & hcan->Instance->sFIFOMailBox[FIFONumber].RIR;
  /* 获取数据长度代码（DLC） */
  pRxMsg->DLC = (uint8_t)0x0FU & hcan->Instance->sFIFOMailBox[FIFONumber].RDTR;
  /* 获取过滤器匹配索引（FMI） */
  pRxMsg->FMI = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDTR >> 8U);
  /* 获取 FIFO 编号 */
  pRxMsg->FIFONumber = FIFONumber;
  /* 获取数据段 */
  pRxMsg->Data[0] = (uint8_t)0xFFU & hcan->Instance->sFIFOMailBox[FIFONumber].RDLR;
  pRxMsg->Data[1] = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 8U);
  pRxMsg->Data[2] = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 16U);
  pRxMsg->Data[3] = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 24U);
  pRxMsg->Data[4] = (uint8_t)0xFFU & hcan->Instance->sFIFOMailBox[FIFONumber].RDHR;
  pRxMsg->Data[5] = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 8U);
  pRxMsg->Data[6] = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 16U);
  pRxMsg->Data[7] = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 24U);
  
  /* 释放 FIFO */
  if(FIFONumber == CAN_FIFO0)
  {
    /* 释放 FIFO0 */
    __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO0);
  }
  else /* FIFONumber == CAN_FIFO1 */
  {
    /* 释放 FIFO1 */
    __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO1);
  }
  
  /* 改变 CAN 状态 */
  if (FIFONumber == CAN_FIFO0)
  {
    switch(hcan->State)
    {
      case(HAL_CAN_STATE_BUSY_TX_RX0):
        hcan->State = HAL_CAN_STATE_BUSY_TX;
        break;
      case(HAL_CAN_STATE_BUSY_RX0_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_RX1;
        break;
      case(HAL_CAN_STATE_BUSY_TX_RX0_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX1;
        break;
      default: /* HAL_CAN_STATE_BUSY_RX0 */
        hcan->State = HAL_CAN_STATE_READY;
        break;
    }
  }
  else /* FIFONumber == CAN_FIFO1 */
  {
    switch(hcan->State)
    {
      case(HAL_CAN_STATE_BUSY_TX_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_TX;
        break;
      case(HAL_CAN_STATE_BUSY_RX0_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_RX0;
        break;
      case(HAL_CAN_STATE_BUSY_TX_RX0_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0;
        break;
      default: /* HAL_CAN_STATE_BUSY_RX1 */
        hcan->State = HAL_CAN_STATE_READY;
        break;
    }
  }

  /* 进程解锁 */
  __HAL_UNLOCK(hcan);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  接收一个正确的 CAN 报文帧。
  * @param  hcan: 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构的指针。
  * @param  FIFONumber: 指定 FIFO 编号    
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_Receive_IT(CAN_HandleTypeDef* hcan, uint8_t FIFONumber)
{
  /* 检查参数 */
  assert_param(IS_CAN_FIFO(FIFONumber));
  
  /* 检查 CAN 状态对于 RX FIFO0 是否不处于繁忙状态 */
  if((FIFONumber == CAN_FIFO0) && ((hcan->State == HAL_CAN_STATE_BUSY_RX0) ||         \
                                   (hcan->State == HAL_CAN_STATE_BUSY_TX_RX0) ||      \
                                   (hcan->State == HAL_CAN_STATE_BUSY_RX0_RX1) ||     \
                                   (hcan->State == HAL_CAN_STATE_BUSY_TX_RX0_RX1)))
  {
    return HAL_BUSY;
  }

  /* 检查 CAN 的 RX FIFO1 状态是否不为繁忙 */
  if((FIFONumber == CAN_FIFO1) && ((hcan->State == HAL_CAN_STATE_BUSY_RX1) ||         \
                                   (hcan->State == HAL_CAN_STATE_BUSY_TX_RX1) ||      \
                                   (hcan->State == HAL_CAN_STATE_BUSY_RX0_RX1) ||     \
                                   (hcan->State == HAL_CAN_STATE_BUSY_TX_RX0_RX1)))
  {
    return HAL_BUSY;
  }

  /* 进程锁定 */
  __HAL_LOCK(hcan);

  /* 改变 CAN 状态 */
  if(FIFONumber == CAN_FIFO0)
  {
    switch(hcan->State)
    {
      case(HAL_CAN_STATE_BUSY_TX):
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0;
        break;
      case(HAL_CAN_STATE_BUSY_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_RX0_RX1;
        break;
      case(HAL_CAN_STATE_BUSY_TX_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0_RX1;
        break;
      default: /* HAL_CAN_STATE_READY */
        hcan->State = HAL_CAN_STATE_BUSY_RX0;
        break;
    }
  }
  else /* FIFONumber == CAN_FIFO1 */
  {
    switch(hcan->State)
    {
      case(HAL_CAN_STATE_BUSY_TX):
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX1;
        break;
      case(HAL_CAN_STATE_BUSY_RX0):
        hcan->State = HAL_CAN_STATE_BUSY_RX0_RX1;
        break;
      case(HAL_CAN_STATE_BUSY_TX_RX0):
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0_RX1;
        break;
      default: /* HAL_CAN_STATE_READY */
        hcan->State = HAL_CAN_STATE_BUSY_RX1;
        break;
    }
  }
  /* 将 CAN 错误码设置为无错误 (none) */
  hcan->ErrorCode = HAL_CAN_ERROR_NONE;


  /* 使能中断： */
  /*  - 使能错误警告中断 */
  /*  - 使能被动错误中断 */
  /*  - 使能总线关闭中断 */
  /*  - 使能上次错误码中断 */
  /*  - 使能错误中断 */
  /*  - 使能发送邮箱空中断 */
  __HAL_CAN_ENABLE_IT(hcan, CAN_IT_EWG |
                            CAN_IT_EPV |
                            CAN_IT_BOF |
                            CAN_IT_LEC |
                            CAN_IT_ERR |
                            CAN_IT_TME  );
  
   /* 进程解锁 */
   __HAL_UNLOCK(hcan);

  if(FIFONumber == CAN_FIFO0)
  {
    /* 使能 FIFO 0 溢出和消息挂起中断 */
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_FOV0 | CAN_IT_FMP0);
  }
  else
  {
    /* 使能 FIFO 1 溢出和消息挂起中断 */
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_FOV1 | CAN_IT_FMP1);
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  进入睡眠（低功耗）模式。
  * @param  hcan: 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态。
  */
HAL_StatusTypeDef HAL_CAN_Sleep(CAN_HandleTypeDef* hcan)
{
  uint32_t tickstart = 0U;
   
  /* 进程已锁定 */
  __HAL_LOCK(hcan);
  
  /* 改变 CAN 状态 */
  hcan->State = HAL_CAN_STATE_BUSY; 
    
  /* 请求进入睡眠模式 */
  MODIFY_REG(hcan->Instance->MCR,
             CAN_MCR_INRQ       ,
             CAN_MCR_SLEEP       );

  /* 睡眠模式状态 */
  if (HAL_IS_BIT_CLR(hcan->Instance->MSR, CAN_MSR_SLAK) ||
      HAL_IS_BIT_SET(hcan->Instance->MSR, CAN_MSR_INAK)   )
  {
    /* 进程已解锁 */
    __HAL_UNLOCK(hcan);

    /* 返回函数状态 */
    return HAL_ERROR;
  }
  
  /* 获取嘀嗒计数值 */
  tickstart = HAL_GetTick();
  
  /* 等待应答 */
  while (HAL_IS_BIT_CLR(hcan->Instance->MSR, CAN_MSR_SLAK) ||
         HAL_IS_BIT_SET(hcan->Instance->MSR, CAN_MSR_INAK))
  {
    if((HAL_GetTick()-tickstart) > CAN_TIMEOUT_VALUE)
    {
      hcan->State = HAL_CAN_STATE_TIMEOUT;

      /* 进程已解锁 */
      __HAL_UNLOCK(hcan);

      return HAL_TIMEOUT;
    }
  }
  
  /* 改变 CAN 状态 */
  hcan->State = HAL_CAN_STATE_READY;
  
  /* 进程已解锁 */
  __HAL_UNLOCK(hcan);
  
  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  将 CAN 外设从睡眠模式唤醒，唤醒后 CAN 外设
  *         进入正常模式。
  * @param  hcan: 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态。
  */
HAL_StatusTypeDef HAL_CAN_WakeUp(CAN_HandleTypeDef* hcan)
{
  uint32_t tickstart = 0U;
    
  /* 进程加锁 */
  __HAL_LOCK(hcan);
  
  /* 改变 CAN 状态 */
  hcan->State = HAL_CAN_STATE_BUSY;  
 
  /* 唤醒请求 */
  CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_SLEEP);
    
  /* 获取超时时间 */
  tickstart = HAL_GetTick();   
  
  /* 睡眠模式状态 */
  while((hcan->Instance->MSR & CAN_MSR_SLAK) == CAN_MSR_SLAK)
  {
    if((HAL_GetTick()-tickstart) > CAN_TIMEOUT_VALUE)
    {
      hcan->State= HAL_CAN_STATE_TIMEOUT;
      /* 进程解锁 */
      __HAL_UNLOCK(hcan);
      return HAL_TIMEOUT;
    }
  }
  if(HAL_IS_BIT_SET(hcan->Instance->MSR, CAN_MSR_SLAK))
  {
    /* 进程解锁 */
    __HAL_UNLOCK(hcan);

    /* 返回函数状态 */
    return HAL_ERROR;
  }
  
  /* 改变 CAN 状态 */
  hcan->State = HAL_CAN_STATE_READY; 
  
  /* 进程解锁 */
  __HAL_UNLOCK(hcan);
  
  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  处理 CAN 中断请求
  * @param  hcan: 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
void HAL_CAN_IRQHandler(CAN_HandleTypeDef* hcan)
{
  uint32_t tmp1 = 0U, tmp2 = 0U, tmp3 = 0U;
  uint32_t errorcode = HAL_CAN_ERROR_NONE;

  /* 检查 FIFO0 的溢出标志 */
  tmp1 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_FOV0);
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_FOV0);
  if((tmp1 != 0U) && tmp2)
  {
    /* 将 CAN 错误码设置为 FOV0 错误 */
    errorcode |= HAL_CAN_ERROR_FOV0;

    /* 清除 FIFO0 溢出标志 (Overrun Flag) */
    __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV0);
  }

  /* 检查 FIFO1 溢出标志 (Overrun Flag) */
  tmp1 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_FOV1);
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_FOV1);
  if((tmp1 != 0U) && tmp2)
  {
    /* 将 CAN 错误码设置为 FOV1 错误 */
    errorcode |= HAL_CAN_ERROR_FOV1;

    /* 清除 FIFO1 溢出标志 (Overrun Flag) */
    __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV1);
  }

  /* 检查发送结束标志 */
  if(__HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_TME))
  {
    /* 检查发送请求完成状态 */
    tmp1 = __HAL_CAN_TRANSMIT_STATUS(hcan, CAN_TXMAILBOX_0);
    tmp2 = __HAL_CAN_TRANSMIT_STATUS(hcan, CAN_TXMAILBOX_1);
    tmp3 = __HAL_CAN_TRANSMIT_STATUS(hcan, CAN_TXMAILBOX_2);
    if(tmp1 || tmp2 || tmp3)  
    {
      tmp1 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_TXOK0);
      tmp2 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_TXOK1);
      tmp3 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_TXOK2);
      /* 检查发送是否成功 */
      if((tmp1) || (tmp2) || (tmp3))
      {
        /* 调用发送函数 */
        CAN_Transmit_IT(hcan);
      }
      else /* 发送失败 */
      {
        /* 将 CAN 错误码设置为 TXFAIL 错误 */
        errorcode |= HAL_CAN_ERROR_TXFAIL;
      }

      /* 清除发送状态标志 (RQCPx 和 TXOKx) */
      SET_BIT(hcan->Instance->TSR, CAN_TSR_RQCP0  | CAN_TSR_RQCP1  | CAN_TSR_RQCP2 | \
                                   CAN_FLAG_TXOK0 | CAN_FLAG_TXOK1 | CAN_FLAG_TXOK2);
    }
  }
  
  tmp1 = __HAL_CAN_MSG_PENDING(hcan, CAN_FIFO0);
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_FMP0);
  /* 检查 FIFO0 接收结束标志 */
  if((tmp1 != 0U) && tmp2)
  {
    /* 调用接收函数 */
    CAN_Receive_IT(hcan, CAN_FIFO0);
  }
  
  tmp1 = __HAL_CAN_MSG_PENDING(hcan, CAN_FIFO1);
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_FMP1);
  /* 检查 FIFO1 接收结束标志 */
  if((tmp1 != 0U) && tmp2)
  {
    /* 调用接收函数 */
    CAN_Receive_IT(hcan, CAN_FIFO1);
  }

  /* 在句柄中设置错误代码 */
  hcan->ErrorCode |= errorcode;

  tmp1 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_EWG);
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_EWG);
  tmp3 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_ERR);
  /* 检查错误警告标志 (Error Warning Flag) */
  if(tmp1 && tmp2 && tmp3)
  {
    /* 将 CAN 错误代码设置为 EWG 错误 */
    hcan->ErrorCode |= HAL_CAN_ERROR_EWG;
    /* 错误警告标志为只读，无需清除 */
  }
  
  tmp1 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_EPV);
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_EPV);
  tmp3 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_ERR); 
  /* 检查错误被动标志 (Error Passive Flag) */
  if(tmp1 && tmp2 && tmp3)
  {
    /* 将 CAN 错误代码设置为 EPV 错误 */
    hcan->ErrorCode |= HAL_CAN_ERROR_EPV;
    /* 错误被动标志为只读，无需清除 */ 
  }
  
  tmp1 = __HAL_CAN_GET_FLAG(hcan, CAN_FLAG_BOF);
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_BOF);
  tmp3 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_ERR);  
  /* 检查总线关闭标志 (Bus-Off Flag) */
  if(tmp1 && tmp2 && tmp3)
  {
    /* 将 CAN 错误代码设置为 BOF 错误 */
    hcan->ErrorCode |= HAL_CAN_ERROR_BOF;
    /* 总线关闭标志为只读，无需清除 */
  }
  
  tmp1 = HAL_IS_BIT_CLR(hcan->Instance->ESR, CAN_ESR_LEC);
  tmp2 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_LEC);
  tmp3 = __HAL_CAN_GET_IT_SOURCE(hcan, CAN_IT_ERR);
  /* 检查最后一次错误代码标志 (Last error code Flag) */
  if((!tmp1) && tmp2 && tmp3)
  {
    tmp1 = (hcan->Instance->ESR & CAN_ESR_LEC);
    switch(tmp1)
    {
      case(CAN_ESR_LEC_0):
          /* 将 CAN 错误代码设置为 STF 错误 */
          hcan->ErrorCode |= HAL_CAN_ERROR_STF;
          break;
      case(CAN_ESR_LEC_1):
          /* 将 CAN 错误代码设置为 FOR 错误 */
          hcan->ErrorCode |= HAL_CAN_ERROR_FOR;
          break;
      case(CAN_ESR_LEC_1 | CAN_ESR_LEC_0):
          /* 将 CAN 错误代码设置为 ACK 错误 */
          hcan->ErrorCode |= HAL_CAN_ERROR_ACK;
          break;
      case(CAN_ESR_LEC_2):
          /* 将 CAN 错误代码设置为 BR 错误 */
          hcan->ErrorCode |= HAL_CAN_ERROR_BR;
          break;
      case(CAN_ESR_LEC_2 | CAN_ESR_LEC_0):
          /* 将 CAN 错误代码设置为 BD 错误 */
          hcan->ErrorCode |= HAL_CAN_ERROR_BD;
          break;
      case(CAN_ESR_LEC_2 | CAN_ESR_LEC_1):
          /* 将 CAN 错误代码设置为 CRC 错误 */
          hcan->ErrorCode |= HAL_CAN_ERROR_CRC;
          break;
      default:
          break;
    }

    /* 清除上一次错误代码标志 */ 
    CLEAR_BIT(hcan->Instance->ESR, CAN_ESR_LEC);
  }

  /* 发生错误时调用错误回调函数 */
  if(hcan->ErrorCode != HAL_CAN_ERROR_NONE)
  {
    /* 清除 ERRI 标志 */ 
    hcan->Instance->MSR = CAN_MSR_ERRI; 
    /* 将 CAN 状态设置为就绪，以便能够重新启动该过程 */
    hcan->State = HAL_CAN_STATE_READY;

    /* 禁用中断： */
    /*  - 禁用错误警告中断 */
    /*  - 禁用被动错误中断 */
    /*  - 禁用总线关闭中断 */
    /*  - 禁用上一次错误代码中断 */
    /*  - 禁用错误中断 */
    /*  - 禁用 FIFO 0 消息挂起中断 */
    /*  - 禁用 FIFO 0 溢出中断 */
    /*  - 禁用 FIFO 1 消息挂起中断 */
    /*  - 禁用 FIFO 1 溢出中断 */
    /*  - 禁用发送邮箱空中断 */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_EWG |
                               CAN_IT_EPV |
                               CAN_IT_BOF |
                               CAN_IT_LEC |
                               CAN_IT_ERR |
                               CAN_IT_FMP0|
                               CAN_IT_FOV0|
                               CAN_IT_FMP1|
                               CAN_IT_FOV1|
                               CAN_IT_TME  );

    /* 调用错误回调函数 */
    HAL_CAN_ErrorCallback(hcan);
  }  
}

/**
  * @brief  非阻塞模式下的发送完成回调函数
  * @param  hcan: 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);
  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_CAN_TxCpltCallback
   */
}

/**
  * @brief  非阻塞模式下的接收完成回调函数
  * @param  hcan: 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);
  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_CAN_RxCpltCallback
   */
}

/**
  * @brief  CAN 错误回调函数。
  * @param  hcan: 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);
  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现 HAL_CAN_ErrorCallback
   */
}

/**
  * @}
  */

/** @defgroup CAN_Exported_Functions_Group3 外设状态和错误函数
  *  @brief   CAN 外设状态函数 
  *
@verbatim   
  ==============================================================================
            ##### 外设状态和错误函数 #####
  ==============================================================================
    [..]
    本节提供的函数可用于：
      (+) 检查 CAN 状态。
      (+) 检查中断处理过程中检测到的 CAN 错误
         
@endverbatim
  * @{
  */

/**
  * @brief  返回 CAN 状态
  * @param  hcan: 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_CAN_StateTypeDef HAL_CAN_GetState(CAN_HandleTypeDef* hcan)
{
  /* 返回 CAN 状态 */
  return hcan->State;
}

/**
  * @brief  返回 CAN 错误代码
  * @param  hcan: 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval CAN 错误代码
  */
uint32_t HAL_CAN_GetError(CAN_HandleTypeDef *hcan)
{
  return hcan->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup CAN_Private_Functions
  * @{
  */
/**
  * @brief  启动并发送一个 CAN 报文帧。
  * @param  hcan: 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
static HAL_StatusTypeDef CAN_Transmit_IT(CAN_HandleTypeDef* hcan)
{
  /* 禁用发送邮箱为空中断 */
  __HAL_CAN_DISABLE_IT(hcan, CAN_IT_TME);
  
  if(hcan->State == HAL_CAN_STATE_BUSY_TX)
  {   
    /* 禁用中断： */
    /*  - 禁用错误警告中断 */
    /*  - 禁用错误被动中断 */
    /*  - 禁用总线关闭中断 */
    /*  - 禁用上次错误代码中断 */
    /*  - 禁用错误中断 */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_EWG |
                               CAN_IT_EPV |
                               CAN_IT_BOF |
                               CAN_IT_LEC |
                               CAN_IT_ERR);
  }

  /* 改变 CAN 状态 */
  switch(hcan->State)
  {
    case(HAL_CAN_STATE_BUSY_TX_RX0):
      hcan->State = HAL_CAN_STATE_BUSY_RX0;
      break;
    case(HAL_CAN_STATE_BUSY_TX_RX1):
      hcan->State = HAL_CAN_STATE_BUSY_RX1;
      break;
    case(HAL_CAN_STATE_BUSY_TX_RX0_RX1):
      hcan->State = HAL_CAN_STATE_BUSY_RX0_RX1;
      break;
    default: /* HAL_CAN_STATE_BUSY_TX */
      hcan->State = HAL_CAN_STATE_READY;
      break;
  }

  /* 发送完成回调函数 */ 
  HAL_CAN_TxCpltCallback(hcan);
  
  return HAL_OK;
}

/**
  * @brief  接收一个正确的 CAN 帧。
  * @param  hcan:       指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。  
  * @param  FIFONumber: 指定 FIFO 编号    
  * @retval HAL 状态
  * @retval None
  */
static HAL_StatusTypeDef CAN_Receive_IT(CAN_HandleTypeDef* hcan, uint8_t FIFONumber)
{
  uint32_t tmp1 = 0U;
  CanRxMsgTypeDef* pRxMsg = NULL;

  /* 设置 RxMsg 指针 */
  if(FIFONumber == CAN_FIFO0)
  {
    pRxMsg = hcan->pRxMsg;
  }
  else /* FIFONumber == CAN_FIFO1 */
  {
    pRxMsg = hcan->pRx1Msg;
  }

  /* 获取标识符（ID） */
  pRxMsg->IDE = (uint8_t)0x04U & hcan->Instance->sFIFOMailBox[FIFONumber].RIR;
  if (pRxMsg->IDE == CAN_ID_STD)
  {
    pRxMsg->StdId = 0x000007FFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 21U);
  }
  else
  {
    pRxMsg->ExtId = 0x1FFFFFFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 3U);
  }
  
  pRxMsg->RTR = (uint8_t)0x02U & hcan->Instance->sFIFOMailBox[FIFONumber].RIR;
  /* 获取数据长度代码（DLC） */
  pRxMsg->DLC = (uint8_t)0x0FU & hcan->Instance->sFIFOMailBox[FIFONumber].RDTR;
  /* 获取 FIFONumber */
  pRxMsg->FIFONumber = FIFONumber;
  /* 获取过滤器匹配序号（FMI） */
  pRxMsg->FMI = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDTR >> 8U);
  /* 获取数据段 */
  pRxMsg->Data[0] = (uint8_t)0xFFU & hcan->Instance->sFIFOMailBox[FIFONumber].RDLR;
  pRxMsg->Data[1] = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 8U);
  pRxMsg->Data[2] = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 16U);
  pRxMsg->Data[3] = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 24U);
  pRxMsg->Data[4] = (uint8_t)0xFFU & hcan->Instance->sFIFOMailBox[FIFONumber].RDHR;
  pRxMsg->Data[5] = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 8U);
  pRxMsg->Data[6] = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 16U);
  pRxMsg->Data[7] = (uint8_t)0xFFU & (hcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 24U);
  /* 释放 FIFO */
  /* 释放 FIFO0 */
  if (FIFONumber == CAN_FIFO0)
  {
    __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO0);
    
    /* 禁用 FIFO 0 溢出和消息挂起中断 */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_FOV0 | CAN_IT_FMP0);
  }
  /* 释放 FIFO1 */
  else /* FIFONumber == CAN_FIFO1 */
  {
    __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO1);
    
    /* 禁用 FIFO 1 溢出和消息挂起中断 */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_FOV1 | CAN_IT_FMP1);
  }

  tmp1 = hcan->State;
  if((tmp1 == HAL_CAN_STATE_BUSY_RX0) || (tmp1 == HAL_CAN_STATE_BUSY_RX1))
  {
    /* 禁用中断： */
    /*  - 禁用错误警告中断 */
    /*  - 禁用被动错误中断 */
    /*  - 禁用总线关闭中断 */
    /*  - 禁用最后错误代码中断 */
    /*  - 禁用错误中断 */
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_EWG |
                               CAN_IT_EPV |
                               CAN_IT_BOF |
                               CAN_IT_LEC |
                               CAN_IT_ERR);
  }

  /* 更改 CAN 状态 */
  if (FIFONumber == CAN_FIFO0)
  {
    switch(hcan->State)
    {
      case(HAL_CAN_STATE_BUSY_TX_RX0):
        hcan->State = HAL_CAN_STATE_BUSY_TX;
        break;
      case(HAL_CAN_STATE_BUSY_RX0_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_RX1;
        break;
      case(HAL_CAN_STATE_BUSY_TX_RX0_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX1;
        break;
      default: /* HAL_CAN_STATE_BUSY_RX0 */
        hcan->State = HAL_CAN_STATE_READY;
        break;
    }
  }
  else /* FIFONumber == CAN_FIFO1 */
  {
    switch(hcan->State)
    {
      case(HAL_CAN_STATE_BUSY_TX_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_TX;
        break;
      case(HAL_CAN_STATE_BUSY_RX0_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_RX0;
        break;
      case(HAL_CAN_STATE_BUSY_TX_RX0_RX1):
        hcan->State = HAL_CAN_STATE_BUSY_TX_RX0;
        break;
      default: /* HAL_CAN_STATE_BUSY_RX1 */
        hcan->State = HAL_CAN_STATE_READY;
        break;
    }
  }

  /* 接收完成回调函数 */ 
  HAL_CAN_RxCpltCallback(hcan);

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @}
  */
#endif /* STM32F103x6) || STM32F103xB || STM32F103xE || STM32F103xG) || STM32F105xC || STM32F107xC */

#endif /* HAL_CAN_LEGACY_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

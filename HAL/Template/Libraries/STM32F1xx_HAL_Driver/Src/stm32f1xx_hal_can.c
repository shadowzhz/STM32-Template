/**
  ******************************************************************************
  * @file    stm32f1xx_hal_can.c
  * @author  MCD Application Team
  * @brief   CAN HAL 模块驱动程序。
  *          本文件提供固件函数以管理控制器局域网 (CAN) 外设的以下功能：
  *           + 初始化和去初始化函数
  *           + 配置函数
  *           + 控制函数
  *           + 中断管理
  *           + 回调函数
  *           + 外设状态与错误管理函数
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
  ==============================================================================
                        ##### 如何使用本驱动程序 #####
  ==============================================================================
    [..]
      (#) 实现 HAL_CAN_MspInit() 以初始化 CAN 底层资源：
         (++) 使用 __HAL_RCC_CANx_CLK_ENABLE() 使能 CAN 接口时钟
         (++) 配置 CAN 引脚
             (+++) 使能 CAN GPIO 的时钟
             (+++) 将 CAN 引脚配置为复用功能
         (++) 如果使用中断（例如 HAL_CAN_ActivateNotification()）
             (+++) 使用 HAL_NVIC_SetPriority() 配置 CAN 中断优先级
             (+++) 使用 HAL_NVIC_EnableIRQ() 使能 CAN IRQ 中断通道
             (+++) 在 CAN IRQ 中断服务程序中调用 HAL_CAN_IRQHandler()

      (#) 使用 HAL_CAN_Init() 函数初始化 CAN 外设。该函数会调用
          HAL_CAN_MspInit() 进行底层初始化。

      (#) 使用以下配置函数配置接收筛选器：
            (++) HAL_CAN_ConfigFilter()

      (#) 使用 HAL_CAN_Start() 函数启动 CAN 模块。此时该节点在总线上处于
          活动状态：可接收报文，亦可发送报文。

      (#) 要管理报文发送，可使用以下 Tx 控制函数：
            (++) HAL_CAN_AddTxMessage() 请求发送新报文。
            (++) HAL_CAN_AbortTxRequest() 中止处于挂起状态报文的发送。
            (++) HAL_CAN_GetTxMailboxesFreeLevel() 获取空闲 Tx 发送邮箱数量。
            (++) HAL_CAN_IsTxMessagePending() 检查 Tx 邮箱中是否有报文处于挂起状态。
            (++) HAL_CAN_GetTxTimestamp() 获取发送 Tx 报文的时间戳（如果使能了时间触发通信模式）。

      (#) 当 CAN Rx FIFO 接收到报文时，可使用 HAL_CAN_GetRxMessage() 函数读取报文。
          函数 HAL_CAN_GetRxFifoFillLevel() 可用于获取 Rx FIFO 中存储的 Rx 报文数量。

      (#) 调用 HAL_CAN_Stop() 函数停止 CAN 模块。

      (#) 通过 HAL_CAN_DeInit() 函数完成去初始化。


      *** 轮询模式操作 ***
      ==============================
    [..]
      (#) 接收：
            (++) 使用 HAL_CAN_GetRxFifoFillLevel() 监视报文接收情况，
                 直到至少接收到一条报文。
            (++) 然后使用 HAL_CAN_GetRxMessage() 读取报文。

      (#) 发送：
            (++) 使用 HAL_CAN_GetTxMailboxesFreeLevel() 监视 Tx 邮箱可用性，
                 直到至少有一个 Tx 邮箱空闲。
            (++) 然后使用 HAL_CAN_AddTxMessage() 请求发送报文。


      *** 中断模式操作 ***
      ================================
    [..]
      (#) 使用 HAL_CAN_ActivateNotification() 函数使能通知。
          随后可通过可用的用户回调函数 HAL_CAN_xxxCallback() 控制处理流程，
          同样使用 HAL_CAN_GetRxMessage() 和 HAL_CAN_AddTxMessage() API。

      (#) 可以使用 HAL_CAN_DeactivateNotification() 函数关闭通知。

      (#) 对于 CAN_IT_RX_FIFO0_MSG_PENDING 和 CAN_IT_RX_FIFO1_MSG_PENDING
          通知应特别注意。这些通知会触发回调函数
          HAL_CAN_RxFIFO0MsgPendingCallback() 和
          HAL_CAN_RxFIFO1MsgPendingCallback()。用户在此处有两种选择：
            (++) 直接在回调函数中使用 HAL_CAN_GetRxMessage() 读取 Rx 报文。
            (++) 或者在回调函数中关闭通知而不读取 Rx 报文。之后可以再使用
                 HAL_CAN_GetRxMessage() 读取 Rx 报文。一旦 Rx 报文读取完毕，
                 可再次使能该通知。


      *** 睡眠模式 ***
      ==================
    [..]
      (#) 可以使用 HAL_CAN_RequestSleep() 将 CAN 外设置于睡眠模式（低功耗）。
          当前的 CAN 操作（发送或接收 CAN 帧）完成后，将立即进入睡眠模式。

      (#) 可以使能通知以在进入睡眠模式时获得提醒。

      (#) 可以使用 HAL_CAN_IsSleepActive() 检查是否已进入睡眠模式。
          请注意，一旦提交睡眠模式请求（尚未真正进入睡眠模式），CAN 状态
          （可通过 API HAL_CAN_GetState() 访问）即变为 HAL_CAN_STATE_SLEEP_PENDING，
          当睡眠模式生效后变为 HAL_CAN_STATE_SLEEP_ACTIVE。

      (#) 可以通过以下两种方式触发从睡眠模式唤醒：
            (++) 使用 HAL_CAN_WakeUp()。当此函数返回时，即退出睡眠模式（如果返回状态为 HAL_OK）。
            (++) 如果使能了自动唤醒模式，当 CAN 外设检测到 Rx CAN 帧的起始位时自动唤醒。

  *** 回调函数注册 ***
  =============================================

  当编译宏定义 USE_HAL_CAN_REGISTER_CALLBACKS 设置为 1 时，
  允许用户动态配置驱动程序回调函数。
  使用 HAL_CAN_RegisterCallback() 函数注册中断回调函数。

  函数 HAL_CAN_RegisterCallback() 允许注册以下回调函数：
    (+) TxMailbox0CompleteCallback   : 发送邮箱 0 完成回调函数。
    (+) TxMailbox1CompleteCallback   : 发送邮箱 1 完成回调函数。
    (+) TxMailbox2CompleteCallback   : 发送邮箱 2 完成回调函数。
    (+) TxMailbox0AbortCallback      : 发送邮箱 0 中止回调函数。
    (+) TxMailbox1AbortCallback      : 发送邮箱 1 中止回调函数。
    (+) TxMailbox2AbortCallback      : 发送邮箱 2 中止回调函数。
    (+) RxFifo0MsgPendingCallback    : 接收 FIFO 0 报文挂起回调函数。
    (+) RxFifo0FullCallback          : 接收 FIFO 0 已满回调函数。
    (+) RxFifo1MsgPendingCallback    : 接收 FIFO 1 报文挂起回调函数。
    (+) RxFifo1FullCallback          : 接收 FIFO 1 已满回调函数。
    (+) SleepCallback                : 睡眠回调函数。
    (+) WakeUpFromRxMsgCallback      : 接收报文唤醒回调函数。
    (+) ErrorCallback                : 错误回调函数。
    (+) MspInitCallback              : CAN MspInit。
    (+) MspDeInitCallback            : CAN MspDeInit。
  该函数将 HAL 外设句柄、回调 ID 以及指向用户回调函数的指针作为参数。

  使用函数 HAL_CAN_UnRegisterCallback() 可将回调函数重置为默认的 weak 函数。
  HAL_CAN_UnRegisterCallback 将 HAL 外设句柄和回调 ID 作为参数。
  该函数允许重置以下回调函数：
    (+) TxMailbox0CompleteCallback   : 发送邮箱 0 完成回调函数。
    (+) TxMailbox1CompleteCallback   : 发送邮箱 1 完成回调函数。
    (+) TxMailbox2CompleteCallback   : 发送邮箱 2 完成回调函数。
    (+) TxMailbox0AbortCallback      : 发送邮箱 0 中止回调函数。
    (+) TxMailbox1AbortCallback      : 发送邮箱 1 中止回调函数。
    (+) TxMailbox2AbortCallback      : 发送邮箱 2 中止回调函数。
    (+) RxFifo0MsgPendingCallback    : 接收 FIFO 0 报文挂起回调函数。
    (+) RxFifo0FullCallback          : 接收 FIFO 0 已满回调函数。
    (+) RxFifo1MsgPendingCallback    : 接收 FIFO 1 报文挂起回调函数。
    (+) RxFifo1FullCallback          : 接收 FIFO 1 已满回调函数。
    (+) SleepCallback                : 睡眠回调函数。
    (+) WakeUpFromRxMsgCallback      : 接收报文唤醒回调函数。
    (+) ErrorCallback                : 错误回调函数。
    (+) MspInitCallback              : CAN MspInit。
    (+) MspDeInitCallback            : CAN MspDeInit。

  默认情况下，在 HAL_CAN_Init() 之后且状态为 HAL_CAN_STATE_RESET 时，
  所有回调函数都设置为对应的 weak 函数：
  例如 HAL_CAN_ErrorCallback()。
  例外的是 MspInit 和 MspDeInit 函数，仅当这些回调函数为空（事先未注册）时，
  在 HAL_CAN_Init()/HAL_CAN_DeInit() 中才会被重置为传统的 weak 函数。
  如果不为空，即 MspInit 或 MspDeInit 已注册，HAL_CAN_Init()/HAL_CAN_DeInit()
  将保留并使用事先注册的用户 MspInit/MspDeInit 回调函数。

  回调函数仅可在 HAL_CAN_STATE_READY 状态下进行注册/注销。
  例外的是 MspInit/MspDeInit，它们可以在 HAL_CAN_STATE_READY 或 HAL_CAN_STATE_RESET
  状态下注册/注销，因此已注册的（用户）MspInit/DeInit 回调函数可以在 Init/DeInit 期间使用。
  在此种情况下，在调用 HAL_CAN_DeInit() 或 HAL_CAN_Init() 函数之前，
  首先使用 HAL_CAN_RegisterCallback() 注册 MspInit/MspDeInit 用户回调函数。

  当编译宏定义 USE_HAL_CAN_REGISTER_CALLBACKS 设置为 0 或未定义时，
  回调函数注册功能不可用，所有回调函数均设置为对应的 weak 函数。

  @endverbatim
  ******************************************************************************
  */

/* 包含文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

#if defined(CAN1)

/** @defgroup CAN CAN
  * @brief CAN 驱动模块
  * @{
  */

#ifdef HAL_CAN_MODULE_ENABLED

#ifdef HAL_CAN_LEGACY_MODULE_ENABLED
#error "The CAN driver cannot be used with its legacy, Please enable only one CAN module at once"
#endif /* HAL_CAN_LEGACY_MODULE_ENABLED */

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/** @defgroup CAN_Private_Constants CAN 私有常量
  * @{
  */
#define CAN_TIMEOUT_VALUE 10U
#define CAN_WAKEUP_TIMEOUT_COUNTER 1000000U
/**
  * @}
  */
/* 私有宏 --------------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 私有函数原型 --------------------------------------------------------------*/
/* 导出函数 ------------------------------------------------------------------*/

/** @defgroup CAN_Exported_Functions CAN 导出函数
  * @{
  */

/** @defgroup CAN_Exported_Functions_Group1 初始化和去初始化函数
  * @brief    初始化和配置函数
  *
@verbatim
  ==============================================================================
              ##### 初始化和去初始化函数 #####
  ==============================================================================
    [..]  本节提供的函数用于：
      (+) HAL_CAN_Init                       : 初始化并配置 CAN。
      (+) HAL_CAN_DeInit                     : 去初始化 CAN。
      (+) HAL_CAN_MspInit                    : 初始化 CAN MSP 底层资源。
      (+) HAL_CAN_MspDeInit                  : 去初始化 CAN MSP 底层资源。

@endverbatim
  * @{
  */

/**
  * @brief  根据 CAN_InitStruct 中指定的参数初始化 CAN 外设。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_Init(CAN_HandleTypeDef *hcan)
{
  uint32_t tickstart;

  /* 检查 CAN 句柄 */
  if (hcan == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_CAN_ALL_INSTANCE(hcan->Instance));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.TimeTriggeredMode));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.AutoBusOff));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.AutoWakeUp));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.AutoRetransmission));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.ReceiveFifoLocked));
  assert_param(IS_FUNCTIONAL_STATE(hcan->Init.TransmitFifoPriority));
  assert_param(IS_CAN_MODE(hcan->Init.Mode));
  assert_param(IS_CAN_SJW(hcan->Init.SyncJumpWidth));
  assert_param(IS_CAN_BS1(hcan->Init.TimeSeg1));
  assert_param(IS_CAN_BS2(hcan->Init.TimeSeg2));
  assert_param(IS_CAN_PRESCALER(hcan->Init.Prescaler));

#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
  if (hcan->State == HAL_CAN_STATE_RESET)
  {
    /* 将回调函数重置为传统的弱定义函数 */
    hcan->RxFifo0MsgPendingCallback  =  HAL_CAN_RxFifo0MsgPendingCallback;  /* 传统的弱定义 RxFifo0MsgPendingCallback  */
    hcan->RxFifo0FullCallback        =  HAL_CAN_RxFifo0FullCallback;        /* 传统的弱定义 RxFifo0FullCallback        */
    hcan->RxFifo1MsgPendingCallback  =  HAL_CAN_RxFifo1MsgPendingCallback;  /* 传统的弱定义 RxFifo1MsgPendingCallback  */
    hcan->RxFifo1FullCallback        =  HAL_CAN_RxFifo1FullCallback;        /* 传统的弱定义 RxFifo1FullCallback        */
    hcan->TxMailbox0CompleteCallback =  HAL_CAN_TxMailbox0CompleteCallback; /* 传统的弱定义 TxMailbox0CompleteCallback */
    hcan->TxMailbox1CompleteCallback =  HAL_CAN_TxMailbox1CompleteCallback; /* 传统的弱定义 TxMailbox1CompleteCallback */
    hcan->TxMailbox2CompleteCallback =  HAL_CAN_TxMailbox2CompleteCallback; /* 传统的弱定义 TxMailbox2CompleteCallback */
    hcan->TxMailbox0AbortCallback    =  HAL_CAN_TxMailbox0AbortCallback;    /* 传统的弱定义 TxMailbox0AbortCallback    */
    hcan->TxMailbox1AbortCallback    =  HAL_CAN_TxMailbox1AbortCallback;    /* 传统的弱定义 TxMailbox1AbortCallback    */
    hcan->TxMailbox2AbortCallback    =  HAL_CAN_TxMailbox2AbortCallback;    /* 传统的弱定义 TxMailbox2AbortCallback    */
    hcan->SleepCallback              =  HAL_CAN_SleepCallback;              /* 传统的弱定义 SleepCallback              */
    hcan->WakeUpFromRxMsgCallback    =  HAL_CAN_WakeUpFromRxMsgCallback;    /* 传统的弱定义 WakeUpFromRxMsgCallback    */
    hcan->ErrorCallback              =  HAL_CAN_ErrorCallback;              /* 传统的弱定义 ErrorCallback              */

    if (hcan->MspInitCallback == NULL)
    {
      hcan->MspInitCallback = HAL_CAN_MspInit; /* 传统的弱定义 MspInit */
    }

    /* 初始化底层硬件：CLOCK、NVIC */
    hcan->MspInitCallback(hcan);
  }

#else
  if (hcan->State == HAL_CAN_STATE_RESET)
  {
    /* 初始化底层硬件：CLOCK、NVIC */
    HAL_CAN_MspInit(hcan);
  }
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */

  /* 请求初始化 */
  SET_BIT(hcan->Instance->MCR, CAN_MCR_INRQ);

  /* 获取滴答定时器计数值 */
  tickstart = HAL_GetTick();

  /* 等待初始化确认 */
  while ((hcan->Instance->MSR & CAN_MSR_INAK) == 0U)
  {
    if ((HAL_GetTick() - tickstart) > CAN_TIMEOUT_VALUE)
    {
      /* 更新错误代码 */
      hcan->ErrorCode |= HAL_CAN_ERROR_TIMEOUT;

      /* 改变 CAN 状态 */
      hcan->State = HAL_CAN_STATE_ERROR;

      return HAL_ERROR;
    }
  }

  /* 退出睡眠模式 */
  CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_SLEEP);

  /* 获取滴答定时器计数值 */
  tickstart = HAL_GetTick();

  /* 检查退出睡眠模式确认 */
  while ((hcan->Instance->MSR & CAN_MSR_SLAK) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > CAN_TIMEOUT_VALUE)
    {
      /* 更新错误代码 */
      hcan->ErrorCode |= HAL_CAN_ERROR_TIMEOUT;

      /* 改变 CAN 状态 */
      hcan->State = HAL_CAN_STATE_ERROR;

      return HAL_ERROR;
    }
  }

  /* 设置时间触发通信模式 */
  if (hcan->Init.TimeTriggeredMode == ENABLE)
  {
    SET_BIT(hcan->Instance->MCR, CAN_MCR_TTCM);
  }
  else
  {
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_TTCM);
  }

  /* 设置自动离线管理 */
  if (hcan->Init.AutoBusOff == ENABLE)
  {
    SET_BIT(hcan->Instance->MCR, CAN_MCR_ABOM);
  }
  else
  {
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_ABOM);
  }

  /* 设置自动唤醒模式 */
  if (hcan->Init.AutoWakeUp == ENABLE)
  {
    SET_BIT(hcan->Instance->MCR, CAN_MCR_AWUM);
  }
  else
  {
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_AWUM);
  }

  /* 设置自动重传 */
  if (hcan->Init.AutoRetransmission == ENABLE)
  {
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_NART);
  }
  else
  {
    SET_BIT(hcan->Instance->MCR, CAN_MCR_NART);
  }

  /* 设置接收 FIFO 锁定模式 */
  if (hcan->Init.ReceiveFifoLocked == ENABLE)
  {
    SET_BIT(hcan->Instance->MCR, CAN_MCR_RFLM);
  }
  else
  {
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_RFLM);
  }

  /* 设置发送 FIFO 优先级 */
  if (hcan->Init.TransmitFifoPriority == ENABLE)
  {
    SET_BIT(hcan->Instance->MCR, CAN_MCR_TXFP);
  }
  else
  {
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_TXFP);
  }

  /* 设置位时序寄存器 */
  WRITE_REG(hcan->Instance->BTR, (uint32_t)(hcan->Init.Mode           |
                                            hcan->Init.SyncJumpWidth  |
                                            hcan->Init.TimeSeg1       |
                                            hcan->Init.TimeSeg2       |
                                            (hcan->Init.Prescaler - 1U)));

  /* 初始化错误代码 */
  hcan->ErrorCode = HAL_CAN_ERROR_NONE;

  /* 初始化 CAN 状态 */
  hcan->State = HAL_CAN_STATE_READY;

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  将 CAN 外设寄存器去初始化恢复为其默认复位值。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_DeInit(CAN_HandleTypeDef *hcan)
{
  /* 检查 CAN 句柄 */
  if (hcan == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_CAN_ALL_INSTANCE(hcan->Instance));

  /* 停止 CAN 模块 */
  (void)HAL_CAN_Stop(hcan);

#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
  if (hcan->MspDeInitCallback == NULL)
  {
    hcan->MspDeInitCallback = HAL_CAN_MspDeInit; /* 传统的弱定义 MspDeInit */
  }

  /* 反初始化底层硬件：时钟、NVIC */
  hcan->MspDeInitCallback(hcan);

#else
  /* 反初始化底层硬件：时钟、NVIC */
  HAL_CAN_MspDeInit(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */

  /* 复位 CAN 外设 */
  SET_BIT(hcan->Instance->MCR, CAN_MCR_RESET);

  /* 复位 CAN 错误代码 */
  hcan->ErrorCode = HAL_CAN_ERROR_NONE;

  /* 更改 CAN 状态 */
  hcan->State = HAL_CAN_STATE_RESET;

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  初始化 CAN MSP。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);

  /* 注意：不应修改此函数，当需要使用回调函数时，
            可以在用户文件中实现 HAL_CAN_MspInit
   */
}

/**
  * @brief  反初始化 CAN MSP。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);

  /* 注意：不应修改此函数，当需要使用回调函数时，
            可以在用户文件中实现 HAL_CAN_MspDeInit
   */
}

#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
/**
  * @brief  注册 CAN 回调函数。
  *         用于替代预定义的弱函数回调
  * @param  hcan 指向包含 CAN 模块配置信息的 CAN_HandleTypeDef 结构体的指针
  * @param  CallbackID 要注册的回调函数 ID
  *         该参数可以是以下值之一：
  *           @arg @ref HAL_CAN_TX_MAILBOX0_COMPLETE_CB_ID 发送邮箱 0 完成回调 ID
  *           @arg @ref HAL_CAN_TX_MAILBOX1_COMPLETE_CB_ID 发送邮箱 1 完成回调 ID
  *           @arg @ref HAL_CAN_TX_MAILBOX2_COMPLETE_CB_ID 发送邮箱 2 完成回调 ID
  *           @arg @ref HAL_CAN_TX_MAILBOX0_ABORT_CB_ID 发送邮箱 0 中止回调 ID
  *           @arg @ref HAL_CAN_TX_MAILBOX1_ABORT_CB_ID 发送邮箱 1 中止回调 ID
  *           @arg @ref HAL_CAN_TX_MAILBOX2_ABORT_CB_ID 发送邮箱 2 中止回调 ID
  *           @arg @ref HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID 接收 FIFO 0 消息挂起回调 ID
  *           @arg @ref HAL_CAN_RX_FIFO0_FULL_CB_ID 接收 FIFO 0 满回调 ID
  *           @arg @ref HAL_CAN_RX_FIFO1_MSG_PENDING_CB_ID 接收 FIFO 1 消息挂起回调 ID
  *           @arg @ref HAL_CAN_RX_FIFO1_FULL_CB_ID 接收 FIFO 1 满回调 ID
  *           @arg @ref HAL_CAN_SLEEP_CB_ID 睡眠回调 ID
  *           @arg @ref HAL_CAN_WAKEUP_FROM_RX_MSG_CB_ID 从接收消息唤醒回调 ID
  *           @arg @ref HAL_CAN_ERROR_CB_ID 错误回调 ID
  *           @arg @ref HAL_CAN_MSPINIT_CB_ID MspInit 回调 ID
  *           @arg @ref HAL_CAN_MSPDEINIT_CB_ID MspDeInit 回调 ID
  * @param  pCallback 指向回调函数的指针
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_RegisterCallback(CAN_HandleTypeDef *hcan, HAL_CAN_CallbackIDTypeDef CallbackID,
                                           void (* pCallback)(CAN_HandleTypeDef *_hcan))
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    /* 更新错误代码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }

  if (hcan->State == HAL_CAN_STATE_READY)
  {
    switch (CallbackID)
    {
      case HAL_CAN_TX_MAILBOX0_COMPLETE_CB_ID :
        hcan->TxMailbox0CompleteCallback = pCallback;
        break;

      case HAL_CAN_TX_MAILBOX1_COMPLETE_CB_ID :
        hcan->TxMailbox1CompleteCallback = pCallback;
        break;

      case HAL_CAN_TX_MAILBOX2_COMPLETE_CB_ID :
        hcan->TxMailbox2CompleteCallback = pCallback;
        break;

      case HAL_CAN_TX_MAILBOX0_ABORT_CB_ID :
        hcan->TxMailbox0AbortCallback = pCallback;
        break;

      case HAL_CAN_TX_MAILBOX1_ABORT_CB_ID :
        hcan->TxMailbox1AbortCallback = pCallback;
        break;

      case HAL_CAN_TX_MAILBOX2_ABORT_CB_ID :
        hcan->TxMailbox2AbortCallback = pCallback;
        break;

      case HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID :
        hcan->RxFifo0MsgPendingCallback = pCallback;
        break;

      case HAL_CAN_RX_FIFO0_FULL_CB_ID :
        hcan->RxFifo0FullCallback = pCallback;
        break;

      case HAL_CAN_RX_FIFO1_MSG_PENDING_CB_ID :
        hcan->RxFifo1MsgPendingCallback = pCallback;
        break;

      case HAL_CAN_RX_FIFO1_FULL_CB_ID :
        hcan->RxFifo1FullCallback = pCallback;
        break;

      case HAL_CAN_SLEEP_CB_ID :
        hcan->SleepCallback = pCallback;
        break;

      case HAL_CAN_WAKEUP_FROM_RX_MSG_CB_ID :
        hcan->WakeUpFromRxMsgCallback = pCallback;
        break;

      case HAL_CAN_ERROR_CB_ID :
        hcan->ErrorCallback = pCallback;
        break;

      case HAL_CAN_MSPINIT_CB_ID :
        hcan->MspInitCallback = pCallback;
        break;

      case HAL_CAN_MSPDEINIT_CB_ID :
        hcan->MspDeInitCallback = pCallback;
        break;

      default :
        /* 更新错误代码 */
        hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (hcan->State == HAL_CAN_STATE_RESET)
  {
    switch (CallbackID)
    {
      case HAL_CAN_MSPINIT_CB_ID :
        hcan->MspInitCallback = pCallback;
        break;

      case HAL_CAN_MSPDEINIT_CB_ID :
        hcan->MspDeInitCallback = pCallback;
        break;

      default :
        /* 更新错误代码 */
        hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* 更新错误代码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;

    /* 返回错误状态 */
    status =  HAL_ERROR;
  }

  return status;
}

/**
  * @brief  注销一个 CAN 回调函数。
  *         CAN 回调函数将被重定向到预定义的弱函数（weak）回调
  * @param  hcan 指向包含 CAN 模块配置信息的 CAN_HandleTypeDef 结构的指针
  * @param  CallbackID 要注销的回调函数 ID
  *         该参数可以是以下值之一：
  *           @arg @ref HAL_CAN_TX_MAILBOX0_COMPLETE_CB_ID 发送邮箱 0 完成回调 ID
  *           @arg @ref HAL_CAN_TX_MAILBOX1_COMPLETE_CB_ID 发送邮箱 1 完成回调 ID
  *           @arg @ref HAL_CAN_TX_MAILBOX2_COMPLETE_CB_ID 发送邮箱 2 完成回调 ID
  *           @arg @ref HAL_CAN_TX_MAILBOX0_ABORT_CB_ID 发送邮箱 0 中止回调 ID
  *           @arg @ref HAL_CAN_TX_MAILBOX1_ABORT_CB_ID 发送邮箱 1 中止回调 ID
  *           @arg @ref HAL_CAN_TX_MAILBOX2_ABORT_CB_ID 发送邮箱 2 中止回调 ID
  *           @arg @ref HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID 接收 FIFO 0 消息挂起回调 ID
  *           @arg @ref HAL_CAN_RX_FIFO0_FULL_CB_ID 接收 FIFO 0 满回调 ID
  *           @arg @ref HAL_CAN_RX_FIFO1_MSG_PENDING_CB_ID 接收 FIFO 1 消息挂起回调 ID
  *           @arg @ref HAL_CAN_RX_FIFO1_FULL_CB_ID 接收 FIFO 1 满回调 ID
  *           @arg @ref HAL_CAN_SLEEP_CB_ID 休眠回调 ID
  *           @arg @ref HAL_CAN_WAKEUP_FROM_RX_MSG_CB_ID 接收消息唤醒回调 ID
  *           @arg @ref HAL_CAN_ERROR_CB_ID 错误回调 ID
  *           @arg @ref HAL_CAN_MSPINIT_CB_ID MspInit 回调 ID
  *           @arg @ref HAL_CAN_MSPDEINIT_CB_ID MspDeInit 回调 ID
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_UnRegisterCallback(CAN_HandleTypeDef *hcan, HAL_CAN_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (hcan->State == HAL_CAN_STATE_READY)
  {
    switch (CallbackID)
    {
      case HAL_CAN_TX_MAILBOX0_COMPLETE_CB_ID :
        hcan->TxMailbox0CompleteCallback = HAL_CAN_TxMailbox0CompleteCallback;
        break;

      case HAL_CAN_TX_MAILBOX1_COMPLETE_CB_ID :
        hcan->TxMailbox1CompleteCallback = HAL_CAN_TxMailbox1CompleteCallback;
        break;

      case HAL_CAN_TX_MAILBOX2_COMPLETE_CB_ID :
        hcan->TxMailbox2CompleteCallback = HAL_CAN_TxMailbox2CompleteCallback;
        break;

      case HAL_CAN_TX_MAILBOX0_ABORT_CB_ID :
        hcan->TxMailbox0AbortCallback = HAL_CAN_TxMailbox0AbortCallback;
        break;

      case HAL_CAN_TX_MAILBOX1_ABORT_CB_ID :
        hcan->TxMailbox1AbortCallback = HAL_CAN_TxMailbox1AbortCallback;
        break;

      case HAL_CAN_TX_MAILBOX2_ABORT_CB_ID :
        hcan->TxMailbox2AbortCallback = HAL_CAN_TxMailbox2AbortCallback;
        break;

      case HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID :
        hcan->RxFifo0MsgPendingCallback = HAL_CAN_RxFifo0MsgPendingCallback;
        break;

      case HAL_CAN_RX_FIFO0_FULL_CB_ID :
        hcan->RxFifo0FullCallback = HAL_CAN_RxFifo0FullCallback;
        break;

      case HAL_CAN_RX_FIFO1_MSG_PENDING_CB_ID :
        hcan->RxFifo1MsgPendingCallback = HAL_CAN_RxFifo1MsgPendingCallback;
        break;

      case HAL_CAN_RX_FIFO1_FULL_CB_ID :
        hcan->RxFifo1FullCallback = HAL_CAN_RxFifo1FullCallback;
        break;

      case HAL_CAN_SLEEP_CB_ID :
        hcan->SleepCallback = HAL_CAN_SleepCallback;
        break;

      case HAL_CAN_WAKEUP_FROM_RX_MSG_CB_ID :
        hcan->WakeUpFromRxMsgCallback = HAL_CAN_WakeUpFromRxMsgCallback;
        break;

      case HAL_CAN_ERROR_CB_ID :
        hcan->ErrorCallback = HAL_CAN_ErrorCallback;
        break;

      case HAL_CAN_MSPINIT_CB_ID :
        hcan->MspInitCallback = HAL_CAN_MspInit;
        break;

      case HAL_CAN_MSPDEINIT_CB_ID :
        hcan->MspDeInitCallback = HAL_CAN_MspDeInit;
        break;

      default :
        /* 更新错误代码 */
        hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (hcan->State == HAL_CAN_STATE_RESET)
  {
    switch (CallbackID)
    {
      case HAL_CAN_MSPINIT_CB_ID :
        hcan->MspInitCallback = HAL_CAN_MspInit;
        break;

      case HAL_CAN_MSPDEINIT_CB_ID :
        hcan->MspDeInitCallback = HAL_CAN_MspDeInit;
        break;

      default :
        /* 更新错误代码 */
        hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* 更新错误代码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_INVALID_CALLBACK;

    /* 返回错误状态 */
    status =  HAL_ERROR;
  }

  return status;
}
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup CAN_Exported_Functions_Group2 Configuration functions
  * @brief    配置函数。
  *
@verbatim
  ==============================================================================
              ##### 配置函数 #####
  ==============================================================================
    [..]  本节提供的函数可用于：
      (+) HAL_CAN_ConfigFilter            : 配置 CAN 接收过滤器

@endverbatim
  * @{
  */

/**
  * @brief  根据 CAN_FilterInitStruct 中指定的参数配置 CAN 接收过滤器。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @param  sFilterConfig 指向包含过滤器配置信息的 CAN_FilterTypeDef 结构体的指针。
  * @retval 无
  */
HAL_StatusTypeDef HAL_CAN_ConfigFilter(CAN_HandleTypeDef *hcan, const CAN_FilterTypeDef *sFilterConfig)
{
  uint32_t filternbrbitpos;
  CAN_TypeDef *can_ip = hcan->Instance;
  HAL_CAN_StateTypeDef state = hcan->State;

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 检查参数有效性 */
    assert_param(IS_CAN_FILTER_ID_HALFWORD(sFilterConfig->FilterIdHigh));
    assert_param(IS_CAN_FILTER_ID_HALFWORD(sFilterConfig->FilterIdLow));
    assert_param(IS_CAN_FILTER_ID_HALFWORD(sFilterConfig->FilterMaskIdHigh));
    assert_param(IS_CAN_FILTER_ID_HALFWORD(sFilterConfig->FilterMaskIdLow));
    assert_param(IS_CAN_FILTER_MODE(sFilterConfig->FilterMode));
    assert_param(IS_CAN_FILTER_SCALE(sFilterConfig->FilterScale));
    assert_param(IS_CAN_FILTER_FIFO(sFilterConfig->FilterFIFOAssignment));
    assert_param(IS_CAN_FILTER_ACTIVATION(sFilterConfig->FilterActivation));

#if   defined(CAN2)
    /* CAN1 和 CAN2 为双实例，共用 28 个过滤器组 */
    /* 选择主实例以访问过滤器组 */
    can_ip = CAN1;

    /* 检查参数有效性 */
    assert_param(IS_CAN_FILTER_BANK_DUAL(sFilterConfig->FilterBank));
    assert_param(IS_CAN_FILTER_BANK_DUAL(sFilterConfig->SlaveStartFilterBank));
#else
    /* CAN1 为单实例，具有 14 个专用过滤器组 */

    /* 检查参数有效性 */
    assert_param(IS_CAN_FILTER_BANK_SINGLE(sFilterConfig->FilterBank));
#endif /* CAN3 */

    /* 过滤器的初始化模式 */
    SET_BIT(can_ip->FMR, CAN_FMR_FINIT);

#if   defined(CAN2)
    /* 选择从实例 CAN2 的起始过滤器编号 */
    CLEAR_BIT(can_ip->FMR, CAN_FMR_CAN2SB);
    SET_BIT(can_ip->FMR, sFilterConfig->SlaveStartFilterBank << CAN_FMR_CAN2SB_Pos);

#endif /* CAN3 */
    /* 将过滤器编号转换为位偏移位置 */
    filternbrbitpos = (uint32_t)1 << (sFilterConfig->FilterBank & 0x1FU);

    /* 禁用过滤器 */
    CLEAR_BIT(can_ip->FA1R, filternbrbitpos);

    /* 过滤器位宽 */
    if (sFilterConfig->FilterScale == CAN_FILTERSCALE_16BIT)
    {
      /* 过滤器配置为 16 位宽 */
      CLEAR_BIT(can_ip->FS1R, filternbrbitpos);

      /* 第一个 16 位标识符和第一个 16 位掩码 */
      /* 或第一个 16 位标识符和第二个 16 位标识符 */
      can_ip->sFilterRegister[sFilterConfig->FilterBank].FR1 =
        ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdLow) << 16U) |
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdLow);

      /* 第二个 16 位标识符和第二个 16 位掩码 */
      /* 或第三个 16 位标识符和第四个 16 位标识符 */
      can_ip->sFilterRegister[sFilterConfig->FilterBank].FR2 =
        ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdHigh) << 16U) |
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdHigh);
    }

    if (sFilterConfig->FilterScale == CAN_FILTERSCALE_32BIT)
    {
      /* 过滤器配置为 32 位宽 */
      SET_BIT(can_ip->FS1R, filternbrbitpos);

      /* 32 位标识符或第一个 32 位标识符 */
      can_ip->sFilterRegister[sFilterConfig->FilterBank].FR1 =
        ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdHigh) << 16U) |
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterIdLow);

      /* 32 位掩码或第二个 32 位标识符 */
      can_ip->sFilterRegister[sFilterConfig->FilterBank].FR2 =
        ((0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdHigh) << 16U) |
        (0x0000FFFFU & (uint32_t)sFilterConfig->FilterMaskIdLow);
    }

    /* 过滤器模式 */
    if (sFilterConfig->FilterMode == CAN_FILTERMODE_IDMASK)
    {
      /* 过滤器配置为标识符掩码模式 */
      CLEAR_BIT(can_ip->FM1R, filternbrbitpos);
    }
    else /* CAN_FilterInitStruct->CAN_FilterMode == CAN_FilterMode_IdList */
    {
      /* 过滤器配置为标识符列表模式 */
      SET_BIT(can_ip->FM1R, filternbrbitpos);
    }

    /* 过滤器关联 FIFO */
    if (sFilterConfig->FilterFIFOAssignment == CAN_FILTER_FIFO0)
    {
      /* 过滤器分配给 FIFO 0 */
      CLEAR_BIT(can_ip->FFA1R, filternbrbitpos);
    }
    else
    {
      /* 将该过滤器分配给 FIFO 1 */
      SET_BIT(can_ip->FFA1R, filternbrbitpos);
    }

    /* 激活过滤器 */
    if (sFilterConfig->FilterActivation == CAN_FILTER_ENABLE)
    {
      SET_BIT(can_ip->FA1R, filternbrbitpos);
    }

    /* 退出过滤器初始化模式 */
    CLEAR_BIT(can_ip->FMR, CAN_FMR_FINIT);

    /* 返回函数状态 */
    return HAL_OK;
  }
  else
  {
    /* 更新错误代码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;

    return HAL_ERROR;
  }
}

/**
  * @}
  */

/** @defgroup CAN_Exported_Functions_Group3 Control functions
  * @brief    控制函数
  *
@verbatim
  ==============================================================================
                      ##### 控制函数 #####
  ==============================================================================
    [..]  本节提供的函数可实现：
      (+) HAL_CAN_Start                    : 启动 CAN 模块
      (+) HAL_CAN_Stop                     : 停止 CAN 模块
      (+) HAL_CAN_RequestSleep             : 请求进入睡眠模式
      (+) HAL_CAN_WakeUp                   : 从睡眠模式唤醒
      (+) HAL_CAN_IsSleepActive            : 检查睡眠模式是否处于激活状态
      (+) HAL_CAN_AddTxMessage             : 向发送邮箱添加报文并激活相应的发送请求
      (+) HAL_CAN_AbortTxRequest           : 中止发送请求
      (+) HAL_CAN_GetTxMailboxesFreeLevel  : 返回发送邮箱空闲级别
      (+) HAL_CAN_IsTxMessagePending       : 检查指定发送邮箱上是否有发送请求挂起
      (+) HAL_CAN_GetRxMessage             : 从接收 FIFO 获取一帧 CAN 报文
      (+) HAL_CAN_GetRxFifoFillLevel       : 返回接收 FIFO 填充级别

@endverbatim
  * @{
  */

/**
  * @brief  启动 CAN 模块。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_Start(CAN_HandleTypeDef *hcan)
{
  uint32_t tickstart;

  if (hcan->State == HAL_CAN_STATE_READY)
  {
    /* 更改 CAN 外设状态 */
    hcan->State = HAL_CAN_STATE_LISTENING;

    /* 请求退出初始化模式 */
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_INRQ);

    /* 获取当前滴答计数 */
    tickstart = HAL_GetTick();

    /* 等待应答确认 */
    while ((hcan->Instance->MSR & CAN_MSR_INAK) != 0U)
    {
      /* 检查是否超时 */
      if ((HAL_GetTick() - tickstart) > CAN_TIMEOUT_VALUE)
      {
        /* 更新错误代码 */
        hcan->ErrorCode |= HAL_CAN_ERROR_TIMEOUT;

        /* 更改 CAN 状态 */
        hcan->State = HAL_CAN_STATE_ERROR;

        return HAL_ERROR;
      }
    }

    /* 重置 CAN 错误码 (ErrorCode) */
    hcan->ErrorCode = HAL_CAN_ERROR_NONE;

    /* 返回函数状态 */
    return HAL_OK;
  }
  else
  {
    /* 更新错误码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_READY;

    return HAL_ERROR;
  }
}

/**
  * @brief  停止 CAN 模块并允许访问配置寄存器。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_Stop(CAN_HandleTypeDef *hcan)
{
  uint32_t tickstart;

  if (hcan->State == HAL_CAN_STATE_LISTENING)
  {
    /* 请求初始化 */
    SET_BIT(hcan->Instance->MCR, CAN_MCR_INRQ);

    /* 获取系统滴答计数 (tick) */
    tickstart = HAL_GetTick();

    /* 等待应答 */
    while ((hcan->Instance->MSR & CAN_MSR_INAK) == 0U)
    {
      /* 检查是否超时 */
      if ((HAL_GetTick() - tickstart) > CAN_TIMEOUT_VALUE)
      {
        /* 更新错误码 */
        hcan->ErrorCode |= HAL_CAN_ERROR_TIMEOUT;

        /* 更改 CAN 状态 */
        hcan->State = HAL_CAN_STATE_ERROR;

        return HAL_ERROR;
      }
    }

    /* 退出睡眠模式 */
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_SLEEP);

    /* 更改 CAN 外设状态 */
    hcan->State = HAL_CAN_STATE_READY;

    /* 返回函数状态 */
    return HAL_OK;
  }
  else
  {
    /* 更新错误码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_STARTED;

    return HAL_ERROR;
  }
}

/**
  * @brief  请求进入睡眠模式（低功耗）。
  *         从此函数返回时，一旦当前的 CAN 活动（CAN 帧的发送或接收）完成，
  *         将立即进入睡眠模式。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态。
  */
HAL_StatusTypeDef HAL_CAN_RequestSleep(CAN_HandleTypeDef *hcan)
{
  HAL_CAN_StateTypeDef state = hcan->State;

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 请求进入休眠模式 */
    SET_BIT(hcan->Instance->MCR, CAN_MCR_SLEEP);

    /* 返回函数状态 */
    return HAL_OK;
  }
  else
  {
    /* 更新错误代码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;

    /* 返回函数状态 */
    return HAL_ERROR;
  }
}

/**
  * @brief  从休眠模式中唤醒。
  *         当该函数返回 HAL_OK 状态时，表示已退出休眠模式。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态。
  */
HAL_StatusTypeDef HAL_CAN_WakeUp(CAN_HandleTypeDef *hcan)
{
  __IO uint32_t count = 0;
  HAL_CAN_StateTypeDef state = hcan->State;

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 唤醒请求 */
    CLEAR_BIT(hcan->Instance->MCR, CAN_MCR_SLEEP);

    /* 等待退出休眠模式 */
    do
    {
      /* 递增计数器 */
      count++;

      /* 检查是否发生超时 */
      if (count > CAN_WAKEUP_TIMEOUT_COUNTER)
      {
        /* 更新错误代码 */
        hcan->ErrorCode |= HAL_CAN_ERROR_TIMEOUT;

        return HAL_ERROR;
      }
    } while ((hcan->Instance->MSR & CAN_MSR_SLAK) != 0U);

    /* 返回函数状态 */
    return HAL_OK;
  }
  else
  {
    /* 更新错误代码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;

    return HAL_ERROR;
  }
}

/**
  * @brief  检查休眠模式是否处于激活状态。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval 状态
  *          - 0 : 休眠模式未激活。
  *          - 1 : 休眠模式处于激活状态。
  */
uint32_t HAL_CAN_IsSleepActive(const CAN_HandleTypeDef *hcan)
{
  uint32_t status = 0U;
  HAL_CAN_StateTypeDef state = hcan->State;

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 检查休眠模式 */
    if ((hcan->Instance->MSR & CAN_MSR_SLAK) != 0U)
    {
      status = 1U;
    }
  }

  /* 返回函数状态 */
  return status;
}

/**
  * @brief  将消息添加到第一个空闲的发送邮箱中并激活对应的发送请求。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @param  pHeader 指向 CAN_TxHeaderTypeDef 结构体的指针。
  * @param  aData 包含发送帧有效载荷（数据）的数组。
  * @param  pTxMailbox 指向变量的指针，该变量用于返回存储发送消息的发送邮箱。
  *         该参数可以是 @arg CAN_Tx_Mailboxes 的一个值。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_AddTxMessage(CAN_HandleTypeDef *hcan, const CAN_TxHeaderTypeDef *pHeader,
                                       const uint8_t aData[], uint32_t *pTxMailbox)
{
  uint32_t transmitmailbox;
  HAL_CAN_StateTypeDef state = hcan->State;
  uint32_t tsr = READ_REG(hcan->Instance->TSR);

  /* 检查参数 */
  assert_param(IS_CAN_IDTYPE(pHeader->IDE));
  assert_param(IS_CAN_RTR(pHeader->RTR));
  assert_param(IS_CAN_DLC(pHeader->DLC));
  if (pHeader->IDE == CAN_ID_STD)
  {
    assert_param(IS_CAN_STDID(pHeader->StdId));
  }
  else
  {
    assert_param(IS_CAN_EXTID(pHeader->ExtId));
  }
  assert_param(IS_FUNCTIONAL_STATE(pHeader->TransmitGlobalTime));

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 检查是否所有发送邮箱均未满 */
    if (((tsr & CAN_TSR_TME0) != 0U) ||
        ((tsr & CAN_TSR_TME1) != 0U) ||
        ((tsr & CAN_TSR_TME2) != 0U))
    {
      /* 选择一个空的发送邮箱 */
      transmitmailbox = (tsr & CAN_TSR_CODE) >> CAN_TSR_CODE_Pos;

      /* 保存发送邮箱 */
      *pTxMailbox = (uint32_t)1 << transmitmailbox;

      /* 设置标识符 (Id) */
      if (pHeader->IDE == CAN_ID_STD)
      {
        hcan->Instance->sTxMailBox[transmitmailbox].TIR = ((pHeader->StdId << CAN_TI0R_STID_Pos) |
                                                           pHeader->RTR);
      }
      else
      {
        hcan->Instance->sTxMailBox[transmitmailbox].TIR = ((pHeader->ExtId << CAN_TI0R_EXID_Pos) |
                                                           pHeader->IDE |
                                                           pHeader->RTR);
      }

      /* 设置数据长度代码 (DLC) */
      hcan->Instance->sTxMailBox[transmitmailbox].TDTR = (pHeader->DLC);

      /* 设置全局时间发送 (Transmit Global Time) 模式 */
      if (pHeader->TransmitGlobalTime == ENABLE)
      {
        SET_BIT(hcan->Instance->sTxMailBox[transmitmailbox].TDTR, CAN_TDT0R_TGT);
      }

      /* 设置数据字段 */
      WRITE_REG(hcan->Instance->sTxMailBox[transmitmailbox].TDHR,
                ((uint32_t)aData[7] << CAN_TDH0R_DATA7_Pos) |
                ((uint32_t)aData[6] << CAN_TDH0R_DATA6_Pos) |
                ((uint32_t)aData[5] << CAN_TDH0R_DATA5_Pos) |
                ((uint32_t)aData[4] << CAN_TDH0R_DATA4_Pos));
      WRITE_REG(hcan->Instance->sTxMailBox[transmitmailbox].TDLR,
                ((uint32_t)aData[3] << CAN_TDL0R_DATA3_Pos) |
                ((uint32_t)aData[2] << CAN_TDL0R_DATA2_Pos) |
                ((uint32_t)aData[1] << CAN_TDL0R_DATA1_Pos) |
                ((uint32_t)aData[0] << CAN_TDL0R_DATA0_Pos));

      /* 请求发送 */
      SET_BIT(hcan->Instance->sTxMailBox[transmitmailbox].TIR, CAN_TI0R_TXRQ);

      /* 返回函数状态 */
      return HAL_OK;
    }
    else
    {
      /* 更新错误代码 */
      hcan->ErrorCode |= HAL_CAN_ERROR_PARAM;

      return HAL_ERROR;
    }
  }
  else
  {
    /* 更新错误代码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;

    return HAL_ERROR;
  }
}

/**
  * @brief  中止发送请求
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @param  TxMailboxes 要中止的发送邮箱列表。
  *         该参数可以是 @arg CAN_Tx_Mailboxes 的任意组合。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_AbortTxRequest(CAN_HandleTypeDef *hcan, uint32_t TxMailboxes)
{
  HAL_CAN_StateTypeDef state = hcan->State;

  /* 检查函数参数 */
  assert_param(IS_CAN_TX_MAILBOX_LIST(TxMailboxes));

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 检查发送邮箱 0 */
    if ((TxMailboxes & CAN_TX_MAILBOX0) != 0U)
    {
      /* 请求中止发送邮箱 0 的发送操作 */
      SET_BIT(hcan->Instance->TSR, CAN_TSR_ABRQ0);
    }

    /* 检查发送邮箱 1 */
    if ((TxMailboxes & CAN_TX_MAILBOX1) != 0U)
    {
      /* 请求中止发送邮箱 1 的发送操作 */
      SET_BIT(hcan->Instance->TSR, CAN_TSR_ABRQ1);
    }

    /* 检查发送邮箱 2 */
    if ((TxMailboxes & CAN_TX_MAILBOX2) != 0U)
    {
      /* 请求中止发送邮箱 2 的发送操作 */
      SET_BIT(hcan->Instance->TSR, CAN_TSR_ABRQ2);
    }

    /* 返回函数状态 */
    return HAL_OK;
  }
  else
  {
    /* 更新错误代码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;

    return HAL_ERROR;
  }
}

/**
  * @brief  返回发送邮箱空闲级别：空闲发送邮箱的数量。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构的指针。
  * @retval 空闲发送邮箱的数量。
  */
uint32_t HAL_CAN_GetTxMailboxesFreeLevel(const CAN_HandleTypeDef *hcan)
{
  uint32_t freelevel = 0U;
  HAL_CAN_StateTypeDef state = hcan->State;

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 检查发送邮箱 0 状态 */
    if ((hcan->Instance->TSR & CAN_TSR_TME0) != 0U)
    {
      freelevel++;
    }

    /* 检查发送邮箱 1 状态 */
    if ((hcan->Instance->TSR & CAN_TSR_TME1) != 0U)
    {
      freelevel++;
    }

    /* 检查发送邮箱 2 状态 */
    if ((hcan->Instance->TSR & CAN_TSR_TME2) != 0U)
    {
      freelevel++;
    }
  }

  /* 返回发送邮箱空闲级别 */
  return freelevel;
}

/**
  * @brief  检查所选发送邮箱上是否有挂起的发送请求。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构的指针。
  * @param  TxMailboxes 待检查的发送邮箱列表。
  *         该参数可以是 @arg CAN_Tx_Mailboxes 的任意组合。
  * @retval 状态
  *          - 0 : 所选发送邮箱均无挂起的发送请求。
  *          - 1 : 所选发送邮箱中至少有一个存在挂起的发送请求。
  */
uint32_t HAL_CAN_IsTxMessagePending(const CAN_HandleTypeDef *hcan, uint32_t TxMailboxes)
{
  uint32_t status = 0U;
  HAL_CAN_StateTypeDef state = hcan->State;

  /* 检查函数参数 */
  assert_param(IS_CAN_TX_MAILBOX_LIST(TxMailboxes));

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 检查所选发送邮箱（Tx Mailbox）是否有待处理的传输请求 */
    if ((hcan->Instance->TSR & (TxMailboxes << CAN_TSR_TME0_Pos)) != (TxMailboxes << CAN_TSR_TME0_Pos))
    {
      status = 1U;
    }
  }

  /* 返回状态 */
  return status;
}

/**
  * @brief  若启用了时间触发通信模式，则返回已发送 Tx 消息的时间戳。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @param  TxMailbox 要读取已发送消息时间戳的发送邮箱（Tx Mailbox）。
  *         该参数可以是 @arg CAN_Tx_Mailboxes 的其中一个值。
  * @retval 从 Tx 邮箱发送的消息的时间戳。
  */
uint32_t HAL_CAN_GetTxTimestamp(const CAN_HandleTypeDef *hcan, uint32_t TxMailbox)
{
  uint32_t timestamp = 0U;
  uint32_t transmitmailbox;
  HAL_CAN_StateTypeDef state = hcan->State;

  /* 检查函数参数 */
  assert_param(IS_CAN_TX_MAILBOX(TxMailbox));

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 选择发送邮箱 */
    transmitmailbox = POSITION_VAL(TxMailbox);

    /* 获取时间戳 */
    timestamp = (hcan->Instance->sTxMailBox[transmitmailbox].TDTR & CAN_TDT0R_TIME) >> CAN_TDT0R_TIME_Pos;
  }

  /* 返回时间戳 */
  return timestamp;
}

/**
  * @brief  从接收 FIFO 区域获取一个 CAN 帧到消息 RAM 中。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @param  RxFifo 要读取接收消息的 FIFO 编号。
  *         该参数可以是 @arg CAN_receive_FIFO_number 的一个值。
  * @param  pHeader 指向用于存储接收帧报头的 CAN_RxHeaderTypeDef 结构体的指针。
  * @param  aData 用于存储接收帧有效载荷（payload）的数据数组。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_GetRxMessage(CAN_HandleTypeDef *hcan, uint32_t RxFifo,
                                       CAN_RxHeaderTypeDef *pHeader, uint8_t aData[])
{
  HAL_CAN_StateTypeDef state = hcan->State;

  assert_param(IS_CAN_RX_FIFO(RxFifo));

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 检查接收 FIFO */
    if (RxFifo == CAN_RX_FIFO0) /* 接收元素分配给接收 FIFO 0 */
    {
      /* 检查接收 FIFO 0 是否非空 */
      if ((hcan->Instance->RF0R & CAN_RF0R_FMP0) == 0U)
      {
        /* 更新错误代码 */
        hcan->ErrorCode |= HAL_CAN_ERROR_PARAM;

        return HAL_ERROR;
      }
    }
    else /* 接收元素分配给接收 FIFO 1 */
    {
      /* 检查接收 FIFO 1 是否非空 */
      if ((hcan->Instance->RF1R & CAN_RF1R_FMP1) == 0U)
      {
        /* 更新错误代码 */
        hcan->ErrorCode |= HAL_CAN_ERROR_PARAM;

        return HAL_ERROR;
      }
    }

    /* 获取报文头 */
    pHeader->IDE = CAN_RI0R_IDE & hcan->Instance->sFIFOMailBox[RxFifo].RIR;
    if (pHeader->IDE == CAN_ID_STD)
    {
      pHeader->StdId = (CAN_RI0R_STID & hcan->Instance->sFIFOMailBox[RxFifo].RIR) >> CAN_TI0R_STID_Pos;
    }
    else
    {
      pHeader->ExtId = ((CAN_RI0R_EXID | CAN_RI0R_STID) &
                        hcan->Instance->sFIFOMailBox[RxFifo].RIR) >> CAN_RI0R_EXID_Pos;
    }
    pHeader->RTR = (CAN_RI0R_RTR & hcan->Instance->sFIFOMailBox[RxFifo].RIR);
    if (((CAN_RDT0R_DLC & hcan->Instance->sFIFOMailBox[RxFifo].RDTR) >> CAN_RDT0R_DLC_Pos) >= 8U)
    {
      /* 如果接收到的 DLC 字段超范围，则截断为 8 */
      pHeader->DLC = 8U;
    }
    else
    {
      pHeader->DLC = (CAN_RDT0R_DLC & hcan->Instance->sFIFOMailBox[RxFifo].RDTR) >> CAN_RDT0R_DLC_Pos;
    }
    pHeader->FilterMatchIndex = (CAN_RDT0R_FMI & hcan->Instance->sFIFOMailBox[RxFifo].RDTR) >> CAN_RDT0R_FMI_Pos;
    pHeader->Timestamp = (CAN_RDT0R_TIME & hcan->Instance->sFIFOMailBox[RxFifo].RDTR) >> CAN_RDT0R_TIME_Pos;

    /* 获取数据 */
    aData[0] = (uint8_t)((CAN_RDL0R_DATA0 & hcan->Instance->sFIFOMailBox[RxFifo].RDLR) >> CAN_RDL0R_DATA0_Pos);
    aData[1] = (uint8_t)((CAN_RDL0R_DATA1 & hcan->Instance->sFIFOMailBox[RxFifo].RDLR) >> CAN_RDL0R_DATA1_Pos);
    aData[2] = (uint8_t)((CAN_RDL0R_DATA2 & hcan->Instance->sFIFOMailBox[RxFifo].RDLR) >> CAN_RDL0R_DATA2_Pos);
    aData[3] = (uint8_t)((CAN_RDL0R_DATA3 & hcan->Instance->sFIFOMailBox[RxFifo].RDLR) >> CAN_RDL0R_DATA3_Pos);
    aData[4] = (uint8_t)((CAN_RDH0R_DATA4 & hcan->Instance->sFIFOMailBox[RxFifo].RDHR) >> CAN_RDH0R_DATA4_Pos);
    aData[5] = (uint8_t)((CAN_RDH0R_DATA5 & hcan->Instance->sFIFOMailBox[RxFifo].RDHR) >> CAN_RDH0R_DATA5_Pos);
    aData[6] = (uint8_t)((CAN_RDH0R_DATA6 & hcan->Instance->sFIFOMailBox[RxFifo].RDHR) >> CAN_RDH0R_DATA6_Pos);
    aData[7] = (uint8_t)((CAN_RDH0R_DATA7 & hcan->Instance->sFIFOMailBox[RxFifo].RDHR) >> CAN_RDH0R_DATA7_Pos);

    /* 释放 FIFO */
    if (RxFifo == CAN_RX_FIFO0) /* 接收元素分配至 Rx FIFO 0 */
    {
      /* 释放 RX FIFO 0 */
      SET_BIT(hcan->Instance->RF0R, CAN_RF0R_RFOM0);
    }
    else /* 接收元素分配至 Rx FIFO 1 */
    {
      /* 释放 RX FIFO 1 */
      SET_BIT(hcan->Instance->RF1R, CAN_RF1R_RFOM1);
    }

    /* 返回函数状态 */
    return HAL_OK;
  }
  else
  {
    /* 更新错误代码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;

    return HAL_ERROR;
  }
}

/**
  * @brief  返回 Rx FIFO 填充级别（消息存量）。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @param  RxFifo Rx FIFO。
  *         该参数可以是 @arg CAN_receive_FIFO_number 的一个值。
  * @retval Rx FIFO 中可用的消息数量。
  */
uint32_t HAL_CAN_GetRxFifoFillLevel(const CAN_HandleTypeDef *hcan, uint32_t RxFifo)
{
  uint32_t filllevel = 0U;
  HAL_CAN_StateTypeDef state = hcan->State;

  /* 检查函数参数 */
  assert_param(IS_CAN_RX_FIFO(RxFifo));

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    if (RxFifo == CAN_RX_FIFO0)
    {
      filllevel = hcan->Instance->RF0R & CAN_RF0R_FMP0;
    }
    else /* RxFifo == CAN_RX_FIFO1 */
    {
      filllevel = hcan->Instance->RF1R & CAN_RF1R_FMP1;
    }
  }

  /* 返回 Rx FIFO 填充级别 */
  return filllevel;
}

/**
  * @}
  */

/** @defgroup CAN_Exported_Functions_Group4 中断管理
  * @brief    中断管理
  *
@verbatim
  ==============================================================================
                       ##### 中断管理 #####
  ==============================================================================
    [..]  本节提供的函数可用于：
      (+) HAL_CAN_ActivateNotification      : 使能中断
      (+) HAL_CAN_DeactivateNotification    : 禁用中断
      (+) HAL_CAN_IRQHandler                : 处理 CAN 中断请求

@endverbatim
  * @{
  */

/**
  * @brief  使能中断。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @param  ActiveITs 指示要使能的中断。
  *         该参数可以是 @arg CAN_Interrupts 的任意组合。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_ActivateNotification(CAN_HandleTypeDef *hcan, uint32_t ActiveITs)
{
  HAL_CAN_StateTypeDef state = hcan->State;

  /* 检查函数参数 */
  assert_param(IS_CAN_IT(ActiveITs));

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 使能所选中断 */
    __HAL_CAN_ENABLE_IT(hcan, ActiveITs);

    /* 返回函数状态 */
    return HAL_OK;
  }
  else
  {
    /* 更新错误代码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;

    return HAL_ERROR;
  }
}

/**
  * @brief  禁用中断。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @param  InactiveITs 指示要禁用的中断。
  *         该参数可以是 @arg CAN_Interrupts 的任意组合。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_DeactivateNotification(CAN_HandleTypeDef *hcan, uint32_t InactiveITs)
{
  HAL_CAN_StateTypeDef state = hcan->State;

  /* 检查函数参数 */
  assert_param(IS_CAN_IT(InactiveITs));

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 禁用所选中断 */
    __HAL_CAN_DISABLE_IT(hcan, InactiveITs);

    /* 返回函数状态 */
    return HAL_OK;
  }
  else
  {
    /* 更新错误代码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;

    return HAL_ERROR;
  }
}

/**
  * @brief  处理 CAN 中断请求
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
void HAL_CAN_IRQHandler(CAN_HandleTypeDef *hcan)
{
  uint32_t errorcode = HAL_CAN_ERROR_NONE;
  uint32_t interrupts = READ_REG(hcan->Instance->IER);
  uint32_t msrflags = READ_REG(hcan->Instance->MSR);
  uint32_t tsrflags = READ_REG(hcan->Instance->TSR);
  uint32_t rf0rflags = READ_REG(hcan->Instance->RF0R);
  uint32_t rf1rflags = READ_REG(hcan->Instance->RF1R);
  uint32_t esrflags = READ_REG(hcan->Instance->ESR);

  /* 发送邮箱空中断管理 *****************************/
  if ((interrupts & CAN_IT_TX_MAILBOX_EMPTY) != 0U)
  {
    /* 发送邮箱 0 管理 *****************************************/
    if ((tsrflags & CAN_TSR_RQCP0) != 0U)
    {
      /* 清除传输完成标志（以及 TXOK0、ALST0、TERR0 位） */
      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_RQCP0);

      if ((tsrflags & CAN_TSR_TXOK0) != 0U)
      {
        /* 发送邮箱 0 完成回调 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
        /* 调用已注册的回调函数 */
        hcan->TxMailbox0CompleteCallback(hcan);
#else
        /* 调用弱定义（被重写）的回调函数 */
        HAL_CAN_TxMailbox0CompleteCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
      }
      else
      {
        if ((tsrflags & CAN_TSR_ALST0) != 0U)
        {
          /* 更新错误代码 */
          errorcode |= HAL_CAN_ERROR_TX_ALST0;
        }
        else if ((tsrflags & CAN_TSR_TERR0) != 0U)
        {
          /* 更新错误代码 */
          errorcode |= HAL_CAN_ERROR_TX_TERR0;
        }
        else
        {
          /* 发送邮箱 0 中止回调 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
          /* 调用已注册的回调函数 */
          hcan->TxMailbox0AbortCallback(hcan);
#else
          /* 调用弱定义（被重写）的回调函数 */
          HAL_CAN_TxMailbox0AbortCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
        }
      }
    }

    /* 发送邮箱 1 管理 *****************************************/
    if ((tsrflags & CAN_TSR_RQCP1) != 0U)
    {
      /* 清除发送完成标志（以及 TXOK1、ALST1、TERR1 位） */
      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_RQCP1);

      if ((tsrflags & CAN_TSR_TXOK1) != 0U)
      {
        /* 发送邮箱 1 完成回调 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
        /* 调用已注册的回调函数 */
        hcan->TxMailbox1CompleteCallback(hcan);
#else
        /* 调用弱定义（被重写）的回调函数 */
        HAL_CAN_TxMailbox1CompleteCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
      }
      else
      {
        if ((tsrflags & CAN_TSR_ALST1) != 0U)
        {
          /* 更新错误代码 */
          errorcode |= HAL_CAN_ERROR_TX_ALST1;
        }
        else if ((tsrflags & CAN_TSR_TERR1) != 0U)
        {
          /* 更新错误代码 */
          errorcode |= HAL_CAN_ERROR_TX_TERR1;
        }
        else
        {
          /* 发送邮箱 1 中止回调函数 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
          /* 调用已注册的回调函数 */
          hcan->TxMailbox1AbortCallback(hcan);
#else
          /* 调用弱定义（可重写）的回调函数 */
          HAL_CAN_TxMailbox1AbortCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
        }
      }
    }

    /* 发送邮箱 2 管理 *****************************************/
    if ((tsrflags & CAN_TSR_RQCP2) != 0U)
    {
      /* 清除发送完成标志（以及 TXOK2、ALST2、TERR2 位） */
      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_RQCP2);

      if ((tsrflags & CAN_TSR_TXOK2) != 0U)
      {
        /* 发送邮箱 2 完成回调函数 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
        /* 调用已注册的回调函数 */
        hcan->TxMailbox2CompleteCallback(hcan);
#else
        /* 调用弱定义（可重写）的回调函数 */
        HAL_CAN_TxMailbox2CompleteCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
      }
      else
      {
        if ((tsrflags & CAN_TSR_ALST2) != 0U)
        {
          /* 更新错误代码 */
          errorcode |= HAL_CAN_ERROR_TX_ALST2;
        }
        else if ((tsrflags & CAN_TSR_TERR2) != 0U)
        {
          /* 更新错误代码 */
          errorcode |= HAL_CAN_ERROR_TX_TERR2;
        }
        else
        {
          /* 发送邮箱 2 中止回调函数 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
          /* 调用已注册的回调函数 */
          hcan->TxMailbox2AbortCallback(hcan);
#else
          /* 调用弱定义（重载）的回调函数 */
          HAL_CAN_TxMailbox2AbortCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
        }
      }
    }
  }

  /* 接收 FIFO 0 溢出中断管理 *****************************/
  if ((interrupts & CAN_IT_RX_FIFO0_OVERRUN) != 0U)
  {
    if ((rf0rflags & CAN_RF0R_FOVR0) != 0U)
    {
      /* 将 CAN 错误码设置为 Rx FIFO 0 溢出错误 */
      errorcode |= HAL_CAN_ERROR_RX_FOV0;

      /* 清除 FIFO 0 溢出标志位 */
      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV0);
    }
  }

  /* 接收 FIFO 0 满中断管理 ********************************/
  if ((interrupts & CAN_IT_RX_FIFO0_FULL) != 0U)
  {
    if ((rf0rflags & CAN_RF0R_FULL0) != 0U)
    {
      /* 清除 FIFO 0 满标志位 */
      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FF0);

      /* 接收 FIFO 0 满回调函数 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
      /* 调用已注册的回调函数 */
      hcan->RxFifo0FullCallback(hcan);
#else
      /* 调用弱定义（重载）的回调函数 */
      HAL_CAN_RxFifo0FullCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
    }
  }

  /* 接收 FIFO 0 消息挂起中断管理 *********************/
  if ((interrupts & CAN_IT_RX_FIFO0_MSG_PENDING) != 0U)
  {
    /* 检查消息是否仍处于挂起状态 */
    if ((hcan->Instance->RF0R & CAN_RF0R_FMP0) != 0U)
    {
      /* 接收 FIFO 0 消息挂起回调函数 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
      /* 调用已注册的回调函数 */
      hcan->RxFifo0MsgPendingCallback(hcan);
#else
      /* 调用弱定义（被重写）的回调函数 */
      HAL_CAN_RxFifo0MsgPendingCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
    }
  }

  /* 接收 FIFO 1 溢出中断管理 *********************************/
  if ((interrupts & CAN_IT_RX_FIFO1_OVERRUN) != 0U)
  {
    if ((rf1rflags & CAN_RF1R_FOVR1) != 0U)
    {
      /* 将 CAN 错误码设置为接收 FIFO 1 溢出错误 */
      errorcode |= HAL_CAN_ERROR_RX_FOV1;

      /* 清除 FIFO 1 溢出标志位 */
      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV1);
    }
  }

  /* 接收 FIFO 1 满中断管理 ************************************/
  if ((interrupts & CAN_IT_RX_FIFO1_FULL) != 0U)
  {
    if ((rf1rflags & CAN_RF1R_FULL1) != 0U)
    {
      /* 清除 FIFO 1 满标志位 */
      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FF1);

      /* 接收 FIFO 1 满回调函数 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
      /* 调用已注册的回调函数 */
      hcan->RxFifo1FullCallback(hcan);
#else
      /* 调用弱定义（被重写）的回调函数 */
      HAL_CAN_RxFifo1FullCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
    }
  }

  /* 接收 FIFO 1 消息挂起中断管理 *****************************/ 
  if ((interrupts & CAN_IT_RX_FIFO1_MSG_PENDING) != 0U)
  {
    /* 检查消息是否仍处于挂起状态 */
    if ((hcan->Instance->RF1R & CAN_RF1R_FMP1) != 0U)
    {
      /* 接收 FIFO 1 消息挂起回调函数 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
      /* 调用已注册的回调函数 */
      hcan->RxFifo1MsgPendingCallback(hcan);
#else
      /* 调用弱定义（被重写）的回调函数 */
      HAL_CAN_RxFifo1MsgPendingCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
    }
  }

  /* 睡眠中断管理 *********************************************/
  if ((interrupts & CAN_IT_SLEEP_ACK) != 0U)
  {
    if ((msrflags & CAN_MSR_SLAKI) != 0U)
    {
      /* 清除睡眠中断标志位 */
      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_SLAKI);

      /* 睡眠回调函数 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
      /* 调用已注册的回调函数 */
      hcan->SleepCallback(hcan);
#else
      /* 调用弱定义（被重写）的回调函数 */
      HAL_CAN_SleepCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
    }
  }

  /* 唤醒中断管理 *********************************************/
  if ((interrupts & CAN_IT_WAKEUP) != 0U)
  {
    if ((msrflags & CAN_MSR_WKUI) != 0U)
    {
      /* 清除唤醒标志位 */
      __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_WKU);

      /* 唤醒回调函数 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
      /* 调用已注册的回调函数 */
      hcan->WakeUpFromRxMsgCallback(hcan);
#else
      /* 调用弱定义（被重写）的回调函数 */
      HAL_CAN_WakeUpFromRxMsgCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
    }
  }

  /* 错误中断管理 *********************************************/
  if ((interrupts & CAN_IT_ERROR) != 0U)
  {
    if ((msrflags & CAN_MSR_ERRI) != 0U)
    {
      /* 检查错误警告标志位（Error Warning Flag） */
      if (((interrupts & CAN_IT_ERROR_WARNING) != 0U) &&
          ((esrflags & CAN_ESR_EWGF) != 0U))
      {
        /* 将 CAN 错误码设置为错误警告（Error Warning） */
        errorcode |= HAL_CAN_ERROR_EWG;

        /* 错误警告标志位为只读属性，无需手动清除 */
      }

      /* 检查消极错误标志位（Error Passive Flag） */
      if (((interrupts & CAN_IT_ERROR_PASSIVE) != 0U) &&
          ((esrflags & CAN_ESR_EPVF) != 0U))
      {
        /* 将 CAN 错误码设置为消极错误（Error Passive） */
        errorcode |= HAL_CAN_ERROR_EPV;

        /* 消极错误标志位为只读属性，无需手动清除 */
      }

      /* 检查总线关闭标志位（Bus-off Flag） */
      if (((interrupts & CAN_IT_BUSOFF) != 0U) &&
          ((esrflags & CAN_ESR_BOFF) != 0U))
      {
        /* 将 CAN 错误码设置为总线关闭（Bus-Off） */
        errorcode |= HAL_CAN_ERROR_BOF;

        /* 总线关闭错误标志位为只读属性，无需手动清除 */
      }

      /* 检查最后错误代码标志位（Last Error Code Flag） */
      if (((interrupts & CAN_IT_LAST_ERROR_CODE) != 0U) &&
          ((esrflags & CAN_ESR_LEC) != 0U))
      {
        switch (esrflags & CAN_ESR_LEC)
        {
          case (CAN_ESR_LEC_0):
            /* 将 CAN 错误码设置为填充错误（Stuff error） */
            errorcode |= HAL_CAN_ERROR_STF;
            break;
          case (CAN_ESR_LEC_1):
            /* 将 CAN 错误码设置为格式错误（Form error） */
            errorcode |= HAL_CAN_ERROR_FOR;
            break;
          case (CAN_ESR_LEC_1 | CAN_ESR_LEC_0):
            /* 将 CAN 错误码设置为应答错误（Acknowledgement error） */
            errorcode |= HAL_CAN_ERROR_ACK;
            break;
          case (CAN_ESR_LEC_2):
            /* 将 CAN 错误代码设置为隐性位错误 */
            errorcode |= HAL_CAN_ERROR_BR;
            break;
          case (CAN_ESR_LEC_2 | CAN_ESR_LEC_0):
            /* 将 CAN 错误代码设置为显性位错误 */
            errorcode |= HAL_CAN_ERROR_BD;
            break;
          case (CAN_ESR_LEC_2 | CAN_ESR_LEC_1):
            /* 将 CAN 错误代码设置为 CRC 错误 */
            errorcode |= HAL_CAN_ERROR_CRC;
            break;
          default:
            break;
        }

        /* 清除最后一次错误代码标志 */
        CLEAR_BIT(hcan->Instance->ESR, CAN_ESR_LEC);
      }
    }

    /* 清除 ERRI 标志 */
    __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_ERRI);
  }

  /* 发生错误时调用错误回调函数 */
  if (errorcode != HAL_CAN_ERROR_NONE)
  {
    /* 更新句柄中的错误代码 */
    hcan->ErrorCode |= errorcode;

    /* 调用错误回调函数 */
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
    /* 调用已注册的回调函数 */
    hcan->ErrorCallback(hcan);
#else
    /* 调用弱定义（可重写）的回调函数 */
    HAL_CAN_ErrorCallback(hcan);
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
  }
}

/**
  * @}
  */

/** @defgroup CAN_Exported_Functions_Group5 Callback functions
  * @brief   CAN 回调函数
  *
@verbatim
  ==============================================================================
                          ##### 回调函数 #####
  ==============================================================================
    [..]
    本小节提供以下回调函数：
      (+) HAL_CAN_TxMailbox0CompleteCallback
      (+) HAL_CAN_TxMailbox1CompleteCallback
      (+) HAL_CAN_TxMailbox2CompleteCallback
      (+) HAL_CAN_TxMailbox0AbortCallback
      (+) HAL_CAN_TxMailbox1AbortCallback
      (+) HAL_CAN_TxMailbox2AbortCallback
      (+) HAL_CAN_RxFifo0MsgPendingCallback
      (+) HAL_CAN_RxFifo0FullCallback
      (+) HAL_CAN_RxFifo1MsgPendingCallback
      (+) HAL_CAN_RxFifo1FullCallback
      (+) HAL_CAN_SleepCallback
      (+) HAL_CAN_WakeUpFromRxMsgCallback
      (+) HAL_CAN_ErrorCallback

@endverbatim
  * @{
  */

/**
  * @brief  发送邮箱 0 完成回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构的指针。
  * @retval 无
  */
__weak void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(hcan);

  /* NOTE : 不应修改此函数，当需要该回调函数时，
            可以在用户文件中重新实现
            HAL_CAN_TxMailbox0CompleteCallback
   */
}

/**
  * @brief  发送邮箱 1 完成回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);

  /* NOTE : 不应修改此函数，当需要该回调函数时，
            可以在用户文件中重新实现
            HAL_CAN_TxMailbox1CompleteCallback
   */
}

/**
  * @brief  发送邮箱 2 完成回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);

  /* NOTE : 不应修改此函数，当需要该回调函数时，
            可以在用户文件中重新实现
            HAL_CAN_TxMailbox2CompleteCallback
   */
}

/**
  * @brief  发送邮箱 0 取消回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);

  /* NOTE : 不应修改此函数，当需要该回调函数时，
            可以在用户文件中重新实现
            HAL_CAN_TxMailbox0AbortCallback
   */
}

/**
  * @brief  发送邮箱 1 取消回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);

  /* NOTE : 不应修改此函数，当需要该回调函数时，
            可以在用户文件中重新实现
            HAL_CAN_TxMailbox1AbortCallback
   */
}

/**
  * @brief  发送邮箱 2 取消回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现
            HAL_CAN_TxMailbox2AbortCallback
   */
}

/**
  * @brief  接收 FIFO 0 消息挂起（待处理）回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未引用的形参导致编译警告 */
  UNUSED(hcan);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现
            HAL_CAN_RxFifo0MsgPendingCallback
   */
}

/**
  * @brief  接收 FIFO 0 满回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未引用的形参导致编译警告 */
  UNUSED(hcan);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现
            HAL_CAN_RxFifo0FullCallback
   */
}

/**
  * @brief  接收 FIFO 1 消息挂起（待处理）回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未引用的形参导致编译警告 */
  UNUSED(hcan);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现
            HAL_CAN_RxFifo1MsgPendingCallback
   */
}

/**
  * @brief  接收 FIFO 1 满回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未引用的形参导致编译警告 */
  UNUSED(hcan);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            可以在用户文件中重新实现
            HAL_CAN_RxFifo1FullCallback
   */
}

/**
  * @brief  休眠回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_SleepCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未引用的形参导致编译警告 */
  UNUSED(hcan);

  /* 注意：请勿修改此函数。当需要该回调函数时，
            可以在用户文件中重新实现 HAL_CAN_SleepCallback
   */
}

/**
  * @brief  接收消息唤醒回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_WakeUpFromRxMsgCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);

  /* 注意：请勿修改此函数。当需要该回调函数时，
            可以在用户文件中重新实现
            HAL_CAN_WakeUpFromRxMsgCallback
   */
}

/**
  * @brief  CAN 错误回调函数。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hcan);

  /* 注意：请勿修改此函数。当需要该回调函数时，
            可以在用户文件中重新实现 HAL_CAN_ErrorCallback
   */
}

/**
  * @}
  */

/** @defgroup CAN_Exported_Functions_Group6 外设状态和错误函数
  * @brief   CAN 外设状态函数
  *
@verbatim
  ==============================================================================
            ##### 外设状态和错误函数 #####
  ==============================================================================
    [..]
    本小节提供的函数用于：
      (+) HAL_CAN_GetState()  : 返回 CAN 状态。
      (+) HAL_CAN_GetError()  : 返回 CAN 错误代码（如果有）。
      (+) HAL_CAN_ResetError(): 复位 CAN 错误代码（如果有）。

@endverbatim
  * @{
  */

/**
  * @brief  返回 CAN 状态。
  * @param  hcan 指向包含指定 CAN 配置信息的
  *         CAN_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_CAN_StateTypeDef HAL_CAN_GetState(const CAN_HandleTypeDef *hcan)
{
  HAL_CAN_StateTypeDef state = hcan->State;

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 检查睡眠模式应答标志 */
    if ((hcan->Instance->MSR & CAN_MSR_SLAK) != 0U)
    {
      /* 睡眠模式处于激活状态 */
      state = HAL_CAN_STATE_SLEEP_ACTIVE;
    }
    /* 检查睡眠模式请求标志 */
    else if ((hcan->Instance->MCR & CAN_MCR_SLEEP) != 0U)
    {
      /* 睡眠模式请求挂起中 */
      state = HAL_CAN_STATE_SLEEP_PENDING;
    }
    else
    {
      /* 既无睡眠模式请求也无睡眠模式应答 */
    }
  }

  /* 返回 CAN 状态 */
  return state;
}

/**
  * @brief  返回 CAN 错误代码。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构的指针。
  * @retval CAN 错误代码
  */
uint32_t HAL_CAN_GetError(const CAN_HandleTypeDef *hcan)
{
  /* 返回 CAN 错误代码 */
  return hcan->ErrorCode;
}

/**
  * @brief  复位 CAN 错误代码。
  * @param  hcan 指向包含指定 CAN 配置信息的 CAN_HandleTypeDef 结构的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_CAN_ResetError(CAN_HandleTypeDef *hcan)
{
  HAL_StatusTypeDef status = HAL_OK;
  HAL_CAN_StateTypeDef state = hcan->State;

  if ((state == HAL_CAN_STATE_READY) ||
      (state == HAL_CAN_STATE_LISTENING))
  {
    /* 复位 CAN 错误代码 */
    hcan->ErrorCode = 0U;
  }
  else
  {
    /* 更新错误代码 */
    hcan->ErrorCode |= HAL_CAN_ERROR_NOT_INITIALIZED;

    status = HAL_ERROR;
  }

  /* 返回状态 */
  return status;
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_CAN_MODULE_ENABLED */

/**
  * @}
  */

#endif /* CAN1 */

/**
  * @}
  */

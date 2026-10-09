/**
  ******************************************************************************
  * @file    stm32f1xx_hal_uart.c
  * @author  MCD Application Team
  * @brief   UART HAL 模块驱动程序。
  *          本文件提供固件函数以管理通用异步收发器（UART）外设的以下功能：
  *           + 初始化与反初始化函数
  *           + I/O 操作函数
  *           + 外设控制函数
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
                        ##### 如何使用此驱动程序 #####
  ==============================================================================
  [..]
    UART HAL 驱动程序的使用方法如下：

    (#) 声明一个 UART_HandleTypeDef 句柄结构体（例如：UART_HandleTypeDef huart）。
    (#) 通过实现 HAL_UART_MspInit() API 来初始化 UART 底层资源：
        (##) 使能 USARTx 接口时钟。
        (##) UART 引脚配置：
            (+++) 使能 UART GPIO 时钟。
            (+++) 将 UART TX/RX 引脚配置为复用推挽/浮空或上拉。
        (##) 若需使用中断方式（HAL_UART_Transmit_IT() 和 HAL_UART_Receive_IT() API），需配置 NVIC：
            (+++) 配置 USARTx 中断优先级。
            (+++) 使能 NVIC USART IRQ 中断通道。
        (##) 若需使用 DMA 方式（HAL_UART_Transmit_DMA() 和 HAL_UART_Receive_DMA() API），需配置 DMA：
            (+++) 为 Tx/Rx 通道声明 DMA 句柄结构体。
            (+++) 使能 DMAx 接口时钟。
            (+++) 使用所需的 Tx/Rx 参数配置已声明的 DMA 句柄结构体。
            (+++) 配置 DMA Tx/Rx 通道。
            (+++) 将初始化后的 DMA 句柄关联至 UART DMA Tx/Rx 句柄。
            (+++) 配置 DMA Tx/Rx 通道传输完成中断的优先级并使能 NVIC。
            (+++) 配置 USARTx 中断优先级并使能 NVIC USART IRQ 中断通道
                  （用于在 DMA 非循环模式下检测最后一个字节的发送完成）

    (#) 在 huart 的 Init 结构体中配置波特率（Baud Rate）、字长（Word Length）、停止位（Stop Bit）、
        校验位（Parity）、硬件流控（Hardware flow control）和模式（Receiver/Transmitter）。

    (#) 对于 UART 异步模式，调用 HAL_UART_Init() API 初始化 UART 寄存器。

    (#) 对于 UART 半双工模式，调用 HAL_HalfDuplex_Init() API 初始化 UART 寄存器。

    (#) 对于 LIN 模式，调用 HAL_LIN_Init() API 初始化 UART 寄存器。

    (#) 对于多处理器模式，调用 HAL_MultiProcessor_Init() API 初始化 UART 寄存器。

     [..]
       (@) 特定的 UART 中断（发送完成中断、RXNE 中断和错误中断）将在发送和接收处理过程中
           通过宏 __HAL_UART_ENABLE_IT() 和 __HAL_UART_DISABLE_IT() 进行管理。

     [..]
       (@) 这些 API（HAL_UART_Init() 和 HAL_HalfDuplex_Init()）还会通过调用用户自定义的
           HAL_UART_MspInit() API 来配置底层硬件（GPIO、时钟、CORTEX 等）。

    ##### 回调函数注册 #####
    ==================================

    [..]
    当编译宏 USE_HAL_UART_REGISTER_CALLBACKS 设置为 1 时，
    允许用户动态配置驱动程序的回调函数。

    [..]
    使用 HAL_UART_RegisterCallback() 函数注册用户回调函数。
    HAL_UART_RegisterCallback() 函数允许注册以下回调函数：
    (+) TxHalfCpltCallback        : 发送过半完成回调函数。
    (+) TxCpltCallback            : 发送完成回调函数。
    (+) RxHalfCpltCallback        : 接收过半完成回调函数。
    (+) RxCpltCallback            : 接收完成回调函数。
    (+) ErrorCallback             : 错误回调函数。
    (+) AbortCpltCallback         : 中止完成回调函数。
    (+) AbortTransmitCpltCallback : 发送中止完成回调函数。
    (+) AbortReceiveCpltCallback  : 接收中止完成回调函数。
    (+) MspInitCallback           : UART MspInit。
    (+) MspDeInitCallback         : UART MspDeInit。
    该函数接收 HAL 外设句柄、回调函数 ID 以及指向用户回调函数的指针作为参数。

    [..]
    使用 HAL_UART_UnRegisterCallback() 函数将回调函数重置为默认的 weak（弱定义覆写）函数。
    HAL_UART_UnRegisterCallback() 接收 HAL 外设句柄和回调函数 ID 作为参数。
    该函数允许重置以下回调函数：
    (+) TxHalfCpltCallback        : 发送过半完成回调函数。
    (+) TxCpltCallback            : 发送完成回调函数。
    (+) RxHalfCpltCallback        : 接收过半完成回调函数。
    (+) RxCpltCallback            : 接收完成回调函数。
    (+) ErrorCallback             : 错误回调函数。
    (+) AbortCpltCallback         : 中止完成回调函数。
    (+) AbortTransmitCpltCallback : 发送中止完成回调函数。
    (+) AbortReceiveCpltCallback  : 接收中止完成回调函数。
    (+) MspInitCallback           : UART MspInit。
    (+) MspDeInitCallback         : UART MspDeInit。

    [..]
    对于特定的回调函数 RxEventCallback，使用专用的注册/注销函数：
    分别为 HAL_UART_RegisterRxEventCallback() 和 HAL_UART_UnRegisterRxEventCallback()。

    [..]
    默认情况下，在执行 HAL_UART_Init() 之后且状态为 HAL_UART_STATE_RESET 时，
    所有回调函数都会被设置为对应的 weak（弱定义覆写）函数：
    例如 HAL_UART_TxCpltCallback()、HAL_UART_RxHalfCpltCallback()。
    但 MspInit 和 MspDeInit 函数例外，仅当这两个回调函数为空（未预先注册）时，
    它们才会在 HAL_UART_Init() 和 HAL_UART_DeInit() 中分别被重置为原有的 weak（弱定义覆写）函数。
    如果 MspInit 或 MspDeInit 不为空，HAL_UART_Init() 和 HAL_UART_DeInit() 将保留并使用预先注册的用户 MspInit/MspDeInit 回调函数。

    [..]
    回调函数仅能在 HAL_UART_STATE_READY 状态下进行注册/注销。
    但 MspInit/MspDeInit 例外，它们可以在 HAL_UART_STATE_READY 或 HAL_UART_STATE_RESET 状态下注册/注销，
    因此注册的（用户）MspInit/DeInit 回调函数可以在 Init/DeInit 过程中使用。
    在此情况下，需在调用 HAL_UART_DeInit() 或 HAL_UART_Init() 函数之前，
    先使用 HAL_UART_RegisterCallback() 注册 MspInit/MspDeInit 用户回调函数。

    [..]
    当编译宏 USE_HAL_UART_REGISTER_CALLBACKS 设置为 0 或未定义时，
    回调函数注册功能不可用，驱动将使用 weak（弱定义覆写）回调函数。

     [..]
        本驱动程序提供三种操作模式：

     *** 轮询模式 I/O 操作 ***
     =================================
     [..]
       (+) 使用 HAL_UART_Transmit() 在阻塞模式下发送一定量的数据
       (+) 使用 HAL_UART_Receive() 在阻塞模式下接收一定量的数据

     *** 中断模式 I/O 操作 ***
     ===================================
     [..]
       (+) 使用 HAL_UART_Transmit_IT() 在非阻塞模式下发送一定量的数据
       (+) 发送传输结束时将执行 HAL_UART_TxCpltCallback，用户可通过自定义函数指针 HAL_UART_TxCpltCallback 添加自己的代码
       (+) 使用 HAL_UART_Receive_IT() 在非阻塞模式下接收一定量的数据
       (+) 接收传输结束时将执行 HAL_UART_RxCpltCallback，用户可通过自定义函数指针 HAL_UART_RxCpltCallback 添加自己的代码
       (+) 若发生传输错误，将执行 HAL_UART_ErrorCallback() 函数，用户可通过自定义函数指针 HAL_UART_ErrorCallback 添加自己的代码

     *** DMA 模式 I/O 操作 ***
     ==============================
     [..]
       (+) 使用 HAL_UART_Transmit_DMA() 在非阻塞（DMA）模式下发送一定量的数据
       (+) 发送过半传输结束时将执行 HAL_UART_TxHalfCpltCallback，用户可通过自定义函数指针 HAL_UART_TxHalfCpltCallback 添加自己的代码
       (+) 发送传输结束时将执行 HAL_UART_TxCpltCallback，用户可通过自定义函数指针 HAL_UART_TxCpltCallback 添加自己的代码
       (+) 使用 HAL_UART_Receive_DMA() 在非阻塞（DMA）模式下接收一定量的数据
       (+) 接收过半传输结束时将执行 HAL_UART_RxHalfCpltCallback，用户可通过自定义函数指针 HAL_UART_RxHalfCpltCallback 添加自己的代码
       (+) 接收传输结束时将执行 HAL_UART_RxCpltCallback，用户可通过自定义函数指针 HAL_UART_RxCpltCallback 添加自己的代码
       (+) 若发生传输错误，将执行 HAL_UART_ErrorCallback() 函数，用户可通过自定义函数指针 HAL_UART_ErrorCallback 添加自己的代码
       (+) 使用 HAL_UART_DMAPause() 暂停 DMA 传输
       (+) 使用 HAL_UART_DMAResume() 恢复 DMA 传输
       (+) 使用 HAL_UART_DMAStop() 停止 DMA 传输


    [..] 本小节还提供了一组附加函数，为用户提供增强的接收服务。（例如，这些函数允许应用程序处理待接收数据量未知的应用场景）。

    (#) 与仅将接收到的数据元素数量作为接收完成条件的标准接收服务相比，
        这些函数还考虑将附加事件作为向调用者更新接收状态的触发条件：
       (+) 检测空闲周期（RX 线路在给定时间内无活动）。
          (++) 通过 IDLE 事件检测 RX 空闲，即在接收到最后一个字节后，
               RX 线路处于空闲状态（通常为高电平）达 1 个帧周期。

    (#) 共有两种传输模式：
       (+) 阻塞模式：以轮询模式执行接收，直到接收到预期数量的数据，
           或发生 IDLE 事件。仅在函数执行期间处理接收。
           当函数退出后，将不会再接收数据。函数在传输结束后返回 HAL 状态和实际接收到的数据元素数量。
       (+) 非阻塞模式：使用中断或 DMA 执行接收。
           这些 API 返回 HAL 状态。
           使用中断模式时将通过专用的 UART IRQ 指示数据处理结束，
           使用 DMA 模式时则通过 DMA IRQ 指示。
           在接收处理期间将执行 HAL_UARTEx_RxEventCallback() 用户回调函数。
           当检测到接收错误时将执行 HAL_UART_ErrorCallback() 用户回调函数。

    (#) 阻塞模式 API：
        (+) HAL_UARTEx_ReceiveToIdle()

    (#) 中断方式的非阻塞模式 API：
        (+) HAL_UARTEx_ReceiveToIdle_IT()

    (#) DMA 方式的非阻塞模式 API：
        (+) HAL_UARTEx_ReceiveToIdle_DMA()


     *** UART HAL 驱动常用宏列表 ***
     =============================================
     [..]
       以下是 UART HAL 驱动程序中最常用的宏列表。

      (+) __HAL_UART_ENABLE: 使能 UART 外设
      (+) __HAL_UART_DISABLE: 禁用 UART 外设
      (+) __HAL_UART_GET_FLAG : 检查指定的 UART 标志位是否置位
      (+) __HAL_UART_CLEAR_FLAG : 清除指定的 UART 挂起标志位
      (+) __HAL_UART_ENABLE_IT: 使能指定的 UART 中断
      (+) __HAL_UART_DISABLE_IT: 禁用指定的 UART 中断
      (+) __HAL_UART_GET_IT_SOURCE: 检查指定的 UART 中断源是否产生中断

     [..]
       (@) 您可以参考 UART HAL 驱动头文件以获取更多有用的宏定义

  @endverbatim
     [..]
       (@) 补充说明：如果使能了校验位，则写入数据寄存器的数据的最高有效位（MSB）
           仍会发送，但会被校验位所替换。
           根据 M 位定义的帧长度（8 位或 9 位），可能存在的 UART 帧格式如下表所示：
    +-------------------------------------------------------------+
    |   M 位  |  PCE 位   |            UART 帧结构                    |
    |---------|-----------|---------------------------------------|
    |    0    |    0      |    | 起始位 | 8位数据 | 停止位 |         |
    |---------|-----------|---------------------------------------|
    |    0    |    1      |    | 起始位 | 7位数据 | 校验位 | 停止位 |  |
    |---------|-----------|---------------------------------------|
    |    1    |    0      |    | 起始位 | 9位数据 | 停止位 |         |
    |---------|-----------|---------------------------------------|
    |    1    |    1      |    | 起始位 | 8位数据 | 校验位 | 停止位 |  |
    +-------------------------------------------------------------+
  ******************************************************************************
  */

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup UART UART
  * @brief HAL UART 模块驱动程序
  * @{
  */
#ifdef HAL_UART_MODULE_ENABLED

/* 私有类型定义 -----------------------------------------------------------*/
/* 私有宏定义 ------------------------------------------------------------*/
/** @addtogroup UART_Private_Constants
  * @{
  */
/**
  * @}
  */
/* 私有宏 -----------------------------------------------------------------*/
/* 私有变量 -------------------------------------------------------------*/
/* 私有函数原型 -----------------------------------------------------------*/
/** @addtogroup UART_Private_Functions  UART 私有函数
  * @{
  */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
void UART_InitCallbacksToDefault(UART_HandleTypeDef *huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
static void UART_EndTxTransfer(UART_HandleTypeDef *huart);
static void UART_EndRxTransfer(UART_HandleTypeDef *huart);
static void UART_DMATransmitCplt(DMA_HandleTypeDef *hdma);
static void UART_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
static void UART_DMATxHalfCplt(DMA_HandleTypeDef *hdma);
static void UART_DMARxHalfCplt(DMA_HandleTypeDef *hdma);
static void UART_DMAError(DMA_HandleTypeDef *hdma);
static void UART_DMAAbortOnError(DMA_HandleTypeDef *hdma);
static void UART_DMATxAbortCallback(DMA_HandleTypeDef *hdma);
static void UART_DMARxAbortCallback(DMA_HandleTypeDef *hdma);
static void UART_DMATxOnlyAbortCallback(DMA_HandleTypeDef *hdma);
static void UART_DMARxOnlyAbortCallback(DMA_HandleTypeDef *hdma);
static HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart);
static HAL_StatusTypeDef UART_EndTransmit_IT(UART_HandleTypeDef *huart);
static HAL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart);
static HAL_StatusTypeDef UART_WaitOnFlagUntilTimeout(UART_HandleTypeDef *huart, uint32_t Flag, FlagStatus Status,
                                                     uint32_t Tickstart, uint32_t Timeout);
static void UART_SetConfig(UART_HandleTypeDef *huart);

/**
  * @}
  */

/* 导出函数 ---------------------------------------------------------*/
/** @defgroup UART_Exported_Functions UART 导出函数
  * @{
  */

/** @defgroup UART_Exported_Functions_Group1 初始化和反初始化函数
  *  @brief    初始化与配置函数
  *
@verbatim
 ===============================================================================
            ##### 初始化与配置函数 #####
 ===============================================================================
    [..]
    本节提供了一组函数，用于将 USARTx 或 UARTy 初始化为异步模式。
      (+) 对于异步模式，仅可配置以下参数：
        (++) 波特率 (Baud Rate)
        (++) 字长 (Word Length)
        (++) 停止位 (Stop Bit)
        (++) 奇偶校验位 (Parity)：如果使能了奇偶校验，则写入数据寄存器的数据最高位 (MSB)
             会被传输，但会被校验位替换。
             根据 M 位定义的帧长度（8 位或 9 位），可能支持的 UART 帧格式请参考参考手册。
        (++) 硬件流控制 (Hardware flow control)
        (++) 接收器/发送器模式 (Receiver/transmitter modes)
        (++) 过采样方法 (Over Sampling Method)
    [..]
    HAL_UART_Init()、HAL_HalfDuplex_Init()、HAL_LIN_Init() 和 HAL_MultiProcessor_Init() API
    分别遵循 UART 异步、UART 半双工、LIN 和多处理器配置流程（流程详情可参阅参考手册
    （STM32F10Xxx 系列 MCU 参阅 RM0008，STM32F100xx 系列 MCU 参阅 RM0041））。

@endverbatim
  * @{
  */

/**
  * @brief  根据 UART_InitTypeDef 中指定的参数初始化 UART 模式，
  *         并创建关联的句柄。
  * @param  huart  指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart)
{
  /* 检查 UART 句柄是否已分配 */
  if (huart == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数有效性 */
  if (huart->Init.HwFlowCtl != UART_HWCONTROL_NONE)
  {
    /* 硬件流控制仅适用于 USART1、USART2 和 USART3 */
    assert_param(IS_UART_HWFLOW_INSTANCE(huart->Instance));
    assert_param(IS_UART_HARDWARE_FLOW_CONTROL(huart->Init.HwFlowCtl));
  }
  else
  {
    assert_param(IS_UART_INSTANCE(huart->Instance));
  }
  assert_param(IS_UART_WORD_LENGTH(huart->Init.WordLength));
#if defined(USART_CR1_OVER8)
  assert_param(IS_UART_OVERSAMPLING(huart->Init.OverSampling));
#endif /* USART_CR1_OVER8 */

  if (huart->gState == HAL_UART_STATE_RESET)
  {
    /* 分配锁资源并进行初始化 */
    huart->Lock = HAL_UNLOCKED;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    UART_InitCallbacksToDefault(huart);

    if (huart->MspInitCallback == NULL)
    {
      huart->MspInitCallback = HAL_UART_MspInit;
    }

    /* 初始化底层硬件 */
    huart->MspInitCallback(huart);
#else
    /* 初始化底层硬件：GPIO、时钟 */
    HAL_UART_MspInit(huart);
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS) */
  }

  huart->gState = HAL_UART_STATE_BUSY;

  /* 禁用外设 */
  __HAL_UART_DISABLE(huart);

  /* 配置 UART 通信参数 */
  UART_SetConfig(huart);

  /* 在异步模式下，必须保持以下位清零：
     - USART_CR2 寄存器中的 LINEN 和 CLKEN 位，
     - USART_CR3 寄存器中的 SCEN、HDSEL 和 IREN 位。*/
  CLEAR_BIT(huart->Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
  CLEAR_BIT(huart->Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

  /* 使能外设 */
  __HAL_UART_ENABLE(huart);

  /* 初始化 UART 状态 */
  huart->ErrorCode = HAL_UART_ERROR_NONE;
  huart->gState = HAL_UART_STATE_READY;
  huart->RxState = HAL_UART_STATE_READY;
  huart->RxEventType = HAL_UART_RXEVENT_TC;

  return HAL_OK;
}

/**
  * @brief  根据 UART_InitTypeDef 中指定的参数初始化半双工模式，
  *         并创建关联的句柄。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_HalfDuplex_Init(UART_HandleTypeDef *huart)
{
  /* 检查 UART 句柄分配 */
  if (huart == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_UART_HALFDUPLEX_INSTANCE(huart->Instance));
  assert_param(IS_UART_WORD_LENGTH(huart->Init.WordLength));
#if defined(USART_CR1_OVER8)
  assert_param(IS_UART_OVERSAMPLING(huart->Init.OverSampling));
#endif /* USART_CR1_OVER8 */

  if (huart->gState == HAL_UART_STATE_RESET)
  {
    /* 分配互斥锁资源并初始化 */
    huart->Lock = HAL_UNLOCKED;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    UART_InitCallbacksToDefault(huart);

    if (huart->MspInitCallback == NULL)
    {
      huart->MspInitCallback = HAL_UART_MspInit;
    }

    /* 初始化底层硬件 */
    huart->MspInitCallback(huart);
#else
    /* 初始化底层硬件：GPIO、时钟 */
    HAL_UART_MspInit(huart);
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS) */
  }

  huart->gState = HAL_UART_STATE_BUSY;

  /* 禁用外设 */
  __HAL_UART_DISABLE(huart);

  /* 设置 UART 通信参数 */
  UART_SetConfig(huart);

  /* 在半双工模式下，以下位必须保持清零：
     - USART_CR2 寄存器中的 LINEN 和 CLKEN 位，
     - USART_CR3 寄存器中的 SCEN 和 IREN 位。*/
  CLEAR_BIT(huart->Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
  CLEAR_BIT(huart->Instance->CR3, (USART_CR3_IREN | USART_CR3_SCEN));

  /* 通过置位 CR3 寄存器中的 HDSEL 位以使能半双工模式 */
  SET_BIT(huart->Instance->CR3, USART_CR3_HDSEL);

  /* 使能外设 */
  __HAL_UART_ENABLE(huart);

  /* 初始化 UART 状态*/
  huart->ErrorCode = HAL_UART_ERROR_NONE;
  huart->gState = HAL_UART_STATE_READY;
  huart->RxState = HAL_UART_STATE_READY;
  huart->RxEventType = HAL_UART_RXEVENT_TC;

  return HAL_OK;
}

/**
  * @brief  根据 UART_InitTypeDef 中指定的参数初始化 LIN 模式，
  *         并创建关联的句柄。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @param  BreakDetectLength 指定 LIN 断开符号检测长度。
  *         该参数可以是以下值之一：
  *            @arg UART_LINBREAKDETECTLENGTH_10B: 10 位断开符号检测
  *            @arg UART_LINBREAKDETECTLENGTH_11B: 11 位断开符号检测
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_LIN_Init(UART_HandleTypeDef *huart, uint32_t BreakDetectLength)
{
  /* 检查 UART 句柄分配有效性 */
  if (huart == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查 LIN UART 外设实例 */
  assert_param(IS_UART_LIN_INSTANCE(huart->Instance));

  /* 检查断开符号检测长度参数 */
  assert_param(IS_UART_LIN_BREAK_DETECT_LENGTH(BreakDetectLength));
  assert_param(IS_UART_LIN_WORD_LENGTH(huart->Init.WordLength));
#if defined(USART_CR1_OVER8)
  assert_param(IS_UART_LIN_OVERSAMPLING(huart->Init.OverSampling));
#endif /* USART_CR1_OVER8 */

  if (huart->gState == HAL_UART_STATE_RESET)
  {
    /* 分配锁资源并进行初始化 */
    huart->Lock = HAL_UNLOCKED;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    UART_InitCallbacksToDefault(huart);

    if (huart->MspInitCallback == NULL)
    {
      huart->MspInitCallback = HAL_UART_MspInit;
    }

    /* 初始化底层硬件 */
    huart->MspInitCallback(huart);
#else
    /* 初始化底层硬件：GPIO、时钟 */
    HAL_UART_MspInit(huart);
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS) */
  }

  huart->gState = HAL_UART_STATE_BUSY;

  /* 禁用外设 */
  __HAL_UART_DISABLE(huart);

  /* 设置 UART 通信参数 */
  UART_SetConfig(huart);

  /* 在 LIN 模式下，以下位必须保持清零：
     - USART_CR2 寄存器中的 CLKEN 位，
     - USART_CR3 寄存器中的 SCEN、HDSEL 和 IREN 位。*/
  CLEAR_BIT(huart->Instance->CR2, (USART_CR2_CLKEN));
  CLEAR_BIT(huart->Instance->CR3, (USART_CR3_HDSEL | USART_CR3_IREN | USART_CR3_SCEN));

  /* 通过置位 CR2 寄存器中的 LINEN 位来使能 LIN 模式 */
  SET_BIT(huart->Instance->CR2, USART_CR2_LINEN);

  /* 设置 USART LIN 断开符号检测长度。*/
  CLEAR_BIT(huart->Instance->CR2, USART_CR2_LBDL);
  SET_BIT(huart->Instance->CR2, BreakDetectLength);

  /* 使能外设 */
  __HAL_UART_ENABLE(huart);

  /* 初始化 UART 状态 */
  huart->ErrorCode = HAL_UART_ERROR_NONE;
  huart->gState = HAL_UART_STATE_READY;
  huart->RxState = HAL_UART_STATE_READY;
  huart->RxEventType = HAL_UART_RXEVENT_TC;

  return HAL_OK;
}

/**
  * @brief  根据 UART_InitTypeDef 中指定的参数初始化多处理器模式，
  *         并创建关联的句柄。
  * @param  huart 指向 UART_HandleTypeDef 结构的指针，该结构包含
  *                指定 UART 模块的配置信息。
  * @param  Address USART 地址
  * @param  WakeUpMethod 指定 USART 唤醒方式。
  *         该参数可以是以下值之一：
  *            @arg UART_WAKEUPMETHOD_IDLELINE: 空闲总线检测唤醒
  *            @arg UART_WAKEUPMETHOD_ADDRESSMARK: 地址标记唤醒
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_MultiProcessor_Init(UART_HandleTypeDef *huart, uint8_t Address, uint32_t WakeUpMethod)
{
  /* 检查 UART 句柄分配 */
  if (huart == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_UART_INSTANCE(huart->Instance));

  /* 检查地址和唤醒方式参数 */
  assert_param(IS_UART_WAKEUPMETHOD(WakeUpMethod));
  assert_param(IS_UART_ADDRESS(Address));
  assert_param(IS_UART_WORD_LENGTH(huart->Init.WordLength));
#if defined(USART_CR1_OVER8)
  assert_param(IS_UART_OVERSAMPLING(huart->Init.OverSampling));
#endif /* USART_CR1_OVER8 */

  if (huart->gState == HAL_UART_STATE_RESET)
  {
    /* 分配互斥锁资源并进行初始化 */
    huart->Lock = HAL_UNLOCKED;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    UART_InitCallbacksToDefault(huart);

    if (huart->MspInitCallback == NULL)
    {
      huart->MspInitCallback = HAL_UART_MspInit;
    }

    /* 初始化底层硬件 */
    huart->MspInitCallback(huart);
#else
    /* 初始化底层硬件：GPIO、时钟 */
    HAL_UART_MspInit(huart);
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS) */
  }

  huart->gState = HAL_UART_STATE_BUSY;

  /* 禁用外设 */
  __HAL_UART_DISABLE(huart);

  /* 设置 UART 通信参数 */
  UART_SetConfig(huart);

  /* 在多处理器模式下，必须保持以下位为清除状态：
     - USART_CR2 寄存器中的 LINEN 和 CLKEN 位，
     - USART_CR3 寄存器中的 SCEN、HDSEL 和 IREN 位 */
  CLEAR_BIT(huart->Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
  CLEAR_BIT(huart->Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

  /* 设置 USART 节点地址 */
  CLEAR_BIT(huart->Instance->CR2, USART_CR2_ADD);
  SET_BIT(huart->Instance->CR2, Address);

  /* 通过设置 CR1 寄存器中的 WAKE 位来配置唤醒方式 */
  CLEAR_BIT(huart->Instance->CR1, USART_CR1_WAKE);
  SET_BIT(huart->Instance->CR1, WakeUpMethod);

  /* 使能外设 */
  __HAL_UART_ENABLE(huart);

  /* 初始化 UART 状态 */
  huart->ErrorCode = HAL_UART_ERROR_NONE;
  huart->gState = HAL_UART_STATE_READY;
  huart->RxState = HAL_UART_STATE_READY;
  huart->RxEventType = HAL_UART_RXEVENT_TC;

  return HAL_OK;
}

/**
  * @brief  反初始化 UART 外设。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef *huart)
{
  /* 检查 UART 句柄内存分配 */
  if (huart == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数有效性 */
  assert_param(IS_UART_INSTANCE(huart->Instance));

  huart->gState = HAL_UART_STATE_BUSY;

  /* 禁用外设 */
  __HAL_UART_DISABLE(huart);

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  if (huart->MspDeInitCallback == NULL)
  {
    huart->MspDeInitCallback = HAL_UART_MspDeInit;
  }
  /* 反初始化底层硬件 */
  huart->MspDeInitCallback(huart);
#else
  /* 反初始化底层硬件 */
  HAL_UART_MspDeInit(huart);
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS) */

  huart->ErrorCode = HAL_UART_ERROR_NONE;
  huart->gState = HAL_UART_STATE_RESET;
  huart->RxState = HAL_UART_STATE_RESET;
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;
  huart->RxEventType = HAL_UART_RXEVENT_TC;

  /* 进程解锁 */
  __HAL_UNLOCK(huart);

  return HAL_OK;
}

/**
  * @brief  UART MSP 初始化。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(huart);
  /* 注意: 请勿修改此函数，当需要使用该回调函数时，
           可在用户文件中重新实现 HAL_UART_MspInit
   */
}

/**
  * @brief  UART MSP 反初始化。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(huart);
  /* 注意：不应修改此函数，当需要该回调函数时，
           可在用户文件中实现 HAL_UART_MspDeInit
   */
}

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
/**
  * @brief  注册用户 UART 回调函数
  *         用于替代预定义的弱函数 (weak) 回调
  * @note   HAL_UART_RegisterCallback() 可以在 HAL_UART_Init()、HAL_HalfDuplex_Init()、HAL_LIN_Init()、
  *         HAL_MultiProcessor_Init() 之前调用，以注册 HAL_UART_MSPINIT_CB_ID 和 HAL_UART_MSPDEINIT_CB_ID 的回调函数
  * @param  huart UART 句柄
  * @param  CallbackID 要注册的回调函数 ID
  *         该参数可以是以下值之一：
  *           @arg @ref HAL_UART_TX_HALFCOMPLETE_CB_ID 发送半完成回调 ID
  *           @arg @ref HAL_UART_TX_COMPLETE_CB_ID 发送完成回调 ID
  *           @arg @ref HAL_UART_RX_HALFCOMPLETE_CB_ID 接收半完成回调 ID
  *           @arg @ref HAL_UART_RX_COMPLETE_CB_ID 接收完成回调 ID
  *           @arg @ref HAL_UART_ERROR_CB_ID 错误回调 ID
  *           @arg @ref HAL_UART_ABORT_COMPLETE_CB_ID 中止完成回调 ID
  *           @arg @ref HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID 发送中止完成回调 ID
  *           @arg @ref HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID 接收中止完成回调 ID
  *           @arg @ref HAL_UART_MSPINIT_CB_ID MspInit 回调 ID
  *           @arg @ref HAL_UART_MSPDEINIT_CB_ID MspDeInit 回调 ID
  * @param  pCallback 指向回调函数的指针
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_RegisterCallback(UART_HandleTypeDef *huart, HAL_UART_CallbackIDTypeDef CallbackID,
                                            pUART_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    /* 更新错误代码 */
    huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }

  if (huart->gState == HAL_UART_STATE_READY)
  {
    switch (CallbackID)
    {
      case HAL_UART_TX_HALFCOMPLETE_CB_ID :
        huart->TxHalfCpltCallback = pCallback;
        break;

      case HAL_UART_TX_COMPLETE_CB_ID :
        huart->TxCpltCallback = pCallback;
        break;

      case HAL_UART_RX_HALFCOMPLETE_CB_ID :
        huart->RxHalfCpltCallback = pCallback;
        break;

      case HAL_UART_RX_COMPLETE_CB_ID :
        huart->RxCpltCallback = pCallback;
        break;

      case HAL_UART_ERROR_CB_ID :
        huart->ErrorCallback = pCallback;
        break;

      case HAL_UART_ABORT_COMPLETE_CB_ID :
        huart->AbortCpltCallback = pCallback;
        break;

      case HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID :
        huart->AbortTransmitCpltCallback = pCallback;
        break;

      case HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID :
        huart->AbortReceiveCpltCallback = pCallback;
        break;

      case HAL_UART_MSPINIT_CB_ID :
        huart->MspInitCallback = pCallback;
        break;

      case HAL_UART_MSPDEINIT_CB_ID :
        huart->MspDeInitCallback = pCallback;
        break;

      default :
        /* 更新错误代码 */
        huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (huart->gState == HAL_UART_STATE_RESET)
  {
    switch (CallbackID)
    {
      case HAL_UART_MSPINIT_CB_ID :
        huart->MspInitCallback = pCallback;
        break;

      case HAL_UART_MSPDEINIT_CB_ID :
        huart->MspDeInitCallback = pCallback;
        break;

      default :
        /* 更新错误代码 */
        huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* 更新错误代码 */
    huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;

    /* 返回错误状态 */
    status =  HAL_ERROR;
  }

  return status;
}

/**
  * @brief  注销 UART 回调函数
  *         UART 回调函数将被重定向到预定义的弱函数 (weak) 回调
  * @note   HAL_UART_UnRegisterCallback() 可以在 HAL_UART_Init()、HAL_HalfDuplex_Init()、
  *         HAL_LIN_Init()、HAL_MultiProcessor_Init() 之前调用，以注销 HAL_UART_MSPINIT_CB_ID
  *         和 HAL_UART_MSPDEINIT_CB_ID 的回调函数
  * @param  huart UART 句柄
  * @param  CallbackID 要注销的回调函数 ID
  *         该参数可以是以下值之一：
  *           @arg @ref HAL_UART_TX_HALFCOMPLETE_CB_ID 发送半完成回调 ID
  *           @arg @ref HAL_UART_TX_COMPLETE_CB_ID 发送完成回调 ID
  *           @arg @ref HAL_UART_RX_HALFCOMPLETE_CB_ID 接收半完成回调 ID
  *           @arg @ref HAL_UART_RX_COMPLETE_CB_ID 接收完成回调 ID
  *           @arg @ref HAL_UART_ERROR_CB_ID 错误回调 ID
  *           @arg @ref HAL_UART_ABORT_COMPLETE_CB_ID 中止完成回调 ID
  *           @arg @ref HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID 发送中止完成回调 ID
  *           @arg @ref HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID 接收中止完成回调 ID
  *           @arg @ref HAL_UART_MSPINIT_CB_ID MspInit 回调 ID
  *           @arg @ref HAL_UART_MSPDEINIT_CB_ID MspDeInit 回调 ID
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_UnRegisterCallback(UART_HandleTypeDef *huart, HAL_UART_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (HAL_UART_STATE_READY == huart->gState)
  {
    switch (CallbackID)
    {
      case HAL_UART_TX_HALFCOMPLETE_CB_ID :
        huart->TxHalfCpltCallback = HAL_UART_TxHalfCpltCallback;               /* 传统的弱定义 TxHalfCpltCallback       */
        break;

      case HAL_UART_TX_COMPLETE_CB_ID :
        huart->TxCpltCallback = HAL_UART_TxCpltCallback;                       /* 传统的弱定义 TxCpltCallback            */
        break;

      case HAL_UART_RX_HALFCOMPLETE_CB_ID :
        huart->RxHalfCpltCallback = HAL_UART_RxHalfCpltCallback;               /* 传统的弱定义 RxHalfCpltCallback        */
        break;

      case HAL_UART_RX_COMPLETE_CB_ID :
        huart->RxCpltCallback = HAL_UART_RxCpltCallback;                       /* 传统的弱定义 RxCpltCallback            */
        break;

      case HAL_UART_ERROR_CB_ID :
        huart->ErrorCallback = HAL_UART_ErrorCallback;                         /* 传统的弱定义 ErrorCallback             */
        break;

      case HAL_UART_ABORT_COMPLETE_CB_ID :
        huart->AbortCpltCallback = HAL_UART_AbortCpltCallback;                 /* 遗留的弱实现 AbortCpltCallback         */
        break;

      case HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID :
        huart->AbortTransmitCpltCallback = HAL_UART_AbortTransmitCpltCallback; /* 遗留的弱实现 AbortTransmitCpltCallback */
        break;

      case HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID :
        huart->AbortReceiveCpltCallback = HAL_UART_AbortReceiveCpltCallback;   /* 遗留的弱实现 AbortReceiveCpltCallback  */
        break;

      case HAL_UART_MSPINIT_CB_ID :
        huart->MspInitCallback = HAL_UART_MspInit;                             /* 遗留的弱实现 MspInitCallback           */
        break;

      case HAL_UART_MSPDEINIT_CB_ID :
        huart->MspDeInitCallback = HAL_UART_MspDeInit;                         /* 遗留的弱实现 MspDeInitCallback         */
        break;

      default :
        /* 更新错误代码 */
        huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (HAL_UART_STATE_RESET == huart->gState)
  {
    switch (CallbackID)
    {
      case HAL_UART_MSPINIT_CB_ID :
        huart->MspInitCallback = HAL_UART_MspInit;
        break;

      case HAL_UART_MSPDEINIT_CB_ID :
        huart->MspDeInitCallback = HAL_UART_MspDeInit;
        break;

      default :
        /* 更新错误代码 */
        huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* 更新错误代码 */
    huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;

    /* 返回错误状态 */
    status =  HAL_ERROR;
  }

  return status;
}

/**
  * @brief  注册用户 UART 接收事件回调函数 (Rx Event Callback)
  *         用于替代预定义的弱实现回调函数
  * @param  huart     UART 句柄
  * @param  pCallback 指向接收事件回调函数的指针
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_RegisterRxEventCallback(UART_HandleTypeDef *huart, pUART_RxEventCallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }

  /* 进程已锁定 */
  __HAL_LOCK(huart);

  if (huart->gState == HAL_UART_STATE_READY)
  {
    huart->RxEventCallback = pCallback;
  }
  else
  {
    huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;

    status =  HAL_ERROR;
  }

  /* 释放锁 */
  __HAL_UNLOCK(huart);

  return status;
}

/**
  * @brief  注销 UART 接收事件回调函数 (Rx Event Callback)
  *         UART 接收事件回调函数将重定向恢复为预定义的弱实现 HAL_UARTEx_RxEventCallback() 回调函数
  * @param  huart     UART 句柄
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_UnRegisterRxEventCallback(UART_HandleTypeDef *huart)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 进程已锁定 */
  __HAL_LOCK(huart);

  if (huart->gState == HAL_UART_STATE_READY)
  {
    huart->RxEventCallback = HAL_UARTEx_RxEventCallback; /* 遗留的 UART 接收事件弱回调函数 */
  }
  else
  {
    huart->ErrorCode |= HAL_UART_ERROR_INVALID_CALLBACK;

    status =  HAL_ERROR;
  }

  /* 释放锁 */
  __HAL_UNLOCK(huart);
  return status;
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup UART_Exported_Functions_Group2 IO 操作函数
  *  @brief UART 发送和接收函数
  *
@verbatim
 ===============================================================================
                      ##### IO 操作函数 #####
 ===============================================================================
    本小节提供了一组用于管理 UART 异步和半双工数据传输的函数。

    (#) 共有两种传输模式：
       (+) 阻塞模式：通信以轮询方式进行。
           所有数据处理的 HAL 状态在传输完成后由同一个函数返回。
       (+) 非阻塞模式：通信通过中断或 DMA 进行，这些 API 会返回 HAL 状态。
           当使用中断模式时，数据处理的结束将通过专用的 UART IRQ 指示；
           当使用 DMA 模式时，将通过 DMA IRQ 指示。
           用户回调函数 HAL_UART_TxCpltCallback() 和 HAL_UART_RxCpltCallback()
           将分别在发送或接收过程结束时执行。
           当检测到通信错误时，将执行 HAL_UART_ErrorCallback() 用户回调函数。

    (#) 阻塞模式的 API 包括：
        (+) HAL_UART_Transmit()
        (+) HAL_UART_Receive()

    (#) 带中断的非阻塞模式 API 包括：
        (+) HAL_UART_Transmit_IT()
        (+) HAL_UART_Receive_IT()
        (+) HAL_UART_IRQHandler()

    (#) 带 DMA 的非阻塞模式 API 包括：
        (+) HAL_UART_Transmit_DMA()
        (+) HAL_UART_Receive_DMA()
        (+) HAL_UART_DMAPause()
        (+) HAL_UART_DMAResume()
        (+) HAL_UART_DMAStop()

    (#) 非阻塞模式下提供了一组传输完成回调函数：
        (+) HAL_UART_TxHalfCpltCallback()
        (+) HAL_UART_TxCpltCallback()
        (+) HAL_UART_RxHalfCpltCallback()
        (+) HAL_UART_RxCpltCallback()
        (+) HAL_UART_ErrorCallback()

    (#) 非阻塞模式传输可以使用中止 API 进行中止：
        (+) HAL_UART_Abort()
        (+) HAL_UART_AbortTransmit()
        (+) HAL_UART_AbortReceive()
        (+) HAL_UART_Abort_IT()
        (+) HAL_UART_AbortTransmit_IT()
        (+) HAL_UART_AbortReceive_IT()

    (#) 对于基于中断的中止服务 (HAL_UART_Abortxxx_IT)，提供了一组中止完成回调函数：
        (+) HAL_UART_AbortCpltCallback()
        (+) HAL_UART_AbortTransmitCpltCallback()
        (+) HAL_UART_AbortReceiveCpltCallback()

    (#) Rx 事件接收回调函数（Rx 事件通知）可用于增强型接收服务的非阻塞模式：
        (+) HAL_UARTEx_RxEventCallback()

    (#) 在非阻塞模式传输中，可能出现的错误分为两类。
        错误处理机制如下：
       (+) 错误被视为可恢复且非阻塞的：传输可以继续进行直到结束，但错误严重性需由用户评估：
           这涉及中断模式接收中的帧错误（Frame Error）、奇偶校验错误（Parity Error）或噪声错误（Noise Error）。
           随后接收到的字符会被取出并存入 Rx 缓冲区，设置错误代码以便用户识别错误类型，
           并执行 HAL_UART_ErrorCallback() 用户回调函数。UART 端的传输保持继续进行。
           如果用户希望中止传输，应由用户调用中止服务。
       (+) 错误被视为阻塞性的：传输无法正常完成并被中止。
           这涉及中断模式接收中的过载错误（Overrun Error）以及 DMA 模式下的所有错误。
           设置错误代码以便用户识别错误类型，并执行 HAL_UART_ErrorCallback() 用户回调函数。

    -@- 在半双工通信中，禁止并行运行发送和接收过程，
        UART 状态 HAL_UART_STATE_BUSY_TX_RX 不起作用。

@endverbatim
  * @{
  */

/**
  * @brief  以阻塞模式发送一定量的数据。
  * @note   当未使能 UART 奇偶校验 (PCE = 0) 且字长配置为 9 位 (M1-M0 = 01) 时，
  *         发送的数据按一组 u16 进行处理。在这种情况下，Size 必须指定
  *         通过 pData 提供的 u16 数量。
  * @param  huart 指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构的指针。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）。
  * @param  Size  要发送的数据元素数量（u8 或 u16）。
  * @param  Timeout 超时持续时间
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  const uint8_t  *pdata8bits;
  const uint16_t *pdata16bits;
  uint32_t tickstart = 0U;

  /* 检查当前是否未在进行 Tx 过程 */
  if (huart->gState == HAL_UART_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->gState = HAL_UART_STATE_BUSY_TX;

    /* 初始化 tickstart 用于超时管理 */
    tickstart = HAL_GetTick();

    huart->TxXferSize = Size;
    huart->TxXferCount = Size;

    /* 在 9 位/无奇偶校验传输的情况下，pData 需要作为 uint16_t 指针处理 */
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))
    {
      pdata8bits  = NULL;
      pdata16bits = (const uint16_t *) pData;
    }
    else
    {
      pdata8bits  = pData;
      pdata16bits = NULL;
    }

    while (huart->TxXferCount > 0U)
    {
      if (UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_TXE, RESET, tickstart, Timeout) != HAL_OK)
      {
        huart->gState = HAL_UART_STATE_READY;

        return HAL_TIMEOUT;
      }
      if (pdata8bits == NULL)
      {
        huart->Instance->DR = (uint16_t)(*pdata16bits & 0x01FFU);
        pdata16bits++;
      }
      else
      {
        huart->Instance->DR = (uint8_t)(*pdata8bits & 0xFFU);
        pdata8bits++;
      }
      huart->TxXferCount--;
    }

    if (UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_TC, RESET, tickstart, Timeout) != HAL_OK)
    {
      huart->gState = HAL_UART_STATE_READY;

      return HAL_TIMEOUT;
    }

    /* 在 Tx 过程结束时，将 huart->gState 恢复为 Ready */
    huart->gState = HAL_UART_STATE_READY;

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  以阻塞模式接收一定量的数据。
  * @note   当未使能 UART 奇偶校验 (PCE = 0) 且字长配置为 9 位 (M1-M0 = 01) 时，
  *         接收的数据按一组 u16 进行处理。在这种情况下，Size 必须指定
  *         通过 pData 可用的 u16 数量。
  * @param  huart 指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构的指针。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）。
  * @param  Size  要接收的数据元素数量（u8 或 u16）。
  * @param  Timeout 超时持续时间
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  uint8_t  *pdata8bits;
  uint16_t *pdata16bits;
  uint32_t tickstart = 0U;

  /* 检查当前是否未在进行 Rx 过程 */
  if (huart->RxState == HAL_UART_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->RxState = HAL_UART_STATE_BUSY_RX;
    huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

    /* 初始化 tickstart 用于超时管理 */
    tickstart = HAL_GetTick();

    huart->RxXferSize = Size;
    huart->RxXferCount = Size;

    /* 在 9 位/无奇偶校验传输的情况下，pRxData 需要作为 uint16_t 指针处理 */
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))
    {
      pdata8bits  = NULL;
      pdata16bits = (uint16_t *) pData;
    }
    else
    {
      pdata8bits  = pData;
      pdata16bits = NULL;
    }

    /* 检查待接收的剩余数据量 */
    while (huart->RxXferCount > 0U)
    {
      if (UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_RXNE, RESET, tickstart, Timeout) != HAL_OK)
      {
        huart->RxState = HAL_UART_STATE_READY;

        return HAL_TIMEOUT;
      }
      if (pdata8bits == NULL)
      {
        *pdata16bits = (uint16_t)(huart->Instance->DR & 0x01FF);
        pdata16bits++;
      }
      else
      {
        if ((huart->Init.WordLength == UART_WORDLENGTH_9B) || ((huart->Init.WordLength == UART_WORDLENGTH_8B) && (huart->Init.Parity == UART_PARITY_NONE)))
        {
          *pdata8bits = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
        }
        else
        {
          *pdata8bits = (uint8_t)(huart->Instance->DR & (uint8_t)0x007F);
        }
        pdata8bits++;
      }
      huart->RxXferCount--;
    }

    /* 在接收过程结束时，将 huart->RxState 恢复为就绪（Ready）状态 */
    huart->RxState = HAL_UART_STATE_READY;

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  以非阻塞模式发送一定量的数据。
  * @note   当未使能 UART 奇偶校验 (PCE = 0) 且字长配置为 9 位 (M1-M0 = 01) 时，
  *         发送的数据将作为 u16 数组进行处理。在此情况下，Size 必须表示通过
  *         pData 提供的 u16 数据元素个数。
  * @param  huart 指向包含指定 UART 模块配置信息的
  *               UART_HandleTypeDef 结构体的指针。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）。
  * @param  Size  待发送的数据元素（u8 或 u16）数量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size)
{
  /* 检查当前是否没有正在进行的发送过程 */
  if (huart->gState == HAL_UART_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return HAL_ERROR;
    }

    huart->pTxBuffPtr = pData;
    huart->TxXferSize = Size;
    huart->TxXferCount = Size;

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->gState = HAL_UART_STATE_BUSY_TX;

    /* 使能 UART 发送数据寄存器空中断 */
    __HAL_UART_ENABLE_IT(huart, UART_IT_TXE);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  以非阻塞模式接收一定量的数据。
  * @note   当未使能 UART 奇偶校验 (PCE = 0) 且字长配置为 9 位 (M1-M0 = 01) 时，
  *         接收的数据将作为 u16 数组进行处理。在此情况下，Size 必须表示通过
  *         pData 可获取的 u16 数据元素个数。
  * @param  huart 指向包含指定 UART 模块配置信息的
  *               UART_HandleTypeDef 结构体的指针。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）。
  * @param  Size  待接收的数据元素（u8 或 u16）数量。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  /* 检查当前是否没有正在进行的接收过程 */
  if (huart->RxState == HAL_UART_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return HAL_ERROR;
    }

    /* 将接收类型设置为标准接收 */
    huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

    return (UART_Start_Receive_IT(huart, pData, Size));
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  以 DMA 模式发送一定量的数据。
  * @note   当未使能 UART 奇偶校验 (PCE = 0) 且字长配置为 9 位 (M1-M0 = 01) 时，
  *         发送的数据将作为 u16 数组进行处理。在此情况下，Size 必须表示通过
  *         pData 提供的 u16 数据元素个数。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）。
  * @param  Size  待发送的数据元素（u8 或 u16）数量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size)
{
  const uint32_t *tmp;

  /* 检查当前是否没有正在进行的发送过程 */
  if (huart->gState == HAL_UART_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return HAL_ERROR;
    }

    huart->pTxBuffPtr = pData;
    huart->TxXferSize = Size;
    huart->TxXferCount = Size;

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->gState = HAL_UART_STATE_BUSY_TX;

    /* 设置 UART DMA 传输完成回调函数 */
    huart->hdmatx->XferCpltCallback = UART_DMATransmitCplt;

    /* 设置 UART DMA 过半传输完成回调函数 */
    huart->hdmatx->XferHalfCpltCallback = UART_DMATxHalfCplt;

    /* 设置 DMA 错误回调函数 */
    huart->hdmatx->XferErrorCallback = UART_DMAError;

    /* 设置 DMA 中止回调函数 */
    huart->hdmatx->XferAbortCallback = NULL;

    /* 使能 UART 发送 DMA 通道 */
    tmp = (const uint32_t *)&pData;
    if (HAL_DMA_Start_IT(huart->hdmatx, *(const uint32_t *)tmp, (uint32_t)&huart->Instance->DR, Size) != HAL_OK)
    {
      /* 将错误代码设置为 DMA 错误 */
      huart->ErrorCode = HAL_UART_ERROR_DMA;

      /* 将 huart->gState 恢复为就绪状态 */
      huart->gState = HAL_UART_STATE_READY;

      return HAL_ERROR;
    }
    /* 向 SR 寄存器写 0 以清除 TC 标志位 */
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);

    /* 通过置位 UART CR3 寄存器中的 DMAT 位，使能发送请求的 DMA 传输 */
    ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_DMAT);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在 DMA 模式下接收指定量的数据。
  * @note   当未使能 UART 奇偶校验位 (PCE = 0)，且字长配置为 9 位 (M1-M0 = 01) 时，
  *         接收到的数据将被作为一组 u16 处理。在此情况下，Size 必须表示通过
  *         pData 可用的 u16 数据个数。
  * @param  huart 指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）。
  * @param  Size  要接收的数据元素数量（u8 或 u16）。
  * @note   当使能 UART 奇偶校验位 (PCE = 1) 时，接收到的数据包含奇偶校验位。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  /* 检查接收过程是否尚未在进行中 */
  if (huart->RxState == HAL_UART_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return HAL_ERROR;
    }

    /* 将接收类型设置为标准接收 */
    huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

    return (UART_Start_Receive_DMA(huart, pData, Size));
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief 暂停 DMA 传输。
  * @param  huart  指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_DMAPause(UART_HandleTypeDef *huart)
{
  uint32_t dmarequest = 0x00U;

  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT);
  if ((huart->gState == HAL_UART_STATE_BUSY_TX) && dmarequest)
  {
    /* 禁用 UART DMA 发送请求 */
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);
  }

  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);
  if ((huart->RxState == HAL_UART_STATE_BUSY_RX) && dmarequest)
  {
    /* 禁用 RXNE、PE 和 ERR（帧错误、噪声错误、过载错误）中断 */
    ATOMIC_CLEAR_BIT(huart->Instance->CR1, USART_CR1_PEIE);
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

    /* 禁用 UART DMA 接收请求 */
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);
  }

  return HAL_OK;
}

/**
  * @brief 恢复 DMA 传输。
  * @param  huart  指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_DMAResume(UART_HandleTypeDef *huart)
{

  if (huart->gState == HAL_UART_STATE_BUSY_TX)
  {
    /* 使能 UART DMA 发送请求 */
    ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_DMAT);
  }

  if (huart->RxState == HAL_UART_STATE_BUSY_RX)
  {
    /* 在恢复接收传输前清除过载错误 (Overrun) 标志位 */
    __HAL_UART_CLEAR_OREFLAG(huart);

    /* 重新使能 PE 和 ERR（帧错误、噪声错误、过载错误）中断 */
    if (huart->Init.Parity != UART_PARITY_NONE)
    {
      ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_PEIE);
    }
    ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_EIE);

    /* 使能 UART DMA 接收请求 */
    ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);
  }

  return HAL_OK;
}

/**
  * @brief 停止 DMA 传输。
  * @param  huart  指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_DMAStop(UART_HandleTypeDef *huart)
{
  uint32_t dmarequest = 0x00U;
  /* 该 API 未实现加锁机制，以允许用户应用程序在回调函数
     HAL_UART_TxCpltCallback() / HAL_UART_RxCpltCallback() 中调用 HAL UART API：
     当调用 HAL_DMA_Abort() API 时，会产生 DMA 发送/接收传输完成中断，
     并执行相应的回调函数 HAL_UART_TxCpltCallback() / HAL_UART_RxCpltCallback()
     */

  /* 如果 UART DMA 发送请求正在进行，则停止该请求 */
  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT);
  if ((huart->gState == HAL_UART_STATE_BUSY_TX) && dmarequest)
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);

    /* 中止 UART DMA 发送通道 */
    if (huart->hdmatx != NULL)
    {
      HAL_DMA_Abort(huart->hdmatx);
    }
    UART_EndTxTransfer(huart);
  }

  /* 如果 UART DMA 接收请求正在进行，则停止该请求 */
  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);
  if ((huart->RxState == HAL_UART_STATE_BUSY_RX) && dmarequest)
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

    /* 中止 UART DMA 接收通道 */
    if (huart->hdmarx != NULL)
    {
      HAL_DMA_Abort(huart->hdmarx);
    }
    UART_EndRxTransfer(huart);
  }

  return HAL_OK;
}

/**
  * @brief 以阻塞模式接收一定量的数据，直到接收到预期数量的数据或发生空闲（IDLE）事件。
  * @note   如果接收完成（已接收到预期数量的数据）或者在发生 IDLE 事件后停止接收（接收到的数据少于预期数量），
  *         则返回 HAL_OK。
  *         在此情况下，输出参数 RxLen 指示接收缓冲区中的可用数据量。
  * @note   当未使能 UART 奇偶校验 (PCE = 0) 且字长配置为 9 位 (M = 01) 时，
  *         接收到的数据将作为 uint16_t 集合处理。在此情况下，Size 必须指示
  *         通过 pData 可用的 uint16_t 元素个数。
  * @param huart   UART 句柄。
  * @param pData   指向数据缓冲区的指针（uint8_t 或 uint16_t 数据元素）。
  * @param Size    要接收的数据元素数量（uint8_t 或 uint16_t）。
  * @param RxLen   最终接收到的数据元素数量（在因 IDLE 事件结束接收的情况下，该值可能小于 Size）。
  * @param Timeout 超时持续时间，以毫秒 (ms) 为单位（涵盖整个接收过程）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint16_t *RxLen,
                                           uint32_t Timeout)
{
  uint8_t  *pdata8bits;
  uint16_t *pdata16bits;
  uint32_t tickstart;

  /* 检查接收过程是否尚未在进行中 */
  if (huart->RxState == HAL_UART_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->RxState = HAL_UART_STATE_BUSY_RX;
    huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE;
    huart->RxEventType = HAL_UART_RXEVENT_TC;

    /* 初始化 tickstart 用于超时管理 */
    tickstart = HAL_GetTick();

    huart->RxXferSize  = Size;
    huart->RxXferCount = Size;

    /* 在 9位/无校验 传输情况下，pRxData 需要作为 uint16_t 指针处理 */
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))
    {
      pdata8bits  = NULL;
      pdata16bits = (uint16_t *) pData;
    }
    else
    {
      pdata8bits  = pData;
      pdata16bits = NULL;
    }

    /* 初始化已接收元素的输出计数 */
    *RxLen = 0U;

    /* 只要仍有数据需要接收 */
    while (huart->RxXferCount > 0U)
    {
      /* 检查 IDLE 标志位是否置位 */
      if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
      {
        /* 清除 ISR 中的 IDLE 标志位 */
        __HAL_UART_CLEAR_IDLEFLAG(huart);

        /* 若置位，但未接收到任何数据，则清除标志且不退出循环 */
        /* 若置位且已接收到数据，则表明空闲事件有效：结束接收 */
        if (*RxLen > 0U)
        {
          huart->RxEventType = HAL_UART_RXEVENT_IDLE;
          huart->RxState = HAL_UART_STATE_READY;

          return HAL_OK;
        }
      }

      /* 检查 RXNE 标志是否置位 */
      if (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE))
      {
        if (pdata8bits == NULL)
        {
          *pdata16bits = (uint16_t)(huart->Instance->DR & (uint16_t)0x01FF);
          pdata16bits++;
        }
        else
        {
          if ((huart->Init.WordLength == UART_WORDLENGTH_9B) || ((huart->Init.WordLength == UART_WORDLENGTH_8B) && (huart->Init.Parity == UART_PARITY_NONE)))
          {
            *pdata8bits = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
          }
          else
          {
            *pdata8bits = (uint8_t)(huart->Instance->DR & (uint8_t)0x007F);
          }

          pdata8bits++;
        }
        /* 递增已接收数据元素的数量 */
        *RxLen += 1U;
        huart->RxXferCount--;
      }

      /* 检查是否超时 */
      if (Timeout != HAL_MAX_DELAY)
      {
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))
        {
          huart->RxState = HAL_UART_STATE_READY;

          return HAL_TIMEOUT;
        }
      }
    }

    /* 在输出参数 RxLen 中设置已接收的数据元素数量 */
    *RxLen = huart->RxXferSize - huart->RxXferCount;
    /* 接收过程结束时，将 huart->RxState 恢复为 Ready 状态 */
    huart->RxState = HAL_UART_STATE_READY;

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief 在中断模式下接收一定量的数据，直到接收到预期数量的数据或发生 IDLE 事件为止。
  * @note   接收过程由此函数调用启动。后续的接收进展通过
  *         RXNE 和 IDLE 事件触发的 UART 中断来实现。接收结束时将调用回调函数，
  *         并指示已接收的数据元素数量。
  * @note   当未使能 UART 奇偶校验（PCE = 0）且字长配置为 9 位（M = 01）时，
  *         接收到的数据将作为一组 uint16_t 处理。在此情况下，Size 必须表示
  *         通过 pData 可用的 uint16_t 元素数量。
  * @param huart UART 句柄。
  * @param pData 指向数据缓冲区的指针（uint8_t 或 uint16_t 数据元素）。
  * @param Size  待接收的数据元素数量（uint8_t 或 uint16_t）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  HAL_StatusTypeDef status;

  /* 检查是否已有正在进行的接收过程 */
  if (huart->RxState == HAL_UART_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return HAL_ERROR;
    }

    /* 将接收类型设置为接收直至 IDLE 事件 */
    huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE;
    huart->RxEventType = HAL_UART_RXEVENT_TC;

    status =  UART_Start_Receive_IT(huart, pData, Size);

    /* 检查接收过程是否已成功启动 */
    if (status == HAL_OK)
    {
      if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
      {
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);
      }
      else
      {
        /* 如果在接收启动时已有挂起的错误，
           可能已经触发了中断并导致接收中止。
           （例如过载错误）。
           在此类情况下，接收类型已被重置为 HAL_UART_RECEPTION_STANDARD。 */
        status = HAL_ERROR;
      }
    }

    return status;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief 在 DMA 模式下接收一定量的数据，直到接收到预期数量的数据或发生 IDLE 事件为止。
  * @note   接收过程由此函数调用启动。后续的接收进展通过
  *         DMA 服务实现，它会自动将接收到的数据元素传输到用户接收缓冲区，
  *         并在接收过半/结束时调用注册的回调函数。UART IDLE 事件也被用于判定
  *         接收阶段结束。在所有情况下，回调函数的执行都将指示已接收的数据元素数量。
  * @note   当使能 UART 奇偶校验（PCE = 1）时，接收到的数据包含
  *         奇偶校验位（最高有效位 MSB 位置）。
  * @note   当未使能 UART 奇偶校验（PCE = 0）且字长配置为 9 位（M = 01）时，
  *         接收到的数据将作为一组 uint16_t 处理。在此情况下，Size 必须表示
  *         通过 pData 可用的 uint16_t 元素数量。
  * @param huart UART 句柄。
  * @param pData 指向数据缓冲区的指针（uint8_t 或 uint16_t 数据元素）。
  * @param Size  待接收的数据元素数量（uint8_t 或 uint16_t）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  HAL_StatusTypeDef status;

  /* 检查是否已有正在进行的接收过程 */
  if (huart->RxState == HAL_UART_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return HAL_ERROR;
    }

    /* 将接收类型设置为接收直至 IDLE 事件 */
    huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE;
    huart->RxEventType = HAL_UART_RXEVENT_TC;

    status =  UART_Start_Receive_DMA(huart, pData, Size);

    /* 检查接收过程是否已成功启动 */
    if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
    {
      __HAL_UART_CLEAR_IDLEFLAG(huart);
      ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);
    }
    else
    {
      /* 如果在启动接收时已有错误挂起，
         可能已经触发了中断并导致接收中止。
         （例如过载错误 Overrun error）。
         在此情况下，接收类型已被重置为 HAL_UART_RECEPTION_STANDARD。 */
      status = HAL_ERROR;
    }

    return status;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief 提供触发 RxEvent 回调函数执行的接收事件类型。
  * @note  当调用 HAL_UARTEx_ReceiveToIdle_IT() 或 HAL_UARTEx_ReceiveToIdle_DMA() API 时，接收过程的进度
  *        通过调用 Rx 事件回调函数（默认的 HAL_UARTEx_RxEventCallback() 或用户注册的回调函数）提供给应用程序。
  *        由于可能发生多种类型的事件（空闲事件 IDLE、过半传输 Half Transfer 或传输完成 Transfer Complete），
  *        本函数用于获取导致 Rx 事件回调执行的具体接收事件类型。
  * @note  该函数应在用户实现的 Rx 事件回调函数中调用，
  *        以获取准确的值：
  *        中断模式下：
  *           - HAL_UART_RXEVENT_TC : 接收完成时（已接收到预期数量的数据）
  *           - HAL_UART_RXEVENT_IDLE : 在接收完成前发生了空闲事件（接收到的数据量
  *             小于预期数量）
  *        DMA 模式下：
  *           - HAL_UART_RXEVENT_TC : 接收完成时（已接收到预期数量的数据）
  *           - HAL_UART_RXEVENT_HT : 接收到预期数据量的一半时
  *           - HAL_UART_RXEVENT_IDLE : 在接收完成前发生了空闲事件（接收到的数据量
  *             小于预期数量）。
  *        在 DMA 模式下，RxEvent 回调函数可能会被多次调用；
  *        当 DMA 配置为普通模式（Normal Mode）时，HT 事件不会停止接收过程；
  *        当 DMA 配置为循环模式（Circular Mode）时，HT、TC 或 IDLE 事件均不会停止接收过程；
  * @param  huart UART 句柄。
  * @retval 接收事件类型（返回值将是 @ref UART_RxEvent_Type_Values 的值）
  */
HAL_UART_RxEventTypeTypeDef HAL_UARTEx_GetRxEventType(UART_HandleTypeDef *huart)
{
  /* 返回存储在 UART 句柄中的接收事件类型值 */
  return(huart->RxEventType);
}

/**
  * @brief  中止正在进行的传输（阻塞模式）。
  * @param  huart UART 句柄。
  * @note   此过程可用于中止以中断或 DMA 模式启动的任何正在进行的传输。
  *         此过程执行以下操作：
  *           - 禁用 UART 中断（发送和接收）
  *           - 在外设寄存器中禁用 DMA 传输（如果已使能）
  *           - 通过调用 HAL_DMA_Abort 中止 DMA 传输（适用于 DMA 模式下的传输）
  *           - 将句柄状态设置为 READY
  * @note   此过程以阻塞模式执行：退出函数时，中止操作即视为已完成。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_Abort(UART_HandleTypeDef *huart)
{
  /* 禁用 TXEIE、TCIE、RXNE、PE 和 ERR（帧错误、噪声错误、过载错误）中断 */
  ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE));
  ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

  /* 如果正在进行接收直到空闲事件（IDLE）的过程，则禁用 IDLEIE 中断 */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_IDLEIE));
  }

  /* 如果已使能，则禁用 UART DMA 发送请求 */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT))
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);

    /* 中止 UART DMA 发送通道：使用阻塞式 DMA 中止 API（无回调函数） */
    if (huart->hdmatx != NULL)
    {
      /* 将 UART DMA 中止回调函数设置为 Null。
         DMA 中止过程结束时不执行回调函数 */
      huart->hdmatx->XferAbortCallback = NULL;

      if (HAL_DMA_Abort(huart->hdmatx) != HAL_OK)
      {
        if (HAL_DMA_GetError(huart->hdmatx) == HAL_DMA_ERROR_TIMEOUT)
        {
          /* 将错误代码设置为 DMA 错误 */
          huart->ErrorCode = HAL_UART_ERROR_DMA;

          return HAL_TIMEOUT;
        }
      }
    }
  }

  /* 如果已使能，则禁用 UART DMA 接收请求 */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

    /* 中止 UART DMA 接收通道：使用阻塞式 DMA 中止 API（无回调函数） */
    if (huart->hdmarx != NULL)
    {
      /* 将 UART DMA 中止回调函数设置为 Null。
         DMA 中止过程结束时不执行回调函数 */
      huart->hdmarx->XferAbortCallback = NULL;

      if (HAL_DMA_Abort(huart->hdmarx) != HAL_OK)
      {
        if (HAL_DMA_GetError(huart->hdmarx) == HAL_DMA_ERROR_TIMEOUT)
        {
          /* 将错误代码设置为 DMA 错误 */
          huart->ErrorCode = HAL_UART_ERROR_DMA;

          return HAL_TIMEOUT;
        }
      }
    }
  }

  /* 复位 Tx 和 Rx 传输计数器 */
  huart->TxXferCount = 0x00U;
  huart->RxXferCount = 0x00U;

  /* 复位 ErrorCode */
  huart->ErrorCode = HAL_UART_ERROR_NONE;

  /* 将 huart->RxState 和 huart->gState 恢复为 Ready 状态 */
  huart->RxState = HAL_UART_STATE_READY;
  huart->gState = HAL_UART_STATE_READY;
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

  return HAL_OK;
}

/**
  * @brief  终止正在进行的发送传输（阻塞模式）。
  * @param  huart UART 句柄。
  * @note   此过程可用于终止以中断或 DMA 模式启动的任何正在进行的 Tx 传输。
  *         该过程执行以下操作：
  *           - 禁用 UART 中断 (Tx)
  *           - 禁用外设寄存器中的 DMA 传输（若已使能）
  *           - 通过调用 HAL_DMA_Abort 终止 DMA 传输（若为 DMA 模式传输）
  *           - 将句柄状态设置为 READY
  * @note   此过程在阻塞模式下执行：退出函数时，终止操作即视为已完成。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_AbortTransmit(UART_HandleTypeDef *huart)
{
  /* 禁用 TXEIE 和 TCIE 中断 */
  ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));

  /* 禁用 UART DMA Tx 请求（若已使能） */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT))
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);

    /* 终止 UART DMA Tx 通道：使用阻塞式 DMA 终止 API（无回调） */
    if (huart->hdmatx != NULL)
    {
      /* 将 UART DMA 中止回调函数设置为 Null。
         在 DMA 中止流程结束时不执行任何回调 */
      huart->hdmatx->XferAbortCallback = NULL;

      if (HAL_DMA_Abort(huart->hdmatx) != HAL_OK)
      {
        if (HAL_DMA_GetError(huart->hdmatx) == HAL_DMA_ERROR_TIMEOUT)
        {
          /* 将错误代码设置为 DMA 错误 */
          huart->ErrorCode = HAL_UART_ERROR_DMA;

          return HAL_TIMEOUT;
        }
      }
    }
  }

  /* 复位 Tx 传输计数器 */
  huart->TxXferCount = 0x00U;

  /* 将 huart->gState 恢复为 Ready 状态 */
  huart->gState = HAL_UART_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  终止正在进行的接收传输（阻塞模式）。
  * @param  huart UART 句柄。
  * @note   此过程可用于终止以中断或 DMA 模式启动的任何正在进行的 Rx 传输。
  *         该过程执行以下操作：
  *           - 禁用 UART 中断 (Rx)
  *           - 禁用外设寄存器中的 DMA 传输（若已使能）
  *           - 通过调用 HAL_DMA_Abort 终止 DMA 传输（若为 DMA 模式传输）
  *           - 将句柄状态设置为 READY
  * @note   此过程在阻塞模式下执行：退出函数时，终止操作即视为已完成。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_AbortReceive(UART_HandleTypeDef *huart)
{
  /* 禁用 RXNE、PE 和 ERR（帧错误、噪声错误、过载错误）中断 */
  ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
  ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

  /* 如果正在进行接收直至 IDLE 事件的传输，则禁用 IDLEIE 中断 */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_IDLEIE));
  }

  /* 禁用 UART DMA Rx 请求（若已使能） */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

    /* 中止 UART DMA Rx 通道：使用阻塞式 DMA 中止 API（无回调） */
    if (huart->hdmarx != NULL)
    {
      /* 将 UART DMA 中止回调设置为 Null。
         在 DMA 中止流程结束时不执行回调 */
      huart->hdmarx->XferAbortCallback = NULL;

      if (HAL_DMA_Abort(huart->hdmarx) != HAL_OK)
      {
        if (HAL_DMA_GetError(huart->hdmarx) == HAL_DMA_ERROR_TIMEOUT)
        {
          /* 设置错误码为 DMA 错误 */
          huart->ErrorCode = HAL_UART_ERROR_DMA;

          return HAL_TIMEOUT;
        }
      }
    }
  }

  /* 复位 Rx 传输计数器 */
  huart->RxXferCount = 0x00U;

  /* 将 huart->RxState 恢复为 Ready 状态 */
  huart->RxState = HAL_UART_STATE_READY;
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

  return HAL_OK;
}

/**
  * @brief  中止正在进行的传输（中断模式）。
  * @param  huart UART 句柄。
  * @note   此流程可用于中止在中断或 DMA 模式下启动的任何正在进行的传输。
  *         该流程执行以下操作：
  *           - 禁用 UART 中断（Tx 和 Rx）
  *           - 禁用外设寄存器中的 DMA 传输（如果已使能）
  *           - 通过调用 HAL_DMA_Abort_IT 中止 DMA 传输（若处于 DMA 传输模式）
  *           - 将句柄状态设置为 READY
  *           - 中止完成时，调用用户中止完成回调函数
  * @note   此流程在中断模式下执行，这意味着仅当执行了用户中止完成回调函数时，
  *         中止流程才可被视为已完成（退出本函数时并未完成）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_Abort_IT(UART_HandleTypeDef *huart)
{
  uint32_t AbortCplt = 0x01U;

  /* 禁用 TXEIE、TCIE、RXNE、PE 以及 ERR（帧错误、噪声错误、过载错误）中断 */
  ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE));
  ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

  /* 如果正在进行接收直至 IDLE 事件的传输，则禁用 IDLEIE 中断 */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_IDLEIE));
  }

  /* 如果 DMA Tx 和/或 DMA Rx 句柄已关联到 UART 句柄，则在调用 DMA 中止函数之前
     应先初始化 DMA 中止完成回调函数 */
  /* DMA Tx 句柄有效 */
  if (huart->hdmatx != NULL)
  {
    /* 如果 UART DMA Tx 请求已使能，则设置 DMA 中止完成回调函数。
       否则，将其设置为 NULL */
    if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT))
    {
      huart->hdmatx->XferAbortCallback = UART_DMATxAbortCallback;
    }
    else
    {
      huart->hdmatx->XferAbortCallback = NULL;
    }
  }
  /* DMA Rx 句柄有效 */
  if (huart->hdmarx != NULL)
  {
    /* 如果 UART DMA Rx 请求已使能，则设置 DMA 中止完成回调函数。
       否则，将其设置为 NULL */
    if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))
    {
      huart->hdmarx->XferAbortCallback = UART_DMARxAbortCallback;
    }
    else
    {
      huart->hdmarx->XferAbortCallback = NULL;
    }
  }

  /* 如果已使能，则禁用 UART DMA Tx 请求 */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT))
  {
    /* 在 UART 层面禁用 DMA Tx */
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);

    /* 中止 UART DMA Tx 通道：使用非阻塞式 DMA 中止 API（回调） */
    if (huart->hdmatx != NULL)
    {
      /* UART Tx DMA 中止回调已初始化：
         将在 DMA 中止流程结束时调用 HAL_UART_AbortCpltCallback() */

      /* 中止 DMA TX */
      if (HAL_DMA_Abort_IT(huart->hdmatx) != HAL_OK)
      {
        huart->hdmatx->XferAbortCallback = NULL;
      }
      else
      {
        AbortCplt = 0x00U;
      }
    }
  }

  /* 若已使能，则禁用 UART DMA Rx 请求 */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

    /* 中止 UART DMA Rx 通道：使用非阻塞式 DMA 中止 API（回调） */
    if (huart->hdmarx != NULL)
    {
      /* UART Rx DMA 中止回调已初始化：
         将在 DMA 中止流程结束时调用 HAL_UART_AbortCpltCallback() */

      /* 中止 DMA RX */
      if (HAL_DMA_Abort_IT(huart->hdmarx) != HAL_OK)
      {
        huart->hdmarx->XferAbortCallback = NULL;
        AbortCplt = 0x01U;
      }
      else
      {
        AbortCplt = 0x00U;
      }
    }
  }

  /* 若无需执行 DMA 中止完成回调 => 调用用户中止完成回调 */
  if (AbortCplt == 0x01U)
  {
    /* 复位 Tx 和 Rx 传输计数器 */
    huart->TxXferCount = 0x00U;
    huart->RxXferCount = 0x00U;

    /* 复位 ErrorCode */
    huart->ErrorCode = HAL_UART_ERROR_NONE;

    /* 将 huart->gState 和 huart->RxState 恢复为 Ready 状态 */
    huart->gState  = HAL_UART_STATE_READY;
    huart->RxState = HAL_UART_STATE_READY;
    huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

    /* 由于没有需要中止的 DMA，直接调用用户中止完成回调 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    /* 调用已注册的中止完成回调函数 */
    huart->AbortCpltCallback(huart);
#else
    /* 调用传统的弱定义（weak）中止完成回调函数 */
    HAL_UART_AbortCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }

  return HAL_OK;
}

/**
  * @brief  中止正在进行的发送传输（中断模式）。
  * @param  huart UART 句柄。
  * @note   此过程可用于中止以中断或 DMA 模式启动的任何正在进行的 Tx 传输。
  *         该过程执行以下操作：
  *           - 禁用 UART 中断 (Tx)
  *           - 禁用外设寄存器中的 DMA 传输（如果已使能）
  *           - 通过调用 HAL_DMA_Abort_IT 中止 DMA 传输（处于 DMA 模式传输的情况下）
  *           - 将句柄状态设置为 READY
  *           - 中止完成时，调用用户中止完成回调函数
  * @note   此过程在中断模式下执行，这意味着仅当执行了用户中止完成回调函数时（而不是退出函数时），
  *         中止过程才可被视为已完成。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_AbortTransmit_IT(UART_HandleTypeDef *huart)
{
  /* 禁用 TXEIE 和 TCIE 中断 */
  ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));

  /* 禁用 UART DMA Tx 请求（如果已使能） */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT))
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);

    /* 中止 UART DMA Tx 通道：使用阻塞式 DMA 中止 API（无回调） */
    if (huart->hdmatx != NULL)
    {
      /* 设置 UART DMA 中止回调：
         将在 DMA 中止过程结束时调用 HAL_UART_AbortCpltCallback() */
      huart->hdmatx->XferAbortCallback = UART_DMATxOnlyAbortCallback;

      /* 中止 DMA TX */
      if (HAL_DMA_Abort_IT(huart->hdmatx) != HAL_OK)
      {
        /* 发生错误时直接调用 huart->hdmatx->XferAbortCallback 函数 */
        huart->hdmatx->XferAbortCallback(huart->hdmatx);
      }
    }
    else
    {
      /* 复位 Tx 传输计数器 */
      huart->TxXferCount = 0x00U;

      /* 将 huart->gState 恢复为 Ready 状态 */
      huart->gState = HAL_UART_STATE_READY;

      /* 由于没有需要中止的 DMA，直接调用用户中止完成回调函数 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
      /* 调用已注册的发送中止完成回调函数 */
      huart->AbortTransmitCpltCallback(huart);
#else
      /* 调用传统的弱定义发送中止完成回调函数 */
      HAL_UART_AbortTransmitCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
    }
  }
  else
  {
    /* 复位 Tx 传输计数器 */
    huart->TxXferCount = 0x00U;

    /* 将 huart->gState 恢复为 Ready 状态 */
    huart->gState = HAL_UART_STATE_READY;

    /* 由于没有需要中止的 DMA，直接调用用户中止完成回调函数 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    /* 调用已注册的中止发送完成回调函数 */
    huart->AbortTransmitCpltCallback(huart);
#else
    /* 调用传统的弱定义中止发送完成回调函数 */
    HAL_UART_AbortTransmitCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }

  return HAL_OK;
}

/**
  * @brief  中止正在进行的接收传输（中断模式）。
  * @param  huart UART 句柄。
  * @note   此过程可用于中止在中断或 DMA 模式下启动的任何正在进行的 Rx 传输。
  *         该过程执行以下操作：
  *           - 禁用 UART 中断 (Rx)
  *           - 在外设寄存器中禁用 DMA 传输（如果已使能）
  *           - 通过调用 HAL_DMA_Abort_IT 中止 DMA 传输（对于 DMA 模式下的传输）
  *           - 将句柄状态设置为 READY
  *           - 中止完成时，调用用户中止完成回调函数
  * @note   此过程在中断模式下执行，这意味着只有在执行用户中止完成回调函数时，
  *         中止过程才可被视为已完成（而不是在退出函数时）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_UART_AbortReceive_IT(UART_HandleTypeDef *huart)
{
  /* 禁用 RXNE、PE 和 ERR（帧错误、噪声错误、过载错误）中断 */
  ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
  ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

  /* 如果正在进行接收至空闲事件的传输，则禁用 IDLEIE 中断 */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_IDLEIE));
  }

  /* 禁用 UART DMA Rx 请求（如果已使能） */
  if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

    /* 中止 UART DMA Rx 通道：使用阻塞式 DMA 中止 API（无回调） */
    if (huart->hdmarx != NULL)
    {
      /* 设置 UART DMA 中止回调函数：
         将在 DMA 中止过程结束时调用 HAL_UART_AbortCpltCallback() */
      huart->hdmarx->XferAbortCallback = UART_DMARxOnlyAbortCallback;

      /* 中止 DMA RX */
      if (HAL_DMA_Abort_IT(huart->hdmarx) != HAL_OK)
      {
        /* 发生错误时直接调用 huart->hdmarx->XferAbortCallback 函数 */
        huart->hdmarx->XferAbortCallback(huart->hdmarx);
      }
    }
    else
    {
      /* 复位 Rx 传输计数器 */
      huart->RxXferCount = 0x00U;

      /* 将 huart->RxState 恢复为 Ready 状态 */
      huart->RxState = HAL_UART_STATE_READY;
      huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

      /* 由于没有需要中止的 DMA，直接调用用户中止完成回调函数 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
      /* 调用已注册的中止接收完成回调函数 */
      huart->AbortReceiveCpltCallback(huart);
#else
      /* 调用传统的弱定义中止接收完成回调函数 */
      HAL_UART_AbortReceiveCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
    }
  }
  else
  {
    /* 复位接收传输计数器 */
    huart->RxXferCount = 0x00U;

    /* 将 huart->RxState 恢复为就绪状态 (Ready) */
    huart->RxState = HAL_UART_STATE_READY;
    huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

    /* 由于没有需要中止的 DMA，直接调用用户中止完成回调函数 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    /* 调用已注册的中止接收完成回调函数 */
    huart->AbortReceiveCpltCallback(huart);
#else
    /* 调用传统的弱定义中止接收完成回调函数 */
    HAL_UART_AbortReceiveCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }

  return HAL_OK;
}

/**
  * @brief  此函数用于处理 UART 中断请求。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval 无
  */
void HAL_UART_IRQHandler(UART_HandleTypeDef *huart)
{
  uint32_t isrflags   = READ_REG(huart->Instance->SR);
  uint32_t cr1its     = READ_REG(huart->Instance->CR1);
  uint32_t cr3its     = READ_REG(huart->Instance->CR3);
  uint32_t errorflags = 0x00U;
  uint32_t dmarequest = 0x00U;

  /* 如果未发生错误 */
  errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
  if (errorflags == RESET)
  {
    /* UART 处于接收模式 -----------------------------------------------------*/
    if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
    {
      UART_Receive_IT(huart);
      return;
    }
  }

  /* 如果发生错误 */
  if ((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET)
                                || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))
  {
    /* 发生了 UART 奇偶校验错误中断 ------------------------------------------*/
    if (((isrflags & USART_SR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))
    {
      huart->ErrorCode |= HAL_UART_ERROR_PE;
    }

    /* 发生了 UART 噪声错误中断 ----------------------------------------------*/
    if (((isrflags & USART_SR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
    {
      huart->ErrorCode |= HAL_UART_ERROR_NE;
    }

    /* UART 帧错误中断发生 -----------------------------------*/
    if (((isrflags & USART_SR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
    {
      huart->ErrorCode |= HAL_UART_ERROR_FE;
    }

    /* UART 过载错误中断发生 --------------------------------------*/
    if (((isrflags & USART_SR_ORE) != RESET) && (((cr1its & USART_CR1_RXNEIE) != RESET)
                                                 || ((cr3its & USART_CR3_EIE) != RESET)))
    {
      huart->ErrorCode |= HAL_UART_ERROR_ORE;
    }

    /* 如有需要，调用 UART 错误回调函数 --------------------------*/
    if (huart->ErrorCode != HAL_UART_ERROR_NONE)
    {
      /* UART 处于接收模式 -----------------------------------------------*/
      if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
      {
        UART_Receive_IT(huart);
      }

      /* 若发生过载错误，或 DMA 接收模式下发生任何错误，
         则视该错误为阻塞性错误 */
      dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);
      if (((huart->ErrorCode & HAL_UART_ERROR_ORE) != RESET) || dmarequest)
      {
        /* 阻塞性错误：传输被中止
           将 UART 状态设置为就绪，以便能够重新启动该过程，
           若接收正在进行，则禁用 Rx 中断并禁用 Rx DMA 请求 */
        UART_EndRxTransfer(huart);

        /* 若已使能，则禁用 UART DMA Rx 请求 */
        if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))
        {
          ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

          /* 中止 UART DMA Rx 通道 */
          if (huart->hdmarx != NULL)
          {
            /* 设置 UART DMA 中止回调函数：
               将在 DMA 中止过程结束时调用 HAL_UART_ErrorCallback() */
            huart->hdmarx->XferAbortCallback = UART_DMAAbortOnError;
            if (HAL_DMA_Abort_IT(huart->hdmarx) != HAL_OK)
            {
              /* 发生错误时直接调用 XferAbortCallback 函数 */
              huart->hdmarx->XferAbortCallback(huart->hdmarx);
            }
          }
          else
          {
            /* 调用用户错误回调函数 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
            /*调用已注册的错误回调函数*/
            huart->ErrorCallback(huart);
#else
            /*调用传统的弱定义错误回调函数*/
            HAL_UART_ErrorCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
          }
        }
        else
        {
          /* 调用用户错误回调函数 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
          /*调用已注册的错误回调函数*/
          huart->ErrorCallback(huart);
#else
          /*调用传统的弱定义错误回调函数*/
          HAL_UART_ErrorCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
        }
      }
      else
      {
        /* 非阻塞错误：传输可继续进行。
           通过用户错误回调函数向用户通知错误 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
        /*调用已注册的错误回调函数*/
        huart->ErrorCallback(huart);
#else
        /*调用传统的弱定义错误回调函数*/
        HAL_UART_ErrorCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

        huart->ErrorCode = HAL_UART_ERROR_NONE;
      }
    }
    return;
  } /* 如果发生错误则终止 */

  /* 检查当前接收模式：
     如果已选择接收直至 IDLE 事件模式：*/
  if ((huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
      && ((isrflags & USART_SR_IDLE) != 0U)
      && ((cr1its & USART_CR1_IDLEIE) != 0U))
  {
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    /* 检查 UART 中是否启用了 DMA 模式 */
    if (HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))
    {
      /* DMA 模式已启用 */
      /* 检查接收长度：如果已接收所有预期数据，则不执行任何操作，
         （将调用 DMA 完成回调函数）。
         否则，如果已接收至少一个数据，则向用户通知 IDLE 事件 */
      uint16_t nb_remaining_rx_data = (uint16_t) __HAL_DMA_GET_COUNTER(huart->hdmarx);
      if ((nb_remaining_rx_data > 0U)
          && (nb_remaining_rx_data < huart->RxXferSize))
      {
        /* 接收未完成 */
        huart->RxXferCount = nb_remaining_rx_data;

        /* 在普通模式下，终止 DMA 传输和 HAL UART Rx 接收流程*/
        if (huart->hdmarx->Init.Mode != DMA_CIRCULAR)
        {
          /* 禁用 PE 和 ERR（帧错误、噪声错误、过载错误）中断 */
          ATOMIC_CLEAR_BIT(huart->Instance->CR1, USART_CR1_PEIE);
          ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

          /* 通过复位 UART CR3 寄存器中的 DMAR 位，禁用接收请求的 DMA 传输 */
          ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

          /* 接收处理结束时，将 huart->RxState 恢复为 Ready 状态 */
          huart->RxState = HAL_UART_STATE_READY;
          huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

          ATOMIC_CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);

          /* 最后若干字节已接收完成，由于中止操作是立即生效的，因此无需处理 */
          (void)HAL_DMA_Abort(huart->hdmarx);
        }

        /* 初始化与 RxEvent 回调执行相对应的 RxEvent 类型；
        此时，Rx 事件类型为空闲事件（Idle Event） */
        huart->RxEventType = HAL_UART_RXEVENT_IDLE;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
        /* 调用已注册的 Rx Event 回调函数 */
        huart->RxEventCallback(huart, (huart->RxXferSize - huart->RxXferCount));
#else
        /* 调用传统的弱定义 Rx Event 回调函数 */
        HAL_UARTEx_RxEventCallback(huart, (huart->RxXferSize - huart->RxXferCount));
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
      }
      else
      {
        /* 若 DMA 处于循环模式（Circular mode），即使空闲事件发生在 DMA 传输完成（Transfer Complete）事件之后，
           仍需向用户报告该空闲事件 */
        if (nb_remaining_rx_data == huart->RxXferSize)
        {
          if (huart->hdmarx->Init.Mode == DMA_CIRCULAR)
          {
            /* 初始化与 RxEvent 回调执行相对应的 RxEvent 类型；
               此时，Rx 事件类型为空闲事件（Idle Event） */
            huart->RxEventType = HAL_UART_RXEVENT_IDLE;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
            /* 调用已注册的 Rx Event 回调函数 */
            huart->RxEventCallback(huart, huart->RxXferSize);
#else
            /* 调用传统的弱定义 Rx Event 回调函数 */
            HAL_UARTEx_RxEventCallback(huart, huart->RxXferSize);
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS) */
          }
        }
      }
      return;
    }
    else
    {
      /* 未使能 DMA 模式 */
      /* 检查已接收长度：若预期数据已全部接收，则不执行任何操作。
         否则，若已接收至少一个数据，则需向用户通知 IDLE 事件 */
      uint16_t nb_rx_data = huart->RxXferSize - huart->RxXferCount;
      if ((huart->RxXferCount > 0U)
          && (nb_rx_data > 0U))
      {
        /* 禁用 UART 奇偶校验错误中断和 RXNE 中断 */
        ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));

        /* 禁用 UART 错误中断：（帧错误、噪声错误、过载错误） */
        ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

        /* 接收过程完成，将 huart->RxState 恢复为就绪（Ready）状态 */
        huart->RxState = HAL_UART_STATE_READY;
        huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

        ATOMIC_CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);

        /* 初始化与 RxEvent 回调执行相对应的 RxEvent 类型；
           在此情况下，Rx 事件类型为空闲事件（Idle Event） */
        huart->RxEventType = HAL_UART_RXEVENT_IDLE;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
        /* 调用已注册的接收完成回调函数 */
        huart->RxEventCallback(huart, nb_rx_data);
#else
        /* 调用传统的弱定义 Rx 事件回调函数 */
        HAL_UARTEx_RxEventCallback(huart, nb_rx_data);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
      }
      return;
    }
  }

  /* UART 处于发送器模式 ------------------------------------------------*/
  if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
  {
    UART_Transmit_IT(huart);
    return;
  }

  /* UART 发送器模式结束 --------------------------------------------*/
  if (((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
  {
    UART_EndTransmit_IT(huart);
    return;
  }
}

/**
  * @brief  发送传输完成回调函数。
  * @param  huart  指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(huart);
  /* 注意：不应修改此函数，当需要使用该回调函数时，
           可在用户文件中实现 HAL_UART_TxCpltCallback
   */
}

/**
  * @brief  发送过半传输完成回调函数。
  * @param  huart  指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(huart);
  /* 注意：不应修改此函数，当需要使用该回调函数时，
           可在用户文件中实现 HAL_UART_TxHalfCpltCallback
   */
}

/**
  * @brief  接收传输完成回调函数。
  * @param  huart  指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(huart);
  /* NOTE: 请勿修改此函数，当需要使用该回调函数时，
           可在用户文件中重新实现 HAL_UART_RxCpltCallback
   */
}

/**
  * @brief  接收半传输完成回调函数。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(huart);
  /* NOTE: 请勿修改此函数，当需要使用该回调函数时，
           可在用户文件中重新实现 HAL_UART_RxHalfCpltCallback
   */
}

/**
  * @brief  UART 错误回调函数。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(huart);
  /* NOTE: 请勿修改此函数，当需要使用该回调函数时，
           可在用户文件中重新实现 HAL_UART_ErrorCallback
   */
}

/**
  * @brief  UART 中止完成回调函数。
  * @param  huart UART 句柄。
  * @retval 无
  */
__weak void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(huart);

  /* NOTE : 请勿修改此函数，当需要使用该回调函数时，
            可在用户文件中实现 HAL_UART_AbortCpltCallback。
   */
}

/**
  * @brief  UART 中止完成回调函数。
  * @param  huart UART 句柄。
  * @retval 无
  */
__weak void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(huart);

  /* NOTE : 请勿修改此函数，当需要使用该回调函数时，
            可在用户文件中实现 HAL_UART_AbortTransmitCpltCallback。
   */
}

/**
  * @brief  UART 中止接收完成回调函数。
  * @param  huart UART 句柄。
  * @retval 无
  */
__weak void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(huart);

  /* 注意：不应修改此函数，当需要使用回调函数时，
            可以在用户文件中重新实现 HAL_UART_AbortReceiveCpltCallback。
   */
}

/**
  * @brief  接收事件回调函数（在使用高级接收服务后调用的接收事件通知）。
  * @param  huart UART 句柄
  * @param  Size  应用程序接收缓冲区中可用的数据量（指示接收缓冲区中
  *               数据可用的截止位置）
  * @retval 无
  */
__weak void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(huart);
  UNUSED(Size);

  /* 注意：不应修改此函数，当需要使用回调函数时，
            可以在用户文件中重新实现 HAL_UARTEx_RxEventCallback。
   */
}

/**
  * @}
  */

/** @defgroup UART_Exported_Functions_Group3 Peripheral Control functions
  *  @brief   UART 控制函数
  *
@verbatim
  ==============================================================================
                      ##### 外设控制函数 #####
  ==============================================================================
  [..]
    本小节提供了一组用于控制 UART 的函数：
    (+) HAL_LIN_SendBreak() API 可用于发送同步断开符（Break 字符）。
    (+) HAL_MultiProcessor_EnterMuteMode() API 可用于使 UART 进入静默模式。
    (+) HAL_MultiProcessor_ExitMuteMode() API 可用于通过软件使 UART 退出静默模式。
    (+) HAL_HalfDuplex_EnableTransmitter() API 用于在半双工模式下使能 UART 发送器并禁用 UART 接收器。
    (+) HAL_HalfDuplex_EnableReceiver() API 用于在半双工模式下使能 UART 接收器并禁用 UART 发送器。

@endverbatim
  * @{
  */

/**
  * @brief  发送断开符（Break 字符）。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_LIN_SendBreak(UART_HandleTypeDef *huart)
{
  /* 检查参数 */
  assert_param(IS_UART_INSTANCE(huart->Instance));

  /* 锁定处理流程 */
  __HAL_LOCK(huart);

  huart->gState = HAL_UART_STATE_BUSY;

  /* 发送断开符 */
  ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_SBK);

  huart->gState = HAL_UART_STATE_READY;

  /* 解锁处理流程 */
  __HAL_UNLOCK(huart);

  return HAL_OK;
}

/**
  * @brief  使 UART 进入静默模式。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_MultiProcessor_EnterMuteMode(UART_HandleTypeDef *huart)
{
  /* 检查参数 */
  assert_param(IS_UART_INSTANCE(huart->Instance));

  /* 锁定处理流程 */
  __HAL_LOCK(huart);

  huart->gState = HAL_UART_STATE_BUSY;

  /* 通过置位 CR1 寄存器中的 RWU 位使能 USART 静音模式 */
  ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_RWU);

  huart->gState = HAL_UART_STATE_READY;
  huart->RxEventType = HAL_UART_RXEVENT_TC;

  /* 进程解锁 */
  __HAL_UNLOCK(huart);

  return HAL_OK;
}

/**
  * @brief  退出 UART 静音模式：软件唤醒。
  * @param  huart  指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_MultiProcessor_ExitMuteMode(UART_HandleTypeDef *huart)
{
  /* 检查参数 */
  assert_param(IS_UART_INSTANCE(huart->Instance));

  /* 进程锁定 */
  __HAL_LOCK(huart);

  huart->gState = HAL_UART_STATE_BUSY;

  /* 通过清零 CR1 寄存器中的 RWU 位禁用 USART 静音模式 */
  ATOMIC_CLEAR_BIT(huart->Instance->CR1, USART_CR1_RWU);

  huart->gState = HAL_UART_STATE_READY;
  huart->RxEventType = HAL_UART_RXEVENT_TC;

  /* 进程解锁 */
  __HAL_UNLOCK(huart);

  return HAL_OK;
}

/**
  * @brief  使能 UART 发送器并禁用 UART 接收器。
  * @param  huart  指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_HalfDuplex_EnableTransmitter(UART_HandleTypeDef *huart)
{
  uint32_t tmpreg = 0x00U;

  /* 进程锁定 */
  __HAL_LOCK(huart);

  huart->gState = HAL_UART_STATE_BUSY;

  /*-------------------------- USART CR1 配置 -----------------------*/
  tmpreg = huart->Instance->CR1;

  /* 清零 TE 和 RE 位 */
  tmpreg &= (uint32_t)~((uint32_t)(USART_CR1_TE | USART_CR1_RE));

  /* 通过置位 USART CR1 寄存器中的 TE 位使能 USART 发送接口 */
  tmpreg |= (uint32_t)USART_CR1_TE;

  /* 写入 USART CR1 */
  WRITE_REG(huart->Instance->CR1, (uint32_t)tmpreg);

  huart->gState = HAL_UART_STATE_READY;

  /* 进程解锁 */
  __HAL_UNLOCK(huart);

  return HAL_OK;
}

/**
  * @brief  使能 UART 接收器并禁用 UART 发送器。
  * @param  huart  指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_HalfDuplex_EnableReceiver(UART_HandleTypeDef *huart)
{
  uint32_t tmpreg = 0x00U;

  /* 进程已上锁 */
  __HAL_LOCK(huart);

  huart->gState = HAL_UART_STATE_BUSY;

  /*-------------------------- USART CR1 配置 -----------------------*/
  tmpreg = huart->Instance->CR1;

  /* 清除 TE 和 RE 位 */
  tmpreg &= (uint32_t)~((uint32_t)(USART_CR1_TE | USART_CR1_RE));

  /* 通过置位 USART CR1 寄存器中的 RE 位以使能 USART 接收接口 */
  tmpreg |= (uint32_t)USART_CR1_RE;

  /* 写入 USART CR1 */
  WRITE_REG(huart->Instance->CR1, (uint32_t)tmpreg);

  huart->gState = HAL_UART_STATE_READY;

  /* 进程已解锁 */
  __HAL_UNLOCK(huart);

  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup UART_Exported_Functions_Group4 Peripheral State and Errors functions
  *  @brief   UART 状态和错误处理函数
  *
@verbatim
  ==============================================================================
                 ##### 外设状态与错误函数 #####
  ==============================================================================
 [..]
   本小节提供了一组函数，用于返回 UART 通信过程的状态，
   以及返回通信过程中发生的外设错误
   (+) HAL_UART_GetState() API 有助于在运行时检查 UART 外设的状态。
   (+) HAL_UART_GetError() 用于在运行时检查通信期间可能发生的错误。

@endverbatim
  * @{
  */

/**
  * @brief  返回 UART 状态。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_UART_StateTypeDef HAL_UART_GetState(const UART_HandleTypeDef *huart)
{
  uint32_t temp1 = 0x00U, temp2 = 0x00U;
  temp1 = huart->gState;
  temp2 = huart->RxState;

  return (HAL_UART_StateTypeDef)(temp1 | temp2);
}

/**
  * @brief  返回 UART 错误代码
  * @param  huart 指向包含指定 UART 配置信息的
  *               UART_HandleTypeDef 结构体的指针。
  * @retval UART 错误代码
  */
uint32_t HAL_UART_GetError(const UART_HandleTypeDef *huart)
{
  return huart->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup UART_Private_Functions UART 私有函数
  * @{
  */

/**
  * @brief  将回调函数初始化为其默认值。
  * @param  huart UART 句柄。
  * @retval 无
  */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
void UART_InitCallbacksToDefault(UART_HandleTypeDef *huart)
{
  /* 初始化 UART 回调函数配置 */
  huart->TxHalfCpltCallback        = HAL_UART_TxHalfCpltCallback;        /* 传统的弱定义 TxHalfCpltCallback 回调函数        */
  huart->TxCpltCallback            = HAL_UART_TxCpltCallback;            /* 传统的弱定义 TxCpltCallback 回调函数            */
  huart->RxHalfCpltCallback        = HAL_UART_RxHalfCpltCallback;        /* 传统的弱定义 RxHalfCpltCallback 回调函数        */
  huart->RxCpltCallback            = HAL_UART_RxCpltCallback;            /* 传统的弱定义 RxCpltCallback 回调函数            */
  huart->ErrorCallback             = HAL_UART_ErrorCallback;             /* 传统的弱定义 ErrorCallback 回调函数             */
  huart->AbortCpltCallback         = HAL_UART_AbortCpltCallback;         /* 传统的弱定义 AbortCpltCallback 回调函数         */
  huart->AbortTransmitCpltCallback = HAL_UART_AbortTransmitCpltCallback; /* 传统的弱定义 AbortTransmitCpltCallback 回调函数 */
  huart->AbortReceiveCpltCallback  = HAL_UART_AbortReceiveCpltCallback;  /* 传统的弱定义 AbortReceiveCpltCallback 回调函数  */
  huart->RxEventCallback           = HAL_UARTEx_RxEventCallback;         /* 传统的弱定义 RxEventCallback 回调函数           */

}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

/**
  * @brief  DMA UART 发送过程完成回调函数。
  * @param  hdma  指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void UART_DMATransmitCplt(DMA_HandleTypeDef *hdma)
{
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;
  /* DMA 正常模式 */
  if ((hdma->Instance->CCR & DMA_CCR_CIRC) == 0U)
  {
    huart->TxXferCount = 0x00U;

    /* 通过复位 UART CR3 寄存器中的 DMAT 位来禁用发送请求的 DMA 传输 */
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);

    /* 使能 UART 发送完成中断 */
    ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_TCIE);

  }
  /* DMA 循环模式 */
  else
  {
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    /* 调用已注册的发送完成回调函数 */
    huart->TxCpltCallback(huart);
#else
    /* 调用传统的弱定义发送完成回调函数 */
    HAL_UART_TxCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }
}

/**
  * @brief DMA UART 发送过程过半回调函数
  * @param  hdma  指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void UART_DMATxHalfCplt(DMA_HandleTypeDef *hdma)
{
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  /* 调用已注册的发送完成回调函数 */
  huart->TxHalfCpltCallback(huart);
#else
  /* 调用传统的弱定义发送完成回调函数 */
  HAL_UART_TxHalfCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA UART 接收过程完成回调函数。
  * @param  hdma  指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void UART_DMAReceiveCplt(DMA_HandleTypeDef *hdma)
{
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  /* DMA 正常模式 */
  if ((hdma->Instance->CCR & DMA_CCR_CIRC) == 0U)
  {
    huart->RxXferCount = 0U;

    /* 禁用 RXNE、PE 和 ERR（帧错误、噪声错误、过载错误）中断 */
    ATOMIC_CLEAR_BIT(huart->Instance->CR1, USART_CR1_PEIE);
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

    /* 通过重置 UART CR3 寄存器中的 DMAR 位，禁用针对接收请求的 DMA 传输 */
    ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

    /* 在接收过程结束时，将 huart->RxState 恢复为 Ready 状态 */
    huart->RxState = HAL_UART_STATE_READY;

    /* 如果已选择接收直到 IDLE 事件，则禁用 IDLE 中断 */
    if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
    {
      ATOMIC_CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);
    }
  }

  /* 初始化对应于 RxEvent 回调函数执行的 RxEvent 类型；
   在这种情况下，Rx 事件类型为传输完成 (Transfer Complete) */
  huart->RxEventType = HAL_UART_RXEVENT_TC;

  /* 检查当前接收模式：
     如果已选择接收直到 IDLE 事件：使用 Rx Event 回调函数 */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
  {
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    /* 调用已注册的接收事件（Rx Event）回调函数 */
    huart->RxEventCallback(huart, huart->RxXferSize);
#else
    /* 调用传统的弱定义接收事件（Rx Event）回调函数 */
    HAL_UARTEx_RxEventCallback(huart, huart->RxXferSize);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }
  else
  {
    /* 其他情况：使用接收完成（Rx Complete）回调函数 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    /* 调用已注册的接收完成（Rx complete）回调函数 */
    huart->RxCpltCallback(huart);
#else
    /* 调用传统的弱定义接收完成（Rx complete）回调函数 */
    HAL_UART_RxCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }
}

/**
  * @brief DMA UART 接收过半完成回调函数
  * @param  hdma  指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void UART_DMARxHalfCplt(DMA_HandleTypeDef *hdma)
{
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  /* 初始化与 RxEvent 回调执行相对应的 RxEvent 类型；
     在此情况下，Rx Event 类型为过半传输（Half Transfer） */
  huart->RxEventType = HAL_UART_RXEVENT_HT;

  /* 检查当前接收模式：
     如果选择了接收直到空闲（IDLE）事件：使用 Rx Event 回调函数 */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
  {
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    /* 调用已注册的接收事件（Rx Event）回调函数 */
    huart->RxEventCallback(huart, huart->RxXferSize / 2U);
#else
    /* 调用传统的弱定义接收事件（Rx Event）回调函数 */
    HAL_UARTEx_RxEventCallback(huart, huart->RxXferSize / 2U);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }
  else
  {
    /* 其他情况：使用接收过半完成（Rx Half Complete）回调函数 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    /* 调用已注册的接收过半完成（Rx Half complete）回调函数 */
    huart->RxHalfCpltCallback(huart);
#else
    /*调用传统的弱定义接收过半回调函数*/
    HAL_UART_RxHalfCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
  }
}

/**
  * @brief  DMA UART 通信错误回调函数。
  * @param  hdma  指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void UART_DMAError(DMA_HandleTypeDef *hdma)
{
  uint32_t dmarequest = 0x00U;
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  /* 若 UART DMA 发送请求正在进行，则停止该请求 */
  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT);
  if ((huart->gState == HAL_UART_STATE_BUSY_TX) && dmarequest)
  {
    huart->TxXferCount = 0x00U;
    UART_EndTxTransfer(huart);
  }

  /* 若 UART DMA 接收请求正在进行，则停止该请求 */
  dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);
  if ((huart->RxState == HAL_UART_STATE_BUSY_RX) && dmarequest)
  {
    huart->RxXferCount = 0x00U;
    UART_EndRxTransfer(huart);
  }

  huart->ErrorCode |= HAL_UART_ERROR_DMA;
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  /*调用已注册的错误回调函数*/
  huart->ErrorCallback(huart);
#else
  /*调用传统的弱定义错误回调函数*/
  HAL_UART_ErrorCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}

/**
  * @brief  此函数处理 UART 通信超时。它会等待，
  *         直到某个标志位不再处于指定状态。
  * @param  huart  指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @param  Flag 指定要检查的 UART 标志位。
  * @param  Status 当前标志位状态 (SET 或 RESET)。
  * @param  Tickstart 滴答定时器起始值
  * @param  Timeout 超时时间
  * @retval HAL 状态
  */
static HAL_StatusTypeDef UART_WaitOnFlagUntilTimeout(UART_HandleTypeDef *huart, uint32_t Flag, FlagStatus Status,
                                                     uint32_t Tickstart, uint32_t Timeout)
{
  /* 等待标志位置位 */
  while ((__HAL_UART_GET_FLAG(huart, Flag) ? SET : RESET) == Status)
  {
    /* 检查是否超时 */
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
      {

        return HAL_TIMEOUT;
      }

      if ((READ_BIT(huart->Instance->CR1, USART_CR1_RE) != 0U) && (Flag != UART_FLAG_TXE) && (Flag != UART_FLAG_TC))
      {
        if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) == SET)
        {
          /* 清除过载错误 (Overrun Error) 标志位*/
          __HAL_UART_CLEAR_OREFLAG(huart);

          /* 阻塞性错误：传输已中止
          将 UART 状态设置为就绪 (ready) 以便能够重新启动处理流程，
          若接收中断正在进行中则将其禁用 */
          UART_EndRxTransfer(huart);

          huart->ErrorCode = HAL_UART_ERROR_ORE;

          /* 进程已解锁 */
          __HAL_UNLOCK(huart);

          return HAL_ERROR;
        }
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  以中断模式启动接收操作。
  * @note   此函数可被所有提供中断模式接收的 HAL UART API 调用。
  * @note   调用此函数时，参数有效性被视为已检查完毕，
  *         即 Rx 状态、缓冲区地址等已确认无误。
  *         假定 UART 句柄已处于锁定 (Locked) 状态。
  * @param  huart UART 句柄。
  * @param  pData 指向数据缓冲区的指针 (u8 或 u16 数据元素)。
  * @param  Size  要接收的数据元素数量 (u8 或 u16)。
  * @retval HAL 状态
  */
HAL_StatusTypeDef UART_Start_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  huart->pRxBuffPtr = pData;
  huart->RxXferSize = Size;
  huart->RxXferCount = Size;

  huart->ErrorCode = HAL_UART_ERROR_NONE;
  huart->RxState = HAL_UART_STATE_BUSY_RX;

  if (huart->Init.Parity != UART_PARITY_NONE)
  {
    /* 使能 UART 奇偶校验错误中断 */
    __HAL_UART_ENABLE_IT(huart, UART_IT_PE);
  }

  /* 使能 UART 错误中断：（帧错误、噪声错误、过载错误） */
  __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

  /* 使能 UART 数据寄存器非空中断 */
  __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);

  return HAL_OK;
}

/**
  * @brief  启动 DMA 模式下的接收操作。
  * @note   该函数可被所有提供 DMA 模式接收功能的 HAL UART API 调用。
  * @note   调用此函数时，参数有效性已被视为检查完毕，
  *         即 Rx 状态、缓冲区地址等……
  *         且 UART 句柄已被假定处于锁定状态（Locked）。
  * @param  huart UART 句柄。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）。
  * @param  Size  待接收的数据元素数量（u8 或 u16）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef UART_Start_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  uint32_t *tmp;

  huart->pRxBuffPtr = pData;
  huart->RxXferSize = Size;

  huart->ErrorCode = HAL_UART_ERROR_NONE;
  huart->RxState = HAL_UART_STATE_BUSY_RX;

  /* 设置 UART DMA 传输完成回调函数 */
  huart->hdmarx->XferCpltCallback = UART_DMAReceiveCplt;

  /* 设置 UART DMA 半传输完成回调函数 */
  huart->hdmarx->XferHalfCpltCallback = UART_DMARxHalfCplt;

  /* 设置 DMA 错误回调函数 */
  huart->hdmarx->XferErrorCallback = UART_DMAError;

  /* 设置 DMA 中止回调函数 */
  huart->hdmarx->XferAbortCallback = NULL;

  /* 使能 DMA 数据流（Stream） */
  tmp = (uint32_t *)&pData;
  if (HAL_DMA_Start_IT(huart->hdmarx, (uint32_t)&huart->Instance->DR, *(uint32_t *)tmp, Size) != HAL_OK)
  {
    /* 将错误代码设置为 DMA 错误 */
    huart->ErrorCode = HAL_UART_ERROR_DMA;

    /* 将 huart->RxState 恢复为就绪（Ready）状态 */
    huart->RxState = HAL_UART_STATE_READY;

    return HAL_ERROR;
  }
  /* 在使能 DMA 接收请求之前清除过载错误（Overrun）标志：这对于第二次传输可能是必需的 */
  __HAL_UART_CLEAR_OREFLAG(huart);

  if (huart->Init.Parity != UART_PARITY_NONE)
  {
    /* 使能 UART 奇偶校验错误中断 */
    ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_PEIE);
  }

  /* 使能 UART 错误中断：（帧错误、噪声错误、过载错误） */
  ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_EIE);

  /* 通过置位 UART CR3 寄存器中的 DMAR 位，使能接收器请求的 DMA 传输 */
  ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

  return HAL_OK;
}

/**
  * @brief  终止 UART 外设上正在进行的 Tx 传输（在检测到错误或发送完成之后）。
  * @param  huart UART 句柄。
  * @retval 无
  */
static void UART_EndTxTransfer(UART_HandleTypeDef *huart)
{
  /* 禁用 TXEIE 和 TCIE 中断 */
  ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));

  /* 在 Tx 过程结束时，将 huart->gState 恢复为 Ready 状态 */
  huart->gState = HAL_UART_STATE_READY;
}

/**
  * @brief  终止 UART 外设上正在进行的 Rx 传输（在检测到错误或接收完成之后）。
  * @param  huart UART 句柄。
  * @retval 无
  */
static void UART_EndRxTransfer(UART_HandleTypeDef *huart)
{
  /* 禁用 RXNE、PE 和 ERR（帧错误、噪声错误、过载错误）中断 */
  ATOMIC_CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
  ATOMIC_CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

  /* 如果接收正在等待 IDLE 事件，同时禁用 IDLE IE 中断源 */
  if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
  {
    ATOMIC_CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);
  }

  /* 在 Rx 过程结束时，将 huart->RxState 恢复为 Ready 状态 */
  huart->RxState = HAL_UART_STATE_READY;
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;
}

/**
  * @brief  发生错误时由 HAL 服务发起的 DMA UART 通信中止回调函数
  *         （在发生错误后执行 DMA 中止流程结束时调用）。
  * @param  hdma  指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void UART_DMAAbortOnError(DMA_HandleTypeDef *hdma)
{
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;
  huart->RxXferCount = 0x00U;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  /* 调用已注册的错误回调函数 */
  huart->ErrorCallback(huart);
#else
  /* 调用传统的弱定义错误回调函数 */
  HAL_UART_ErrorCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}

/**
  * @brief  由用户发起的 DMA UART Tx 通信中止回调函数
  *         （在响应用户中止请求执行 DMA Tx 中止流程结束时调用）。
  * @note   执行此回调时，仅当 Rx DMA 句柄上没有正在进行的中止操作时，
  *         才会调用用户中止完成回调函数。
  * @param  hdma  指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void UART_DMATxAbortCallback(DMA_HandleTypeDef *hdma)
{
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  huart->hdmatx->XferAbortCallback = NULL;

  /* 检查是否仍有中止流程正在进行 */
  if (huart->hdmarx != NULL)
  {
    if (huart->hdmarx->XferAbortCallback != NULL)
    {
      return;
    }
  }

  /* 没有正在进行的中止流程：所有 DMA 通道均已中止，调用用户中止完成回调函数 */
  huart->TxXferCount = 0x00U;
  huart->RxXferCount = 0x00U;

  /* 复位 ErrorCode */
  huart->ErrorCode = HAL_UART_ERROR_NONE;

  /* 将 huart->gState 和 huart->RxState 恢复为就绪（Ready）状态 */
  huart->gState  = HAL_UART_STATE_READY;
  huart->RxState = HAL_UART_STATE_READY;
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

  /* 调用用户中止完成回调函数 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  /* 调用已注册的中止完成回调函数 */
  huart->AbortCpltCallback(huart);
#else
  /* 调用传统的弱定义（weak）中止完成回调函数 */
  HAL_UART_AbortCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}

/**
  * @brief  用户发起的 DMA UART 接收通信中止回调函数
  *         （在响应用户中止请求的 DMA 接收中止流程结束时调用）。
  * @note   当执行此回调函数时，仅在 Tx DMA 句柄上没有正在进行的中止操作时，
  *         才会调用用户中止完成回调函数。
  * @param  hdma  指向 DMA_HandleTypeDef 结构体的指针，该结构体包含
  *               指定 DMA 模块的配置信息。
  * @retval 无
  */
static void UART_DMARxAbortCallback(DMA_HandleTypeDef *hdma)
{
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  huart->hdmarx->XferAbortCallback = NULL;

  /* 检查是否仍有中止流程在进行中 */
  if (huart->hdmatx != NULL)
  {
    if (huart->hdmatx->XferAbortCallback != NULL)
    {
      return;
    }
  }

  /* 没有正在进行的中止流程：所有 DMA 通道均已中止，调用用户中止完成回调函数 */
  huart->TxXferCount = 0x00U;
  huart->RxXferCount = 0x00U;

  /* 复位 ErrorCode */
  huart->ErrorCode = HAL_UART_ERROR_NONE;

  /* 将 huart->gState 和 huart->RxState 恢复为就绪（Ready）状态 */
  huart->gState  = HAL_UART_STATE_READY;
  huart->RxState = HAL_UART_STATE_READY;
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

  /* 调用用户中止完成回调函数 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  /* 调用已注册的中止完成回调函数 */
  huart->AbortCpltCallback(huart);
#else
  /* 调用传统的弱定义（weak）中止完成回调函数 */
  HAL_UART_AbortCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}

/**
  * @brief  由用户调用 HAL_UART_AbortTransmit_IT API 发起的 DMA UART 发送通信中止回调函数（仅中止发送传输）
  *         （该回调函数在响应用户中止请求的 DMA 发送中止流程结束时执行，
  *         并会触发用户发送中止完成回调函数的执行）。
  * @param  hdma  指向 DMA_HandleTypeDef 结构体的指针，该结构体包含
  *               指定 DMA 模块的配置信息。
  * @retval 无
  */
static void UART_DMATxOnlyAbortCallback(DMA_HandleTypeDef *hdma)
{
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  huart->TxXferCount = 0x00U;

  /* 将 huart->gState 恢复为就绪（Ready）状态 */
  huart->gState = HAL_UART_STATE_READY;

  /* 调用用户中止完成回调函数 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  /* 调用已注册的中止发送完成回调函数 */
  huart->AbortTransmitCpltCallback(huart);
#else
  /* 调用传统的弱定义中止发送完成回调函数 */
  HAL_UART_AbortTransmitCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}

/**
  * @brief  当用户调用 HAL_UART_AbortReceive_IT API（仅中止接收传输）发起时，
  *         执行的 DMA UART 接收通信中止回调函数
  *         （此回调函数在用户中止请求之后的 DMA 接收中止流程结束时执行，
  *         并将触发用户接收中止完成回调函数的执行）。
  * @param  hdma  指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void UART_DMARxOnlyAbortCallback(DMA_HandleTypeDef *hdma)
{
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  huart->RxXferCount = 0x00U;

  /* 将 huart->RxState 恢复为就绪（Ready）状态 */
  huart->RxState = HAL_UART_STATE_READY;
  huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

  /* 调用用户中止完成回调函数 */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  /* 调用已注册的中止接收完成回调函数 */
  huart->AbortReceiveCpltCallback(huart);
#else
  /* 调用传统的弱定义中止接收完成回调函数 */
  HAL_UART_AbortReceiveCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
}

/**
  * @brief  在非阻塞模式下发送指定量的数据。
  * @param  huart  指向包含指定 UART 模块配置信息的 UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
static HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart)
{
  const uint16_t *tmp;

  /* 检查发送流程是否正在进行 */
  if (huart->gState == HAL_UART_STATE_BUSY_TX)
  {
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))
    {
      tmp = (const uint16_t *) huart->pTxBuffPtr;
      huart->Instance->DR = (uint16_t)(*tmp & (uint16_t)0x01FF);
      huart->pTxBuffPtr += 2U;
    }
    else
    {
      huart->Instance->DR = (uint8_t)(*huart->pTxBuffPtr++ & (uint8_t)0x00FF);
    }

    if (--huart->TxXferCount == 0U)
    {
      /* 禁用 UART 发送数据寄存器空中断 */
      __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);

      /* 使能 UART 发送完成中断 */
      __HAL_UART_ENABLE_IT(huart, UART_IT_TC);
    }
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  结束非阻塞模式下的数据发送。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
static HAL_StatusTypeDef UART_EndTransmit_IT(UART_HandleTypeDef *huart)
{
  /* 禁用 UART 发送完成中断 */
  __HAL_UART_DISABLE_IT(huart, UART_IT_TC);

  /* 发送过程结束，将 huart->gState 恢复为就绪状态（Ready） */
  huart->gState = HAL_UART_STATE_READY;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  /* 调用已注册的发送完成回调函数 */
  huart->TxCpltCallback(huart);
#else
  /* 调用传统的弱定义发送完成回调函数 */
  HAL_UART_TxCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

  return HAL_OK;
}

/**
  * @brief  在非阻塞模式下接收指定量的数据
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
static HAL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart)
{
  uint8_t  *pdata8bits = NULL;
  uint16_t *pdata16bits = NULL;

  /* 检查接收过程是否正在进行 */
  if (huart->RxState == HAL_UART_STATE_BUSY_RX)
  {
    if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))
    {
      /* 未使用 pdata8bits */
      UNUSED(pdata8bits);
      pdata16bits = (uint16_t *) huart->pRxBuffPtr;
      *pdata16bits = (uint16_t)(huart->Instance->DR & (uint16_t)0x01FF);
      huart->pRxBuffPtr += 2U;
    }
    else
    {
      pdata8bits = (uint8_t *) huart->pRxBuffPtr;
      /* 未使用 pdata16bits */
      UNUSED(pdata16bits);

      if ((huart->Init.WordLength == UART_WORDLENGTH_9B) || ((huart->Init.WordLength == UART_WORDLENGTH_8B) && (huart->Init.Parity == UART_PARITY_NONE)))
      {
        *pdata8bits = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
      }
      else
      {
        *pdata8bits = (uint8_t)(huart->Instance->DR & (uint8_t)0x007F);
      }
      huart->pRxBuffPtr += 1U;
    }

    if (--huart->RxXferCount == 0U)
    {
      /* 禁用 UART 数据寄存器非空中断 */
      __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);

      /* 禁用 UART 奇偶校验错误中断 */
      __HAL_UART_DISABLE_IT(huart, UART_IT_PE);

      /* 禁用 UART 错误中断：（帧错误、噪声错误、过载错误） */
      __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);

      /* 接收过程已完成，将 huart->RxState 恢复为就绪状态（Ready） */
      huart->RxState = HAL_UART_STATE_READY;

      /* 将 RxEvent 的类型初始化为传输完成（Transfer Complete） */
      huart->RxEventType = HAL_UART_RXEVENT_TC;

      /* 检查当前接收模式：
         如果已选择接收直至 IDLE 事件： */
      if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
      {
        /* 将接收类型设置为标准模式 */
        huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

        /* 禁用 IDLE 中断 */
        ATOMIC_CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);

        /* 检查 IDLE 标志是否置位 */
        if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
        {
          /* 在 ISR 中清除 IDLE 标志 */
          __HAL_UART_CLEAR_IDLEFLAG(huart);
        }

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
        /*调用已注册的 Rx 事件回调函数*/
        huart->RxEventCallback(huart, huart->RxXferSize);
#else
        /*调用传统的弱定义 Rx 事件回调函数*/
        HAL_UARTEx_RxEventCallback(huart, huart->RxXferSize);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
      }
      else
      {
        /* 调用了标准接收 API */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
        /*调用已注册的 Rx 完成回调函数*/
        huart->RxCpltCallback(huart);
#else
        /*调用传统的弱定义 Rx 完成回调函数*/
        HAL_UART_RxCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
      }

      return HAL_OK;
    }
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  配置 UART 外设。
  * @param  huart  指向包含指定 UART 模块配置信息的
  *                UART_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void UART_SetConfig(UART_HandleTypeDef *huart)
{
  uint32_t tmpreg;
  uint32_t pclk;

  /* 检查参数 */
  assert_param(IS_UART_BAUDRATE(huart->Init.BaudRate));
  assert_param(IS_UART_STOPBITS(huart->Init.StopBits));
  assert_param(IS_UART_PARITY(huart->Init.Parity));
  assert_param(IS_UART_MODE(huart->Init.Mode));

  /*-------------------------- USART CR2 配置 -----------------------*/
  /* 配置 UART 停止位：根据 huart->Init.StopBits 的值
     设置 STOP[13:12] 位 */
  MODIFY_REG(huart->Instance->CR2, USART_CR2_STOP, huart->Init.StopBits);

  /*-------------------------- USART CR1 配置 -----------------------*/
  /* 配置 UART 字长、校验位和模式：
     根据 huart->Init.WordLength 的值设置 M 位
     根据 huart->Init.Parity 的值设置 PCE 和 PS 位
     根据 huart->Init.Mode 的值设置 TE 和 RE 位
     根据 huart->Init.OverSampling 的值设置 OVER8 位 */

#if defined(USART_CR1_OVER8)
  tmpreg = (uint32_t)huart->Init.WordLength | huart->Init.Parity | huart->Init.Mode | huart->Init.OverSampling;
  MODIFY_REG(huart->Instance->CR1,
             (uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8),
             tmpreg);
#else
  tmpreg = (uint32_t)huart->Init.WordLength | huart->Init.Parity | huart->Init.Mode;
  MODIFY_REG(huart->Instance->CR1,
             (uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE),
             tmpreg);
#endif /* USART_CR1_OVER8 */

  /*-------------------------- USART CR3 配置 -----------------------*/
  /* 配置 UART 硬件流控：根据 huart->Init.HwFlowCtl 的值设置 CTSE 和 RTSE 位 */
  MODIFY_REG(huart->Instance->CR3, (USART_CR3_RTSE | USART_CR3_CTSE), huart->Init.HwFlowCtl);


  if(huart->Instance == USART1)
  {
    pclk = HAL_RCC_GetPCLK2Freq();
  }
  else
  {
    pclk = HAL_RCC_GetPCLK1Freq();
  }

  /*-------------------------- USART BRR 配置 ---------------------*/
#if defined(USART_CR1_OVER8)
  if (huart->Init.OverSampling == UART_OVERSAMPLING_8)
  {
    huart->Instance->BRR = UART_BRR_SAMPLING8(pclk, huart->Init.BaudRate);
  }
  else
  {
    huart->Instance->BRR = UART_BRR_SAMPLING16(pclk, huart->Init.BaudRate);
  }
#else
  huart->Instance->BRR = UART_BRR_SAMPLING16(pclk, huart->Init.BaudRate);
#endif /* USART_CR1_OVER8 */
}

/**
  * @}
  */

#endif /* HAL_UART_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */


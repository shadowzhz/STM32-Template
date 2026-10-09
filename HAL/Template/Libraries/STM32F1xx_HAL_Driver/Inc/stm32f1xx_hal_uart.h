/**
  ******************************************************************************
  * @file    stm32f1xx_hal_uart.h
  * @author  MCD Application Team
  * @brief   UART HAL 模块头文件。
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * 保留所有权利。
  *
  * 本软件根据可在该软件组件根目录下的 LICENSE 文件中找到的条款获得许可。
  * 如果本软件未随附 LICENSE 文件，则按“原样 (AS-IS)”提供。
  *
  ******************************************************************************
  */

/* 防止递归包含的宏定义 ------------------------------------------------------*/
#ifndef __STM32F1xx_HAL_UART_H
#define __STM32F1xx_HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup UART
  * @{
  */

/* 导出类型 ------------------------------------------------------------------*/
/** @defgroup UART_Exported_Types UART 导出类型
  * @{
  */

/**
  * @brief UART 初始化结构体定义
  */
typedef struct
{
  uint32_t BaudRate;                  /*!< 该成员用于配置 UART 通信波特率。
                                           波特率使用以下公式计算：
                                           - IntegerDivider = ((PCLKx) / (16 * (huart->Init.BaudRate)))
                                           - FractionalDivider = ((IntegerDivider - ((uint32_t) IntegerDivider)) * 16) + 0.5 */

  uint32_t WordLength;                /*!< 指定一帧中发送或接收的数据位宽。
                                           该参数可以是 @ref UART_Word_Length 的取值之一 */

  uint32_t StopBits;                  /*!< 指定发送的停止位数量。
                                           该参数可以是 @ref UART_Stop_Bits 的取值之一 */

  uint32_t Parity;                    /*!< 指定奇偶校验模式。
                                           该参数可以是 @ref UART_Parity 的取值之一
                                           @note 当使能奇偶校验时，计算出的奇偶校验位将插入到
                                                 发送数据的最高有效位 (MSB) 位置（当字长设置为 9 位数据位时为第 9 位；
                                                 当字长设置为 8 位数据位时为第 8 位）。 */

  uint32_t Mode;                      /*!< 指定是使能还是禁用接收或发送模式。
                                           该参数可以是 @ref UART_Mode 的取值之一 */

  uint32_t HwFlowCtl;                 /*!< 指定是使能还是禁用硬件流控制模式。
                                           该参数可以是 @ref UART_Hardware_Flow_Control 的取值之一 */

  uint32_t OverSampling;              /*!< 指定是使能还是禁用 8 倍过采样，以实现更高的通信速率（最高可达 fPCLK/8）。
                                           该参数可以是 @ref UART_Over_Sampling 的取值之一。该特性仅在
                                           STM32F100xx 系列微控制器上可用，因此 OverSampling 参数应始终设置为 16。 */
} UART_InitTypeDef;

/**
  * @brief HAL UART 状态结构体定义
  * @note  HAL UART 状态值由 2 个不同的子状态组合而成：gState 和 RxState。
  *        - gState 包含与全局句柄管理相关的 UART 状态信息，
  *          以及与发送（Tx）操作相关的信息。
  *          gState 值的编码遵循以下位图描述：
  *          b7-b6  错误信息
  *             00 : 无错误
  *             01 : （未保留/未定义）
  *             10 : 超时
  *             11 : 错误
  *          b5     外设初始化状态
  *             0  : 复位（外设未初始化）
  *             1  : 初始化完成（外设已初始化，HAL UART Init 函数已被调用）
  *          b4-b3  （保留未用）
  *             xx : 应设置为 00
  *          b2     内部处理状态
  *             0  : 就绪
  *             1  : 忙（外设正忙于某些配置或内部操作）
  *          b1     （保留未用）
  *             x  : 应设置为 0
  *          b0     Tx 状态
  *             0  : 就绪（无正在进行的 Tx 操作）
  *             1  : 忙（Tx 操作正在进行）
  *        - RxState 包含与接收（Rx）操作相关的信息。
  *          RxState 值的编码遵循以下位图描述：
  *          b7-b6  （保留未用）
  *             xx : 应设置为 00
  *          b5     外设初始化状态
  *             0  : 复位（外设未初始化）
  *             1  : 初始化完成（外设已初始化）
  *          b4-b2  （保留未用）
  *            xxx : 应设置为 000
  *          b1     Rx 状态
  *             0  : 就绪（无正在进行的 Rx 操作）
  *             1  : 忙（Rx 操作正在进行）
  *          b0     （保留未用）
  *             x  : 应设置为 0。
  */
typedef enum
{
  HAL_UART_STATE_RESET             = 0x00U,    /*!< 外设尚未初始化
                                                   此值适用于 gState 和 RxState */
  HAL_UART_STATE_READY             = 0x20U,    /*!< 外设已初始化并就绪可用
                                                   此值适用于 gState 和 RxState */
  HAL_UART_STATE_BUSY              = 0x24U,    /*!< 内部处理正在进行中
                                                   此值仅适用于 gState */
  HAL_UART_STATE_BUSY_TX           = 0x21U,    /*!< 数据发送处理正在进行中
                                                   此值仅适用于 gState */
  HAL_UART_STATE_BUSY_RX           = 0x22U,    /*!< 数据接收处理正在进行中
                                                   此值仅适用于 RxState */
  HAL_UART_STATE_BUSY_TX_RX        = 0x23U,    /*!< 数据发送和接收处理均在进行中
                                                   不能单独用于 gState 或 RxState。
                                                   该值是 gState 和 RxState 值按位或（Or）组合的结果 */
  HAL_UART_STATE_TIMEOUT           = 0xA0U,    /*!< 超时状态
                                                   此值仅适用于 gState */
  HAL_UART_STATE_ERROR             = 0xE0U     /*!< 错误状态
                                                   此值仅适用于 gState */
} HAL_UART_StateTypeDef;

/**
  * @brief HAL UART 接收类型定义
  * @note  HAL UART 接收类型值用于标识当前正在进行的接收类型。
  *        该参数可以是 @ref UART_Reception_Type_Values 的取值之一：
  *           HAL_UART_RECEPTION_STANDARD         = 0x00U,
  *           HAL_UART_RECEPTION_TOIDLE           = 0x01U,
  */
typedef uint32_t HAL_UART_RxTypeTypeDef;

/**
  * @brief HAL UART 接收事件类型定义
  * @note  HAL UART 接收事件类型值用于标识触发 RxEvent 回调函数调用的
  *        事件类型。
  *        该参数可以是 @ref UART_RxEvent_Type_Values 的取值之一：
  *           HAL_UART_RXEVENT_TC                 = 0x00U,
  *           HAL_UART_RXEVENT_HT                 = 0x01U,
  *           HAL_UART_RXEVENT_IDLE               = 0x02U,
  */
typedef uint32_t HAL_UART_RxEventTypeTypeDef;

/**
  * @brief  UART 句柄结构体定义
  */
typedef struct __UART_HandleTypeDef
{
  USART_TypeDef                 *Instance;        /*!< UART 寄存器基地址                  */

  UART_InitTypeDef              Init;             /*!< UART 通信参数                      */

  const uint8_t                 *pTxBuffPtr;      /*!< 指向 UART 发送数据缓冲区的指针     */

  uint16_t                      TxXferSize;       /*!< UART 发送传输大小                  */

  __IO uint16_t                 TxXferCount;      /*!< UART 发送传输计数器                */

  uint8_t                       *pRxBuffPtr;      /*!< 指向 UART 接收传输缓冲区的指针     */

  uint16_t                      RxXferSize;       /*!< UART 接收传输大小                  */

  __IO uint16_t                 RxXferCount;      /*!< UART 接收传输计数器                */

  __IO HAL_UART_RxTypeTypeDef ReceptionType;      /*!< 当前正在进行的接收类型             */

  __IO HAL_UART_RxEventTypeTypeDef RxEventType;   /*!< 接收事件类型                       */

  DMA_HandleTypeDef             *hdmatx;          /*!< UART 发送 DMA 句柄参数             */

  DMA_HandleTypeDef             *hdmarx;          /*!< UART 接收 DMA 句柄参数             */

  HAL_LockTypeDef               Lock;             /*!< 锁对象                             */

  __IO HAL_UART_StateTypeDef    gState;           /*!< 与全局句柄管理以及发送操作相关的 UART 状态信息。
                                                       该参数可以是 @ref HAL_UART_StateTypeDef 的值 */

  __IO HAL_UART_StateTypeDef    RxState;          /*!< 与接收操作相关的 UART 状态信息。
                                                       该参数可以是 @ref HAL_UART_StateTypeDef 的值 */

  __IO uint32_t                 ErrorCode;        /*!< UART 错误代码                      */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  void (* TxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);        /*!< UART 发送过半完成回调函数          */
  void (* TxCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART 发送完成回调函数              */
  void (* RxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);        /*!< UART 接收过半完成回调函数             */
  void (* RxCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART 接收完成回调函数                 */
  void (* ErrorCallback)(struct __UART_HandleTypeDef *huart);             /*!< UART 错误回调函数                     */
  void (* AbortCpltCallback)(struct __UART_HandleTypeDef *huart);         /*!< UART 中止完成回调函数                 */
  void (* AbortTransmitCpltCallback)(struct __UART_HandleTypeDef *huart); /*!< UART 发送中止完成回调函数             */
  void (* AbortReceiveCpltCallback)(struct __UART_HandleTypeDef *huart);  /*!< UART 接收中止完成回调函数             */
  void (* WakeupCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART 唤醒回调函数                     */
  void (* RxEventCallback)(struct __UART_HandleTypeDef *huart, uint16_t Pos); /*!< UART 接收事件回调函数                 */

  void (* MspInitCallback)(struct __UART_HandleTypeDef *huart);           /*!< UART Msp 初始化回调函数               */
  void (* MspDeInitCallback)(struct __UART_HandleTypeDef *huart);         /*!< UART Msp 反初始化回调函数             */
#endif  /* USE_HAL_UART_REGISTER_CALLBACKS */

} UART_HandleTypeDef;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL UART 回调函数 ID 枚举定义
  */
typedef enum
{
  HAL_UART_TX_HALFCOMPLETE_CB_ID         = 0x00U,    /*!< UART 发送过半完成回调函数 ID             */
  HAL_UART_TX_COMPLETE_CB_ID             = 0x01U,    /*!< UART 发送完成回调函数 ID                 */
  HAL_UART_RX_HALFCOMPLETE_CB_ID         = 0x02U,    /*!< UART 接收过半完成回调函数 ID             */
  HAL_UART_RX_COMPLETE_CB_ID             = 0x03U,    /*!< UART 接收完成回调函数 ID                 */
  HAL_UART_ERROR_CB_ID                   = 0x04U,    /*!< UART 错误回调函数 ID                     */
  HAL_UART_ABORT_COMPLETE_CB_ID          = 0x05U,    /*!< UART 中止完成回调函数 ID                 */
  HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID = 0x06U,    /*!< UART 中止发送完成回调函数 ID             */
  HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID  = 0x07U,    /*!< UART 中止接收完成回调函数 ID             */
  HAL_UART_WAKEUP_CB_ID                  = 0x08U,    /*!< UART 唤醒回调函数 ID                     */

  HAL_UART_MSPINIT_CB_ID                 = 0x0BU,    /*!< UART MspInit 回调函数 ID                 */
  HAL_UART_MSPDEINIT_CB_ID               = 0x0CU     /*!< UART MspDeInit 回调函数 ID               */

} HAL_UART_CallbackIDTypeDef;

/**
  * @brief  HAL UART 回调函数指针定义
  */
typedef  void (*pUART_CallbackTypeDef)(UART_HandleTypeDef *huart);  /*!< 指向 UART 回调函数的指针 */
typedef  void (*pUART_RxEventCallbackTypeDef)(struct __UART_HandleTypeDef *huart, uint16_t Pos);   /*!< 指向 UART 接收事件专用回调函数的指针 */

#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

/**
  * @}
  */

/* 导出常量 ------------------------------------------------------------------*/
/** @defgroup UART_Exported_Constants UART 导出常量
  * @{
  */

/** @defgroup UART_Error_Code UART 错误代码
  * @{
  */
#define HAL_UART_ERROR_NONE              0x00000000U   /*!< 无错误              */
#define HAL_UART_ERROR_PE                0x00000001U   /*!< 奇偶校验错误        */
#define HAL_UART_ERROR_NE                0x00000002U   /*!< 噪声错误            */
#define HAL_UART_ERROR_FE                0x00000004U   /*!< 帧错误              */
#define HAL_UART_ERROR_ORE               0x00000008U   /*!< 过载错误            */
#define HAL_UART_ERROR_DMA               0x00000010U   /*!< DMA 传输错误        */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
#define  HAL_UART_ERROR_INVALID_CALLBACK 0x00000020U   /*!< 无效回调错误        */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup UART_Word_Length UART 数据字长
  * @{
  */
#define UART_WORDLENGTH_8B                  0x00000000U
#define UART_WORDLENGTH_9B                  ((uint32_t)USART_CR1_M)
/**
  * @}
  */

/** @defgroup UART_Stop_Bits UART 停止位个数
  * @{
  */
#define UART_STOPBITS_1                     0x00000000U
#define UART_STOPBITS_2                     ((uint32_t)USART_CR2_STOP_1)
/**
  * @}
  */

/** @defgroup UART_Parity UART 奇偶校验
  * @{
  */
#define UART_PARITY_NONE                    0x00000000U
#define UART_PARITY_EVEN                    ((uint32_t)USART_CR1_PCE)
#define UART_PARITY_ODD                     ((uint32_t)(USART_CR1_PCE | USART_CR1_PS))
/**
  * @}
  */

/** @defgroup UART_Hardware_Flow_Control UART 硬件流控制
  * @{
  */
#define UART_HWCONTROL_NONE                  0x00000000U
#define UART_HWCONTROL_RTS                   ((uint32_t)USART_CR3_RTSE)
#define UART_HWCONTROL_CTS                   ((uint32_t)USART_CR3_CTSE)
#define UART_HWCONTROL_RTS_CTS               ((uint32_t)(USART_CR3_RTSE | USART_CR3_CTSE))
/**
  * @}
  */

/** @defgroup UART_Mode UART 传输模式
  * @{
  */
#define UART_MODE_RX                        ((uint32_t)USART_CR1_RE)
#define UART_MODE_TX                        ((uint32_t)USART_CR1_TE)
#define UART_MODE_TX_RX                     ((uint32_t)(USART_CR1_TE | USART_CR1_RE))
/**
  * @}
  */

/** @defgroup UART_State UART 状态
  * @{
  */
#define UART_STATE_DISABLE                  0x00000000U
#define UART_STATE_ENABLE                   ((uint32_t)USART_CR1_UE)
/**
  * @}
  */

/** @defgroup UART_Over_Sampling UART 过采样
  * @{
  */
#define UART_OVERSAMPLING_16                    0x00000000U
#if defined(USART_CR1_OVER8)
#define UART_OVERSAMPLING_8                     ((uint32_t)USART_CR1_OVER8)
#endif /* USART_CR1_OVER8 */
/**
  * @}
  */

/** @defgroup UART_LIN_Break_Detection_Length  UART LIN 断开符检测长度
  * @{
  */
#define UART_LINBREAKDETECTLENGTH_10B      0x00000000U
#define UART_LINBREAKDETECTLENGTH_11B      ((uint32_t)USART_CR2_LBDL)
/**
  * @}
  */

/** @defgroup UART_WakeUp_functions  UART 唤醒功能
  * @{
  */
#define UART_WAKEUPMETHOD_IDLELINE                0x00000000U
#define UART_WAKEUPMETHOD_ADDRESSMARK             ((uint32_t)USART_CR1_WAKE)
/**
  * @}
  */

/** @defgroup UART_Flags   UART 标志位
  *        元素取值约定：0xXXXX
  *           - 0xXXXX  : SR 寄存器中的标志位掩码
  * @{
  */
#define UART_FLAG_CTS                       ((uint32_t)USART_SR_CTS)
#define UART_FLAG_LBD                       ((uint32_t)USART_SR_LBD)
#define UART_FLAG_TXE                       ((uint32_t)USART_SR_TXE)
#define UART_FLAG_TC                        ((uint32_t)USART_SR_TC)
#define UART_FLAG_RXNE                      ((uint32_t)USART_SR_RXNE)
#define UART_FLAG_IDLE                      ((uint32_t)USART_SR_IDLE)
#define UART_FLAG_ORE                       ((uint32_t)USART_SR_ORE)
#define UART_FLAG_NE                        ((uint32_t)USART_SR_NE)
#define UART_FLAG_FE                        ((uint32_t)USART_SR_FE)
#define UART_FLAG_PE                        ((uint32_t)USART_SR_PE)
/**
  * @}
  */

/** @defgroup UART_Interrupt_definition UART 中断定义
  *        元素取值约定：0xY000XXXX
  *           - XXXX  : Y 寄存器中的中断屏蔽位 (16 bits)
  *           - Y  : 中断源寄存器 (2bits)
  *                   - 0001: CR1 寄存器
  *                   - 0010: CR2 寄存器
  *                   - 0011: CR3 寄存器
  * @{
  */

#define UART_IT_PE                       ((uint32_t)(UART_CR1_REG_INDEX << 28U | USART_CR1_PEIE))
#define UART_IT_TXE                      ((uint32_t)(UART_CR1_REG_INDEX << 28U | USART_CR1_TXEIE))
#define UART_IT_TC                       ((uint32_t)(UART_CR1_REG_INDEX << 28U | USART_CR1_TCIE))
#define UART_IT_RXNE                     ((uint32_t)(UART_CR1_REG_INDEX << 28U | USART_CR1_RXNEIE))
#define UART_IT_IDLE                     ((uint32_t)(UART_CR1_REG_INDEX << 28U | USART_CR1_IDLEIE))

#define UART_IT_LBD                      ((uint32_t)(UART_CR2_REG_INDEX << 28U | USART_CR2_LBDIE))

#define UART_IT_CTS                      ((uint32_t)(UART_CR3_REG_INDEX << 28U | USART_CR3_CTSIE))
#define UART_IT_ERR                      ((uint32_t)(UART_CR3_REG_INDEX << 28U | USART_CR3_EIE))
/**
  * @}
  */

/** @defgroup UART_Reception_Type_Values UART 接收类型值
  * @{
  */
#define HAL_UART_RECEPTION_STANDARD          (0x00000000U)             /*!< 标准接收                                 */
#define HAL_UART_RECEPTION_TOIDLE            (0x00000001U)             /*!< 接收直至完成或产生空闲 (IDLE) 事件       */
/**
  * @}
  */

/** @defgroup UART_RxEvent_Type_Values UART 接收事件 (RxEvent) 类型值
  * @{
  */
#define HAL_UART_RXEVENT_TC                  (0x00000000U)             /*!< 关联到传输完成 (Transfer Complete) 事件的 RxEvent */
#define HAL_UART_RXEVENT_HT                  (0x00000001U)             /*!< 关联到过半传输 (Half Transfer) 事件的 RxEvent     */
#define HAL_UART_RXEVENT_IDLE                (0x00000002U)
/**
  * @}
  */

/**
  * @}
  */

/* 导出宏 --------------------------------------------------------------------*/
/** @defgroup UART_Exported_Macros UART 导出的宏
  * @{
  */

/** @brief 复位 UART 句柄的 gstate 和 RxState
  * @param  __HANDLE__ 指定 UART 句柄。
  *         UART 句柄用于选择 USARTx 或 UARTy 外设
  *         (USART、UART 的可用性以及 x、y 的取值取决于具体器件)。
  * @retval 无
  */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
#define __HAL_UART_RESET_HANDLE_STATE(__HANDLE__)  do{                                                   \
                                                       (__HANDLE__)->gState = HAL_UART_STATE_RESET;      \
                                                       (__HANDLE__)->RxState = HAL_UART_STATE_RESET;     \
                                                       (__HANDLE__)->MspInitCallback = NULL;             \
                                                       (__HANDLE__)->MspDeInitCallback = NULL;           \
                                                     } while(0U)
#else
#define __HAL_UART_RESET_HANDLE_STATE(__HANDLE__)  do{                                                   \
                                                       (__HANDLE__)->gState = HAL_UART_STATE_RESET;      \
                                                       (__HANDLE__)->RxState = HAL_UART_STATE_RESET;     \
                                                     } while(0U)
#endif /*USE_HAL_UART_REGISTER_CALLBACKS */

/** @brief  刷新（清空）UART DR 寄存器
  * @param  __HANDLE__ 指定 UART 句柄。
  *         UART 句柄用于选择 USARTx 或 UARTy 外设
  *         （USART、UART 的可用性及 x、y 的取值取决于具体器件）。
  */
#define __HAL_UART_FLUSH_DRREGISTER(__HANDLE__) ((__HANDLE__)->Instance->DR)

/** @brief  检查指定的 UART 标志位是否置位。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         UART 句柄用于选择 USARTx 或 UARTy 外设
  *         （USART、UART 的可用性及 x、y 的取值取决于具体器件）。
  * @param  __FLAG__ 指定要检查的标志位。
  *        该参数可以是以下值之一：
  *            @arg UART_FLAG_CTS:  CTS 改变标志（不适用于 UART4 和 UART5）
  *            @arg UART_FLAG_LBD:  LIN 断开符号检测标志
  *            @arg UART_FLAG_TXE:  发送数据寄存器空中断标志
  *            @arg UART_FLAG_TC:   发送完成标志
  *            @arg UART_FLAG_RXNE: 接收数据寄存器非空标志
  *            @arg UART_FLAG_IDLE: 空闲总线检测标志
  *            @arg UART_FLAG_ORE:  过载错误标志
  *            @arg UART_FLAG_NE:   噪声错误标志
  *            @arg UART_FLAG_FE:   帧错误标志
  *            @arg UART_FLAG_PE:   奇偶校验错误标志
  * @retval __FLAG__ 的新状态（TRUE 或 FALSE）。
  */
#define __HAL_UART_GET_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->Instance->SR & (__FLAG__)) == (__FLAG__))

/** @brief  清除指定的 UART 挂起标志位。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         UART 句柄用于选择 USARTx 或 UARTy 外设
  *         （USART、UART 的可用性及 x、y 的取值取决于具体器件）。
  * @param  __FLAG__ 指定要检查的标志位。
  *          该参数可以是以下值的任意组合：
  *            @arg UART_FLAG_CTS:  CTS 改变标志（不适用于 UART4 和 UART5）。
  *            @arg UART_FLAG_LBD:  LIN 断开符号检测标志。
  *            @arg UART_FLAG_TC:   发送完成标志。
  *            @arg UART_FLAG_RXNE: 接收数据寄存器非空标志。
  *
  * @note   PE（奇偶校验错误）、FE（帧错误）、NE（噪声错误）、ORE（过载错误）
  *          和 IDLE（空闲线路检测）标志通过软件序列清除：先对 USART_SR
  *          寄存器执行读操作，随后对 USART_DR 寄存器执行读操作。
  * @note   RXNE 标志也可以通过读取 USART_DR 寄存器来清除。
  * @note   TC 标志也可以通过软件序列清除：先对 USART_SR 寄存器执行读操作，
  *          随后对 USART_DR 寄存器执行写操作。
  * @note   TXE 标志只能通过对 USART_DR 寄存器执行写操作来清除。
  *
  * @retval 无
  */
#define __HAL_UART_CLEAR_FLAG(__HANDLE__, __FLAG__) ((__HANDLE__)->Instance->SR = ~(__FLAG__))

/** @brief  清除 UART PE 挂起标志位。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         UART 句柄用于选择 USARTx 或 UARTy 外设
  *         （USART、UART 的可用性及 x、y 的取值取决于具体器件）。
  * @retval 无
  */
#define __HAL_UART_CLEAR_PEFLAG(__HANDLE__)     \
  do{                                           \
    __IO uint32_t tmpreg = 0x00U;               \
    tmpreg = (__HANDLE__)->Instance->SR;        \
    tmpreg = (__HANDLE__)->Instance->DR;        \
    UNUSED(tmpreg);                             \
  } while(0U)

/** @brief  清除 UART FE 挂起标志位。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         UART 句柄用于选择 USARTx 或 UARTy 外设
  *         （USART、UART 的可用性及 x、y 的取值取决于具体器件）。
  * @retval 无
  */
#define __HAL_UART_CLEAR_FEFLAG(__HANDLE__) __HAL_UART_CLEAR_PEFLAG(__HANDLE__)

/** @brief  清除 UART NE 挂起标志位。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         UART 句柄用于选择 USARTx 或 UARTy 外设
  *         （USART、UART 的可用性及 x、y 的取值取决于具体器件）。
  * @retval 无
  */
#define __HAL_UART_CLEAR_NEFLAG(__HANDLE__) __HAL_UART_CLEAR_PEFLAG(__HANDLE__)

/** @brief  清除 UART ORE 挂起标志位。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         UART 句柄用于选择 USARTx 或 UARTy 外设
  *         （USART、UART 的可用性及 x、y 的取值取决于具体器件）。
  * @retval 无
  */
#define __HAL_UART_CLEAR_OREFLAG(__HANDLE__) __HAL_UART_CLEAR_PEFLAG(__HANDLE__)

/** @brief  清除 UART IDLE 挂起标志位。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         UART 句柄用于选择 USARTx 或 UARTy 外设
  *         （USART、UART 的可用性及 x、y 的取值取决于具体器件）。
  * @retval 无
  */
#define __HAL_UART_CLEAR_IDLEFLAG(__HANDLE__) __HAL_UART_CLEAR_PEFLAG(__HANDLE__)

/** @brief  使能指定的 UART 中断。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         UART 句柄用于选择 USARTx 或 UARTy 外设
  *         （USART、UART 的可用性及 x、y 的取值取决于具体器件）。
  * @param  __INTERRUPT__ 指定要使能的 UART 中断源。
  *          该参数可以是以下值之一：
  *            @arg UART_IT_CTS:  CTS 改变中断
  *            @arg UART_IT_LBD:  LIN 断开符号检测中断
  *            @arg UART_IT_TXE:  发送数据寄存器空中断
  *            @arg UART_IT_TC:   发送完成中断
  *            @arg UART_IT_RXNE: 接收数据寄存器非空中断
  *            @arg UART_IT_IDLE: 空闲总线检测中断
  *            @arg UART_IT_PE:   奇偶校验错误中断
  *            @arg UART_IT_ERR:  错误中断（帧错误、噪声错误、过载错误）
  * @retval 无
  */
#define __HAL_UART_ENABLE_IT(__HANDLE__, __INTERRUPT__)   ((((__INTERRUPT__) >> 28U) == UART_CR1_REG_INDEX)? ((__HANDLE__)->Instance->CR1 |= ((__INTERRUPT__) & UART_IT_MASK)): \
                                                           (((__INTERRUPT__) >> 28U) == UART_CR2_REG_INDEX)? ((__HANDLE__)->Instance->CR2 |= ((__INTERRUPT__) & UART_IT_MASK)): \
                                                           ((__HANDLE__)->Instance->CR3 |= ((__INTERRUPT__) & UART_IT_MASK)))

/** @brief  禁用指定的 UART 中断。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         UART 句柄用于选择 USARTx 或 UARTy 外设
  *         （USART、UART 的可用性及 x、y 的取值取决于具体器件）。
  * @param  __INTERRUPT__ 指定要禁用的 UART 中断源。
  *          该参数可以是以下值之一：
  *            @arg UART_IT_CTS:  CTS 改变中断
  *            @arg UART_IT_LBD:  LIN 断开符号检测中断
  *            @arg UART_IT_TXE:  发送数据寄存器空中断
  *            @arg UART_IT_TC:   发送完成中断
  *            @arg UART_IT_RXNE: 接收数据寄存器非空中断
  *            @arg UART_IT_IDLE: 空闲总线检测中断
  *            @arg UART_IT_PE:   奇偶校验错误中断
  *            @arg UART_IT_ERR:  错误中断（帧错误、噪声错误、过载错误）
  * @retval 无
  */
#define __HAL_UART_DISABLE_IT(__HANDLE__, __INTERRUPT__)  ((((__INTERRUPT__) >> 28U) == UART_CR1_REG_INDEX)? ((__HANDLE__)->Instance->CR1 &= ~((__INTERRUPT__) & UART_IT_MASK)): \
                                                           (((__INTERRUPT__) >> 28U) == UART_CR2_REG_INDEX)? ((__HANDLE__)->Instance->CR2 &= ~((__INTERRUPT__) & UART_IT_MASK)): \
                                                           ((__HANDLE__)->Instance->CR3 &= ~ ((__INTERRUPT__) & UART_IT_MASK)))

/** @brief  检查指定的 UART 中断源是否已使能。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         UART 句柄用于选择 USARTx 或 UARTy 外设
  *         （USART、UART 的可用性及 x、y 的取值取决于具体器件）。
  * @param  __IT__ 指定要检查的 UART 中断源。
  *          该参数可以是以下值之一：
  *            @arg UART_IT_CTS: CTS 改变中断（不适用于 UART4 和 UART5）
  *            @arg UART_IT_LBD: LIN 断开符号检测中断
  *            @arg UART_IT_TXE: 发送数据寄存器空中断
  *            @arg UART_IT_TC:  发送完成中断
  *            @arg UART_IT_RXNE: 接收数据寄存器非空中断
  *            @arg UART_IT_IDLE: 空闲总线检测中断
  *            @arg UART_IT_ERR: 错误中断
  * @retval __IT__ 的新状态（TRUE 或 FALSE）。
  */
#define __HAL_UART_GET_IT_SOURCE(__HANDLE__, __IT__) (((((__IT__) >> 28U) == UART_CR1_REG_INDEX)? (__HANDLE__)->Instance->CR1:(((((uint32_t)(__IT__)) >> 28U) == UART_CR2_REG_INDEX)? \
                                                       (__HANDLE__)->Instance->CR2 : (__HANDLE__)->Instance->CR3)) & (((uint32_t)(__IT__)) & UART_IT_MASK))

/** @brief  使能 CTS 流控制
  * @note   该宏允许为给定的 UART 实例使能 CTS 硬件流控制，
  *         而无需调用 HAL_UART_Init() 函数。
  *         由于涉及对 UART 寄存器的直接访问，使用该宏的风险应由用户完全承担。
  * @note   由于该宏用于修改 CTS 硬件流控制特性的使能状态，而无需
  *         对 USART 实例执行反初始化/初始化（Deinit/Init），因此调用该宏应满足以下条件：
  *           - UART 实例应已完成初始化（通过调用 HAL_UART_Init()）
  *           - 只有在对应的 UART 实例被禁用（即 __HAL_UART_DISABLE(__HANDLE__)）时才可调用该宏，
  *             并且调用后应紧跟使能宏（即 __HAL_UART_ENABLE(__HANDLE__)）。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         句柄实例可以是任意 USARTx（支持硬件流控制特性）。
  *         用于选择 USART 外设（USART 的可用性及 x 的取值取决于具体器件）。
  * @retval 无
  */
#define __HAL_UART_HWCONTROL_CTS_ENABLE(__HANDLE__)        \
  do{                                                      \
    ATOMIC_SET_BIT((__HANDLE__)->Instance->CR3, USART_CR3_CTSE);  \
    (__HANDLE__)->Init.HwFlowCtl |= USART_CR3_CTSE;        \
  } while(0U)

/** @brief  禁用 CTS 流控制
  * @note   该宏允许为给定的 UART 实例禁用 CTS 硬件流控制，
  *         而无需调用 HAL_UART_Init() 函数。
  *         由于涉及对 UART 寄存器的直接访问，使用该宏的风险应由用户完全承担。
  * @note   由于该宏用于修改 CTS 硬件流控制特性的使能状态，而无需
  *         对 USART 实例执行反初始化/初始化（Deinit/Init），因此调用该宏应满足以下条件：
  *           - UART 实例应已完成初始化（通过调用 HAL_UART_Init()）
  *           - 只有在对应的 UART 实例被禁用（即 __HAL_UART_DISABLE(__HANDLE__)）时才可调用该宏，
  *             并且调用后应紧跟使能宏（即 __HAL_UART_ENABLE(__HANDLE__)）。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         句柄实例可以是任意 USARTx（支持硬件流控制特性）。
  *         用于选择 USART 外设（USART 的可用性及 x 的取值取决于具体器件）。
  * @retval 无
  */
#define __HAL_UART_HWCONTROL_CTS_DISABLE(__HANDLE__)        \
  do{                                                       \
    ATOMIC_CLEAR_BIT((__HANDLE__)->Instance->CR3, USART_CR3_CTSE); \
    (__HANDLE__)->Init.HwFlowCtl &= ~(USART_CR3_CTSE);      \
  } while(0U)

/** @brief  使能 RTS 流控制
  *         该宏允许为给定的 UART 实例使能 RTS 硬件流控制，
  *         而无需调用 HAL_UART_Init() 函数。
  *         由于涉及对 UART 寄存器的直接访问，使用该宏的风险应由用户完全承担。
  * @note   由于该宏用于修改 RTS 硬件流控制特性的使能状态，而无需
  *         对 USART 实例执行反初始化/初始化（Deinit/Init），因此调用该宏应满足以下条件：
  *           - UART 实例应已完成初始化（通过调用 HAL_UART_Init()）
  *           - 只有在对应的 UART 实例被禁用（即 __HAL_UART_DISABLE(__HANDLE__)）时才可调用该宏，
  *             并且调用后应紧跟使能宏（即 __HAL_UART_ENABLE(__HANDLE__)）。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         句柄实例可以是任意 USARTx（支持硬件流控制特性）。
  *         用于选择 USART 外设（USART 的可用性及 x 的取值取决于具体器件）。
  * @retval 无
  */
#define __HAL_UART_HWCONTROL_RTS_ENABLE(__HANDLE__)       \
  do{                                                     \
    ATOMIC_SET_BIT((__HANDLE__)->Instance->CR3, USART_CR3_RTSE); \
    (__HANDLE__)->Init.HwFlowCtl |= USART_CR3_RTSE;       \
  } while(0U)

/** @brief  禁用 RTS 流控制
  *         该宏允许在给定的 UART 实例上禁用 RTS 硬件流控制，
  *         而无需调用 HAL_UART_Init() 函数。
  *         由于涉及对 UART 寄存器的直接访问，用户需自行承担使用该宏的风险。
  * @note   由于该宏旨在修改 RTS 硬件流控制功能的激活状态，且无需
  *         对 USART 实例执行反初始化/初始化（Deinit/Init），因此调用该宏需满足以下条件：
  *           - UART 实例必须已经完成初始化（通过调用 HAL_UART_Init()）
  *           - 仅当对应的 UART 实例被禁用（即 __HAL_UART_DISABLE(__HANDLE__)）时才能调用该宏，
  *             且调用后应紧跟使能宏（即 __HAL_UART_ENABLE(__HANDLE__)）。
  * @param  __HANDLE__ 指定 UART 句柄。
  *         句柄实例可以是任意支持硬件流控制功能的 USARTx。
  *         用于选择 USART 外设（USART 可用性及 x 的取值取决于具体器件型号）。
  * @retval 无
  */
#define __HAL_UART_HWCONTROL_RTS_DISABLE(__HANDLE__)       \
  do{                                                      \
    ATOMIC_CLEAR_BIT((__HANDLE__)->Instance->CR3, USART_CR3_RTSE);\
    (__HANDLE__)->Init.HwFlowCtl &= ~(USART_CR3_RTSE);     \
  } while(0U)
#if defined(USART_CR3_ONEBIT)

/** @brief  用于使能 UART 单位采样（one bit sample）方法的宏
  * @param  __HANDLE__ 指定 UART 句柄。
  * @retval 无
  */
#define __HAL_UART_ONE_BIT_SAMPLE_ENABLE(__HANDLE__) ((__HANDLE__)->Instance->CR3|= USART_CR3_ONEBIT)

/** @brief  用于禁用 UART 单位采样（one bit sample）方法的宏
  * @param  __HANDLE__ 指定 UART 句柄。
  * @retval 无
  */
#define __HAL_UART_ONE_BIT_SAMPLE_DISABLE(__HANDLE__) ((__HANDLE__)->Instance->CR3\
                                                       &= (uint16_t)~((uint16_t)USART_CR3_ONEBIT))
#endif /* UART_ONE_BIT_SAMPLE_Feature */

/** @brief  使能 UART
  * @param  __HANDLE__ 指定 UART 句柄。
  * @retval 无
  */
#define __HAL_UART_ENABLE(__HANDLE__)               ((__HANDLE__)->Instance->CR1 |=  USART_CR1_UE)

/** @brief  禁用 UART
  * @param  __HANDLE__ 指定 UART 句柄。
  * @retval 无
  */
#define __HAL_UART_DISABLE(__HANDLE__)              ((__HANDLE__)->Instance->CR1 &=  ~USART_CR1_UE)
/**
  * @}
  */

/* 导出函数 --------------------------------------------------------*/
/** @addtogroup UART_Exported_Functions
  * @{
  */

/** @addtogroup UART_Exported_Functions_Group1 初始化与反初始化函数
  * @{
  */

/* 初始化/反初始化函数  **********************************/
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_HalfDuplex_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_LIN_Init(UART_HandleTypeDef *huart, uint32_t BreakDetectLength);
HAL_StatusTypeDef HAL_MultiProcessor_Init(UART_HandleTypeDef *huart, uint8_t Address, uint32_t WakeUpMethod);
HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef *huart);
void HAL_UART_MspInit(UART_HandleTypeDef *huart);
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart);

/* 回调注册/注销函数  ***********************************/
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
HAL_StatusTypeDef HAL_UART_RegisterCallback(UART_HandleTypeDef *huart, HAL_UART_CallbackIDTypeDef CallbackID,
                                            pUART_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_UART_UnRegisterCallback(UART_HandleTypeDef *huart, HAL_UART_CallbackIDTypeDef CallbackID);

HAL_StatusTypeDef HAL_UART_RegisterRxEventCallback(UART_HandleTypeDef *huart, pUART_RxEventCallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_UART_UnRegisterRxEventCallback(UART_HandleTypeDef *huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @addtogroup UART_Exported_Functions_Group2 IO 操作函数
  * @{
  */

/* IO 操作函数 *******************************************************/
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_DMAPause(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_DMAResume(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_DMAStop(UART_HandleTypeDef *huart);

HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint16_t *RxLen,
                                           uint32_t Timeout);
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

HAL_UART_RxEventTypeTypeDef HAL_UARTEx_GetRxEventType(UART_HandleTypeDef *huart);

/* 传输中止函数 */
HAL_StatusTypeDef HAL_UART_Abort(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_AbortTransmit(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_AbortReceive(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_Abort_IT(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_AbortTransmit_IT(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_AbortReceive_IT(UART_HandleTypeDef *huart);

void HAL_UART_IRQHandler(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart);

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

/**
  * @}
  */

/** @addtogroup UART_Exported_Functions_Group3
  * @{
  */
/* 外设控制函数  ************************************************/
HAL_StatusTypeDef HAL_LIN_SendBreak(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_MultiProcessor_EnterMuteMode(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_MultiProcessor_ExitMuteMode(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_HalfDuplex_EnableTransmitter(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_HalfDuplex_EnableReceiver(UART_HandleTypeDef *huart);
/**
  * @}
  */

/** @addtogroup UART_Exported_Functions_Group4
  * @{
  */
/* 外设状态函数  **************************************************/
HAL_UART_StateTypeDef HAL_UART_GetState(const UART_HandleTypeDef *huart);
uint32_t              HAL_UART_GetError(const UART_HandleTypeDef *huart);
/**
  * @}
  */

/**
  * @}
  */
/* 私有类型 -------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/
/* 私有常量 ---------------------------------------------------------*/
/** @defgroup UART_Private_Constants UART 私有常量
  * @{
  */
/** @brief UART 中断标志掩码
  *
  */
#define UART_IT_MASK                     0x0000FFFFU

#define UART_CR1_REG_INDEX               1U
#define UART_CR2_REG_INDEX               2U
#define UART_CR3_REG_INDEX               3U
/**
  * @}
  */

/* 私有宏 ------------------------------------------------------------*/
/** @defgroup UART_Private_Macros UART 私有宏
  * @{
  */
#define IS_UART_WORD_LENGTH(LENGTH) (((LENGTH) == UART_WORDLENGTH_8B) || \
                                     ((LENGTH) == UART_WORDLENGTH_9B))
#define IS_UART_LIN_WORD_LENGTH(LENGTH) (((LENGTH) == UART_WORDLENGTH_8B))
#define IS_UART_STOPBITS(STOPBITS) (((STOPBITS) == UART_STOPBITS_1) || \
                                    ((STOPBITS) == UART_STOPBITS_2))
#define IS_UART_PARITY(PARITY) (((PARITY) == UART_PARITY_NONE) || \
                                ((PARITY) == UART_PARITY_EVEN) || \
                                ((PARITY) == UART_PARITY_ODD))
#define IS_UART_HARDWARE_FLOW_CONTROL(CONTROL)\
                              (((CONTROL) == UART_HWCONTROL_NONE) || \
                               ((CONTROL) == UART_HWCONTROL_RTS) || \
                               ((CONTROL) == UART_HWCONTROL_CTS) || \
                               ((CONTROL) == UART_HWCONTROL_RTS_CTS))
#define IS_UART_MODE(MODE) ((((MODE) & 0x0000FFF3U) == 0x00U) && ((MODE) != 0x00U))
#define IS_UART_STATE(STATE) (((STATE) == UART_STATE_DISABLE) || \
                              ((STATE) == UART_STATE_ENABLE))
#if defined(USART_CR1_OVER8)
#define IS_UART_OVERSAMPLING(SAMPLING) (((SAMPLING) == UART_OVERSAMPLING_16) || \
                                        ((SAMPLING) == UART_OVERSAMPLING_8))
#endif /* USART_CR1_OVER8 */
#define IS_UART_LIN_OVERSAMPLING(SAMPLING) (((SAMPLING) == UART_OVERSAMPLING_16))
#define IS_UART_LIN_BREAK_DETECT_LENGTH(LENGTH) (((LENGTH) == UART_LINBREAKDETECTLENGTH_10B) || \
                                                 ((LENGTH) == UART_LINBREAKDETECTLENGTH_11B))
#define IS_UART_WAKEUPMETHOD(WAKEUP) (((WAKEUP) == UART_WAKEUPMETHOD_IDLELINE) || \
                                      ((WAKEUP) == UART_WAKEUPMETHOD_ADDRESSMARK))
#define IS_UART_BAUDRATE(BAUDRATE) ((BAUDRATE) <= 4500000U)
#define IS_UART_ADDRESS(ADDRESS) ((ADDRESS) <= 0x0FU)

#define UART_DIV_SAMPLING16(_PCLK_, _BAUD_)            (((_PCLK_)*25U)/(4U*(_BAUD_)))
#define UART_DIVMANT_SAMPLING16(_PCLK_, _BAUD_)        (UART_DIV_SAMPLING16((_PCLK_), (_BAUD_))/100U)
#define UART_DIVFRAQ_SAMPLING16(_PCLK_, _BAUD_)        ((((UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) * 100U)) * 16U)\
                                                         + 50U) / 100U)
/* UART BRR = 尾数 + 溢出位 + 小数
            = (UART DIVMANT << 4) + (UART DIVFRAQ & 0xF0) + (UART DIVFRAQ & 0x0FU) */
#define UART_BRR_SAMPLING16(_PCLK_, _BAUD_)            (((UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) << 4U) + \
                                                         (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0xF0U)) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0x0FU))

#define UART_DIV_SAMPLING8(_PCLK_, _BAUD_)             (((_PCLK_)*25U)/(2U*(_BAUD_)))
#define UART_DIVMANT_SAMPLING8(_PCLK_, _BAUD_)         (UART_DIV_SAMPLING8((_PCLK_), (_BAUD_))/100U)
#define UART_DIVFRAQ_SAMPLING8(_PCLK_, _BAUD_)         ((((UART_DIV_SAMPLING8((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING8((_PCLK_), (_BAUD_)) * 100U)) * 8U)\
                                                         + 50U) / 100U)
/* UART BRR = 尾数 + 溢出位 + 小数
            = (UART DIVMANT << 4) + ((UART DIVFRAQ & 0xF8) << 1) + (UART DIVFRAQ & 0x07U) */
#define UART_BRR_SAMPLING8(_PCLK_, _BAUD_)             (((UART_DIVMANT_SAMPLING8((_PCLK_), (_BAUD_)) << 4U) + \
                                                         ((UART_DIVFRAQ_SAMPLING8((_PCLK_), (_BAUD_)) & 0xF8U) << 1U)) + \
                                                        (UART_DIVFRAQ_SAMPLING8((_PCLK_), (_BAUD_)) & 0x07U))

/**
  * @}
  */

/* 私有函数 ---------------------------------------------------------*/
/** @defgroup UART_Private_Functions UART 私有函数
  * @{
  */

HAL_StatusTypeDef UART_Start_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef UART_Start_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

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

#endif /* __STM32F1xx_HAL_UART_H */


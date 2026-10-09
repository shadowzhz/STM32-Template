/**
  ******************************************************************************
  * @file    stm32f1xx_hal_can_legacy.h
  * @author  MCD Application Team
  * @brief   CAN HAL 模块头文件。
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
  */

/* 防止递归包含的宏定义 -----------------------------------------------------*/
#ifndef __STM32F1xx_HAL_CAN_LEGACY_H
#define __STM32F1xx_HAL_CAN_LEGACY_H

#ifdef __cplusplus
 extern "C" {
#endif
     
#if defined(STM32F103x6) || defined(STM32F103xB) || defined(STM32F103xE) || \
    defined(STM32F103xG) || defined(STM32F105xC) || defined(STM32F107xC)

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup CAN
  * @{
  */

/* 导出的类型 ----------------------------------------------------------------*/
/** @defgroup CAN_Exported_Types CAN 导出的类型
  * @{
  */

/**
  * @brief  HAL 状态结构体定义
  */
typedef enum
{
  HAL_CAN_STATE_RESET             = 0x00U,  /*!< CAN 尚未初始化或已禁用 */
  HAL_CAN_STATE_READY             = 0x01U,  /*!< CAN 已初始化且处于就绪状态 */
  HAL_CAN_STATE_BUSY              = 0x02U,  /*!< CAN 进程正在进行中 */
  HAL_CAN_STATE_BUSY_TX           = 0x12U,  /*!< CAN 进程正在进行中 */
  HAL_CAN_STATE_BUSY_RX0          = 0x22U,  /*!< CAN 进程正在进行中 */
  HAL_CAN_STATE_BUSY_RX1          = 0x32U,  /*!< CAN 进程正在进行中 */
  HAL_CAN_STATE_BUSY_TX_RX0       = 0x42U,  /*!< CAN 进程正在进行中 */
  HAL_CAN_STATE_BUSY_TX_RX1       = 0x52U,  /*!< CAN 进程正在进行中                  */
  HAL_CAN_STATE_BUSY_RX0_RX1      = 0x62U,  /*!< CAN 进程正在进行中                  */
  HAL_CAN_STATE_BUSY_TX_RX0_RX1   = 0x72U,  /*!< CAN 进程正在进行中                  */
  HAL_CAN_STATE_TIMEOUT           = 0x03U,  /*!< CAN 处于超时状态                    */
  HAL_CAN_STATE_ERROR             = 0x04U   /*!< CAN 错误状态                        */

}HAL_CAN_StateTypeDef;

/**
  * @brief  CAN 初始化结构体定义
  */
typedef struct
{
  uint32_t Prescaler;  /*!< 指定时间份额（Time Quantum）的长度。
                            该参数必须是介于 Min_Data = 1 到 Max_Data = 1024 之间的数值 */

  uint32_t Mode;       /*!< 指定 CAN 的工作模式。
                            此参数可以是 @ref CAN_operating_mode 中的一个值 */

  uint32_t SJW;        /*!< 指定 CAN 硬件允许延长或缩短位时间以执行再同步的最大时间份额数。
                            此参数可以是 @ref CAN_synchronisation_jump_width 中的一个值 */

  uint32_t BS1;        /*!< 指定时间段 1（Bit Segment 1）中的时间份额数。
                            此参数可以是 @ref CAN_time_quantum_in_bit_segment_1 中的一个值 */

  uint32_t BS2;        /*!< 指定时间段 2（Bit Segment 2）中的时间份额数。
                            此参数可以是 @ref CAN_time_quantum_in_bit_segment_2 中的一个值 */

  uint32_t TTCM;       /*!< 使能或失能时间触发通信模式。
                            此参数可设置为 ENABLE 或 DISABLE。 */

  uint32_t ABOM;       /*!< 使能或失能自动离线管理（Bus-Off Management）。
                            此参数可设置为 ENABLE 或 DISABLE */

  uint32_t AWUM;       /*!< 使能或失能自动唤醒模式。
                            此参数可设置为 ENABLE 或 DISABLE */

  uint32_t NART;       /*!< 使能或失能非自动重传模式。
                            此参数可设置为 ENABLE 或 DISABLE */

  uint32_t RFLM;       /*!< 使能或禁止接收 FIFO 锁定模式。
                            此参数可设置为 ENABLE 或 DISABLE */

  uint32_t TXFP;       /*!< 使能或禁止发送 FIFO 优先级。
                            此参数可设置为 ENABLE 或 DISABLE */
}CAN_InitTypeDef;

/** 
  * @brief  CAN 发送消息结构体定义  
  */
typedef struct
{
  uint32_t StdId;    /*!< 指定标准标识符。
                          此参数必须是介于 Min_Data = 0 与 Max_Data = 0x7FF 之间的数值 */

  uint32_t ExtId;    /*!< 指定扩展标识符。
                          此参数必须是介于 Min_Data = 0 与 Max_Data = 0x1FFFFFFF 之间的数值 */

  uint32_t IDE;      /*!< 指定待发送消息的标识符类型。
                          此参数可以是 @ref CAN_Identifier_Type 的取值之一 */

  uint32_t RTR;      /*!< 指定待发送消息的帧类型。
                          此参数可以是 @ref CAN_remote_transmission_request 的取值之一 */

  uint32_t DLC;      /*!< 指定待发送的数据帧长度。
                          此参数必须是介于 Min_Data = 0 与 Max_Data = 8 之间的数值 */

  uint8_t Data[8];   /*!< 包含待发送的数据。
                          此参数必须是介于 Min_Data = 0 与 Max_Data = 0xFF 之间的数值 */

}CanTxMsgTypeDef;

/**
  * @brief  CAN 接收消息结构体定义
  */
typedef struct
{
  uint32_t StdId;       /*!< 指定标准标识符。
                             此参数必须是介于 Min_Data = 0 与 Max_Data = 0x7FF 之间的数值 */

  uint32_t ExtId;       /*!< 指定扩展标识符。
                             此参数必须是介于 Min_Data = 0 与 Max_Data = 0x1FFFFFFF 之间的数值 */

  uint32_t IDE;         /*!< 指定待接收消息的标识符类型。
                             此参数可以是 @ref CAN_Identifier_Type 的取值之一 */

  uint32_t RTR;         /*!< 指定接收消息的帧类型。
                             此参数可以是 @ref CAN_remote_transmission_request 的取值之一 */

  uint32_t DLC;         /*!< 指定待接收的数据帧长度。
                             此参数必须是介于 Min_Data = 0 与 Max_Data = 8 之间的数值 */

  uint8_t Data[8];      /*!< 包含待接收的数据。
                             该参数必须是介于 Min_Data = 0 到 Max_Data = 0xFF 之间的数值 */

  uint32_t FMI;         /*!< 指定存储在邮箱中的报文所通过的过滤器索引。
                             该参数必须是介于 Min_Data = 0 到 Max_Data = 0xFF 之间的数值 */

  uint32_t FIFONumber;  /*!< 指定接收 FIFO 编号。
                             该参数可以是 CAN_FIFO0 或 CAN_FIFO1 */

}CanRxMsgTypeDef;

/**
  * @brief  CAN 句柄结构体定义
  */
typedef struct
{
  CAN_TypeDef                 *Instance;  /*!< 寄存器基地址                   */

  CAN_InitTypeDef             Init;       /*!< CAN 必需参数                   */

  CanTxMsgTypeDef*            pTxMsg;     /*!< 发送结构体指针                 */

  CanRxMsgTypeDef*            pRxMsg;     /*!< RX FIFO0 报文接收结构体指针    */

  CanRxMsgTypeDef*            pRx1Msg;    /*!< RX FIFO1 报文接收结构体指针    */

  __IO HAL_CAN_StateTypeDef   State;      /*!< CAN 通信状态                   */

  HAL_LockTypeDef             Lock;       /*!< CAN 锁对象                     */

  __IO uint32_t               ErrorCode;  /*!< CAN 错误码                     */

}CAN_HandleTypeDef;

/**
  * @}
  */

/* 导出常量 ------------------------------------------------------------------*/
/** @defgroup CAN_Exported_Constants CAN 导出常量
  * @{
  */

/** @defgroup CAN_Error_Code CAN 错误代码
  * @{
  */
#define   HAL_CAN_ERROR_NONE      0x00000000U    /*!< 无错误               */
#define   HAL_CAN_ERROR_EWG       0x00000001U    /*!< 错误警告（EWG）错误  */
#define   HAL_CAN_ERROR_EPV       0x00000002U    /*!< 被动错误（EPV）错误  */
#define   HAL_CAN_ERROR_BOF       0x00000004U    /*!< 总线关闭（BOF）错误  */
#define   HAL_CAN_ERROR_STF       0x00000008U    /*!< 位填充错误           */
#define   HAL_CAN_ERROR_FOR       0x00000010U    /*!< 格式错误             */
#define   HAL_CAN_ERROR_ACK       0x00000020U    /*!< 应答错误             */
#define   HAL_CAN_ERROR_BR        0x00000040U    /*!< 隐性位错误           */
#define   HAL_CAN_ERROR_BD        0x00000080U    /*!< LEC 显性位错误       */
#define   HAL_CAN_ERROR_CRC       0x00000100U    /*!< LEC 发送错误         */
#define   HAL_CAN_ERROR_FOV0      0x00000200U    /*!< FIFO0 溢出错误       */
#define   HAL_CAN_ERROR_FOV1      0x00000400U    /*!< FIFO1 溢出错误       */
#define   HAL_CAN_ERROR_TXFAIL    0x00000800U    /*!< 发送失败             */
/**
  * @}
  */

/** @defgroup CAN_InitStatus CAN 初始化状态
  * @{
  */
#define CAN_INITSTATUS_FAILED       0x00000000U  /*!< CAN 初始化失败 */
#define CAN_INITSTATUS_SUCCESS      0x00000001U  /*!< CAN 初始化成功 */
/**
  * @}
  */

/** @defgroup CAN_operating_mode CAN 工作模式
  * @{
  */
#define CAN_MODE_NORMAL             0x00000000U                                /*!< 正常模式   */
#define CAN_MODE_LOOPBACK           ((uint32_t)CAN_BTR_LBKM)                   /*!< 回环模式 */
#define CAN_MODE_SILENT             ((uint32_t)CAN_BTR_SILM)                   /*!< 静默模式   */
#define CAN_MODE_SILENT_LOOPBACK    ((uint32_t)(CAN_BTR_LBKM | CAN_BTR_SILM))  /*!< 回环并联静默模式 */
/**
  * @}
  */

/** @defgroup CAN_synchronisation_jump_width CAN 再同步跳转宽度
  * @{
  */
#define CAN_SJW_1TQ                 0x00000000U                /*!< 1 个时间量子 (time quantum) */
#define CAN_SJW_2TQ                 ((uint32_t)CAN_BTR_SJW_0)  /*!< 2 个时间量子 (time quantum) */
#define CAN_SJW_3TQ                 ((uint32_t)CAN_BTR_SJW_1)  /*!< 3 个时间量子 (time quantum) */
#define CAN_SJW_4TQ                 ((uint32_t)CAN_BTR_SJW)    /*!< 4 个时间量子 (time quantum) */
/**
  * @}
  */

/** @defgroup CAN_time_quantum_in_bit_segment_1 位段 1 中的 CAN 时间份额 (Time Quantum)
  * @{
  */
#define CAN_BS1_1TQ                 0x00000000U                                                  /*!< 1 个时间份额 (time quantum)  */
#define CAN_BS1_2TQ                 ((uint32_t)CAN_BTR_TS1_0)                                    /*!< 2 个时间份额 (time quantum)  */
#define CAN_BS1_3TQ                 ((uint32_t)CAN_BTR_TS1_1)                                    /*!< 3 个时间份额 (time quantum)  */
#define CAN_BS1_4TQ                 ((uint32_t)(CAN_BTR_TS1_1 | CAN_BTR_TS1_0))                  /*!< 4 个时间份额 (time quantum)  */
#define CAN_BS1_5TQ                 ((uint32_t)CAN_BTR_TS1_2)                                    /*!< 5 个时间份额 (time quantum)  */
#define CAN_BS1_6TQ                 ((uint32_t)(CAN_BTR_TS1_2 | CAN_BTR_TS1_0))                  /*!< 6 个时间份额 (time quantum)  */
#define CAN_BS1_7TQ                 ((uint32_t)(CAN_BTR_TS1_2 | CAN_BTR_TS1_1))                  /*!< 7 个时间份额 (time quantum)  */
#define CAN_BS1_8TQ                 ((uint32_t)(CAN_BTR_TS1_2 | CAN_BTR_TS1_1 | CAN_BTR_TS1_0))  /*!< 8 个时间份额 (time quantum)  */
#define CAN_BS1_9TQ                 ((uint32_t)CAN_BTR_TS1_3)                                    /*!< 9 个时间份额 (time quantum)  */
#define CAN_BS1_10TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_0))                  /*!< 10 个时间份额 (time quantum) */
#define CAN_BS1_11TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_1))                  /*!< 11 个时间份额 (time quantum) */
#define CAN_BS1_12TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_1 | CAN_BTR_TS1_0))  /*!< 12 个时间份额 (time quantum) */
#define CAN_BS1_13TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_2))                  /*!< 13 个时间份额 (time quantum) */
#define CAN_BS1_14TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_2 | CAN_BTR_TS1_0))  /*!< 14 个时间份额 (time quantum) */
#define CAN_BS1_15TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_2 | CAN_BTR_TS1_1))  /*!< 15 个时间份额 (time quantum) */
#define CAN_BS1_16TQ                ((uint32_t)CAN_BTR_TS1) /*!< 16 个时间份额 (time quantum) */
/**
  * @}
  */

/** @defgroup CAN_time_quantum_in_bit_segment_2 CAN 位段 2 中的时间份额 (Time Quantum)
  * @{
  */
#define CAN_BS2_1TQ                 0x00000000U                                  /*!< 1 个时间份额 (time quantum) */
#define CAN_BS2_2TQ                 ((uint32_t)CAN_BTR_TS2_0)                    /*!< 2 个时间份额 (time quantum) */
#define CAN_BS2_3TQ                 ((uint32_t)CAN_BTR_TS2_1)                    /*!< 3 个时间份额 (time quantum) */
#define CAN_BS2_4TQ                 ((uint32_t)(CAN_BTR_TS2_1 | CAN_BTR_TS2_0))  /*!< 4 个时间份额 (time quantum) */
#define CAN_BS2_5TQ                 ((uint32_t)CAN_BTR_TS2_2)                    /*!< 5 个时间份额 (time quantum) */
#define CAN_BS2_6TQ                 ((uint32_t)(CAN_BTR_TS2_2 | CAN_BTR_TS2_0))  /*!< 6 个时间份额 (time quantum) */
#define CAN_BS2_7TQ                 ((uint32_t)(CAN_BTR_TS2_2 | CAN_BTR_TS2_1))  /*!< 7 个时间份额 (time quantum) */
#define CAN_BS2_8TQ                 ((uint32_t)CAN_BTR_TS2)                      /*!< 8 个时间份额 (time quantum) */
/**
  * @}
  */

/** @defgroup CAN_filter_mode  CAN 筛选器模式
  * @{
  */
#define CAN_FILTERMODE_IDMASK       ((uint8_t)0x00)  /*!< 标识符掩码模式 */
#define CAN_FILTERMODE_IDLIST       ((uint8_t)0x01)  /*!< 标识符列表模式 */
/**
  * @}
  */

/** @defgroup CAN_filter_scale CAN 过滤器位宽
  * @{
  */
#define CAN_FILTERSCALE_16BIT       ((uint8_t)0x00)  /*!< 两个 16 位过滤器 */
#define CAN_FILTERSCALE_32BIT       ((uint8_t)0x01)  /*!< 一个 32 位过滤器 */
/**
  * @}
  */

/** @defgroup CAN_filter_FIFO CAN 过滤器关联 FIFO
  * @{
  */
#define CAN_FILTER_FIFO0             ((uint8_t)0x00)  /*!< 过滤器 x 分配给 FIFO 0 */
#define CAN_FILTER_FIFO1             ((uint8_t)0x01)  /*!< 过滤器 x 分配给 FIFO 1 */
/**
  * @}
  */

/** @defgroup CAN_Identifier_Type CAN 标识符类型
  * @{
  */
#define CAN_ID_STD                  0x00000000U  /*!< 标准帧 ID */
#define CAN_ID_EXT                  0x00000004U  /*!< 扩展帧 ID */
/**
  * @}
  */

/** @defgroup CAN_remote_transmission_request CAN 远程传输请求
  * @{
  */
#define CAN_RTR_DATA                0x00000000U  /*!< 数据帧 */
#define CAN_RTR_REMOTE              0x00000002U  /*!< 远程帧 */
/**
  * @}
  */

/** @defgroup CAN_transmit_constants CAN 发送常量
  * @{
  */
#define CAN_TXSTATUS_NOMAILBOX      ((uint8_t)0x04)  /*!< CAN 控制器未返回 CAN_TxStatus_NoMailBox */
/**
  * @}
  */

/** @defgroup CAN_receive_FIFO_number_constants CAN 接收 FIFO 编号
  * @{
  */
#define CAN_FIFO0                   ((uint8_t)0x00)  /*!< 用于接收的 CAN FIFO 0 */
#define CAN_FIFO1                   ((uint8_t)0x01)  /*!< 用于接收的 CAN FIFO 1 */
/**
  * @}
  */

/** @defgroup CAN_flags CAN 标志位
  * @{
  */
/* 若标志位格式为 0x3XXXXXXX，表示该标志位可配合 CAN_GetFlagStatus()
   和 CAN_ClearFlag() 函数使用。 */
/* 若标志位格式为 0x1XXXXXXX，表示该标志位仅可配合
   CAN_GetFlagStatus() 函数使用。  */

/* 发送标志位 */
#define CAN_FLAG_RQCP0             ((uint32_t)((TSR_REGISTER_INDEX << 8U) | CAN_TSR_RQCP0_BIT_POSITION))  /*!< 发送邮箱0请求标志位           */
#define CAN_FLAG_RQCP1             ((uint32_t)((TSR_REGISTER_INDEX << 8U) | CAN_TSR_RQCP1_BIT_POSITION))  /*!< 发送邮箱1请求标志位           */
#define CAN_FLAG_RQCP2             ((uint32_t)((TSR_REGISTER_INDEX << 8U) | CAN_TSR_RQCP2_BIT_POSITION))  /*!< 发送邮箱2请求标志位           */
#define CAN_FLAG_TXOK0             ((uint32_t)((TSR_REGISTER_INDEX << 8U) | CAN_TSR_TXOK0_BIT_POSITION))  /*!< 发送邮箱0发送成功标志位       */
#define CAN_FLAG_TXOK1             ((uint32_t)((TSR_REGISTER_INDEX << 8U) | CAN_TSR_TXOK1_BIT_POSITION))  /*!< 发送邮箱1发送成功标志位       */
#define CAN_FLAG_TXOK2             ((uint32_t)((TSR_REGISTER_INDEX << 8U) | CAN_TSR_TXOK2_BIT_POSITION))  /*!< 发送邮箱2发送成功标志位       */
#define CAN_FLAG_TME0              ((uint32_t)((TSR_REGISTER_INDEX << 8U) | CAN_TSR_TME0_BIT_POSITION))   /*!< 发送邮箱0空中断标志位         */
#define CAN_FLAG_TME1              ((uint32_t)((TSR_REGISTER_INDEX << 8U) | CAN_TSR_TME1_BIT_POSITION))   /*!< 发送邮箱0空中断标志位         */
#define CAN_FLAG_TME2              ((uint32_t)((TSR_REGISTER_INDEX << 8U) | CAN_TSR_TME2_BIT_POSITION))   /*!< 发送邮箱0空中断标志位         */

/* 接收标志位 */
#define CAN_FLAG_FF0               ((uint32_t)((RF0R_REGISTER_INDEX << 8U) | CAN_RF0R_FF0_BIT_POSITION))  /*!< FIFO 0 满标志位     */
#define CAN_FLAG_FOV0              ((uint32_t)((RF0R_REGISTER_INDEX << 8U) | CAN_RF0R_FOV0_BIT_POSITION)) /*!< FIFO 0 溢出标志位   */

#define CAN_FLAG_FF1               ((uint32_t)((RF1R_REGISTER_INDEX << 8U) | CAN_RF1R_FF1_BIT_POSITION))  /*!< FIFO 1 满标志位     */
#define CAN_FLAG_FOV1              ((uint32_t)((RF1R_REGISTER_INDEX << 8U) | CAN_RF1R_FOV1_BIT_POSITION)) /*!< FIFO 1 溢出标志位   */

/* 工作模式标志位 */
#define CAN_FLAG_WKU               ((uint32_t)((MSR_REGISTER_INDEX << 8U) | CAN_MSR_WKU_BIT_POSITION))    /*!< 唤醒标志               */
#define CAN_FLAG_SLAK              ((uint32_t)((MSR_REGISTER_INDEX << 8U) | CAN_MSR_SLAK_BIT_POSITION))   /*!< 睡眠应答标志           */
#define CAN_FLAG_SLAKI             ((uint32_t)((MSR_REGISTER_INDEX << 8U) | CAN_MSR_SLAKI_BIT_POSITION))  /*!< 睡眠应答标志           */

/* @note 当禁用 SLAK 中断时 (SLKIE=0)，无法轮询 SLAKI。
         在这种情况下，可以轮询 SLAK 位。*/

/* 错误标志 */
#define CAN_FLAG_EWG               ((uint32_t)((ESR_REGISTER_INDEX << 8U) | CAN_ESR_EWG_BIT_POSITION))    /*!< 错误警告标志         */
#define CAN_FLAG_EPV               ((uint32_t)((ESR_REGISTER_INDEX << 8U) | CAN_ESR_EPV_BIT_POSITION))    /*!< 被动错误标志         */
#define CAN_FLAG_BOF               ((uint32_t)((ESR_REGISTER_INDEX << 8U) | CAN_ESR_BOF_BIT_POSITION))    /*!< 总线关闭 (Bus-Off) 标志 */

/**
  * @}
  */

/** @defgroup CAN_Interrupts CAN 中断
  * @{
  */
#define CAN_IT_TME                  ((uint32_t)CAN_IER_TMEIE)   /*!< 发送邮箱空中断                   */

/* 接收中断 */
#define CAN_IT_FMP0                 ((uint32_t)CAN_IER_FMPIE0)  /*!< FIFO 0 消息挂起中断              */
#define CAN_IT_FF0                  ((uint32_t)CAN_IER_FFIE0)   /*!< FIFO 0 满中断                    */
#define CAN_IT_FOV0                 ((uint32_t)CAN_IER_FOVIE0)  /*!< FIFO 0 溢出中断                  */
#define CAN_IT_FMP1                 ((uint32_t)CAN_IER_FMPIE1)  /*!< FIFO 1 消息挂起中断 */
#define CAN_IT_FF1                  ((uint32_t)CAN_IER_FFIE1)   /*!< FIFO 1 满中断            */
#define CAN_IT_FOV1                 ((uint32_t)CAN_IER_FOVIE1)  /*!< FIFO 1 溢出中断         */

/* 工作模式中断 */
#define CAN_IT_WKU                  ((uint32_t)CAN_IER_WKUIE)  /*!< 唤醒中断           */
#define CAN_IT_SLK                  ((uint32_t)CAN_IER_SLKIE)  /*!< 睡眠应答中断 */

/* 错误中断 */
#define CAN_IT_EWG                  ((uint32_t)CAN_IER_EWGIE) /*!< 错误警告中断   */
#define CAN_IT_EPV                  ((uint32_t)CAN_IER_EPVIE) /*!< 被动错误中断   */
#define CAN_IT_BOF                  ((uint32_t)CAN_IER_BOFIE) /*!< 离线中断         */
#define CAN_IT_LEC                  ((uint32_t)CAN_IER_LECIE) /*!< 上次错误代码中断 */
#define CAN_IT_ERR                  ((uint32_t)CAN_IER_ERRIE) /*!< 错误中断           */
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup CAN_Private_Constants CAN 私有常量
  * @{
  */

/* 用于 CAN 标志的 CAN 中间移位值 */
#define TSR_REGISTER_INDEX      0x5U
#define RF0R_REGISTER_INDEX     0x2U
#define RF1R_REGISTER_INDEX     0x4U
#define MSR_REGISTER_INDEX      0x1U
#define ESR_REGISTER_INDEX      0x3U

/* CAN 标志位在其各自寄存器（TSR、RF0R、RF1R 或 MSR 寄存器）中的位置 */
/* 发送标志 */
#define CAN_TSR_RQCP0_BIT_POSITION     0x00000000U
#define CAN_TSR_RQCP1_BIT_POSITION     0x00000008U
#define CAN_TSR_RQCP2_BIT_POSITION     0x00000010U
#define CAN_TSR_TXOK0_BIT_POSITION     0x00000001U
#define CAN_TSR_TXOK1_BIT_POSITION     0x00000009U
#define CAN_TSR_TXOK2_BIT_POSITION     0x00000011U
#define CAN_TSR_TME0_BIT_POSITION      0x0000001AU
#define CAN_TSR_TME1_BIT_POSITION      0x0000001BU
#define CAN_TSR_TME2_BIT_POSITION      0x0000001CU

/* 接收标志 */
#define CAN_RF0R_FF0_BIT_POSITION      0x00000003U
#define CAN_RF0R_FOV0_BIT_POSITION     0x00000004U

#define CAN_RF1R_FF1_BIT_POSITION      0x00000003U
#define CAN_RF1R_FOV1_BIT_POSITION     0x00000004U

/* 工作模式标志 */
#define CAN_MSR_WKU_BIT_POSITION       0x00000003U
#define CAN_MSR_SLAK_BIT_POSITION      0x00000001U
#define CAN_MSR_SLAKI_BIT_POSITION     0x00000004U

/* 错误标志 */
#define CAN_ESR_EWG_BIT_POSITION       0x00000000U
#define CAN_ESR_EPV_BIT_POSITION       0x00000001U
#define CAN_ESR_BOF_BIT_POSITION       0x00000002U

/* 宏用于获取/清除 CAN 标志的掩码 */
#define CAN_FLAG_MASK                  0x000000FFU

/* 邮箱定义 */
#define CAN_TXMAILBOX_0   ((uint8_t)0x00)
#define CAN_TXMAILBOX_1   ((uint8_t)0x01)
#define CAN_TXMAILBOX_2   ((uint8_t)0x02)
/**
  * @}
  */

/* 导出的宏 -----------------------------------------------------------*/
/** @defgroup CAN_Exported_Macros CAN 导出的宏
  * @{
  */

/** @brief  复位 CAN 句柄状态
  * @param  __HANDLE__: CAN 句柄。
  * @retval 无
  */
#define __HAL_CAN_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_CAN_STATE_RESET)

/**
  * @brief  使能指定的 CAN 中断
  * @param  __HANDLE__: CAN 句柄。
  * @param  __INTERRUPT__: CAN 中断。
  *         该参数可以是以下值之一：
  *            @arg CAN_IT_TME: 发送邮箱空中断使能
  *            @arg CAN_IT_FMP0: FIFO 0 消息挂起中断
  *            @arg CAN_IT_FF0 : FIFO 0 满中断
  *            @arg CAN_IT_FOV0: FIFO 0 溢出中断
  *            @arg CAN_IT_FMP1: FIFO 1 消息挂起中断
  *            @arg CAN_IT_FF1 : FIFO 1 满中断
  *            @arg CAN_IT_FOV1: FIFO 1 溢出中断
  *            @arg CAN_IT_WKU : 唤醒中断
  *            @arg CAN_IT_SLK : 睡眠应答中断
  *            @arg CAN_IT_EWG : 错误警告中断
  *            @arg CAN_IT_EPV : 被动错误中断
  *            @arg CAN_IT_BOF : 离线中断
  *            @arg CAN_IT_LEC : 上次错误代码中断
  *            @arg CAN_IT_ERR : 错误中断
  * @retval 无。
  */
#define __HAL_CAN_ENABLE_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->Instance->IER) |= (__INTERRUPT__))

/**
  * @brief  禁用指定的 CAN 中断
  * @param  __HANDLE__: CAN 句柄。
  * @param  __INTERRUPT__: CAN 中断。
  *         该参数可以是以下值之一：
  *            @arg CAN_IT_TME: 发送邮箱空中断使能
  *            @arg CAN_IT_FMP0: FIFO 0 消息挂起中断
  *            @arg CAN_IT_FF0 : FIFO 0 满中断
  *            @arg CAN_IT_FOV0: FIFO 0 溢出中断
  *            @arg CAN_IT_FMP1: FIFO 1 消息挂起中断
  *            @arg CAN_IT_FF1 : FIFO 1 满中断
  *            @arg CAN_IT_FOV1: FIFO 1 溢出中断
  *            @arg CAN_IT_WKU : 唤醒中断
  *            @arg CAN_IT_SLK : 睡眠应答中断
  *            @arg CAN_IT_EWG : 错误警告中断
  *            @arg CAN_IT_EPV : 被动错误中断
  *            @arg CAN_IT_BOF : 离线中断
  *            @arg CAN_IT_LEC : 上次错误代码中断
  *            @arg CAN_IT_ERR : 错误中断
  * @retval 无。
  */
#define __HAL_CAN_DISABLE_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->Instance->IER) &= ~(__INTERRUPT__))

/**
  * @brief  返回挂起的接收消息数量。
  * @param  __HANDLE__: CAN 句柄。
  * @param  __FIFONUMBER__: 接收 FIFO 编号，CAN_FIFO0 或 CAN_FIFO1。
  * @retval 挂起的消息数量。
  */
#define __HAL_CAN_MSG_PENDING(__HANDLE__, __FIFONUMBER__) (((__FIFONUMBER__) == CAN_FIFO0)? \
((uint8_t)((__HANDLE__)->Instance->RF0R & 0x03U)) : ((uint8_t)((__HANDLE__)->Instance->RF1R & 0x03U)))

/** @brief  检查指定的 CAN 标志位是否置位。
  * @param  __HANDLE__: 指定 CAN 句柄。
  * @param  __FLAG__: 指定要检查的标志位。
  *         该参数可以是以下值之一：
  *            @arg CAN_TSR_RQCP0: 发送邮箱 0 请求完成标志
  *            @arg CAN_TSR_RQCP1: 发送邮箱 1 请求完成标志
  *            @arg CAN_TSR_RQCP2: 发送邮箱 2 请求完成标志
  *            @arg CAN_FLAG_TXOK0: 邮箱 0 发送成功标志
  *            @arg CAN_FLAG_TXOK1: 邮箱 1 发送成功标志
  *            @arg CAN_FLAG_TXOK2: 邮箱 2 发送成功标志
  *            @arg CAN_FLAG_TME0: 发送邮箱 0 空标志
  *            @arg CAN_FLAG_TME1: 发送邮箱 1 空标志
  *            @arg CAN_FLAG_TME2: 发送邮箱 2 空标志
  *            @arg CAN_FLAG_FMP0: FIFO 0 消息挂起标志
  *            @arg CAN_FLAG_FF0: FIFO 0 满标志
  *            @arg CAN_FLAG_FOV0: FIFO 0 溢出标志
  *            @arg CAN_FLAG_FMP1: FIFO 1 消息挂起标志
  *            @arg CAN_FLAG_FF1: FIFO 1 满标志
  *            @arg CAN_FLAG_FOV1: FIFO 1 溢出标志
  *            @arg CAN_FLAG_WKU: 唤醒标志
  *            @arg CAN_FLAG_SLAK: 睡眠应答标志
  *            @arg CAN_FLAG_SLAKI: 睡眠应答中断标志
  *            @arg CAN_FLAG_EWG: 错误警告标志
  *            @arg CAN_FLAG_EPV: 被动错误标志
  *            @arg CAN_FLAG_BOF: 离线（Bus-Off）标志
  * @retval __FLAG__ 的新状态（TRUE 或 FALSE）。
  */
#define __HAL_CAN_GET_FLAG(__HANDLE__, __FLAG__) \
((((__FLAG__) >> 8U) == 5U)? ((((__HANDLE__)->Instance->TSR) & (1U << ((__FLAG__) & CAN_FLAG_MASK))) == (1U << ((__FLAG__) & CAN_FLAG_MASK))): \
 (((__FLAG__) >> 8U) == 2U)? ((((__HANDLE__)->Instance->RF0R) & (1U << ((__FLAG__) & CAN_FLAG_MASK))) == (1U << ((__FLAG__) & CAN_FLAG_MASK))): \
 (((__FLAG__) >> 8U) == 4U)? ((((__HANDLE__)->Instance->RF1R) & (1U << ((__FLAG__) & CAN_FLAG_MASK))) == (1U << ((__FLAG__) & CAN_FLAG_MASK))): \
 (((__FLAG__) >> 8U) == 1U)? ((((__HANDLE__)->Instance->MSR) & (1U << ((__FLAG__) & CAN_FLAG_MASK))) == (1U << ((__FLAG__) & CAN_FLAG_MASK))): \
 ((((__HANDLE__)->Instance->ESR) & (1U << ((__FLAG__) & CAN_FLAG_MASK))) == (1U << ((__FLAG__) & CAN_FLAG_MASK))))

/** @brief  清除指定的 CAN 挂起标志位。
  * @param  __HANDLE__: 指定 CAN 句柄。
  * @param  __FLAG__: 指定要检查的标志位。
  *         该参数可以是以下值之一：
  *            @arg CAN_TSR_RQCP0: 发送邮箱 0 请求完成标志
  *            @arg CAN_TSR_RQCP1: 发送邮箱 1 请求完成标志
  *            @arg CAN_TSR_RQCP2: 发送邮箱 2 请求完成标志
  *            @arg CAN_FLAG_TXOK0: 邮箱 0 发送成功标志
  *            @arg CAN_FLAG_TXOK1: 邮箱 1 发送成功标志
  *            @arg CAN_FLAG_TXOK2: 邮箱 2 发送成功标志
  *            @arg CAN_FLAG_TME0: 发送邮箱 0 空标志
  *            @arg CAN_FLAG_TME1: 发送邮箱 1 空标志
  *            @arg CAN_FLAG_TME2: 发送邮箱 2 空标志
  *            @arg CAN_FLAG_FMP0: FIFO 0 消息挂起标志
  *            @arg CAN_FLAG_FF0: FIFO 0 满标志
  *            @arg CAN_FLAG_FOV0: FIFO 0 溢出标志
  *            @arg CAN_FLAG_FMP1: FIFO 1 消息挂起标志
  *            @arg CAN_FLAG_FF1: FIFO 1 满标志
  *            @arg CAN_FLAG_FOV1: FIFO 1 溢出标志
  *            @arg CAN_FLAG_WKU: 唤醒标志
  *            @arg CAN_FLAG_SLAKI: 睡眠应答中断标志
  * @retval __FLAG__ 的新状态（TRUE 或 FALSE）。
  */
#define __HAL_CAN_CLEAR_FLAG(__HANDLE__, __FLAG__) \
((((__FLAG__) >> 8U) == TSR_REGISTER_INDEX) ? (((__HANDLE__)->Instance->TSR)  = (1U << ((__FLAG__) & CAN_FLAG_MASK))): \
 (((__FLAG__) >> 8U) == RF0R_REGISTER_INDEX)? (((__HANDLE__)->Instance->RF0R) = (1U << ((__FLAG__) & CAN_FLAG_MASK))): \
 (((__FLAG__) >> 8U) == RF1R_REGISTER_INDEX)? (((__HANDLE__)->Instance->RF1R) = (1U << ((__FLAG__) & CAN_FLAG_MASK))): \
 (((__FLAG__) >> 8U) == MSR_REGISTER_INDEX) ? (((__HANDLE__)->Instance->MSR)  = (1U << ((__FLAG__) & CAN_FLAG_MASK))): 0U)

/** @brief  检查指定的 CAN 中断源是否使能。
  * @param  __HANDLE__: 指定 CAN 句柄。
  * @param  __INTERRUPT__: 指定要检查的 CAN 中断源。
  *         该参数可以是以下值之一：
  *            @arg CAN_IT_TME: 发送邮箱空中断使能
  *            @arg CAN_IT_FMP0: FIFO 0 消息挂起中断
  *            @arg CAN_IT_FF0 : FIFO 0 满中断
  *            @arg CAN_IT_FOV0: FIFO 0 溢出中断
  *            @arg CAN_IT_FMP1: FIFO 1 消息挂起中断
  *            @arg CAN_IT_FF1 : FIFO 1 满中断
  *            @arg CAN_IT_FOV1: FIFO 1 溢出中断
  *            @arg CAN_IT_WKU : 唤醒中断
  *            @arg CAN_IT_SLK : 睡眠应答中断
  *            @arg CAN_IT_EWG : 错误警告中断
  *            @arg CAN_IT_EPV : 被动错误中断
  *            @arg CAN_IT_BOF : 离线中断
  *            @arg CAN_IT_LEC : 上次错误代码中断
  *            @arg CAN_IT_ERR : 错误中断
  * @retval __IT__ 的新状态（TRUE 或 FALSE）。
  */
#define __HAL_CAN_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) ((((__HANDLE__)->Instance->IER & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)

/**
  * @brief  检查 CAN 报文的发送状态。
  * @param  __HANDLE__: 指定 CAN 句柄。
  * @param  __TRANSMITMAILBOX__: 用于发送的邮箱编号。
  * @retval 新的发送状态（TRUE 或 FALSE）。
  */
#define __HAL_CAN_TRANSMIT_STATUS(__HANDLE__, __TRANSMITMAILBOX__)\
(((__TRANSMITMAILBOX__) == CAN_TXMAILBOX_0)? ((((__HANDLE__)->Instance->TSR) & (CAN_TSR_RQCP0 | CAN_TSR_TME0)) == (CAN_TSR_RQCP0 | CAN_TSR_TME0)) :\
 ((__TRANSMITMAILBOX__) == CAN_TXMAILBOX_1)? ((((__HANDLE__)->Instance->TSR) & (CAN_TSR_RQCP1 | CAN_TSR_TME1)) == (CAN_TSR_RQCP1 | CAN_TSR_TME1)) :\
 ((((__HANDLE__)->Instance->TSR) & (CAN_TSR_RQCP2 | CAN_TSR_TME2)) == (CAN_TSR_RQCP2 | CAN_TSR_TME2)))

/**
  * @brief  释放指定的接收 FIFO。
  * @param  __HANDLE__: CAN 句柄。
  * @param  __FIFONUMBER__: 接收 FIFO 编号，CAN_FIFO0 或 CAN_FIFO1。
  * @retval 无。
  */
#define __HAL_CAN_FIFO_RELEASE(__HANDLE__, __FIFONUMBER__) (((__FIFONUMBER__) == CAN_FIFO0)? \
((__HANDLE__)->Instance->RF0R = CAN_RF0R_RFOM0) : ((__HANDLE__)->Instance->RF1R = CAN_RF1R_RFOM1)) 

/**
  * @brief  取消发送请求。
  * @param  __HANDLE__: 指定 CAN 句柄。
  * @param  __TRANSMITMAILBOX__: 用于发送的邮箱编号。
  * @retval 无。
  */
#define __HAL_CAN_CANCEL_TRANSMIT(__HANDLE__, __TRANSMITMAILBOX__)\
(((__TRANSMITMAILBOX__) == CAN_TXMAILBOX_0)? ((__HANDLE__)->Instance->TSR = CAN_TSR_ABRQ0) :\
 ((__TRANSMITMAILBOX__) == CAN_TXMAILBOX_1)? ((__HANDLE__)->Instance->TSR = CAN_TSR_ABRQ1) :\
 ((__HANDLE__)->Instance->TSR = CAN_TSR_ABRQ2))

/**
  * @brief  使能或禁用 CAN 的调试冻结（DBG Freeze）功能。
  * @param  __HANDLE__: 指定 CAN 句柄。
  * @param  __NEWSTATE__: CAN 外设的新状态。
  *         该参数可以是：ENABLE（调试期间 CAN 接收/发送冻结。
  *         接收 FIFO 仍可正常访问/控制）
  *         或 DISABLE（调试期间 CAN 正常工作）。
  * @retval 无
  */
#define __HAL_CAN_DBG_FREEZE(__HANDLE__, __NEWSTATE__) (((__NEWSTATE__) == ENABLE)? \
((__HANDLE__)->Instance->MCR |= CAN_MCR_DBF) : ((__HANDLE__)->Instance->MCR &= ~CAN_MCR_DBF))

/**
  * @}
  */

/* 包含 CAN HAL 扩展模块 */
#include "stm32f1xx_hal_can_ex_legacy.h"

/* 导出函数 ------------------------------------------------------------------*/
/** @addtogroup CAN_Exported_Functions
  * @{
  */

/** @addtogroup CAN_Exported_Functions_Group1
  * @brief    初始化与配置函数 
  * @{
  */
/* 初始化与去初始化函数 ********************************************************/
HAL_StatusTypeDef HAL_CAN_Init(CAN_HandleTypeDef* hcan);
HAL_StatusTypeDef HAL_CAN_ConfigFilter(CAN_HandleTypeDef* hcan, CAN_FilterConfTypeDef* sFilterConfig);
HAL_StatusTypeDef HAL_CAN_DeInit(CAN_HandleTypeDef* hcan);
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan);
void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan);
/**
  * @}
  */

/** @addtogroup CAN_Exported_Functions_Group2
  * @brief    I/O 操作函数 
  * @{
  */
/* I/O 操作函数 *****************************************************/
HAL_StatusTypeDef HAL_CAN_Transmit(CAN_HandleTypeDef *hcan, uint32_t Timeout);
HAL_StatusTypeDef HAL_CAN_Transmit_IT(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef HAL_CAN_Receive(CAN_HandleTypeDef *hcan, uint8_t FIFONumber, uint32_t Timeout);
HAL_StatusTypeDef HAL_CAN_Receive_IT(CAN_HandleTypeDef *hcan, uint8_t FIFONumber);
HAL_StatusTypeDef HAL_CAN_Sleep(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef HAL_CAN_WakeUp(CAN_HandleTypeDef *hcan);
void HAL_CAN_IRQHandler(CAN_HandleTypeDef* hcan);
void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan);
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan);
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan);
/**
  * @}
  */

/** @addtogroup CAN_Exported_Functions_Group3
  * @brief   CAN 外设状态函数 
  * @{
  */
/* 外设状态与错误管理函数 ***************************************/
uint32_t HAL_CAN_GetError(CAN_HandleTypeDef *hcan);
HAL_CAN_StateTypeDef HAL_CAN_GetState(CAN_HandleTypeDef* hcan);
/**
  * @}
  */

/**
  * @}
  */

/* 私有宏 --------------------------------------------------------*/
/** @defgroup CAN_Private_Macros   CAN 私有宏
  * @{
  */

#define IS_CAN_MODE(MODE) (((MODE) == CAN_MODE_NORMAL) || \
                           ((MODE) == CAN_MODE_LOOPBACK)|| \
                           ((MODE) == CAN_MODE_SILENT) || \
                           ((MODE) == CAN_MODE_SILENT_LOOPBACK))
#define IS_CAN_SJW(SJW) (((SJW) == CAN_SJW_1TQ) || ((SJW) == CAN_SJW_2TQ)|| \
                         ((SJW) == CAN_SJW_3TQ) || ((SJW) == CAN_SJW_4TQ))
#define IS_CAN_BS1(BS1) ((BS1) <= CAN_BS1_16TQ)
#define IS_CAN_BS2(BS2) ((BS2) <= CAN_BS2_8TQ)
#define IS_CAN_PRESCALER(PRESCALER) (((PRESCALER) >= 1U) && ((PRESCALER) <= 1024U))

#define IS_CAN_FILTER_MODE(MODE) (((MODE) == CAN_FILTERMODE_IDMASK) || \
                                  ((MODE) == CAN_FILTERMODE_IDLIST))
#define IS_CAN_FILTER_SCALE(SCALE) (((SCALE) == CAN_FILTERSCALE_16BIT) || \
                                    ((SCALE) == CAN_FILTERSCALE_32BIT))
#define IS_CAN_FILTER_FIFO(FIFO) (((FIFO) == CAN_FILTER_FIFO0) || \
                                  ((FIFO) == CAN_FILTER_FIFO1))
#define IS_CAN_BANKNUMBER(BANKNUMBER) ((BANKNUMBER) <= 28U)

#define IS_CAN_TRANSMITMAILBOX(TRANSMITMAILBOX) ((TRANSMITMAILBOX) <= ((uint8_t)0x02))
#define IS_CAN_STDID(STDID)   ((STDID) <= 0x00007FFU)
#define IS_CAN_EXTID(EXTID)   ((EXTID) <= 0x1FFFFFFFU)
#define IS_CAN_DLC(DLC)       ((DLC) <= ((uint8_t)0x08))

#define IS_CAN_IDTYPE(IDTYPE)  (((IDTYPE) == CAN_ID_STD) || \
                                ((IDTYPE) == CAN_ID_EXT))
#define IS_CAN_RTR(RTR) (((RTR) == CAN_RTR_DATA) || ((RTR) == CAN_RTR_REMOTE))
#define IS_CAN_FIFO(FIFO) (((FIFO) == CAN_FIFO0) || ((FIFO) == CAN_FIFO1))

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* STM32F103x6) || STM32F103xB || STM32F103xE || STM32F103xG) || STM32F105xC || STM32F107xC */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_CAN_LEGACY_H */

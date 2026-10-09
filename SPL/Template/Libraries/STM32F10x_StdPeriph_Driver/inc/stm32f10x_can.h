/**
  ******************************************************************************
  * @file    stm32f10x_can.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 CAN 固件库的全部函数原型。
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
#ifndef __STM32F10x_CAN_H
#define __STM32F10x_CAN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup CAN   CAN 总线
  * @{
  */

/** @defgroup CAN_Exported_Types   CAN 导出类型
  * @{
  */

#define IS_CAN_ALL_PERIPH(PERIPH) (((PERIPH) == CAN1) || \
                                   ((PERIPH) == CAN2))

/**
  * @brief  CAN 初始化结构体定义
  */

typedef struct
{
  uint16_t CAN_Prescaler;   /*!< 指定一个时间份额的长度。
                                 取值范围为 1 到 1024。 */
  
  uint8_t CAN_Mode;         /*!< 指定 CAN 的工作模式。
                                 该参数可取 @ref CAN_operating_mode 中的值 */

  uint8_t CAN_SJW;          /*!< 指定 CAN 硬件为进行重新同步
                                 而对某一位允许延长或缩短的
                                 最大时间份额数。
                                 该参数可取
                                 @ref CAN_synchronisation_jump_width 中的值 */

  uint8_t CAN_BS1;          /*!< 指定位段 1 中的时间份额数。
                                 该参数可取
                                 @ref CAN_time_quantum_in_bit_segment_1 中的值 */

  uint8_t CAN_BS2;          /*!< 指定位段 2 中的时间份额数。
                                 该参数可取
                                 @ref CAN_time_quantum_in_bit_segment_2 中的值 */
  
  FunctionalState CAN_TTCM; /*!< 使能或关闭时间触发通信模式。
                                 该参数可设为 ENABLE 或 DISABLE。 */
  
  FunctionalState CAN_ABOM;  /*!< 使能或关闭自动总线关闭管理。
                                  该参数可设为 ENABLE 或 DISABLE。 */

  FunctionalState CAN_AWUM;  /*!< 使能或关闭自动唤醒模式。
                                  该参数可设为 ENABLE 或 DISABLE。 */

  FunctionalState CAN_NART;  /*!< 使能或关闭禁止自动重传模式。
                                  该参数可设为 ENABLE 或 DISABLE。 */

  FunctionalState CAN_RFLM;  /*!< 使能或关闭接收 FIFO 锁定模式。
                                  该参数可设为 ENABLE 或 DISABLE。 */

  FunctionalState CAN_TXFP;  /*!< 使能或关闭发送 FIFO 优先级。
                                  该参数可设为 ENABLE 或 DISABLE。 */
} CAN_InitTypeDef;

/**
  * @brief  CAN 过滤器初始化结构体定义
  */

typedef struct
{
  uint16_t CAN_FilterIdHigh;         /*!< 指定过滤器标识符编号（32 位配置时为高 16 位，
                                              16 位配置时为第一个）。
                                              该参数可取 0x0000 到 0xFFFF 之间的值 */

  uint16_t CAN_FilterIdLow;          /*!< 指定过滤器标识符编号（32 位配置时为低 16 位，
                                              16 位配置时为第二个）。
                                              该参数可取 0x0000 到 0xFFFF 之间的值 */

  uint16_t CAN_FilterMaskIdHigh;     /*!< 根据模式指定过滤器掩码编号或标识符编号
                                              （32 位配置时为高 16 位，
                                              16 位配置时为第一个）。
                                              该参数可取 0x0000 到 0xFFFF 之间的值 */

  uint16_t CAN_FilterMaskIdLow;      /*!< 根据模式指定过滤器掩码编号或标识符编号
                                              （32 位配置时为低 16 位，
                                              16 位配置时为第二个）。
                                              该参数可取 0x0000 到 0xFFFF 之间的值 */

  uint16_t CAN_FilterFIFOAssignment; /*!< 指定分配给该过滤器的 FIFO（0 或 1）。
                                              该参数可取 @ref CAN_filter_FIFO 中的值 */
  
  uint8_t CAN_FilterNumber;          /*!< 指定将要初始化的过滤器。取值范围为 0 到 13。 */

  uint8_t CAN_FilterMode;            /*!< 指定要初始化的过滤器模式。
                                              该参数可取 @ref CAN_filter_mode 中的值 */

  uint8_t CAN_FilterScale;           /*!< 指定过滤器位宽。
                                              该参数可取 @ref CAN_filter_scale 中的值 */

  FunctionalState CAN_FilterActivation; /*!< 使能或关闭该过滤器。
                                              该参数可设为 ENABLE 或 DISABLE。 */
} CAN_FilterInitTypeDef;

/**
  * @brief  CAN 发送报文结构体定义
  */

typedef struct
{
  uint32_t StdId;  /*!< 指定标准标识符。
                        该参数可取 0 到 0x7FF 之间的值。 */

  uint32_t ExtId;  /*!< 指定扩展标识符。
                        该参数可取 0 到 0x1FFFFFFF 之间的值。 */

  uint8_t IDE;     /*!< 指定将要发送报文的标识符类型。
                        该参数可取
                        @ref CAN_identifier_type 中的值 */

  uint8_t RTR;     /*!< 指定将要发送报文的帧类型。
                        该参数可取
                        @ref CAN_remote_transmission_request 中的值 */

  uint8_t DLC;     /*!< 指定将要发送帧的长度。
                        该参数可取 0 到 8 之间的值 */

  uint8_t Data[8]; /*!< 包含将要发送的数据。取值范围为 0 到 0xFF。 */
} CanTxMsg;

/**
  * @brief  CAN 接收报文结构体定义
  */

typedef struct
{
  uint32_t StdId;  /*!< 指定标准标识符。
                        该参数可取 0 到 0x7FF 之间的值。 */

  uint32_t ExtId;  /*!< 指定扩展标识符。
                        该参数可取 0 到 0x1FFFFFFF 之间的值。 */

  uint8_t IDE;     /*!< 指定将要接收报文的标识符类型。
                        该参数可取
                        @ref CAN_identifier_type 中的值 */

  uint8_t RTR;     /*!< 指定所接收报文的帧类型。
                        该参数可取
                        @ref CAN_remote_transmission_request 中的值 */

  uint8_t DLC;     /*!< 指定将要接收帧的长度。
                        该参数可取 0 到 8 之间的值 */

  uint8_t Data[8]; /*!< 包含将要接收的数据。取值范围为 0 到 0xFF。 */

  uint8_t FMI;     /*!< 指定邮箱中存储的报文所通过的过滤器索引。
                        该参数可取 0 到 0xFF 之间的值 */
} CanRxMsg;

/**
  * @}
  */

/** @defgroup CAN_Exported_Constants   CAN 导出常量
  * @{
  */

/** @defgroup CAN_sleep_constants   CAN 睡眠常量
  * @{
  */

#define CAN_InitStatus_Failed              ((uint8_t)0x00) /*!< CAN 初始化失败 */
#define CAN_InitStatus_Success             ((uint8_t)0x01) /*!< CAN 初始化成功 */

/**
  * @}
  */

/** @defgroup CAN_Mode   CAN 模式
  * @{
  */

#define CAN_Mode_Normal             ((uint8_t)0x00)  /*!< 正常模式 */
#define CAN_Mode_LoopBack           ((uint8_t)0x01)  /*!< 回环模式 */
#define CAN_Mode_Silent             ((uint8_t)0x02)  /*!< 静默模式 */
#define CAN_Mode_Silent_LoopBack    ((uint8_t)0x03)  /*!< 回环与静默组合模式 */

#define IS_CAN_MODE(MODE) (((MODE) == CAN_Mode_Normal) || \
                           ((MODE) == CAN_Mode_LoopBack)|| \
                           ((MODE) == CAN_Mode_Silent) || \
                           ((MODE) == CAN_Mode_Silent_LoopBack))
/**
  * @}
  */


/**
  * @defgroup CAN_Operating_Mode   CAN 工作模式
  * @{
  */  
#define CAN_OperatingMode_Initialization  ((uint8_t)0x00) /*!< 初始化模式 */
#define CAN_OperatingMode_Normal          ((uint8_t)0x01) /*!< 正常模式 */
#define CAN_OperatingMode_Sleep           ((uint8_t)0x02) /*!< 睡眠模式 */


#define IS_CAN_OPERATING_MODE(MODE) (((MODE) == CAN_OperatingMode_Initialization) ||\
                                    ((MODE) == CAN_OperatingMode_Normal)|| \
																		((MODE) == CAN_OperatingMode_Sleep))
/**
  * @}
  */
  
/**
  * @defgroup CAN_Mode_Status   CAN 模式状态
  * @{
  */  

#define CAN_ModeStatus_Failed    ((uint8_t)0x00)                /*!< CAN 进入该模式失败 */
#define CAN_ModeStatus_Success   ((uint8_t)!CAN_ModeStatus_Failed)   /*!< CAN 成功进入该模式 */


/**
  * @}
  */

/** @defgroup CAN_synchronisation_jump_width   同步跳转宽度
  * @{
  */

#define CAN_SJW_1tq                 ((uint8_t)0x00)  /*!< 1 个时间份额 */
#define CAN_SJW_2tq                 ((uint8_t)0x01)  /*!< 2 个时间份额 */
#define CAN_SJW_3tq                 ((uint8_t)0x02)  /*!< 3 个时间份额 */
#define CAN_SJW_4tq                 ((uint8_t)0x03)  /*!< 4 个时间份额 */

#define IS_CAN_SJW(SJW) (((SJW) == CAN_SJW_1tq) || ((SJW) == CAN_SJW_2tq)|| \
                         ((SJW) == CAN_SJW_3tq) || ((SJW) == CAN_SJW_4tq))
/**
  * @}
  */

/** @defgroup CAN_time_quantum_in_bit_segment_1   位段 1 中的时间份额数
  * @{
  */

#define CAN_BS1_1tq                 ((uint8_t)0x00)  /*!< 1 个时间份额 */
#define CAN_BS1_2tq                 ((uint8_t)0x01)  /*!< 2 个时间份额 */
#define CAN_BS1_3tq                 ((uint8_t)0x02)  /*!< 3 个时间份额 */
#define CAN_BS1_4tq                 ((uint8_t)0x03)  /*!< 4 个时间份额 */
#define CAN_BS1_5tq                 ((uint8_t)0x04)  /*!< 5 个时间份额 */
#define CAN_BS1_6tq                 ((uint8_t)0x05)  /*!< 6 个时间份额 */
#define CAN_BS1_7tq                 ((uint8_t)0x06)  /*!< 7 个时间份额 */
#define CAN_BS1_8tq                 ((uint8_t)0x07)  /*!< 8 个时间份额 */
#define CAN_BS1_9tq                 ((uint8_t)0x08)  /*!< 9 个时间份额 */
#define CAN_BS1_10tq                ((uint8_t)0x09)  /*!< 10 个时间份额 */
#define CAN_BS1_11tq                ((uint8_t)0x0A)  /*!< 11 个时间份额 */
#define CAN_BS1_12tq                ((uint8_t)0x0B)  /*!< 12 个时间份额 */
#define CAN_BS1_13tq                ((uint8_t)0x0C)  /*!< 13 个时间份额 */
#define CAN_BS1_14tq                ((uint8_t)0x0D)  /*!< 14 个时间份额 */
#define CAN_BS1_15tq                ((uint8_t)0x0E)  /*!< 15 个时间份额 */
#define CAN_BS1_16tq                ((uint8_t)0x0F)  /*!< 16 个时间份额 */

#define IS_CAN_BS1(BS1) ((BS1) <= CAN_BS1_16tq)
/**
  * @}
  */

/** @defgroup CAN_time_quantum_in_bit_segment_2   位段 2 中的时间份额数
  * @{
  */

#define CAN_BS2_1tq                 ((uint8_t)0x00)  /*!< 1 个时间份额 */
#define CAN_BS2_2tq                 ((uint8_t)0x01)  /*!< 2 个时间份额 */
#define CAN_BS2_3tq                 ((uint8_t)0x02)  /*!< 3 个时间份额 */
#define CAN_BS2_4tq                 ((uint8_t)0x03)  /*!< 4 个时间份额 */
#define CAN_BS2_5tq                 ((uint8_t)0x04)  /*!< 5 个时间份额 */
#define CAN_BS2_6tq                 ((uint8_t)0x05)  /*!< 6 个时间份额 */
#define CAN_BS2_7tq                 ((uint8_t)0x06)  /*!< 7 个时间份额 */
#define CAN_BS2_8tq                 ((uint8_t)0x07)  /*!< 8 个时间份额 */

#define IS_CAN_BS2(BS2) ((BS2) <= CAN_BS2_8tq)

/**
  * @}
  */

/** @defgroup CAN_clock_prescaler   CAN 时钟预分频
  * @{
  */

#define IS_CAN_PRESCALER(PRESCALER) (((PRESCALER) >= 1) && ((PRESCALER) <= 1024))

/**
  * @}
  */

/** @defgroup CAN_filter_number   过滤器编号
  * @{
  */
#ifndef STM32F10X_CL
  #define IS_CAN_FILTER_NUMBER(NUMBER) ((NUMBER) <= 13)
#else
  #define IS_CAN_FILTER_NUMBER(NUMBER) ((NUMBER) <= 27)
#endif /* STM32F10X_CL */ 
/**
  * @}
  */

/** @defgroup CAN_filter_mode   过滤器模式
  * @{
  */

#define CAN_FilterMode_IdMask       ((uint8_t)0x00)  /*!< 标识符/掩码模式 */
#define CAN_FilterMode_IdList       ((uint8_t)0x01)  /*!< 标识符列表模式 */

#define IS_CAN_FILTER_MODE(MODE) (((MODE) == CAN_FilterMode_IdMask) || \
                                  ((MODE) == CAN_FilterMode_IdList))
/**
  * @}
  */

/** @defgroup CAN_filter_scale   过滤器位宽
  * @{
  */

#define CAN_FilterScale_16bit       ((uint8_t)0x00) /*!< 两个 16 位过滤器 */
#define CAN_FilterScale_32bit       ((uint8_t)0x01) /*!< 一个 32 位过滤器 */

#define IS_CAN_FILTER_SCALE(SCALE) (((SCALE) == CAN_FilterScale_16bit) || \
                                    ((SCALE) == CAN_FilterScale_32bit))

/**
  * @}
  */

/** @defgroup CAN_filter_FIFO   过滤器 FIFO
  * @{
  */

#define CAN_Filter_FIFO0             ((uint8_t)0x00)  /*!< 将过滤器 x 分配到 FIFO 0 */
#define CAN_Filter_FIFO1             ((uint8_t)0x01)  /*!< 将过滤器 x 分配到 FIFO 1 */
#define IS_CAN_FILTER_FIFO(FIFO) (((FIFO) == CAN_FilterFIFO0) || \
                                  ((FIFO) == CAN_FilterFIFO1))
/**
  * @}
  */

/** @defgroup Start_bank_filter_for_slave_CAN   从 CAN 的起始过滤器组
  * @{
  */
#define IS_CAN_BANKNUMBER(BANKNUMBER) (((BANKNUMBER) >= 1) && ((BANKNUMBER) <= 27))
/**
  * @}
  */

/** @defgroup CAN_Tx   CAN 发送
  * @{
  */

#define IS_CAN_TRANSMITMAILBOX(TRANSMITMAILBOX) ((TRANSMITMAILBOX) <= ((uint8_t)0x02))
#define IS_CAN_STDID(STDID)   ((STDID) <= ((uint32_t)0x7FF))
#define IS_CAN_EXTID(EXTID)   ((EXTID) <= ((uint32_t)0x1FFFFFFF))
#define IS_CAN_DLC(DLC)       ((DLC) <= ((uint8_t)0x08))

/**
  * @}
  */

/** @defgroup CAN_identifier_type   标识符类型
  * @{
  */

#define CAN_Id_Standard             ((uint32_t)0x00000000)  /*!< 标准标识符 */
#define CAN_Id_Extended             ((uint32_t)0x00000004)  /*!< 扩展标识符 */
#define IS_CAN_IDTYPE(IDTYPE) (((IDTYPE) == CAN_Id_Standard) || \
                               ((IDTYPE) == CAN_Id_Extended))
/**
  * @}
  */

/** @defgroup CAN_remote_transmission_request   远程发送请求
  * @{
  */

#define CAN_RTR_Data                ((uint32_t)0x00000000)  /*!< 数据帧 */
#define CAN_RTR_Remote              ((uint32_t)0x00000002)  /*!< 远程帧 */
#define IS_CAN_RTR(RTR) (((RTR) == CAN_RTR_Data) || ((RTR) == CAN_RTR_Remote))

/**
  * @}
  */

/** @defgroup CAN_transmit_constants   CAN 发送常量
  * @{
  */

#define CAN_TxStatus_Failed         ((uint8_t)0x00)/*!< CAN 发送失败 */
#define CAN_TxStatus_Ok             ((uint8_t)0x01) /*!< CAN 发送成功 */
#define CAN_TxStatus_Pending        ((uint8_t)0x02) /*!< CAN 发送挂起 */
#define CAN_TxStatus_NoMailBox      ((uint8_t)0x04) /*!< CAN 单元未能提供空闲邮箱 */

/**
  * @}
  */

/** @defgroup CAN_receive_FIFO_number_constants   CAN 接收 FIFO 编号常量
  * @{
  */

#define CAN_FIFO0                 ((uint8_t)0x00) /*!< 使用 CAN FIFO 0 接收 */
#define CAN_FIFO1                 ((uint8_t)0x01) /*!< 使用 CAN FIFO 1 接收 */

#define IS_CAN_FIFO(FIFO) (((FIFO) == CAN_FIFO0) || ((FIFO) == CAN_FIFO1))

/**
  * @}
  */

/** @defgroup CAN_sleep_constants   CAN 睡眠常量
  * @{
  */

#define CAN_Sleep_Failed     ((uint8_t)0x00) /*!< CAN 未进入睡眠模式 */
#define CAN_Sleep_Ok         ((uint8_t)0x01) /*!< CAN 已进入睡眠模式 */

/**
  * @}
  */

/** @defgroup CAN_wake_up_constants   CAN 唤醒常量
  * @{
  */

#define CAN_WakeUp_Failed        ((uint8_t)0x00) /*!< CAN 未退出睡眠模式 */
#define CAN_WakeUp_Ok            ((uint8_t)0x01) /*!< CAN 已退出睡眠模式 */

/**
  * @}
  */

/**
  * @defgroup   CAN_Error_Code_constants   CAN 错误码常量
  * @{
  */  
                                                                
#define CAN_ErrorCode_NoErr           ((uint8_t)0x00) /*!< 无错误 */ 
#define	CAN_ErrorCode_StuffErr        ((uint8_t)0x10) /*!< 填充错误 */ 
#define	CAN_ErrorCode_FormErr         ((uint8_t)0x20) /*!< 格式错误 */ 
#define	CAN_ErrorCode_ACKErr          ((uint8_t)0x30) /*!< 应答错误 */ 
#define	CAN_ErrorCode_BitRecessiveErr ((uint8_t)0x40) /*!< 隐性位错误 */ 
#define	CAN_ErrorCode_BitDominantErr  ((uint8_t)0x50) /*!< 显性位错误 */ 
#define	CAN_ErrorCode_CRCErr          ((uint8_t)0x60) /*!< CRC 错误 */ 
#define	CAN_ErrorCode_SoftwareSetErr  ((uint8_t)0x70) /*!< 软件设置错误 */ 


/**
  * @}
  */

/** @defgroup CAN_flags   CAN 标志
  * @{
  */
/* 若标志为 0x3XXXXXXX，表示它可用于 CAN_GetFlagStatus()
   和 CAN_ClearFlag() 函数。 */
/* 若标志为 0x1XXXXXXX，表示它只能用于 CAN_GetFlagStatus() 函数。 */

/* 发送标志 */
#define CAN_FLAG_RQCP0             ((uint32_t)0x38000001) /*!< 请求邮箱 0 标志 */
#define CAN_FLAG_RQCP1             ((uint32_t)0x38000100) /*!< 请求邮箱 1 标志 */
#define CAN_FLAG_RQCP2             ((uint32_t)0x38010000) /*!< 请求邮箱 2 标志 */

/* 接收标志 */
#define CAN_FLAG_FMP0              ((uint32_t)0x12000003) /*!< FIFO 0 报文待处理标志 */
#define CAN_FLAG_FF0               ((uint32_t)0x32000008) /*!< FIFO 0 满标志 */
#define CAN_FLAG_FOV0              ((uint32_t)0x32000010) /*!< FIFO 0 溢出标志 */
#define CAN_FLAG_FMP1              ((uint32_t)0x14000003) /*!< FIFO 1 报文待处理标志 */
#define CAN_FLAG_FF1               ((uint32_t)0x34000008) /*!< FIFO 1 满标志 */
#define CAN_FLAG_FOV1              ((uint32_t)0x34000010) /*!< FIFO 1 溢出标志 */

/* 工作模式标志 */
#define CAN_FLAG_WKU               ((uint32_t)0x31000008) /*!< 唤醒标志 */
#define CAN_FLAG_SLAK              ((uint32_t)0x31000012) /*!< 睡眠应答标志 */
/* 注意：当 SLAK 中断被关闭（SLKIE=0）时，无法通过轮询获取 SLAKI。
         此时可以改为轮询 SLAK 位。 */

/* 错误标志 */
#define CAN_FLAG_EWG               ((uint32_t)0x10F00001) /*!< 错误警告标志 */
#define CAN_FLAG_EPV               ((uint32_t)0x10F00002) /*!< 错误被动标志 */
#define CAN_FLAG_BOF               ((uint32_t)0x10F00004) /*!< 总线关闭标志 */
#define CAN_FLAG_LEC               ((uint32_t)0x30F00070) /*!< 最近一次错误码标志 */

#define IS_CAN_GET_FLAG(FLAG) (((FLAG) == CAN_FLAG_LEC)  || ((FLAG) == CAN_FLAG_BOF)   || \
                               ((FLAG) == CAN_FLAG_EPV)  || ((FLAG) == CAN_FLAG_EWG)   || \
                               ((FLAG) == CAN_FLAG_WKU)  || ((FLAG) == CAN_FLAG_FOV0)  || \
                               ((FLAG) == CAN_FLAG_FF0)  || ((FLAG) == CAN_FLAG_FMP0)  || \
                               ((FLAG) == CAN_FLAG_FOV1) || ((FLAG) == CAN_FLAG_FF1)   || \
                               ((FLAG) == CAN_FLAG_FMP1) || ((FLAG) == CAN_FLAG_RQCP2) || \
                               ((FLAG) == CAN_FLAG_RQCP1)|| ((FLAG) == CAN_FLAG_RQCP0) || \
                               ((FLAG) == CAN_FLAG_SLAK ))

#define IS_CAN_CLEAR_FLAG(FLAG)(((FLAG) == CAN_FLAG_LEC) || ((FLAG) == CAN_FLAG_RQCP2) || \
                                ((FLAG) == CAN_FLAG_RQCP1)  || ((FLAG) == CAN_FLAG_RQCP0) || \
                                ((FLAG) == CAN_FLAG_FF0)  || ((FLAG) == CAN_FLAG_FOV0) ||\
                                ((FLAG) == CAN_FLAG_FF1) || ((FLAG) == CAN_FLAG_FOV1) || \
                                ((FLAG) == CAN_FLAG_WKU) || ((FLAG) == CAN_FLAG_SLAK))
/**
  * @}
  */

  
/** @defgroup CAN_interrupts   CAN 中断
  * @{
  */


  
#define CAN_IT_TME                  ((uint32_t)0x00000001) /*!< 发送邮箱空中断 */

/* 接收中断 */
#define CAN_IT_FMP0                 ((uint32_t)0x00000002) /*!< FIFO 0 报文待处理中断 */
#define CAN_IT_FF0                  ((uint32_t)0x00000004) /*!< FIFO 0 满中断 */
#define CAN_IT_FOV0                 ((uint32_t)0x00000008) /*!< FIFO 0 溢出中断 */
#define CAN_IT_FMP1                 ((uint32_t)0x00000010) /*!< FIFO 1 报文待处理中断 */
#define CAN_IT_FF1                  ((uint32_t)0x00000020) /*!< FIFO 1 满中断 */
#define CAN_IT_FOV1                 ((uint32_t)0x00000040) /*!< FIFO 1 溢出中断 */

/* 工作模式中断 */
#define CAN_IT_WKU                  ((uint32_t)0x00010000) /*!< 唤醒中断 */
#define CAN_IT_SLK                  ((uint32_t)0x00020000) /*!< 睡眠应答中断 */

/* 错误中断 */
#define CAN_IT_EWG                  ((uint32_t)0x00000100) /*!< 错误警告中断 */
#define CAN_IT_EPV                  ((uint32_t)0x00000200) /*!< 错误被动中断 */
#define CAN_IT_BOF                  ((uint32_t)0x00000400) /*!< 总线关闭中断 */
#define CAN_IT_LEC                  ((uint32_t)0x00000800) /*!< 最近一次错误码中断 */
#define CAN_IT_ERR                  ((uint32_t)0x00008000) /*!< 错误中断 */

/* 以「中断」命名的标志：仅为保持固件兼容性而保留 */
#define CAN_IT_RQCP0   CAN_IT_TME
#define CAN_IT_RQCP1   CAN_IT_TME
#define CAN_IT_RQCP2   CAN_IT_TME


#define IS_CAN_IT(IT)        (((IT) == CAN_IT_TME) || ((IT) == CAN_IT_FMP0)  ||\
                             ((IT) == CAN_IT_FF0)  || ((IT) == CAN_IT_FOV0)  ||\
                             ((IT) == CAN_IT_FMP1) || ((IT) == CAN_IT_FF1)   ||\
                             ((IT) == CAN_IT_FOV1) || ((IT) == CAN_IT_EWG)   ||\
                             ((IT) == CAN_IT_EPV)  || ((IT) == CAN_IT_BOF)   ||\
                             ((IT) == CAN_IT_LEC)  || ((IT) == CAN_IT_ERR)   ||\
                             ((IT) == CAN_IT_WKU)  || ((IT) == CAN_IT_SLK))

#define IS_CAN_CLEAR_IT(IT) (((IT) == CAN_IT_TME) || ((IT) == CAN_IT_FF0)    ||\
                             ((IT) == CAN_IT_FOV0)|| ((IT) == CAN_IT_FF1)    ||\
                             ((IT) == CAN_IT_FOV1)|| ((IT) == CAN_IT_EWG)    ||\
                             ((IT) == CAN_IT_EPV) || ((IT) == CAN_IT_BOF)    ||\
                             ((IT) == CAN_IT_LEC) || ((IT) == CAN_IT_ERR)    ||\
                             ((IT) == CAN_IT_WKU) || ((IT) == CAN_IT_SLK))

/**
  * @}
  */

/** @defgroup CAN_Legacy   兼容旧版命名
  * @{
  */
#define CANINITFAILED               CAN_InitStatus_Failed
#define CANINITOK                   CAN_InitStatus_Success
#define CAN_FilterFIFO0             CAN_Filter_FIFO0
#define CAN_FilterFIFO1             CAN_Filter_FIFO1
#define CAN_ID_STD                  CAN_Id_Standard           
#define CAN_ID_EXT                  CAN_Id_Extended
#define CAN_RTR_DATA                CAN_RTR_Data         
#define CAN_RTR_REMOTE              CAN_RTR_Remote
#define CANTXFAILE                  CAN_TxStatus_Failed
#define CANTXOK                     CAN_TxStatus_Ok
#define CANTXPENDING                CAN_TxStatus_Pending
#define CAN_NO_MB                   CAN_TxStatus_NoMailBox
#define CANSLEEPFAILED              CAN_Sleep_Failed
#define CANSLEEPOK                  CAN_Sleep_Ok
#define CANWAKEUPFAILED             CAN_WakeUp_Failed        
#define CANWAKEUPOK                 CAN_WakeUp_Ok        

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup CAN_Exported_Macros   CAN 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup CAN_Exported_Functions   CAN 导出函数
  * @{
  */
/*  用于将 CAN 配置恢复为默认复位状态的函数 *****/ 
void CAN_DeInit(CAN_TypeDef* CANx);

/* 初始化与配置函数 *********************************/ 
uint8_t CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct);
void CAN_FilterInit(CAN_FilterInitTypeDef* CAN_FilterInitStruct);
void CAN_StructInit(CAN_InitTypeDef* CAN_InitStruct);
void CAN_SlaveStartBank(uint8_t CAN_BankNumber); 
void CAN_DBGFreeze(CAN_TypeDef* CANx, FunctionalState NewState);
void CAN_TTComModeCmd(CAN_TypeDef* CANx, FunctionalState NewState);

/* 发送相关函数 *********************************************************/
uint8_t CAN_Transmit(CAN_TypeDef* CANx, CanTxMsg* TxMessage);
uint8_t CAN_TransmitStatus(CAN_TypeDef* CANx, uint8_t TransmitMailbox);
void CAN_CancelTransmit(CAN_TypeDef* CANx, uint8_t Mailbox);

/* 接收相关函数 **********************************************************/
void CAN_Receive(CAN_TypeDef* CANx, uint8_t FIFONumber, CanRxMsg* RxMessage);
void CAN_FIFORelease(CAN_TypeDef* CANx, uint8_t FIFONumber);
uint8_t CAN_MessagePending(CAN_TypeDef* CANx, uint8_t FIFONumber);


/* 工作模式相关函数 **************************************************/
uint8_t CAN_OperatingModeRequest(CAN_TypeDef* CANx, uint8_t CAN_OperatingMode);
uint8_t CAN_Sleep(CAN_TypeDef* CANx);
uint8_t CAN_WakeUp(CAN_TypeDef* CANx);

/* 错误管理相关函数 *************************************************/
uint8_t CAN_GetLastErrorCode(CAN_TypeDef* CANx);
uint8_t CAN_GetReceiveErrorCounter(CAN_TypeDef* CANx);
uint8_t CAN_GetLSBTransmitErrorCounter(CAN_TypeDef* CANx);

/* 中断与标志管理相关函数 **********************************/
void CAN_ITConfig(CAN_TypeDef* CANx, uint32_t CAN_IT, FunctionalState NewState);
FlagStatus CAN_GetFlagStatus(CAN_TypeDef* CANx, uint32_t CAN_FLAG);
void CAN_ClearFlag(CAN_TypeDef* CANx, uint32_t CAN_FLAG);
ITStatus CAN_GetITStatus(CAN_TypeDef* CANx, uint32_t CAN_IT);
void CAN_ClearITPendingBit(CAN_TypeDef* CANx, uint32_t CAN_IT);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_CAN_H */
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

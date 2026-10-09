/**
  ******************************************************************************
  * @file    stm32f1xx_hal_i2c.h
  * @author  MCD Application Team
  * @brief   I2C HAL 模块头文件。
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * 保留所有权利。
  *
  * 本软件根据可在该软件组件根目录下的 LICENSE 文件中找到的条款获得许可。
  * 若本软件未附带 LICENSE 文件，则按“原样”提供。
  *
  ******************************************************************************
  */

/* 防止递归包含的宏定义 -----------------------------------------------------*/
#ifndef __STM32F1xx_HAL_I2C_H
#define __STM32F1xx_HAL_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup I2C
  * @{
  */

/* 导出的类型 ----------------------------------------------------------------*/
/** @defgroup I2C_Exported_Types I2C 导出的类型
  * @{
  */

/** @defgroup I2C_Configuration_Structure_definition I2C 配置结构体定义
  * @brief  I2C 配置结构体定义
  * @{
  */
typedef struct
{
  uint32_t ClockSpeed;       /*!< 指定时钟频率。
                                  此参数必须设置为低于 400kHz 的值 */

  uint32_t DutyCycle;        /*!< 指定 I2C 快速模式下的占空比。
                                  该参数可以是 @ref I2C_duty_cycle_in_fast_mode 中的一个值 */

  uint32_t OwnAddress1;      /*!< 指定器件自身的第一个地址。
                                  该参数可以是 7 位或 10 位地址。 */

  uint32_t AddressingMode;   /*!< 指定选择 7 位还是 10 位寻址模式。
                                  该参数可以是 @ref I2C_addressing_mode 中的一个值 */

  uint32_t DualAddressMode;  /*!< 指定是否选择双重寻址模式。
                                  该参数可以是 @ref I2C_dual_addressing_mode 中的一个值 */

  uint32_t OwnAddress2;      /*!< 当选择双重寻址模式时，指定器件自身的第二个地址
                                  该参数可以是 7 位地址。 */

  uint32_t GeneralCallMode;  /*!< 指定是否选择广播呼叫（通用呼叫）模式。
                                  该参数可以是 @ref I2C_general_call_addressing_mode 中的一个值 */

  uint32_t NoStretchMode;    /*!< 指定是否选择禁止时钟延展模式（nostretch 模式）。
                                  此参数可取值为 @ref I2C_nostretch_mode 的一个值 */

} I2C_InitTypeDef;

/**
  * @}
  */

/** @defgroup HAL_state_structure_definition HAL state structure definition
  * @brief  HAL 状态结构体定义
  * @note  HAL I2C 状态编码遵循以下位图描述：
  *          b7-b6  错误信息
  *             00 : 无错误
  *             01 : 中止（用户中止请求正在执行）
  *             10 : 超时
  *             11 : 错误
  *          b5     外设初始化状态
  *             0  : 复位（外设未初始化）
  *             1  : 初始化完成（外设已初始化并就绪可用。已调用 HAL I2C 初始化函数）
  *          b4     （未使能/未使用）
  *             x  : 应置为 0
  *          b3
  *             0  : 就绪或忙（未处于监听模式）
  *             1  : 监听（外设处于地址监听模式）
  *          b2     内部固有进程状态
  *             0  : 就绪
  *             1  : 忙（外设正忙于某些配置或内部操作）
  *          b1     接收（Rx）状态
  *             0  : 就绪（无正在进行的接收操作）
  *             1  : 忙（接收操作正在进行）
  *          b0     发送（Tx）状态
  *             0  : 就绪（无正在进行的发送操作）
  *             1  : 忙（发送操作正在进行）
  * @{
  */
typedef enum
{
  HAL_I2C_STATE_RESET             = 0x00U,   /*!< 外设尚未初始化                            */
  HAL_I2C_STATE_READY             = 0x20U,   /*!< 外设已初始化并就绪可用                    */
  HAL_I2C_STATE_BUSY              = 0x24U,   /*!< 内部处理流程正在进行                      */
  HAL_I2C_STATE_BUSY_TX           = 0x21U,   /*!< 数据发送流程正在进行                      */
  HAL_I2C_STATE_BUSY_RX           = 0x22U,   /*!< 数据接收流程正在进行                      */
  HAL_I2C_STATE_LISTEN            = 0x28U,   /*!< 地址监听模式正在进行                      */
  HAL_I2C_STATE_BUSY_TX_LISTEN    = 0x29U,   /*!< 地址监听模式及数据发送
                                                 流程正在进行                               */
  HAL_I2C_STATE_BUSY_RX_LISTEN    = 0x2AU,   /*!< 地址监听模式及数据接收
                                                 流程正在进行                               */
  HAL_I2C_STATE_ABORT             = 0x60U,   /*!< 用户中止请求正在执行                      */
  HAL_I2C_STATE_TIMEOUT           = 0xA0U,   /*!< 超时状态                                  */
  HAL_I2C_STATE_ERROR             = 0xE0U    /*!< 错误                                      */

} HAL_I2C_StateTypeDef;

/**
  * @}
  */

/** @defgroup HAL_mode_structure_definition HAL 模式结构体定义
  * @brief  HAL 模式结构体定义
  * @note  HAL I2C 模式值的编码遵循以下位图描述：\n
  *          b7     (未定义/保留)\n
  *             x  : 应置为 0\n
  *          b6\n
  *             0  : 无\n
  *             1  : 内存模式 (HAL I2C 通信处于内存模式)\n
  *          b5\n
  *             0  : 无\n
  *             1  : 从机模式 (HAL I2C 通信处于从机模式)\n
  *          b4\n
  *             0  : 无\n
  *             1  : 主机模式 (HAL I2C 通信处于主机模式)\n
  *          b3-b2-b1-b0  (未定义/保留)\n
  *             xxxx : 应置为 0000
  * @{
  */
typedef enum
{
  HAL_I2C_MODE_NONE               = 0x00U,   /*!< 当前无进行中的 I2C 通信                   */
  HAL_I2C_MODE_MASTER             = 0x10U,   /*!< I2C 通信处于主机模式                      */
  HAL_I2C_MODE_SLAVE              = 0x20U,   /*!< I2C 通信处于从机模式                      */
  HAL_I2C_MODE_MEM                = 0x40U    /*!< I2C 通信处于内存模式                      */

} HAL_I2C_ModeTypeDef;

/**
  * @}
  */

/** @defgroup I2C_Error_Code_definition I2C 错误码定义
  * @brief  I2C 错误码定义
  * @{
  */
#define HAL_I2C_ERROR_NONE              0x00000000U    /*!< 无错误                */
#define HAL_I2C_ERROR_BERR              0x00000001U    /*!< 总线错误 (BERR)       */
#define HAL_I2C_ERROR_ARLO              0x00000002U    /*!< 仲裁丢失错误 (ARLO)   */
#define HAL_I2C_ERROR_AF                0x00000004U    /*!< 应答失败错误 (AF)     */
#define HAL_I2C_ERROR_OVR               0x00000008U    /*!< 过载/欠载错误 (OVR)   */
#define HAL_I2C_ERROR_DMA               0x00000010U    /*!< DMA 传输错误          */
#define HAL_I2C_ERROR_TIMEOUT           0x00000020U    /*!< 超时错误              */
#define HAL_I2C_ERROR_SIZE              0x00000040U    /*!< 数据大小管理错误      */
#define HAL_I2C_ERROR_DMA_PARAM         0x00000080U    /*!< DMA 参数错误          */
#define HAL_I2C_WRONG_START             0x00000200U    /*!< 启动条件错误          */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
#define HAL_I2C_ERROR_INVALID_CALLBACK  0x00000100U    /*!< 无效的回调函数错误    */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup I2C_handle_Structure_definition I2C 句柄结构体定义
  * @brief  I2C 句柄结构体定义
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct __I2C_HandleTypeDef
#else
typedef struct
#endif  /* USE_HAL_I2C_REGISTER_CALLBACKS */
{
  I2C_TypeDef                *Instance;      /*!< I2C 寄存器基地址                         */

  I2C_InitTypeDef            Init;           /*!< I2C 通信参数                             */

  uint8_t                    *pBuffPtr;      /*!< 指向 I2C 传输缓冲区的指针                */

  uint16_t                   XferSize;       /*!< I2C 传输大小                             */

  __IO uint16_t              XferCount;      /*!< I2C 传输计数器                           */

  __IO uint32_t              XferOptions;    /*!< I2C 传输选项                             */

  __IO uint32_t              PreviousState;  /*!< 供内部使用的 I2C 通信先前状态和模式上下文 */

  DMA_HandleTypeDef          *hdmatx;        /*!< I2C 发送 DMA 句柄参数                    */

  DMA_HandleTypeDef          *hdmarx;        /*!< I2C 接收 DMA 句柄参数                    */

  HAL_LockTypeDef            Lock;           /*!< I2C 锁对象                               */

  __IO HAL_I2C_StateTypeDef  State;          /*!< I2C 通信状态                             */

  __IO HAL_I2C_ModeTypeDef   Mode;           /*!< I2C 通信模式                             */

  __IO uint32_t              ErrorCode;      /*!< I2C 错误代码                             */

  __IO uint32_t              Devaddress;     /*!< I2C 目标设备地址                         */

  __IO uint32_t              Memaddress;     /*!< I2C 目标内存地址                         */

  __IO uint32_t              MemaddSize;     /*!< I2C 目标内存地址大小                     */

  __IO uint32_t              EventCount;     /*!< I2C 事件计数器                           */


#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
  void (* MasterTxCpltCallback)(struct __I2C_HandleTypeDef *hi2c);           /*!< I2C 主机发送传输完成回调函数             */
  void (* MasterRxCpltCallback)(struct __I2C_HandleTypeDef *hi2c);           /*!< I2C 主机接收传输完成回调函数             */
  void (* SlaveTxCpltCallback)(struct __I2C_HandleTypeDef *hi2c);            /*!< I2C 从机发送传输完成回调函数             */
  void (* SlaveRxCpltCallback)(struct __I2C_HandleTypeDef *hi2c);            /*!< I2C 从机接收传输完成回调函数             */
  void (* ListenCpltCallback)(struct __I2C_HandleTypeDef *hi2c);             /*!< I2C 监听完成回调函数                     */
  void (* MemTxCpltCallback)(struct __I2C_HandleTypeDef *hi2c);              /*!< I2C 内存发送传输完成回调函数             */
  void (* MemRxCpltCallback)(struct __I2C_HandleTypeDef *hi2c);              /*!< I2C 存储器接收传输完成回调函数 */
  void (* ErrorCallback)(struct __I2C_HandleTypeDef *hi2c);                  /*!< I2C 错误回调函数                          */
  void (* AbortCpltCallback)(struct __I2C_HandleTypeDef *hi2c);              /*!< I2C 中止回调函数                          */

  void (* AddrCallback)(struct __I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);  /*!< I2C 从机地址匹配回调函数 */

  void (* MspInitCallback)(struct __I2C_HandleTypeDef *hi2c);                /*!< I2C Msp 初始化回调函数                   */
  void (* MspDeInitCallback)(struct __I2C_HandleTypeDef *hi2c);              /*!< I2C Msp 反初始化回调函数                 */

#endif  /* USE_HAL_I2C_REGISTER_CALLBACKS */
} I2C_HandleTypeDef;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL I2C 回调函数 ID 枚举定义
  */
typedef enum
{
  HAL_I2C_MASTER_TX_COMPLETE_CB_ID      = 0x00U,    /*!< I2C 主机发送传输完成回调函数 ID               */
  HAL_I2C_MASTER_RX_COMPLETE_CB_ID      = 0x01U,    /*!< I2C 主机接收传输完成回调函数 ID               */
  HAL_I2C_SLAVE_TX_COMPLETE_CB_ID       = 0x02U,    /*!< I2C 从机发送传输完成回调函数 ID               */
  HAL_I2C_SLAVE_RX_COMPLETE_CB_ID       = 0x03U,    /*!< I2C 从机接收传输完成回调函数 ID               */
  HAL_I2C_LISTEN_COMPLETE_CB_ID         = 0x04U,    /*!< I2C 监听完成回调函数 ID                       */
  HAL_I2C_MEM_TX_COMPLETE_CB_ID         = 0x05U,    /*!< I2C 存储器发送传输回调函数 ID                 */
  HAL_I2C_MEM_RX_COMPLETE_CB_ID         = 0x06U,    /*!< I2C 存储器接收传输完成回调函数 ID             */
  HAL_I2C_ERROR_CB_ID                   = 0x07U,    /*!< I2C 错误回调函数 ID                           */
  HAL_I2C_ABORT_CB_ID                   = 0x08U,    /*!< I2C 中止回调函数 ID                           */

  HAL_I2C_MSPINIT_CB_ID                 = 0x09U,    /*!< I2C Msp 初始化回调函数 ID                    */
  HAL_I2C_MSPDEINIT_CB_ID               = 0x0AU     /*!< I2C Msp 反初始化回调函数 ID                  */

} HAL_I2C_CallbackIDTypeDef;

/**
  * @brief  HAL I2C 回调函数指针定义
  */
typedef  void (*pI2C_CallbackTypeDef)(I2C_HandleTypeDef *hi2c); /*!< 指向 I2C 回调函数的指针 */
typedef  void (*pI2C_AddrCallbackTypeDef)(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode); /*!< 指向 I2C 地址匹配回调函数的指针 */

#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
/**
  * @}
  */

/**
  * @}
  */
/* 导出常量 ------------------------------------------------------------------*/

/** @defgroup I2C_Exported_Constants I2C 导出常量
  * @{
  */

/** @defgroup I2C_duty_cycle_in_fast_mode I2C 快速模式占空比
  * @{
  */
#define I2C_DUTYCYCLE_2                 0x00000000U
#define I2C_DUTYCYCLE_16_9              I2C_CCR_DUTY
/**
  * @}
  */

/** @defgroup I2C_addressing_mode I2C 寻址模式
  * @{
  */
#define I2C_ADDRESSINGMODE_7BIT         0x00004000U
#define I2C_ADDRESSINGMODE_10BIT        (I2C_OAR1_ADDMODE | 0x00004000U)
/**
  * @}
  */

/** @defgroup I2C_dual_addressing_mode I2C 双地址寻址模式
  * @{
  */
#define I2C_DUALADDRESS_DISABLE        0x00000000U
#define I2C_DUALADDRESS_ENABLE         I2C_OAR2_ENDUAL
/**
  * @}
  */

/** @defgroup I2C_general_call_addressing_mode I2C 广播呼叫寻址模式
  * @{
  */
#define I2C_GENERALCALL_DISABLE        0x00000000U
#define I2C_GENERALCALL_ENABLE         I2C_CR1_ENGC
/**
  * @}
  */

/** @defgroup I2C_nostretch_mode I2C 时钟延展禁用模式 (nostretch)
  * @{
  */
#define I2C_NOSTRETCH_DISABLE          0x00000000U
#define I2C_NOSTRETCH_ENABLE           I2C_CR1_NOSTRETCH
/**
  * @}
  */

/** @defgroup I2C_Memory_Address_Size I2C 内存地址大小
  * @{
  */
#define I2C_MEMADD_SIZE_8BIT            0x00000001U
#define I2C_MEMADD_SIZE_16BIT           0x00000010U
/**
  * @}
  */

/** @defgroup I2C_XferDirection_definition I2C 传输方向定义
  * @{
  */
#define I2C_DIRECTION_RECEIVE           0x00000000U
#define I2C_DIRECTION_TRANSMIT          0x00000001U
/**
  * @}
  */

/** @defgroup I2C_XferOptions_definition I2C 传输选项定义
  * @{
  */
#define  I2C_FIRST_FRAME                0x00000001U
#define  I2C_FIRST_AND_NEXT_FRAME       0x00000002U
#define  I2C_NEXT_FRAME                 0x00000004U
#define  I2C_FIRST_AND_LAST_FRAME       0x00000008U
#define  I2C_LAST_FRAME_NO_STOP         0x00000010U
#define  I2C_LAST_FRAME                 0x00000020U

/* 以下情况下使用的 XferOptions 列表：
 * 1- 所有使用场景下的重复起始条件 (Restart condition)（无论传输方向是否改变）
 */
#define  I2C_OTHER_FRAME                (0x00AA0000U)
#define  I2C_OTHER_AND_LAST_FRAME       (0xAA000000U)
/**
  * @}
  */

/** @defgroup I2C_Interrupt_configuration_definition I2C 中断配置定义
  * @brief I2C 中断定义
  *        元素值规则：0xXXXXXXXX
  *           - XXXXXXXX  : 中断控制掩码
  * @{
  */
#define I2C_IT_BUF                      I2C_CR2_ITBUFEN
#define I2C_IT_EVT                      I2C_CR2_ITEVTEN
#define I2C_IT_ERR                      I2C_CR2_ITERREN
/**
  * @}
  */

/** @defgroup I2C_Flag_definition I2C 标志定义
  * @{
  */

#define I2C_FLAG_OVR                    0x00010800U
#define I2C_FLAG_AF                     0x00010400U
#define I2C_FLAG_ARLO                   0x00010200U
#define I2C_FLAG_BERR                   0x00010100U
#define I2C_FLAG_TXE                    0x00010080U
#define I2C_FLAG_RXNE                   0x00010040U
#define I2C_FLAG_STOPF                  0x00010010U
#define I2C_FLAG_ADD10                  0x00010008U
#define I2C_FLAG_BTF                    0x00010004U
#define I2C_FLAG_ADDR                   0x00010002U
#define I2C_FLAG_SB                     0x00010001U
#define I2C_FLAG_DUALF                  0x00100080U
#define I2C_FLAG_GENCALL                0x00100010U
#define I2C_FLAG_TRA                    0x00100004U
#define I2C_FLAG_BUSY                   0x00100002U
#define I2C_FLAG_MSL                    0x00100001U
/**
  * @}
  */

/**
  * @}
  */

/* 导出的宏 -----------------------------------------------------------*/

/** @defgroup I2C_Exported_Macros I2C 导出的宏
  * @{
  */

/** @brief 重置 I2C 句柄状态。
  * @param  __HANDLE__ 指定 I2C 句柄。
  * @retval 无
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
#define __HAL_I2C_RESET_HANDLE_STATE(__HANDLE__)                do{                                                   \
                                                                    (__HANDLE__)->State = HAL_I2C_STATE_RESET;       \
                                                                    (__HANDLE__)->MspInitCallback = NULL;            \
                                                                    (__HANDLE__)->MspDeInitCallback = NULL;          \
                                                                  } while(0)
#else
#define __HAL_I2C_RESET_HANDLE_STATE(__HANDLE__)                ((__HANDLE__)->State = HAL_I2C_STATE_RESET)
#endif

/** @brief  使能或禁用指定的 I2C 中断。
  * @param  __HANDLE__ 指定 I2C 句柄。
  * @param  __INTERRUPT__ 指定要使能或禁用的中断源。
  *         该参数可以是以下值之一：
  *            @arg I2C_IT_BUF: 缓冲区中断使能
  *            @arg I2C_IT_EVT: 事件中断使能
  *            @arg I2C_IT_ERR: 错误中断使能
  * @retval 无
  */
#define __HAL_I2C_ENABLE_IT(__HANDLE__, __INTERRUPT__)   SET_BIT((__HANDLE__)->Instance->CR2,(__INTERRUPT__))
#define __HAL_I2C_DISABLE_IT(__HANDLE__, __INTERRUPT__)  CLEAR_BIT((__HANDLE__)->Instance->CR2, (__INTERRUPT__))

/** @brief  检查指定的 I2C 中断源是否已使能。
  * @param  __HANDLE__ 指定 I2C 句柄。
  * @param  __INTERRUPT__ 指定要检查的 I2C 中断源。
  *          该参数可以是以下值之一：
  *            @arg I2C_IT_BUF: 缓冲区中断使能
  *            @arg I2C_IT_EVT: 事件中断使能
  *            @arg I2C_IT_ERR: 错误中断使能
  * @retval __INTERRUPT__ 的新状态（TRUE 或 FALSE）。
  */
#define __HAL_I2C_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) ((((__HANDLE__)->Instance->CR2 & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)

/** @brief  检查指定的 I2C 标志位是否置位。
  * @param  __HANDLE__ 指定 I2C 句柄。
  * @param  __FLAG__ 指定要检查的标志位。
  *         该参数可以是以下值之一：
  *            @arg I2C_FLAG_OVR: 过载/欠载标志
  *            @arg I2C_FLAG_AF: 应答失败标志
  *            @arg I2C_FLAG_ARLO: 仲裁丢失标志
  *            @arg I2C_FLAG_BERR: 总线错误标志
  *            @arg I2C_FLAG_TXE: 数据寄存器为空标志
  *            @arg I2C_FLAG_RXNE: 数据寄存器非空标志
  *            @arg I2C_FLAG_STOPF: 停止条件检测标志
  *            @arg I2C_FLAG_ADD10: 10 位头序列已发送标志
  *            @arg I2C_FLAG_BTF: 字节传输完成标志
  *            @arg I2C_FLAG_ADDR: 地址已发送标志
  *                                地址匹配标志
  *            @arg I2C_FLAG_SB: 起始位标志
  *            @arg I2C_FLAG_DUALF: 双重地址标志
  *            @arg I2C_FLAG_GENCALL: 广播呼叫头标志
  *            @arg I2C_FLAG_TRA: 发送器/接收器标志
  *            @arg I2C_FLAG_BUSY: 总线忙标志
  *            @arg I2C_FLAG_MSL: 主/从模式标志
  * @retval __FLAG__ 的新状态（TRUE 或 FALSE）。
  */
#define __HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__) ((((uint8_t)((__FLAG__) >> 16U)) == 0x01U) ? \
                                                  (((((__HANDLE__)->Instance->SR1) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET) : \
                                                  (((((__HANDLE__)->Instance->SR2) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET))

/** @brief  清除通过向特定位写 0 来清除的 I2C 挂起标志。
  * @param  __HANDLE__ 指定 I2C 句柄。
  * @param  __FLAG__ 指定要清除的标志。
  *         该参数可以是以下值的任意组合：
  *            @arg I2C_FLAG_OVR: 过载/欠载标志（从模式）
  *            @arg I2C_FLAG_AF: 应答失败标志
  *            @arg I2C_FLAG_ARLO: 仲裁丢失标志（主模式）
  *            @arg I2C_FLAG_BERR: 总线错误标志
  * @retval 无
  */
#define __HAL_I2C_CLEAR_FLAG(__HANDLE__, __FLAG__) ((__HANDLE__)->Instance->SR1 = ~((__FLAG__) & I2C_FLAG_MASK))

/** @brief  清除 I2C ADDR 挂起标志。
  * @param  __HANDLE__ 指定 I2C 句柄。
  *         该参数可以是 I2Cx（其中 x 可为 1、2 或 3）以选择 I2C 外设。
  * @retval 无
  */
#define __HAL_I2C_CLEAR_ADDRFLAG(__HANDLE__)    \
  do{                                           \
    __IO uint32_t tmpreg = 0x00U;               \
    tmpreg = (__HANDLE__)->Instance->SR1;       \
    tmpreg = (__HANDLE__)->Instance->SR2;       \
    UNUSED(tmpreg);                             \
  } while(0)

/** @brief  清除 I2C STOPF 挂起标志。
  * @param  __HANDLE__ 指定 I2C 句柄。
  * @retval 无
  */
#define __HAL_I2C_CLEAR_STOPFLAG(__HANDLE__)           \
  do{                                                  \
    __IO uint32_t tmpreg = 0x00U;                      \
    tmpreg = (__HANDLE__)->Instance->SR1;              \
    SET_BIT((__HANDLE__)->Instance->CR1, I2C_CR1_PE);  \
    UNUSED(tmpreg);                                    \
  } while(0)

/** @brief  使能指定的 I2C 外设。
  * @param  __HANDLE__ 指定 I2C 句柄。
  * @retval 无
  */
#define __HAL_I2C_ENABLE(__HANDLE__)                  SET_BIT((__HANDLE__)->Instance->CR1, I2C_CR1_PE)

/** @brief  禁用指定的 I2C 外设。
  * @param  __HANDLE__ 指定 I2C 句柄。
  * @retval 无
  */
#define __HAL_I2C_DISABLE(__HANDLE__)                 CLEAR_BIT((__HANDLE__)->Instance->CR1, I2C_CR1_PE)

/**
  * @}
  */

/* 导出函数 --------------------------------------------------------*/
/** @addtogroup I2C_Exported_Functions
  * @{
  */

/** @addtogroup I2C_Exported_Functions_Group1 初始化和反初始化函数
  * @{
  */
/* 初始化和反初始化函数******************************/
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c);

/* 回调函数注册/注销函数  ***********************************/
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
HAL_StatusTypeDef HAL_I2C_RegisterCallback(I2C_HandleTypeDef *hi2c, HAL_I2C_CallbackIDTypeDef CallbackID, pI2C_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_I2C_UnRegisterCallback(I2C_HandleTypeDef *hi2c, HAL_I2C_CallbackIDTypeDef CallbackID);

HAL_StatusTypeDef HAL_I2C_RegisterAddrCallback(I2C_HandleTypeDef *hi2c, pI2C_AddrCallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_I2C_UnRegisterAddrCallback(I2C_HandleTypeDef *hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @addtogroup I2C_Exported_Functions_Group2 输入和输出操作函数
  * @{
  */
/* IO 操作函数  ****************************************************/
/******* 阻塞模式：轮询 */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout);

/******* 非阻塞模式：中断 */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Slave_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Mem_Write_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Mem_Read_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);

HAL_StatusTypeDef HAL_I2C_Master_Seq_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_Master_Seq_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_EnableListen_IT(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_DisableListen_IT(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_Master_Abort_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress);

/******* 非阻塞模式：DMA */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Slave_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Mem_Write_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Mem_Read_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);

HAL_StatusTypeDef HAL_I2C_Master_Seq_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_Master_Seq_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
/**
  * @}
  */

/** @addtogroup I2C_IRQ_Handler_and_Callbacks 中断处理程序和回调函数
 * @{
 */
/******* 用于非阻塞模式（中断和 DMA）的 I2C 中断处理函数及回调函数 */
void HAL_I2C_EV_IRQHandler(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ER_IRQHandler(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c);
/**
  * @}
  */

/** @addtogroup I2C_Exported_Functions_Group3 外设状态、模式与错误管理函数
  * @{
  */
/* 外设状态、模式与错误管理函数  *********************************/
HAL_I2C_StateTypeDef HAL_I2C_GetState(I2C_HandleTypeDef *hi2c);
HAL_I2C_ModeTypeDef HAL_I2C_GetMode(I2C_HandleTypeDef *hi2c);
uint32_t HAL_I2C_GetError(I2C_HandleTypeDef *hi2c);

/**
  * @}
  */

/**
  * @}
  */
/* 私有类型定义 -------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/
/* 私有常量 ---------------------------------------------------------*/
/** @defgroup I2C_Private_Constants I2C 私有常量
  * @{
  */
#define I2C_FLAG_MASK                    0x0000FFFFU
#define I2C_MIN_PCLK_FREQ_STANDARD       2000000U     /*!< 2 MHz                     */
#define I2C_MIN_PCLK_FREQ_FAST           4000000U     /*!< 4 MHz                     */
/**
  * @}
  */

/* 私有宏 ------------------------------------------------------------*/
/** @defgroup I2C_Private_Macros I2C 私有宏
  * @{
  */

#define I2C_MIN_PCLK_FREQ(__PCLK__, __SPEED__)             (((__SPEED__) <= 100000U) ? ((__PCLK__) < I2C_MIN_PCLK_FREQ_STANDARD) : ((__PCLK__) < I2C_MIN_PCLK_FREQ_FAST))
#define I2C_CCR_CALCULATION(__PCLK__, __SPEED__, __COEFF__)     (((((__PCLK__) - 1U)/((__SPEED__) * (__COEFF__))) + 1U) & I2C_CCR_CCR)
#define I2C_FREQRANGE(__PCLK__)                            ((__PCLK__)/1000000U)
#define I2C_RISE_TIME(__FREQRANGE__, __SPEED__)            (((__SPEED__) <= 100000U) ? ((__FREQRANGE__) + 1U) : ((((__FREQRANGE__) * 300U) / 1000U) + 1U))
#define I2C_SPEED_STANDARD(__PCLK__, __SPEED__)            ((I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 2U) < 4U)? 4U:I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 2U))
#define I2C_SPEED_FAST(__PCLK__, __SPEED__, __DUTYCYCLE__) (((__DUTYCYCLE__) == I2C_DUTYCYCLE_2)? I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 3U) : (I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 25U) | I2C_DUTYCYCLE_16_9))
#define I2C_SPEED(__PCLK__, __SPEED__, __DUTYCYCLE__)      (((__SPEED__) <= 100000U)? (I2C_SPEED_STANDARD((__PCLK__), (__SPEED__))) : \
                                                                  ((I2C_SPEED_FAST((__PCLK__), (__SPEED__), (__DUTYCYCLE__)) & I2C_CCR_CCR) == 0U)? 1U : \
                                                                  ((I2C_SPEED_FAST((__PCLK__), (__SPEED__), (__DUTYCYCLE__))) | I2C_CCR_FS))

#define I2C_7BIT_ADD_WRITE(__ADDRESS__)                    ((uint8_t)((__ADDRESS__) & (uint8_t)(~I2C_OAR1_ADD0)))
#define I2C_7BIT_ADD_READ(__ADDRESS__)                     ((uint8_t)((__ADDRESS__) | I2C_OAR1_ADD0))

#define I2C_10BIT_ADDRESS(__ADDRESS__)                     ((uint8_t)((uint16_t)((__ADDRESS__) & (uint16_t)0x00FF)))
#define I2C_10BIT_HEADER_WRITE(__ADDRESS__)                ((uint8_t)((uint16_t)((uint16_t)(((uint16_t)((__ADDRESS__) & (uint16_t)0x0300)) >> 7) | (uint16_t)0x00F0)))
#define I2C_10BIT_HEADER_READ(__ADDRESS__)                 ((uint8_t)((uint16_t)((uint16_t)(((uint16_t)((__ADDRESS__) & (uint16_t)0x0300)) >> 7) | (uint16_t)(0x00F1))))

#define I2C_MEM_ADD_MSB(__ADDRESS__)                       ((uint8_t)((uint16_t)(((uint16_t)((__ADDRESS__) & (uint16_t)0xFF00)) >> 8)))
#define I2C_MEM_ADD_LSB(__ADDRESS__)                       ((uint8_t)((uint16_t)((__ADDRESS__) & (uint16_t)0x00FF)))

/** @defgroup I2C_IS_RTC_Definitions I2C 用于检查输入参数的私有宏
  * @{
  */
#define IS_I2C_DUTY_CYCLE(CYCLE) (((CYCLE) == I2C_DUTYCYCLE_2) || \
                                  ((CYCLE) == I2C_DUTYCYCLE_16_9))
#define IS_I2C_ADDRESSING_MODE(ADDRESS) (((ADDRESS) == I2C_ADDRESSINGMODE_7BIT) || \
                                         ((ADDRESS) == I2C_ADDRESSINGMODE_10BIT))
#define IS_I2C_DUAL_ADDRESS(ADDRESS) (((ADDRESS) == I2C_DUALADDRESS_DISABLE) || \
                                      ((ADDRESS) == I2C_DUALADDRESS_ENABLE))
#define IS_I2C_GENERAL_CALL(CALL) (((CALL) == I2C_GENERALCALL_DISABLE) || \
                                   ((CALL) == I2C_GENERALCALL_ENABLE))
#define IS_I2C_NO_STRETCH(STRETCH) (((STRETCH) == I2C_NOSTRETCH_DISABLE) || \
                                    ((STRETCH) == I2C_NOSTRETCH_ENABLE))
#define IS_I2C_MEMADD_SIZE(SIZE) (((SIZE) == I2C_MEMADD_SIZE_8BIT) || \
                                  ((SIZE) == I2C_MEMADD_SIZE_16BIT))
#define IS_I2C_CLOCK_SPEED(SPEED) (((SPEED) > 0U) && ((SPEED) <= 400000U))
#define IS_I2C_OWN_ADDRESS1(ADDRESS1) (((ADDRESS1) & 0xFFFFFC00U) == 0U)
#define IS_I2C_OWN_ADDRESS2(ADDRESS2) (((ADDRESS2) & 0xFFFFFF01U) == 0U)
#define IS_I2C_TRANSFER_OPTIONS_REQUEST(REQUEST)      (((REQUEST) == I2C_FIRST_FRAME)              || \
                                                       ((REQUEST) == I2C_FIRST_AND_NEXT_FRAME)     || \
                                                       ((REQUEST) == I2C_NEXT_FRAME)               || \
                                                       ((REQUEST) == I2C_FIRST_AND_LAST_FRAME)     || \
                                                       ((REQUEST) == I2C_LAST_FRAME)               || \
                                                       ((REQUEST) == I2C_LAST_FRAME_NO_STOP)       || \
                                                       IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(REQUEST))

#define IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(REQUEST) (((REQUEST) == I2C_OTHER_FRAME)     || \
                                                        ((REQUEST) == I2C_OTHER_AND_LAST_FRAME))

#define I2C_CHECK_FLAG(__ISR__, __FLAG__)         ((((__ISR__) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET)
#define I2C_CHECK_IT_SOURCE(__CR1__, __IT__)      ((((__CR1__) & (__IT__)) == (__IT__)) ? SET : RESET)
/**
  * @}
  */

/**
  * @}
  */

/* 私有函数 ---------------------------------------------------------*/
/** @defgroup I2C_Private_Functions I2C 私有函数
  * @{
  */

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


#endif /* __STM32F1xx_HAL_I2C_H */


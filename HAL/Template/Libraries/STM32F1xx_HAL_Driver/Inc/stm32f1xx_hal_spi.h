/**
  ******************************************************************************
  * @file    stm32f1xx_hal_spi.h
  * @author  MCD 应用团队
  * @brief   SPI HAL 模块头文件。
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * 保留所有权利。
  *
  * 本软件根据该软件组件根目录下的 LICENSE 文件中的条款进行许可。
  * 如果本软件未附带 LICENSE 文件，则按“原样”提供。
  *
  ******************************************************************************
  */

/* 防止递归包含的宏定义 -----------------------------------------------------*/
#ifndef STM32F1xx_HAL_SPI_H
#define STM32F1xx_HAL_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* 头文件包含 ----------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup SPI
  * @{
  */

/* 导出类型 ------------------------------------------------------------------*/
/** @defgroup SPI_Exported_Types SPI 导出类型
  * @{
  */

/**
  * @brief  SPI 配置结构体定义
  */
typedef struct
{
  uint32_t Mode;                /*!< 指定 SPI 工作模式。
                                     该参数可以是 @ref SPI_Mode 的取值 */

  uint32_t Direction;           /*!< 指定 SPI 双向模式状态。
                                     该参数可以是 @ref SPI_Direction 的取值 */

  uint32_t DataSize;            /*!< 指定 SPI 数据大小。
                                     该参数可以是 @ref SPI_Data_Size 的取值 */

  uint32_t CLKPolarity;         /*!< 指定串行时钟稳态（极性）。
                                     该参数可以是 @ref SPI_Clock_Polarity 的取值 */

  uint32_t CLKPhase;            /*!< 指定用于数据位捕获的时钟有效沿（相位）。
                                     该参数可以是 @ref SPI_Clock_Phase 的取值 */

  uint32_t NSS;                 /*!< 指定 NSS 信号是由硬件（NSS 引脚）管理，
                                     还是由软件通过 SSI 位管理。
                                     该参数可以是 @ref SPI_Slave_Select_management 的取值 */

  uint32_t BaudRatePrescaler;   /*!< 指定波特率预分频值，该值将用于
                                     配置发送和接收 SCK 时钟。
                                     该参数可以是 @ref SPI_BaudRate_Prescaler 的取值
                                     @note 通信时钟派生自主机时钟。
                                     从机时钟无需设置。 */

  uint32_t FirstBit;            /*!< 指定数据传输是从 MSB 位还是 LSB 位开始。
                                     该参数可以取 @ref SPI_MSB_LSB_transmission 的值 */

  uint32_t TIMode;              /*!< 指定是否使能 TI 模式。
                                     该参数可以取 @ref SPI_TI_mode 的值 */

  uint32_t CRCCalculation;      /*!< 指定是否使能 CRC 计算。
                                     该参数可以取 @ref SPI_CRC_Calculation 的值 */

  uint32_t CRCPolynomial;       /*!< 指定用于 CRC 计算的多项式。
                                     该参数必须是介于 Min_Data = 1 和 Max_Data = 65535 之间的奇数 */
} SPI_InitTypeDef;

/**
  * @brief  HAL SPI 状态结构体定义
  */
typedef enum
{
  HAL_SPI_STATE_RESET      = 0x00U,    /*!< 外设未初始化                                       */
  HAL_SPI_STATE_READY      = 0x01U,    /*!< 外设已初始化并就绪可用                             */
  HAL_SPI_STATE_BUSY       = 0x02U,    /*!< 内部处理正在进行中                                 */
  HAL_SPI_STATE_BUSY_TX    = 0x03U,    /*!< 数据发送处理正在进行中                             */
  HAL_SPI_STATE_BUSY_RX    = 0x04U,    /*!< 数据接收处理正在进行中                             */
  HAL_SPI_STATE_BUSY_TX_RX = 0x05U,    /*!< 数据发送和接收处理正在进行中                       */
  HAL_SPI_STATE_ERROR      = 0x06U,    /*!< SPI 错误状态                                       */
  HAL_SPI_STATE_ABORT      = 0x07U     /*!< SPI 中止操作正在进行中                             */
} HAL_SPI_StateTypeDef;

/**
  * @brief  SPI 句柄结构体定义
  */
typedef struct __SPI_HandleTypeDef
{
  SPI_TypeDef                *Instance;      /*!< SPI 寄存器基地址                         */

  SPI_InitTypeDef            Init;           /*!< SPI 通信参数                             */

  const uint8_t              *pTxBuffPtr;    /*!< 指向 SPI 发送传输缓冲区的指针            */

  uint16_t                   TxXferSize;     /*!< SPI 发送传输大小                         */

  __IO uint16_t              TxXferCount;    /*!< SPI 发送传输计数器                       */

  uint8_t                    *pRxBuffPtr;    /*!< 指向 SPI 接收传输缓冲区的指针            */

  uint16_t                   RxXferSize;     /*!< SPI 接收传输大小                         */

  __IO uint16_t              RxXferCount;    /*!< SPI 接收传输计数器                       */

  void (*RxISR)(struct __SPI_HandleTypeDef *hspi);   /*!< 接收 ISR 的函数指针              */

  void (*TxISR)(struct __SPI_HandleTypeDef *hspi);   /*!< 发送 ISR 的函数指针              */

  DMA_HandleTypeDef          *hdmatx;        /*!< SPI 发送 DMA 句柄参数                    */

  DMA_HandleTypeDef          *hdmarx;        /*!< SPI 接收 DMA 句柄参数                    */

  HAL_LockTypeDef            Lock;           /*!< 锁对象                                   */

  __IO HAL_SPI_StateTypeDef  State;          /*!< SPI 通信状态                             */

  __IO uint32_t              ErrorCode;      /*!< SPI 错误代码                             */

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  void (* TxCpltCallback)(struct __SPI_HandleTypeDef *hspi);             /*!< SPI 发送完成回调函数               */
  void (* RxCpltCallback)(struct __SPI_HandleTypeDef *hspi);             /*!< SPI 接收完成回调函数          */
  void (* TxRxCpltCallback)(struct __SPI_HandleTypeDef *hspi);           /*!< SPI 发送与接收完成回调函数    */
  void (* TxHalfCpltCallback)(struct __SPI_HandleTypeDef *hspi);         /*!< SPI 发送过半完成回调函数     */
  void (* RxHalfCpltCallback)(struct __SPI_HandleTypeDef *hspi);         /*!< SPI 接收过半完成回调函数     */
  void (* TxRxHalfCpltCallback)(struct __SPI_HandleTypeDef *hspi);       /*!< SPI 发送与接收过半完成回调函数 */
  void (* ErrorCallback)(struct __SPI_HandleTypeDef *hspi);              /*!< SPI 错误回调函数                 */
  void (* AbortCpltCallback)(struct __SPI_HandleTypeDef *hspi);          /*!< SPI 中止回调函数                 */
  void (* MspInitCallback)(struct __SPI_HandleTypeDef *hspi);            /*!< SPI MSP 初始化回调函数          */
  void (* MspDeInitCallback)(struct __SPI_HandleTypeDef *hspi);          /*!< SPI MSP 反初始化回调函数        */

#endif  /* USE_HAL_SPI_REGISTER_CALLBACKS */
} SPI_HandleTypeDef;

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
/**
  * @brief  HAL SPI 回调函数 ID 枚举定义
  */
typedef enum
{
  HAL_SPI_TX_COMPLETE_CB_ID             = 0x00U,    /*!< SPI 发送完成回调函数 ID         */
  HAL_SPI_RX_COMPLETE_CB_ID             = 0x01U,    /*!< SPI 接收完成回调函数 ID         */
  HAL_SPI_TX_RX_COMPLETE_CB_ID          = 0x02U,    /*!< SPI 发送与接收完成回调函数 ID   */
  HAL_SPI_TX_HALF_COMPLETE_CB_ID        = 0x03U,    /*!< SPI 发送过半完成回调函数 ID    */
  HAL_SPI_RX_HALF_COMPLETE_CB_ID        = 0x04U,    /*!< SPI 接收过半回调函数 ID                */
  HAL_SPI_TX_RX_HALF_COMPLETE_CB_ID     = 0x05U,    /*!< SPI 发送/接收过半回调函数 ID            */
  HAL_SPI_ERROR_CB_ID                   = 0x06U,    /*!< SPI 错误回调函数 ID                    */
  HAL_SPI_ABORT_CB_ID                   = 0x07U,    /*!< SPI 中止回调函数 ID                    */
  HAL_SPI_MSPINIT_CB_ID                 = 0x08U,    /*!< SPI Msp 初始化回调函数 ID              */
  HAL_SPI_MSPDEINIT_CB_ID               = 0x09U     /*!< SPI Msp 反初始化回调函数 ID            */

} HAL_SPI_CallbackIDTypeDef;

/**
  * @brief  HAL SPI 回调函数指针定义
  */
typedef  void (*pSPI_CallbackTypeDef)(SPI_HandleTypeDef *hspi); /*!< 指向 SPI 回调函数的指针 */

#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
/**
  * @}
  */

/* 导出常量 ------------------------------------------------------------------*/
/** @defgroup SPI_Exported_Constants SPI 导出常量
  * @{
  */

/** @defgroup SPI_Error_Code SPI 错误代码
  * @{
  */
#define HAL_SPI_ERROR_NONE              (0x00000000U)   /*!< 无错误                                 */
#define HAL_SPI_ERROR_MODF              (0x00000001U)   /*!< 模式错误（MODF 错误）                   */
#define HAL_SPI_ERROR_CRC               (0x00000002U)   /*!< CRC 错误                               */
#define HAL_SPI_ERROR_OVR               (0x00000004U)   /*!< 过载错误 (OVR error)                   */
#define HAL_SPI_ERROR_DMA               (0x00000010U)   /*!< DMA 传输错误                           */
#define HAL_SPI_ERROR_FLAG              (0x00000020U)   /*!< RXNE/TXE/BSY 标志位错误                */
#define HAL_SPI_ERROR_ABORT             (0x00000040U)   /*!< SPI 中止过程错误                       */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
#define HAL_SPI_ERROR_INVALID_CALLBACK  (0x00000080U)   /*!< 无效回调函数错误                       */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup SPI_Mode SPI 模式
  * @{
  */
#define SPI_MODE_SLAVE                  (0x00000000U)
#define SPI_MODE_MASTER                 (SPI_CR1_MSTR | SPI_CR1_SSI)
/**
  * @}
  */

/** @defgroup SPI_Direction SPI 通信方向模式
  * @{
  */
#define SPI_DIRECTION_2LINES            (0x00000000U)
#define SPI_DIRECTION_2LINES_RXONLY     SPI_CR1_RXONLY
#define SPI_DIRECTION_1LINE             SPI_CR1_BIDIMODE
/**
  * @}
  */

/** @defgroup SPI_Data_Size SPI 数据大小
  * @{
  */
#define SPI_DATASIZE_8BIT               (0x00000000U)
#define SPI_DATASIZE_16BIT              SPI_CR1_DFF
/**
  * @}
  */

/** @defgroup SPI_Clock_Polarity SPI 时钟极性
  * @{
  */
#define SPI_POLARITY_LOW                (0x00000000U)
#define SPI_POLARITY_HIGH               SPI_CR1_CPOL
/**
  * @}
  */

/** @defgroup SPI_Clock_Phase SPI 时钟相位
  * @{
  */
#define SPI_PHASE_1EDGE                 (0x00000000U)
#define SPI_PHASE_2EDGE                 SPI_CR1_CPHA
/**
  * @}
  */

/** @defgroup SPI_Slave_Select_management SPI 从机选择管理
  * @{
  */
#define SPI_NSS_SOFT                    SPI_CR1_SSM
#define SPI_NSS_HARD_INPUT              (0x00000000U)
#define SPI_NSS_HARD_OUTPUT             (SPI_CR2_SSOE << 16U)
/**
  * @}
  */

/** @defgroup SPI_BaudRate_Prescaler SPI 波特率预分频器
  * @{
  */
#define SPI_BAUDRATEPRESCALER_2         (0x00000000U)
#define SPI_BAUDRATEPRESCALER_4         (SPI_CR1_BR_0)
#define SPI_BAUDRATEPRESCALER_8         (SPI_CR1_BR_1)
#define SPI_BAUDRATEPRESCALER_16        (SPI_CR1_BR_1 | SPI_CR1_BR_0)
#define SPI_BAUDRATEPRESCALER_32        (SPI_CR1_BR_2)
#define SPI_BAUDRATEPRESCALER_64        (SPI_CR1_BR_2 | SPI_CR1_BR_0)
#define SPI_BAUDRATEPRESCALER_128       (SPI_CR1_BR_2 | SPI_CR1_BR_1)
#define SPI_BAUDRATEPRESCALER_256       (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)
/**
  * @}
  */

/** @defgroup SPI_MSB_LSB_transmission SPI 高位/低位优先传输
  * @{
  */
#define SPI_FIRSTBIT_MSB                (0x00000000U)
#define SPI_FIRSTBIT_LSB                SPI_CR1_LSBFIRST
/**
  * @}
  */

/** @defgroup SPI_TI_mode SPI TI 模式
  * @{
  */
#define SPI_TIMODE_DISABLE              (0x00000000U)
/**
  * @}
  */

/** @defgroup SPI_CRC_Calculation SPI CRC 校验计算
  * @{
  */
#define SPI_CRCCALCULATION_DISABLE      (0x00000000U)
#define SPI_CRCCALCULATION_ENABLE       SPI_CR1_CRCEN
/**
  * @}
  */

/** @defgroup SPI_Interrupt_definition SPI 中断定义
  * @{
  */
#define SPI_IT_TXE                      SPI_CR2_TXEIE
#define SPI_IT_RXNE                     SPI_CR2_RXNEIE
#define SPI_IT_ERR                      SPI_CR2_ERRIE
/**
  * @}
  */

/** @defgroup SPI_Flags_definition SPI 标志定义
  * @{
  */
#define SPI_FLAG_RXNE                   SPI_SR_RXNE   /* SPI 状态标志：接收缓冲区非空标志                  */
#define SPI_FLAG_TXE                    SPI_SR_TXE    /* SPI 状态标志：发送缓冲区为空标志                  */
#define SPI_FLAG_BSY                    SPI_SR_BSY    /* SPI 状态标志：忙标志                              */
#define SPI_FLAG_CRCERR                 SPI_SR_CRCERR /* SPI 错误标志：CRC 校验错误标志                    */
#define SPI_FLAG_MODF                   SPI_SR_MODF   /* SPI 错误标志：模式故障标志                        */
#define SPI_FLAG_OVR                    SPI_SR_OVR    /* SPI 错误标志：溢出错误标志                        */
#define SPI_FLAG_MASK                   (SPI_SR_RXNE | SPI_SR_TXE | SPI_SR_BSY\
                                         | SPI_SR_CRCERR | SPI_SR_MODF | SPI_SR_OVR)
/**
  * @}
  */

/**
  * @}
  */

/* 导出宏 --------------------------------------------------------------------*/
/** @defgroup SPI_Exported_Macros SPI 导出宏
  * @{
  */

/** @brief  复位 SPI 句柄状态。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPI，其中 x 可以是 1、2 或 3，用于选择 SPI 外设。
  * @retval 无
  */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
#define __HAL_SPI_RESET_HANDLE_STATE(__HANDLE__)      \
  do{                                                 \
    (__HANDLE__)->State = HAL_SPI_STATE_RESET;        \
    (__HANDLE__)->MspInitCallback = NULL;             \
    (__HANDLE__)->MspDeInitCallback = NULL;           \
  } while(0)
#else
#define __HAL_SPI_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_SPI_STATE_RESET)
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

/** @brief  使能指定的 SPI 中断。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPI，其中 x 可以是 1、2 或 3，用于选择 SPI 外设。
  * @param  __INTERRUPT__ 指定要使能的中断源。
  *         该参数可以是以下值之一：
  *            @arg SPI_IT_TXE: 发送缓冲区空中断使能
  *            @arg SPI_IT_RXNE: 接收缓冲区非空中断使能
  *            @arg SPI_IT_ERR: 错误中断使能
  * @retval 无
  */
#define __HAL_SPI_ENABLE_IT(__HANDLE__, __INTERRUPT__)   SET_BIT((__HANDLE__)->Instance->CR2, (__INTERRUPT__))

/** @brief  禁用指定的 SPI 中断。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPIx，其中 x 可以是 1、2 或 3，用于选择 SPI 外设。
  * @param  __INTERRUPT__ 指定要禁用的中断源。
  *         该参数可以是以下值之一：
  *            @arg SPI_IT_TXE: 发送缓冲区空中断使能
  *            @arg SPI_IT_RXNE: 接收缓冲区非空中断使能
  *            @arg SPI_IT_ERR: 错误中断使能
  * @retval 无
  */
#define __HAL_SPI_DISABLE_IT(__HANDLE__, __INTERRUPT__)  CLEAR_BIT((__HANDLE__)->Instance->CR2, (__INTERRUPT__))

/** @brief  检查指定的 SPI 中断源是否已使能。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPIx，其中 x 可为 1、2 或 3，用以选择 SPI 外设。
  * @param  __INTERRUPT__ 指定待检查的 SPI 中断源。
  *          该参数可以是以下值之一：
  *            @arg SPI_IT_TXE: 发送缓冲区空中断使能
  *            @arg SPI_IT_RXNE: 接收缓冲区非空中断使能
  *            @arg SPI_IT_ERR: 错误中断使能
  * @retval __IT__ 的新状态（TRUE 或 FALSE）。
  */
#define __HAL_SPI_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) ((((__HANDLE__)->Instance->CR2\
                                                              & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)

/** @brief  检查指定的 SPI 标志位是否已置位。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPIx，其中 x 可为 1、2 或 3，用以选择 SPI 外设。
  * @param  __FLAG__ 指定待检查的标志位。
  *         该参数可以是以下值之一：
  *            @arg SPI_FLAG_RXNE: 接收缓冲区非空标志
  *            @arg SPI_FLAG_TXE: 发送缓冲区空标志
  *            @arg SPI_FLAG_CRCERR: CRC 校验错误标志
  *            @arg SPI_FLAG_MODF: 模式错误标志
  *            @arg SPI_FLAG_OVR: 过载/溢出错误标志
  *            @arg SPI_FLAG_BSY: 忙标志
  * @retval __FLAG__ 的新状态（TRUE 或 FALSE）。
  */
#define __HAL_SPI_GET_FLAG(__HANDLE__, __FLAG__) ((((__HANDLE__)->Instance->SR) & (__FLAG__)) == (__FLAG__))

/** @brief  清除 SPI CRCERR 挂起标志。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPIx，其中 x 可为 1、2 或 3，用以选择 SPI 外设。
  * @retval 无
  */
#define __HAL_SPI_CLEAR_CRCERRFLAG(__HANDLE__) ((__HANDLE__)->Instance->SR = (uint16_t)(~SPI_FLAG_CRCERR))

/** @brief  清除 SPI MODF 挂起标志。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPIx，其中 x 可为 1、2 或 3，用以选择 SPI 外设。
  * @retval 无
  */
#define __HAL_SPI_CLEAR_MODFFLAG(__HANDLE__)             \
  do{                                                    \
    __IO uint32_t tmpreg_modf = 0x00U;                   \
    tmpreg_modf = (__HANDLE__)->Instance->SR;            \
    CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_SPE); \
    UNUSED(tmpreg_modf);                                 \
  } while(0U)

/** @brief  清除 SPI OVR 挂起标志。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPIx，其中 x 可为 1、2 或 3，用以选择 SPI 外设。
  * @retval 无
  */
#define __HAL_SPI_CLEAR_OVRFLAG(__HANDLE__)        \
  do{                                              \
    __IO uint32_t tmpreg_ovr = 0x00U;              \
    tmpreg_ovr = (__HANDLE__)->Instance->DR;       \
    tmpreg_ovr = (__HANDLE__)->Instance->SR;       \
    UNUSED(tmpreg_ovr);                            \
  } while(0U)

/** @brief  使能 SPI 外设。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPIx，其中 x 可为 1、2 或 3，用以选择 SPI 外设。
  * @retval 无
  */
#define __HAL_SPI_ENABLE(__HANDLE__)  SET_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_SPE)

/** @brief  禁用 SPI 外设。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPIx，其中 x 可为 1、2 或 3，用以选择 SPI 外设。
  * @retval 无
  */
#define __HAL_SPI_DISABLE(__HANDLE__) CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_SPE)

/**
  * @}
  */

/* 私有常量 ------------------------------------------------------------------*/
/** @defgroup SPI_Private_Constants SPI 私有常量
  * @{
  */
#define SPI_INVALID_CRC_ERROR     0U          /* 误检测到 CRC 错误 */
#define SPI_VALID_CRC_ERROR       1U          /* CRC 错误确实发生 */
/**
  * @}
  */

/* 私有宏 --------------------------------------------------------------------*/
/** @defgroup SPI_Private_Macros SPI 私有宏
  * @{
  */

/** @brief  设置 SPI 为仅发送模式。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPIx，其中 x 可取 1、2 或 3 以选择 SPI 外设。
  * @retval 无
  */
#define SPI_1LINE_TX(__HANDLE__)  SET_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_BIDIOE)

/** @brief  设置 SPI 为仅接收模式。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPIx，其中 x 可取 1、2 或 3 以选择 SPI 外设。
  * @retval 无
  */
#define SPI_1LINE_RX(__HANDLE__)  CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_BIDIOE)

/** @brief  复位 SPI 的 CRC 计算。
  * @param  __HANDLE__ 指定 SPI 句柄。
  *         该参数可以是 SPIx，其中 x 可取 1、2 或 3 以选择 SPI 外设。
  * @retval 无
  */
#define SPI_RESET_CRC(__HANDLE__)                           \
  do{                                                       \
    CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_CRCEN);  \
    SET_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_CRCEN);    \
  } while(0U)

/** @brief  检查指定的 SPI 标志位是否置位。
  * @param  __SR__  SPI SR 寄存器的副本。
  * @param  __FLAG__ 指定要检查的标志位。
  *         该参数可以是以下值之一：
  *            @arg SPI_FLAG_RXNE: 接收缓冲区非空标志
  *            @arg SPI_FLAG_TXE: 发送缓冲区为空标志
  *            @arg SPI_FLAG_CRCERR: CRC 错误标志
  *            @arg SPI_FLAG_MODF: 模式错误标志
  *            @arg SPI_FLAG_OVR: 过载/溢出标志
  *            @arg SPI_FLAG_BSY: 忙标志
  * @retval SET 或 RESET。
  */
#define SPI_CHECK_FLAG(__SR__, __FLAG__) ((((__SR__) & ((__FLAG__) & SPI_FLAG_MASK)) == \
                                           ((__FLAG__) & SPI_FLAG_MASK)) ? SET : RESET)

/** @brief  检查指定的 SPI 中断是否置位（使能）。
  * @param  __CR2__  SPI CR2 寄存器的副本。
  * @param  __INTERRUPT__ 指定要检查的 SPI 中断源。
  *         该参数可以是以下值之一：
  *            @arg SPI_IT_TXE: 发送缓冲区空中断使能
  *            @arg SPI_IT_RXNE: 接收缓冲区非空中断使能
  *            @arg SPI_IT_ERR: 错误中断使能
  * @retval SET 或 RESET。
  */
#define SPI_CHECK_IT_SOURCE(__CR2__, __INTERRUPT__) ((((__CR2__) & (__INTERRUPT__)) == \
                                                      (__INTERRUPT__)) ? SET : RESET)

/** @brief  检查 SPI 工作模式参数是否在允许的范围内。
  * @param  __MODE__ 指定 SPI 工作模式。
  *         该参数可以是 @ref SPI_Mode 的一个值
  * @retval 无
  */
#define IS_SPI_MODE(__MODE__)      (((__MODE__) == SPI_MODE_SLAVE)   || \
                                    ((__MODE__) == SPI_MODE_MASTER))

/** @brief  检查 SPI 通信方向模式参数是否在允许的范围内。
  * @param  __MODE__ 指定 SPI 通信方向模式。
  *         该参数可以是 @ref SPI_Direction 的一个值
  * @retval 无
  */
#define IS_SPI_DIRECTION(__MODE__) (((__MODE__) == SPI_DIRECTION_2LINES)        || \
                                    ((__MODE__) == SPI_DIRECTION_2LINES_RXONLY) || \
                                    ((__MODE__) == SPI_DIRECTION_1LINE))

/** @brief  检查 SPI 通信方向模式参数是否为双线模式。
  * @param  __MODE__ 指定 SPI 通信方向模式。
  * @retval 无
  */
#define IS_SPI_DIRECTION_2LINES(__MODE__) ((__MODE__) == SPI_DIRECTION_2LINES)

/** @brief  检查 SPI 通信方向模式参数是否为单线或双线模式。
  * @param  __MODE__ 指定 SPI 通信方向模式。
  * @retval 无
  */
#define IS_SPI_DIRECTION_2LINES_OR_1LINE(__MODE__) (((__MODE__) == SPI_DIRECTION_2LINES) || \
                                                    ((__MODE__) == SPI_DIRECTION_1LINE))

/** @brief  检查 SPI 数据帧大小参数是否在允许的范围内。
  * @param  __DATASIZE__ 指定 SPI 数据帧大小。
  *         该参数可以是 @ref SPI_Data_Size 的一个值
  * @retval 无
  */
#define IS_SPI_DATASIZE(__DATASIZE__) (((__DATASIZE__) == SPI_DATASIZE_16BIT) || \
                                       ((__DATASIZE__) == SPI_DATASIZE_8BIT))

/** @brief  检查 SPI 串行时钟空闲状态（极性）参数是否在允许的范围内。
  * @param  __CPOL__ 指定 SPI 串行时钟空闲状态。
  *         该参数可以是 @ref SPI_Clock_Polarity 的一个值
  * @retval 无
  */
#define IS_SPI_CPOL(__CPOL__)      (((__CPOL__) == SPI_POLARITY_LOW) || \
                                    ((__CPOL__) == SPI_POLARITY_HIGH))

/** @brief  检查 SPI 时钟相位参数是否在允许的范围内。
  * @param  __CPHA__ 指定 SPI 时钟相位。
  *         该参数可以是 @ref SPI_Clock_Phase 的一个值
  * @retval 无
  */
#define IS_SPI_CPHA(__CPHA__)      (((__CPHA__) == SPI_PHASE_1EDGE) || \
                                    ((__CPHA__) == SPI_PHASE_2EDGE))

/** @brief  检查 SPI 片选信号 (NSS) 参数是否在允许的范围内。
  * @param  __NSS__ 指定 SPI 片选管理参数。
  *         该参数可以是 @ref SPI_Slave_Select_management 的一个值
  * @retval 无
  */
#define IS_SPI_NSS(__NSS__)        (((__NSS__) == SPI_NSS_SOFT)       || \
                                    ((__NSS__) == SPI_NSS_HARD_INPUT) || \
                                    ((__NSS__) == SPI_NSS_HARD_OUTPUT))

/** @brief  检查 SPI 波特率预分频器参数是否在允许的范围内。
  * @param  __PRESCALER__ 指定 SPI 波特率预分频器。
  *         该参数可以是 @ref SPI_BaudRate_Prescaler 的一个值
  * @retval 无
  */
#define IS_SPI_BAUDRATE_PRESCALER(__PRESCALER__) (((__PRESCALER__) == SPI_BAUDRATEPRESCALER_2)   || \
                                                  ((__PRESCALER__) == SPI_BAUDRATEPRESCALER_4)   || \
                                                  ((__PRESCALER__) == SPI_BAUDRATEPRESCALER_8)   || \
                                                  ((__PRESCALER__) == SPI_BAUDRATEPRESCALER_16)  || \
                                                  ((__PRESCALER__) == SPI_BAUDRATEPRESCALER_32)  || \
                                                  ((__PRESCALER__) == SPI_BAUDRATEPRESCALER_64)  || \
                                                  ((__PRESCALER__) == SPI_BAUDRATEPRESCALER_128) || \
                                                  ((__PRESCALER__) == SPI_BAUDRATEPRESCALER_256))

/** @brief  检查 SPI MSB/LSB 传输顺序参数是否在允许的范围内。
  * @param  __BIT__ 指定 SPI MSB/LSB 传输顺序（数据传输是从最高有效位 MSB 还是最低有效位 LSB 开始）。
  *         该参数可以是 @ref SPI_MSB_LSB_transmission 的一个值
  * @retval 无
  */
#define IS_SPI_FIRST_BIT(__BIT__)  (((__BIT__) == SPI_FIRSTBIT_MSB) || \
                                    ((__BIT__) == SPI_FIRSTBIT_LSB))

/** @brief  检查 SPI TI 模式参数是否已禁用。
  * @param  __MODE__ SPI_TIMODE_DISABLE。器件不支持 TI 模式。
  *         该参数可以是 @ref SPI_TI_mode 的取值之一
  * @retval 无
  */
#define IS_SPI_TIMODE(__MODE__) ((__MODE__) == SPI_TIMODE_DISABLE)

/** @brief  检查 SPI CRC 计算使能状态是否在允许的范围内。
  * @param  __CALCULATION__ 指定 SPI CRC 计算使能状态。
  *         该参数可以是 @ref SPI_CRC_Calculation 的取值之一
  * @retval 无
  */
#define IS_SPI_CRC_CALCULATION(__CALCULATION__) (((__CALCULATION__) == SPI_CRCCALCULATION_DISABLE) || \
                                                 ((__CALCULATION__) == SPI_CRCCALCULATION_ENABLE))

/** @brief  检查用于 CRC 计算的 SPI 多项式值是否在允许的范围内。
  * @param  __POLYNOMIAL__ 指定用于 CRC 计算的 SPI 多项式值。
  *         该参数必须是介于 Min_Data = 0 和 Max_Data = 65535 之间的数值
  * @retval 无
  */
#define IS_SPI_CRC_POLYNOMIAL(__POLYNOMIAL__) (((__POLYNOMIAL__) >= 0x1U)    && \
                                               ((__POLYNOMIAL__) <= 0xFFFFU) && \
                                               (((__POLYNOMIAL__)&0x1U) != 0U))

/** @brief  检查 DMA 句柄是否有效。
  * @param  __HANDLE__ 指定 DMA 句柄。
  * @retval 无
  */
#define IS_SPI_DMA_HANDLE(__HANDLE__) ((__HANDLE__) != NULL)

/**
  * @}
  */

/* 私有函数 -----------------------------------------------------------------*/
/** @defgroup SPI_Private_Functions SPI 私有函数
  * @{
  */
uint8_t SPI_ISCRCErrorValid(SPI_HandleTypeDef *hspi);
/**
  * @}
  */

/* 导出函数 -----------------------------------------------------------------*/
/** @addtogroup SPI_Exported_Functions
  * @{
  */

/** @addtogroup SPI_Exported_Functions_Group1
  * @{
  */
/* 初始化/反初始化函数  *******************************************************/
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi);

/* 回调函数注册/注销函数  *****************************************************/
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
HAL_StatusTypeDef HAL_SPI_RegisterCallback(SPI_HandleTypeDef *hspi, HAL_SPI_CallbackIDTypeDef CallbackID,
                                           pSPI_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_SPI_UnRegisterCallback(SPI_HandleTypeDef *hspi, HAL_SPI_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @addtogroup SPI_Exported_Functions_Group2
  * @{
  */
/* I/O 操作函数  ***************************************************/
HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData,
                                          uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData,
                                             uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData,
                                              uint16_t Size);
HAL_StatusTypeDef HAL_SPI_DMAPause(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_DMAResume(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_DMAStop(SPI_HandleTypeDef *hspi);
/* 传输中止函数 */
HAL_StatusTypeDef HAL_SPI_Abort(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_Abort_IT(SPI_HandleTypeDef *hspi);

void HAL_SPI_IRQHandler(SPI_HandleTypeDef *hspi);
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_AbortCpltCallback(SPI_HandleTypeDef *hspi);
/**
  * @}
  */

/** @addtogroup SPI_Exported_Functions_Group3
  * @{
  */
/* 外设状态与错误处理函数 ***************************************/
HAL_SPI_StateTypeDef HAL_SPI_GetState(const SPI_HandleTypeDef *hspi);
uint32_t             HAL_SPI_GetError(const SPI_HandleTypeDef *hspi);
/**
  * @}
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

#endif /* STM32F1xx_HAL_SPI_H */


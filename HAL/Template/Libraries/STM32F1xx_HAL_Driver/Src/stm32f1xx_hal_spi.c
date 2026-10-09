/**
  ******************************************************************************
  * @file    stm32f1xx_hal_spi.c
  * @author  MCD Application Team
  * @brief   SPI HAL 模块驱动程序。
  *          本文件提供固件函数以管理串行外设接口 (SPI) 外设的以下功能：
  *           + 初始化与反初始化函数
  *           + I/O 操作函数
  *           + 外设控制函数
  *           + 外设状态函数
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
      SPI HAL 驱动程序的使用步骤如下：

      (#) 声明一个 SPI_HandleTypeDef 句柄结构体，例如：
          SPI_HandleTypeDef  hspi;

      (#) 通过实现 HAL_SPI_MspInit() API 初始化 SPI 底层硬件资源：
          (##) 使能 SPIx 接口时钟
          (##) SPI 引脚配置
              (+++) 使能 SPI GPIO 时钟
              (+++) 将这些 SPI 引脚配置为复用推挽输出
          (##) 若需使用中断处理，则进行 NVIC 配置
              (+++) 配置 SPIx 中断优先级
              (+++) 使能 NVIC SPI 中断处理程序
          (##) 若需使用 DMA 处理，则进行 DMA 配置
              (+++) 为发送或接收数据流/通道声明一个 DMA_HandleTypeDef 句柄结构体
              (+++) 使能 DMAx 时钟
              (+++) 配置 DMA 句柄参数
              (+++) 配置 DMA 发送或接收数据流/通道
              (+++) 将已初始化的 hdma_tx（或 _rx）句柄与 hspi 的 DMA Tx 或 Rx 句柄进行关联
              (+++) 配置 DMA 发送或接收数据流/通道的传输完成中断优先级，并使能 NVIC 中断

      (#) 在 hspi 的 Init 结构体中配置主从模式（Mode）、双向模式（BidirectionalMode）、数据位宽（Data size）、
          波特率预分频系数（Baudrate Prescaler）、NSS 信号管理、时钟极性和相位（Clock polarity and phase）、
          高低位优先（FirstBit）以及 CRC 配置。

      (#) 调用 HAL_SPI_Init() API 初始化 SPI 寄存器：
          (++) 该 API 还会通过调用用户自定义的 HAL_SPI_MspInit() API
              来配置底层硬件（GPIO、时钟、CORTEX 等）。
     [..]
       循环模式限制：
      (#) 当 SPI 配置为以下模式时，无法使用 DMA 循环模式：
          (##) 主机双线仅接收（Master 2Lines RxOnly）
          (##) 主机单线接收（Master 1Line Rx）
      (#) 当使能 DMA 循环模式时，不支持 CRC 功能
      (#) 使用 SPI DMA 暂停/停止功能时，必须仅在 SPI 回调函数中
          调用 HAL_SPI_DMAPause() / HAL_SPI_DMAStop() API
     [..]
       主机接收模式限制：
      (#) 在主机单向仅接收模式（MSTR=1, BIDIMODE=0, RXONLY=1）或
          双向接收模式（MSTR=1, BIDIMODE=1, BIDIOE=0）下，为确保 SPI
          不会启动新的传输，必须遵循以下步骤：
          (##) HAL_SPI_DeInit()
          (##) HAL_SPI_Init()
     [..]
       回调函数注册：

      (#) 当编译宏 USE_HAL_SPI_REGISTER_CALLBACKS 设置为 1U 时，
          允许用户动态配置驱动程序回调函数。
          使用函数 HAL_SPI_RegisterCallback() 注册中断回调函数。

          函数 HAL_SPI_RegisterCallback() 允许注册以下回调函数：
            (++) TxCpltCallback        : SPI 发送完成回调函数
            (++) RxCpltCallback        : SPI 接收完成回调函数
            (++) TxRxCpltCallback      : SPI 收发完成回调函数
            (++) TxHalfCpltCallback    : SPI 发送过半回调函数
            (++) RxHalfCpltCallback    : SPI 接收过半回调函数
            (++) TxRxHalfCpltCallback  : SPI 收发过半回调函数
            (++) ErrorCallback         : SPI 错误回调函数
            (++) AbortCpltCallback     : SPI 中止回调函数
            (++) MspInitCallback       : SPI Msp 初始化回调函数
            (++) MspDeInitCallback     : SPI Msp 反初始化回调函数
          该函数接收 HAL 外设句柄、回调函数 ID 以及指向用户回调函数的指针作为参数。

      (#) 使用函数 HAL_SPI_UnRegisterCallback 将回调函数重置为默认弱定义函数。
          HAL_SPI_UnRegisterCallback 接收 HAL 外设句柄和回调函数 ID 作为参数。
          该函数允许重置以下回调函数：
            (++) TxCpltCallback        : SPI 发送完成回调函数
            (++) RxCpltCallback        : SPI 接收完成回调函数
            (++) TxRxCpltCallback      : SPI 收发完成回调函数
            (++) TxHalfCpltCallback    : SPI 发送过半回调函数
            (++) RxHalfCpltCallback    : SPI 接收过半回调函数
            (++) TxRxHalfCpltCallback  : SPI 收发过半回调函数
            (++) ErrorCallback         : SPI 错误回调函数
            (++) AbortCpltCallback     : SPI 中止回调函数
            (++) MspInitCallback       : SPI Msp 初始化回调函数
            (++) MspDeInitCallback     : SPI Msp 反初始化回调函数

       [..]
       默认情况下，在执行 HAL_SPI_Init() 且状态为 HAL_SPI_STATE_RESET 时，
       所有回调函数均被设置为对应的弱定义函数：
       例如 HAL_SPI_MasterTxCpltCallback()、HAL_SPI_MasterRxCpltCallback()。
       MspInit 和 MspDeInit 函数例外，仅当这些回调函数为空（事先未注册）时，
       才会在 HAL_SPI_Init()/HAL_SPI_DeInit() 中被重置为原有的弱定义函数。
       如果 MspInit 或 MspDeInit 不为空，无论处于何种状态，
       HAL_SPI_Init()/HAL_SPI_DeInit() 都会保留并使用用户（事先注册的）MspInit/MspDeInit 回调函数。

       [..]
       仅在 HAL_SPI_STATE_READY 状态下才允许注册/注销回调函数。
       MspInit/MspDeInit 函数例外，可以在 HAL_SPI_STATE_READY 或 HAL_SPI_STATE_RESET 状态下注册/注销，
       因此在 Init/DeInit 期间可以使用已注册的（用户）MspInit/DeInit 回调函数。
       即用户应在调用 HAL_SPI_DeInit() 或 HAL_SPI_Init() 函数之前，
       先使用 HAL_SPI_RegisterCallback() 注册 MspInit/MspDeInit 用户回调函数。

       [..]
       当编译宏 USE_HAL_PPP_REGISTER_CALLBACKS 设置为 0 或未定义时，
       回调函数注册功能不可用，驱动将使用弱定义（重载）回调函数。

     [..]
       使用 HAL 时，无法在所有不同的 SPI 模式下均达到所支持的最大 SPI 频率，
       下表根据 SPI 实例所使用的 APBx 外设时钟频率 (fPCLK)，
       汇总了在 8 位/16 位数据宽度下所能达到的最大 SPI 频率。

  @endverbatim

  附加表格：

       DataSize = SPI_DATASIZE_8BIT:
       +----------------------------------------------------------------------------------------------+
       |         |                | 2Lines Fullduplex   |     2Lines RxOnly    |         1Line        |
       | Process | Transfer mode  |---------------------|----------------------|----------------------|
       |         |                |  Master  |  Slave   |  Master   |  Slave   |  Master   |  Slave   |
       |==============================================================================================|
       |    T    |     Polling    | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |    /    |     Interrupt  | Fpclk/4  | Fpclk/8  |    NA     |    NA    |    NA     |   NA     |
       |    R    |----------------|----------|----------|-----------|----------|-----------|----------|
       |    X    |       DMA      | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |
       |=========|================|==========|==========|===========|==========|===========|==========|
       |         |     Polling    | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/64  | Fpclk/2  |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    R    |     Interrupt  | Fpclk/8  | Fpclk/8  | Fpclk/64  | Fpclk/2  | Fpclk/64  | Fpclk/2  |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/128 | Fpclk/2  |
       |=========|================|==========|==========|===========|==========|===========|==========|
       |         |     Polling    | Fpclk/2  | Fpclk/4  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    T    |     Interrupt  | Fpclk/2  | Fpclk/4  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/128|
       +----------------------------------------------------------------------------------------------+

       DataSize = SPI_DATASIZE_16BIT:
       +----------------------------------------------------------------------------------------------+
       |         |                | 2Lines Fullduplex   |     2Lines RxOnly    |         1Line        |
       | Process | Transfer mode  |---------------------|----------------------|----------------------|
       |         |                |  Master  |  Slave   |  Master   |  Slave   |  Master   |  Slave   |
       |==============================================================================================|
       |    T    |     Polling    | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |    /    |     Interrupt  | Fpclk/4  | Fpclk/4  |    NA     |    NA    |    NA     |   NA     |
       |    R    |----------------|----------|----------|-----------|----------|-----------|----------|
       |    X    |       DMA      | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |
       |=========|================|==========|==========|===========|==========|===========|==========|
       |         |     Polling    | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/32  | Fpclk/2  |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    R    |     Interrupt  | Fpclk/4  | Fpclk/4  | Fpclk/64  | Fpclk/2  | Fpclk/64  | Fpclk/2  |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/128 | Fpclk/2  |
       |=========|================|==========|==========|===========|==========|===========|==========|
       |         |     Polling    | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/32 |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    T    |     Interrupt  | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/128|
       +----------------------------------------------------------------------------------------------+
       @note SPI 最大频率取决于 SPI 数据位宽（8 位、16 位）、
             SPI 模式（双线全双工、双线仅接收、单线收发）以及处理模式（轮询、中断、DMA）。
       @note
            (#) TX/RX 过程对应 HAL_SPI_TransmitReceive()、HAL_SPI_TransmitReceive_IT() 和
                HAL_SPI_TransmitReceive_DMA()
            (#) RX 过程对应 HAL_SPI_Receive()、HAL_SPI_Receive_IT() 和 HAL_SPI_Receive_DMA()
            (#) TX 过程对应 HAL_SPI_Transmit()、HAL_SPI_Transmit_IT() 和 HAL_SPI_Transmit_DMA()

  */

/* 包含文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup SPI SPI
  * @brief SPI HAL 模块驱动
  * @{
  */
#ifdef HAL_SPI_MODULE_ENABLED

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#if (USE_SPI_CRC != 0U) && defined(SPI_CRC_ERROR_WORKAROUND_FEATURE)
/* CRC 规避措施特性：该变量用于判断器件是否受 SPI2 错误 CRC 错误检测规避措施实现的影响。
 * 应用该规避措施的条件为：
 *  - STM32F101CDE/STM32F103CDE
 *  - 修订版本 ID（Revision ID）：Z
 *  - SPI2
 *  - 在仅接收模式且使能 CRC 计算的情况下，在 CRC 接收结束时，
 *     软件需要检查 CRCERR 标志。若置位，则读回 SPI_RXCRC：
 *       + 若值为 0，则完整数据传输成功。
 *       + 否则，CPU 或 DMA 在数据传输过程中检测到了一个或多个错误。
 *    若 CRCERR 复位，则认为完整数据传输成功。
 *
 * 检查 RevisionID 值以确认器件是否为 Rev Z (0x0001)，以便使能针对错误检测
 * CRC 错误的规避措施
 */
/* 问题在于 ES_STM32F10xxCDE 还指出了非调试模式下访问调试寄存器的问题
 * 修订版本 ID 信息仅在调试模式下可用，因此无法实现规避措施
 * 来区分 Rev Z 器件（存在问题）与更新版本（已修复问题）。
 * 因此，对于 Revision Z 的 F101 或 F103 器件，以下宏定义应赋值为 1。
 */
#define  USE_SPI_CRC_ERROR_WORKAROUND   0U
#endif /* USE_SPI_CRC */
/** @defgroup SPI_Private_Constants SPI 私有常量
  * @{
  */
#define SPI_DEFAULT_TIMEOUT 100U
/**
  * @}
  */

/* 私有宏 --------------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 私有函数原型 --------------------------------------------------------------*/
/** @defgroup SPI_Private_Functions SPI 私有函数
  * @{
  */
static void SPI_DMATransmitCplt(DMA_HandleTypeDef *hdma);
static void SPI_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
static void SPI_DMATransmitReceiveCplt(DMA_HandleTypeDef *hdma);
static void SPI_DMAHalfTransmitCplt(DMA_HandleTypeDef *hdma);
static void SPI_DMAHalfReceiveCplt(DMA_HandleTypeDef *hdma);
static void SPI_DMAHalfTransmitReceiveCplt(DMA_HandleTypeDef *hdma);
static void SPI_DMAError(DMA_HandleTypeDef *hdma);
static void SPI_DMAAbortOnError(DMA_HandleTypeDef *hdma);
static void SPI_DMATxAbortCallback(DMA_HandleTypeDef *hdma);
static void SPI_DMARxAbortCallback(DMA_HandleTypeDef *hdma);
static HAL_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus State,
                                                       uint32_t Timeout, uint32_t Tickstart);
static void SPI_TxISR_8BIT(struct __SPI_HandleTypeDef *hspi);
static void SPI_TxISR_16BIT(struct __SPI_HandleTypeDef *hspi);
static void SPI_RxISR_8BIT(struct __SPI_HandleTypeDef *hspi);
static void SPI_RxISR_16BIT(struct __SPI_HandleTypeDef *hspi);
static void SPI_2linesRxISR_8BIT(struct __SPI_HandleTypeDef *hspi);
static void SPI_2linesTxISR_8BIT(struct __SPI_HandleTypeDef *hspi);
static void SPI_2linesTxISR_16BIT(struct __SPI_HandleTypeDef *hspi);
static void SPI_2linesRxISR_16BIT(struct __SPI_HandleTypeDef *hspi);
#if (USE_SPI_CRC != 0U)
static void SPI_RxISR_8BITCRC(struct __SPI_HandleTypeDef *hspi);
static void SPI_RxISR_16BITCRC(struct __SPI_HandleTypeDef *hspi);
static void SPI_2linesRxISR_8BITCRC(struct __SPI_HandleTypeDef *hspi);
static void SPI_2linesRxISR_16BITCRC(struct __SPI_HandleTypeDef *hspi);
#endif /* USE_SPI_CRC */
static void SPI_AbortRx_ISR(SPI_HandleTypeDef *hspi);
static void SPI_AbortTx_ISR(SPI_HandleTypeDef *hspi);
static void SPI_CloseRxTx_ISR(SPI_HandleTypeDef *hspi);
static void SPI_CloseRx_ISR(SPI_HandleTypeDef *hspi);
static void SPI_CloseTx_ISR(SPI_HandleTypeDef *hspi);
static HAL_StatusTypeDef SPI_EndRxTransaction(SPI_HandleTypeDef *hspi, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef SPI_EndRxTxTransaction(SPI_HandleTypeDef *hspi, uint32_t Timeout, uint32_t Tickstart);
/**
  * @}
  */

/* 导出函数 --------------------------------------------------------*/
/** @defgroup SPI_Exported_Functions SPI 导出函数
  * @{
  */

/** @defgroup SPI_Exported_Functions_Group1 初始化和反初始化函数
  *  @brief    初始化与配置函数
  *
@verbatim
 ===============================================================================
              ##### 初始化和反初始化函数 #####
 ===============================================================================
    [..]  本节提供了一组用于初始化和反初始化 SPIx 外设的函数：

      (+) 用户必须实现 HAL_SPI_MspInit() 函数，并在其中配置所有相关的外设资源
          （时钟 CLOCK、GPIO、DMA、中断 IT 以及 NVIC）。

      (+) 调用 HAL_SPI_Init() 函数以按照所选配置对指定设备进行配置：
        (++) 工作模式（Mode）
        (++) 传输方向（Direction）
        (++) 数据大小（Data Size）
        (++) 时钟极性和相位（Clock Polarity and Phase）
        (++) NSS 硬件/软件管理（NSS Management）
        (++) 波特率预分频系数（BaudRate Prescaler）
        (++) 高/低位先行（FirstBit）
        (++) TI 模式（TIMode）
        (++) CRC 校验计算（CRC Calculation）
        (++) 使能 CRC 时的 CRC 多项式（CRC Polynomial if CRC enabled）

      (+) 调用 HAL_SPI_DeInit() 函数以将所选 SPIx 外设恢复为默认配置。

@endverbatim
  * @{
  */

/**
  * @brief  根据 SPI_InitTypeDef 中指定的参数初始化 SPI，
  *         并初始化关联的句柄。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)
{
  /* 检查 SPI 句柄分配有效性 */
  if (hspi == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数有效性 */
  assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));
  assert_param(IS_SPI_MODE(hspi->Init.Mode));
  assert_param(IS_SPI_DIRECTION(hspi->Init.Direction));
  assert_param(IS_SPI_DATASIZE(hspi->Init.DataSize));
  assert_param(IS_SPI_NSS(hspi->Init.NSS));
  assert_param(IS_SPI_BAUDRATE_PRESCALER(hspi->Init.BaudRatePrescaler));
  assert_param(IS_SPI_FIRST_BIT(hspi->Init.FirstBit));
  /* 该器件不支持 TI 模式。
     TIMode 参数必须强制设置为 SPI_TIMODE_DISABLE */
  assert_param(IS_SPI_TIMODE(hspi->Init.TIMode));
  if (hspi->Init.TIMode == SPI_TIMODE_DISABLE)
  {
    assert_param(IS_SPI_CPOL(hspi->Init.CLKPolarity));
    assert_param(IS_SPI_CPHA(hspi->Init.CLKPhase));

    if (hspi->Init.Mode == SPI_MODE_MASTER)
    {
      assert_param(IS_SPI_BAUDRATE_PRESCALER(hspi->Init.BaudRatePrescaler));
    }
    else
    {
      /* 波特率预分频器在 Motorola 从机模式下不使用。强制设为默认值 */
      hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    }
  }
  else
  {
    assert_param(IS_SPI_BAUDRATE_PRESCALER(hspi->Init.BaudRatePrescaler));

    /* 强制将极性和相位配置为满足 TI 协议的要求 */
    hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi->Init.CLKPhase    = SPI_PHASE_1EDGE;
  }
#if (USE_SPI_CRC != 0U)
  assert_param(IS_SPI_CRC_CALCULATION(hspi->Init.CRCCalculation));
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    assert_param(IS_SPI_CRC_POLYNOMIAL(hspi->Init.CRCPolynomial));
  }
#else
  hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
#endif /* USE_SPI_CRC */

  if (hspi->State == HAL_SPI_STATE_RESET)
  {
    /* 分配锁资源并进行初始化 */
    hspi->Lock = HAL_UNLOCKED;

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    /* 初始化 SPI 回调函数设置 */
    hspi->TxCpltCallback       = HAL_SPI_TxCpltCallback;       /* 遗留的弱定义 TxCpltCallback       */
    hspi->RxCpltCallback       = HAL_SPI_RxCpltCallback;       /* 传统的弱定义 RxCpltCallback       */
    hspi->TxRxCpltCallback     = HAL_SPI_TxRxCpltCallback;     /* 传统的弱定义 TxRxCpltCallback     */
    hspi->TxHalfCpltCallback   = HAL_SPI_TxHalfCpltCallback;   /* 传统的弱定义 TxHalfCpltCallback   */
    hspi->RxHalfCpltCallback   = HAL_SPI_RxHalfCpltCallback;   /* 传统的弱定义 RxHalfCpltCallback   */
    hspi->TxRxHalfCpltCallback = HAL_SPI_TxRxHalfCpltCallback; /* 传统的弱定义 TxRxHalfCpltCallback */
    hspi->ErrorCallback        = HAL_SPI_ErrorCallback;        /* 传统的弱定义 ErrorCallback        */
    hspi->AbortCpltCallback    = HAL_SPI_AbortCpltCallback;    /* 传统的弱定义 AbortCpltCallback    */

    if (hspi->MspInitCallback == NULL)
    {
      hspi->MspInitCallback = HAL_SPI_MspInit; /* 传统的弱定义 MspInit  */
    }

    /* 初始化底层硬件：GPIO、CLOCK、NVIC... */
    hspi->MspInitCallback(hspi);
#else
    /* 初始化底层硬件：GPIO、CLOCK、NVIC... */
    HAL_SPI_MspInit(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
  }

  hspi->State = HAL_SPI_STATE_BUSY;

  /* 禁用指定的 SPI 外设 */
  __HAL_SPI_DISABLE(hspi);

  /*----------------------- SPIx CR1 与 CR2 配置 ---------------------*/
  /* 配置：SPI 模式、通信模式、数据大小、时钟极性和相位、NSS 管理、
  通信速率、起始位以及 CRC 校验计算状态 */
  WRITE_REG(hspi->Instance->CR1, ((hspi->Init.Mode & (SPI_CR1_MSTR | SPI_CR1_SSI)) |
                                  (hspi->Init.Direction & (SPI_CR1_RXONLY | SPI_CR1_BIDIMODE)) |
                                  (hspi->Init.DataSize & SPI_CR1_DFF) |
                                  (hspi->Init.CLKPolarity & SPI_CR1_CPOL) |
                                  (hspi->Init.CLKPhase & SPI_CR1_CPHA) |
                                  (hspi->Init.NSS & SPI_CR1_SSM) |
                                  (hspi->Init.BaudRatePrescaler & SPI_CR1_BR_Msk) |
                                  (hspi->Init.FirstBit  & SPI_CR1_LSBFIRST) |
                                  (hspi->Init.CRCCalculation & SPI_CR1_CRCEN)));

  /* 配置：NSS 管理 */
  WRITE_REG(hspi->Instance->CR2, ((hspi->Init.NSS >> 16U) & SPI_CR2_SSOE));

#if (USE_SPI_CRC != 0U)
  /*---------------------------- SPIx CRCPOLY 配置 ------------------*/
  /* 配置：CRC 多项式 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    WRITE_REG(hspi->Instance->CRCPR, (hspi->Init.CRCPolynomial & SPI_CRCPR_CRCPOLY_Msk));
  }
#endif /* USE_SPI_CRC */

#if defined(SPI_I2SCFGR_I2SMOD)
  /* 激活 SPI 模式（确保 I2SCFGR 寄存器中的 I2SMOD 位被复位） */
  CLEAR_BIT(hspi->Instance->I2SCFGR, SPI_I2SCFGR_I2SMOD);
#endif /* SPI_I2SCFGR_I2SMOD */

  hspi->ErrorCode = HAL_SPI_ERROR_NONE;
  hspi->State     = HAL_SPI_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  反初始化 SPI 外设。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi)
{
  /* 检查 SPI 句柄分配 */
  if (hspi == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查 SPI 实例参数 */
  assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));

  hspi->State = HAL_SPI_STATE_BUSY;

  /* 禁用 SPI 外设时钟 */
  __HAL_SPI_DISABLE(hspi);

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  if (hspi->MspDeInitCallback == NULL)
  {
    hspi->MspDeInitCallback = HAL_SPI_MspDeInit; /* 传统的弱定义 MspDeInit */
  }

  /* 反初始化底层硬件：GPIO、时钟、NVIC... */
  hspi->MspDeInitCallback(hspi);
#else
  /* 反初始化底层硬件：GPIO、时钟、NVIC... */
  HAL_SPI_MspDeInit(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

  hspi->ErrorCode = HAL_SPI_ERROR_NONE;
  hspi->State = HAL_SPI_STATE_RESET;

  /* 释放锁 */
  __HAL_UNLOCK(hspi);

  return HAL_OK;
}

/**
  * @brief  初始化 SPI MSP。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  /* 防止未使用参数产生的编译警告 */
  UNUSED(hspi);

  /* 注意：不应修改此函数，当需要回调函数时，
            应在用户文件中实现 HAL_SPI_MspInit
   */
}

/**
  * @brief  反初始化 SPI MSP。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  /* 防止未使用参数产生的编译警告 */
  UNUSED(hspi);

  /* 注意：不应修改此函数，当需要回调函数时，
            应在用户文件中实现 HAL_SPI_MspDeInit
   */
}

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
/**
  * @brief  注册用户 SPI 回调函数
  *         用于替代预定义的弱（weak）回调函数
  * @param  hspi 指向包含指定 SPI 配置信息的
  *                SPI_HandleTypeDef 结构体的指针。
  * @param  CallbackID 要注册的回调函数 ID
  * @param  pCallback 指向回调函数的指针
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_RegisterCallback(SPI_HandleTypeDef *hspi, HAL_SPI_CallbackIDTypeDef CallbackID,
                                           pSPI_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    /* 更新错误代码 */
    hspi->ErrorCode |= HAL_SPI_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }
  /* 进程已锁定 */
  __HAL_LOCK(hspi);

  if (HAL_SPI_STATE_READY == hspi->State)
  {
    switch (CallbackID)
    {
      case HAL_SPI_TX_COMPLETE_CB_ID :
        hspi->TxCpltCallback = pCallback;
        break;

      case HAL_SPI_RX_COMPLETE_CB_ID :
        hspi->RxCpltCallback = pCallback;
        break;

      case HAL_SPI_TX_RX_COMPLETE_CB_ID :
        hspi->TxRxCpltCallback = pCallback;
        break;

      case HAL_SPI_TX_HALF_COMPLETE_CB_ID :
        hspi->TxHalfCpltCallback = pCallback;
        break;

      case HAL_SPI_RX_HALF_COMPLETE_CB_ID :
        hspi->RxHalfCpltCallback = pCallback;
        break;

      case HAL_SPI_TX_RX_HALF_COMPLETE_CB_ID :
        hspi->TxRxHalfCpltCallback = pCallback;
        break;

      case HAL_SPI_ERROR_CB_ID :
        hspi->ErrorCallback = pCallback;
        break;

      case HAL_SPI_ABORT_CB_ID :
        hspi->AbortCpltCallback = pCallback;
        break;

      case HAL_SPI_MSPINIT_CB_ID :
        hspi->MspInitCallback = pCallback;
        break;

      case HAL_SPI_MSPDEINIT_CB_ID :
        hspi->MspDeInitCallback = pCallback;
        break;

      default :
        /* 更新错误代码 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (HAL_SPI_STATE_RESET == hspi->State)
  {
    switch (CallbackID)
    {
      case HAL_SPI_MSPINIT_CB_ID :
        hspi->MspInitCallback = pCallback;
        break;

      case HAL_SPI_MSPDEINIT_CB_ID :
        hspi->MspDeInitCallback = pCallback;
        break;

      default :
        /* 更新错误代码 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* 更新错误代码 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);

    /* 返回错误状态 */
    status =  HAL_ERROR;
  }

  /* 释放锁 */
  __HAL_UNLOCK(hspi);
  return status;
}

/**
  * @brief  注销 SPI 回调函数
  *         SPI 回调函数将被重定向至预定义的弱函数（weak）回调
  * @param  hspi 指向包含指定 SPI 配置信息的 SPI_HandleTypeDef 结构体的指针
  * @param  CallbackID 要注销的回调函数 ID
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_UnRegisterCallback(SPI_HandleTypeDef *hspi, HAL_SPI_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 进程已锁定 */
  __HAL_LOCK(hspi);

  if (HAL_SPI_STATE_READY == hspi->State)
  {
    switch (CallbackID)
    {
      case HAL_SPI_TX_COMPLETE_CB_ID :
        hspi->TxCpltCallback = HAL_SPI_TxCpltCallback;             /* 传统的弱定义 TxCpltCallback       */
        break;

      case HAL_SPI_RX_COMPLETE_CB_ID :
        hspi->RxCpltCallback = HAL_SPI_RxCpltCallback;             /* 传统的弱定义 RxCpltCallback       */
        break;

      case HAL_SPI_TX_RX_COMPLETE_CB_ID :
        hspi->TxRxCpltCallback = HAL_SPI_TxRxCpltCallback;         /* 传统的弱定义 TxRxCpltCallback     */
        break;

      case HAL_SPI_TX_HALF_COMPLETE_CB_ID :
        hspi->TxHalfCpltCallback = HAL_SPI_TxHalfCpltCallback;     /* 传统的弱定义 TxHalfCpltCallback   */
        break;

      case HAL_SPI_RX_HALF_COMPLETE_CB_ID :
        hspi->RxHalfCpltCallback = HAL_SPI_RxHalfCpltCallback;     /* 传统的弱定义 RxHalfCpltCallback   */
        break;

      case HAL_SPI_TX_RX_HALF_COMPLETE_CB_ID :
        hspi->TxRxHalfCpltCallback = HAL_SPI_TxRxHalfCpltCallback; /* 传统的弱定义 TxRxHalfCpltCallback */
        break;

      case HAL_SPI_ERROR_CB_ID :
        hspi->ErrorCallback = HAL_SPI_ErrorCallback;               /* 传统的弱定义 ErrorCallback        */
        break;

      case HAL_SPI_ABORT_CB_ID :
        hspi->AbortCpltCallback = HAL_SPI_AbortCpltCallback;       /* 传统的弱定义 AbortCpltCallback    */
        break;

      case HAL_SPI_MSPINIT_CB_ID :
        hspi->MspInitCallback = HAL_SPI_MspInit;                   /* 传统的弱定义 MspInit              */
        break;

      case HAL_SPI_MSPDEINIT_CB_ID :
        hspi->MspDeInitCallback = HAL_SPI_MspDeInit;               /* 传统的弱定义 MspDeInit            */
        break;

      default :
        /* 更新错误代码 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (HAL_SPI_STATE_RESET == hspi->State)
  {
    switch (CallbackID)
    {
      case HAL_SPI_MSPINIT_CB_ID :
        hspi->MspInitCallback = HAL_SPI_MspInit;                   /* 传统的弱定义 MspInit              */
        break;

      case HAL_SPI_MSPDEINIT_CB_ID :
        hspi->MspDeInitCallback = HAL_SPI_MspDeInit;               /* 遗留的弱定义 MspDeInit            */
        break;

      default :
        /* 更新错误代码 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* 更新错误代码 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);

    /* 返回错误状态 */
    status =  HAL_ERROR;
  }

  /* 释放锁 */
  __HAL_UNLOCK(hspi);
  return status;
}
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup SPI_Exported_Functions_Group2 IO operation functions
  *  @brief   数据传输函数
  *
@verbatim
  ==============================================================================
                      ##### IO 操作函数 #####
 ===============================================================================
 [..]
    本节提供了一组用于管理 SPI 数据传输的函数。

    [..] SPI 支持主模式和从模式：

    (#) 存在两种传输模式：
       (++) 阻塞模式：通信以轮询方式进行。
            所有数据处理的 HAL 状态均在传输完成后由同一个函数返回。
       (++) 非阻塞模式：通信使用中断或 DMA 方式进行，这些 API 会直接返回 HAL 状态。
            在使用中断模式时，数据处理的结束将通过专用的 SPI IRQ 指示；
            在使用 DMA 模式时，则通过 DMA IRQ 指示。
            HAL_SPI_TxCpltCallback()、HAL_SPI_RxCpltCallback() 和 HAL_SPI_TxRxCpltCallback() 用户回调函数
            将分别在发送或接收过程结束时执行。
            当检测到通信错误时，将执行 HAL_SPI_ErrorCallback() 用户回调函数。

    (#) 为这两种传输模式（阻塞模式，以及使用中断或 DMA 的非阻塞模式）提供的 API
        均支持单线（半双工/单工）和双线（全双工）模式。

@endverbatim
  * @{
  */

/**
  * @brief  以阻塞模式发送一定量的数据。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）。
  * @param  Size 要发送的数据元素量（u8 或 u16）。
  * @param  Timeout 超时时间（毫秒）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  uint32_t tickstart;
  uint16_t initial_TxXferCount;

  /* 检查 Direction 参数 */
  assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE(hspi->Init.Direction));

  /* 初始化 tickstart 以进行超时管理 */
  tickstart = HAL_GetTick();
  initial_TxXferCount = Size;

  if (hspi->State != HAL_SPI_STATE_READY)
  {
    return HAL_BUSY;
  }

  if ((pData == NULL) || (Size == 0U))
  {
    return HAL_ERROR;
  }

  /* 进程锁定 */
  __HAL_LOCK(hspi);

  /* 设置传输事务信息 */
  hspi->State       = HAL_SPI_STATE_BUSY_TX;
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pTxBuffPtr  = (const uint8_t *)pData;
  hspi->TxXferSize  = Size;
  hspi->TxXferCount = Size;

  /* 将句柄中未使用的字段清零 */
  hspi->pRxBuffPtr  = (uint8_t *)NULL;
  hspi->RxXferSize  = 0U;
  hspi->RxXferCount = 0U;
  hspi->TxISR       = NULL;
  hspi->RxISR       = NULL;

  /* 配置通信方向：单线 */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
  {
    /* 在设置单线方向（BIDIOE 位）之前禁用 SPI 外设 */
    __HAL_SPI_DISABLE(hspi);
    SPI_1LINE_TX(hspi);
  }

#if (USE_SPI_CRC != 0U)
  /* 复位 CRC 计算 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
  }
#endif /* USE_SPI_CRC */

  /* 检查 SPI 是否已经使能 */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* 使能 SPI 外设 */
    __HAL_SPI_ENABLE(hspi);
  }

  /* 在 16 位模式下发送数据 */
  if (hspi->Init.DataSize == SPI_DATASIZE_16BIT)
  {
    if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U))
    {
      hspi->Instance->DR = *((const uint16_t *)hspi->pTxBuffPtr);
      hspi->pTxBuffPtr += sizeof(uint16_t);
      hspi->TxXferCount--;
    }
    /* 在 16 位模式下发送数据 */
    while (hspi->TxXferCount > 0U)
    {
      /* 等待 TXE 标志位置位以发送数据 */
      if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE))
      {
        hspi->Instance->DR = *((const uint16_t *)hspi->pTxBuffPtr);
        hspi->pTxBuffPtr += sizeof(uint16_t);
        hspi->TxXferCount--;
      }
      else
      {
        /* 超时管理 */
        if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
        {
          hspi->State = HAL_SPI_STATE_READY;
          __HAL_UNLOCK(hspi);
          return HAL_TIMEOUT;
        }
      }
    }
  }
  /* 在 8 位模式下发送数据 */
  else
  {
    if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U))
    {
      *((__IO uint8_t *)&hspi->Instance->DR) = *((const uint8_t *)hspi->pTxBuffPtr);
      hspi->pTxBuffPtr += sizeof(uint8_t);
      hspi->TxXferCount--;
    }
    while (hspi->TxXferCount > 0U)
    {
      /* 等待 TXE 标志位置位以发送数据 */
      if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE))
      {
        *((__IO uint8_t *)&hspi->Instance->DR) = *((const uint8_t *)hspi->pTxBuffPtr);
        hspi->pTxBuffPtr += sizeof(uint8_t);
        hspi->TxXferCount--;
      }
      else
      {
        /* 超时管理 */
        if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
        {
          hspi->State = HAL_SPI_STATE_READY;
          __HAL_UNLOCK(hspi);
          return HAL_TIMEOUT;
        }
      }
    }
  }
#if (USE_SPI_CRC != 0U)
  /* 使能 CRC 发送 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
  }
#endif /* USE_SPI_CRC */

  /* 检查传输是否结束 */
  if (SPI_EndRxTxTransaction(hspi, Timeout, tickstart) != HAL_OK)
  {
    hspi->ErrorCode = HAL_SPI_ERROR_FLAG;
  }

  /* 在双线通信模式下清除过载（Overrun）标志，因为未读取接收到的数据 */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)
  {
    __HAL_SPI_CLEAR_OVRFLAG(hspi);
  }

  hspi->State = HAL_SPI_STATE_READY;
  /* 进程解锁 */
  __HAL_UNLOCK(hspi);

  if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
  {
    return HAL_ERROR;
  }
  else
  {
    return HAL_OK;
  }
}

/**
  * @brief  在阻塞模式下接收指定量的数据。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构的指针。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）
  * @param  Size 待接收的数据元素数量（u8 或 u16）
  * @param  Timeout 超时时间（毫秒）
  * @retval HAL 状态
  * @note   在主模式下，如果通信方向设置为 SPI_DIRECTION_2LINES，
  *         则会向数据寄存器 (DR) 写入接收缓冲区以生成时钟脉冲
  *         并接收数据
  */
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
#if (USE_SPI_CRC != 0U)
  __IO uint32_t tmpreg = 0U;
#endif /* USE_SPI_CRC */
  uint32_t tickstart;

  if (hspi->State != HAL_SPI_STATE_READY)
  {
    return HAL_BUSY;
  }

  if ((pData == NULL) || (Size == 0U))
  {
    return HAL_ERROR;
  }

  if ((hspi->Init.Mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES))
  {
    hspi->State = HAL_SPI_STATE_BUSY_RX;
    /* 调用收发一体函数在 Tx 线上发送伪数据（Dummy data），并在 CLK 线上生成时钟脉冲 */
    return HAL_SPI_TransmitReceive(hspi, pData, pData, Size, Timeout);
  }

  /* 初始化 tickstart 以进行超时管理 */
  tickstart = HAL_GetTick();

  /* 进程加锁 */
  __HAL_LOCK(hspi);

  /* 设置传输信息 */
  hspi->State       = HAL_SPI_STATE_BUSY_RX;
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pRxBuffPtr  = (uint8_t *)pData;
  hspi->RxXferSize  = Size;
  hspi->RxXferCount = Size;

  /* 将句柄中未使用的字段初始化为零 */
  hspi->pTxBuffPtr  = (uint8_t *)NULL;
  hspi->TxXferSize  = 0U;
  hspi->TxXferCount = 0U;
  hspi->RxISR       = NULL;
  hspi->TxISR       = NULL;

#if (USE_SPI_CRC != 0U)
  /* 复位 CRC 计算 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
    /* 此操作用于在最后一个数据之前处理 CRCNEXT */
    hspi->RxXferCount--;
  }
#endif /* USE_SPI_CRC */

  /* 配置通信方向：单线（1Line） */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
  {
    /* 在设置单线方向（BIDIOE 位）之前禁用 SPI 外设 */
    __HAL_SPI_DISABLE(hspi);
    SPI_1LINE_RX(hspi);
  }

  /* 检查 SPI 是否已使能 */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* 使能 SPI 外设 */
    __HAL_SPI_ENABLE(hspi);
  }

  /* 以 8 位模式接收数据 */
  if (hspi->Init.DataSize == SPI_DATASIZE_8BIT)
  {
    /* 传输循环 */
    while (hspi->RxXferCount > 0U)
    {
      /* 检查 RXNE 标志 */
      if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE))
      {
        /* 读取接收到的数据 */
        (* (uint8_t *)hspi->pRxBuffPtr) = *(__IO uint8_t *)&hspi->Instance->DR;
        hspi->pRxBuffPtr += sizeof(uint8_t);
        hspi->RxXferCount--;
      }
      else
      {
        /* 超时管理 */
        if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
        {
          hspi->State = HAL_SPI_STATE_READY;
          __HAL_UNLOCK(hspi);
          return HAL_TIMEOUT;
        }
      }
    }
  }
  else
  {
    /* 传输循环 */
    while (hspi->RxXferCount > 0U)
    {
      /* 检查 RXNE 标志 */
      if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE))
      {
        *((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)hspi->Instance->DR;
        hspi->pRxBuffPtr += sizeof(uint16_t);
        hspi->RxXferCount--;
      }
      else
      {
        /* 超时管理 */
        if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
        {
          hspi->State = HAL_SPI_STATE_READY;
          __HAL_UNLOCK(hspi);
          return HAL_TIMEOUT;
        }
      }
    }
  }

#if (USE_SPI_CRC != 0U)
  /* 处理 CRC 传输 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    /* 在读取最后一个数据之前冻结 CRC */
    SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);

    /* 检查 CRCNEXT 是否已成功由硬件复位 */
    if (READ_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT))
    {
      /* 用于在 CRCNEXT 未被硬件自动复位的情况下强制将其清零的规避方案 (Workaround) */
      CLEAR_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
    }
    /* 读取最后一个数据 */
    if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, Timeout, tickstart) != HAL_OK)
    {
      /* 尚未接收到最新数据 */
      __HAL_UNLOCK(hspi);
      return HAL_TIMEOUT;
    }

    /* 在 16 位模式下接收最后一个数据 */
    if (hspi->Init.DataSize == SPI_DATASIZE_16BIT)
    {
      *((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)hspi->Instance->DR;
    }
    /* 在 8 位模式下接收最后一个数据 */
    else
    {
      (*(uint8_t *)hspi->pRxBuffPtr) = *(__IO uint8_t *)&hspi->Instance->DR;
    }

    /* 等待 CRC 数据 */
    if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, Timeout, tickstart) != HAL_OK)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);
      hspi->State = HAL_SPI_STATE_READY;
      __HAL_UNLOCK(hspi);
      return HAL_TIMEOUT;
    }

    /* 读取 CRC 以清空 DR 寄存器和 RXNE 标志 */
    tmpreg = READ_REG(hspi->Instance->DR);
    /* 避免 GCC 警告 */
    UNUSED(tmpreg);
  }
#endif /* USE_SPI_CRC */

  /* 检查传输是否结束 */
  if (SPI_EndRxTransaction(hspi, Timeout, tickstart) != HAL_OK)
  {
    hspi->ErrorCode = HAL_SPI_ERROR_FLAG;
  }

#if (USE_SPI_CRC != 0U)
  /* 检查是否发生 CRC 错误 */
  if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR) != RESET)
  {
    /* 检查 CRC 错误是否有效（是否需要应用变通方案） */
    if (SPI_ISCRCErrorValid(hspi) == SPI_VALID_CRC_ERROR)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);

      /* 复位 CRC 计算 */
      SPI_RESET_CRC(hspi);
    }
    else
    {
      __HAL_SPI_CLEAR_CRCERRFLAG(hspi);
    }
  }
#endif /* USE_SPI_CRC */

  hspi->State = HAL_SPI_STATE_READY;
  /* 解锁进程 */
  __HAL_UNLOCK(hspi);
  if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
  {
    return HAL_ERROR;
  }
  else
  {
    return HAL_OK;
  }
}

/**
  * @brief  在阻塞模式下收发一定量的数据。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构的指针。
  * @param  pTxData 指向发送数据缓冲区的指针（u8 或 u16 数据元素）
  * @param  pRxData 指向接收数据缓冲区的指针（u8 或 u16 数据元素）
  * @param  Size 要发送和接收的数据元素数量（u8 或 u16）
  * @param  Timeout 超时时间（单位：毫秒）
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData,
                                          uint16_t Size, uint32_t Timeout)
{
  uint16_t             initial_TxXferCount;
  uint32_t             tmp_mode;
  HAL_SPI_StateTypeDef tmp_state;
  uint32_t             tickstart;
#if (USE_SPI_CRC != 0U)
  __IO uint32_t tmpreg = 0U;
#endif /* USE_SPI_CRC */

  /* 用于在传输过程中交替进行接收（Rx）和发送（Tx）的变量 */
  uint32_t             txallowed = 1U;

  /* 检查 Direction 参数 */
  assert_param(IS_SPI_DIRECTION_2LINES(hspi->Init.Direction));

  /* 初始化用于超时管理的 tickstart */
  tickstart = HAL_GetTick();

  /* 初始化临时变量 */
  tmp_state           = hspi->State;
  tmp_mode            = hspi->Init.Mode;
  initial_TxXferCount = Size;

  if (!((tmp_state == HAL_SPI_STATE_READY) || \
        ((tmp_mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES) &&
         (tmp_state == HAL_SPI_STATE_BUSY_RX))))
  {
    return HAL_BUSY;
  }

  if ((pTxData == NULL) || (pRxData == NULL) || (Size == 0U))
  {
    return HAL_ERROR;
  }

  /* 进程已锁定 */
  __HAL_LOCK(hspi);

  /* 在状态为 HAL_SPI_STATE_BUSY_RX 的情况下不要覆盖 */
  if (hspi->State != HAL_SPI_STATE_BUSY_RX)
  {
    hspi->State = HAL_SPI_STATE_BUSY_TX_RX;
  }

  /* 设置传输事务信息 */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pRxBuffPtr  = (uint8_t *)pRxData;
  hspi->RxXferCount = Size;
  hspi->RxXferSize  = Size;
  hspi->pTxBuffPtr  = (const uint8_t *)pTxData;
  hspi->TxXferCount = Size;
  hspi->TxXferSize  = Size;

  /* 将句柄中未使用的字段初始化为零 */
  hspi->RxISR       = NULL;
  hspi->TxISR       = NULL;

#if (USE_SPI_CRC != 0U)
  /* 复位 CRC 计算 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
  }
#endif /* USE_SPI_CRC */

  /* 检查 SPI 是否已使能 */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* 使能 SPI 外设 */
    __HAL_SPI_ENABLE(hspi);
  }

  /* 在 16 位模式下发送和接收数据 */
  if (hspi->Init.DataSize == SPI_DATASIZE_16BIT)
  {
    if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U))
    {
      hspi->Instance->DR = *((const uint16_t *)hspi->pTxBuffPtr);
      hspi->pTxBuffPtr += sizeof(uint16_t);
      hspi->TxXferCount--;

#if (USE_SPI_CRC != 0U)
      /* 使能 CRC 发送 */
      if ((hspi->TxXferCount == 0U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE))
      {
        SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
      }
#endif /* USE_SPI_CRC */

    }
    while ((hspi->TxXferCount > 0U) || (hspi->RxXferCount > 0U))
    {
      /* 检查 TXE 标志 */
      if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE)) && (hspi->TxXferCount > 0U) && (txallowed == 1U))
      {
        hspi->Instance->DR = *((const uint16_t *)hspi->pTxBuffPtr);
        hspi->pTxBuffPtr += sizeof(uint16_t);
        hspi->TxXferCount--;
        /* 下一个数据为接收 (Rx)。不允许发送 (Tx) */
        txallowed = 0U;

#if (USE_SPI_CRC != 0U)
        /* 使能 CRC 发送 */
        if ((hspi->TxXferCount == 0U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE))
        {
          SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
        }
#endif /* USE_SPI_CRC */
      }

      /* 检查 RXNE 标志 */
      if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) && (hspi->RxXferCount > 0U))
      {
        *((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)hspi->Instance->DR;
        hspi->pRxBuffPtr += sizeof(uint16_t);
        hspi->RxXferCount--;
        /* 下一个数据为发送 (Tx)。允许发送 (Tx) */
        txallowed = 1U;
      }
      if (((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY))
      {
        hspi->State = HAL_SPI_STATE_READY;
        __HAL_UNLOCK(hspi);
        return HAL_TIMEOUT;
      }
    }
  }
  /* 以 8 位模式发送和接收数据 */
  else
  {
    if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U))
    {
      *((__IO uint8_t *)&hspi->Instance->DR) = *((const uint8_t *)hspi->pTxBuffPtr);
      hspi->pTxBuffPtr += sizeof(uint8_t);
      hspi->TxXferCount--;

#if (USE_SPI_CRC != 0U)
      /* 使能 CRC 发送 */
      if ((hspi->TxXferCount == 0U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE))
      {
        SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
      }
#endif /* USE_SPI_CRC */
    }
    while ((hspi->TxXferCount > 0U) || (hspi->RxXferCount > 0U))
    {
      /* 检查 TXE 标志 */
      if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE)) && (hspi->TxXferCount > 0U) && (txallowed == 1U))
      {
        *(__IO uint8_t *)&hspi->Instance->DR = *((const uint8_t *)hspi->pTxBuffPtr);
        hspi->pTxBuffPtr++;
        hspi->TxXferCount--;
        /* 下一个数据为接收 (Rx)。不允许发送 (Tx) */
        txallowed = 0U;

#if (USE_SPI_CRC != 0U)
        /* 使能 CRC 发送 */
        if ((hspi->TxXferCount == 0U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE))
        {
          SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
        }
#endif /* USE_SPI_CRC */
      }

      /* 等待直到 RXNE 标志复位 */
      if ((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) && (hspi->RxXferCount > 0U))
      {
        (*(uint8_t *)hspi->pRxBuffPtr) = hspi->Instance->DR;
        hspi->pRxBuffPtr++;
        hspi->RxXferCount--;
        /* 下一个数据为发送 (Tx)。允许发送 (Tx) */
        txallowed = 1U;
      }
      if ((((HAL_GetTick() - tickstart) >=  Timeout) && ((Timeout != HAL_MAX_DELAY))) || (Timeout == 0U))
      {
        hspi->State = HAL_SPI_STATE_READY;
        __HAL_UNLOCK(hspi);
        return HAL_TIMEOUT;
      }
    }
  }

#if (USE_SPI_CRC != 0U)
  /* 从 DR 读取 CRC 以结束 CRC 计算流程 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    /* 等待 TXE 标志置位 */
    if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, Timeout, tickstart) != HAL_OK)
    {
      /* CRC 接收错误 */
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);
      hspi->State = HAL_SPI_STATE_READY;
      __HAL_UNLOCK(hspi);
      return HAL_TIMEOUT;
    }
    /* 读取 CRC */
    tmpreg = READ_REG(hspi->Instance->DR);
    /* 避免 GCC 编译器警告 */
    UNUSED(tmpreg);
  }

  /* 检查是否发生 CRC 错误 */
  if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR) != RESET)
  {
    /* 检查 CRC 错误是否有效（是否需要应用变通方案） */
    if (SPI_ISCRCErrorValid(hspi) == SPI_VALID_CRC_ERROR)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);

      /* 复位 CRC 计算 */
      SPI_RESET_CRC(hspi);
      __HAL_UNLOCK(hspi);
      return HAL_ERROR;
    }
    else
    {
      __HAL_SPI_CLEAR_CRCERRFLAG(hspi);
    }
  }
#endif /* USE_SPI_CRC */

  /* 检查本次传输事务是否结束 */
  if (SPI_EndRxTxTransaction(hspi, Timeout, tickstart) != HAL_OK)
  {
    hspi->ErrorCode = HAL_SPI_ERROR_FLAG;
    __HAL_UNLOCK(hspi);
    return HAL_ERROR;
  }

  /* 在双线通信模式下清除过载（OVR）标志，因为未读取接收到的数据 */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)
  {
    __HAL_SPI_CLEAR_OVRFLAG(hspi);
  }


  hspi->State = HAL_SPI_STATE_READY;
  /* 解锁进程 */
  __HAL_UNLOCK(hspi);

  if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
  {
    return HAL_ERROR;
  }
  else
  {
    return HAL_OK;
  }
}

/**
  * @brief  在中断非阻塞模式下发送一定数量的数据。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构的指针。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）
  * @param  Size 要发送的数据元素（u8 或 u16）数量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size)
{

  /* 检查 Direction 参数 */
  assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE(hspi->Init.Direction));


  if ((pData == NULL) || (Size == 0U))
  {
    return HAL_ERROR;
  }

  if (hspi->State != HAL_SPI_STATE_READY)
  {
    return HAL_BUSY;
  }

  /* 进程已锁定 */
  __HAL_LOCK(hspi);

  /* 设置传输事务信息 */
  hspi->State       = HAL_SPI_STATE_BUSY_TX;
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pTxBuffPtr  = (const uint8_t *)pData;
  hspi->TxXferSize  = Size;
  hspi->TxXferCount = Size;

  /* 将句柄中未使用的字段初始化为零 */
  hspi->pRxBuffPtr  = (uint8_t *)NULL;
  hspi->RxXferSize  = 0U;
  hspi->RxXferCount = 0U;
  hspi->RxISR       = NULL;

  /* 设置中断处理函数 */
  if (hspi->Init.DataSize > SPI_DATASIZE_8BIT)
  {
    hspi->TxISR = SPI_TxISR_16BIT;
  }
  else
  {
    hspi->TxISR = SPI_TxISR_8BIT;
  }

  /* 配置通信方向：单线模式 */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
  {
    /* 在设置单线方向（BIDIOE 位）前禁用 SPI 外设 */
    __HAL_SPI_DISABLE(hspi);
    SPI_1LINE_TX(hspi);
  }

#if (USE_SPI_CRC != 0U)
  /* 复位 CRC 计算 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
  }
#endif /* USE_SPI_CRC */

  /* 检查 SPI 是否已经使能 */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* 使能 SPI 外设 */
    __HAL_SPI_ENABLE(hspi);
  }

  /* 进程解锁 */
  __HAL_UNLOCK(hspi);
  /* 使能 TXE 和 ERR 中断 */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_ERR));

  return HAL_OK;
}

/**
  * @brief  在中断模式下以非阻塞方式接收指定量的数据。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构的指针。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）
  * @param  Size 待接收的数据元素数量（u8 或 u16）
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{

  if (hspi->State != HAL_SPI_STATE_READY)
  {
    return HAL_BUSY;
  }

  if ((pData == NULL) || (Size == 0U))
  {
    return HAL_ERROR;
  }

  if ((hspi->Init.Direction == SPI_DIRECTION_2LINES) && (hspi->Init.Mode == SPI_MODE_MASTER))
  {
    hspi->State = HAL_SPI_STATE_BUSY_RX;
    /* 调用收发函数在 Tx 线上发送伪数据并在 CLK 线上生成时钟 */
    return HAL_SPI_TransmitReceive_IT(hspi, pData, pData, Size);
  }


  /* 进程加锁 */
  __HAL_LOCK(hspi);

  /* 设置传输事务信息 */
  hspi->State       = HAL_SPI_STATE_BUSY_RX;
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pRxBuffPtr  = (uint8_t *)pData;
  hspi->RxXferSize  = Size;
  hspi->RxXferCount = Size;

  /* 将句柄中未使用的字段清零 */
  hspi->pTxBuffPtr  = (uint8_t *)NULL;
  hspi->TxXferSize  = 0U;
  hspi->TxXferCount = 0U;
  hspi->TxISR       = NULL;

  /* 设置用于中断处理的函数 */
  if (hspi->Init.DataSize > SPI_DATASIZE_8BIT)
  {
    hspi->RxISR = SPI_RxISR_16BIT;
  }
  else
  {
    hspi->RxISR = SPI_RxISR_8BIT;
  }

  /* 配置通信方向：单线模式 (1Line) */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
  {
    /* 在设置单线方向 (BIDIOE 位) 之前禁用 SPI 外设 */
    __HAL_SPI_DISABLE(hspi);
    SPI_1LINE_RX(hspi);
  }

#if (USE_SPI_CRC != 0U)
  /* 复位 CRC 计算 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
  }
#endif /* USE_SPI_CRC */

  /* 注意：必须在解锁当前进程之后再使能 SPI，
            以避免在当前进程解锁之前执行 SPI 中断处理程序的风险 */

  /* 检查 SPI 是否已经使能 */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* 使能 SPI 外设 */
    __HAL_SPI_ENABLE(hspi);
  }

  /* 进程已解锁 */
  __HAL_UNLOCK(hspi);
  /* 使能 RXNE 和 ERR 中断 */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));

  return HAL_OK;
}

/**
  * @brief  在中断非阻塞模式下收发一定数量的数据。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构的指针。
  * @param  pTxData 指向发送数据缓冲区的指针（u8 或 u16 数据元素）
  * @param  pRxData 指向接收数据缓冲区的指针（u8 或 u16 数据元素）
  * @param  Size 要发送和接收的数据元素数量（u8 或 u16）
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData,
                                             uint16_t Size)
{
  uint32_t             tmp_mode;
  HAL_SPI_StateTypeDef tmp_state;

  /* 检查方向参数 */
  assert_param(IS_SPI_DIRECTION_2LINES(hspi->Init.Direction));

  /* 初始化临时变量 */
  tmp_state           = hspi->State;
  tmp_mode            = hspi->Init.Mode;

  if (!((tmp_state == HAL_SPI_STATE_READY) || \
        ((tmp_mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES) &&
         (tmp_state == HAL_SPI_STATE_BUSY_RX))))
  {
    return HAL_BUSY;
  }

  if ((pTxData == NULL) || (pRxData == NULL) || (Size == 0U))
  {
    return HAL_ERROR;
  }

  /* 进程已加锁 */
  __HAL_LOCK(hspi);

  /* 若处于 HAL_SPI_STATE_BUSY_RX 状态则不覆盖 */
  if (hspi->State != HAL_SPI_STATE_BUSY_RX)
  {
    hspi->State = HAL_SPI_STATE_BUSY_TX_RX;
  }

  /* 设置传输事务信息 */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pTxBuffPtr  = (const uint8_t *)pTxData;
  hspi->TxXferSize  = Size;
  hspi->TxXferCount = Size;
  hspi->pRxBuffPtr  = (uint8_t *)pRxData;
  hspi->RxXferSize  = Size;
  hspi->RxXferCount = Size;

  /* 设置中断处理函数 */
  if (hspi->Init.DataSize > SPI_DATASIZE_8BIT)
  {
    hspi->RxISR     = SPI_2linesRxISR_16BIT;
    hspi->TxISR     = SPI_2linesTxISR_16BIT;
  }
  else
  {
    hspi->RxISR     = SPI_2linesRxISR_8BIT;
    hspi->TxISR     = SPI_2linesTxISR_8BIT;
  }

#if (USE_SPI_CRC != 0U)
  /* 复位 CRC 计算 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
  }
#endif /* USE_SPI_CRC */


  /* 检查 SPI 是否已使能 */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* 使能 SPI 外设 */
    __HAL_SPI_ENABLE(hspi);
  }

  /* 进程解锁 */
  __HAL_UNLOCK(hspi);
  /* 使能 TXE、RXNE 和 ERR 中断 */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

  return HAL_OK;
}

/**
  * @brief  使用 DMA 在非阻塞模式下发送一定数量的数据。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构的指针。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）
  * @param  Size 待发送的数据元素数量（u8 或 u16）
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size)
{

  /* 检查 TX DMA 句柄 */
  assert_param(IS_SPI_DMA_HANDLE(hspi->hdmatx));

  /* 检查 Direction 参数 */
  assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE(hspi->Init.Direction));

  if (hspi->State != HAL_SPI_STATE_READY)
  {
    return HAL_BUSY;
  }

  if ((pData == NULL) || (Size == 0U))
  {
    return HAL_ERROR;
  }

  /* 进程加锁 */
  __HAL_LOCK(hspi);

  /* 设置传输事务信息 */
  hspi->State       = HAL_SPI_STATE_BUSY_TX;
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pTxBuffPtr  = (const uint8_t *)pData;
  hspi->TxXferSize  = Size;
  hspi->TxXferCount = Size;

  /* 将句柄中未使用的字段初始化为零 */
  hspi->pRxBuffPtr  = (uint8_t *)NULL;
  hspi->TxISR       = NULL;
  hspi->RxISR       = NULL;
  hspi->RxXferSize  = 0U;
  hspi->RxXferCount = 0U;

  /* 配置通信方向：单线 */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
  {
    /* 在设置单线方向（BIDIOE 位）之前禁用 SPI 外设 */
    __HAL_SPI_DISABLE(hspi);
    SPI_1LINE_TX(hspi);
  }

#if (USE_SPI_CRC != 0U)
  /* 复位 CRC 计算 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
  }
#endif /* USE_SPI_CRC */

  /* 设置 SPI TxDMA 半传输完成回调函数 */
  hspi->hdmatx->XferHalfCpltCallback = SPI_DMAHalfTransmitCplt;

  /* 设置 SPI TxDMA 传输完成回调函数 */
  hspi->hdmatx->XferCpltCallback = SPI_DMATransmitCplt;

  /* 设置 DMA 错误回调函数 */
  hspi->hdmatx->XferErrorCallback = SPI_DMAError;

  /* 设置 DMA 中止完成回调函数 (AbortCpltCallback) */
  hspi->hdmatx->XferAbortCallback = NULL;

  /* 使能 Tx DMA 数据流/通道 */
  if (HAL_OK != HAL_DMA_Start_IT(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)&hspi->Instance->DR,
                                 hspi->TxXferCount))
  {
    /* 更新 SPI 错误代码 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
    /* 解锁进程 */
    __HAL_UNLOCK(hspi);
    return HAL_ERROR;
  }

  /* 检查 SPI 是否已使能 */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* 使能 SPI 外设 */
    __HAL_SPI_ENABLE(hspi);
  }

  /* 解锁进程 */
  __HAL_UNLOCK(hspi);

  /* 使能 SPI 错误中断位 */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_ERR));

  /* 使能发送 DMA 请求 */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN);

  return HAL_OK;
}

/**
  * @brief  在非阻塞模式下通过 DMA 接收指定数量的数据。
  * @note   在主机模式（MASTER）且为双线全双工方向（SPI_DIRECTION_2LINES）时，必须定义 hdmatx。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @param  pData 指向数据缓冲区的指针（u8 或 u16 数据元素）
  * @note   使能 CRC 功能时，pData 的长度必须为 Size + 1。
  * @param  Size 要接收的数据元素（u8 或 u16）数量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
  /* 检查接收 DMA 句柄 */
  assert_param(IS_SPI_DMA_HANDLE(hspi->hdmarx));

  if (hspi->State != HAL_SPI_STATE_READY)
  {
    return HAL_BUSY;
  }

  if ((pData == NULL) || (Size == 0U))
  {
    return HAL_ERROR;
  }

  if ((hspi->Init.Direction == SPI_DIRECTION_2LINES) && (hspi->Init.Mode == SPI_MODE_MASTER))
  {
    hspi->State = HAL_SPI_STATE_BUSY_RX;

    /* 检查发送 DMA 句柄 */
    assert_param(IS_SPI_DMA_HANDLE(hspi->hdmatx));

    /* 调用收发函数在 Tx 线上发送伪数据（Dummy data）并在 CLK 线上产生时钟 */
    return HAL_SPI_TransmitReceive_DMA(hspi, pData, pData, Size);
  }

  /* 进程加锁 */
  __HAL_LOCK(hspi);

  /* 设置传输信息 */
  hspi->State       = HAL_SPI_STATE_BUSY_RX;
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pRxBuffPtr  = (uint8_t *)pData;
  hspi->RxXferSize  = Size;
  hspi->RxXferCount = Size;

  /* 将句柄中未使用的字段初始化为零 */
  hspi->RxISR       = NULL;
  hspi->TxISR       = NULL;
  hspi->TxXferSize  = 0U;
  hspi->TxXferCount = 0U;

  /* 配置通信方向：单线模式（1Line） */
  if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
  {
    /* 在设置单线方向（BIDIOE 位）之前禁用 SPI 外设 */
    __HAL_SPI_DISABLE(hspi);
    SPI_1LINE_RX(hspi);
  }

#if (USE_SPI_CRC != 0U)
  /* 复位 CRC 计算 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
  }
#endif /* USE_SPI_CRC */

  /* 设置 SPI 接收 DMA 半传输完成回调函数 */
  hspi->hdmarx->XferHalfCpltCallback = SPI_DMAHalfReceiveCplt;

  /* 设置 SPI 接收 DMA 传输完成回调函数 */
  hspi->hdmarx->XferCpltCallback = SPI_DMAReceiveCplt;

  /* 设置 DMA 错误回调函数 */
  hspi->hdmarx->XferErrorCallback = SPI_DMAError;

  /* 设置 DMA 中止完成回调函数 (AbortCpltCallback) */
  hspi->hdmarx->XferAbortCallback = NULL;

  /* 使能接收 DMA 数据流/通道 (Rx DMA Stream/Channel) */
  if (HAL_OK != HAL_DMA_Start_IT(hspi->hdmarx, (uint32_t)&hspi->Instance->DR, (uint32_t)hspi->pRxBuffPtr,
                                 hspi->RxXferCount))
  {
    /* 更新 SPI 错误代码 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
    /* 进程解锁 */
    __HAL_UNLOCK(hspi);
    return HAL_ERROR;
  }

  /* 检查 SPI 是否已使能 */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* 使能 SPI 外设 */
    __HAL_SPI_ENABLE(hspi);
  }

  /* 进程解锁 */
  __HAL_UNLOCK(hspi);

  /* 使能 SPI 错误中断位 */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_ERR));

  /* 使能接收 DMA 请求 */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_RXDMAEN);

  return HAL_OK;
}

/**
  * @brief  使用 DMA 在非阻塞模式下发送和接收指定量的数据。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @param  pTxData 指向发送数据缓冲区的指针（u8 或 u16 数据元素）
  * @param  pRxData 指向接收数据缓冲区的指针（u8 或 u16 数据元素）
  * @note   使能 CRC 功能时，pRxData 的长度必须为 Size + 1
  * @param  Size 要发送和接收的数据元素（u8 或 u16）数量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData,
                                              uint16_t Size)
{
  uint32_t             tmp_mode;
  HAL_SPI_StateTypeDef tmp_state;

  /* 检查 rx 和 tx 的 DMA 句柄 */
  assert_param(IS_SPI_DMA_HANDLE(hspi->hdmarx));
  assert_param(IS_SPI_DMA_HANDLE(hspi->hdmatx));

  /* 检查方向 (Direction) 参数 */
  assert_param(IS_SPI_DIRECTION_2LINES(hspi->Init.Direction));

  /* 初始化临时变量 */
  tmp_state           = hspi->State;
  tmp_mode            = hspi->Init.Mode;

  if (!((tmp_state == HAL_SPI_STATE_READY) ||
        ((tmp_mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES) &&
         (tmp_state == HAL_SPI_STATE_BUSY_RX))))
  {
    return HAL_BUSY;
  }

  if ((pTxData == NULL) || (pRxData == NULL) || (Size == 0U))
  {
    return HAL_ERROR;
  }

  /* 进程加锁 */
  __HAL_LOCK(hspi);

  /* 若处于 HAL_SPI_STATE_BUSY_RX 状态则不要覆盖 */
  if (hspi->State != HAL_SPI_STATE_BUSY_RX)
  {
    hspi->State = HAL_SPI_STATE_BUSY_TX_RX;
  }

  /* 设置传输事务信息 */
  hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
  hspi->pTxBuffPtr  = (const uint8_t *)pTxData;
  hspi->TxXferSize  = Size;
  hspi->TxXferCount = Size;
  hspi->pRxBuffPtr  = (uint8_t *)pRxData;
  hspi->RxXferSize  = Size;
  hspi->RxXferCount = Size;

  /* 将句柄中未使用的字段初始化为零 */
  hspi->RxISR       = NULL;
  hspi->TxISR       = NULL;

#if (USE_SPI_CRC != 0U)
  /* 复位 CRC 计算 */
  if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
  {
    SPI_RESET_CRC(hspi);
  }
#endif /* USE_SPI_CRC */

  /* 检查是仅接收模式还是收发模式，并配置 DMA 传输完成回调函数 */
  if (hspi->State == HAL_SPI_STATE_BUSY_RX)
  {
    /* 设置 SPI 接收 DMA 半传输完成回调函数 */
    hspi->hdmarx->XferHalfCpltCallback = SPI_DMAHalfReceiveCplt;
    hspi->hdmarx->XferCpltCallback     = SPI_DMAReceiveCplt;
  }
  else
  {
    /* 设置 SPI 发送/接收 DMA 半传输完成回调函数 */
    hspi->hdmarx->XferHalfCpltCallback = SPI_DMAHalfTransmitReceiveCplt;
    hspi->hdmarx->XferCpltCallback     = SPI_DMATransmitReceiveCplt;
  }

  /* 设置 DMA 错误回调函数 */
  hspi->hdmarx->XferErrorCallback = SPI_DMAError;

  /* 设置 DMA 中止完成回调函数 (AbortCpltCallback) */
  hspi->hdmarx->XferAbortCallback = NULL;

  /* 使能接收 DMA 数据流/通道 */
  if (HAL_OK != HAL_DMA_Start_IT(hspi->hdmarx, (uint32_t)&hspi->Instance->DR, (uint32_t)hspi->pRxBuffPtr,
                                 hspi->RxXferCount))
  {
    /* 更新 SPI 错误代码 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
    /* 进程解锁 */
    __HAL_UNLOCK(hspi);
    return HAL_ERROR;
  }

  /* 使能接收 DMA 请求 */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_RXDMAEN);

  /* 将 SPI 发送 DMA 传输完成回调函数设为 NULL，因为通信关闭操作
  在 DMA 接收完成回调函数中执行 */
  hspi->hdmatx->XferHalfCpltCallback = NULL;
  hspi->hdmatx->XferCpltCallback     = NULL;
  hspi->hdmatx->XferErrorCallback    = NULL;
  hspi->hdmatx->XferAbortCallback    = NULL;

  /* 使能发送 DMA 数据流/通道 */
  if (HAL_OK != HAL_DMA_Start_IT(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)&hspi->Instance->DR,
                                 hspi->TxXferCount))
  {
    /* 更新 SPI 错误代码 */
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
    /* 进程解锁 */
    __HAL_UNLOCK(hspi);
    return HAL_ERROR;
  }

  /* 检查 SPI 是否已使能 */
  if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* 使能 SPI 外设 */
    __HAL_SPI_ENABLE(hspi);
  }

  /* 进程解锁 */
  __HAL_UNLOCK(hspi);

  /* 使能 SPI 错误中断位 */
  __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_ERR));

  /* 使能 Tx DMA 请求 */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN);

  return HAL_OK;
}

/**
  * @brief  中止正在进行的传输（阻塞模式）。
  * @param  hspi SPI 句柄。
  * @note   此过程可用于中止任何正在进行的传输（发送和接收），
  *         无论该传输是以中断模式还是 DMA 模式启动。
  *         该过程执行以下操作：
  *           - 禁用 SPI 中断（取决于传输方向）
  *           - 在外设寄存器中禁用 DMA 传输（如果已使能）
  *           - 通过调用 HAL_DMA_Abort 中止 DMA 传输（如果是 DMA 模式传输）
  *           - 将句柄状态设置为 READY
  * @note   此过程在阻塞模式下执行：退出函数时，中止操作被视为已完成。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_Abort(SPI_HandleTypeDef *hspi)
{
  HAL_StatusTypeDef errorcode;
  __IO uint32_t count;
  __IO uint32_t resetcount;

  /* 初始化局部变量 */
  errorcode = HAL_OK;
  resetcount = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);
  count = resetcount;

  /* 清除 ERRIE 中断，以避免在中止过程中产生错误中断 */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_ERRIE);

  /* 禁用 TXEIE、RXNEIE 和 ERRIE（模式错误事件、过载错误、TI 帧错误）中断 */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXEIE))
  {
    hspi->TxISR = SPI_AbortTx_ISR;
    /* 等待 HAL_SPI_STATE_ABORT 状态 */
    do
    {
      if (count == 0U)
      {
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
        break;
      }
      count--;
    } while (hspi->State != HAL_SPI_STATE_ABORT);
    /* 重置超时计数器 */
    count = resetcount;
  }

  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXNEIE))
  {
    hspi->RxISR = SPI_AbortRx_ISR;
    /* 等待 HAL_SPI_STATE_ABORT 状态 */
    do
    {
      if (count == 0U)
      {
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
        break;
      }
      count--;
    } while (hspi->State != HAL_SPI_STATE_ABORT);
    /* 重置超时计数器 */
    count = resetcount;
  }

  /* 若已使能，则禁用 SPI DMA 发送请求 */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXDMAEN))
  {
    /* 中止 SPI DMA 发送数据流/通道：使用阻塞式 DMA 中止 API（无回调） */
    if (hspi->hdmatx != NULL)
    {
      /* 设置 SPI DMA 中止回调函数：
      将在 DMA 中止流程结束时调用 HAL_SPI_AbortCpltCallback() */
      hspi->hdmatx->XferAbortCallback = NULL;

      /* 中止与 SPI 外设关联的 DMA 发送句柄 */
      if (HAL_DMA_Abort(hspi->hdmatx) != HAL_OK)
      {
        hspi->ErrorCode = HAL_SPI_ERROR_ABORT;
      }

      /* 禁用发送 DMA 请求 */
      CLEAR_BIT(hspi->Instance->CR2, (SPI_CR2_TXDMAEN));

      /* 等待 TXE 标志位置位 */
      do
      {
        if (count == 0U)
        {
          SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
          break;
        }
        count--;
      } while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET);
    }
  }

  /* 若已使能，则禁用 SPI DMA 接收请求 */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXDMAEN))
  {
    /* 中止 SPI DMA 接收数据流/通道：使用阻塞式 DMA 中止 API（无回调） */
    if (hspi->hdmarx != NULL)
    {
      /* 设置 SPI DMA 中止回调函数：
      将在 DMA 中止流程结束时调用 HAL_SPI_AbortCpltCallback() */
      hspi->hdmarx->XferAbortCallback = NULL;

      /* 中止与 SPI 外设关联的 DMA 接收句柄 */
      if (HAL_DMA_Abort(hspi->hdmarx) != HAL_OK)
      {
        hspi->ErrorCode = HAL_SPI_ERROR_ABORT;
      }

      /* 禁用外设 */
      __HAL_SPI_DISABLE(hspi);

      /* 禁用接收 DMA 请求 */
      CLEAR_BIT(hspi->Instance->CR2, (SPI_CR2_RXDMAEN));
    }
  }
  /* 重置发送和接收传输计数器 */
  hspi->RxXferCount = 0U;
  hspi->TxXferCount = 0U;

  /* 检查中止流程中是否存在错误 */
  if (hspi->ErrorCode == HAL_SPI_ERROR_ABORT)
  {
    /* 若中止流程中发生错误，则返回 HAL_Error */
    errorcode = HAL_ERROR;
  }
  else
  {
    /* 复位 errorCode */
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;
  }

  /* 清除 SR 寄存器中的错误标志 */
  __HAL_SPI_CLEAR_OVRFLAG(hspi);

  /* 恢复 hspi->state 为 ready 状态 */
  hspi->State = HAL_SPI_STATE_READY;

  return errorcode;
}

/**
  * @brief  中止正在进行的传输（中断模式）。
  * @param  hspi SPI 句柄。
  * @note   此过程可用于中止在中断或 DMA 模式下启动的任何正在进行的传输（发送和接收）。
  *         该过程执行以下操作：
  *           - 禁用 SPI 中断（取决于传输方向）
  *           - 禁用外设寄存器中的 DMA 传输（如果已使能）
  *           - 通过调用 HAL_DMA_Abort_IT 中止 DMA 传输（适用于 DMA 模式下的传输）
  *           - 将句柄状态设置为 READY
  *           - 中止完成时，调用用户中止完成回调函数
  * @note   此过程在中断模式下执行，这意味着只有在执行用户中止完成回调函数时（而不是退出函数时），
  *         中止过程才可被视为已完成。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_Abort_IT(SPI_HandleTypeDef *hspi)
{
  HAL_StatusTypeDef errorcode;
  uint32_t abortcplt ;
  __IO uint32_t count;
  __IO uint32_t resetcount;

  /* 初始化局部变量 */
  errorcode = HAL_OK;
  abortcplt = 1U;
  resetcount = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);
  count = resetcount;

  /* 清除 ERRIE 中断以避免在中止过程中产生错误中断 */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_ERRIE);

  /* 更改 Rx 和 Tx 中断处理程序以禁用 TXEIE、RXNEIE 和 ERRIE 中断 */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXEIE))
  {
    hspi->TxISR = SPI_AbortTx_ISR;
    /* 等待 HAL_SPI_STATE_ABORT 状态 */
    do
    {
      if (count == 0U)
      {
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
        break;
      }
      count--;
    } while (hspi->State != HAL_SPI_STATE_ABORT);
    /* 重置超时计数器 */
    count = resetcount;
  }

  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXNEIE))
  {
    hspi->RxISR = SPI_AbortRx_ISR;
    /* 等待 HAL_SPI_STATE_ABORT 状态 */
    do
    {
      if (count == 0U)
      {
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
        break;
      }
      count--;
    } while (hspi->State != HAL_SPI_STATE_ABORT);
    /* 重置超时计数器 */
    count = resetcount;
  }

  /* 如果 DMA Tx 和/或 DMA Rx 句柄关联到了 SPI 句柄，则在调用任何 DMA 中止函数之前，
     必须先初始化 DMA 中止完成回调函数 */
  /* DMA Tx 句柄有效 */
  if (hspi->hdmatx != NULL)
  {
    /* 如果使能了 UART DMA Tx 请求，则设置 DMA 中止完成回调函数。
       否则，将其设置为 NULL */
    if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXDMAEN))
    {
      hspi->hdmatx->XferAbortCallback = SPI_DMATxAbortCallback;
    }
    else
    {
      hspi->hdmatx->XferAbortCallback = NULL;
    }
  }
  /* DMA Rx 句柄有效 */
  if (hspi->hdmarx != NULL)
  {
    /* 如果使能了 UART DMA Rx 请求，则设置 DMA 中止完成回调函数。
       否则，将其设置为 NULL */
    if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXDMAEN))
    {
      hspi->hdmarx->XferAbortCallback = SPI_DMARxAbortCallback;
    }
    else
    {
      hspi->hdmarx->XferAbortCallback = NULL;
    }
  }

  /* 如果使能了 SPI DMA Tx 请求，则将其禁用 */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_TXDMAEN))
  {
    /* 中止 SPI DMA Tx 数据流/通道 */
    if (hspi->hdmatx != NULL)
    {
      /* 中止连接到 SPI 外设的 DMA Tx 句柄 */
      if (HAL_DMA_Abort_IT(hspi->hdmatx) != HAL_OK)
      {
        hspi->hdmatx->XferAbortCallback = NULL;
        hspi->ErrorCode = HAL_SPI_ERROR_ABORT;
      }
      else
      {
        abortcplt = 0U;
      }
    }
  }
  /* 如果使能了 SPI DMA Rx 请求，则将其禁用 */
  if (HAL_IS_BIT_SET(hspi->Instance->CR2, SPI_CR2_RXDMAEN))
  {
    /* 中止 SPI DMA Rx 数据流/通道 */
    if (hspi->hdmarx != NULL)
    {
      /* 中止连接到 SPI 外设的 DMA Rx 句柄 */
      if (HAL_DMA_Abort_IT(hspi->hdmarx) !=  HAL_OK)
      {
        hspi->hdmarx->XferAbortCallback = NULL;
        hspi->ErrorCode = HAL_SPI_ERROR_ABORT;
      }
      else
      {
        abortcplt = 0U;
      }
    }
  }

  if (abortcplt == 1U)
  {
    /* 复位 Tx 和 Rx 传输计数器 */
    hspi->RxXferCount = 0U;
    hspi->TxXferCount = 0U;

    /* 检查中止流程中是否存在错误 */
    if (hspi->ErrorCode == HAL_SPI_ERROR_ABORT)
    {
      /* 如果中止流程中发生错误，则返回 HAL_Error */
      errorcode = HAL_ERROR;
    }
    else
    {
      /* 复位 errorCode */
      hspi->ErrorCode = HAL_SPI_ERROR_NONE;
    }

    /* 清除 SR 寄存器中的错误标志 */
    __HAL_SPI_CLEAR_OVRFLAG(hspi);

    /* 将 hspi->State 恢复为就绪状态（Ready） */
    hspi->State = HAL_SPI_STATE_READY;

    /* 由于没有需要中止的 DMA，直接调用用户中止完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    hspi->AbortCpltCallback(hspi);
#else
    HAL_SPI_AbortCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
  }

  return errorcode;
}

/**
  * @brief  暂停 DMA 传输。
  * @param  hspi 指向包含指定 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_DMAPause(SPI_HandleTypeDef *hspi)
{
  /* 进程加锁 */
  __HAL_LOCK(hspi);

  /* 禁用 SPI DMA 发送和接收请求 */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);

  /* 进程解锁 */
  __HAL_UNLOCK(hspi);

  return HAL_OK;
}

/**
  * @brief  恢复 DMA 传输。
  * @param  hspi 指向包含指定 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_DMAResume(SPI_HandleTypeDef *hspi)
{
  /* 进程加锁 */
  __HAL_LOCK(hspi);

  /* 使能 SPI DMA 发送和接收请求 */
  SET_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);

  /* 进程解锁 */
  __HAL_UNLOCK(hspi);

  return HAL_OK;
}

/**
  * @brief  停止 DMA 传输。
  * @param  hspi 指向包含指定 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_SPI_DMAStop(SPI_HandleTypeDef *hspi)
{
  HAL_StatusTypeDef errorcode = HAL_OK;
  /* 此 API 未实现加锁操作，以便用户应用程序能够在
     HAL_SPI_TxCpltCallback()、HAL_SPI_RxCpltCallback() 或
     HAL_SPI_TxRxCpltCallback() 回调函数中调用 HAL SPI API：
     当调用 HAL_DMA_Abort() API 时，会生成 DMA TX/RX 传输完成中断，
     并执行相应的回调函数 HAL_SPI_TxCpltCallback()、HAL_SPI_RxCpltCallback()
     或 HAL_SPI_TxRxCpltCallback()
     */

  /* 中止 SPI DMA 发送数据流/通道 */
  if (hspi->hdmatx != NULL)
  {
    if (HAL_OK != HAL_DMA_Abort(hspi->hdmatx))
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
      errorcode = HAL_ERROR;
    }
  }
  /* 中止 SPI DMA 接收数据流/通道 */
  if (hspi->hdmarx != NULL)
  {
    if (HAL_OK != HAL_DMA_Abort(hspi->hdmarx))
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
      errorcode = HAL_ERROR;
    }
  }

  /* 禁用 SPI DMA 发送和接收请求 */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
  hspi->State = HAL_SPI_STATE_READY;
  return errorcode;
}

/**
  * @brief  处理 SPI 中断请求。
  * @param  hspi 指向包含指定 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
void HAL_SPI_IRQHandler(SPI_HandleTypeDef *hspi)
{
  uint32_t itsource = hspi->Instance->CR2;
  uint32_t itflag   = hspi->Instance->SR;

  /* SPI 处于接收模式 ----------------------------------------------------*/
  if ((SPI_CHECK_FLAG(itflag, SPI_FLAG_OVR) == RESET) &&
      (SPI_CHECK_FLAG(itflag, SPI_FLAG_RXNE) != RESET) && (SPI_CHECK_IT_SOURCE(itsource, SPI_IT_RXNE) != RESET))
  {
    hspi->RxISR(hspi);
    return;
  }

  /* SPI 发送器模式 -------------------------------------------------*/
  if ((SPI_CHECK_FLAG(itflag, SPI_FLAG_TXE) != RESET) && (SPI_CHECK_IT_SOURCE(itsource, SPI_IT_TXE) != RESET))
  {
    hspi->TxISR(hspi);
    return;
  }

  /* SPI 错误处理 --------------------------------------------------*/
  if (((SPI_CHECK_FLAG(itflag, SPI_FLAG_MODF) != RESET) || (SPI_CHECK_FLAG(itflag, SPI_FLAG_OVR) != RESET))
      && (SPI_CHECK_IT_SOURCE(itsource, SPI_IT_ERR) != RESET))
  {
    /* 发生 SPI 过载错误（Overrun error）中断 ----------------------------------*/
    if (SPI_CHECK_FLAG(itflag, SPI_FLAG_OVR) != RESET)
    {
      if (hspi->State != HAL_SPI_STATE_BUSY_TX)
      {
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_OVR);
        __HAL_SPI_CLEAR_OVRFLAG(hspi);
      }
      else
      {
        __HAL_SPI_CLEAR_OVRFLAG(hspi);
        return;
      }
    }

    /* 发生 SPI 模式故障（Mode Fault）错误中断 -------------------------------*/
    if (SPI_CHECK_FLAG(itflag, SPI_FLAG_MODF) != RESET)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_MODF);
      __HAL_SPI_CLEAR_MODFFLAG(hspi);
    }

    /* 发生 SPI 帧错误（Frame error）中断 ------------------------------------*/

    if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
    {
      /* 禁用所有中断 */
      __HAL_SPI_DISABLE_IT(hspi, SPI_IT_RXNE | SPI_IT_TXE | SPI_IT_ERR);

      hspi->State = HAL_SPI_STATE_READY;
      /* 如果已启用，则禁用 SPI DMA 请求 */
      if ((HAL_IS_BIT_SET(itsource, SPI_CR2_TXDMAEN)) || (HAL_IS_BIT_SET(itsource, SPI_CR2_RXDMAEN)))
      {
        CLEAR_BIT(hspi->Instance->CR2, (SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN));

        /* 中止 SPI DMA Rx 通道 */
        if (hspi->hdmarx != NULL)
        {
          /* 设置 SPI DMA 中止回调函数：
          将在 DMA 中止流程结束时调用 HAL_SPI_ErrorCallback() */
          hspi->hdmarx->XferAbortCallback = SPI_DMAAbortOnError;
          if (HAL_OK != HAL_DMA_Abort_IT(hspi->hdmarx))
          {
            SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
          }
        }
        /* 中止 SPI DMA Tx 通道 */
        if (hspi->hdmatx != NULL)
        {
          /* 设置 SPI DMA 中止回调函数：
          将在 DMA 中止流程结束时调用 HAL_SPI_ErrorCallback() */
          hspi->hdmatx->XferAbortCallback = SPI_DMAAbortOnError;
          if (HAL_OK != HAL_DMA_Abort_IT(hspi->hdmatx))
          {
            SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
          }
        }
      }
      else
      {
        /* 调用用户错误回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
        hspi->ErrorCallback(hspi);
#else
        HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      }
    }
    return;
  }
}

/**
  * @brief  发送传输完成回调函数。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构的指针。
  * @retval 无
  */
__weak void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* 防止未使用参数引起的编译警告 */
  UNUSED(hspi);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            应在用户文件中实现 HAL_SPI_TxCpltCallback
   */
}

/**
  * @brief  接收传输完成回调函数。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hspi);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            应在用户文件中实现 HAL_SPI_RxCpltCallback
   */
}

/**
  * @brief  发送和接收传输完成回调函数。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hspi);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            应在用户文件中实现 HAL_SPI_TxRxCpltCallback
   */
}

/**
  * @brief  发送过半传输完成回调函数。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hspi);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            应在用户文件中实现 HAL_SPI_TxHalfCpltCallback
   */
}

/**
  * @brief  接收过半传输完成回调函数。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hspi);

  /* 注意：不应修改此函数，当需要使用该回调函数时，
            应在用户文件中实现 HAL_SPI_RxHalfCpltCallback()
   */
}

/**
  * @brief  发送和接收过半传输回调函数。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hspi);

  /* 注意：不应修改此函数，当需要该回调函数时，
            应在用户文件中实现 HAL_SPI_TxRxHalfCpltCallback()
   */
}

/**
  * @brief  SPI 错误回调函数。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hspi);

  /* 注意：不应修改此函数，当需要该回调函数时，
            应在用户文件中实现 HAL_SPI_ErrorCallback
   */
  /* 注意：hspi 句柄中的 ErrorCode 参数由 SPI 处理进程更新，
            用户可使用 HAL_SPI_GetError() API 检查最近发生的错误
   */
}

/**
  * @brief  SPI 中止完成回调函数。
  * @param  hspi SPI 句柄。
  * @retval 无
  */
__weak void HAL_SPI_AbortCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hspi);

  /* 注意：不应修改此函数，当需要该回调函数时，
            可在用户文件中实现 HAL_SPI_AbortCpltCallback。
   */
}

/**
  * @}
  */

/** @defgroup SPI_Exported_Functions_Group3 外设状态和错误函数
  * @brief   SPI 控制函数
  *
@verbatim
 ===============================================================================
                      ##### 外设状态和错误函数 #####
 ===============================================================================
    [..]
    本小节提供了一组用于控制 SPI 的函数。
     (+) HAL_SPI_GetState() API 可用于在运行时检查 SPI 外设的状态
     (+) HAL_SPI_GetError() 用于在运行时检查通信期间发生的错误
@endverbatim
  * @{
  */

/**
  * @brief  返回 SPI 句柄状态。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval SPI 状态
  */
HAL_SPI_StateTypeDef HAL_SPI_GetState(const SPI_HandleTypeDef *hspi)
{
  /* 返回 SPI 句柄状态 */
  return hspi->State;
}

/**
  * @brief  返回 SPI 错误代码。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval 位图格式的 SPI 错误代码
  */
uint32_t HAL_SPI_GetError(const SPI_HandleTypeDef *hspi)
{
  /* 返回 SPI ErrorCode */
  return hspi->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup SPI_Private_Functions
  * @brief   私有函数
  * @{
  */

/**
  * @brief  SPI DMA 发送过程完成回调函数。
  * @param  hdma 指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_DMATransmitCplt(DMA_HandleTypeDef *hdma)
{
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent);
  uint32_t tickstart;

  /* 初始化 tickstart 用于超时管理 */
  tickstart = HAL_GetTick();

  /* DMA 正常模式 */
  if ((hdma->Instance->CCR & DMA_CCR_CIRC) != DMA_CCR_CIRC)
  {
    /* 禁用 ERR 中断 */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_ERR);

    /* 禁用发送 DMA 请求 */
    CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN);

    /* 检查传输是否结束 */
    if (SPI_EndRxTxTransaction(hspi, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
    }

    /* 在双线通信模式下清除过载（overrun）标志，因为未读取接收到的数据 */
    if (hspi->Init.Direction == SPI_DIRECTION_2LINES)
    {
      __HAL_SPI_CLEAR_OVRFLAG(hspi);
    }

    hspi->TxXferCount = 0U;
    hspi->State = HAL_SPI_STATE_READY;

    if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
    {
      /* 调用用户错误回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
      hspi->ErrorCallback(hspi);
#else
      HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      return;
    }
  }
  /* 调用用户发送完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  hspi->TxCpltCallback(hspi);
#else
  HAL_SPI_TxCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  SPI DMA 接收过程完成回调函数。
  * @param  hdma 指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_DMAReceiveCplt(DMA_HandleTypeDef *hdma)
{
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent);
  uint32_t tickstart;
#if (USE_SPI_CRC != 0U)
  __IO uint32_t tmpreg = 0U;
#endif /* USE_SPI_CRC */

  /* 初始化 tickstart 以进行超时管理 */
  tickstart = HAL_GetTick();

  /* DMA 正常模式 */
  if ((hdma->Instance->CCR & DMA_CCR_CIRC) != DMA_CCR_CIRC)
  {
    /* 禁用 ERR 中断 */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_ERR);

#if (USE_SPI_CRC != 0U)
    /* CRC 处理 */
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
    {
      /* 等待 RXNE 标志置位 */
      if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)
      {
        /* CRC 接收错误 */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);
      }
      /* 读取 CRC */
      tmpreg = READ_REG(hspi->Instance->DR);
      /* 避免 GCC 警告 */
      UNUSED(tmpreg);
    }
#endif /* USE_SPI_CRC */

    /* 检查是否处于主机双线接收模式 (Master RX 2 line mode) */
    if ((hspi->Init.Direction == SPI_DIRECTION_2LINES) && (hspi->Init.Mode == SPI_MODE_MASTER))
    {
      /* 禁用 Rx/Tx DMA 请求（默认执行，以处理主机双线接收方向的情况） */
      CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
    }
    else
    {
      /* 正常情况 */
      CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_RXDMAEN);
    }

    /* 检查传输是否结束 */
    if (SPI_EndRxTransaction(hspi, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)
    {
      hspi->ErrorCode = HAL_SPI_ERROR_FLAG;
    }

    hspi->RxXferCount = 0U;
    hspi->State = HAL_SPI_STATE_READY;

#if (USE_SPI_CRC != 0U)
    /* 检查是否发生 CRC 错误 */
    if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR) != RESET)
    {
      /* 检查 CRC 错误是否有效（是否需要应用变通方案） */
      if (SPI_ISCRCErrorValid(hspi) == SPI_VALID_CRC_ERROR)
      {
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);

        /* 复位 CRC 计算 */
        SPI_RESET_CRC(hspi);
      }
      else
      {
        __HAL_SPI_CLEAR_CRCERRFLAG(hspi);
      }
    }
#endif /* USE_SPI_CRC */

    if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
    {
      /* 调用用户错误回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
      hspi->ErrorCallback(hspi);
#else
      HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      return;
    }
  }
  /* 调用用户接收完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  hspi->RxCpltCallback(hspi);
#else
  HAL_SPI_RxCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA SPI 发送和接收处理完成回调函数。
  * @param  hdma 指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_DMATransmitReceiveCplt(DMA_HandleTypeDef *hdma)
{
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent);
  uint32_t tickstart;
#if (USE_SPI_CRC != 0U)
  __IO uint32_t tmpreg = 0U;
#endif /* USE_SPI_CRC */

  /* 初始化 tickstart 用于超时管理 */
  tickstart = HAL_GetTick();

  /* DMA 正常模式 */
  if ((hdma->Instance->CCR & DMA_CCR_CIRC) != DMA_CCR_CIRC)
  {
    /* 禁用 ERR 中断 */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_ERR);

#if (USE_SPI_CRC != 0U)
    /* CRC 处理 */
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
    {
      /* 等待 CRC 数据 */
      if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)
      {
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);
      }
      /* 读取 CRC 以清空 DR 寄存器和 RXNE 标志 */
      tmpreg = READ_REG(hspi->Instance->DR);
      /* 避免 GCC 编译器警告 */
      UNUSED(tmpreg);
    }
#endif /* USE_SPI_CRC */

    /* 检查传输事务是否结束 */
    if (SPI_EndRxTxTransaction(hspi, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
    }

    /* 禁用 Rx/Tx DMA 请求 */
    CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);

    hspi->TxXferCount = 0U;
    hspi->RxXferCount = 0U;
    hspi->State = HAL_SPI_STATE_READY;

#if (USE_SPI_CRC != 0U)
    /* 检查是否发生 CRC 错误 */
    if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR) != RESET)
    {
      /* 检查 CRC 错误是否有效（判断是否需要应用变通方案/规避措施） */
      if (SPI_ISCRCErrorValid(hspi) == SPI_VALID_CRC_ERROR)
      {
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);

        /* 复位 CRC 计算 */
        SPI_RESET_CRC(hspi);
      }
      else
      {
        __HAL_SPI_CLEAR_CRCERRFLAG(hspi);
      }
    }
#endif /* USE_SPI_CRC */

    if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
    {
      /* 调用用户错误回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
      hspi->ErrorCallback(hspi);
#else
      HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      return;
    }
  }
  /* 调用用户收发完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  hspi->TxRxCpltCallback(hspi);
#else
  HAL_SPI_TxRxCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA SPI 发送过半完成回调函数。
  * @param  hdma 指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_DMAHalfTransmitCplt(DMA_HandleTypeDef *hdma)
{
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent);

  /* 调用用户发送过半完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  hspi->TxHalfCpltCallback(hspi);
#else
  HAL_SPI_TxHalfCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA SPI 接收过半完成回调函数
  * @param  hdma 指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_DMAHalfReceiveCplt(DMA_HandleTypeDef *hdma)
{
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent);

  /* 调用用户接收过半完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  hspi->RxHalfCpltCallback(hspi);
#else
  HAL_SPI_RxHalfCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA SPI 发送与接收过半完成回调函数。
  * @param  hdma 指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_DMAHalfTransmitReceiveCplt(DMA_HandleTypeDef *hdma)
{
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent);

  /* 调用用户发送与接收过半完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  hspi->TxRxHalfCpltCallback(hspi);
#else
  HAL_SPI_TxRxHalfCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA SPI 通信错误回调函数。
  * @param  hdma 指向包含指定 DMA 模块配置信息的 DMA_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_DMAError(DMA_HandleTypeDef *hdma)
{
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent);

  /* 停止 SPI 侧禁用的 DMA 传输 */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);

  SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
  hspi->State = HAL_SPI_STATE_READY;
  /* 调用用户错误回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  hspi->ErrorCallback(hspi);
#else
  HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA SPI 通信中止回调函数，由 HAL 服务在发生错误时发起
  *         （在发生错误并执行完 DMA 中止流程后调用）。
  * @param  hdma DMA 句柄。
  * @retval 无
  */
static void SPI_DMAAbortOnError(DMA_HandleTypeDef *hdma)
{
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent);
  hspi->RxXferCount = 0U;
  hspi->TxXferCount = 0U;

  /* 调用用户错误回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  hspi->ErrorCallback(hspi);
#else
  HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA SPI 发送通信中止回调函数，由用户发起
  *         （在响应用户中止请求并执行完 DMA 发送中止流程后调用）。
  * @note   执行此回调时，仅当 Rx DMA 句柄没有正在进行的中止操作时，
  *         才会调用用户中止完成回调函数。
  * @param  hdma DMA 句柄。
  * @retval 无
  */
static void SPI_DMATxAbortCallback(DMA_HandleTypeDef *hdma)
{
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent);
  __IO uint32_t count;

  hspi->hdmatx->XferAbortCallback = NULL;
  count = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);

  /* 禁用发送 DMA 请求 */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_TXDMAEN);

  /* 等待直到 TXE 标志位置位 */
  do
  {
    if (count == 0U)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
      break;
    }
    count--;
  } while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET);

  /* 检查中止（Abort）流程是否仍在进行中 */
  if (hspi->hdmarx != NULL)
  {
    if (hspi->hdmarx->XferAbortCallback != NULL)
    {
      return;
    }
  }

  /* 中止流程已全部结束：所有 DMA 数据流/通道均已中止，调用用户中止完成回调函数 */
  hspi->RxXferCount = 0U;
  hspi->TxXferCount = 0U;

  /* 检查中止流程期间是否有错误发生 */
  if (hspi->ErrorCode != HAL_SPI_ERROR_ABORT)
  {
    /* 复位错误代码（errorCode） */
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;
  }

  /* 清除 SR 寄存器中的错误标志位 */
  __HAL_SPI_CLEAR_OVRFLAG(hspi);

  /* 将 hspi->State 恢复为就绪（Ready）状态 */
  hspi->State  = HAL_SPI_STATE_READY;

  /* 调用用户中止完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  hspi->AbortCpltCallback(hspi);
#else
  HAL_SPI_AbortCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  由用户发起的 DMA SPI 接收通信中止回调函数
  *         （在响应用户中止请求的 DMA 接收中止流程结束时调用）。
  * @note   执行此回调时，仅当发送 DMA 句柄上没有仍在进行中的
  *         中止流程时，才会调用用户中止完成回调函数。
  * @param  hdma DMA 句柄。
  * @retval 无
  */
static void SPI_DMARxAbortCallback(DMA_HandleTypeDef *hdma)
{
  SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)(((DMA_HandleTypeDef *)hdma)->Parent);

  /* 禁用 SPI 外设 */
  __HAL_SPI_DISABLE(hspi);

  hspi->hdmarx->XferAbortCallback = NULL;

  /* 禁用接收 DMA 请求 */
  CLEAR_BIT(hspi->Instance->CR2, SPI_CR2_RXDMAEN);

  /* 检查忙标志位（Busy flag） */
  if (SPI_EndRxTxTransaction(hspi, SPI_DEFAULT_TIMEOUT, HAL_GetTick()) != HAL_OK)
  {
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
  }

  /* 检查中止（Abort）流程是否仍在进行中 */
  if (hspi->hdmatx != NULL)
  {
    if (hspi->hdmatx->XferAbortCallback != NULL)
    {
      return;
    }
  }

  /* 中止流程已全部结束：所有 DMA 数据流/通道均已中止，调用用户中止完成回调函数 */
  hspi->RxXferCount = 0U;
  hspi->TxXferCount = 0U;

  /* 检查中止流程期间是否有错误发生 */
  if (hspi->ErrorCode != HAL_SPI_ERROR_ABORT)
  {
    /* 复位 errorCode */
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;
  }

  /* 清除 SR 寄存器中的错误标志位 */
  __HAL_SPI_CLEAR_OVRFLAG(hspi);

  /* 将 hspi->State 恢复为就绪（Ready）状态 */
  hspi->State  = HAL_SPI_STATE_READY;

  /* 调用用户中止完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  hspi->AbortCpltCallback(hspi);
#else
  HAL_SPI_AbortCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  中断模式下发送和接收的 8 位 Rx 处理函数。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_2linesRxISR_8BIT(struct __SPI_HandleTypeDef *hspi)
{
  /* 以 8 位模式接收数据 */
  *hspi->pRxBuffPtr = *((__IO uint8_t *)&hspi->Instance->DR);
  hspi->pRxBuffPtr++;
  hspi->RxXferCount--;

  /* 检查接收是否结束 */
  if (hspi->RxXferCount == 0U)
  {
#if (USE_SPI_CRC != 0U)
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
    {
      hspi->RxISR =  SPI_2linesRxISR_8BITCRC;
      return;
    }
#endif /* USE_SPI_CRC */

    /* 禁用 RXNE 和 ERR 中断 */
    __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));

    if (hspi->TxXferCount == 0U)
    {
      SPI_CloseRxTx_ISR(hspi);
    }
  }
}

#if (USE_SPI_CRC != 0U)
/**
  * @brief  中断模式下发送和接收的 8 位 Rx 处理函数。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_2linesRxISR_8BITCRC(struct __SPI_HandleTypeDef *hspi)
{
  __IO uint8_t  *ptmpreg8;
  __IO uint8_t  tmpreg8 = 0;

  /* 初始化 8 位临时指针 */
  ptmpreg8 = (__IO uint8_t *)&hspi->Instance->DR;
  /* 读取 8 位 CRC 以清空数据寄存器 */
  tmpreg8 = *ptmpreg8;
  /* 避免 GCC 警告 */
  UNUSED(tmpreg8);

  /* 禁用 RXNE 和 ERR 中断 */
  __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));

  if (hspi->TxXferCount == 0U)
  {
    SPI_CloseRxTx_ISR(hspi);
  }
}
#endif /* USE_SPI_CRC */

/**
  * @brief  中断模式下发送和接收的 8 位发送处理函数。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_2linesTxISR_8BIT(struct __SPI_HandleTypeDef *hspi)
{
  *(__IO uint8_t *)&hspi->Instance->DR = *((const uint8_t *)hspi->pTxBuffPtr);
  hspi->pTxBuffPtr++;
  hspi->TxXferCount--;

  /* 检查传输是否结束 */
  if (hspi->TxXferCount == 0U)
  {
#if (USE_SPI_CRC != 0U)
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
    {
      /* 设置 CRCNEXT 位以发送 CRC */
      SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
      /* 禁用 TXE 中断 */
      __HAL_SPI_DISABLE_IT(hspi, SPI_IT_TXE);
      return;
    }
#endif /* USE_SPI_CRC */

    /* 禁用 TXE 中断 */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_TXE);

    if (hspi->RxXferCount == 0U)
    {
      SPI_CloseRxTx_ISR(hspi);
    }
  }
}

/**
  * @brief  中断模式下发送和接收的 16 位接收处理函数。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_2linesRxISR_16BIT(struct __SPI_HandleTypeDef *hspi)
{
  /* 在 16 位模式下接收数据 */
  *((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)(hspi->Instance->DR);
  hspi->pRxBuffPtr += sizeof(uint16_t);
  hspi->RxXferCount--;

  if (hspi->RxXferCount == 0U)
  {
#if (USE_SPI_CRC != 0U)
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
    {
      hspi->RxISR =  SPI_2linesRxISR_16BITCRC;
      return;
    }
#endif /* USE_SPI_CRC */

    /* 禁用 RXNE 中断 */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_RXNE);

    if (hspi->TxXferCount == 0U)
    {
      SPI_CloseRxTx_ISR(hspi);
    }
  }
}

#if (USE_SPI_CRC != 0U)
/**
  * @brief  管理中断模式下发送和接收的 16 位 CRC 接收。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_2linesRxISR_16BITCRC(struct __SPI_HandleTypeDef *hspi)
{
  __IO uint32_t tmpreg = 0U;

  /* 读取 16 位 CRC 以清空数据寄存器 */
  tmpreg = READ_REG(hspi->Instance->DR);
  /* 避免 GCC 编译器警告 */
  UNUSED(tmpreg);

  /* 禁用 RXNE 中断 */
  __HAL_SPI_DISABLE_IT(hspi, SPI_IT_RXNE);

  SPI_CloseRxTx_ISR(hspi);
}
#endif /* USE_SPI_CRC */

/**
  * @brief  中断模式下同时发送与接收的 16 位发送处理函数。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_2linesTxISR_16BIT(struct __SPI_HandleTypeDef *hspi)
{
  /* 以 16 位模式发送数据 */
  hspi->Instance->DR = *((const uint16_t *)hspi->pTxBuffPtr);
  hspi->pTxBuffPtr += sizeof(uint16_t);
  hspi->TxXferCount--;

  /* 使能 CRC 传输 */
  if (hspi->TxXferCount == 0U)
  {
#if (USE_SPI_CRC != 0U)
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
    {
      /* 置位 CRCNEXT 位以发送 CRC */
      SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
      /* 禁用 TXE 中断 */
      __HAL_SPI_DISABLE_IT(hspi, SPI_IT_TXE);
      return;
    }
#endif /* USE_SPI_CRC */

    /* 禁用 TXE 中断 */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_TXE);

    if (hspi->RxXferCount == 0U)
    {
      SPI_CloseRxTx_ISR(hspi);
    }
  }
}

#if (USE_SPI_CRC != 0U)
/**
  * @brief  在中断上下文中处理 8 位 CRC 接收。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_RxISR_8BITCRC(struct __SPI_HandleTypeDef *hspi)
{
  __IO uint8_t  *ptmpreg8;
  __IO uint8_t  tmpreg8 = 0;

  /* 初始化 8 位临时指针 */
  ptmpreg8 = (__IO uint8_t *)&hspi->Instance->DR;
  /* 读取 8 位 CRC 以清空数据寄存器 */
  tmpreg8 = *ptmpreg8;
  /* 避免 GCC 编译器警告 */
  UNUSED(tmpreg8);

  SPI_CloseRx_ISR(hspi);
}
#endif /* USE_SPI_CRC */

/**
  * @brief  在中断上下文中处理 8 位数据接收。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_RxISR_8BIT(struct __SPI_HandleTypeDef *hspi)
{
  *hspi->pRxBuffPtr = (*(__IO uint8_t *)&hspi->Instance->DR);
  hspi->pRxBuffPtr++;
  hspi->RxXferCount--;

#if (USE_SPI_CRC != 0U)
  /* 使能 CRC 传输 */
  if ((hspi->RxXferCount == 1U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE))
  {
    SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
  }
  /* 检查硬件是否已成功复位 CRCNEXT */
  if (READ_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT))
  {
    /* 规避方案：若硬件未自动复位 CRCNEXT，则强制将 CRCNEXT 位置零 */
    CLEAR_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
  }

#endif /* USE_SPI_CRC */

  if (hspi->RxXferCount == 0U)
  {
#if (USE_SPI_CRC != 0U)
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
    {
      hspi->RxISR =  SPI_RxISR_8BITCRC;
      return;
    }
#endif /* USE_SPI_CRC */
    SPI_CloseRx_ISR(hspi);
  }
}

#if (USE_SPI_CRC != 0U)
/**
  * @brief  在中断上下文中处理 16 位 CRC 接收。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_RxISR_16BITCRC(struct __SPI_HandleTypeDef *hspi)
{
  __IO uint32_t tmpreg = 0U;

  /* 读取 16 位 CRC 以清空数据寄存器 */
  tmpreg = READ_REG(hspi->Instance->DR);
  /* 用于消除 GCC 警告 */
  UNUSED(tmpreg);

  /* 禁用 RXNE 和 ERR 中断 */
  __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));

  SPI_CloseRx_ISR(hspi);
}
#endif /* USE_SPI_CRC */

/**
  * @brief  在中断上下文中处理 16 位数据接收。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_RxISR_16BIT(struct __SPI_HandleTypeDef *hspi)
{
  *((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)(hspi->Instance->DR);
  hspi->pRxBuffPtr += sizeof(uint16_t);
  hspi->RxXferCount--;

#if (USE_SPI_CRC != 0U)
  /* 使能 CRC 传输 */
  if ((hspi->RxXferCount == 1U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE))
  {
    SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
  }
  /* 检查硬件是否已成功复位 CRCNEXT */
  if (READ_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT))
  {
    /* 规避方案：若硬件未自动复位 CRCNEXT，则强制将 CRCNEXT 位置零 */
    CLEAR_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
  }

#endif /* USE_SPI_CRC */

  if (hspi->RxXferCount == 0U)
  {
#if (USE_SPI_CRC != 0U)
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
    {
      hspi->RxISR = SPI_RxISR_16BITCRC;
      return;
    }
#endif /* USE_SPI_CRC */
    SPI_CloseRx_ISR(hspi);
  }
}

/**
  * @brief  在中断模式下处理 8 位数据发送。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_TxISR_8BIT(struct __SPI_HandleTypeDef *hspi)
{
  *(__IO uint8_t *)&hspi->Instance->DR = *((const uint8_t *)hspi->pTxBuffPtr);
  hspi->pTxBuffPtr++;
  hspi->TxXferCount--;

  if (hspi->TxXferCount == 0U)
  {
#if (USE_SPI_CRC != 0U)
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
    {
      /* 使能 CRC 发送 */
      SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
    }
#endif /* USE_SPI_CRC */
    SPI_CloseTx_ISR(hspi);
  }
}

/**
  * @brief  在中断模式下处理 16 位数据发送。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *               SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_TxISR_16BIT(struct __SPI_HandleTypeDef *hspi)
{
  /* 以 16 位模式发送数据 */
  hspi->Instance->DR = *((const uint16_t *)hspi->pTxBuffPtr);
  hspi->pTxBuffPtr += sizeof(uint16_t);
  hspi->TxXferCount--;

  if (hspi->TxXferCount == 0U)
  {
#if (USE_SPI_CRC != 0U)
    if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
    {
      /* 使能 CRC 发送 */
      SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
    }
#endif /* USE_SPI_CRC */
    SPI_CloseTx_ISR(hspi);
  }
}

/**
  * @brief  处理 SPI 通信超时。
  * @param  hspi 指向包含 SPI 模块配置信息的
  *              SPI_HandleTypeDef 结构体的指针。
  * @param  Flag 要检查的 SPI 标志
  * @param  State 要检查的标志状态
  * @param  Timeout 超时持续时间
  * @param  Tickstart 滴答定时器起始值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus State,
                                                       uint32_t Timeout, uint32_t Tickstart)
{
  __IO uint32_t count;
  uint32_t tmp_timeout;
  uint32_t tmp_tickstart;

  /* 传输结束时调整超时时间 */
  tmp_timeout   = Timeout - (HAL_GetTick() - Tickstart);
  tmp_tickstart = HAL_GetTick();

  /* 基于软件循环计算超时，以避免 SysTick 被禁用时出现阻塞问题 */
  count = tmp_timeout * ((SystemCoreClock * 32U) >> 20U);

  while ((__HAL_SPI_GET_FLAG(hspi, Flag) ? SET : RESET) != State)
  {
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - tmp_tickstart) >= tmp_timeout) || (tmp_timeout == 0U))
      {
        /* 禁用 SPI 并复位 CRC：主设备和从设备端均应清除 CRC 值，
           以便重新同步主设备和从设备各自的 CRC 计算 */

        /* 针对中断处理流程，禁用 TXE、RXNE 和 ERR 中断 */
        __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

        if ((hspi->Init.Mode == SPI_MODE_MASTER) && ((hspi->Init.Direction == SPI_DIRECTION_1LINE)
                                                     || (hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY)))
        {
          /* 禁用 SPI 外设 */
          __HAL_SPI_DISABLE(hspi);
        }

        /* 复位 CRC 计算 */
        if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
        {
          SPI_RESET_CRC(hspi);
        }

        hspi->State = HAL_SPI_STATE_READY;

        /* 进程解锁 */
        __HAL_UNLOCK(hspi);

        return HAL_TIMEOUT;
      }
      /* 如果 Systick 被禁用或未递增，则关闭超时机制以进入禁用循环流程 */
      if (count == 0U)
      {
        tmp_timeout = 0U;
      }
      else
      {
        count--;
      }
    }
  }

  return HAL_OK;
}

/**
  * @brief  处理 RX 传输完成检查。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @param  Timeout 超时时间
  * @param  Tickstart 初始滴答计数值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef SPI_EndRxTransaction(SPI_HandleTypeDef *hspi,  uint32_t Timeout, uint32_t Tickstart)
{
  if ((hspi->Init.Mode == SPI_MODE_MASTER) && ((hspi->Init.Direction == SPI_DIRECTION_1LINE)
                                               || (hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY)))
  {
    /* 禁用 SPI 外设 */
    __HAL_SPI_DISABLE(hspi);
  }

  if ((hspi->Init.Mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY))
  {
    /* 等待 RXNE 复位 */
    if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, RESET, Timeout, Tickstart) != HAL_OK)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
      return HAL_TIMEOUT;
    }
  }
  else
  {
    /* 检查 BSY 标志位 */
    if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_BSY, RESET, Timeout, Tickstart) != HAL_OK)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
      return HAL_TIMEOUT;
    }
  }
  return HAL_OK;
}

/**
  * @brief  处理 RXTX 或 TX 传输完成检查。
  * @param  hspi SPI 句柄
  * @param  Timeout 超时时间
  * @param  Tickstart 初始滴答计数值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef SPI_EndRxTxTransaction(SPI_HandleTypeDef *hspi, uint32_t Timeout, uint32_t Tickstart)
{
  /* 等待 TXE 标志位置位 */
  if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_TXE, SET, Timeout, Tickstart) != HAL_OK)
  {
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
    return HAL_TIMEOUT;
  }

  /* 检查 BSY 标志位 */
  if (SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_BSY, RESET, Timeout, Tickstart) != HAL_OK)
  {
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
    return HAL_TIMEOUT;
  }
  return HAL_OK;
}

/**
  * @brief  处理 RXTX 传输结束。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_CloseRxTx_ISR(SPI_HandleTypeDef *hspi)
{
  uint32_t tickstart;
  __IO uint32_t count = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);

  /* 初始化超时管理所用的 tickstart */
  tickstart = HAL_GetTick();

  /* 禁用 ERR 中断 */
  __HAL_SPI_DISABLE_IT(hspi, SPI_IT_ERR);

  /* 等待 TXE 标志位置位 */
  do
  {
    if (count == 0U)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
      break;
    }
    count--;
  } while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET);

  /* 检查传输是否结束 */
  if (SPI_EndRxTxTransaction(hspi, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)
  {
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
  }

  /* 在双线通信模式下清除过载（overrun）标志，因为接收到的数据未被读取 */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)
  {
    __HAL_SPI_CLEAR_OVRFLAG(hspi);
  }

#if (USE_SPI_CRC != 0U)
  /* 检查是否发生 CRC 错误 */
  if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR) != RESET)
  {
    /* 检查 CRC 错误是否有效（是否需要应用变通方案） */
    if (SPI_ISCRCErrorValid(hspi) == SPI_VALID_CRC_ERROR)
    {
      hspi->State = HAL_SPI_STATE_READY;
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);

      /* 复位 CRC 计算 */
      SPI_RESET_CRC(hspi);

      /* 调用用户错误回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
      hspi->ErrorCallback(hspi);
#else
      HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    }
    else
    {
      __HAL_SPI_CLEAR_CRCERRFLAG(hspi);
    }
  }
  else
  {
#endif /* USE_SPI_CRC */
    if (hspi->ErrorCode == HAL_SPI_ERROR_NONE)
    {
      if (hspi->State == HAL_SPI_STATE_BUSY_RX)
      {
        hspi->State = HAL_SPI_STATE_READY;
        /* 调用用户接收完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
        hspi->RxCpltCallback(hspi);
#else
        HAL_SPI_RxCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      }
      else
      {
        hspi->State = HAL_SPI_STATE_READY;
        /* 调用用户发送与接收完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
        hspi->TxRxCpltCallback(hspi);
#else
        HAL_SPI_TxRxCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      }
    }
    else
    {
      hspi->State = HAL_SPI_STATE_READY;
      /* 调用用户错误回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
      hspi->ErrorCallback(hspi);
#else
      HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    }
#if (USE_SPI_CRC != 0U)
  }
#endif /* USE_SPI_CRC */
}

/**
  * @brief  处理接收传输结束事件。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_CloseRx_ISR(SPI_HandleTypeDef *hspi)
{
  /* 禁用 RXNE 和 ERR 中断 */
  __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));

  /* 检查传输是否结束 */
  if (SPI_EndRxTransaction(hspi, SPI_DEFAULT_TIMEOUT, HAL_GetTick()) != HAL_OK)
  {
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
  }

  /* 在双线通信模式下清除过载（overrun）标志，因为接收到的数据未被读取 */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)
  {
    __HAL_SPI_CLEAR_OVRFLAG(hspi);
  }
  hspi->State = HAL_SPI_STATE_READY;

#if (USE_SPI_CRC != 0U)
  /* 检查是否发生 CRC 错误 */
  if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR) != RESET)
  {
    /* 检查 CRC 错误是否有效（是否需要应用变通方案/workaround） */
    if (SPI_ISCRCErrorValid(hspi) == SPI_VALID_CRC_ERROR)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);

      /* 复位 CRC 计算 */
      SPI_RESET_CRC(hspi);

      /* 调用用户错误回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
      hspi->ErrorCallback(hspi);
#else
      HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    }
    else
    {
      __HAL_SPI_CLEAR_CRCERRFLAG(hspi);
    }
  }
  else
  {
#endif /* USE_SPI_CRC */
    if (hspi->ErrorCode == HAL_SPI_ERROR_NONE)
    {
      /* 调用用户接收完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
      hspi->RxCpltCallback(hspi);
#else
      HAL_SPI_RxCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    }
    else
    {
      /* 调用用户错误回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
      hspi->ErrorCallback(hspi);
#else
      HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    }
#if (USE_SPI_CRC != 0U)
  }
#endif /* USE_SPI_CRC */
}

/**
  * @brief  处理 TX 传输事务结束。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构的指针。
  * @retval 无
  */
static void SPI_CloseTx_ISR(SPI_HandleTypeDef *hspi)
{
  uint32_t tickstart;
  __IO uint32_t count = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);

  /* 初始化用于超时管理的 tickstart */
  tickstart = HAL_GetTick();

  /* 等待直到 TXE 标志置位 */
  do
  {
    if (count == 0U)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
      break;
    }
    count--;
  } while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET);

  /* 禁用 TXE 和 ERR 中断 */
  __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_ERR));

  /* 检查传输是否结束 */
  if (SPI_EndRxTxTransaction(hspi, SPI_DEFAULT_TIMEOUT, tickstart) != HAL_OK)
  {
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
  }

  /* 在双线通信模式下清除过载标志（OVR），因为未读取接收到的数据 */
  if (hspi->Init.Direction == SPI_DIRECTION_2LINES)
  {
    __HAL_SPI_CLEAR_OVRFLAG(hspi);
  }

  hspi->State = HAL_SPI_STATE_READY;
  if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
  {
    /* 调用用户错误回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    hspi->ErrorCallback(hspi);
#else
    HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
  }
  else
  {
    /* 调用用户接收完成回调函数 */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    hspi->TxCpltCallback(hspi);
#else
    HAL_SPI_TxCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
  }
}

/**
  * @brief  处理中止 Rx 传输事务。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_AbortRx_ISR(SPI_HandleTypeDef *hspi)
{
  __IO uint32_t tmpreg = 0U;
  __IO uint32_t count = SPI_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);

  /* 等待直到 TXE 标志位置位 */
  do
  {
    if (count == 0U)
    {
      SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
      break;
    }
    count--;
  } while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET);

  /* 禁用 SPI 外设 */
  __HAL_SPI_DISABLE(hspi);

  /* 禁用 TXEIE、RXNEIE 和 ERRIE（模式错误事件、过载错误、TI 帧格式错误）中断 */
  CLEAR_BIT(hspi->Instance->CR2, (SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE));

  /* 通过一次空读操作刷新数据寄存器（DR） */
  tmpreg = READ_REG(hspi->Instance->DR);
  /* 用于避免 GCC 编译器警告 */
  UNUSED(tmpreg);

  hspi->State = HAL_SPI_STATE_ABORT;
}

/**
  * @brief  处理中止 Tx 或 Rx/Tx 传输事务。
  * @param  hspi 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval 无
  */
static void SPI_AbortTx_ISR(SPI_HandleTypeDef *hspi)
{
  /* 禁用 TXEIE 中断 */
  CLEAR_BIT(hspi->Instance->CR2, (SPI_CR2_TXEIE));

  /* 禁用 SPI 外设 */
  __HAL_SPI_DISABLE(hspi);

  hspi->State = HAL_SPI_STATE_ABORT;
}

#if (USE_SPI_CRC != 0U)
/**
  * @brief  根据 SPI 实例、器件类型以及修订版本 ID，检查所遇到的 CRC 错误是否可能属于误检错误。
  * @param  hspi: 指向包含 SPI 模块配置信息的 SPI_HandleTypeDef 结构体的指针。
  * @retval CRC 错误有效性 (SPI_INVALID_CRC_ERROR 或 SPI_VALID_CRC_ERROR)。
  */
uint8_t SPI_ISCRCErrorValid(SPI_HandleTypeDef *hspi)
{
#if defined(SPI_CRC_ERROR_WORKAROUND_FEATURE) && (USE_SPI_CRC_ERROR_WORKAROUND != 0U)
  /* 检查如何处理此 CRC 错误（是否需要应用变通方案） */
  /* 如果 CRC 错误可能存在误报（STM32F10xxC/D/E 芯片勘误表 ES 中的问题 2.15.2
     (DocID14732 Rev 13)） */
  if (hspi->Instance == SPI2)
  {
    if (hspi->Instance->RXCRCR == 0U)
    {
      return (SPI_INVALID_CRC_ERROR);
    }
  }
#endif /* USE_SPI_CRC_ERROR_WORKAROUND */
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hspi);

  return (SPI_VALID_CRC_ERROR);
}
#endif /* USE_SPI_CRC */
/**
  * @}
  */

#endif /* HAL_SPI_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */


/**
  ******************************************************************************
  * @file    stm32f1xx_hal_i2c.c
  * @author  MCD 应用团队
  * @brief   I2C HAL 模块驱动程序。
  *          本文件提供了用于管理集成电路间总线 (I2C) 外设以下功能的固件函数：
  *           + 初始化与反初始化函数
  *           + IO 操作函数
  *           + 外设状态、模式与错误管理函数
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * 本软件根据可在该软件组件根目录下的 LICENSE 文件中找到的条款获得许可。
  * 如果该软件没有随附 LICENSE 文件，则按“原样”提供。
  *
  ******************************************************************************
  @verbatim
  ==============================================================================
                        ##### 如何使用本驱动程序 #####
  ==============================================================================
  [..]
    I2C HAL 驱动程序的使用方法如下：

    (#) 声明一个 I2C_HandleTypeDef 句柄结构体，例如：
        I2C_HandleTypeDef  hi2c;

    (#) 通过实现 HAL_I2C_MspInit() API 来初始化 I2C 底层资源：
        (##) 使能 I2Cx 接口时钟
        (##) I2C 引脚配置
            (+++) 使能 I2C GPIO 时钟
            (+++) 将 I2C 引脚配置为复用开漏模式 (alternate function open-drain)
        (##) 若需使用中断处理，则进行 NVIC 配置
            (+++) 配置 I2Cx 中断优先级
            (+++) 使能 NVIC I2C IRQ 通道
        (##) 若需使用 DMA 处理，则进行 DMA 配置
            (+++) 声明用于发送或接收通道的 DMA_HandleTypeDef 句柄结构体
            (+++) 使能 DMAx 接口时钟
            (+++) 配置 DMA 句柄参数
            (+++) 配置 DMA 发送 (Tx) 或接收 (Rx) 通道
            (+++) 将初始化后的 DMA 句柄与 hi2c 的 DMA Tx 或 Rx 句柄关联
            (+++) 配置 DMA Tx 或 Rx 通道传输完成中断的优先级并使能 NVIC

    (#) 在 hi2c 的 Init 结构体中配置通信速度 (Communication Speed)、占空比 (Duty cycle)、
        寻址模式 (Addressing mode)、自身地址1 (Own Address1)、双重寻址模式 (Dual Addressing mode)、
        自身地址2 (Own Address2)、广播寻址 (General call) 以及禁止时钟延长模式 (Nostretch mode)。

    (#) 调用 HAL_I2C_Init() 初始化 I2C 寄存器，该函数还会通过调用自定义的
        HAL_I2C_MspInit() API 配置底层硬件（GPIO、CLOCK、NVIC 等）。

    (#) 如需检测目标设备是否准备好进行通信，使用函数 HAL_I2C_IsDeviceReady()

    (#) 对于 I2C IO 以及 IO 内存操作，本驱动程序提供了三种操作模式：

    *** 轮询模式 IO 操作 ***
    =================================
    [..]
      (+) 主机模式下使用 HAL_I2C_Master_Transmit() 以阻塞模式发送指定数量的数据
      (+) 主机模式下使用 HAL_I2C_Master_Receive() 以阻塞模式接收指定数量的数据
      (+) 从机模式下使用 HAL_I2C_Slave_Transmit() 以阻塞模式发送指定数量的数据
      (+) 从机模式下使用 HAL_I2C_Slave_Receive() 以阻塞模式接收指定数量的数据

    *** 轮询模式 IO 内存操作 ***
    =====================================
    [..]
      (+) 使用 HAL_I2C_Mem_Write() 以阻塞模式向指定内存地址写入指定数量的数据
      (+) 使用 HAL_I2C_Mem_Read() 以阻塞模式从指定内存地址读取指定数量的数据


    *** 中断模式 IO 操作 ***
    ===================================
    [..]
      (+) 主机模式下使用 HAL_I2C_Master_Transmit_IT() 以非阻塞模式发送指定数量的数据
      (+) 发送传输完成时，执行 HAL_I2C_MasterTxCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_MasterTxCpltCallback() 添加自己的代码
      (+) 主机模式下使用 HAL_I2C_Master_Receive_IT() 以非阻塞模式接收指定数量的数据
      (+) 接收传输完成时，执行 HAL_I2C_MasterRxCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_MasterRxCpltCallback() 添加自己的代码
      (+) 从机模式下使用 HAL_I2C_Slave_Transmit_IT() 以非阻塞模式发送指定数量的数据
      (+) 发送传输完成时，执行 HAL_I2C_SlaveTxCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_SlaveTxCpltCallback() 添加自己的代码
      (+) 从机模式下使用 HAL_I2C_Slave_Receive_IT() 以非阻塞模式接收指定数量的数据
      (+) 接收传输完成时，执行 HAL_I2C_SlaveRxCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_SlaveRxCpltCallback() 添加自己的代码
      (+) 传输发生错误时，执行 HAL_I2C_ErrorCallback() 函数，用户可通过
          自定义函数指针 HAL_I2C_ErrorCallback() 添加自己的代码
      (+) 使用带中断的 HAL_I2C_Master_Abort_IT() 中止主机或内存 I2C 处理通信
      (+) 中止处理结束时，执行 HAL_I2C_AbortCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_AbortCpltCallback() 添加自己的代码

    *** 中断模式或 DMA 模式 IO 连续操作 ***
    ==========================================================
    [..]
      (@) 这些接口允许在传输期间方向改变时，管理带重复起始条件 (repeated start condition)
          的连续传输
    [..]
      (+) 特定的选项字段用于管理连续传输的不同阶段
      (+) 选项字段的值通过 I2C_XferOptions_definition 定义，并在下方列出：
      (++) I2C_FIRST_AND_LAST_FRAME: 非连续传输，功能与非连续模式下的相关接口相同
      (++) I2C_FIRST_FRAME: 连续传输，该选项允许管理带起始条件、地址
                            以及要传输的数据且不带最终停止条件的序列
      (++) I2C_FIRST_AND_NEXT_FRAME: 连续传输（仅限主机），该选项允许管理带起始条件、地址
                            以及要传输的数据且不带最终停止条件的序列，进而允许连续多次调用同一个主机连续接口
                            （例如连续调用 HAL_I2C_Master_Seq_Transmit_IT()
                            或连续调用 HAL_I2C_Master_Seq_Transmit_DMA()）
      (++) I2C_NEXT_FRAME: 连续传输，该选项允许管理带有重启动条件、地址
                            以及若方向改变则包含新数据的序列；若方向未改变，则仅管理新传输数据，
                            两种情况下均不带最终停止条件
      (++) I2C_LAST_FRAME: 连续传输，该选项允许管理带有重启动条件、地址
                            以及若方向改变则包含新数据的序列；若方向未改变，则仅管理新传输数据，
                            两种情况下均带有最终停止条件
      (++) I2C_LAST_FRAME_NO_STOP: 连续传输（仅限主机），该选项允许在多次调用同一个主机连续接口后
                            管理重启动条件（与选项 I2C_FIRST_AND_NEXT_FRAME 关联）。
                            用法：可使用 HAL_I2C_Master_Seq_Transmit_IT（先使用选项 I2C_FIRST_AND_NEXT_FRAME 然后 I2C_NEXT_FRAME）逐字节传输多个字节
                              或使用 HAL_I2C_Master_Seq_Receive_IT（先使用选项 I2C_FIRST_AND_NEXT_FRAME 然后 I2C_NEXT_FRAME）
                              或使用 HAL_I2C_Master_Seq_Transmit_DMA（先使用选项 I2C_FIRST_AND_NEXT_FRAME 然后 I2C_NEXT_FRAME）
                              或使用 HAL_I2C_Master_Seq_Receive_DMA（先使用选项 I2C_FIRST_AND_NEXT_FRAME 然后 I2C_NEXT_FRAME）。
                            然后在最后一个发送或接收序列中使用该选项 I2C_LAST_FRAME_NO_STOP，
                              即可在不停止通信的情况下调用相反的接收或发送接口，从而产生重启动条件。
      (++) I2C_OTHER_FRAME: 连续传输（仅限主机），该选项允许在每次调用同一个主机连续接口后
                            管理重启动条件。
                            用法：可使用 HAL_I2C_Master_Seq_Transmit_IT（先使用选项 I2C_FIRST_FRAME 然后 I2C_OTHER_FRAME）逐字节传输，且每个字节之间带从机地址重启动
                              或使用 HAL_I2C_Master_Seq_Receive_IT（先使用选项 I2C_FIRST_FRAME 然后 I2C_OTHER_FRAME）
                              或使用 HAL_I2C_Master_Seq_Transmit_DMA（先使用选项 I2C_FIRST_FRAME 然后 I2C_OTHER_FRAME）
                              或使用 HAL_I2C_Master_Seq_Receive_DMA（先使用选项 I2C_FIRST_FRAME 然后 I2C_OTHER_FRAME）。
                            然后在最后一帧使用选项 I2C_OTHER_AND_LAST_FRAME 以辅助自动生成 STOP 条件。

      (+) 不同的连续 I2C 接口列出如下：
      (++) 主机 I2C 模式下使用 HAL_I2C_Master_Seq_Transmit_IT()
            或使用 HAL_I2C_Master_Seq_Transmit_DMA() 以非阻塞模式连续发送指定数量的数据
      (+++) 当前帧传输发送完成时，执行 HAL_I2C_MasterTxCpltCallback()，用户可通过
           自定义函数指针 HAL_I2C_MasterTxCpltCallback() 添加自己的代码
      (++) 主机 I2C 模式下使用 HAL_I2C_Master_Seq_Receive_IT()
            或使用 HAL_I2C_Master_Seq_Receive_DMA() 以非阻塞模式连续接收指定数量的数据
      (+++) 当前帧传输接收完成时，执行 HAL_I2C_MasterRxCpltCallback()，用户可通过
           自定义函数指针 HAL_I2C_MasterRxCpltCallback() 添加自己的代码
      (++) 使用带中断的 HAL_I2C_Master_Abort_IT() 中止主机或内存 IT 或 DMA I2C 流程通信
      (+++) 中止流程结束时，执行 HAL_I2C_AbortCpltCallback()，用户可通过
           自定义函数指针 HAL_I2C_AbortCpltCallback() 添加自己的代码
      (++) 在从机 I2C 模式下使用 HAL_I2C_EnableListen_IT() 和 HAL_I2C_DisableListen_IT() 使能/禁用地址监听模式
      (+++) 当从机 I2C 地址匹配时，执行 HAL_I2C_AddrCallback()，用户可
           添加自己的代码以检查地址匹配代码 (Address Match Code) 及主机请求的传输方向（写/读）。
      (+++) 监听模式结束时执行 HAL_I2C_ListenCpltCallback()，用户可通过
           自定义函数指针 HAL_I2C_ListenCpltCallback() 添加自己的代码
      (++) 从机 I2C 模式下使用 HAL_I2C_Slave_Seq_Transmit_IT()
            或使用 HAL_I2C_Slave_Seq_Transmit_DMA() 以非阻塞模式连续发送指定数量的数据
      (+++) 当前帧传输发送完成时，执行 HAL_I2C_SlaveTxCpltCallback()，用户可通过
           自定义函数指针 HAL_I2C_SlaveTxCpltCallback() 添加自己的代码
      (++) 从机 I2C 模式下使用 HAL_I2C_Slave_Seq_Receive_IT()
            或使用 HAL_I2C_Slave_Seq_Receive_DMA() 以非阻塞模式连续接收指定数量的数据
      (+++) 当前帧传输接收完成时，执行 HAL_I2C_SlaveRxCpltCallback()，用户可通过
           自定义函数指针 HAL_I2C_SlaveRxCpltCallback() 添加自己的代码
      (++) 发生传输错误时，执行 HAL_I2C_ErrorCallback() 函数，用户可通过
           自定义函数指针 HAL_I2C_ErrorCallback() 添加自己的代码

    *** 中断模式 IO 内存操作 ***
    =======================================
    [..]
      (+) 使用中断通过 HAL_I2C_Mem_Write_IT() 以非阻塞模式向指定内存地址
          写入指定数量的数据
      (+) 内存写入传输完成时，执行 HAL_I2C_MemTxCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_MemTxCpltCallback() 添加自己的代码
      (+) 使用中断通过 HAL_I2C_Mem_Read_IT() 以非阻塞模式从指定内存地址
          读取指定数量的数据
      (+) 内存读取传输完成时，执行 HAL_I2C_MemRxCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_MemRxCpltCallback() 添加自己的代码
      (+) 发生传输错误时，执行 HAL_I2C_ErrorCallback() 函数，用户可通过
          自定义函数指针 HAL_I2C_ErrorCallback() 添加自己的代码

    *** DMA 模式 IO 操作 ***
    ==============================
    [..]
      (+) 主机模式下使用 HAL_I2C_Master_Transmit_DMA() 以非阻塞模式 (DMA)
          发送指定数量的数据
      (+) 发送传输完成时，执行 HAL_I2C_MasterTxCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_MasterTxCpltCallback() 添加自己的代码
      (+) 主机模式下使用 HAL_I2C_Master_Receive_DMA() 以非阻塞模式 (DMA)
          接收指定数量的数据
      (+) 接收传输完成时，执行 HAL_I2C_MasterRxCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_MasterRxCpltCallback() 添加自己的代码
      (+) 从机模式下使用 HAL_I2C_Slave_Transmit_DMA() 以非阻塞模式 (DMA)
          发送指定数量的数据
      (+) 发送传输完成时，执行 HAL_I2C_SlaveTxCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_SlaveTxCpltCallback() 添加自己的代码
      (+) 从机模式下使用 HAL_I2C_Slave_Receive_DMA() 以非阻塞模式 (DMA)
          接收指定数量的数据
      (+) 接收传输完成时，执行 HAL_I2C_SlaveRxCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_SlaveRxCpltCallback() 添加自己的代码
      (+) 发生传输错误时，执行 HAL_I2C_ErrorCallback() 函数，用户可通过
          自定义函数指针 HAL_I2C_ErrorCallback() 添加自己的代码
      (+) 使用中断通过 HAL_I2C_Master_Abort_IT() 中止主机或内存 I2C 流程通信
      (+) 中止流程结束时，执行 HAL_I2C_AbortCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_AbortCpltCallback() 添加自己的代码

    *** DMA 模式 IO 内存操作 ***
    =================================
    [..]
      (+) 使用 DMA 通过 HAL_I2C_Mem_Write_DMA() 以非阻塞模式向指定内存地址
          写入指定数量的数据
      (+) 内存写入传输完成时，执行 HAL_I2C_MemTxCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_MemTxCpltCallback() 添加自己的代码
      (+) 使用 DMA 通过 HAL_I2C_Mem_Read_DMA() 以非阻塞模式从指定内存地址
          读取指定数量的数据
      (+) 内存读取传输完成时，执行 HAL_I2C_MemRxCpltCallback()，用户可通过
          自定义函数指针 HAL_I2C_MemRxCpltCallback() 添加自己的代码
      (+) 发生传输错误时，执行 HAL_I2C_ErrorCallback() 函数，用户可通过
          自定义函数指针 HAL_I2C_ErrorCallback() 添加自己的代码


     *** I2C HAL 驱动宏列表 ***
     ==================================
     [..]
       以下是 I2C HAL 驱动程序中最常用的宏列表：

      (+) __HAL_I2C_ENABLE:     使能 I2C 外设
      (+) __HAL_I2C_DISABLE:    禁用 I2C 外设
      (+) __HAL_I2C_GET_FLAG:   检查指定的 I2C 标志位是否置位
      (+) __HAL_I2C_CLEAR_FLAG: 清除指定的 I2C 挂起标志位
      (+) __HAL_I2C_ENABLE_IT:  使能指定的 I2C 中断
      (+) __HAL_I2C_DISABLE_IT: 禁用指定的 I2C 中断

     *** 回调函数注册 ***
     =============================================
    [..]
     当编译标志 USE_HAL_I2C_REGISTER_CALLBACKS 置为 1 时，
     允许用户动态配置驱动程序回调函数。
     使用函数 HAL_I2C_RegisterCallback() 或 HAL_I2C_RegisterAddrCallback()
     注册中断回调函数。
    [..]
     函数 HAL_I2C_RegisterCallback() 允许注册以下回调函数：
       (+) MasterTxCpltCallback : 主机发送传输完成回调函数。
       (+) MasterRxCpltCallback : 主机接收传输完成回调函数。
       (+) SlaveTxCpltCallback  : 从机发送传输完成回调函数。
       (+) SlaveRxCpltCallback  : 从机接收传输完成回调函数。
       (+) ListenCpltCallback   : 监听模式结束回调函数。
       (+) MemTxCpltCallback    : 内存发送传输完成回调函数。
       (+) MemRxCpltCallback    : 内存接收传输完成回调函数。
       (+) ErrorCallback        : 错误检测回调函数。
       (+) AbortCpltCallback    : 中止流程完成回调函数。
       (+) MspInitCallback      : MSP 初始化回调函数。
       (+) MspDeInitCallback    : MSP 反初始化回调函数。
     该函数接收 HAL 外设句柄、回调 ID
     以及指向用户回调函数的指针作为参数。
    [..]
     对于特定回调函数 AddrCallback，使用专用的注册回调函数：HAL_I2C_RegisterAddrCallback()。
    [..]
     使用函数 HAL_I2C_UnRegisterCallback 将回调函数重置为默认的弱函数 (weak function)。
     HAL_I2C_UnRegisterCallback 接收 HAL 外设句柄
     和回调 ID 作为参数。
     该函数允许重置以下回调函数：
       (+) MasterTxCpltCallback : 主机发送传输完成回调函数。
       (+) MasterRxCpltCallback : 主机接收传输完成回调函数。
       (+) SlaveTxCpltCallback  : 从机发送传输完成回调函数。
       (+) SlaveRxCpltCallback  : 从机接收传输完成回调函数。
       (+) ListenCpltCallback   : 监听模式结束回调函数。
       (+) MemTxCpltCallback    : 内存发送传输完成回调函数。
       (+) MemRxCpltCallback    : 内存接收传输完成回调函数。
       (+) ErrorCallback        : 错误检测回调函数。
       (+) AbortCpltCallback    : 中止流程完成回调函数。
       (+) MspInitCallback      : MSP 初始化回调函数。
       (+) MspDeInitCallback    : MSP 反初始化回调函数。
    [..]
     对于回调函数 AddrCallback，使用专用的注销回调函数：HAL_I2C_UnRegisterAddrCallback()。
    [..]
     默认情况下，在执行 HAL_I2C_Init() 后且状态为 HAL_I2C_STATE_RESET 时，
     所有回调函数均被设置为对应的弱函数：
     例如 HAL_I2C_MasterTxCpltCallback()、HAL_I2C_MasterRxCpltCallback()。
     但 MspInit 和 MspDeInit 函数例外：只有当这些回调为空（此前未注册）时，
     才会在 HAL_I2C_Init()/ HAL_I2C_DeInit() 中被重置为原有的弱函数。
     如果 MspInit 或 MspDeInit 不为空，无论当前状态为何，HAL_I2C_Init()/
     HAL_I2C_DeInit() 都会保留并使用（此前已注册的）用户 MspInit/MspDeInit 回调函数。
    [..]
     仅在 HAL_I2C_STATE_READY 状态下才能注册/注销回调函数。
     但 MspInit/MspDeInit 函数例外，它们可以在 HAL_I2C_STATE_READY
     或 HAL_I2C_STATE_RESET 状态下注册/注销，
     因此在 Init/DeInit 期间可以使用已注册的（用户）MspInit/DeInit 回调。
     此时，用户应在调用 HAL_I2C_DeInit() 或 HAL_I2C_Init() 函数之前，
     先使用 HAL_I2C_RegisterCallback() 注册 MspInit/MspDeInit 用户回调。
    [..]
     当编译标志 USE_HAL_I2C_REGISTER_CALLBACKS 设置为 0 或
     未定义时，回调函数注册功能不可用，所有回调函数
     都被设置为对应的弱函数。


     *** 与芯片硬件限制相关的 I2C 规避措施 (Workarounds) ***
     ====================================================
     [..]
       以下是针对 STM32F1xx 产品在 HAL 中实现的所有芯片硬件限制规避方案列表。
       (@) 请参阅勘误表 (ErrataSheet) 以获取您产品的完整芯片限制列表。

       (+) I2C HAL 驱动内部实现的规避措施
          (++) 数据寄存器中读入错误数据（轮询和中断模式）
          (++) 发生错误放置的 Stop 条件后无法生成 Start 条件
          (++) 某些软件事件必须在当前字节传输之前处理：
               规避方法：通常使用 DMA，但主机接收单个字节的情况除外。
               对于中断模式，I2C 在应用程序中应具有最高优先级。
          (++) “重复起始条件的建立时间 (Setup time for a repeated Start condition)”时序参数不匹配：
               规避方法：将频率降低至 88 kHz，或者如果从机支持，
               则使用 I2C 快速模式 (Fast-mode)。
          (++) 数据有效时间 (tVD;DAT) 违规且未置位 OVR 标志：
               规避方法：若从设备允许，在 HAL_I2C_Init 中通过编程
               NoStretchMode = I2C_NOSTRETCH_DISABLE 来使用时钟延长 (clock stretching) 机制。

     [..]
       (@) 您可以参考 I2C HAL 驱动头文件以获取更多有用的宏定义

  @endverbatim
  */

/* 头文件包含 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup I2C I2C
  * @brief I2C HAL 模块驱动
  * @{
  */

#ifdef HAL_I2C_MODULE_ENABLED

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/** @defgroup I2C_Private_Define I2C 私有宏定义
  * @{
  */
#define I2C_TIMEOUT_FLAG          35U         /*!< 超时时间 35 ms            */
#define I2C_TIMEOUT_BUSY_FLAG     25U         /*!< 超时时间 25 ms            */
#define I2C_TIMEOUT_STOP_FLAG     5U          /*!< 超时时间 5 ms             */
#define I2C_NO_OPTION_FRAME       0xFFFF0000U /*!< XferOptions 默认值        */

/* 用于 @ref PreviousState 的私有宏定义 */
#define I2C_STATE_MSK             ((uint32_t)((uint32_t)((uint32_t)HAL_I2C_STATE_BUSY_TX | (uint32_t)HAL_I2C_STATE_BUSY_RX) & (uint32_t)(~((uint32_t)HAL_I2C_STATE_READY)))) /*!< 状态掩码定义，仅保留 RX 和 TX 位                       */
#define I2C_STATE_NONE            ((uint32_t)(HAL_I2C_MODE_NONE))                                                        /*!< 默认值                                                 */
#define I2C_STATE_MASTER_BUSY_TX  ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_TX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_MASTER))            /*!< 主机忙发送，由 State LSB 与 Mode 枚举组合而成          */
#define I2C_STATE_MASTER_BUSY_RX  ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_RX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_MASTER))            /*!< 主机忙接收，由状态低有效位 (LSB) 与模式枚举值组合而成 */
#define I2C_STATE_SLAVE_BUSY_TX   ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_TX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_SLAVE))             /*!< 从机忙发送，由状态低有效位 (LSB) 与模式枚举值组合而成 */
#define I2C_STATE_SLAVE_BUSY_RX   ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_RX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_SLAVE))             /*!< 从机忙接收，由状态低有效位 (LSB) 与模式枚举值组合而成 */

/**
  * @}
  */

/* 私有宏 --------------------------------------------------------------------*/
/** @addtogroup I2C_Private_Macros
  * @{
  */
/* 用于获取 DMA 端待传输剩余数据量的宏 */
#define I2C_GET_DMA_REMAIN_DATA(__HANDLE__)     __HAL_DMA_GET_COUNTER(__HANDLE__)
/**
  * @}
  */
/* 私有变量 ------------------------------------------------------------------*/
/* 私有函数原型 --------------------------------------------------------------*/

/** @defgroup I2C_Private_Functions I2C 私有函数
  * @{
  */
/* 用于处理 DMA 传输的私有函数 */
static void I2C_DMAXferCplt(DMA_HandleTypeDef *hdma);
static void I2C_DMAError(DMA_HandleTypeDef *hdma);
static void I2C_DMAAbort(DMA_HandleTypeDef *hdma);

static void I2C_ITError(I2C_HandleTypeDef *hi2c);

static HAL_StatusTypeDef I2C_MasterRequestWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef I2C_MasterRequestRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef I2C_RequestMemoryWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef I2C_RequestMemoryRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart);

/* 用于在轮询传输期间处理标志位的私有函数 */
static HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef I2C_WaitOnMasterAddressFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef I2C_WaitOnTXEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef I2C_WaitOnBTFFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef I2C_WaitOnRXNEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef I2C_WaitOnSTOPFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef I2C_WaitOnSTOPRequestThroughIT(I2C_HandleTypeDef *hi2c);
static HAL_StatusTypeDef I2C_IsAcknowledgeFailed(I2C_HandleTypeDef *hi2c);

/* 用于 I2C 传输中断服务函数 (IRQ) 的私有函数 */
static void I2C_MasterTransmit_TXE(I2C_HandleTypeDef *hi2c);
static void I2C_MasterTransmit_BTF(I2C_HandleTypeDef *hi2c);
static void I2C_MasterReceive_RXNE(I2C_HandleTypeDef *hi2c);
static void I2C_MasterReceive_BTF(I2C_HandleTypeDef *hi2c);
static void I2C_Master_SB(I2C_HandleTypeDef *hi2c);
static void I2C_Master_ADD10(I2C_HandleTypeDef *hi2c);
static void I2C_Master_ADDR(I2C_HandleTypeDef *hi2c);

static void I2C_SlaveTransmit_TXE(I2C_HandleTypeDef *hi2c);
static void I2C_SlaveTransmit_BTF(I2C_HandleTypeDef *hi2c);
static void I2C_SlaveReceive_RXNE(I2C_HandleTypeDef *hi2c);
static void I2C_SlaveReceive_BTF(I2C_HandleTypeDef *hi2c);
static void I2C_Slave_ADDR(I2C_HandleTypeDef *hi2c, uint32_t IT2Flags);
static void I2C_Slave_STOPF(I2C_HandleTypeDef *hi2c);
static void I2C_Slave_AF(I2C_HandleTypeDef *hi2c);

static void I2C_MemoryTransmit_TXE_BTF(I2C_HandleTypeDef *hi2c);

/* 用于转换特定选项的私有函数 */
static void I2C_ConvertOtherXferOptions(I2C_HandleTypeDef *hi2c);

/* 用于清空 DR 寄存器的私有函数 */
static void I2C_Flush_DR(I2C_HandleTypeDef *hi2c);
/**
  * @}
  */

/* 导出函数 ------------------------------------------------------------------*/

/** @defgroup I2C_Exported_Functions I2C 导出函数
  * @{
  */

/** @defgroup I2C_Exported_Functions_Group1 初始化和反初始化函数
 *  @brief    初始化与配置函数
 *
@verbatim
 ===============================================================================
              ##### 初始化和反初始化函数 #####
 ===============================================================================
    [..]  本小节提供了一组用于初始化和反初始化 I2Cx 外设的函数：

      (+) 用户必须实现 HAL_I2C_MspInit() 函数，并在其中配置所有相关的外设资源
          （时钟、GPIO、DMA、中断和 NVIC）。

      (+) 调用 HAL_I2C_Init() 函数以按照所选配置对指定设备进行配置：
        (++) 通信速率
        (++) 占空比
        (++) 寻址模式
        (++) 自身地址 1
        (++) 双地址模式
        (++) 自身地址 2
        (++) 广播呼叫模式
        (++) 禁止时钟延长模式

      (+) 调用 HAL_I2C_DeInit() 函数以恢复所选 I2Cx 外设的默认配置。

@endverbatim
  * @{
  */

/**
  * @brief  根据 I2C_InitTypeDef 中指定的参数初始化 I2C，
  *         并初始化关联的句柄。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c)
{
  uint32_t freqrange;
  uint32_t pclk1;

  /* 检查 I2C 句柄分配有效性 */
  if (hi2c == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数有效性 */
  assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));
  assert_param(IS_I2C_CLOCK_SPEED(hi2c->Init.ClockSpeed));
  assert_param(IS_I2C_DUTY_CYCLE(hi2c->Init.DutyCycle));
  assert_param(IS_I2C_OWN_ADDRESS1(hi2c->Init.OwnAddress1));
  assert_param(IS_I2C_ADDRESSING_MODE(hi2c->Init.AddressingMode));
  assert_param(IS_I2C_DUAL_ADDRESS(hi2c->Init.DualAddressMode));
  assert_param(IS_I2C_OWN_ADDRESS2(hi2c->Init.OwnAddress2));
  assert_param(IS_I2C_GENERAL_CALL(hi2c->Init.GeneralCallMode));
  assert_param(IS_I2C_NO_STRETCH(hi2c->Init.NoStretchMode));

  if (hi2c->State == HAL_I2C_STATE_RESET)
  {
    /* 分配锁资源并进行初始化 */
    hi2c->Lock = HAL_UNLOCKED;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
    /* 初始化 I2C 回调函数设置 */
    hi2c->MasterTxCpltCallback = HAL_I2C_MasterTxCpltCallback; /* 兼容旧版本的弱定义 MasterTxCpltCallback */
    hi2c->MasterRxCpltCallback = HAL_I2C_MasterRxCpltCallback; /* 兼容旧版本的弱定义 MasterRxCpltCallback */
    hi2c->SlaveTxCpltCallback  = HAL_I2C_SlaveTxCpltCallback;  /* 兼容旧版本的弱定义 SlaveTxCpltCallback  */
    hi2c->SlaveRxCpltCallback  = HAL_I2C_SlaveRxCpltCallback;  /* 兼容旧版本的弱定义 SlaveRxCpltCallback  */
    hi2c->ListenCpltCallback   = HAL_I2C_ListenCpltCallback;   /* 兼容旧版本的弱定义 ListenCpltCallback   */
    hi2c->MemTxCpltCallback    = HAL_I2C_MemTxCpltCallback;    /* 遗留的弱定义 MemTxCpltCallback    */
    hi2c->MemRxCpltCallback    = HAL_I2C_MemRxCpltCallback;    /* 遗留的弱定义 MemRxCpltCallback    */
    hi2c->ErrorCallback        = HAL_I2C_ErrorCallback;        /* 遗留的弱定义 ErrorCallback        */
    hi2c->AbortCpltCallback    = HAL_I2C_AbortCpltCallback;    /* 遗留的弱定义 AbortCpltCallback    */
    hi2c->AddrCallback         = HAL_I2C_AddrCallback;         /* 遗留的弱定义 AddrCallback         */

    if (hi2c->MspInitCallback == NULL)
    {
      hi2c->MspInitCallback = HAL_I2C_MspInit; /* 遗留的弱定义 MspInit  */
    }

    /* 初始化底层硬件：GPIO、时钟、NVIC */
    hi2c->MspInitCallback(hi2c);
#else
    /* 初始化底层硬件：GPIO、时钟、NVIC */
    HAL_I2C_MspInit(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }

  hi2c->State = HAL_I2C_STATE_BUSY;

  /* 禁用选定的 I2C 外设 */
  __HAL_I2C_DISABLE(hi2c);

  /* 复位 I2C */
  hi2c->Instance->CR1 |= I2C_CR1_SWRST;
  hi2c->Instance->CR1 &= ~I2C_CR1_SWRST;

  /* 获取 PCLK1 时钟频率 */
  pclk1 = HAL_RCC_GetPCLK1Freq();

  /* 检查 PCLK1 允许的最小频率 */
  if (I2C_MIN_PCLK_FREQ(pclk1, hi2c->Init.ClockSpeed) == 1U)
  {
    return HAL_ERROR;
  }

  /* 计算频率范围 */
  freqrange = I2C_FREQRANGE(pclk1);

  /*---------------------------- I2Cx CR2 配置 ----------------------*/
  /* 配置 I2Cx：频率范围 */
  MODIFY_REG(hi2c->Instance->CR2, I2C_CR2_FREQ, freqrange);

  /*---------------------------- I2Cx TRISE 配置 ----------------------------*/
  /* 配置 I2Cx：上升时间 */
  MODIFY_REG(hi2c->Instance->TRISE, I2C_TRISE_TRISE, I2C_RISE_TIME(freqrange, hi2c->Init.ClockSpeed));

  /*---------------------------- I2Cx CCR 配置 ------------------------------*/
  /* 配置 I2Cx：速度 */
  MODIFY_REG(hi2c->Instance->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), I2C_SPEED(pclk1, hi2c->Init.ClockSpeed, hi2c->Init.DutyCycle));

  /*---------------------------- I2Cx CR1 配置 ------------------------------*/
  /* 配置 I2Cx：广播呼叫和禁止时钟延长模式 */
  MODIFY_REG(hi2c->Instance->CR1, (I2C_CR1_ENGC | I2C_CR1_NOSTRETCH), (hi2c->Init.GeneralCallMode | hi2c->Init.NoStretchMode));

  /*---------------------------- I2Cx OAR1 配置 -----------------------------*/
  /* 配置 I2Cx：自身地址 1 及寻址模式 */
  MODIFY_REG(hi2c->Instance->OAR1, (I2C_OAR1_ADDMODE | I2C_OAR1_ADD8_9 | I2C_OAR1_ADD1_7 | I2C_OAR1_ADD0), (hi2c->Init.AddressingMode | hi2c->Init.OwnAddress1));

  /*---------------------------- I2Cx OAR2 配置 -----------------------------*/
  /* 配置 I2Cx：双地址模式及自身地址 2 */
  MODIFY_REG(hi2c->Instance->OAR2, (I2C_OAR2_ENDUAL | I2C_OAR2_ADD2), (hi2c->Init.DualAddressMode | hi2c->Init.OwnAddress2));

  /* 使能所选的 I2C 外设 */
  __HAL_I2C_ENABLE(hi2c);

  hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
  hi2c->State = HAL_I2C_STATE_READY;
  hi2c->PreviousState = I2C_STATE_NONE;
  hi2c->Mode = HAL_I2C_MODE_NONE;

  return HAL_OK;
}

/**
  * @brief  反初始化 I2C 外设。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c)
{
  /* 检查 I2C 句柄分配 */
  if (hi2c == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数有效性 */
  assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));

  hi2c->State = HAL_I2C_STATE_BUSY;

  /* 禁用 I2C 外设时钟 */
  __HAL_I2C_DISABLE(hi2c);

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
  if (hi2c->MspDeInitCallback == NULL)
  {
    hi2c->MspDeInitCallback = HAL_I2C_MspDeInit; /* 传统的弱定义 MspDeInit */
  }

  /* 反初始化底层硬件：GPIO、CLOCK、NVIC */
  hi2c->MspDeInitCallback(hi2c);
#else
  /* 反初始化底层硬件：GPIO、CLOCK、NVIC */
  HAL_I2C_MspDeInit(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

  hi2c->ErrorCode     = HAL_I2C_ERROR_NONE;
  hi2c->State         = HAL_I2C_STATE_RESET;
  hi2c->PreviousState = I2C_STATE_NONE;
  hi2c->Mode          = HAL_I2C_MODE_NONE;

  /* 释放锁 */
  __HAL_UNLOCK(hi2c);

  return HAL_OK;
}

/**
  * @brief  初始化 I2C MSP。
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *         I2C_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hi2c);

  /* 注意：不应修改此函数，当需要回调函数时，
            可以在用户文件中实现 HAL_I2C_MspInit
   */
}

/**
  * @brief  反初始化 I2C MSP。
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *         I2C_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hi2c);

  /* 注意：不应修改此函数，当需要回调函数时，
            可以在用户文件中实现 HAL_I2C_MspDeInit
   */
}

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
/**
  * @brief  注册用户 I2C 回调函数
  *         用于替代预定义的弱函数回调
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @param  CallbackID 要注册的回调函数 ID
  *         该参数可以是以下值之一：
  *          @arg @ref HAL_I2C_MASTER_TX_COMPLETE_CB_ID 主机发送完成回调 ID
  *          @arg @ref HAL_I2C_MASTER_RX_COMPLETE_CB_ID 主机接收完成回调 ID
  *          @arg @ref HAL_I2C_SLAVE_TX_COMPLETE_CB_ID 从机发送完成回调 ID
  *          @arg @ref HAL_I2C_SLAVE_RX_COMPLETE_CB_ID 从机接收完成回调 ID
  *          @arg @ref HAL_I2C_LISTEN_COMPLETE_CB_ID 监听完成回调 ID
  *          @arg @ref HAL_I2C_MEM_TX_COMPLETE_CB_ID 内存写传输完成回调 ID
  *          @arg @ref HAL_I2C_MEM_RX_COMPLETE_CB_ID 内存读传输完成回调 ID
  *          @arg @ref HAL_I2C_ERROR_CB_ID 错误回调 ID
  *          @arg @ref HAL_I2C_ABORT_CB_ID 中止回调 ID
  *          @arg @ref HAL_I2C_MSPINIT_CB_ID MspInit 回调 ID
  *          @arg @ref HAL_I2C_MSPDEINIT_CB_ID MspDeInit 回调 ID
  * @param  pCallback 指向回调函数的指针
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_RegisterCallback(I2C_HandleTypeDef *hi2c, HAL_I2C_CallbackIDTypeDef CallbackID, pI2C_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    /* 更新错误代码 */
    hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }
  /* 进程已锁定 */
  __HAL_LOCK(hi2c);

  if (HAL_I2C_STATE_READY == hi2c->State)
  {
    switch (CallbackID)
    {
      case HAL_I2C_MASTER_TX_COMPLETE_CB_ID :
        hi2c->MasterTxCpltCallback = pCallback;
        break;

      case HAL_I2C_MASTER_RX_COMPLETE_CB_ID :
        hi2c->MasterRxCpltCallback = pCallback;
        break;

      case HAL_I2C_SLAVE_TX_COMPLETE_CB_ID :
        hi2c->SlaveTxCpltCallback = pCallback;
        break;

      case HAL_I2C_SLAVE_RX_COMPLETE_CB_ID :
        hi2c->SlaveRxCpltCallback = pCallback;
        break;

      case HAL_I2C_LISTEN_COMPLETE_CB_ID :
        hi2c->ListenCpltCallback = pCallback;
        break;

      case HAL_I2C_MEM_TX_COMPLETE_CB_ID :
        hi2c->MemTxCpltCallback = pCallback;
        break;

      case HAL_I2C_MEM_RX_COMPLETE_CB_ID :
        hi2c->MemRxCpltCallback = pCallback;
        break;

      case HAL_I2C_ERROR_CB_ID :
        hi2c->ErrorCallback = pCallback;
        break;

      case HAL_I2C_ABORT_CB_ID :
        hi2c->AbortCpltCallback = pCallback;
        break;

      case HAL_I2C_MSPINIT_CB_ID :
        hi2c->MspInitCallback = pCallback;
        break;

      case HAL_I2C_MSPDEINIT_CB_ID :
        hi2c->MspDeInitCallback = pCallback;
        break;

      default :
        /* 更新错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (HAL_I2C_STATE_RESET == hi2c->State)
  {
    switch (CallbackID)
    {
      case HAL_I2C_MSPINIT_CB_ID :
        hi2c->MspInitCallback = pCallback;
        break;

      case HAL_I2C_MSPDEINIT_CB_ID :
        hi2c->MspDeInitCallback = pCallback;
        break;

      default :
        /* 更新错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* 更新错误代码 */
    hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;

    /* 返回错误状态 */
    status =  HAL_ERROR;
  }

  /* 释放锁 */
  __HAL_UNLOCK(hi2c);
  return status;
}

/**
  * @brief  注销 I2C 回调函数
  *         I2C 回调函数将被重定向到预定义的弱函数 (weak) 回调
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针
  * @param  CallbackID 要注销的回调函数 ID
  *         该参数可以是以下值之一：
  *         该参数可以是以下值之一：
  *          @arg @ref HAL_I2C_MASTER_TX_COMPLETE_CB_ID 主机发送完成回调 ID
  *          @arg @ref HAL_I2C_MASTER_RX_COMPLETE_CB_ID 主机接收完成回调 ID
  *          @arg @ref HAL_I2C_SLAVE_TX_COMPLETE_CB_ID 从机发送完成回调 ID
  *          @arg @ref HAL_I2C_SLAVE_RX_COMPLETE_CB_ID 从机接收完成回调 ID
  *          @arg @ref HAL_I2C_LISTEN_COMPLETE_CB_ID 监听完成回调 ID
  *          @arg @ref HAL_I2C_MEM_TX_COMPLETE_CB_ID 内存写入发送回调 ID
  *          @arg @ref HAL_I2C_MEM_RX_COMPLETE_CB_ID 内存读取接收回调 ID
  *          @arg @ref HAL_I2C_ERROR_CB_ID 错误回调 ID
  *          @arg @ref HAL_I2C_ABORT_CB_ID 中止回调 ID
  *          @arg @ref HAL_I2C_MSPINIT_CB_ID MspInit 回调 ID
  *          @arg @ref HAL_I2C_MSPDEINIT_CB_ID MspDeInit 回调 ID
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_UnRegisterCallback(I2C_HandleTypeDef *hi2c, HAL_I2C_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 进程已锁定 */
  __HAL_LOCK(hi2c);

  if (HAL_I2C_STATE_READY == hi2c->State)
  {
    switch (CallbackID)
    {
      case HAL_I2C_MASTER_TX_COMPLETE_CB_ID :
        hi2c->MasterTxCpltCallback = HAL_I2C_MasterTxCpltCallback; /* 兼容旧版本的弱函数 MasterTxCpltCallback */
        break;

      case HAL_I2C_MASTER_RX_COMPLETE_CB_ID :
        hi2c->MasterRxCpltCallback = HAL_I2C_MasterRxCpltCallback; /* 兼容旧版本的弱函数 MasterRxCpltCallback */
        break;

      case HAL_I2C_SLAVE_TX_COMPLETE_CB_ID :
        hi2c->SlaveTxCpltCallback = HAL_I2C_SlaveTxCpltCallback;   /* 兼容旧版本的弱函数 SlaveTxCpltCallback  */
        break;

      case HAL_I2C_SLAVE_RX_COMPLETE_CB_ID :
        hi2c->SlaveRxCpltCallback = HAL_I2C_SlaveRxCpltCallback;   /* 兼容旧版本的弱函数 SlaveRxCpltCallback  */
        break;

      case HAL_I2C_LISTEN_COMPLETE_CB_ID :
        hi2c->ListenCpltCallback = HAL_I2C_ListenCpltCallback;     /* 兼容旧版本的弱函数 ListenCpltCallback   */
        break;

      case HAL_I2C_MEM_TX_COMPLETE_CB_ID :
        hi2c->MemTxCpltCallback = HAL_I2C_MemTxCpltCallback;       /* 兼容旧版本的弱函数 MemTxCpltCallback    */
        break;

      case HAL_I2C_MEM_RX_COMPLETE_CB_ID :
        hi2c->MemRxCpltCallback = HAL_I2C_MemRxCpltCallback;       /* 传统的弱定义 MemRxCpltCallback    */
        break;

      case HAL_I2C_ERROR_CB_ID :
        hi2c->ErrorCallback = HAL_I2C_ErrorCallback;               /* 传统的弱定义 ErrorCallback        */
        break;

      case HAL_I2C_ABORT_CB_ID :
        hi2c->AbortCpltCallback = HAL_I2C_AbortCpltCallback;       /* 传统的弱定义 AbortCpltCallback    */
        break;

      case HAL_I2C_MSPINIT_CB_ID :
        hi2c->MspInitCallback = HAL_I2C_MspInit;                   /* 传统的弱定义 MspInit              */
        break;

      case HAL_I2C_MSPDEINIT_CB_ID :
        hi2c->MspDeInitCallback = HAL_I2C_MspDeInit;               /* 传统的弱定义 MspDeInit            */
        break;

      default :
        /* 更新错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (HAL_I2C_STATE_RESET == hi2c->State)
  {
    switch (CallbackID)
    {
      case HAL_I2C_MSPINIT_CB_ID :
        hi2c->MspInitCallback = HAL_I2C_MspInit;                   /* 传统的弱定义 MspInit              */
        break;

      case HAL_I2C_MSPDEINIT_CB_ID :
        hi2c->MspDeInitCallback = HAL_I2C_MspDeInit;               /* 传统的弱定义 MspDeInit            */
        break;

      default :
        /* 更新错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;

        /* 返回错误状态 */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* 更新错误代码 */
    hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;

    /* 返回错误状态 */
    status =  HAL_ERROR;
  }

  /* 释放锁 */
  __HAL_UNLOCK(hi2c);
  return status;
}

/**
  * @brief  注册从机地址匹配 I2C 回调函数
  *         用于替代预定义的弱函数 HAL_I2C_AddrCallback()
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @param  pCallback 指向地址匹配回调函数的指针
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_RegisterAddrCallback(I2C_HandleTypeDef *hi2c, pI2C_AddrCallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    /* 更新错误代码 */
    hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }
  /* 进程锁定 */
  __HAL_LOCK(hi2c);

  if (HAL_I2C_STATE_READY == hi2c->State)
  {
    hi2c->AddrCallback = pCallback;
  }
  else
  {
    /* 更新错误代码 */
    hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;

    /* 返回错误状态 */
    status =  HAL_ERROR;
  }

  /* 释放锁 */
  __HAL_UNLOCK(hi2c);
  return status;
}

/**
  * @brief  注销从机地址匹配 I2C 回调函数
  *         信息就绪 I2C 回调函数被重定向到预定义的弱函数 HAL_I2C_AddrCallback()
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_UnRegisterAddrCallback(I2C_HandleTypeDef *hi2c)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 进程锁定 */
  __HAL_LOCK(hi2c);

  if (HAL_I2C_STATE_READY == hi2c->State)
  {
    hi2c->AddrCallback = HAL_I2C_AddrCallback; /* 遗留的弱函数 AddrCallback */
  }
  else
  {
    /* 更新错误代码 */
    hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;

    /* 返回错误状态 */
    status =  HAL_ERROR;
  }

  /* 释放锁 */
  __HAL_UNLOCK(hi2c);
  return status;
}

#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

/**
  * @brief  I2C 数据寄存器刷新处理过程。
  * @param  hi2c I2C 句柄。
  * @retval 无
  */
static void I2C_Flush_DR(I2C_HandleTypeDef *hi2c)
{
  /* 向 DR 写入伪数据以清除 TXE 标志 */
  if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXE) != RESET)
  {
    hi2c->Instance->DR = 0x00U;
  }
}

/**
  * @}
  */

/** @defgroup I2C_Exported_Functions_Group2 输入和输出操作函数
 *  @brief   数据传输函数
 *
@verbatim
 ===============================================================================
                      ##### IO 操作函数 #####
 ===============================================================================
    [..]
    本节提供了一组用于管理 I2C 数据传输的函数。

    (#) 存在两种传输模式：
       (++) 阻塞模式：通信以轮询方式执行。
            所有数据处理的状态在传输完成后由同一个函数返回。
       (++) 非阻塞模式：通信使用中断或 DMA 方式执行。
            这些函数返回传输启动的状态。
            数据处理的结束将在中断模式下通过专用的 I2C IRQ 指示，
            或在 DMA 模式下通过 DMA IRQ 指示。

    (#) 阻塞模式函数包括：
        (++) HAL_I2C_Master_Transmit()
        (++) HAL_I2C_Master_Receive()
        (++) HAL_I2C_Slave_Transmit()
        (++) HAL_I2C_Slave_Receive()
        (++) HAL_I2C_Mem_Write()
        (++) HAL_I2C_Mem_Read()
        (++) HAL_I2C_IsDeviceReady()

    (#) 基于中断的非阻塞模式函数包括：
        (++) HAL_I2C_Master_Transmit_IT()
        (++) HAL_I2C_Master_Receive_IT()
        (++) HAL_I2C_Slave_Transmit_IT()
        (++) HAL_I2C_Slave_Receive_IT()
        (++) HAL_I2C_Mem_Write_IT()
        (++) HAL_I2C_Mem_Read_IT()
        (++) HAL_I2C_Master_Seq_Transmit_IT()
        (++) HAL_I2C_Master_Seq_Receive_IT()
        (++) HAL_I2C_Slave_Seq_Transmit_IT()
        (++) HAL_I2C_Slave_Seq_Receive_IT()
        (++) HAL_I2C_EnableListen_IT()
        (++) HAL_I2C_DisableListen_IT()
        (++) HAL_I2C_Master_Abort_IT()

    (#) 基于 DMA 的非阻塞模式函数包括：
        (++) HAL_I2C_Master_Transmit_DMA()
        (++) HAL_I2C_Master_Receive_DMA()
        (++) HAL_I2C_Slave_Transmit_DMA()
        (++) HAL_I2C_Slave_Receive_DMA()
        (++) HAL_I2C_Mem_Write_DMA()
        (++) HAL_I2C_Mem_Read_DMA()
        (++) HAL_I2C_Master_Seq_Transmit_DMA()
        (++) HAL_I2C_Master_Seq_Receive_DMA()
        (++) HAL_I2C_Slave_Seq_Transmit_DMA()
        (++) HAL_I2C_Slave_Seq_Receive_DMA()

    (#) 非阻塞模式下提供了一组传输完成回调函数：
        (++) HAL_I2C_MasterTxCpltCallback()
        (++) HAL_I2C_MasterRxCpltCallback()
        (++) HAL_I2C_SlaveTxCpltCallback()
        (++) HAL_I2C_SlaveRxCpltCallback()
        (++) HAL_I2C_MemTxCpltCallback()
        (++) HAL_I2C_MemRxCpltCallback()
        (++) HAL_I2C_AddrCallback()
        (++) HAL_I2C_ListenCpltCallback()
        (++) HAL_I2C_ErrorCallback()
        (++) HAL_I2C_AbortCpltCallback()

@endverbatim
  * @{
  */

/**
  * @brief  在主机模式下以阻塞模式发送指定量的数据。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  DevAddress 目标设备地址：在调用该接口之前，数据手册中的 7 位设备地址值必须左移一位
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  Timeout 超时时间
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  /* 初始化 tickstart 用于超时管理 */
  uint32_t tickstart = HAL_GetTick();

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待直到 BUSY 标志位被复位 */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)
    {
      return HAL_BUSY;
    }

    /* 进程锁定 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已被使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State       = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode        = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    /* 发送从机地址 */
    if (I2C_MasterRequestWrite(hi2c, DevAddress, Timeout, tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* 清除 ADDR 标志位 */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

    while (hi2c->XferSize > 0U)
    {
      /* 等待直到 TXE 标志位置位 */
      if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
      {
        if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)
        {
          /* 生成停止条件 */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
        }
        return HAL_ERROR;
      }

      /* 将数据写入 DR */
      hi2c->Instance->DR = *hi2c->pBuffPtr;

      /* 递增缓冲区指针 */
      hi2c->pBuffPtr++;

      /* 更新计数器 */
      hi2c->XferCount--;
      hi2c->XferSize--;

      if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET) && (hi2c->XferSize != 0U))
      {
        /* 将数据写入 DR 寄存器 */
        hi2c->Instance->DR = *hi2c->pBuffPtr;

        /* 递增缓冲区指针 */
        hi2c->pBuffPtr++;

        /* 更新计数器 */
        hi2c->XferCount--;
        hi2c->XferSize--;
      }

      /* 等待 BTF 标志位置位 */
      if (I2C_WaitOnBTFFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
      {
        if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)
        {
          /* 生成停止信号 */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
        }
        return HAL_ERROR;
      }
    }

    /* 生成停止信号 */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->Mode = HAL_I2C_MODE_NONE;

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在主模式下以阻塞模式接收指定量的数据。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @param  DevAddress 目标设备地址：数据手册中的设备 7 位地址值
  *         在调用该接口前必须进行左移操作
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  Timeout 超时时间
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  __IO uint32_t count = 0U;

  /* 初始化超时管理起始滴答计数 tickstart */
  uint32_t tickstart = HAL_GetTick();

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待 BUSY 标志位复位 */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)
    {
      return HAL_BUSY;
    }

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State       = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode        = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    /* 发送从机地址 */
    if (I2C_MasterRequestRead(hi2c, DevAddress, Timeout, tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    if (hi2c->XferSize == 0U)
    {
      /* 清除 ADDR 标志 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

      /* 生成停止条件 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
    }
    else if (hi2c->XferSize == 1U)
    {
      /* 禁用应答 (NACK) */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 在清除 ADDR 和配置 STOP 期间禁用所有活动的 IRQ，因为 EV6_3
      软件时序必须在当前字节传输结束前完成 */
      __disable_irq();

      /* 清除 ADDR 标志 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

      /* 生成停止条件 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

      /* 重新使能 IRQ */
      __enable_irq();
    }
    else if (hi2c->XferSize == 2U)
    {
      /* 使能 Pos */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

      /* 在清除 ADDR 和配置 STOP 期间禁用所有活动的 IRQ，因为 EV6_3
      软件时序必须在当前字节传输结束前完成 */
      __disable_irq();

      /* 清除 ADDR 标志 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

      /* 禁用应答 (NACK) */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 重新使能 IRQ */
      __enable_irq();
    }
    else
    {
      /* 使能应答 (ACK) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 清除 ADDR 标志位 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
    }

    while (hi2c->XferSize > 0U)
    {
      if (hi2c->XferSize <= 3U)
      {
        /* 单字节 */
        if (hi2c->XferSize == 1U)
        {
          /* 等待 RXNE 标志位置位 */
          if (I2C_WaitOnRXNEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
          {
            return HAL_ERROR;
          }

          /* 从 DR 寄存器读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 缓冲区指针递增 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;
        }
        /* 双字节 */
        else if (hi2c->XferSize == 2U)
        {
          /* 等待 BTF 标志位置位 */
          if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout, tickstart) != HAL_OK)
          {
            return HAL_ERROR;
          }

          /* 在清除 ADDR 和配置 STOP 期间禁用所有活动的 IRQ，因为 EV6_3
             软件序列必须在当前字节传输结束前完成 */
          __disable_irq();

          /* 产生停止条件 (STOP) */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

          /* 从 DR 寄存器读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 缓冲区指针递增 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;

          /* 重新使能 IRQ */
          __enable_irq();

          /* 从 DR 寄存器读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 递增缓冲区指针 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;
        }
        /* 最后 3 个字节 */
        else
        {
          /* 等待 BTF 标志置位 */
          if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout, tickstart) != HAL_OK)
          {
            return HAL_ERROR;
          }

          /* 禁用应答 (ACK) */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

          /* 在清除 ADDR 和配置 STOP 期间禁用所有活动中断，因为 EV6_3 软件序列必须在当前字节传输完成之前执行完毕 */
          __disable_irq();

          /* 从 DR 读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 递增缓冲区指针 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;

          /* 等待 BTF 标志置位 */
          count = I2C_TIMEOUT_FLAG * (SystemCoreClock / 25U / 1000U);
          do
          {
            count--;
            if (count == 0U)
            {
              hi2c->PreviousState       = I2C_STATE_NONE;
              hi2c->State               = HAL_I2C_STATE_READY;
              hi2c->Mode                = HAL_I2C_MODE_NONE;
              hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

              /* 重新使能中断 */
              __enable_irq();

              /* 进程解锁 */
              __HAL_UNLOCK(hi2c);

              return HAL_ERROR;
            }
          }
          while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == RESET);

          /* 生成停止条件 */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

          /* 从 DR 读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 递增缓冲区指针 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;

          /* 重新使能中断 */
          __enable_irq();

          /* 从 DR 寄存器读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 缓冲区指针递增 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;
        }
      }
      else
      {
        /* 等待 RXNE 标志位置位 */
        if (I2C_WaitOnRXNEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
        {
          return HAL_ERROR;
        }

        /* 从 DR 寄存器读取数据 */
        *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

        /* 缓冲区指针递增 */
        hi2c->pBuffPtr++;

        /* 更新计数器 */
        hi2c->XferSize--;
        hi2c->XferCount--;

        if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET)
        {

          if (hi2c->XferSize == 3U)
          {
            /* 禁用应答 (ACK) */
            CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
          }

          /* 从 DR 寄存器读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 缓冲区指针递增 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;
        }
      }
    }

    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->Mode = HAL_I2C_MODE_NONE;

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在从机模式下以阻塞模式发送指定量的数据。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  Timeout 超时时间
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  /* 初始化用于超时管理的 tickstart */
  uint32_t tickstart = HAL_GetTick();

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* 进程已锁定 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State       = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode        = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    /* 使能地址应答 (ACK) */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    /* 等待 ADDR 标志位置位 */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, RESET, Timeout, tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* 清除 ADDR 标志位 */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

    /* 如果选择了 10 位寻址模式 */
    if (hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_10BIT)
    {
      /* 等待 ADDR 标志位置位 */
      if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, RESET, Timeout, tickstart) != HAL_OK)
      {
        return HAL_ERROR;
      }

      /* 清除 ADDR 标志位 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
    }

    while (hi2c->XferSize > 0U)
    {
      /* 等待 TXE 标志位置位 */
      if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
      {
        /* 禁用地址应答 (NACK) */
        CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

        return HAL_ERROR;
      }

      /* 向 DR 寄存器写入数据 */
      hi2c->Instance->DR = *hi2c->pBuffPtr;

      /* 递增缓冲区指针 */
      hi2c->pBuffPtr++;

      /* 更新计数器 */
      hi2c->XferCount--;
      hi2c->XferSize--;

      if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET) && (hi2c->XferSize != 0U))
      {
        /* 将数据写入 DR 寄存器 */
        hi2c->Instance->DR = *hi2c->pBuffPtr;

        /* 递增缓冲区指针 */
        hi2c->pBuffPtr++;

        /* 更新计数器 */
        hi2c->XferCount--;
        hi2c->XferSize--;
      }
    }

    /* 等待 AF 标志位置位 */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_AF, RESET, Timeout, tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* 清除 AF 标志位 */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

    /* 禁用地址应答 */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->Mode = HAL_I2C_MODE_NONE;

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在从机模式下以阻塞模式接收指定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  Timeout 超时时间
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  /* 初始化 tickstart 用于超时管理 */
  uint32_t tickstart = HAL_GetTick();

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    if ((pData == NULL) || (Size == (uint16_t)0))
    {
      return HAL_ERROR;
    }

    /* 进程锁定 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 外设是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State       = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode        = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    /* 使能地址应答 */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    /* 等待 ADDR 标志位置位 */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, RESET, Timeout, tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* 清除 ADDR 标志位 */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

    while (hi2c->XferSize > 0U)
    {
      /* 等待 RXNE 标志位置位 */
      if (I2C_WaitOnRXNEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
      {
        /* 禁用地址应答 */
        CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

        return HAL_ERROR;
      }

      /* 从 DR 读取数据 */
      *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

      /* 缓冲区指针递增 */
      hi2c->pBuffPtr++;

      /* 更新计数器 */
      hi2c->XferSize--;
      hi2c->XferCount--;

      if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET) && (hi2c->XferSize != 0U))
      {
        /* 从 DR 读取数据 */
        *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

        /* 缓冲区指针递增 */
        hi2c->pBuffPtr++;

        /* 更新计数器 */
        hi2c->XferSize--;
        hi2c->XferCount--;
      }
    }

    /* 等待 STOP 标志位置位 */
    if (I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
    {
      /* 禁用地址应答 */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      return HAL_ERROR;
    }

    /* 清除 STOP 标志位 */
    __HAL_I2C_CLEAR_STOPFLAG(hi2c);

    /* 禁用地址应答 */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->Mode = HAL_I2C_MODE_NONE;

    /* 解锁进程 */
    __HAL_UNLOCK(hi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在主机模式下以中断非阻塞方式发送指定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @param  DevAddress 目标设备地址：数据手册中的 7 位设备地址值
  *         在调用此接口前必须左移一位
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 待发送的数据量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
  __IO uint32_t count = 0U;

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待 BUSY 标志位清零 */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);
    do
    {
      count--;
      if (count == 0U)
      {
        hi2c->PreviousState       = I2C_STATE_NONE;
        hi2c->State               = HAL_I2C_STATE_READY;
        hi2c->Mode                = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

        return HAL_BUSY;
      }
    }
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);

    /* 锁定进程 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 外设是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode      = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;
    hi2c->Devaddress  = DevAddress;

    /* 解锁进程 */
    __HAL_UNLOCK(hi2c);

    /* 注意：必须在解锁当前进程之后再使能 I2C 中断，
              以避免在当前进程解锁之前执行 I2C 中断处理函数的风险 */
    /* 使能 EVT、BUF 和 ERR 中断 */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

    /* 生成起始条件 (Start) */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在主机模式下以中断非阻塞方式接收指定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @param  DevAddress 目标设备地址：数据手册中的 7 位设备地址值
  *         在调用此接口前必须左移一位
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 待接收的数据量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
  __IO uint32_t count = 0U;

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待 BUSY 标志位清零 */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);
    do
    {
      count--;
      if (count == 0U)
      {
        hi2c->PreviousState       = I2C_STATE_NONE;
        hi2c->State               = HAL_I2C_STATE_READY;
        hi2c->Mode                = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

        return HAL_BUSY;
      }
    }
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode      = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;
    hi2c->Devaddress  = DevAddress;


    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    /* 注意：必须在当前进程解锁后使能 I2C 中断，
    以避免在当前进程解锁前执行 I2C 中断处理函数的风险 */

    /* 使能 EVT、BUF 和 ERR 中断 */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

    /* 使能应答 */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    /* 生成起始条件 (Start) */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  从机模式下以非阻塞中断方式发送指定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 待发送的数据量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)
{

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    /* 使能地址应答 */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    /* 注意：必须在当前进程解锁后使能 I2C 中断，
              以避免在当前进程解锁之前
              执行 I2C 中断处理函数的风险 */

    /* 使能 EVT、BUF 和 ERR 中断 */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  从机模式下以非阻塞中断方式接收一定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Slave_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)
{

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* 进程锁定 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    /* 使能地址应答 */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    /* 注意：必须在当前进程解锁后使能 I2C 中断，
              以避免在当前进程解锁之前
              执行 I2C 中断处理函数的风险 */

    /* 使能 EVT、BUF 和 ERR 中断 */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在主机模式下使用 DMA 以非阻塞模式发送一定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  DevAddress 目标设备地址：数据手册中的设备 7 位地址值
  *         在调用此接口之前必须左移一位
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
  __IO uint32_t count = 0U;
  HAL_StatusTypeDef dmaxferstatus;

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待 BUSY 标志位复位 */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);
    do
    {
      count--;
      if (count == 0U)
      {
        hi2c->PreviousState       = I2C_STATE_NONE;
        hi2c->State               = HAL_I2C_STATE_READY;
        hi2c->Mode                = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

        return HAL_BUSY;
      }
    }
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已经使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode      = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;
    hi2c->Devaddress  = DevAddress;

    if (hi2c->XferSize > 0U)
    {
      if (hi2c->hdmatx != NULL)
      {
        /* 设置 I2C DMA 传输完成回调函数 */
        hi2c->hdmatx->XferCpltCallback = I2C_DMAXferCplt;

        /* 设置 DMA 错误回调函数 */
        hi2c->hdmatx->XferErrorCallback = I2C_DMAError;

        /* 将未使用的 DMA 回调函数设置为 NULL */
        hi2c->hdmatx->XferHalfCpltCallback = NULL;
        hi2c->hdmatx->XferAbortCallback = NULL;

        /* 使能 DMA 通道 */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DR, hi2c->XferSize);
      }
      else
      {
        /* 更新 I2C 状态 */
        hi2c->State     = HAL_I2C_STATE_READY;
        hi2c->Mode      = HAL_I2C_MODE_NONE;

        /* 更新 I2C 错误码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }

      if (dmaxferstatus == HAL_OK)
      {
        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        /* 注意：必须在解锁当前进程之后使能 I2C 中断，
        以避免在当前进程解锁之前执行 I2C 中断处理的风险 */

        /* 使能 EVT 和 ERR 中断 */
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

        /* 使能 DMA 请求 */
        SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);

        /* 使能应答 (Acknowledge) */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

        /* 生成起始条件 (Start) */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
      }
      else
      {
        /* 更新 I2C 状态 */
        hi2c->State     = HAL_I2C_STATE_READY;
        hi2c->Mode      = HAL_I2C_MODE_NONE;

        /* 更新 I2C 错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;

        /* 进程已解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }
    }
    else
    {
      /* 使能应答 (Acknowledge) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 生成起始条件 (Start) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

      /* 进程已解锁 */
      __HAL_UNLOCK(hi2c);

      /* 注意：必须在解锁当前进程之后使能 I2C 中断，
      以避免在当前进程解锁之前执行 I2C 中断处理的风险 */

      /* 使能 EVT、BUF 和 ERR 中断 */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
    }

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在主机模式下使用 DMA 以非阻塞模式接收指定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  DevAddress 目标设备地址：数据手册中的设备 7 位地址值
  *         在调用该接口前必须进行左移操作
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
  __IO uint32_t count = 0U;
  HAL_StatusTypeDef dmaxferstatus;

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待直到 BUSY 标志位被复位 */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);
    do
    {
      count--;
      if (count == 0U)
      {
        hi2c->PreviousState       = I2C_STATE_NONE;
        hi2c->State               = HAL_I2C_STATE_READY;
        hi2c->Mode                = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

        return HAL_BUSY;
      }
    }
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);

    /* 进程已上锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode      = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;
    hi2c->Devaddress  = DevAddress;

    if (hi2c->XferSize > 0U)
    {
      if (hi2c->hdmarx != NULL)
      {
        /* 设置 I2C DMA 传输完成回调函数 */
        hi2c->hdmarx->XferCpltCallback = I2C_DMAXferCplt;

        /* 设置 DMA 错误回调函数 */
        hi2c->hdmarx->XferErrorCallback = I2C_DMAError;

        /* 将未使用的 DMA 回调函数设置为 NULL */
        hi2c->hdmarx->XferHalfCpltCallback = NULL;
        hi2c->hdmarx->XferAbortCallback = NULL;

        /* 使能 DMA 通道 */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DR, (uint32_t)hi2c->pBuffPtr, hi2c->XferSize);
      }
      else
      {
        /* 更新 I2C 状态 */
        hi2c->State     = HAL_I2C_STATE_READY;
        hi2c->Mode      = HAL_I2C_MODE_NONE;

        /* 更新 I2C 错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;

        /* 进程已解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }

      if (dmaxferstatus == HAL_OK)
      {
        /* 使能应答 (ACK) */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

        /* 生成起始条件 (Start) */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

        /* 进程已解锁 */
        __HAL_UNLOCK(hi2c);

        /* 注意：必须在解锁当前进程之后再使能 I2C 中断，
        以避免在当前进程解锁之前就执行 I2C 中断处理程序的风险 */

        /* 使能事件 (EVT) 和错误 (ERR) 中断 */
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

        /* 使能 DMA 请求 */
        SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);
      }
      else
      {
        /* 更新 I2C 状态 */
        hi2c->State     = HAL_I2C_STATE_READY;
        hi2c->Mode      = HAL_I2C_MODE_NONE;

        /* 更新 I2C 错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;

        /* 进程已解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }
    }
    else
    {
      /* 进程已解锁 */
      __HAL_UNLOCK(hi2c);

      /* 注意：必须在解锁当前进程之后再使能 I2C 中断，
      以避免在当前进程解锁之前就执行 I2C 中断处理程序的风险 */

      /* 使能事件 (EVT)、缓冲区 (BUF) 和错误 (ERR) 中断 */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

      /* 使能应答 (Acknowledge) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 生成起始条件 (Start) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
    }

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在从机模式下使用 DMA 以非阻塞方式发送指定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)
{
  HAL_StatusTypeDef dmaxferstatus;

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* 进程已锁定 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已经使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    if (hi2c->hdmatx != NULL)
    {
      /* 设置 I2C DMA 传输完成回调函数 */
      hi2c->hdmatx->XferCpltCallback = I2C_DMAXferCplt;

      /* 设置 DMA 错误回调函数 */
      hi2c->hdmatx->XferErrorCallback = I2C_DMAError;

      /* 将未使用的 DMA 回调函数设置为 NULL */
      hi2c->hdmatx->XferHalfCpltCallback = NULL;
      hi2c->hdmatx->XferAbortCallback = NULL;

      /* 使能 DMA 通道 */
      dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DR, hi2c->XferSize);
    }
    else
    {
      /* 更新 I2C 状态 */
      hi2c->State     = HAL_I2C_STATE_LISTEN;
      hi2c->Mode      = HAL_I2C_MODE_NONE;

      /* 更新 I2C 错误代码 */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }

    if (dmaxferstatus == HAL_OK)
    {
      /* 使能地址应答 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      /* 注意：I2C 中断必须在当前进程解锁后使能，
      以避免在当前进程解锁之前执行 I2C 中断处理程序的风险 */
      /* 使能 EVT 和 ERR 中断 */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

      /* 使能 DMA 请求 */
      hi2c->Instance->CR2 |= I2C_CR2_DMAEN;

      return HAL_OK;
    }
    else
    {
      /* 更新 I2C 状态 */
      hi2c->State     = HAL_I2C_STATE_READY;
      hi2c->Mode      = HAL_I2C_MODE_NONE;

      /* 更新 I2C 错误代码 */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  从机模式下通过 DMA 以非阻塞方式接收指定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 待发送的数据量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Slave_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)
{
  HAL_StatusTypeDef dmaxferstatus;

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已经使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    if (hi2c->hdmarx != NULL)
    {
      /* 设置 I2C DMA 传输完成回调函数 */
      hi2c->hdmarx->XferCpltCallback = I2C_DMAXferCplt;

      /* 设置 DMA 错误回调函数 */
      hi2c->hdmarx->XferErrorCallback = I2C_DMAError;

      /* 将未使用的 DMA 回调函数设置为 NULL */
      hi2c->hdmarx->XferHalfCpltCallback = NULL;
      hi2c->hdmarx->XferAbortCallback = NULL;

      /* 使能 DMA 通道 */
      dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DR, (uint32_t)hi2c->pBuffPtr, hi2c->XferSize);
    }
    else
    {
      /* 更新 I2C 状态 */
      hi2c->State     = HAL_I2C_STATE_LISTEN;
      hi2c->Mode      = HAL_I2C_MODE_NONE;

      /* 更新 I2C 错误代码 */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }

    if (dmaxferstatus == HAL_OK)
    {
      /* 使能地址应答 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      /* 注意：必须在当前进程解锁后使能 I2C 中断，
      以避免在当前进程解锁之前
      执行 I2C 中断处理程序的风险 */
      /* 使能事件 (EVT) 和错误 (ERR) 中断 */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

      /* 使能 DMA 请求 */
      SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);

      return HAL_OK;
    }
    else
    {
      /* 更新 I2C 状态 */
      hi2c->State     = HAL_I2C_STATE_READY;
      hi2c->Mode      = HAL_I2C_MODE_NONE;

      /* 更新 I2C 错误代码 */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  以阻塞模式向特定的内存地址写入指定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @param  DevAddress 目标设备地址：在调用该接口之前，数据手册中的
  *         设备 7 位地址值必须进行左移
  * @param  MemAddress 内部内存地址
  * @param  MemAddSize 内部内存地址大小
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  Timeout 超时时间
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  /* 初始化 tickstart 用于超时管理 */
  uint32_t tickstart = HAL_GetTick();

  /* 检查参数有效性 */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待 BUSY 标志位复位 */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)
    {
      return HAL_BUSY;
    }

    /* 进程上锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode      = HAL_I2C_MODE_MEM;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    /* 发送从机地址和内存地址 */
    if (I2C_RequestMemoryWrite(hi2c, DevAddress, MemAddress, MemAddSize, Timeout, tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    while (hi2c->XferSize > 0U)
    {
      /* 等待 TXE 标志位置位 */
      if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
      {
        if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)
        {
          /* 生成停止条件 */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
        }
        return HAL_ERROR;
      }

      /* 将数据写入 DR */
      hi2c->Instance->DR = *hi2c->pBuffPtr;

      /* 缓冲区指针递增 */
      hi2c->pBuffPtr++;

      /* 更新计数器 */
      hi2c->XferSize--;
      hi2c->XferCount--;

      if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET) && (hi2c->XferSize != 0U))
      {
        /* 将数据写入 DR */
        hi2c->Instance->DR = *hi2c->pBuffPtr;

        /* 缓冲区指针递增 */
        hi2c->pBuffPtr++;

        /* 更新计数器 */
        hi2c->XferSize--;
        hi2c->XferCount--;
      }
    }

    /* 等待 BTF 标志位置位 */
    if (I2C_WaitOnBTFFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
    {
      if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)
      {
        /* 生成停止条件 */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
      }
      return HAL_ERROR;
    }

    /* 生成停止条件 */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->Mode = HAL_I2C_MODE_NONE;

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  以阻塞模式从指定的内存地址读取指定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  DevAddress 目标设备地址：在调用该接口前，数据手册中的设备 7 位地址值
  *         必须左移一位
  * @param  MemAddress 内部内存地址
  * @param  MemAddSize 内部内存地址大小
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  Timeout 超时时间
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  __IO uint32_t count = 0U;

  /* 初始化 tickstart 用于超时管理 */
  uint32_t tickstart = HAL_GetTick();

  /* 检查参数 */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待 BUSY 标志位复位 */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)
    {
      return HAL_BUSY;
    }

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode      = HAL_I2C_MODE_MEM;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    /* 发送从机地址和内存地址 */
    if (I2C_RequestMemoryRead(hi2c, DevAddress, MemAddress, MemAddSize, Timeout, tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    if (hi2c->XferSize == 0U)
    {
      /* 清除 ADDR 标志位 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

      /* 生成停止条件 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
    }
    else if (hi2c->XferSize == 1U)
    {
      /* 禁用应答 (NACK) */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 在清除 ADDR 和配置 STOP 期间禁用所有活动的中断，因为 EV6_3
         软件序列必须在当前字节传输结束前完成 */
      __disable_irq();

      /* 清除 ADDR 标志位 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

      /* 生成停止条件 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

      /* 重新使能中断 */
      __enable_irq();
    }
    else if (hi2c->XferSize == 2U)
    {
      /* 使能 Pos */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

      /* 在清除 ADDR 标志和配置 STOP 期间禁用所有活动中断，因为 EV6_3
         软件序列必须在当前字节传输结束前完成 */
      __disable_irq();

      /* 清除 ADDR 标志 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

      /* 禁用应答 (NACK) */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 重新使能中断 */
      __enable_irq();
    }
    else
    {
      /* 使能应答 (ACK) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
      /* 清除 ADDR 标志 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
    }

    while (hi2c->XferSize > 0U)
    {
      if (hi2c->XferSize <= 3U)
      {
        /* 单字节 */
        if (hi2c->XferSize == 1U)
        {
          /* 等待 RXNE 标志置位 */
          if (I2C_WaitOnRXNEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
          {
            return HAL_ERROR;
          }

          /* 从 DR 寄存器读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 递增缓冲区指针 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;
        }
        /* 双字节 */
        else if (hi2c->XferSize == 2U)
        {
          /* 等待 BTF 标志置位 */
          if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout, tickstart) != HAL_OK)
          {
            return HAL_ERROR;
          }

          /* 在清除 ADDR 和配置 STOP 期间禁用所有活动中断，因为 EV6_3
             软件序列必须在当前字节传输结束前完成 */
          __disable_irq();

          /* 生成停止条件 */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

          /* 从 DR 读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 缓冲区指针递增 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;

          /* 重新使能中断 */
          __enable_irq();

          /* 从 DR 读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 缓冲区指针递增 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;
        }
        /* 最后 3 个字节 */
        else
        {
          /* 等待 BTF 标志置位 */
          if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout, tickstart) != HAL_OK)
          {
            return HAL_ERROR;
          }

          /* 禁用应答 (ACK) */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

          /* 在清除 ADDR 和配置 STOP 期间禁用所有活动中断，因为 EV6_3
             软件序列必须在当前字节传输结束前完成 */
          __disable_irq();

          /* 从 DR 读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 递增缓冲区指针 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;

          /* 等待 BTF 标志位置位 */
          count = I2C_TIMEOUT_FLAG * (SystemCoreClock / 25U / 1000U);
          do
          {
            count--;
            if (count == 0U)
            {
              hi2c->PreviousState       = I2C_STATE_NONE;
              hi2c->State               = HAL_I2C_STATE_READY;
              hi2c->Mode                = HAL_I2C_MODE_NONE;
              hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

              /* 重新使能中断 */
              __enable_irq();

              /* 进程解锁 */
              __HAL_UNLOCK(hi2c);

              return HAL_ERROR;
            }
          }
          while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == RESET);

          /* 生成停止信号 */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

          /* 从 DR 读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 递增缓冲区指针 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;

          /* 重新使能中断 */
          __enable_irq();

          /* 从 DR 读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 递增缓冲区指针 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;
        }
      }
      else
      {
        /* 等待 RXNE 标志位置位 */
        if (I2C_WaitOnRXNEFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
        {
          return HAL_ERROR;
        }

        /* 从 DR 读取数据 */
        *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

        /* 递增缓冲区指针 */
        hi2c->pBuffPtr++;

        /* 更新计数器 */
        hi2c->XferSize--;
        hi2c->XferCount--;

        if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET)
        {
          if (hi2c->XferSize == 3U)
          {
            /* 禁用应答 (ACK) */
            CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
          }
          /* 从 DR 寄存器读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 递增缓冲区指针 */
          hi2c->pBuffPtr++;

          /* 更新计数器 */
          hi2c->XferSize--;
          hi2c->XferCount--;
        }
      }
    }

    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->Mode = HAL_I2C_MODE_NONE;

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  以中断非阻塞模式向特定内存地址写入指定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针
  * @param  DevAddress 目标设备地址：数据手册中的设备 7 位地址值
  *         在调用该接口前必须进行左移
  * @param  MemAddress 内部内存地址
  * @param  MemAddSize 内部内存地址大小
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Mem_Write_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
  __IO uint32_t count = 0U;

  /* 检查参数有效性 */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待 BUSY 标志位被复位 */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);
    do
    {
      count--;
      if (count == 0U)
      {
        hi2c->PreviousState       = I2C_STATE_NONE;
        hi2c->State               = HAL_I2C_STATE_READY;
        hi2c->Mode                = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

        return HAL_BUSY;
      }
    }
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos (POS 位) */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode      = HAL_I2C_MODE_MEM;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;
    hi2c->Devaddress  = DevAddress;
    hi2c->Memaddress  = MemAddress;
    hi2c->MemaddSize  = MemAddSize;
    hi2c->EventCount  = 0U;

    /* 生成起始条件 */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    /* 注意：必须在解锁当前进程之后使能 I2C 中断，
    以避免在当前进程解锁之前执行 I2C 中断处理的风险 */

    /* 使能 EVT、BUF 和 ERR 中断 */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在中断非阻塞模式下从特定内存地址读取指定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针
  * @param  DevAddress 目标设备地址
  * @param  MemAddress 内部内存地址
  * @param  MemAddSize 内部内存地址大小
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Mem_Read_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
  __IO uint32_t count = 0U;

  /* 检查参数 */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待直到 BUSY 标志位复位 */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);
    do
    {
      count--;
      if (count == 0U)
      {
        hi2c->PreviousState       = I2C_STATE_NONE;
        hi2c->State               = HAL_I2C_STATE_READY;
        hi2c->Mode                = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

        return HAL_BUSY;
      }
    }
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已被使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode      = HAL_I2C_MODE_MEM;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;
    hi2c->Devaddress  = DevAddress;
    hi2c->Memaddress  = MemAddress;
    hi2c->MemaddSize  = MemAddSize;
    hi2c->EventCount  = 0U;

    /* 使能应答 */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    /* 生成起始条件 */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    if (hi2c->XferSize > 0U)
    {
      /* 注意：必须在解锁当前进程之后再使能 I2C 中断，
      以避免在当前进程解锁之前就执行 I2C 中断处理函数的风险 */

      /* 使能 EVT、BUF 和 ERR 中断 */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
    }
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在非阻塞模式下使用 DMA 向特定内存地址写入一定量的数据
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  DevAddress 目标设备地址：在调用该接口前，数据手册中的设备 7 位地址值
  *         必须左移一位
  * @param  MemAddress 内部内存地址
  * @param  MemAddSize 内部内存地址大小
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Mem_Write_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
  __IO uint32_t count = 0U;
  HAL_StatusTypeDef dmaxferstatus;

  /* 初始化 tickstart 用于超时管理 */
  uint32_t tickstart = HAL_GetTick();

  /* 检查参数有效性 */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待直到 BUSY 标志位被复位 */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);
    do
    {
      count--;
      if (count == 0U)
      {
        hi2c->PreviousState       = I2C_STATE_NONE;
        hi2c->State               = HAL_I2C_STATE_READY;
        hi2c->Mode                = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

        return HAL_BUSY;
      }
    }
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode      = HAL_I2C_MODE_MEM;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;
    hi2c->Devaddress  = DevAddress;
    hi2c->Memaddress  = MemAddress;
    hi2c->MemaddSize  = MemAddSize;
    hi2c->EventCount  = 0U;

    if (hi2c->XferSize > 0U)
    {
      if (hi2c->hdmatx != NULL)
      {
        /* 设置 I2C DMA 传输完成回调函数 */
        hi2c->hdmatx->XferCpltCallback = I2C_DMAXferCplt;

        /* 设置 DMA 错误回调函数 */
        hi2c->hdmatx->XferErrorCallback = I2C_DMAError;

        /* 将未使用的 DMA 回调函数设置为 NULL */
        hi2c->hdmatx->XferHalfCpltCallback = NULL;
        hi2c->hdmatx->XferAbortCallback = NULL;

        /* 使能 DMA 通道 */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DR, hi2c->XferSize);
      }
      else
      {
        /* 更新 I2C 状态 */
        hi2c->State     = HAL_I2C_STATE_READY;
        hi2c->Mode      = HAL_I2C_MODE_NONE;

        /* 更新 I2C 错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }

      if (dmaxferstatus == HAL_OK)
      {
        /* 发送从机地址和内存地址 */
        if (I2C_RequestMemoryWrite(hi2c, DevAddress, MemAddress, MemAddSize, I2C_TIMEOUT_FLAG, tickstart) != HAL_OK)
        {
          /* 中止正在进行的 DMA 传输 */
          dmaxferstatus = HAL_DMA_Abort_IT(hi2c->hdmatx);

          /* 防止未使用的参数产生编译和 MISRA 警告 */
          UNUSED(dmaxferstatus);

          /* 将未使用的 I2C DMA 传输完成回调函数设置为 NULL */
          hi2c->hdmatx->XferCpltCallback = NULL;

          /* 禁用应答 (ACK) */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

          hi2c->XferSize = 0U;
          hi2c->XferCount = 0U;

          /* 禁用 I2C 外设以防止缓冲区中产生虚假数据 */
          __HAL_I2C_DISABLE(hi2c);

          return HAL_ERROR;
        }

        /* 清除 ADDR 标志位 */
        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        /* 注意：必须在解锁当前进程之后使能 I2C 中断，
        以避免在当前进程解锁之前就执行 I2C 中断处理程序的风险 */
        /* 使能 ERR 中断 */
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_ERR);

        /* 使能 DMA 请求 */
        SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);

        return HAL_OK;
      }
      else
      {
        /* 更新 I2C 状态 */
        hi2c->State     = HAL_I2C_STATE_READY;
        hi2c->Mode      = HAL_I2C_MODE_NONE;

        /* 更新 I2C 错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }
    }
    else
    {
      /* 更新 I2C 状态 */
      hi2c->State     = HAL_I2C_STATE_READY;
      hi2c->Mode      = HAL_I2C_MODE_NONE;

      /* 更新 I2C 错误代码 */
      hi2c->ErrorCode |= HAL_I2C_ERROR_SIZE;

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  使用 DMA 以非阻塞模式从指定内存地址读取指定量的数据。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  DevAddress 目标设备地址：在调用该接口之前，必须将数据手册中的设备 7 位地址值左移
  * @param  MemAddress 内部内存地址
  * @param  MemAddSize 内部内存地址大小
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要读取的数据量
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Mem_Read_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
  /* 初始化 tickstart 用于超时管理 */
  uint32_t tickstart = HAL_GetTick();
  __IO uint32_t count = 0U;
  HAL_StatusTypeDef dmaxferstatus;

  /* 检查参数 */
  assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待 BUSY 标志位复位 */
    count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);
    do
    {
      count--;
      if (count == 0U)
      {
        hi2c->PreviousState       = I2C_STATE_NONE;
        hi2c->State               = HAL_I2C_STATE_READY;
        hi2c->Mode                = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

        return HAL_BUSY;
      }
    }
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode      = HAL_I2C_MODE_MEM;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;
    hi2c->Devaddress  = DevAddress;
    hi2c->Memaddress  = MemAddress;
    hi2c->MemaddSize  = MemAddSize;
    hi2c->EventCount  = 0U;

    if (hi2c->XferSize > 0U)
    {
      if (hi2c->hdmarx != NULL)
      {
        /* 设置 I2C DMA 传输完成回调函数 */
        hi2c->hdmarx->XferCpltCallback = I2C_DMAXferCplt;

        /* 设置 DMA 错误回调函数 */
        hi2c->hdmarx->XferErrorCallback = I2C_DMAError;

        /* 将未使用的 DMA 回调函数置为 NULL */
        hi2c->hdmarx->XferHalfCpltCallback = NULL;
        hi2c->hdmarx->XferAbortCallback = NULL;

        /* 使能 DMA 通道 */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DR, (uint32_t)hi2c->pBuffPtr, hi2c->XferSize);
      }
      else
      {
        /* 更新 I2C 状态 */
        hi2c->State     = HAL_I2C_STATE_READY;
        hi2c->Mode      = HAL_I2C_MODE_NONE;

        /* 更新 I2C 错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }

      if (dmaxferstatus == HAL_OK)
      {
        /* 发送从机地址和内存地址 */
        if (I2C_RequestMemoryRead(hi2c, DevAddress, MemAddress, MemAddSize, I2C_TIMEOUT_FLAG, tickstart) != HAL_OK)
        {
          /* 中止正在进行的 DMA 传输 */
          dmaxferstatus = HAL_DMA_Abort_IT(hi2c->hdmarx);

          /* 防止未使用的参数产生编译及 MISRA 警告 */
          UNUSED(dmaxferstatus);

          /* 将未使用的 I2C DMA 传输完成回调函数置为 NULL */
          hi2c->hdmarx->XferCpltCallback = NULL;

          /* 禁用应答 (ACK) */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

          hi2c->XferSize = 0U;
          hi2c->XferCount = 0U;

          /* 禁用 I2C 外设以防止缓冲区中产生伪数据 */
          __HAL_I2C_DISABLE(hi2c);

          return HAL_ERROR;
        }

        if (hi2c->XferSize == 1U)
        {
          /* 禁用应答 (ACK) */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
        }
        else
        {
          /* 使能 Last DMA 位 */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);
        }

        /* 清除 ADDR 标志位 */
        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        /* 注意：必须在解锁当前进程后使能 I2C 中断，
        以避免在当前进程解锁之前执行 I2C 中断处理函数的风险 */
        /* 使能 ERR 中断 */
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_ERR);

        /* 使能 DMA 请求 */
        hi2c->Instance->CR2 |= I2C_CR2_DMAEN;
      }
      else
      {
        /* 更新 I2C 状态 */
        hi2c->State     = HAL_I2C_STATE_READY;
        hi2c->Mode      = HAL_I2C_MODE_NONE;

        /* 更新 I2C 错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }
    }
    else
    {
      /* 发送从机地址和内存地址 */
      if (I2C_RequestMemoryRead(hi2c, DevAddress, MemAddress, MemAddSize, I2C_TIMEOUT_FLAG, tickstart) != HAL_OK)
      {
        return HAL_ERROR;
      }

      /* 清除 ADDR 标志 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

      /* 生成停止信号 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

      hi2c->State = HAL_I2C_STATE_READY;

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);
    }

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  检查目标设备是否已准备好进行通信。
  * @note   此函数用于存储器设备
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @param  DevAddress 目标设备地址：在调用该接口之前，数据手册中的设备 7 位地址值
  *         必须先左移一位
  * @param  Trials 尝试次数
  * @param  Timeout 超时时间
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)
{
  /* 获取滴答计数值 */
  uint32_t tickstart = HAL_GetTick();
  uint32_t I2C_Trials = 0U;
  FlagStatus tmp1;
  FlagStatus tmp2;

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 等待直到 BUSY 标志位被复位 */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)
    {
      return HAL_BUSY;
    }

    /* 进程锁定 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State = HAL_I2C_STATE_BUSY;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    do
    {
      /* 生成起始条件 (Start) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

      /* 等待 SB 标志位置位 */
      if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, tickstart) != HAL_OK)
      {
        if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)
        {
          hi2c->ErrorCode = HAL_I2C_WRONG_START;
        }
        return HAL_TIMEOUT;
      }

      /* 发送从机地址 */
      hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(DevAddress);

      /* 等待 ADDR 或 AF 标志位置位 */
      /* 获取系统滴答计数值 (tick) */
      tickstart = HAL_GetTick();

      tmp1 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_ADDR);
      tmp2 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF);
      while ((hi2c->State != HAL_I2C_STATE_TIMEOUT) && (tmp1 == RESET) && (tmp2 == RESET))
      {
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))
        {
          hi2c->State = HAL_I2C_STATE_TIMEOUT;
        }
        tmp1 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_ADDR);
        tmp2 = __HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF);
      }

      hi2c->State = HAL_I2C_STATE_READY;

      /* 检查 ADDR 标志位是否已置位 */
      if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_ADDR) == SET)
      {
        /* 生成停止条件 (Stop) */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

        /* 清除 ADDR 标志位 */
        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

        /* 等待 BUSY 标志位复位 */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)
        {
          return HAL_ERROR;
        }

        hi2c->State = HAL_I2C_STATE_READY;

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_OK;
      }
      else
      {
        /* 生成停止条件 (Stop) */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

        /* 清除 AF 标志位 */
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

        /* 等待 BUSY 标志位复位 */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart) != HAL_OK)
        {
          return HAL_ERROR;
        }
      }

      /* 增加尝试次数 */
      I2C_Trials++;
    }
    while (I2C_Trials < Trials);

    hi2c->State = HAL_I2C_STATE_READY;

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    return HAL_ERROR;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在 I2C 主机模式下，通过中断以非阻塞方式连续发送一定数量的数据。
  * @note   此接口允许在传输过程中方向改变时管理重复起始条件（Repeated Start）
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  DevAddress 目标设备地址：数据手册中的 7 位设备地址值
  *         在调用该接口前必须先进行左移
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  XferOptions 传输配置选项，取值见 @ref I2C_XferOptions_definition
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Master_Seq_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
  __IO uint32_t Prev_State = 0x00U;
  __IO uint32_t count      = 0x00U;

  /* 检查参数 */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 仅在首次调用主机接口时检查 Busy 标志 */
    if ((READ_BIT(hi2c->Instance->CR1, I2C_CR1_STOP) == I2C_CR1_STOP) || (XferOptions == I2C_FIRST_AND_LAST_FRAME) || (XferOptions == I2C_FIRST_FRAME))
    {
      /* 等待 BUSY 标志位复位 */
      count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);
      do
      {
        count--;
        if (count == 0U)
        {
          hi2c->PreviousState       = I2C_STATE_NONE;
          hi2c->State               = HAL_I2C_STATE_READY;
          hi2c->Mode                = HAL_I2C_MODE_NONE;
          hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

          return HAL_BUSY;
        }
      }
      while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);
    }

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode      = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = XferOptions;
    hi2c->Devaddress  = DevAddress;

    Prev_State = hi2c->PreviousState;

    /* 如果传输方向未改变且无启动新帧请求，则不生成重复起始条件（Restart Condition） */
    /* 表示先前的状态与当前状态相同 */
    if ((Prev_State != I2C_STATE_MASTER_BUSY_TX) || (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 1))
    {
      /* 生成起始条件（Start） */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
    }

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    /* 注意：必须在解锁当前进程之后使能 I2C 中断，
    以避免在当前进程解锁之前执行 I2C 中断处理函数的风险 */

    /* 使能 EVT、BUF 和 ERR 中断 */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在 I2C 主机模式下使用 DMA 以非阻塞方式连续发送一定数量的数据。
  * @note   该接口允许在传输过程中发生方向改变时管理重复起始条件（Repeated Start）
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  DevAddress 目标设备地址：在调用该接口前，数据手册中的 7 位设备地址值
  *         必须先左移一位
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  XferOptions 传输选项，参考 @ref I2C_XferOptions_definition 的值
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Master_Seq_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
  __IO uint32_t Prev_State = 0x00U;
  __IO uint32_t count      = 0x00U;
  HAL_StatusTypeDef dmaxferstatus;

  /* 检查参数 */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 仅在首次调用主机接口时检查 Busy 标志 */
    if ((READ_BIT(hi2c->Instance->CR1, I2C_CR1_STOP) == I2C_CR1_STOP) || (XferOptions == I2C_FIRST_AND_LAST_FRAME) || (XferOptions == I2C_FIRST_FRAME))
    {
      /* 等待 BUSY 标志被复位 */
      count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);
      do
      {
        count--;
        if (count == 0U)
        {
          hi2c->PreviousState       = I2C_STATE_NONE;
          hi2c->State               = HAL_I2C_STATE_READY;
          hi2c->Mode                = HAL_I2C_MODE_NONE;
          hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

          return HAL_BUSY;
        }
      }
      while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);
    }

    /* 进程上锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode      = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = XferOptions;
    hi2c->Devaddress  = DevAddress;

    Prev_State = hi2c->PreviousState;

    if (hi2c->XferSize > 0U)
    {
      if (hi2c->hdmatx != NULL)
      {
        /* 设置 I2C DMA 传输完成回调函数 */
        hi2c->hdmatx->XferCpltCallback = I2C_DMAXferCplt;

        /* 设置 DMA 错误回调函数 */
        hi2c->hdmatx->XferErrorCallback = I2C_DMAError;

        /* 将未使用的 DMA 回调函数设置为 NULL */
        hi2c->hdmatx->XferHalfCpltCallback = NULL;
        hi2c->hdmatx->XferAbortCallback = NULL;

        /* 使能 DMA 通道 */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DR, hi2c->XferSize);
      }
      else
      {
        /* 更新 I2C 状态 */
        hi2c->State     = HAL_I2C_STATE_READY;
        hi2c->Mode      = HAL_I2C_MODE_NONE;

        /* 更新 I2C 错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }

      if (dmaxferstatus == HAL_OK)
      {
        /* 使能应答 (ACK) */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

        /* 若传输方向未改变且无需启动新的一帧，则不生成重复起始条件 (Restart Condition) */
        /* 意味着先前状态与当前状态相同 */
        if ((Prev_State != I2C_STATE_MASTER_BUSY_TX) || (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 1))
        {
          /* 生成起始条件 (Start) */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
        }

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        /* 注意：必须在当前进程解锁后使能 I2C 中断，
        以避免在当前进程解锁前执行 I2C 中断处理函数的风险 */

        /* 若 XferOptions 未关联到新帧，即不需要发送起始位，则直接使能 DMA 请求 */
        /* 在其他情况下，DMA 请求将在 IRQHandler 中处理从机地址之后使能 */
        if ((XferOptions == I2C_NEXT_FRAME) || (XferOptions == I2C_LAST_FRAME) || (XferOptions == I2C_LAST_FRAME_NO_STOP))
        {
          /* 使能 DMA 请求 */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);
        }

        /* 使能 EVT 和 ERR 中断 */
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);
      }
      else
      {
        /* 更新 I2C 状态 */
        hi2c->State     = HAL_I2C_STATE_READY;
        hi2c->Mode      = HAL_I2C_MODE_NONE;

        /* 更新 I2C 错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }
    }
    else
    {
      /* 使能应答 (ACK) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 若传输方向未改变且没有启动另一帧的请求，则不产生重复起始条件 (Restart Condition) */
      /* 表示先前状态与当前状态相同 */
      if ((Prev_State != I2C_STATE_MASTER_BUSY_TX) || (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 1))
      {
        /* 产生起始条件 (Start) */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
      }

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      /* 注意：必须在当前进程解锁后使能 I2C 中断，
      以避免在当前进程解锁前执行 I2C 中断处理函数的风险 */

      /* 使能 EVT（事件）、BUF（缓冲区）和 ERR（错误）中断 */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
    }

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  I2C 主机模式下使用中断以非阻塞方式连续接收指定量的数据
  * @note   该接口允许在传输过程中改变传输方向时管理重复起始条件 (Repeated Start)
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针
  * @param  DevAddress 目标设备地址：数据手册中的 7 位设备地址值
  *         在调用此接口前必须左移一位
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 待发送的数据量
  * @param  XferOptions 传输选项，取值参考 @ref I2C_XferOptions_definition
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Master_Seq_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
  __IO uint32_t Prev_State = 0x00U;
  __IO uint32_t count = 0U;
  uint32_t enableIT = (I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

  /* 检查参数有效性 */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 仅在首次调用主机接口时检查 Busy 标志 */
    if ((READ_BIT(hi2c->Instance->CR1, I2C_CR1_STOP) == I2C_CR1_STOP) || (XferOptions == I2C_FIRST_AND_LAST_FRAME) || (XferOptions == I2C_FIRST_FRAME))
    {
      /* 等待 BUSY 标志被复位 */
      count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);
      do
      {
        count--;
        if (count == 0U)
        {
          hi2c->PreviousState       = I2C_STATE_NONE;
          hi2c->State               = HAL_I2C_STATE_READY;
          hi2c->Mode                = HAL_I2C_MODE_NONE;
          hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

          return HAL_BUSY;
        }
      }
      while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);
    }

    /* 进程上锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已被使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode      = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = XferOptions;
    hi2c->Devaddress  = DevAddress;

    Prev_State = hi2c->PreviousState;

    if ((hi2c->XferCount == 2U) && ((XferOptions == I2C_LAST_FRAME) || (XferOptions == I2C_LAST_FRAME_NO_STOP)))
    {
      if (Prev_State == I2C_STATE_MASTER_BUSY_RX)
      {
        /* 禁用应答 (Acknowledge) */
        CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

        /* 使能 Pos */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

        /* 清除 IT_BUF 的使能，即取消 RXNE 处理，改由 BTF 事件处理这 2 个字节 */
        enableIT &= ~I2C_IT_BUF;
      }
      else
      {
        /* 使能应答 (Acknowledge) */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
      }
    }
    else
    {
      /* 使能应答 (Acknowledge) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
    }

    /* 若传输方向未改变且无需启动下一帧传输，则不产生重复起始条件 (Restart Condition) */
    /* 表示先前的状态与当前状态相同 */
    if ((Prev_State != I2C_STATE_MASTER_BUSY_RX) || (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 1))
    {
      /* 生成起始信号 (Start) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
    }

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    /* 注意：必须在解锁当前进程之后再使能 I2C 中断，
    以避免在当前进程解锁前就执行 I2C 中断处理函数的风险 */

    /* 使能中断 */
    __HAL_I2C_ENABLE_IT(hi2c, enableIT);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在主机模式下使用 DMA 以非阻塞方式连续接收指定量的数据
  * @note   该接口允许在传输过程中改变传输方向时处理重复起始条件 (repeated start condition)
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  DevAddress 目标设备地址：在调用该接口前，数据手册中的设备 7 位地址值
  *         必须先左移一位
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  XferOptions 传输选项，取值参考 @ref I2C_XferOptions_definition
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Master_Seq_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
  __IO uint32_t Prev_State = 0x00U;
  __IO uint32_t count = 0U;
  uint32_t enableIT = (I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
  HAL_StatusTypeDef dmaxferstatus;

  /* 检查参数 */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));

  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    /* 仅在首次调用主机接口时检查忙标志位 (Busy Flag) */
    if ((READ_BIT(hi2c->Instance->CR1, I2C_CR1_STOP) == I2C_CR1_STOP) || (XferOptions == I2C_FIRST_AND_LAST_FRAME) || (XferOptions == I2C_FIRST_FRAME))
    {
      /* 等待 BUSY 标志位清零 */
      count = I2C_TIMEOUT_BUSY_FLAG * (SystemCoreClock / 25U / 1000U);
      do
      {
        count--;
        if (count == 0U)
        {
          hi2c->PreviousState       = I2C_STATE_NONE;
          hi2c->State               = HAL_I2C_STATE_READY;
          hi2c->Mode                = HAL_I2C_MODE_NONE;
          hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

          return HAL_BUSY;
        }
      }
      while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET);
    }

    /* 进程已锁定 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已被使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    /* 清除 Last DMA 位 */
    CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);

    hi2c->State     = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode      = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = XferOptions;
    hi2c->Devaddress  = DevAddress;

    Prev_State = hi2c->PreviousState;

    if (hi2c->XferSize > 0U)
    {
      if ((hi2c->XferCount == 2U) && ((XferOptions == I2C_LAST_FRAME) || (XferOptions == I2C_LAST_FRAME_NO_STOP)))
      {
        if (Prev_State == I2C_STATE_MASTER_BUSY_RX)
        {
          /* 禁用应答 (Acknowledge) */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

          /* 使能 Pos */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

          /* 使能 Last DMA 位 */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);
        }
        else
        {
          /* 使能应答 (Acknowledge) */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
        }
      }
      else
      {
        /* 使能应答 (Acknowledge) */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

        if ((XferOptions == I2C_LAST_FRAME) || (XferOptions == I2C_OTHER_AND_LAST_FRAME) || (XferOptions == I2C_LAST_FRAME_NO_STOP))
        {
          /* 使能 Last DMA 位 */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);
        }
      }
      if (hi2c->hdmarx != NULL)
      {
        /* 设置 I2C DMA 传输完成回调函数 */
        hi2c->hdmarx->XferCpltCallback = I2C_DMAXferCplt;

        /* 设置 DMA 错误回调函数 */
        hi2c->hdmarx->XferErrorCallback = I2C_DMAError;

        /* 将未使用的 DMA 回调函数置为 NULL */
        hi2c->hdmarx->XferHalfCpltCallback = NULL;
        hi2c->hdmarx->XferAbortCallback = NULL;

        /* 使能 DMA 通道 */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DR, (uint32_t)hi2c->pBuffPtr, hi2c->XferSize);
      }
      else
      {
        /* 更新 I2C 状态 */
        hi2c->State     = HAL_I2C_STATE_READY;
        hi2c->Mode      = HAL_I2C_MODE_NONE;

        /* 更新 I2C 错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;

        /* 解锁进程 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }
      if (dmaxferstatus == HAL_OK)
      {
        /* 如果传输方向未改变且无启动新帧的请求，则不生成重复起始条件（Restart Condition） */
        /* 表示先前的状态与当前状态相同 */
        if ((Prev_State != I2C_STATE_MASTER_BUSY_RX) || (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 1))
        {
          /* 生成起始条件（Start） */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

          /* 仅更新事件（EVT）和错误（ERR）中断 */
          enableIT = (I2C_IT_EVT | I2C_IT_ERR);
        }
        else
        {
          /* 仅更新错误（ERR）中断 */
          enableIT = I2C_IT_ERR;
        }

        /* 解锁进程 */
        __HAL_UNLOCK(hi2c);

        /* 注意：必须在解锁当前进程之后再使能 I2C 中断，
        以避免在当前进程解锁之前就执行 I2C 中断处理函数的风险 */

        /* 如果 XferOptions 未关联到新帧，即未请求起始位，则直接使能 DMA 请求 */
        /* 在其他情况下，DMA 请求将在 IRQHandler 中处理完从机地址后使能 */
        if ((XferOptions == I2C_NEXT_FRAME) || (XferOptions == I2C_LAST_FRAME) || (XferOptions == I2C_LAST_FRAME_NO_STOP))
        {
          /* 使能 DMA 请求 */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);
        }

        /* 使能事件 (EVT) 和错误 (ERR) 中断 */
        __HAL_I2C_ENABLE_IT(hi2c, enableIT);
      }
      else
      {
        /* 更新 I2C 状态 */
        hi2c->State     = HAL_I2C_STATE_READY;
        hi2c->Mode      = HAL_I2C_MODE_NONE;

        /* 更新 I2C 错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }
    }
    else
    {
      /* 使能应答 (ACK) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 如果传输方向未改变且没有启动另一帧的请求，则不生成重复起始条件 (Restart Condition) */
      /* 意味着先前状态与当前状态相同 */
      if ((Prev_State != I2C_STATE_MASTER_BUSY_RX) || (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(XferOptions) == 1))
      {
        /* 生成起始条件 (Start) */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
      }

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      /* 注意：必须在解锁当前进程后使能 I2C 中断，
      以避免在当前进程解锁之前执行 I2C 中断处理的风险 */

      /* 使能中断 */
      __HAL_I2C_ENABLE_IT(hi2c, enableIT);
    }
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  在从机模式下以中断非阻塞方式连续发送指定量的数据
  * @note   此接口允许在传输过程中方向改变时管理重复起始条件 (repeated start condition)
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  XferOptions 传输选项，参考 @ref I2C_XferOptions_definition 的值
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
  /* 检查参数 */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));

  if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_TX_LISTEN;
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = XferOptions;

    /* 清除 ADDR 标志位 */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    /* 注意：必须在当前进程解锁之后再使能 I2C 中断，
              以避免在当前进程解锁之前执行 I2C 中断处理的风险 */

    /* 使能 EVT、BUF 和 ERR 中断 */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  从机模式下使用 DMA 以非阻塞方式连续发送指定量的数据
  * @note   该接口允许在传输过程中方向改变时处理重复起始条件 (Repeated Start)
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  XferOptions 传输配置选项，取值参考 @ref I2C_XferOptions_definition
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
  HAL_StatusTypeDef dmaxferstatus;

  /* 检查参数 */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));

  if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 禁用中断，以防止在多次调用的情况下处理期间发生抢占 */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

    /* I2C 不支持全双工通信，因此如果之前使能了中断则将其禁用 */
    /* 然后将 HAL 从机状态从 RX 切换为 TX 状态 */
    if (hi2c->State == HAL_I2C_STATE_BUSY_RX_LISTEN)
    {
      if ((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN)
      {
        /* 中止 DMA 传输（如果存在） */
        if (hi2c->hdmarx != NULL)
        {
          CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);

          /* 设置 I2C DMA 中止回调函数：
           在 DMA 中止流程结束时将调用 HAL_I2C_ErrorCallback() */
          hi2c->hdmarx->XferAbortCallback = I2C_DMAAbort;

          /* 中止 DMA 接收 */
          if (HAL_DMA_Abort_IT(hi2c->hdmarx) != HAL_OK)
          {
            /* 发生错误时直接调用 XferAbortCallback 函数 */
            hi2c->hdmarx->XferAbortCallback(hi2c->hdmarx);
          }
        }
      }
    }
    else if (hi2c->State == HAL_I2C_STATE_BUSY_TX_LISTEN)
    {
      if ((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN)
      {
        CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);

        /* 中止 DMA 传输（如果存在） */
        if (hi2c->hdmatx != NULL)
        {
          /* 设置 I2C DMA 中止回调函数：
           在 DMA 中止流程结束时将调用 HAL_I2C_ErrorCallback() */
          hi2c->hdmatx->XferAbortCallback = I2C_DMAAbort;

          /* 中止 DMA 发送 */
          if (HAL_DMA_Abort_IT(hi2c->hdmatx) != HAL_OK)
          {
            /* 发生错误时直接调用 XferAbortCallback 函数 */
            hi2c->hdmatx->XferAbortCallback(hi2c->hdmatx);
          }
        }
      }
    }
    else
    {
      /* 无需任何操作 */
    }

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_TX_LISTEN;
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = XferOptions;

    if (hi2c->hdmatx != NULL)
    {
      /* 设置 I2C DMA 传输完成回调函数 */
      hi2c->hdmatx->XferCpltCallback = I2C_DMAXferCplt;

      /* 设置 DMA 错误回调函数 */
      hi2c->hdmatx->XferErrorCallback = I2C_DMAError;

      /* 将未使用的 DMA 回调函数设置为 NULL */
      hi2c->hdmatx->XferHalfCpltCallback = NULL;
      hi2c->hdmatx->XferAbortCallback = NULL;

      /* 使能 DMA 通道 */
      dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DR, hi2c->XferSize);
    }
    else
    {
      /* 更新 I2C 状态 */
      hi2c->State     = HAL_I2C_STATE_LISTEN;
      hi2c->Mode      = HAL_I2C_MODE_NONE;

      /* 更新 I2C 错误代码 */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }

    if (dmaxferstatus == HAL_OK)
    {
      /* 使能地址应答 (ACK) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 清除 ADDR 标志位 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      /* 注意：必须在当前进程解锁后使能 I2C 中断，
      以避免在当前进程解锁之前就执行 I2C 中断处理的风险 */
      /* 使能 EVT（事件）和 ERR（错误）中断 */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

      /* 使能 DMA 请求 */
      hi2c->Instance->CR2 |= I2C_CR2_DMAEN;

      return HAL_OK;
    }
    else
    {
      /* 更新 I2C 状态 */
      hi2c->State     = HAL_I2C_STATE_READY;
      hi2c->Mode      = HAL_I2C_MODE_NONE;

      /* 更新 I2C 错误代码 */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  从机模式下以中断非阻塞方式连续接收指定量的数据
  * @note   此接口允许在传输过程中方向改变时处理重复起始条件 (Repeated Start)
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  XferOptions 传输选项，取值参考 @ref I2C_XferOptions_definition
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
  /* 检查参数 */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));

  if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_RX_LISTEN;
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = XferOptions;

    /* 清除 ADDR 标志位 */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    /* 注意：必须在当前进程解锁之后再使能 I2C 中断，
              以避免在当前进程解锁前就执行 I2C 中断处理函数的风险 */

    /* 使能 EVT、BUF 和 ERR 中断 */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  从机模式下使用 DMA 以非阻塞方式连续接收指定数量的数据
  * @note   此接口允许在传输期间方向改变时管理重复起始条件（Repeated Start）
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  pData 指向数据缓冲区的指针
  * @param  Size 要发送的数据量
  * @param  XferOptions 传输选项，取值参考 @ref I2C_XferOptions_definition
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
  HAL_StatusTypeDef dmaxferstatus;

  /* 检查参数 */
  assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));

  if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)
  {
    if ((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* 进程加锁 */
    __HAL_LOCK(hi2c);

    /* 禁用中断，以防止多次调用处理过程中的抢占 */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

    /* I2C 无法处理全双工交换，因此禁用之前使能的中断（如果有） */
    /* 然后将 HAL 从机 RX 状态切换为 TX 状态 */
    if (hi2c->State == HAL_I2C_STATE_BUSY_RX_LISTEN)
    {
      if ((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN)
      {
        /* 中止 DMA 传输（如果存在） */
        if (hi2c->hdmarx != NULL)
        {
          CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);

          /* 设置 I2C DMA 中止回调函数：
           将在 DMA 中止流程结束时调用 HAL_I2C_ErrorCallback() */
          hi2c->hdmarx->XferAbortCallback = I2C_DMAAbort;

          /* 中止 DMA 接收 */
          if (HAL_DMA_Abort_IT(hi2c->hdmarx) != HAL_OK)
          {
            /* 发生错误时直接调用 XferAbortCallback 函数 */
            hi2c->hdmarx->XferAbortCallback(hi2c->hdmarx);
          }
        }
      }
    }
    else if (hi2c->State == HAL_I2C_STATE_BUSY_TX_LISTEN)
    {
      if ((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN)
      {
        CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);

        /* 中止 DMA 传输（如果存在） */
        if (hi2c->hdmatx != NULL)
        {
          /* 设置 I2C DMA 中止回调函数：
           将在 DMA 中止流程结束时调用 HAL_I2C_ErrorCallback() */
          hi2c->hdmatx->XferAbortCallback = I2C_DMAAbort;

          /* 中止 DMA 发送 */
          if (HAL_DMA_Abort_IT(hi2c->hdmatx) != HAL_OK)
          {
            /* 发生错误时直接调用 XferAbortCallback 函数 */
            hi2c->hdmatx->XferAbortCallback(hi2c->hdmatx);
          }
        }
      }
    }
    else
    {
      /* 无需任何操作 */
    }

    /* 检查 I2C 是否已使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 禁用 Pos */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

    hi2c->State     = HAL_I2C_STATE_BUSY_RX_LISTEN;
    hi2c->Mode      = HAL_I2C_MODE_SLAVE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 准备传输参数 */
    hi2c->pBuffPtr    = pData;
    hi2c->XferCount   = Size;
    hi2c->XferSize    = hi2c->XferCount;
    hi2c->XferOptions = XferOptions;

    if (hi2c->hdmarx != NULL)
    {
      /* 设置 I2C DMA 传输完成回调函数 */
      hi2c->hdmarx->XferCpltCallback = I2C_DMAXferCplt;

      /* 设置 DMA 错误回调函数 */
      hi2c->hdmarx->XferErrorCallback = I2C_DMAError;

      /* 将未使用的 DMA 回调函数设置为 NULL */
      hi2c->hdmarx->XferHalfCpltCallback = NULL;
      hi2c->hdmarx->XferAbortCallback = NULL;

      /* 使能 DMA 通道 */
      dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DR, (uint32_t)hi2c->pBuffPtr, hi2c->XferSize);
    }
    else
    {
      /* 更新 I2C 状态 */
      hi2c->State     = HAL_I2C_STATE_LISTEN;
      hi2c->Mode      = HAL_I2C_MODE_NONE;

      /* 更新 I2C 错误代码 */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA_PARAM;

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }

    if (dmaxferstatus == HAL_OK)
    {
      /* 使能地址应答 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 清除 ADDR 标志位 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      /* 使能 DMA 请求 */
      SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);

      /* 注意：必须在解锁当前进程后使能 I2C 中断，
      以避免在当前进程解锁之前执行 I2C 中断处理程序的风险 */
      /* 使能 EVT 和 ERR 中断 */
      __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

      return HAL_OK;
    }
    else
    {
      /* 更新 I2C 状态 */
      hi2c->State     = HAL_I2C_STATE_READY;
      hi2c->Mode      = HAL_I2C_MODE_NONE;

      /* 更新 I2C 错误代码 */
      hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  使能带中断的地址监听模式。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_EnableListen_IT(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->State == HAL_I2C_STATE_READY)
  {
    hi2c->State = HAL_I2C_STATE_LISTEN;

    /* 检查 I2C 是否已经使能 */
    if ((hi2c->Instance->CR1 & I2C_CR1_PE) != I2C_CR1_PE)
    {
      /* 使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);
    }

    /* 使能地址应答 (Acknowledge) */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    /* 使能事件 (EVT) 和错误 (ERR) 中断 */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  禁用带中断的地址监听模式。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_DisableListen_IT(I2C_HandleTypeDef *hi2c)
{
  /* 声明 tmp 变量，以防止使用 volatile 导致的未定义行为 */
  uint32_t tmp;

  /* 仅在当前没有正在进行的传输时，才禁用地址监听模式 */
  if (hi2c->State == HAL_I2C_STATE_LISTEN)
  {
    tmp = (uint32_t)(hi2c->State) & I2C_STATE_MSK;
    hi2c->PreviousState = tmp | (uint32_t)(hi2c->Mode);
    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->Mode = HAL_I2C_MODE_NONE;

    /* 禁用地址应答 (Acknowledge) */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    /* 禁用事件 (EVT) 和错误 (ERR) 中断 */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  通过中断方式中止主机模式或内存模式下的 I2C 中断 (IT) 或 DMA 传输通信流程。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @param  DevAddress 目标设备地址：在调用该接口之前，数据手册中的 7 位设备地址值必须进行左移操作
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_I2C_Master_Abort_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress)
{
  /* 声明临时变量，以防止使用 volatile 导致的未定义行为 */
  HAL_I2C_ModeTypeDef CurrentMode   = hi2c->Mode;

  /* 防止出现未使用的参数编译警告 */
  UNUSED(DevAddress);

  /* 在接收或发送过程中中止主机传输    */
  if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY) != RESET) && ((CurrentMode == HAL_I2C_MODE_MASTER) || 
                                                             (CurrentMode == HAL_I2C_MODE_MEM)))
  {
    /* 进程已锁定 */
    __HAL_LOCK(hi2c);

    hi2c->PreviousState = I2C_STATE_NONE;
    hi2c->State = HAL_I2C_STATE_ABORT;

    /* 禁用应答 (Acknowledge) */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    /* 生成停止条件 (Stop) */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

    hi2c->XferCount = 0U;

    /* 禁用 EVT、BUF 和 ERR 中断 */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    /* 调用相应的回调函数以通知上层传输结束 */
    I2C_ITError(hi2c);

    return HAL_OK;
  }
  else
  {
    /* 中止 (abort) 函数使用错误 */
    /* 该函数仅应在主设备监控中止的情况下使用 */
    /* 或者外设未处于忙状态，意味着没有正在进行的序列需要中止 */
    return HAL_ERROR;
  }
}

/**
  * @}
  */

/** @defgroup I2C_IRQ_Handler_and_Callbacks IRQ 处理程序和回调函数
 * @{
 */

/**
  * @brief  此函数处理 I2C 事件中断请求。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @retval 无
  */
void HAL_I2C_EV_IRQHandler(I2C_HandleTypeDef *hi2c)
{
  uint32_t sr1itflags;
  uint32_t sr2itflags               = 0U;
  uint32_t itsources                = READ_REG(hi2c->Instance->CR2);
  uint32_t CurrentXferOptions       = hi2c->XferOptions;
  HAL_I2C_ModeTypeDef CurrentMode   = hi2c->Mode;
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;

  /* 选择了主模式或内存模式 */
  if ((CurrentMode == HAL_I2C_MODE_MASTER) || (CurrentMode == HAL_I2C_MODE_MEM))
  {
    sr2itflags   = READ_REG(hi2c->Instance->SR2);
    sr1itflags   = READ_REG(hi2c->Instance->SR1);

    /* 如果请求了其他帧，则退出中断事件处理，直到检测到起始位 (Start Bit) */
    if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_SB) == RESET) && (IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(CurrentXferOptions) == 1U))
    {
      return;
    }

    /* SB 置位 ----------------------------------------------------------------*/
    if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_SB) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))
    {
      /* 转换 OTHER_xxx XferOptions 传输选项（如果有） */
      I2C_ConvertOtherXferOptions(hi2c);

      I2C_Master_SB(hi2c);
    }
    /* ADD10 置位 ------------------------------------------------------------*/
    else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_ADD10) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))
    {
      I2C_Master_ADD10(hi2c);
    }
    /* ADDR 置位 -------------------------------------------------------------*/
    else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_ADDR) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))
    {
      I2C_Master_ADDR(hi2c);
    }
    /* I2C 处于发送器模式 ----------------------------------------------------*/
    else if (I2C_CHECK_FLAG(sr2itflags, I2C_FLAG_TRA) != RESET)
    {
      /* 如果 DMA 传输正在进行，则不检查缓冲区和 BTF 标志 */
      if (READ_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN) != I2C_CR2_DMAEN)
      {
        /* TXE 置位且 BTF 复位 --------------------------------------------------*/
        if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_TXE) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_BUF) != RESET) && (I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) == RESET))
        {
          I2C_MasterTransmit_TXE(hi2c);
        }
        /* BTF 置位 ------------------------------------------------------------*/
        else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))
        {
          if (CurrentState == HAL_I2C_STATE_BUSY_TX)
          {
            I2C_MasterTransmit_BTF(hi2c);
          }
          else /* HAL_I2C_MODE_MEM */
          {
            if (CurrentMode == HAL_I2C_MODE_MEM)
            {
              I2C_MemoryTransmit_TXE_BTF(hi2c);
            }
          }
        }
        else
        {
          /* 不执行任何操作 */
        }
      }
    }
    /* I2C 处于接收器模式 ----------------------------------------------------*/
    else
    {
      /* 如果 DMA 传输正在进行，则不检查缓冲区和 BTF 标志 */
      if (READ_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN) != I2C_CR2_DMAEN)
      {
        /* RXNE 置位且 BTF 复位 --------------------------------------------------*/
        if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_RXNE) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_BUF) != RESET) && (I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) == RESET))
        {
          I2C_MasterReceive_RXNE(hi2c);
        }
        /* BTF 置位 ------------------------------------------------------------*/
        else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))
        {
          I2C_MasterReceive_BTF(hi2c);
        }
        else
        {
          /* 不执行任何操作 */
        }
      }
    }
  }
  /* 已选择从机模式 */
  else
  {
    /* 如果检测到错误，仅读取 SR1 寄存器以防止 */
    /* 在错误处理中，通过读取 SR1 后再读取 SR2 来清除 ADDR 标志位 */
    if (hi2c->ErrorCode != HAL_I2C_ERROR_NONE)
    {
      sr1itflags   = READ_REG(hi2c->Instance->SR1);
    }
    else
    {
      sr2itflags   = READ_REG(hi2c->Instance->SR2);
      sr1itflags   = READ_REG(hi2c->Instance->SR1);
    }

    /* ADDR 位置位 ------------------------------------------------------------*/
    if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_ADDR) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))
    {
      /* 此时读取 SR2，将自动清除 ADDR 标志位 */
      if (hi2c->ErrorCode != HAL_I2C_ERROR_NONE)
      {
        sr2itflags   = READ_REG(hi2c->Instance->SR2);
      }
      I2C_Slave_ADDR(hi2c, sr2itflags);
    }
    /* STOPF 位置位 ------------------------------------------------------------*/
    else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_STOPF) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))
    {
      I2C_Slave_STOPF(hi2c);
    }
    /* I2C 处于发送器模式 ------------------------------------------------------*/
    else if ((CurrentState == HAL_I2C_STATE_BUSY_TX) || (CurrentState == HAL_I2C_STATE_BUSY_TX_LISTEN))
    {
      /* TXE 置位且 BTF 复位 ----------------------------------------------------*/
      if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_TXE) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_BUF) != RESET) && (I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) == RESET))
      {
        I2C_SlaveTransmit_TXE(hi2c);
      }
      /* BTF 位置位 -------------------------------------------------------------*/
      else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))
      {
        I2C_SlaveTransmit_BTF(hi2c);
      }
      else
      {
        /* 不做任何操作 */
      }
    }
    /* I2C 处于接收器模式 ------------------------------------------------------*/
    else
    {
      /* RXNE 置位且 BTF 复位 ----------------------------------------------------*/
      if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_RXNE) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_BUF) != RESET) && (I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) == RESET))
      {
        I2C_SlaveReceive_RXNE(hi2c);
      }
      /* BTF 位置位 -------------------------------------------------------------*/
      else if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BTF) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_EVT) != RESET))
      {
        I2C_SlaveReceive_BTF(hi2c);
      }
      else
      {
        /* 不做任何操作 */
      }
    }
  }
}

/**
  * @brief  处理 I2C 错误中断请求。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @retval 无
  */
void HAL_I2C_ER_IRQHandler(I2C_HandleTypeDef *hi2c)
{
  HAL_I2C_ModeTypeDef tmp1;
  uint32_t tmp2;
  HAL_I2C_StateTypeDef tmp3;
  uint32_t tmp4;
  uint32_t sr1itflags = READ_REG(hi2c->Instance->SR1);
  uint32_t itsources  = READ_REG(hi2c->Instance->CR2);
  uint32_t error      = HAL_I2C_ERROR_NONE;
  HAL_I2C_ModeTypeDef CurrentMode   = hi2c->Mode;

  /* 发生 I2C 总线错误中断 ---------------------------------------------------*/
  if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_BERR) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_ERR) != RESET))
  {
    error |= HAL_I2C_ERROR_BERR;

    /* 清除 BERR 标志位 */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_BERR);

    /* 变通方法（Workaround）：在发生错位的 Stop 信号后无法生成 Start 信号 */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_SWRST);
  }

  /* 发生了 I2C 仲裁丢失错误中断 ---------------------------*/
  if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_ARLO) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_ERR) != RESET))
  {
    error |= HAL_I2C_ERROR_ARLO;

    /* 清除 ARLO 标志位 */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_ARLO);
  }

  /* 发生了 I2C 应答失败错误中断 ------------------------*/
  if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_AF) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_ERR) != RESET))
  {
    tmp1 = CurrentMode;
    tmp2 = hi2c->XferCount;
    tmp3 = hi2c->State;
    tmp4 = hi2c->PreviousState;
    if ((tmp1 == HAL_I2C_MODE_SLAVE) && (tmp2 == 0U) && \
        ((tmp3 == HAL_I2C_STATE_BUSY_TX) || (tmp3 == HAL_I2C_STATE_BUSY_TX_LISTEN) || \
         ((tmp3 == HAL_I2C_STATE_LISTEN) && (tmp4 == I2C_STATE_SLAVE_BUSY_TX))))
    {
      I2C_Slave_AF(hi2c);
    }
    else
    {
      /* 清除 AF 标志位 */
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

      error |= HAL_I2C_ERROR_AF;

      /* 若从机在传输过程中接收到非应答（NACK）（即不在传输结束时），则不生成 STOP 信号 */
      if ((CurrentMode == HAL_I2C_MODE_MASTER) || (CurrentMode == HAL_I2C_MODE_MEM))
      {
        /* 生成 Stop 信号 */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
      }
    }
  }

  /* 发生了 I2C 过载/欠载错误中断 -------------------------------*/
  if ((I2C_CHECK_FLAG(sr1itflags, I2C_FLAG_OVR) != RESET) && (I2C_CHECK_IT_SOURCE(itsources, I2C_IT_ERR) != RESET))
  {
    error |= HAL_I2C_ERROR_OVR;
    /* 清除 OVR 标志位 */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_OVR);
  }

  /* 若检测到错误则调用错误回调函数 -----------------------*/
  if (error != HAL_I2C_ERROR_NONE)
  {
    hi2c->ErrorCode |= error;
    I2C_ITError(hi2c);
  }
}

/**
  * @brief  主机发送传输完成回调函数。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hi2c);

  /* 注意：不应修改此函数，当需要该回调函数时，
            可以在用户文件中重新实现 HAL_I2C_MasterTxCpltCallback
   */
}

/**
  * @brief  主机接收传输完成回调函数。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hi2c);

  /* 注意：不应修改此函数，当需要该回调函数时，
            可以在用户文件中实现 HAL_I2C_MasterRxCpltCallback
   */
}

/** @brief  从机发送传输完成回调函数。
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hi2c);

  /* 注意：不应修改此函数，当需要该回调函数时，
            可以在用户文件中实现 HAL_I2C_SlaveTxCpltCallback
   */
}

/**
  * @brief  从机接收传输完成回调函数。
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hi2c);

  /* 注意：不应修改此函数，当需要该回调函数时，
            可以在用户文件中实现 HAL_I2C_SlaveRxCpltCallback
   */
}

/**
  * @brief  从机地址匹配回调函数。
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @param  TransferDirection 主机请求的传输方向（写/读），取值参考 @ref I2C_XferDirection_definition
  * @param  AddrMatchCode 地址匹配代码
  * @retval 无
  */
__weak void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hi2c);
  UNUSED(TransferDirection);
  UNUSED(AddrMatchCode);

  /* 注意：不应修改此函数，当需要该回调函数时，
            可以在用户文件中实现 HAL_I2C_AddrCallback()
   */
}

/**
  * @brief  监听完成回调函数。
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hi2c);

  /* 注意：不应修改此函数，当需要该回调函数时，
            可以在用户文件中实现 HAL_I2C_ListenCpltCallback()
  */
}

/**
  * @brief  内存发送传输完成回调函数。
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hi2c);

  /* 注意：请勿修改此函数，当需要使用回调函数时，
            可以在用户文件中重新实现 HAL_I2C_MemTxCpltCallback
   */
}

/**
  * @brief  内存接收传输完成回调函数。
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hi2c);

  /* 注意：请勿修改此函数，当需要使用回调函数时，
            可以在用户文件中重新实现 HAL_I2C_MemRxCpltCallback
   */
}

/**
  * @brief  I2C 错误回调函数。
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hi2c);

  /* 注意：请勿修改此函数，当需要使用回调函数时，
            可以在用户文件中重新实现 HAL_I2C_ErrorCallback
   */
}

/**
  * @brief  I2C 中止回调函数。
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @retval 无
  */
__weak void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c)
{
  /* 防止未使用的参数产生编译警告 */
  UNUSED(hi2c);

  /* 注意：请勿修改此函数，当需要使用回调函数时，
            可以在用户文件中重新实现 HAL_I2C_AbortCpltCallback
   */
}

/**
  * @}
  */

/** @defgroup I2C_Exported_Functions_Group3 外设状态、模式与错误函数
 *  @brief   外设状态、模式与错误函数
  *
@verbatim
 ===============================================================================
            ##### 外设状态、模式与错误函数 #####
 ===============================================================================
    [..]
    本节允许在运行时获取外设的状态以及数据流状态。

@endverbatim
  * @{
  */

/**
  * @brief  返回 I2C 句柄状态。
  * @param  hi2c 指向包含指定 I2C 配置信息的
  *                I2C_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
HAL_I2C_StateTypeDef HAL_I2C_GetState(I2C_HandleTypeDef *hi2c)
{
  /* 返回 I2C 句柄状态 */
  return hi2c->State;
}

/**
  * @brief  返回 I2C 的主机、从机、内存模式或无模式。
  * @param  hi2c 指向包含 I2C 模块
  *         配置信息的 I2C_HandleTypeDef 结构体的指针
  * @retval HAL 模式
  */
HAL_I2C_ModeTypeDef HAL_I2C_GetMode(I2C_HandleTypeDef *hi2c)
{
  return hi2c->Mode;
}

/**
  * @brief  返回 I2C 错误代码。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @retval I2C 错误代码
  */
uint32_t HAL_I2C_GetError(I2C_HandleTypeDef *hi2c)
{
  return hi2c->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup I2C_Private_Functions
  * @{
  */

/**
  * @brief  处理主机模式下的 TXE 标志
  * @param  hi2c 指向包含 I2C 模块配置信息的 I2C_HandleTypeDef 结构体的指针
  * @retval 无
  */
static void I2C_MasterTransmit_TXE(I2C_HandleTypeDef *hi2c)
{
  /* 声明临时变量，以防止对 volatile 变量操作时产生未定义行为 */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;
  HAL_I2C_ModeTypeDef CurrentMode   = hi2c->Mode;
  uint32_t CurrentXferOptions       = hi2c->XferOptions;

  if ((hi2c->XferSize == 0U) && (CurrentState == HAL_I2C_STATE_BUSY_TX))
  {
    /* 如果未设置停止模式，则直接调用 TxCpltCallback() */
    if ((CurrentXferOptions != I2C_FIRST_AND_LAST_FRAME) && (CurrentXferOptions != I2C_LAST_FRAME) && (CurrentXferOptions != I2C_NO_OPTION_FRAME))
    {
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

      hi2c->PreviousState = I2C_STATE_MASTER_BUSY_TX;
      hi2c->Mode = HAL_I2C_MODE_NONE;
      hi2c->State = HAL_I2C_STATE_READY;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
      hi2c->MasterTxCpltCallback(hi2c);
#else
      HAL_I2C_MasterTxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }
    else /* 生成停止条件，然后调用 TxCpltCallback() */
    {
      /* 禁用 EVT、BUF 和 ERR 中断 */
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

      /* 生成停止条件 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

      hi2c->PreviousState = I2C_STATE_NONE;
      hi2c->State = HAL_I2C_STATE_READY;

      if (hi2c->Mode == HAL_I2C_MODE_MEM)
      {
        hi2c->Mode = HAL_I2C_MODE_NONE;
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        hi2c->MemTxCpltCallback(hi2c);
#else
        HAL_I2C_MemTxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }
      else
      {
        hi2c->Mode = HAL_I2C_MODE_NONE;
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        hi2c->MasterTxCpltCallback(hi2c);
#else
        HAL_I2C_MasterTxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }
    }
  }
  else if ((CurrentState == HAL_I2C_STATE_BUSY_TX) || \
           ((CurrentMode == HAL_I2C_MODE_MEM) && (CurrentState == HAL_I2C_STATE_BUSY_RX)))
  {
    if (hi2c->XferCount == 0U)
    {
      /* 禁用 BUF 中断 */
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);
    }
    else
    {
      if (hi2c->Mode == HAL_I2C_MODE_MEM)
      {
        I2C_MemoryTransmit_TXE_BTF(hi2c);
      }
      else
      {
        /* 向 DR 写入数据 */
        hi2c->Instance->DR = *hi2c->pBuffPtr;

        /* 缓冲区指针递增 */
        hi2c->pBuffPtr++;

        /* 更新计数器 */
        hi2c->XferCount--;
      }
    }
  }
  else
  {
    /* 不做任何操作 */
  }
}

/**
  * @brief  处理主机发送器的 BTF 标志
  * @param  hi2c 指向包含 I2C 模块配置信息的 I2C_HandleTypeDef 结构的指针
  * @retval 无
  */
static void I2C_MasterTransmit_BTF(I2C_HandleTypeDef *hi2c)
{
  /* 声明临时变量，以防止 volatile 使用中出现未定义行为 */
  uint32_t CurrentXferOptions = hi2c->XferOptions;

  if (hi2c->State == HAL_I2C_STATE_BUSY_TX)
  {
    if (hi2c->XferCount != 0U)
    {
      /* 将数据写入 DR */
      hi2c->Instance->DR = *hi2c->pBuffPtr;

      /* 缓冲区指针递增 */
      hi2c->pBuffPtr++;

      /* 更新计数器 */
      hi2c->XferCount--;
    }
    else
    {
      /* 若未设置停止模式，则直接调用 TxCpltCallback() */
      if ((CurrentXferOptions != I2C_FIRST_AND_LAST_FRAME) && (CurrentXferOptions != I2C_LAST_FRAME) && (CurrentXferOptions != I2C_NO_OPTION_FRAME))
      {
        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

        hi2c->PreviousState = I2C_STATE_MASTER_BUSY_TX;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        hi2c->State = HAL_I2C_STATE_READY;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        hi2c->MasterTxCpltCallback(hi2c);
#else
        HAL_I2C_MasterTxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }
      else /* 生成停止条件后调用 TxCpltCallback() */
      {
        /* 禁用 EVT、BUF 和 ERR 中断 */
        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

        /* 生成停止信号 */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

        hi2c->PreviousState = I2C_STATE_NONE;
        hi2c->State = HAL_I2C_STATE_READY;
        if (hi2c->Mode == HAL_I2C_MODE_MEM)
        {
          hi2c->Mode = HAL_I2C_MODE_NONE;
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
          hi2c->MemTxCpltCallback(hi2c);
#else
          HAL_I2C_MemTxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
        }
        else
        {
          hi2c->Mode = HAL_I2C_MODE_NONE;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
          hi2c->MasterTxCpltCallback(hi2c);
#else
          HAL_I2C_MasterTxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
        }
      }
    }
  }
  else
  {
    /* 无操作 */
  }
}

/**
  * @brief  处理存储器发送器的 TXE 和 BTF 标志
  * @param  hi2c 指向包含 I2C 模块配置信息的
  *         I2C_HandleTypeDef 结构体的指针
  * @retval 无
  */
static void I2C_MemoryTransmit_TXE_BTF(I2C_HandleTypeDef *hi2c)
{
  /* 声明临时变量，以防止 volatile 使用引发的未定义行为 */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;

  if (hi2c->EventCount == 0U)
  {
    /* 如果存储器地址大小为 8 位 */
    if (hi2c->MemaddSize == I2C_MEMADD_SIZE_8BIT)
    {
      /* 发送存储器地址 */
      hi2c->Instance->DR = I2C_MEM_ADD_LSB(hi2c->Memaddress);

      hi2c->EventCount += 2U;
    }
    /* 如果存储器地址大小为 16 位 */
    else
    {
      /* 发送存储器地址的高字节 (MSB) */
      hi2c->Instance->DR = I2C_MEM_ADD_MSB(hi2c->Memaddress);

      hi2c->EventCount++;
    }
  }
  else if (hi2c->EventCount == 1U)
  {
    /* 发送存储器地址的低字节 (LSB) */
    hi2c->Instance->DR = I2C_MEM_ADD_LSB(hi2c->Memaddress);

    hi2c->EventCount++;
  }
  else if (hi2c->EventCount == 2U)
  {
    if (CurrentState == HAL_I2C_STATE_BUSY_RX)
    {
      /* 生成重启 (Restart) 条件 */
      hi2c->Instance->CR1 |= I2C_CR1_START;

      hi2c->EventCount++;
    }
    else if ((hi2c->XferCount > 0U) && (CurrentState == HAL_I2C_STATE_BUSY_TX))
    {
      /* 向 DR 写入数据 */
      hi2c->Instance->DR = *hi2c->pBuffPtr;

      /* 递增缓冲区指针 */
      hi2c->pBuffPtr++;

      /* 更新计数器 */
      hi2c->XferCount--;
    }
    else if ((hi2c->XferCount == 0U) && (CurrentState == HAL_I2C_STATE_BUSY_TX))
    {
      /* 生成停止条件然后调用 TxCpltCallback() */
      /* 禁用 EVT、BUF 和 ERR 中断 */
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

      /* 生成停止条件 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

      hi2c->PreviousState = I2C_STATE_NONE;
      hi2c->State = HAL_I2C_STATE_READY;
      hi2c->Mode = HAL_I2C_MODE_NONE;
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
      hi2c->MemTxCpltCallback(hi2c);
#else
      HAL_I2C_MemTxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }
    else
    {
      /* 不执行任何操作 */
    }
  }
  else
  {
    /* 清除 TXE 和 BTF 标志位 */
    I2C_Flush_DR(hi2c);
  }
}

/**
  * @brief  处理主机模式下的 RXNE 标志位
  * @param  hi2c 指向包含 I2C 模块配置信息的
  *         I2C_HandleTypeDef 结构体的指针
  * @retval 无
  */
static void I2C_MasterReceive_RXNE(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->State == HAL_I2C_STATE_BUSY_RX)
  {
    uint32_t tmp;
    uint32_t CurrentXferOptions;

    CurrentXferOptions = hi2c->XferOptions;
    tmp = hi2c->XferCount;
    if (tmp > 3U)
    {
      /* 从 DR 读取数据 */
      *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

      /* 递增缓冲区指针 */
      hi2c->pBuffPtr++;

      /* 更新计数器 */
      hi2c->XferCount--;

      if (hi2c->XferCount == (uint16_t)3)
      {
        /* 禁用 BUF 中断，有助于在 BTF 子程序中
        正确处理最后 4 个字节 */
        /* 禁用 BUF 中断 */
        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);
      }
    }
    else if ((hi2c->XferOptions != I2C_FIRST_AND_NEXT_FRAME) && ((tmp == 1U) || (tmp == 0U)))
    {
      if (I2C_WaitOnSTOPRequestThroughIT(hi2c) == HAL_OK)
      {
        /* 禁用应答 (Acknowledge) */
        CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

        /* 禁用 EVT、BUF 和 ERR 中断 */
        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

        /* 从 DR 读取数据 */
        *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

        /* 递增缓冲区指针 */
        hi2c->pBuffPtr++;

        /* 更新计数器 */
        hi2c->XferCount--;

        hi2c->State = HAL_I2C_STATE_READY;

        if (hi2c->Mode == HAL_I2C_MODE_MEM)
        {
          hi2c->Mode = HAL_I2C_MODE_NONE;
          hi2c->PreviousState = I2C_STATE_NONE;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
          hi2c->MemRxCpltCallback(hi2c);
#else
          HAL_I2C_MemRxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
        }
        else
        {
          hi2c->Mode = HAL_I2C_MODE_NONE;
          if ((CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME))
          {
            hi2c->PreviousState = I2C_STATE_NONE;
          }
          else
          {
            hi2c->PreviousState = I2C_STATE_MASTER_BUSY_RX;
          }

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
          hi2c->MasterRxCpltCallback(hi2c);
#else
          HAL_I2C_MasterRxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
        }
      }
      else
      {
        /* 禁用 EVT、BUF 和 ERR 中断 */
        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

        /* 从 DR 寄存器读取数据 */
        *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

        /* 缓冲区指针递增 */
        hi2c->pBuffPtr++;

        /* 更新计数器 */
        hi2c->XferCount--;

        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;

        /* 调用用户错误回调函数 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        hi2c->ErrorCallback(hi2c);
#else
        HAL_I2C_ErrorCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }
    }
    else
    {
      /* 禁用 BUF 中断，如果在第 N-1 个和第 N 个字节之间存在接收延迟，
         这有助于在 BTF 子程序中正确处理最后 2 个字节 */
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);
    }
  }
}

/**
  * @brief  处理主接收器模式下的 BTF 标志位
  * @param  hi2c 指向包含 I2C 模块配置信息的
  *         I2C_HandleTypeDef 结构体的指针
  * @retval 无
  */
static void I2C_MasterReceive_BTF(I2C_HandleTypeDef *hi2c)
{
  /* 声明临时变量，以防止 volatile 用法引发未定义行为 */
  uint32_t CurrentXferOptions = hi2c->XferOptions;

  if (hi2c->XferCount == 4U)
  {
    /* 禁用 BUF 中断，如果在第 N-1 个和第 N 个字节之间存在接收延迟，
       这有助于在 BTF 子程序中正确处理最后 2 个字节 */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);

    /* 从 DR 寄存器读取数据 */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

    /* 缓冲区指针递增 */
    hi2c->pBuffPtr++;

    /* 更新计数器 */
    hi2c->XferCount--;
  }
  else if (hi2c->XferCount == 3U)
  {
    /* 禁用 BUF 中断，如果在第 N-1 个和第 N 个字节之间存在接收延迟，
       这有助于在 BTF 子程序中正确处理最后 2 个字节 */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);

    if ((CurrentXferOptions != I2C_NEXT_FRAME) && (CurrentXferOptions != I2C_FIRST_AND_NEXT_FRAME))
    {
      /* 禁用应答 (ACK) */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
    }

    /* 从 DR 寄存器读取数据 */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

    /* 缓冲区指针递增 */
    hi2c->pBuffPtr++;

    /* 更新计数器 */
    hi2c->XferCount--;
  }
  else if (hi2c->XferCount == 2U)
  {
    /* 准备下一次传输或停止当前传输 */
    if ((CurrentXferOptions == I2C_FIRST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME_NO_STOP))
    {
      /* 禁用应答 (ACK) */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
    }
    else if ((CurrentXferOptions == I2C_NEXT_FRAME) || (CurrentXferOptions == I2C_FIRST_AND_NEXT_FRAME))
    {
      /* 使能应答 (ACK) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
    }
    else
    {
      /* 生成停止信号 (STOP) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
    }

    /* 从 DR 寄存器读取数据 */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

    /* 缓冲区指针递增 */
    hi2c->pBuffPtr++;

    /* 更新计数器 */
    hi2c->XferCount--;

    /* 从 DR 寄存器读取数据 */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

    /* 缓冲区指针递增 */
    hi2c->pBuffPtr++;

    /* 更新计数器 */
    hi2c->XferCount--;

    /* 禁用 EVT 和 ERR 中断 */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

    hi2c->State = HAL_I2C_STATE_READY;
    if (hi2c->Mode == HAL_I2C_MODE_MEM)
    {
      hi2c->Mode = HAL_I2C_MODE_NONE;
      hi2c->PreviousState = I2C_STATE_NONE;
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
      hi2c->MemRxCpltCallback(hi2c);
#else
      HAL_I2C_MemRxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }
    else
    {
      hi2c->Mode = HAL_I2C_MODE_NONE;
      if ((CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME))
      {
        hi2c->PreviousState = I2C_STATE_NONE;
      }
      else
      {
        hi2c->PreviousState = I2C_STATE_MASTER_BUSY_RX;
      }
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
      hi2c->MasterRxCpltCallback(hi2c);
#else
      HAL_I2C_MasterRxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }
  }
  else
  {
    /* 从 DR 寄存器读取数据 */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

    /* 递增缓冲区指针 */
    hi2c->pBuffPtr++;

    /* 更新计数器 */
    hi2c->XferCount--;
  }
}

/**
  * @brief  处理主机模式下的 SB（起始位）标志
  * @param  hi2c 指向包含 I2C 模块配置信息的
  *         I2C_HandleTypeDef 结构体的指针
  * @retval 无
  */
static void I2C_Master_SB(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->Mode == HAL_I2C_MODE_MEM)
  {
    if (hi2c->EventCount == 0U)
    {
      /* 发送从机地址 */
      hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(hi2c->Devaddress);
    }
    else
    {
      hi2c->Instance->DR = I2C_7BIT_ADD_READ(hi2c->Devaddress);
    }
  }
  else
  {
    if (hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)
    {
      /* 发送从机 7 位地址 */
      if (hi2c->State == HAL_I2C_STATE_BUSY_TX)
      {
        hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(hi2c->Devaddress);
      }
      else
      {
        hi2c->Instance->DR = I2C_7BIT_ADD_READ(hi2c->Devaddress);
      }

      if (((hi2c->hdmatx != NULL) && (hi2c->hdmatx->XferCpltCallback != NULL))
          || ((hi2c->hdmarx != NULL) && (hi2c->hdmarx->XferCpltCallback != NULL)))
      {
        /* 使能 DMA 请求 */
        SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);
      }
    }
    else
    {
      if (hi2c->EventCount == 0U)
      {
        /* 发送从机地址头部 */
        hi2c->Instance->DR = I2C_10BIT_HEADER_WRITE(hi2c->Devaddress);
      }
      else if (hi2c->EventCount == 1U)
      {
        /* 发送从机地址头部 */
        hi2c->Instance->DR = I2C_10BIT_HEADER_READ(hi2c->Devaddress);
      }
      else
      {
        /* 无操作 */
      }
    }
  }
}

/**
  * @brief  处理主机模式下的 ADD10（10 位头地址已发送）标志
  * @param  hi2c 指向包含 I2C 模块配置信息的
  *         I2C_HandleTypeDef 结构体的指针
  * @retval 无
  */
static void I2C_Master_ADD10(I2C_HandleTypeDef *hi2c)
{
  /* 发送从机地址 */
  hi2c->Instance->DR = I2C_10BIT_ADDRESS(hi2c->Devaddress);

  if (((hi2c->hdmatx != NULL) && (hi2c->hdmatx->XferCpltCallback != NULL))
      || ((hi2c->hdmarx != NULL) && (hi2c->hdmarx->XferCpltCallback != NULL)))
  {
    /* 使能 DMA 请求 */
    SET_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);
  }
}

/**
  * @brief  处理主机模式下的 ADDR 标志
  * @param  hi2c 指向包含 I2C 模块配置信息的
  *         I2C_HandleTypeDef 结构体的指针
  * @retval 无
  */
static void I2C_Master_ADDR(I2C_HandleTypeDef *hi2c)
{
  /* 声明临时变量，以防止 volatile 使用引发的未定义行为 */
  HAL_I2C_ModeTypeDef CurrentMode       = hi2c->Mode;
  uint32_t CurrentXferOptions           = hi2c->XferOptions;
  uint32_t Prev_State                   = hi2c->PreviousState;

  if (hi2c->State == HAL_I2C_STATE_BUSY_RX)
  {
    if ((hi2c->EventCount == 0U) && (CurrentMode == HAL_I2C_MODE_MEM))
    {
      /* 清除 ADDR 标志 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
    }
    else if ((hi2c->EventCount == 0U) && (hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_10BIT))
    {
      /* 清除 ADDR 标志 */
      __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

      /* 生成重启动（Restart）条件 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

      hi2c->EventCount++;
    }
    else
    {
      if (hi2c->XferCount == 0U)
      {
        /* 清除 ADDR 标志 */
        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

        /* 生成停止（Stop）条件 */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
      }
      else if (hi2c->XferCount == 1U)
      {
        if (CurrentXferOptions == I2C_NO_OPTION_FRAME)
        {
          /* 禁用应答（Acknowledge） */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

          if ((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN)
          {
            /* 禁用应答（Acknowledge） */
            CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

            /* 清除 ADDR 标志 */
            __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
          }
          else
          {
            /* 清除 ADDR 标志 */
            __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

            /* 生成停止（Stop）条件 */
            SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
          }
        }
        /* 准备下一次传输或停止当前传输 */
        else if ((CurrentXferOptions != I2C_FIRST_AND_LAST_FRAME) && (CurrentXferOptions != I2C_LAST_FRAME) \
                 && ((Prev_State != I2C_STATE_MASTER_BUSY_RX) || (CurrentXferOptions == I2C_FIRST_FRAME)))
        {
          if ((CurrentXferOptions != I2C_NEXT_FRAME) && (CurrentXferOptions != I2C_FIRST_AND_NEXT_FRAME) && (CurrentXferOptions != I2C_LAST_FRAME_NO_STOP))
          {
            /* 禁用应答（Acknowledge） */
            CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
          }
          else
          {
            /* 使能应答（Acknowledge） */
            SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
          }

          /* 清除 ADDR 标志 */
          __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
        }
        else
        {
          /* 禁用应答 (ACK) */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

          /* 清除 ADDR 标志 */
          __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

          /* 生成停止条件 (Stop) */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
        }
      }
      else if (hi2c->XferCount == 2U)
      {
        if ((CurrentXferOptions != I2C_NEXT_FRAME) && (CurrentXferOptions != I2C_FIRST_AND_NEXT_FRAME) && (CurrentXferOptions != I2C_LAST_FRAME_NO_STOP))
        {
          /* 使能 Pos */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_POS);

          /* 清除 ADDR 标志 */
          __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

          /* 禁用应答 (ACK) */
          CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
        }
        else
        {
          /* 使能应答 (ACK) */
          SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

          /* 清除 ADDR 标志 */
          __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
        }

        if (((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN) && ((CurrentXferOptions == I2C_NO_OPTION_FRAME) || (CurrentXferOptions == I2C_FIRST_FRAME) || (CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME_NO_STOP) || (CurrentXferOptions == I2C_LAST_FRAME)))
        {
          /* 使能 Last DMA 位 */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);
        }
      }
      else
      {
        /* 使能应答 (ACK) */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

        if (((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN) && ((CurrentXferOptions == I2C_NO_OPTION_FRAME) || (CurrentXferOptions == I2C_FIRST_FRAME) || (CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME_NO_STOP) || (CurrentXferOptions == I2C_LAST_FRAME)))
        {
          /* 使能 Last DMA 位 */
          SET_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);
        }

        /* 清除 ADDR 标志 */
        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
      }

      /* 复位事件计数器  */
      hi2c->EventCount = 0U;
    }
  }
  else
  {
    /* 清除 ADDR 标志 */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
  }
}

/**
  * @brief  处理从机模式下的 TXE 标志
  * @param  hi2c 指向包含 I2C 模块配置信息的 I2C_HandleTypeDef 结构体的指针
  * @retval 无
  */
static void I2C_SlaveTransmit_TXE(I2C_HandleTypeDef *hi2c)
{
  /* 声明临时变量，以防止 volatile 用法导致的未定义行为 */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;

  if (hi2c->XferCount != 0U)
  {
    /* 将数据写入 DR 寄存器 */
    hi2c->Instance->DR = *hi2c->pBuffPtr;

    /* 缓冲区指针递增 */
    hi2c->pBuffPtr++;

    /* 更新计数器 */
    hi2c->XferCount--;

    if ((hi2c->XferCount == 0U) && (CurrentState == HAL_I2C_STATE_BUSY_TX_LISTEN))
    {
      /* 最后一个字节已接收，禁用中断 */
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);

      /* 将状态设置为 HAL_I2C_STATE_LISTEN */
      hi2c->PreviousState = I2C_STATE_SLAVE_BUSY_TX;
      hi2c->State = HAL_I2C_STATE_LISTEN;

      /* 调用相应的回调函数通知上层传输结束 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
      hi2c->SlaveTxCpltCallback(hi2c);
#else
      HAL_I2C_SlaveTxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }
  }
}

/**
  * @brief  处理从机发送器模式下的 BTF 标志
  * @param  hi2c 指向包含 I2C 模块配置信息的 I2C_HandleTypeDef 结构体的指针
  * @retval 无
  */
static void I2C_SlaveTransmit_BTF(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->XferCount != 0U)
  {
    /* 将数据写入 DR 寄存器 */
    hi2c->Instance->DR = *hi2c->pBuffPtr;

    /* 缓冲区指针递增 */
    hi2c->pBuffPtr++;

    /* 更新计数器 */
    hi2c->XferCount--;
  }
}

/**
  * @brief  处理从机模式下的 RXNE 标志
  * @param  hi2c 指向包含 I2C 模块配置信息的 I2C_HandleTypeDef 结构体的指针
  * @retval 无
  */
static void I2C_SlaveReceive_RXNE(I2C_HandleTypeDef *hi2c)
{
  /* 声明临时变量，以防止 volatile 用法导致的未定义行为 */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;

  if (hi2c->XferCount != 0U)
  {
    /* 从 DR 寄存器读取数据 */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

    /* 缓冲区指针递增 */
    hi2c->pBuffPtr++;

    /* 更新计数器 */
    hi2c->XferCount--;

    if ((hi2c->XferCount == 0U) && (CurrentState == HAL_I2C_STATE_BUSY_RX_LISTEN))
    {
      /* 已接收最后一个字节，禁用中断 */
      __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_BUF);

      /* 将状态设置为 HAL_I2C_STATE_LISTEN */
      hi2c->PreviousState = I2C_STATE_SLAVE_BUSY_RX;
      hi2c->State = HAL_I2C_STATE_LISTEN;

      /* 调用相应的回调函数通知上层传输结束 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
      hi2c->SlaveRxCpltCallback(hi2c);
#else
      HAL_I2C_SlaveRxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }
  }
}

/**
  * @brief  处理从机接收器的 BTF 标志
  * @param  hi2c 指向包含 I2C 模块配置信息的
  *         I2C_HandleTypeDef 结构的指针
  * @retval 无
  */
static void I2C_SlaveReceive_BTF(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->XferCount != 0U)
  {
    /* 从 DR 寄存器读取数据 */
    *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

    /* 缓冲区指针递增 */
    hi2c->pBuffPtr++;

    /* 更新计数器 */
    hi2c->XferCount--;
  }
}

/**
  * @brief  处理从机的 ADD 标志
  * @param  hi2c 指向包含 I2C 模块配置信息的
  *         I2C_HandleTypeDef 结构的指针
  * @param  IT2Flags 要处理的中断2标志。
  * @retval 无
  */
static void I2C_Slave_ADDR(I2C_HandleTypeDef *hi2c, uint32_t IT2Flags)
{
  uint8_t TransferDirection = I2C_DIRECTION_RECEIVE;
  uint16_t SlaveAddrCode;

  if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)
  {
    /* 禁用 BUF 中断，BUF 的使能由从机特定接口管理 */
    __HAL_I2C_DISABLE_IT(hi2c, (I2C_IT_BUF));

    /* 主机请求的传输方向 */
    if (I2C_CHECK_FLAG(IT2Flags, I2C_FLAG_TRA) == RESET)
    {
      TransferDirection = I2C_DIRECTION_TRANSMIT;
    }

    if (I2C_CHECK_FLAG(IT2Flags, I2C_FLAG_DUALF) == RESET)
    {
      SlaveAddrCode = (uint16_t)hi2c->Init.OwnAddress1;
    }
    else
    {
      SlaveAddrCode = (uint16_t)hi2c->Init.OwnAddress2;
    }

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    /* 调用从机地址匹配回调函数 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
    hi2c->AddrCallback(hi2c, TransferDirection, SlaveAddrCode);
#else
    HAL_I2C_AddrCallback(hi2c, TransferDirection, SlaveAddrCode);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }
  else
  {
    /* 清除 ADDR 标志位 */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);
  }
}

/**
  * @brief  处理从机的 STOPF 标志位
  * @param  hi2c 指向包含 I2C 模块配置信息的
  *         I2C_HandleTypeDef 结构体的指针
  * @retval 无
  */
static void I2C_Slave_STOPF(I2C_HandleTypeDef *hi2c)
{
  /* 声明临时变量，以防止 volatile 用法导致的未定义行为 */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;

  /* 禁用 EVT、BUF 和 ERR 中断 */
  __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

  /* 清除 STOPF 标志位 */
  __HAL_I2C_CLEAR_STOPFLAG(hi2c);

  /* 禁用应答 (NACK) */
  CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

  /* 若 DMA 传输正在进行，更新句柄数据大小上下文 */
  if ((hi2c->Instance->CR2 & I2C_CR2_DMAEN) == I2C_CR2_DMAEN)
  {
    if ((CurrentState == HAL_I2C_STATE_BUSY_RX) || (CurrentState == HAL_I2C_STATE_BUSY_RX_LISTEN))
    {
      hi2c->XferCount = (uint16_t)(I2C_GET_DMA_REMAIN_DATA(hi2c->hdmarx));

      if (hi2c->XferCount != 0U)
      {
        /* 设置无应答 (NACK) 对应的错误代码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_AF;
      }

      /* 禁用并停止当前 DMA 传输 */
      CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);

      /* 中止 DMA 传输（若存在） */
      if (HAL_DMA_GetState(hi2c->hdmarx) != HAL_DMA_STATE_READY)
      {
        /* 设置 I2C DMA 中止回调函数：
        将在 DMA 中止流程结束时调用 HAL_I2C_ErrorCallback() */
        hi2c->hdmarx->XferAbortCallback = I2C_DMAAbort;

        /* 中止 DMA 接收 */
        if (HAL_DMA_Abort_IT(hi2c->hdmarx) != HAL_OK)
        {
          /* 发生错误时直接调用 XferAbortCallback 函数 */
          hi2c->hdmarx->XferAbortCallback(hi2c->hdmarx);
        }
      }
    }
    else
    {
      hi2c->XferCount = (uint16_t)(I2C_GET_DMA_REMAIN_DATA(hi2c->hdmatx));

      if (hi2c->XferCount != 0U)
      {
        /* 将 ErrorCode 设置为无应答（NACK）对应的错误码 */
        hi2c->ErrorCode |= HAL_I2C_ERROR_AF;
      }

      /* 禁用并停止当前的 DMA */
      CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);

      /* 中止 DMA 传输（如果存在） */
      if (HAL_DMA_GetState(hi2c->hdmatx) != HAL_DMA_STATE_READY)
      {
        /* 设置 I2C DMA 中止回调函数：
        将在 DMA 中止流程结束时调用 HAL_I2C_ErrorCallback() */
        hi2c->hdmatx->XferAbortCallback = I2C_DMAAbort;

        /* 中止 DMA 发送（TX） */
        if (HAL_DMA_Abort_IT(hi2c->hdmatx) != HAL_OK)
        {
          /* 发生错误时直接调用 XferAbortCallback 函数 */
          hi2c->hdmatx->XferAbortCallback(hi2c->hdmatx);
        }
      }
    }
  }

  /* 数据未全部传输完成，因此设置相应的错误码 */
  if (hi2c->XferCount != 0U)
  {
    /* 存储最后接收到的数据（如果存在） */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET)
    {
      /* 从 DR 寄存器读取数据 */
      *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

      /* 递增缓冲区指针 */
      hi2c->pBuffPtr++;

      /* 更新计数器 */
      hi2c->XferCount--;
    }

    /* 存储最后接收到的数据（如果存在） */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == SET)
    {
      /* 从 DR 寄存器读取数据 */
      *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

      /* 递增缓冲区指针 */
      hi2c->pBuffPtr++;

      /* 更新计数器 */
      hi2c->XferCount--;
    }

    if (hi2c->XferCount != 0U)
    {
      /* 设置无应答 (Non-Acknowledge) 对应的 ErrorCode */
      hi2c->ErrorCode |= HAL_I2C_ERROR_AF;
    }
  }

  if (hi2c->ErrorCode != HAL_I2C_ERROR_NONE)
  {
    /* 调用相应的回调函数以通知上层传输完成 */
    I2C_ITError(hi2c);
  }
  else
  {
    if (CurrentState == HAL_I2C_STATE_BUSY_RX_LISTEN)
    {
      /* 将状态设置为 HAL_I2C_STATE_LISTEN */
      hi2c->PreviousState = I2C_STATE_NONE;
      hi2c->State = HAL_I2C_STATE_LISTEN;

      /* 调用相应的回调函数以通知上层传输完成 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
      hi2c->SlaveRxCpltCallback(hi2c);
#else
      HAL_I2C_SlaveRxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }

    if (hi2c->State == HAL_I2C_STATE_LISTEN)
    {
      hi2c->XferOptions = I2C_NO_OPTION_FRAME;
      hi2c->PreviousState = I2C_STATE_NONE;
      hi2c->State = HAL_I2C_STATE_READY;
      hi2c->Mode = HAL_I2C_MODE_NONE;

      /* 调用监听完成回调函数，以通知上层监听用例结束 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
      hi2c->ListenCpltCallback(hi2c);
#else
      HAL_I2C_ListenCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }
    else
    {
      if ((hi2c->PreviousState  == I2C_STATE_SLAVE_BUSY_RX) || (CurrentState == HAL_I2C_STATE_BUSY_RX))
      {
        hi2c->PreviousState = I2C_STATE_NONE;
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        hi2c->SlaveRxCpltCallback(hi2c);
#else
        HAL_I2C_SlaveRxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }
    }
  }
}

/**
  * @param  hi2c 指向包含 I2C 模块配置信息的
  *         I2C_HandleTypeDef 结构体的指针
  * @retval 无
  */
static void I2C_Slave_AF(I2C_HandleTypeDef *hi2c)
{
  /* 声明临时变量以防止 volatile 使用导致的未定义行为 */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;
  uint32_t CurrentXferOptions       = hi2c->XferOptions;

  if (((CurrentXferOptions ==  I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME)) && \
      (CurrentState == HAL_I2C_STATE_LISTEN))
  {
    hi2c->XferOptions = I2C_NO_OPTION_FRAME;

    /* 禁用 EVT、BUF 和 ERR 中断 */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

    /* 清除 AF 标志位 */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

    /* 禁用应答 (Acknowledge) */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    hi2c->PreviousState = I2C_STATE_NONE;
    hi2c->State         = HAL_I2C_STATE_READY;
    hi2c->Mode          = HAL_I2C_MODE_NONE;

    /* 调用监听完成回调函数，以通知上层监听用例结束 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
    hi2c->ListenCpltCallback(hi2c);
#else
    HAL_I2C_ListenCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }
  else if (CurrentState == HAL_I2C_STATE_BUSY_TX)
  {
    hi2c->XferOptions   = I2C_NO_OPTION_FRAME;
    hi2c->PreviousState = I2C_STATE_SLAVE_BUSY_TX;
    hi2c->State         = HAL_I2C_STATE_READY;
    hi2c->Mode          = HAL_I2C_MODE_NONE;

    /* 禁用 EVT、BUF 和 ERR 中断 */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

    /* 清除 AF 标志 */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

    /* 禁用应答 (ACK) */
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

    /* 清除 TXE 标志 */
    I2C_Flush_DR(hi2c);

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
    hi2c->SlaveTxCpltCallback(hi2c);
#else
    HAL_I2C_SlaveTxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }
  else
  {
    /* 仅清除 AF 标志 */
    /* 处于监听状态，但 XferOptions 为 FIRST 或 NEXT */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);
  }
}

/**
  * @brief  I2C 中断错误处理
  * @param  hi2c I2C 句柄。
  * @retval 无
  */
static void I2C_ITError(I2C_HandleTypeDef *hi2c)
{
  /* 声明临时变量以防止 volatile 使用中的未定义行为 */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;
  HAL_I2C_ModeTypeDef CurrentMode = hi2c->Mode;
  uint32_t CurrentError;

  if (((CurrentMode == HAL_I2C_MODE_MASTER) || (CurrentMode == HAL_I2C_MODE_MEM)) && (CurrentState == HAL_I2C_STATE_BUSY_RX))
  {
    /* 当主机/存储器接收中断处理过程中发生错误时，禁用 I2C CR1 中的 Pos 位 */
    hi2c->Instance->CR1 &= ~I2C_CR1_POS;
  }

  if (((uint32_t)CurrentState & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)
  {
    /* 保持 HAL_I2C_STATE_LISTEN 状态 */
    hi2c->PreviousState = I2C_STATE_NONE;
    hi2c->State = HAL_I2C_STATE_LISTEN;
  }
  else
  {
    /* 如果当前状态正在进行中止处理，则不改变状态 */
    /* 此更改稍后执行 */
    if ((READ_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN) != I2C_CR2_DMAEN) && (CurrentState != HAL_I2C_STATE_ABORT))
    {
      hi2c->State = HAL_I2C_STATE_READY;
      hi2c->Mode = HAL_I2C_MODE_NONE;
    }
    hi2c->PreviousState = I2C_STATE_NONE;
  }

  /* 中止 DMA 传输 */
  if (READ_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN) == I2C_CR2_DMAEN)
  {
    hi2c->Instance->CR2 &= ~I2C_CR2_DMAEN;

    if (hi2c->hdmatx->State != HAL_DMA_STATE_READY)
    {
      /* 设置 DMA 终止回调函数：
      将在 DMA 终止流程结束时调用 HAL_I2C_ErrorCallback() */
      hi2c->hdmatx->XferAbortCallback = I2C_DMAAbort;

      if (HAL_DMA_Abort_IT(hi2c->hdmatx) != HAL_OK)
      {
        /* 禁用 I2C 外设以防止缓冲区中出现伪数据 */
        __HAL_I2C_DISABLE(hi2c);

        hi2c->State = HAL_I2C_STATE_READY;

        /* 发生错误时直接调用 XferAbortCallback 函数 */
        hi2c->hdmatx->XferAbortCallback(hi2c->hdmatx);
      }
    }
    else
    {
      /* 设置 DMA 终止回调函数：
      将在 DMA 终止流程结束时调用 HAL_I2C_ErrorCallback() */
      hi2c->hdmarx->XferAbortCallback = I2C_DMAAbort;

      if (HAL_DMA_Abort_IT(hi2c->hdmarx) != HAL_OK)
      {
        /* 存储最后接收的数据（若存在） */
        if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == SET)
        {
          /* 从 DR 读取数据 */
          *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

          /* 递增缓冲区指针 */
          hi2c->pBuffPtr++;
        }

        /* 禁用 I2C 外设以防止缓冲区中出现伪数据 */
        __HAL_I2C_DISABLE(hi2c);

        hi2c->State = HAL_I2C_STATE_READY;

        /* 发生错误时直接调用 hi2c->hdmarx->XferAbortCallback 函数 */
        hi2c->hdmarx->XferAbortCallback(hi2c->hdmarx);
      }
    }
  }
  else if (hi2c->State == HAL_I2C_STATE_ABORT)
  {
    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* 存储最后接收的数据（若存在） */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == SET)
    {
      /* 从 DR 读取数据 */
      *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

      /* 递增缓冲区指针 */
      hi2c->pBuffPtr++;
    }

    /* 禁用 I2C 外设以防止缓冲区中出现伪数据 */
    __HAL_I2C_DISABLE(hi2c);

    /* 调用对应的回调函数以通知上层传输完成 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
    hi2c->AbortCpltCallback(hi2c);
#else
    HAL_I2C_AbortCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }
  else
  {
    /* 存储最后接收的数据（如果有） */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == SET)
    {
      /* 从 DR 读取数据 */
      *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

      /* 递增缓冲区指针 */
      hi2c->pBuffPtr++;
    }

    /* 调用用户错误回调函数 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
    hi2c->ErrorCallback(hi2c);
#else
    HAL_I2C_ErrorCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }

  /* 在接收到 NACK、总线错误 (BusError)、仲裁丢失 (ArbitrationLost) 或过载错误 (OverRun) 后，不会置位 STOP 标志 */
  CurrentError = hi2c->ErrorCode;

  if (((CurrentError & HAL_I2C_ERROR_BERR) == HAL_I2C_ERROR_BERR) || \
      ((CurrentError & HAL_I2C_ERROR_ARLO) == HAL_I2C_ERROR_ARLO) || \
      ((CurrentError & HAL_I2C_ERROR_AF) == HAL_I2C_ERROR_AF)     || \
      ((CurrentError & HAL_I2C_ERROR_OVR) == HAL_I2C_ERROR_OVR))
  {
    /* 禁用 EVT、BUF 和 ERR 中断 */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);
  }

  /* 因此可通知上层监听阶段已停止 */
  /* 在处理 NACK 错误期间 */
  CurrentState = hi2c->State;
  if (((hi2c->ErrorCode & HAL_I2C_ERROR_AF) == HAL_I2C_ERROR_AF) && (CurrentState == HAL_I2C_STATE_LISTEN))
  {
    hi2c->XferOptions   = I2C_NO_OPTION_FRAME;
    hi2c->PreviousState = I2C_STATE_NONE;
    hi2c->State         = HAL_I2C_STATE_READY;
    hi2c->Mode          = HAL_I2C_MODE_NONE;

    /* 调用监听完成回调函数，以通知上层监听用例结束 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
    hi2c->ListenCpltCallback(hi2c);
#else
    HAL_I2C_ListenCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }
}

/**
  * @param  hi2c 指向包含 I2C 模块配置信息的
  *         I2C_HandleTypeDef 结构体的指针
  * @param  DevAddress 目标设备地址：在调用该接口前，
  *         数据手册中的设备 7 位地址值必须先左移一位
  * @param  Timeout 超时时间
  * @param  Tickstart Tick 起始值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef I2C_MasterRequestWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Timeout, uint32_t Tickstart)
{
  /* 声明临时变量以防止 volatile 使用时发生未定义行为 */
  uint32_t CurrentXferOptions = hi2c->XferOptions;

  /* 如果是首次传输，则生成起始条件 (Start) */
  if ((CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_FIRST_FRAME) || (CurrentXferOptions == I2C_NO_OPTION_FRAME))
  {
    /* 生成起始条件 (Start) */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
  }
  else if (hi2c->PreviousState == I2C_STATE_MASTER_BUSY_RX)
  {
    /* 生成重复起始条件 (ReStart) */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
  }
  else
  {
    /* 不执行任何操作 */
  }

  /* 等待 SB 标志位置位 */
  if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)
  {
    if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)
    {
      hi2c->ErrorCode = HAL_I2C_WRONG_START;
    }
    return HAL_TIMEOUT;
  }

  if (hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)
  {
    /* 发送从机地址 */
    hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(DevAddress);
  }
  else
  {
    /* 发送从机地址头部 */
    hi2c->Instance->DR = I2C_10BIT_HEADER_WRITE(DevAddress);

    /* 等待 ADD10 标志位置位 */
    if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADD10, Timeout, Tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* 发送从机地址 */
    hi2c->Instance->DR = I2C_10BIT_ADDRESS(DevAddress);
  }

  /* 等待 ADDR 标志位置位 */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  主机发送目标设备地址以发起读请求。
  * @param  hi2c 指向包含 I2C 模块配置信息的 I2C_HandleTypeDef 结构的指针
  * @param  DevAddress 目标设备地址：在调用该接口前，数据手册中的设备 7 位地址值
  *         必须先向左移位
  * @param  Timeout 超时时间
  * @param  Tickstart 滴答起始值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef I2C_MasterRequestRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Timeout, uint32_t Tickstart)
{
  /* 声明临时变量以防止 volatile 用法导致的未定义行为 */
  uint32_t CurrentXferOptions = hi2c->XferOptions;

  /* 使能应答 (Acknowledge) */
  SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

  /* 如果是首次传输，则生成起始条件 */
  if ((CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_FIRST_FRAME)  || (CurrentXferOptions == I2C_NO_OPTION_FRAME))
  {
    /* 生成起始条件 (Start) */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
  }
  else if (hi2c->PreviousState == I2C_STATE_MASTER_BUSY_TX)
  {
    /* 生成重复起始条件 (ReStart) */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);
  }
  else
  {
    /* 不执行任何操作 */
  }

  /* 等待 SB 标志位置位 */
  if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)
  {
    if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)
    {
      hi2c->ErrorCode = HAL_I2C_WRONG_START;
    }
    return HAL_TIMEOUT;
  }

  if (hi2c->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)
  {
    /* 发送从机地址 */
    hi2c->Instance->DR = I2C_7BIT_ADD_READ(DevAddress);
  }
  else
  {
    /* 发送从机地址头部 */
    hi2c->Instance->DR = I2C_10BIT_HEADER_WRITE(DevAddress);

    /* 等待 ADD10 标志位置位 */
    if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADD10, Timeout, Tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* 发送从机地址 */
    hi2c->Instance->DR = I2C_10BIT_ADDRESS(DevAddress);

    /* 等待 ADDR 标志位置位 */
    if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* 清除 ADDR 标志位 */
    __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

    /* 生成重复起始条件 (Restart) */
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

    /* 等待 SB 标志位置位 */
    if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)
    {
      if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)
      {
        hi2c->ErrorCode = HAL_I2C_WRONG_START;
      }
      return HAL_TIMEOUT;
    }

    /* 发送从机地址头部 */
    hi2c->Instance->DR = I2C_10BIT_HEADER_READ(DevAddress);
  }

  /* 等待 ADDR 标志位置位 */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  主机发送目标设备地址及随后的内部内存地址以发起写请求。
  * @param  hi2c 指向包含 I2C 模块配置信息的 I2C_HandleTypeDef 结构体的指针
  * @param  DevAddress 目标设备地址：调用该接口前，必须将数据手册中的设备 7 位地址值向左移位
  * @param  MemAddress 内部内存地址
  * @param  MemAddSize 内部内存地址大小
  * @param  Timeout 超时时间
  * @param  Tickstart 滴答定时器起始值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef I2C_RequestMemoryWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart)
{
  /* 生成起始条件 (Start) */
  SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

  /* 等待 SB 标志位置位 */
  if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)
  {
    if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)
    {
      hi2c->ErrorCode = HAL_I2C_WRONG_START;
    }
    return HAL_TIMEOUT;
  }

  /* 发送从机地址 */
  hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(DevAddress);

  /* 等待 ADDR 标志位置位 */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* 清除 ADDR 标志位 */
  __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

  /* 等待 TXE 标志位置位 */
  if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK)
  {
    if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)
    {
      /* 生成停止条件 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
    }
    return HAL_ERROR;
  }

  /* 如果内存地址大小为 8 位 */
  if (MemAddSize == I2C_MEMADD_SIZE_8BIT)
  {
    /* 发送内存地址 */
    hi2c->Instance->DR = I2C_MEM_ADD_LSB(MemAddress);
  }
  /* 如果内存地址大小为 16 位 */
  else
  {
    /* 发送内存地址的高字节 (MSB) */
    hi2c->Instance->DR = I2C_MEM_ADD_MSB(MemAddress);

    /* 等待 TXE 标志位置位 */
    if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK)
    {
      if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)
      {
        /* 生成停止条件 */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
      }
      return HAL_ERROR;
    }

    /* 发送内存地址的低字节 (LSB) */
    hi2c->Instance->DR = I2C_MEM_ADD_LSB(MemAddress);
  }

  return HAL_OK;
}

/**
  * @brief  主机发送目标器件地址及内部内存地址以发起读请求。
  * @param  hi2c 指向包含 I2C 模块配置信息的
  *         I2C_HandleTypeDef 结构体的指针
  * @param  DevAddress 目标器件地址：数据手册中的器件 7 位地址值
  *         在调用该接口前必须进行左移
  * @param  MemAddress 内部内存地址
  * @param  MemAddSize 内部内存地址大小
  * @param  Timeout 超时时间
  * @param  Tickstart 滴答起始值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef I2C_RequestMemoryRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart)
{
  /* 使能应答 */
  SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

  /* 生成起始条件 */
  SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

  /* 等待 SB 标志位置位 */
  if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)
  {
    if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)
    {
      hi2c->ErrorCode = HAL_I2C_WRONG_START;
    }
    return HAL_TIMEOUT;
  }

  /* 发送从机地址 */
  hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(DevAddress);

  /* 等待 ADDR 标志位置位 */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* 清除 ADDR 标志位 */
  __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

  /* 等待 TXE 标志位置位 */
  if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK)
  {
    if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)
    {
      /* 生成停止条件 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
    }
    return HAL_ERROR;
  }

  /* 如果内存地址大小为 8 位 */
  if (MemAddSize == I2C_MEMADD_SIZE_8BIT)
  {
    /* 发送内存地址 */
    hi2c->Instance->DR = I2C_MEM_ADD_LSB(MemAddress);
  }
  /* 如果内存地址大小为 16 位 */
  else
  {
    /* 发送内存地址的高字节 (MSB) */
    hi2c->Instance->DR = I2C_MEM_ADD_MSB(MemAddress);

    /* 等待 TXE 标志位置位 */
    if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK)
    {
      if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)
      {
        /* 生成停止条件 */
        SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
      }
      return HAL_ERROR;
    }

    /* 发送内存地址的低字节 (LSB) */
    hi2c->Instance->DR = I2C_MEM_ADD_LSB(MemAddress);
  }

  /* 等待 TXE 标志位置位 */
  if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout, Tickstart) != HAL_OK)
  {
    if (hi2c->ErrorCode == HAL_I2C_ERROR_AF)
    {
      /* 生成停止条件 */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
    }
    return HAL_ERROR;
  }

  /* 生成重复起始条件 */
  SET_BIT(hi2c->Instance->CR1, I2C_CR1_START);

  /* 等待 SB 标志位置位 */
  if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart) != HAL_OK)
  {
    if (READ_BIT(hi2c->Instance->CR1, I2C_CR1_START) == I2C_CR1_START)
    {
      hi2c->ErrorCode = HAL_I2C_WRONG_START;
    }
    return HAL_TIMEOUT;
  }

  /* 发送从机地址 */
  hi2c->Instance->DR = I2C_7BIT_ADD_READ(DevAddress);

  /* 等待 ADDR 标志位置位 */
  if (I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout, Tickstart) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  DMA I2C 传输完成回调函数。
  * @param  hdma DMA 句柄
  * @retval 无
  */
static void I2C_DMAXferCplt(DMA_HandleTypeDef *hdma)
{
  I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* 偏离 MISRAC2012-Rule-11.5 规则 */

  /* 声明临时变量以防止 volatile 用法的未定义行为 */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;
  HAL_I2C_ModeTypeDef CurrentMode   = hi2c->Mode;
  uint32_t CurrentXferOptions       = hi2c->XferOptions;

  /* 禁用 EVT 和 ERR 中断 */
  __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

  /* 清除完成回调函数 */
  if (hi2c->hdmatx != NULL)
  {
    hi2c->hdmatx->XferCpltCallback = NULL;
  }
  if (hi2c->hdmarx != NULL)
  {
    hi2c->hdmarx->XferCpltCallback = NULL;
  }

  if ((((uint32_t)CurrentState & (uint32_t)HAL_I2C_STATE_BUSY_TX) == (uint32_t)HAL_I2C_STATE_BUSY_TX) || ((((uint32_t)CurrentState & (uint32_t)HAL_I2C_STATE_BUSY_RX) == (uint32_t)HAL_I2C_STATE_BUSY_RX) && (CurrentMode == HAL_I2C_MODE_SLAVE)))
  {
    /* 禁用 DMA 请求 */
    CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);

    hi2c->XferCount = 0U;

    if (CurrentState == HAL_I2C_STATE_BUSY_TX_LISTEN)
    {
      /* 将状态设置为 HAL_I2C_STATE_LISTEN */
      hi2c->PreviousState = I2C_STATE_SLAVE_BUSY_TX;
      hi2c->State = HAL_I2C_STATE_LISTEN;

      /* 调用相应的回调函数以通知上层传输结束 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
      hi2c->SlaveTxCpltCallback(hi2c);
#else
      HAL_I2C_SlaveTxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }
    else if (CurrentState == HAL_I2C_STATE_BUSY_RX_LISTEN)
    {
      /* 将状态设置为 HAL_I2C_STATE_LISTEN */
      hi2c->PreviousState = I2C_STATE_SLAVE_BUSY_RX;
      hi2c->State = HAL_I2C_STATE_LISTEN;

      /* 调用相应的回调函数以通知上层传输结束 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
      hi2c->SlaveRxCpltCallback(hi2c);
#else
      HAL_I2C_SlaveRxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }
    else
    {
      /* 无需任何操作 */
    }

    /* 使能 EVT 和 ERR 中断，以便在 IRQ 处理函数中处理传输完成 */
    __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);
  }
  /* 检查当前模式，以防 DMA 处理程序已被更高优先级的中断抢占 */
  else if (hi2c->Mode != HAL_I2C_MODE_NONE)
  {
    if (hi2c->XferCount == (uint16_t)1)
    {
      /* 禁用应答 (NACK) */
      CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);
    }

    /* 禁用 EVT 和 ERR 中断 */
    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_ERR);

    /* 准备下一次传输或停止当前传输 */
    if ((CurrentXferOptions == I2C_NO_OPTION_FRAME) || (CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_OTHER_AND_LAST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME))
    {
      /* 生成停止信号 (STOP) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);
    }

    /* 禁用最后一次 DMA 传输 (Last DMA) */
    CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_LAST);

    /* 禁用 DMA 请求 */
    CLEAR_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN);

    hi2c->XferCount = 0U;

    /* 检查传输过程中是否检测到错误 */
    if (hi2c->ErrorCode != HAL_I2C_ERROR_NONE)
    {
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
      hi2c->ErrorCallback(hi2c);
#else
      HAL_I2C_ErrorCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }
    else
    {
      hi2c->State = HAL_I2C_STATE_READY;

      if (hi2c->Mode == HAL_I2C_MODE_MEM)
      {
        hi2c->Mode = HAL_I2C_MODE_NONE;
        hi2c->PreviousState = I2C_STATE_NONE;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        hi2c->MemRxCpltCallback(hi2c);
#else
        HAL_I2C_MemRxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }
      else
      {
        hi2c->Mode = HAL_I2C_MODE_NONE;
        if ((CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) || (CurrentXferOptions == I2C_LAST_FRAME))
        {
          hi2c->PreviousState = I2C_STATE_NONE;
        }
        else
        {
          hi2c->PreviousState = I2C_STATE_MASTER_BUSY_RX;
        }

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        hi2c->MasterRxCpltCallback(hi2c);
#else
        HAL_I2C_MasterRxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
      }
    }
  }
  else
  {
    /* 无需任何操作 */
  }
}

/**
  * @brief  DMA I2C 通信错误回调函数。
  * @param  hdma DMA 句柄
  * @retval 无
  */
static void I2C_DMAError(DMA_HandleTypeDef *hdma)
{
  I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* 偏离 MISRAC2012-Rule-11.5 规则 */

  /* 清除完成回调函数 */
  if (hi2c->hdmatx != NULL)
  {
    hi2c->hdmatx->XferCpltCallback = NULL;
  }
  if (hi2c->hdmarx != NULL)
  {
    hi2c->hdmarx->XferCpltCallback = NULL;
  }

  /* 禁用应答 (ACK) */
  CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

  hi2c->XferCount       = 0U;
  hi2c->State           = HAL_I2C_STATE_READY;
  hi2c->Mode            = HAL_I2C_MODE_NONE;
  hi2c->ErrorCode       |= HAL_I2C_ERROR_DMA;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
  hi2c->ErrorCallback(hi2c);
#else
  HAL_I2C_ErrorCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
}

/**
  * @brief DMA I2C 通信中止回调函数
  *        （在 DMA 中止流程结束时调用）。
  * @param hdma DMA 句柄。
  * @retval 无
  */
static void I2C_DMAAbort(DMA_HandleTypeDef *hdma)
{
  __IO uint32_t count = 0U;
  I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* 偏离 MISRAC2012-Rule-11.5 规则 */

  /* 声明临时变量以防止 volatile 使用导致的未定义行为 */
  HAL_I2C_StateTypeDef CurrentState = hi2c->State;

  /* 在中止处理期间，检查是否存在挂起的 STOP 请求 */
  /* 等待直到 STOP 标志位复位 */
  count = I2C_TIMEOUT_FLAG * (SystemCoreClock / 25U / 1000U);
  do
  {
    if (count == 0U)
    {
      hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
      break;
    }
    count--;
  }
  while (READ_BIT(hi2c->Instance->CR1, I2C_CR1_STOP) == I2C_CR1_STOP);

  /* 清除完成回调函数 */
  if (hi2c->hdmatx != NULL)
  {
    hi2c->hdmatx->XferCpltCallback = NULL;
  }
  if (hi2c->hdmarx != NULL)
  {
    hi2c->hdmarx->XferCpltCallback = NULL;
  }

  /* 禁用应答 (ACK) */
  CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

  hi2c->XferCount = 0U;

  /* 复位 XferAbortCallback */
  if (hi2c->hdmatx != NULL)
  {
    hi2c->hdmatx->XferAbortCallback = NULL;
  }
  if (hi2c->hdmarx != NULL)
  {
    hi2c->hdmarx->XferAbortCallback = NULL;
  }

  /* 禁用 I2C 外设以防止缓冲区中出现伪数据 */
  __HAL_I2C_DISABLE(hi2c);

  /* 检查是否来自用户发起的中止操作 */
  if (hi2c->State == HAL_I2C_STATE_ABORT)
  {
    hi2c->State         = HAL_I2C_STATE_READY;
    hi2c->Mode          = HAL_I2C_MODE_NONE;
    hi2c->ErrorCode     = HAL_I2C_ERROR_NONE;

    /* 调用相应的回调函数以通知上层传输结束 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
    hi2c->AbortCpltCallback(hi2c);
#else
    HAL_I2C_AbortCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }
  else
  {
    if (((uint32_t)CurrentState & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)
    {
      /* 重新使能 I2C 外设 */
      __HAL_I2C_ENABLE(hi2c);

      /* 使能应答 (ACK) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_ACK);

      /* 保持 HAL_I2C_STATE_LISTEN 状态 */
      hi2c->PreviousState = I2C_STATE_NONE;
      hi2c->State = HAL_I2C_STATE_LISTEN;
    }
    else
    {
      hi2c->State = HAL_I2C_STATE_READY;
      hi2c->Mode = HAL_I2C_MODE_NONE;
    }

    /* 调用相应回调函数以通知上层传输结束 */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
    hi2c->ErrorCallback(hi2c);
#else
    HAL_I2C_ErrorCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
  }
}

/**
  * @brief  处理 I2C 通信超时。
  * @param  hi2c 指向包含 I2C 模块配置信息的 I2C_HandleTypeDef 结构体的指针
  * @param  Flag 指定要检查的 I2C 标志位。
  * @param  Status 标志位的新状态 (SET 或 RESET)。
  * @param  Timeout 超时持续时间
  * @param  Tickstart 滴答定时器起始值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t Tickstart)
{
  /* 等待直到标志位置位 */
  while (__HAL_I2C_GET_FLAG(hi2c, Flag) == Status)
  {
    /* 检查是否超时 */
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
      {
        if ((__HAL_I2C_GET_FLAG(hi2c, Flag) == Status))
        {
          hi2c->PreviousState     = I2C_STATE_NONE;
          hi2c->State             = HAL_I2C_STATE_READY;
          hi2c->Mode              = HAL_I2C_MODE_NONE;
          hi2c->ErrorCode         |= HAL_I2C_ERROR_TIMEOUT;

          /* 解锁进程 */
          __HAL_UNLOCK(hi2c);

          return HAL_ERROR;
        }
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  处理主机寻址阶段的 I2C 通信超时。
  * @param  hi2c 指向包含 I2C 模块配置信息的 I2C_HandleTypeDef 结构体的指针
  * @param  Flag 指定要检查的 I2C 标志位。
  * @param  Timeout 超时持续时间
  * @param  Tickstart 滴答定时器起始值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef I2C_WaitOnMasterAddressFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, uint32_t Timeout, uint32_t Tickstart)
{
  while (__HAL_I2C_GET_FLAG(hi2c, Flag) == RESET)
  {
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) == SET)
    {
      /* 生成停止条件 (STOP) */
      SET_BIT(hi2c->Instance->CR1, I2C_CR1_STOP);

      /* 清除 AF (应答失败) 标志位 */
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

      hi2c->PreviousState       = I2C_STATE_NONE;
      hi2c->State               = HAL_I2C_STATE_READY;
      hi2c->Mode                = HAL_I2C_MODE_NONE;
      hi2c->ErrorCode           |= HAL_I2C_ERROR_AF;

      /* 解锁进程 */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }

    /* 检查是否超时 */
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
      {
        if ((__HAL_I2C_GET_FLAG(hi2c, Flag) == RESET))
        {
          hi2c->PreviousState       = I2C_STATE_NONE;
          hi2c->State               = HAL_I2C_STATE_READY;
          hi2c->Mode                = HAL_I2C_MODE_NONE;
          hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

          /* 进程解锁 */
          __HAL_UNLOCK(hi2c);

          return HAL_ERROR;
        }
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  此函数处理针对 TXE 标志特定用例的 I2C 通信超时。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @param  Timeout 超时持续时间
  * @param  Tickstart 滴答定时器起始值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef I2C_WaitOnTXEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)
{
  while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXE) == RESET)
  {
    /* 检查是否检测到 NACK */
    if (I2C_IsAcknowledgeFailed(hi2c) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* 检查是否超时 */
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
      {
        if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXE) == RESET))
        {
          hi2c->PreviousState       = I2C_STATE_NONE;
          hi2c->State               = HAL_I2C_STATE_READY;
          hi2c->Mode                = HAL_I2C_MODE_NONE;
          hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

          /* 进程解锁 */
          __HAL_UNLOCK(hi2c);

          return HAL_ERROR;
        }
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  此函数处理针对 BTF 标志特定用例的 I2C 通信超时。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @param  Timeout 超时持续时间
  * @param  Tickstart 滴答定时器起始值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef I2C_WaitOnBTFFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)
{
  while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == RESET)
  {
    /* 检查是否检测到 NACK */
    if (I2C_IsAcknowledgeFailed(hi2c) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* 检查是否超时 */
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
      {
        if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == RESET))
        {
          hi2c->PreviousState       = I2C_STATE_NONE;
          hi2c->State               = HAL_I2C_STATE_READY;
          hi2c->Mode                = HAL_I2C_MODE_NONE;
          hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

          /* 进程解锁 */
          __HAL_UNLOCK(hi2c);

          return HAL_ERROR;
        }
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  此函数处理针对 STOP 标志特定用例的 I2C 通信超时。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @param  Timeout 超时持续时间
  * @param  Tickstart 滴答定时器起始值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef I2C_WaitOnSTOPFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)
{
  while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == RESET)
  {
    /* 检查是否检测到 NACK */
    if (I2C_IsAcknowledgeFailed(hi2c) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* 检查是否超时 */
    if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
    {
      if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == RESET))
      {
        hi2c->PreviousState       = I2C_STATE_NONE;
        hi2c->State               = HAL_I2C_STATE_READY;
        hi2c->Mode                = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  此函数处理通过中断发出 STOP 请求特定用例的 I2C 通信超时。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构体的指针。
  * @retval HAL 状态
  */
static HAL_StatusTypeDef I2C_WaitOnSTOPRequestThroughIT(I2C_HandleTypeDef *hi2c)
{
  __IO uint32_t count = 0U;

  /* 等待直到 STOP 标志被复位 */
  count = I2C_TIMEOUT_STOP_FLAG * (SystemCoreClock / 25U / 1000U);
  do
  {
    count--;
    if (count == 0U)
    {
      hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

      return HAL_ERROR;
    }
  }
  while (READ_BIT(hi2c->Instance->CR1, I2C_CR1_STOP) == I2C_CR1_STOP);

  return HAL_OK;
}

/**
  * @brief  此函数处理针对 RXNE 标志特定用途的 I2C 通信超时。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @param  Timeout 超时时间
  * @param  Tickstart 滴答起始值
  * @retval HAL 状态
  */
static HAL_StatusTypeDef I2C_WaitOnRXNEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)
{

  while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == RESET)
  {
    /* 检查是否检测到 STOPF */
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == SET)
    {
      /* 清除 STOP 标志 */
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

      hi2c->PreviousState       = I2C_STATE_NONE;
      hi2c->State               = HAL_I2C_STATE_READY;
      hi2c->Mode                = HAL_I2C_MODE_NONE;
      hi2c->ErrorCode           |= HAL_I2C_ERROR_NONE;

      /* 进程解锁 */
      __HAL_UNLOCK(hi2c);

      return HAL_ERROR;
    }

    /* 检查是否超时 */
    if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
    {
      if ((__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == RESET))
      {
        hi2c->PreviousState       = I2C_STATE_NONE;
        hi2c->State               = HAL_I2C_STATE_READY;
        hi2c->Mode                = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;

        /* 进程解锁 */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  此函数处理 I2C 通信期间应答失败检测。
  * @param  hi2c 指向包含指定 I2C 配置信息的 I2C_HandleTypeDef 结构的指针。
  * @retval HAL 状态
  */
static HAL_StatusTypeDef I2C_IsAcknowledgeFailed(I2C_HandleTypeDef *hi2c)
{
  if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) == SET)
  {
    /* 清除 NACKF 标志 */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

    hi2c->PreviousState       = I2C_STATE_NONE;
    hi2c->State               = HAL_I2C_STATE_READY;
    hi2c->Mode                = HAL_I2C_MODE_NONE;
    hi2c->ErrorCode           |= HAL_I2C_ERROR_AF;

    /* 进程解锁 */
    __HAL_UNLOCK(hi2c);

    return HAL_ERROR;
  }
  return HAL_OK;
}

/**
  * @brief  将 I2Cx OTHER_xxx XferOptions 转换为功能性 XferOptions。
  * @param  hi2c I2C 句柄。
  * @retval 无
  */
static void I2C_ConvertOtherXferOptions(I2C_HandleTypeDef *hi2c)
{
  /* 如果用户将 XferOptions 设置为 I2C_OTHER_FRAME          */
  /* 则隐式请求生成重启条件 (Restart Condition)             */
  /* 将 XferOptions 设置为 I2C_FIRST_FRAME                 */
  if (hi2c->XferOptions == I2C_OTHER_FRAME)
  {
    hi2c->XferOptions = I2C_FIRST_FRAME;
  }
  /* 否则如果用户将 XferOptions 设置为 I2C_OTHER_AND_LAST_FRAME */
  /* 则隐式请求生成重启条件 (Restart Condition)                */
  /* 然后在传输结束时生成停止条件                             */
  /* 将 XferOptions 设置为 I2C_FIRST_AND_LAST_FRAME           */
  else if (hi2c->XferOptions == I2C_OTHER_AND_LAST_FRAME)
  {
    hi2c->XferOptions = I2C_FIRST_AND_LAST_FRAME;
  }
  else
  {
    /* 无需执行任何操作 */
  }
}

/**
  * @}
  */

#endif /* HAL_I2C_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */


/**
  ******************************************************************************
  * @file    stm32f1xx_hal_dma.h
  * @author  MCD Application Team
  * @brief   DMA HAL 模块头文件。
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * 保留所有权利。
  *
  * 本软件根据可在该软件组件根目录下的 LICENSE 文件中找到的条款获得许可。
  * 若本软件未随附 LICENSE 文件，则按“原样”提供。
  *
  ******************************************************************************
  */

/* 用于防止递归包含的宏定义 -------------------------------------------------*/
#ifndef __STM32F1xx_HAL_DMA_H
#define __STM32F1xx_HAL_DMA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup DMA
  * @{
  */

/* 导出的类型 ----------------------------------------------------------------*/

/** @defgroup DMA_Exported_Types DMA 导出类型
  * @{
  */

/**
  * @brief  DMA 配置结构体定义
  */
typedef struct
{
  uint32_t Direction;                 /*!< 指定数据传输方向：从内存到外设、
                                           从内存到内存或从外设到内存。
                                           该参数可以是 @ref DMA_Data_transfer_direction 的值 */

  uint32_t PeriphInc;                 /*!< 指定外设地址寄存器是否递增。
                                           该参数可以是 @ref DMA_Peripheral_incremented_mode 的值 */

  uint32_t MemInc;                    /*!< 指定内存地址寄存器是否递增。
                                           该参数可以是 @ref DMA_Memory_incremented_mode 的值 */

  uint32_t PeriphDataAlignment;       /*!< 指定外设数据宽度。
                                           该参数可以是 @ref DMA_Peripheral_data_size 的值 */

  uint32_t MemDataAlignment;          /*!< 指定内存数据宽度。
                                           该参数可以是 @ref DMA_Memory_data_size 的值 */

  uint32_t Mode;                      /*!< 指定 DMAy Channelx 的操作模式。
                                           该参数可以是 @ref DMA_mode 的值
                                           @note 若在所选通道上配置了内存到内存的数据传输，
                                                 则不能使用循环缓冲区模式 */

  uint32_t Priority;                  /*!< 指定 DMAy Channelx 的软件优先级。
                                           该参数可以是 @ref DMA_Priority_level 的值 */
} DMA_InitTypeDef;

/**
  * @brief  HAL DMA 状态结构体定义
  */
typedef enum
{
  HAL_DMA_STATE_RESET             = 0x00U,  /*!< DMA 尚未初始化或已禁用                  */
  HAL_DMA_STATE_READY             = 0x01U,  /*!< DMA 已初始化且就绪可用                  */
  HAL_DMA_STATE_BUSY              = 0x02U,  /*!< DMA 传输正在进行中                      */
  HAL_DMA_STATE_TIMEOUT           = 0x03U   /*!< DMA 超时状态                           */
}HAL_DMA_StateTypeDef;

/**
  * @brief  HAL DMA 错误码结构体定义
  */
typedef enum
{
  HAL_DMA_FULL_TRANSFER           = 0x00U,    /*!< 传输完成           */
  HAL_DMA_HALF_TRANSFER           = 0x01U     /*!< 半传输完成         */
}HAL_DMA_LevelCompleteTypeDef;

/** 
  * @brief  HAL DMA 回调函数 ID 结构体定义
  */
typedef enum
{
  HAL_DMA_XFER_CPLT_CB_ID          = 0x00U,    /*!< 传输完成           */
  HAL_DMA_XFER_HALFCPLT_CB_ID      = 0x01U,    /*!< 半传输完成         */
  HAL_DMA_XFER_ERROR_CB_ID         = 0x02U,    /*!< 错误               */ 
  HAL_DMA_XFER_ABORT_CB_ID         = 0x03U,    /*!< 中止               */ 
  HAL_DMA_XFER_ALL_CB_ID           = 0x04U     /*!< 全部               */ 
    
}HAL_DMA_CallbackIDTypeDef;

/** 
  * @brief  DMA 句柄结构体定义
  */
typedef struct __DMA_HandleTypeDef
{
  DMA_Channel_TypeDef        *Instance;                                                    /*!< 寄存器基地址                          */
  
  DMA_InitTypeDef            Init;                                                         /*!< DMA 通信参数                           */
  
  HAL_LockTypeDef            Lock;                                                         /*!< DMA 锁对象                             */
  
  __IO HAL_DMA_StateTypeDef  State;                                                        /*!< DMA 传输状态                           */
  
  void                       *Parent;                                                      /*!< 父对象状态                             */
  
  void                       (* XferCpltCallback)( struct __DMA_HandleTypeDef * hdma);     /*!< DMA 传输完成回调函数                   */
  
  void                       (* XferHalfCpltCallback)( struct __DMA_HandleTypeDef * hdma); /*!< DMA 半传输完成回调函数                 */
  
  void                       (* XferErrorCallback)( struct __DMA_HandleTypeDef * hdma);    /*!< DMA 传输错误回调函数                   */

  void                       (* XferAbortCallback)( struct __DMA_HandleTypeDef * hdma);    /*!< DMA 传输中止回调函数                   */
  
  __IO uint32_t              ErrorCode;                                                    /*!< DMA 错误代码                           */

  DMA_TypeDef                *DmaBaseAddress;                                             /*!< DMA 通道基地址                         */
  
  uint32_t                   ChannelIndex;                                                /*!< DMA 通道索引                           */

} DMA_HandleTypeDef;    
/**
  * @}
  */

/* 导出常量 ------------------------------------------------------------------*/

/** @defgroup DMA_Exported_Constants DMA 导出常量
  * @{
  */

/** @defgroup DMA_Error_Code DMA 错误代码
  * @{
  */
#define HAL_DMA_ERROR_NONE                     0x00000000U    /*!< 无错误               */
#define HAL_DMA_ERROR_TE                       0x00000001U    /*!< 传输错误             */
#define HAL_DMA_ERROR_NO_XFER                  0x00000004U    /*!< 无正在进行的传输     */
#define HAL_DMA_ERROR_TIMEOUT                  0x00000020U    /*!< 超时错误             */
#define HAL_DMA_ERROR_NOT_SUPPORTED            0x00000100U    /*!< 不支持的模式                          */ 
/**
  * @}
  */

/** @defgroup DMA_Data_transfer_direction DMA 数据传输方向
  * @{
  */
/** @brief 外设到存储器方向（如串口接收 RX、ADC 采集等，外设数据搬移至内存） */
#define DMA_PERIPH_TO_MEMORY         0x00000000U

/** @brief 存储器到外设方向（如串口发送 TX、DAC 输出等，内存数据搬移至外设发送寄存器） */
#define DMA_MEMORY_TO_PERIPH         ((uint32_t)DMA_CCR_DIR)

/** @brief 存储器到存储器方向（纯内存间快速高速搬移拷贝，仅 DMA1 的部分通道支持） */
#define DMA_MEMORY_TO_MEMORY         ((uint32_t)DMA_CCR_MEM2MEM)
/**
  * @}
  */

/** @defgroup DMA_Peripheral_incremented_mode DMA 外设地址自增模式
  * @{
  */
/** @brief 使能外设地址自增模式（极少使用，仅在访问连续外设 FIFO 或内存到内存时作为源） */
#define DMA_PINC_ENABLE        ((uint32_t)DMA_CCR_PINC)

/** @brief 禁用外设地址自增模式（常规配置，固定访问外设单一数据寄存器，如 USART_DR、ADC_DR） */
#define DMA_PINC_DISABLE       0x00000000U
/**
  * @}
  */

/** @defgroup DMA_Memory_incremented_mode DMA 内存地址递增模式
  * @{
  */
/** @brief 使能内存地址递增模式（常规配置，每次搬移后内存指针自增，填充缓冲区数组） */
#define DMA_MINC_ENABLE         ((uint32_t)DMA_CCR_MINC)

/** @brief 禁用内存地址递增模式（固定访问单一内存变量） */
#define DMA_MINC_DISABLE        0x00000000U
/**
  * @}
  */

/** @defgroup DMA_Peripheral_data_size DMA 外设数据宽度
  * @{
  */
/** @brief 外设数据宽度：字节（8 位，常用于 UART、8 位 SPI/I2C 数据寄存器） */
#define DMA_PDATAALIGN_BYTE          0x00000000U

/** @brief 外设数据宽度：半字（16 位，常用于 ADC 规则采样数据、16 位定时器捕获） */
#define DMA_PDATAALIGN_HALFWORD      ((uint32_t)DMA_CCR_PSIZE_0)

/** @brief 外设数据宽度：字（32 位，常用于 32 位定时器或特定高速总线） */
#define DMA_PDATAALIGN_WORD          ((uint32_t)DMA_CCR_PSIZE_1)
/**
  * @}
  */

/** @defgroup DMA_Memory_data_size DMA 内存数据宽度
  * @{
  */
/** @brief 内存数据宽度：字节（8 位，对应 uint8_t 数组缓冲区） */
#define DMA_MDATAALIGN_BYTE          0x00000000U

/** @brief 内存数据宽度：半字（16 位，对应 uint16_t 数组缓冲区，如 ADC 多通道扫描缓存） */
#define DMA_MDATAALIGN_HALFWORD      ((uint32_t)DMA_CCR_MSIZE_0)

/** @brief 内存数据宽度：字（32 位，对应 uint32_t 数组缓冲区） */
#define DMA_MDATAALIGN_WORD          ((uint32_t)DMA_CCR_MSIZE_1)
/**
  * @}
  */

/** @defgroup DMA_mode DMA 模式
  * @{
  */
/**
  * @brief DMA 正常单次传输模式
  * @note  传输指定字节数后 DMA 硬件自动停止；如需再次传输必须在软件中重新配置并启动。
  */
#define DMA_NORMAL         0x00000000U

/**
  * @brief DMA 循环缓冲传输模式
  * @note  传输计数器减到 0 后自动重新装载初始长度并持续循环传输；常用于 ADC 连续多通道扫描、串口循环接收。
  */
#define DMA_CIRCULAR       ((uint32_t)DMA_CCR_CIRC)
/**
  * @}
  */

/** @defgroup DMA_Priority_level DMA 优先级
  * @{
  */
/** @brief DMA 通道软件仲裁优先级：低（Low） */
#define DMA_PRIORITY_LOW             0x00000000U

/** @brief DMA 通道软件仲裁优先级：中（Medium） */
#define DMA_PRIORITY_MEDIUM          ((uint32_t)DMA_CCR_PL_0)

/** @brief DMA 通道软件仲裁优先级：高（High） */
#define DMA_PRIORITY_HIGH            ((uint32_t)DMA_CCR_PL_1)

/** @brief DMA 通道软件仲裁优先级：非常高（Very High，优先响应处理） */
#define DMA_PRIORITY_VERY_HIGH       ((uint32_t)DMA_CCR_PL)
/**
  * @}
  */


/** @defgroup DMA_interrupt_enable_definitions DMA 中断使能定义
  * @{
  */
/** @brief DMA 传输完成中断使能（Transfer Complete Interrupt Enable） */
#define DMA_IT_TC                         ((uint32_t)DMA_CCR_TCIE)

/** @brief DMA 半传输中断使能（Half Transfer Interrupt Enable，常用于双缓冲乒乓操作） */
#define DMA_IT_HT                         ((uint32_t)DMA_CCR_HTIE)

/** @brief DMA 传输错误中断使能（Transfer Error Interrupt Enable） */
#define DMA_IT_TE                         ((uint32_t)DMA_CCR_TEIE)
/**
  * @}
  */

/** @defgroup DMA_flag_definitions DMA 标志位定义
  * @{
  */
#define DMA_FLAG_GL1                      0x00000001U
#define DMA_FLAG_TC1                      0x00000002U
#define DMA_FLAG_HT1                      0x00000004U
#define DMA_FLAG_TE1                      0x00000008U
#define DMA_FLAG_GL2                      0x00000010U
#define DMA_FLAG_TC2                      0x00000020U
#define DMA_FLAG_HT2                      0x00000040U
#define DMA_FLAG_TE2                      0x00000080U
#define DMA_FLAG_GL3                      0x00000100U
#define DMA_FLAG_TC3                      0x00000200U
#define DMA_FLAG_HT3                      0x00000400U
#define DMA_FLAG_TE3                      0x00000800U
#define DMA_FLAG_GL4                      0x00001000U
#define DMA_FLAG_TC4                      0x00002000U
#define DMA_FLAG_HT4                      0x00004000U
#define DMA_FLAG_TE4                      0x00008000U
#define DMA_FLAG_GL5                      0x00010000U
#define DMA_FLAG_TC5                      0x00020000U
#define DMA_FLAG_HT5                      0x00040000U
#define DMA_FLAG_TE5                      0x00080000U
#define DMA_FLAG_GL6                      0x00100000U
#define DMA_FLAG_TC6                      0x00200000U
#define DMA_FLAG_HT6                      0x00400000U
#define DMA_FLAG_TE6                      0x00800000U
#define DMA_FLAG_GL7                      0x01000000U
#define DMA_FLAG_TC7                      0x02000000U
#define DMA_FLAG_HT7                      0x04000000U
#define DMA_FLAG_TE7                      0x08000000U
/**
  * @}
  */

/**
  * @}
  */


/* 导出宏 -----------------------------------------------------------*/
/** @defgroup DMA_Exported_Macros DMA 导出宏
  * @{
  */

/** @brief  复位 DMA 句柄状态。
  * @param  __HANDLE__: DMA 句柄
  * @retval 无
  */
#define __HAL_DMA_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_DMA_STATE_RESET)

/**
  * @brief  使能指定的 DMA 通道。
  * @param  __HANDLE__: DMA 句柄
  * @retval 无
  */
#define __HAL_DMA_ENABLE(__HANDLE__)        (SET_BIT((__HANDLE__)->Instance->CCR, DMA_CCR_EN))

/**
  * @brief  禁用指定的 DMA 通道。
  * @param  __HANDLE__: DMA 句柄
  * @retval 无
  */
#define __HAL_DMA_DISABLE(__HANDLE__)       (CLEAR_BIT((__HANDLE__)->Instance->CCR, DMA_CCR_EN))


/* 中断和标志位管理 */

/**
  * @brief  使能指定的 DMA 通道中断。
  * @param  __HANDLE__: DMA 句柄
  * @param __INTERRUPT__: 指定要使能或禁用的 DMA 中断源。
  *          该参数可以是以下值的任意组合：
  *            @arg DMA_IT_TC:  传输完成中断屏蔽位
  *            @arg DMA_IT_HT:  传输过半中断屏蔽位
  *            @arg DMA_IT_TE:  传输错误中断屏蔽位
  * @retval 无
  */
#define __HAL_DMA_ENABLE_IT(__HANDLE__, __INTERRUPT__)   (SET_BIT((__HANDLE__)->Instance->CCR, (__INTERRUPT__)))

/**
  * @brief  禁用指定的 DMA 通道中断。
  * @param  __HANDLE__: DMA 句柄
  * @param  __INTERRUPT__: 指定要使能或禁用的 DMA 中断源。
  *          该参数可以是以下值的任意组合：
  *            @arg DMA_IT_TC:  传输完成中断屏蔽位
  *            @arg DMA_IT_HT:  传输过半中断屏蔽位
  *            @arg DMA_IT_TE:  传输错误中断屏蔽位
  * @retval 无
  */
#define __HAL_DMA_DISABLE_IT(__HANDLE__, __INTERRUPT__)  (CLEAR_BIT((__HANDLE__)->Instance->CCR , (__INTERRUPT__)))

/**
  * @brief  检查指定的 DMA 通道中断是否已使能。
  * @param  __HANDLE__: DMA 句柄
  * @param  __INTERRUPT__: 指定要检查的 DMA 中断源。
  *          该参数可以是以下值之一：
  *            @arg DMA_IT_TC:  传输完成中断屏蔽位
  *            @arg DMA_IT_HT:  传输过半中断屏蔽位
  *            @arg DMA_IT_TE:  传输错误中断屏蔽位
  * @retval DMA_IT 的状态（SET 或 RESET）。
  */
#define __HAL_DMA_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)  ((((__HANDLE__)->Instance->CCR & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)

/**
  * @brief  返回当前 DMA 通道传输中剩余的数据单元数量。
  * @param  __HANDLE__: DMA 句柄
  * @retval 当前 DMA 通道传输中剩余的数据单元数量。
  */
#define __HAL_DMA_GET_COUNTER(__HANDLE__) ((__HANDLE__)->Instance->CNDTR)

/**
  * @}
  */

/* 包含 DMA HAL 扩展模块 */
#include "stm32f1xx_hal_dma_ex.h"   

/* 导出函数 --------------------------------------------------------*/
/** @addtogroup DMA_Exported_Functions
  * @{
  */

/** @addtogroup DMA_Exported_Functions_Group1
  * @{
  */
/* 初始化和反初始化函数 ********************************************************/
HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_DeInit (DMA_HandleTypeDef *hdma);
/**
  * @}
  */

/** @addtogroup DMA_Exported_Functions_Group2
  * @{
  */
/* IO 操作函数 *****************************************************************/

/**
  * @brief  启动 DMA 传输（轮询阻塞模式）
  * @param  hdma: 指向包含 DMA 通道配置信息的 DMA_HandleTypeDef 结构体指针
  * @param  SrcAddress: 源内存/外设寄存器地址
  * @param  DstAddress: 目的内存/外设寄存器地址
  * @param  DataLength: 传输的数据项数量（单位为配置的数据宽度：Byte / HalfWord / Word）
  * @warning 【缓冲区生命周期与通道映射避坑】
  *          1. 【内存生命周期】：内存缓冲区【必须具有全局（global）或静态（static）生命周期】！
  *             严禁传入函数栈内的局部变量指针。DMA 是独立于 CPU 的硬件控制器，函数退出后栈被复用，
  *             DMA 将在后台读写已损坏的内存，造成极其诡异的数据破坏和 HardFault。
  *          2. 【通道固定绑定】：STM32F1 的 DMA1/DMA2 各通道与外设请求是严格硬件固定的（如 USART1_TX 只能用 DMA1_Channel4），
  *             不支持跨通道任意调度，配置时务必查阅《STM32F10xxx参考手册》的 DMA 请求映射表。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_DMA_Start (DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);

/**
  * @brief  以中断非阻塞模式启动 DMA 传输
  * @param  hdma: 指向包含 DMA 通道配置信息的 DMA_HandleTypeDef 结构体指针
  * @param  SrcAddress: 源内存/外设寄存器地址
  * @param  DstAddress: 目的内存/外设寄存器地址
  * @param  DataLength: 传输的数据项数量
  * @note   传输完成、半传输完成或发生传输错误时会触发 DMA 中断并调用注册的回调函数。
  * @warning 内存缓冲区必须为全局或静态变量，严禁传入局部栈变量！
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_PollForTransfer(DMA_HandleTypeDef *hdma, uint32_t CompleteLevel, uint32_t Timeout);
void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_RegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID, void (* pCallback)( DMA_HandleTypeDef * _hdma));
HAL_StatusTypeDef HAL_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID);

/**
  * @}
  */

/** @addtogroup DMA_Exported_Functions_Group3
  * @{
  */
/* 外设状态与错误处理函数 *******************************************************/
HAL_DMA_StateTypeDef HAL_DMA_GetState(DMA_HandleTypeDef *hdma);
uint32_t HAL_DMA_GetError(DMA_HandleTypeDef *hdma);
/**
  * @}
  */

/**
  * @}
  */

/* 私有宏 --------------------------------------------------------------------*/
/** @defgroup DMA_Private_Macros DMA 私有宏
  * @{
  */

#define IS_DMA_DIRECTION(DIRECTION) (((DIRECTION) == DMA_PERIPH_TO_MEMORY ) || \
                                     ((DIRECTION) == DMA_MEMORY_TO_PERIPH)  || \
                                     ((DIRECTION) == DMA_MEMORY_TO_MEMORY))

#define IS_DMA_BUFFER_SIZE(SIZE) (((SIZE) >= 0x1U) && ((SIZE) < 0x10000U))

#define IS_DMA_PERIPHERAL_INC_STATE(STATE) (((STATE) == DMA_PINC_ENABLE) || \
                                            ((STATE) == DMA_PINC_DISABLE))

#define IS_DMA_MEMORY_INC_STATE(STATE) (((STATE) == DMA_MINC_ENABLE)  || \
                                        ((STATE) == DMA_MINC_DISABLE))

#define IS_DMA_PERIPHERAL_DATA_SIZE(SIZE) (((SIZE) == DMA_PDATAALIGN_BYTE)     || \
                                           ((SIZE) == DMA_PDATAALIGN_HALFWORD) || \
                                           ((SIZE) == DMA_PDATAALIGN_WORD))

#define IS_DMA_MEMORY_DATA_SIZE(SIZE) (((SIZE) == DMA_MDATAALIGN_BYTE)     || \
                                       ((SIZE) == DMA_MDATAALIGN_HALFWORD) || \
                                       ((SIZE) == DMA_MDATAALIGN_WORD ))

#define IS_DMA_MODE(MODE) (((MODE) == DMA_NORMAL )  || \
                           ((MODE) == DMA_CIRCULAR))

#define IS_DMA_PRIORITY(PRIORITY) (((PRIORITY) == DMA_PRIORITY_LOW )   || \
                                   ((PRIORITY) == DMA_PRIORITY_MEDIUM) || \
                                   ((PRIORITY) == DMA_PRIORITY_HIGH)   || \
                                   ((PRIORITY) == DMA_PRIORITY_VERY_HIGH))

/**
  * @}
  */ 

/* 私有函数 ------------------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_DMA_H */


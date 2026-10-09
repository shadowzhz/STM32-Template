/**
  ******************************************************************************
  * @file    stm32f1xx_hal_tim.h
  * @author  MCD Application Team
  * @brief   TIM HAL 模块头文件。
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * 保留所有权利。
  *
  * 本软件根据该软件组件根目录中 LICENSE 文件中的条款进行许可。
  * 如果本软件未随附 LICENSE 文件，则按“原样”提供。
  *
  ******************************************************************************
  */

/* 防止递归包含的宏定义 -----------------------------------------------------*/
#ifndef STM32F1xx_HAL_TIM_H
#define STM32F1xx_HAL_TIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup TIM
  * @{
  */

/* 导出的类型 ----------------------------------------------------------------*/
/** @defgroup TIM_Exported_Types TIM 导出类型
  * @{
  */

/**
  * @brief  TIM 时基配置结构体定义
  */
typedef struct
{
  uint32_t Prescaler;         /*!< 指定用于分频 TIM 时钟的预分频器值。
                                   该参数可以是介于 Min_Data = 0x0000 和 Max_Data = 0xFFFF 之间的数值 */

  uint32_t CounterMode;       /*!< 指定计数器模式。
                                   该参数可以是 @ref TIM_Counter_Mode 的取值之一 */

  uint32_t Period;            /*!< 指定在下一次更新事件发生时加载到活动自动重载寄存器中的周期值。
                                   该参数可以是介于 Min_Data = 0x0000 和 Max_Data = 0xFFFF 之间的数值。  */

  uint32_t ClockDivision;     /*!< 指定时钟分频系数。
                                   该参数可以是 @ref TIM_ClockDivision 的取值之一 */

  uint32_t RepetitionCounter;  /*!< 指定重复计数器的值。每当 RCR 递减计数器
                                    减至 0 时，都会生成一次更新事件，并从 RCR 值 (N) 重新开始计数。
                                    这意味着在 PWM 模式下，(N+1) 对应：
                                        - 边沿对齐模式下的 PWM 周期数
                                        - 中心对齐模式下的半 PWM 周期数
                                     通用定时器：该参数必须是介于 Min_Data = 0x00 和
                                     Max_Data = 0xFF 之间的数值。
                                     高级定时器：该参数必须是介于 Min_Data = 0x0000 和
                                     Max_Data = 0xFFFF 之间的数值。 */

  uint32_t AutoReloadPreload;  /*!< 指定自动重载预装载配置。
                                   该参数可以是 @ref TIM_AutoReloadPreload 的取值之一 */
} TIM_Base_InitTypeDef;

/**
  * @brief  TIM 输出比较配置结构体定义
  */
typedef struct
{
  uint32_t OCMode;        /*!< 指定 TIM 模式。
                               此参数可以是 @ref TIM_Output_Compare_and_PWM_modes 的一个值 */

  uint32_t Pulse;         /*!< 指定加载到捕获/比较寄存器中的脉冲值。
                               此参数可以是介于 Min_Data = 0x0000 与 Max_Data = 0xFFFF 之间的数值 */

  uint32_t OCPolarity;    /*!< 指定输出极性。
                               此参数可以是 @ref TIM_Output_Compare_Polarity 的一个值 */

  uint32_t OCNPolarity;   /*!< 指定互补输出极性。
                               此参数可以是 @ref TIM_Output_Compare_N_Polarity 的一个值
                               @note 此参数仅对支持刹车特性的定时器实例有效。 */

  uint32_t OCFastMode;    /*!< 指定快速模式状态。
                               此参数可以是 @ref TIM_Output_Fast_State 的一个值
                               @note 此参数仅在 PWM1 和 PWM2 模式下有效。 */


  uint32_t OCIdleState;   /*!< 指定空闲状态下的 TIM 输出比较引脚状态。
                               此参数可以是 @ref TIM_Output_Compare_Idle_State 的一个值
                               @note 此参数仅对支持刹车特性的定时器实例有效。 */

  uint32_t OCNIdleState;  /*!< 指定空闲状态下的 TIM 输出比较互补引脚状态。
                               此参数可以是 @ref TIM_Output_Compare_N_Idle_State 的一个值
                               @note 此参数仅对支持刹车特性的定时器实例有效。 */
} TIM_OC_InitTypeDef;

/**
  * @brief  TIM 单脉冲模式配置结构体定义
  */
typedef struct
{
  uint32_t OCMode;        /*!< 指定 TIM 模式。
                               此参数可以是 @ref TIM_Output_Compare_and_PWM_modes 的一个值 */

  uint32_t Pulse;         /*!< 指定加载到捕获/比较寄存器中的脉冲值。
                               此参数可以是介于 Min_Data = 0x0000 与 Max_Data = 0xFFFF 之间的数值 */

  uint32_t OCPolarity;    /*!< 指定输出极性。
                               此参数可以是 @ref TIM_Output_Compare_Polarity 的一个值 */

  uint32_t OCNPolarity;   /*!< 指定互补输出极性。
                               此参数可以是 @ref TIM_Output_Compare_N_Polarity 的一个值
                               @note 此参数仅对支持刹车特性的定时器实例有效。 */

  uint32_t OCIdleState;   /*!< 指定空闲状态下的 TIM 输出比较引脚状态。
                               此参数可以是 @ref TIM_Output_Compare_Idle_State 的一个值
                               @note 此参数仅对支持刹车特性的定时器实例有效。 */

  uint32_t OCNIdleState;  /*!< 指定空闲状态下的 TIM 输出比较互补引脚状态。
                               此参数可以是 @ref TIM_Output_Compare_N_Idle_State 的一个值
                               @note 此参数仅对支持刹车特性的定时器实例有效。 */

  uint32_t ICPolarity;    /*!< 指定输入信号的有效边沿。
                               此参数可以是 @ref TIM_Input_Capture_Polarity 的一个值 */

  uint32_t ICSelection;   /*!< 指定输入源。
                              此参数可以是 @ref TIM_Input_Capture_Selection 的一个值 */

  uint32_t ICFilter;      /*!< 指定输入捕获滤波器。
                              此参数可以是介于 Min_Data = 0x0 和 Max_Data = 0xF 之间的数值 */
} TIM_OnePulse_InitTypeDef;

/**
  * @brief  TIM 输入捕获配置结构体定义
  */
typedef struct
{
  uint32_t  ICPolarity;  /*!< 指定输入信号的有效边沿。
                              此参数可以是 @ref TIM_Input_Capture_Polarity 的一个值 */

  uint32_t ICSelection;  /*!< 指定输入源。
                              此参数可以是 @ref TIM_Input_Capture_Selection 的一个值 */

  uint32_t ICPrescaler;  /*!< 指定输入捕获预分频器。
                              此参数可以是 @ref TIM_Input_Capture_Prescaler 的一个值 */

  uint32_t ICFilter;     /*!< 指定输入捕获滤波器。
                              此参数可以是介于 Min_Data = 0x0 和 Max_Data = 0xF 之间的数值 */
} TIM_IC_InitTypeDef;

/**
  * @brief  TIM 编码器配置结构体定义
  */
typedef struct
{
  uint32_t EncoderMode;   /*!< 指定编码器模式（输入信号的有效边沿）。
                               此参数可以是 @ref TIM_Encoder_Mode 的一个值 */

  uint32_t IC1Polarity;   /*!< 指定输入信号的有效边沿。
                               此参数可以是 @ref TIM_Encoder_Input_Polarity 的一个值 */

  uint32_t IC1Selection;  /*!< 指定输入源。
                               此参数可以是 @ref TIM_Input_Capture_Selection 的一个值 */

  uint32_t IC1Prescaler;  /*!< 指定输入捕获预分频器。
                               此参数可以是 @ref TIM_Input_Capture_Prescaler 的一个值 */

  uint32_t IC1Filter;     /*!< 指定输入捕获滤波器。
                               此参数可以是介于 Min_Data = 0x0 和 Max_Data = 0xF 之间的数值 */

  uint32_t IC2Polarity;   /*!< 指定输入信号的有效边沿。
                               此参数可以是 @ref TIM_Encoder_Input_Polarity 的一个值 */

  uint32_t IC2Selection;  /*!< 指定输入源。
                              此参数可以是 @ref TIM_Input_Capture_Selection 的一个值 */

  uint32_t IC2Prescaler;  /*!< 指定输入捕获预分频器。
                               此参数可以是 @ref TIM_Input_Capture_Prescaler 的值 */

  uint32_t IC2Filter;     /*!< 指定输入捕获滤波器。
                               此参数可以是介于 Min_Data = 0x0 与 Max_Data = 0xF 之间的数值 */
} TIM_Encoder_InitTypeDef;

/**
  * @brief  时钟配置句柄结构体定义
  */
typedef struct
{
  uint32_t ClockSource;     /*!< TIM 时钟源
                                 此参数可以是 @ref TIM_Clock_Source 的值 */
  uint32_t ClockPolarity;   /*!< TIM 时钟极性
                                 此参数可以是 @ref TIM_Clock_Polarity 的值 */
  uint32_t ClockPrescaler;  /*!< TIM 时钟预分频器
                                 此参数可以是 @ref TIM_Clock_Prescaler 的值 */
  uint32_t ClockFilter;     /*!< TIM 时钟滤波器
                                 此参数可以是介于 Min_Data = 0x0 与 Max_Data = 0xF 之间的数值 */
} TIM_ClockConfigTypeDef;

/**
  * @brief  TIM 清除输入配置句柄结构体定义
  */
typedef struct
{
  uint32_t ClearInputState;      /*!< TIM 清除输入状态
                                      此参数可以是 ENABLE 或 DISABLE */
  uint32_t ClearInputSource;     /*!< TIM 清除输入源
                                      此参数可以是 @ref TIM_ClearInput_Source 的值 */
  uint32_t ClearInputPolarity;   /*!< TIM 清除输入极性
                                      此参数可以是 @ref TIM_ClearInput_Polarity 的值 */
  uint32_t ClearInputPrescaler;  /*!< TIM 清除输入预分频器
                                      此参数必须为 0：当 OCRef 清除功能与 ETR 源一起使用时，
                                      ETR 预分频器必须关闭 */
  uint32_t ClearInputFilter;     /*!< TIM 清除输入滤波器
                                      此参数可以是介于 Min_Data = 0x0 与 Max_Data = 0xF 之间的数值 */
} TIM_ClearInputConfigTypeDef;

/**
  * @brief  TIM 主模式配置结构体定义
  */
typedef struct
{
  uint32_t  MasterOutputTrigger;   /*!< 触发输出 (TRGO) 选择
                                        此参数可以是 @ref TIM_Master_Mode_Selection 的值 */
  uint32_t  MasterSlaveMode;       /*!< 主/从模式选择
                                        此参数可取自 @ref TIM_Master_Slave_Mode 的值
                                        @note 启用主/从模式时，触发输入 (TRGI) 上的
                                        事件响应会被延迟，以实现当前定时器与其
                                        从定时器之间（通过 TRGO）的完美同步。
                                        在定时器同步模式下，该配置不是强制性的。 */
} TIM_MasterConfigTypeDef;

/**
  * @brief  TIM 从模式配置结构体定义
  */
typedef struct
{
  uint32_t  SlaveMode;         /*!< 从模式选择
                                    此参数可取自 @ref TIM_Slave_Mode 的值 */
  uint32_t  InputTrigger;      /*!< 输入触发源
                                    此参数可取自 @ref TIM_Trigger_Selection 的值 */
  uint32_t  TriggerPolarity;   /*!< 输入触发极性
                                    此参数可取自 @ref TIM_Trigger_Polarity 的值 */
  uint32_t  TriggerPrescaler;  /*!< 输入触发预分频器
                                    此参数可取自 @ref TIM_Trigger_Prescaler 的值 */
  uint32_t  TriggerFilter;     /*!< 输入触发滤波器
                                    此参数必须是介于 Min_Data = 0x0 和 Max_Data = 0xF 之间的数值 */

} TIM_SlaveConfigTypeDef;

/**
  * @brief  TIM 刹车输入与死区时间配置结构体定义
  * @note   可配置 2 个刹车输入（BKIN 和 BKIN2），并支持配置
  *        滤波器和极性。
  */
typedef struct
{
  uint32_t OffStateRunMode;      /*!< 运行模式下的 TIM 关闭状态选择 (OSSR)，此参数可取自 @ref TIM_OSSR_Off_State_Selection_for_Run_mode_state 的值 */

  uint32_t OffStateIDLEMode;     /*!< 空闲模式下的 TIM 关闭状态选择 (OSSI)，此参数可取自 @ref TIM_OSSI_Off_State_Selection_for_Idle_mode_state 的值 */

  uint32_t LockLevel;            /*!< TIM 锁定级别，此参数可取自 @ref TIM_Lock_level 的值 */

  uint32_t DeadTime;             /*!< TIM 死区时间，此参数必须是介于 Min_Data = 0x00 和 Max_Data = 0xFF 之间的数值 */

  uint32_t BreakState;           /*!< TIM 刹车使能状态，此参数可取自 @ref TIM_Break_Input_enable_disable 的值 */

  uint32_t BreakPolarity;        /*!< TIM 刹车输入极性，此参数可取自 @ref TIM_Break_Polarity 的值 */

  uint32_t BreakFilter;          /*!< 指定刹车输入滤波器。此参数必须是介于 Min_Data = 0x0 和 Max_Data = 0xF 之间的数值 */


  uint32_t AutomaticOutput;      /*!< TIM 自动输出使能状态，该参数可取值于 @ref TIM_AOE_Bit_Set_Reset */

} TIM_BreakDeadTimeConfigTypeDef;

/**
  * @brief  HAL 状态结构体定义
  */
typedef enum
{
  HAL_TIM_STATE_RESET             = 0x00U,    /*!< 外设尚未初始化或已禁用                      */
  HAL_TIM_STATE_READY             = 0x01U,    /*!< 外设已初始化并就绪可用                      */
  HAL_TIM_STATE_BUSY              = 0x02U,    /*!< 内部处理正在进行中                          */
  HAL_TIM_STATE_TIMEOUT           = 0x03U,    /*!< 超时状态                                    */
  HAL_TIM_STATE_ERROR             = 0x04U     /*!< 接收处理正在进行中                          */
} HAL_TIM_StateTypeDef;

/**
  * @brief  TIM 通道状态定义
  */
typedef enum
{
  HAL_TIM_CHANNEL_STATE_RESET             = 0x00U,    /*!< TIM 通道初始状态                                  */
  HAL_TIM_CHANNEL_STATE_READY             = 0x01U,    /*!< TIM 通道就绪可用                                  */
  HAL_TIM_CHANNEL_STATE_BUSY              = 0x02U,    /*!< TIM 通道内部处理正在进行中                        */
} HAL_TIM_ChannelStateTypeDef;

/**
  * @brief  DMA 突发传输状态定义
  */
typedef enum
{
  HAL_DMA_BURST_STATE_RESET             = 0x00U,    /*!< DMA 突发传输初始状态 */
  HAL_DMA_BURST_STATE_READY             = 0x01U,    /*!< DMA 突发传输就绪可用 */
  HAL_DMA_BURST_STATE_BUSY              = 0x02U,    /*!< DMA 突发传输正在进行中   */
} HAL_TIM_DMABurstStateTypeDef;

/**
  * @brief  HAL 有效通道结构体定义
  */
typedef enum
{
  HAL_TIM_ACTIVE_CHANNEL_1        = 0x01U,    /*!< 有效通道为通道 1            */
  HAL_TIM_ACTIVE_CHANNEL_2        = 0x02U,    /*!< 有效通道为通道 2            */
  HAL_TIM_ACTIVE_CHANNEL_3        = 0x04U,    /*!< 有效通道为通道 3            */
  HAL_TIM_ACTIVE_CHANNEL_4        = 0x08U,    /*!< 有效通道为通道 4            */
  HAL_TIM_ACTIVE_CHANNEL_CLEARED  = 0x00U     /*!< 清除所有有效通道            */
} HAL_TIM_ActiveChannel;

/**
  * @brief  TIM 时基句柄结构体定义
  */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
typedef struct __TIM_HandleTypeDef
#else
typedef struct
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
{
  TIM_TypeDef                        *Instance;         /*!< 寄存器基地址                                      */
  TIM_Base_InitTypeDef               Init;              /*!< TIM 时基必需参数                                  */
  HAL_TIM_ActiveChannel              Channel;           /*!< 有效通道                                          */
  DMA_HandleTypeDef                  *hdma[7];          /*!< DMA 句柄数组
                                                             该数组通过 @ref DMA_Handle_index 进行访问 */
  HAL_LockTypeDef                    Lock;              /*!< 锁定对象                                          */
  __IO HAL_TIM_StateTypeDef          State;             /*!< TIM 运行状态                                      */
  __IO HAL_TIM_ChannelStateTypeDef   ChannelState[4];   /*!< TIM 通道运行状态                                  */
  __IO HAL_TIM_ChannelStateTypeDef   ChannelNState[4];  /*!< TIM 互补通道操作状态                             */
  __IO HAL_TIM_DMABurstStateTypeDef  DMABurstState;     /*!< DMA 突发传输操作状态                             */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  void (* Base_MspInitCallback)(struct __TIM_HandleTypeDef *htim);              /*!< TIM 基本定时 Msp 初始化回调函数                  */
  void (* Base_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);            /*!< TIM 基本定时 Msp 反初始化回调函数                */
  void (* IC_MspInitCallback)(struct __TIM_HandleTypeDef *htim);                /*!< TIM 输入捕获 (IC) Msp 初始化回调函数             */
  void (* IC_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);              /*!< TIM 输入捕获 (IC) Msp 反初始化回调函数           */
  void (* OC_MspInitCallback)(struct __TIM_HandleTypeDef *htim);                /*!< TIM 输出比较 (OC) Msp 初始化回调函数             */
  void (* OC_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);              /*!< TIM 输出比较 (OC) Msp 反初始化回调函数           */
  void (* PWM_MspInitCallback)(struct __TIM_HandleTypeDef *htim);               /*!< TIM PWM Msp 初始化回调函数                       */
  void (* PWM_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);             /*!< TIM PWM Msp 反初始化回调函数                     */
  void (* OnePulse_MspInitCallback)(struct __TIM_HandleTypeDef *htim);          /*!< TIM 单脉冲 (One Pulse) Msp 初始化回调函数        */
  void (* OnePulse_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);        /*!< TIM 单脉冲 (One Pulse) Msp 反初始化回调函数      */
  void (* Encoder_MspInitCallback)(struct __TIM_HandleTypeDef *htim);           /*!< TIM 编码器模式 Msp 初始化回调函数                */
  void (* Encoder_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);         /*!< TIM 编码器模式 Msp 反初始化回调函数              */
  void (* HallSensor_MspInitCallback)(struct __TIM_HandleTypeDef *htim);        /*!< TIM 霍尔传感器 Msp 初始化回调函数                */
  void (* HallSensor_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);      /*!< TIM 霍尔传感器 Msp 反初始化回调函数                     */
  void (* PeriodElapsedCallback)(struct __TIM_HandleTypeDef *htim);             /*!< TIM 周期溢出/更新回调函数                                */
  void (* PeriodElapsedHalfCpltCallback)(struct __TIM_HandleTypeDef *htim);     /*!< TIM 周期溢出半完成回调函数                               */
  void (* TriggerCallback)(struct __TIM_HandleTypeDef *htim);                   /*!< TIM 触发回调函数                                        */
  void (* TriggerHalfCpltCallback)(struct __TIM_HandleTypeDef *htim);           /*!< TIM 触发半完成回调函数                                  */
  void (* IC_CaptureCallback)(struct __TIM_HandleTypeDef *htim);                /*!< TIM 输入捕获回调函数                                    */
  void (* IC_CaptureHalfCpltCallback)(struct __TIM_HandleTypeDef *htim);        /*!< TIM 输入捕获半完成回调函数                              */
  void (* OC_DelayElapsedCallback)(struct __TIM_HandleTypeDef *htim);           /*!< TIM 输出比较延时到达回调函数                            */
  void (* PWM_PulseFinishedCallback)(struct __TIM_HandleTypeDef *htim);         /*!< TIM PWM 脉冲完成回调函数                                */
  void (* PWM_PulseFinishedHalfCpltCallback)(struct __TIM_HandleTypeDef *htim); /*!< TIM PWM 脉冲半完成回调函数                              */
  void (* ErrorCallback)(struct __TIM_HandleTypeDef *htim);                     /*!< TIM 错误回调函数                                        */
  void (* CommutationCallback)(struct __TIM_HandleTypeDef *htim);               /*!< TIM 换相回调函数                                        */
  void (* CommutationHalfCpltCallback)(struct __TIM_HandleTypeDef *htim);       /*!< TIM 换相半完成回调函数                                  */
  void (* BreakCallback)(struct __TIM_HandleTypeDef *htim);                     /*!< TIM 刹车回调函数                                        */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
} TIM_HandleTypeDef;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL TIM 回调函数 ID 枚举定义
  */
typedef enum
{
  HAL_TIM_BASE_MSPINIT_CB_ID              = 0x00U   /*!< TIM 基本定时模式 MspInit 回调函数 ID                       */
  , HAL_TIM_BASE_MSPDEINIT_CB_ID          = 0x01U   /*!< TIM 基本定时模式 MspDeInit 回调函数 ID                     */
  , HAL_TIM_IC_MSPINIT_CB_ID              = 0x02U   /*!< TIM 输入捕获 (IC) MspInit 回调函数 ID                      */
  , HAL_TIM_IC_MSPDEINIT_CB_ID            = 0x03U   /*!< TIM 输入捕获 (IC) MspDeInit 回调函数 ID                    */
  , HAL_TIM_OC_MSPINIT_CB_ID              = 0x04U   /*!< TIM 输出比较 (OC) MspInit 回调函数 ID                      */
  , HAL_TIM_OC_MSPDEINIT_CB_ID            = 0x05U   /*!< TIM 输出比较 (OC) MspDeInit 回调函数 ID                    */
  , HAL_TIM_PWM_MSPINIT_CB_ID             = 0x06U   /*!< TIM PWM 模式 MspInit 回调函数 ID                           */
  , HAL_TIM_PWM_MSPDEINIT_CB_ID           = 0x07U   /*!< TIM PWM 模式 MspDeInit 回调函数 ID                         */
  , HAL_TIM_ONE_PULSE_MSPINIT_CB_ID       = 0x08U   /*!< TIM 单脉冲模式 MspInit 回调函数 ID                         */
  , HAL_TIM_ONE_PULSE_MSPDEINIT_CB_ID     = 0x09U   /*!< TIM 单脉冲模式 MspDeInit 回调函数 ID                       */
  , HAL_TIM_ENCODER_MSPINIT_CB_ID         = 0x0AU   /*!< TIM 编码器模式 MspInit 回调函数 ID                         */
  , HAL_TIM_ENCODER_MSPDEINIT_CB_ID       = 0x0BU   /*!< TIM 编码器模式 MspDeInit 回调函数 ID                       */
  , HAL_TIM_HALL_SENSOR_MSPINIT_CB_ID     = 0x0CU   /*!< TIM 霍尔传感器模式 MspDeInit 回调函数 ID                   */
  , HAL_TIM_HALL_SENSOR_MSPDEINIT_CB_ID   = 0x0DU   /*!< TIM 霍尔传感器模式 MspDeInit 回调函数 ID                   */
  , HAL_TIM_PERIOD_ELAPSED_CB_ID          = 0x0EU   /*!< TIM 周期溢出回调函数 ID                                     */
  , HAL_TIM_PERIOD_ELAPSED_HALF_CB_ID     = 0x0FU   /*!< TIM 周期溢出过半回调函数 ID                                 */
  , HAL_TIM_TRIGGER_CB_ID                 = 0x10U   /*!< TIM 触发回调函数 ID                                         */
  , HAL_TIM_TRIGGER_HALF_CB_ID            = 0x11U   /*!< TIM 触发过半回调函数 ID                                     */
  , HAL_TIM_IC_CAPTURE_CB_ID              = 0x12U   /*!< TIM 输入捕获回调函数 ID                                     */
  , HAL_TIM_IC_CAPTURE_HALF_CB_ID         = 0x13U   /*!< TIM 输入捕获过半回调函数 ID                                 */
  , HAL_TIM_OC_DELAY_ELAPSED_CB_ID        = 0x14U   /*!< TIM 输出比较延时到达回调函数 ID                             */
  , HAL_TIM_PWM_PULSE_FINISHED_CB_ID      = 0x15U   /*!< TIM PWM 脉冲完成回调函数 ID                                 */
  , HAL_TIM_PWM_PULSE_FINISHED_HALF_CB_ID = 0x16U   /*!< TIM PWM 脉冲完成过半回调函数 ID                             */
  , HAL_TIM_ERROR_CB_ID                   = 0x17U   /*!< TIM 错误回调函数 ID                                         */
  , HAL_TIM_COMMUTATION_CB_ID             = 0x18U   /*!< TIM 换相回调函数 ID                                         */
  , HAL_TIM_COMMUTATION_HALF_CB_ID        = 0x19U   /*!< TIM 换相过半回调函数 ID                                     */
  , HAL_TIM_BREAK_CB_ID                   = 0x1AU   /*!< TIM 刹车回调函数 ID                                         */
} HAL_TIM_CallbackIDTypeDef;

/**
  * @brief  HAL TIM 回调函数指针定义
  */
typedef  void (*pTIM_CallbackTypeDef)(TIM_HandleTypeDef *htim);  /*!< 指向 TIM 回调函数的指针 */

#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

/**
  * @}
  */
/* 导出类型结束 -----------------------------------------------------*/

/* 导出常量 --------------------------------------------------------*/
/** @defgroup TIM_Exported_Constants TIM 导出常量
  * @{
  */

/** @defgroup TIM_ClearInput_Source TIM 清除输入源
  * @{
  */
#define TIM_CLEARINPUTSOURCE_NONE           0x00000000U   /*!< 禁用 OCREF_CLR */
#define TIM_CLEARINPUTSOURCE_ETR            0x00000001U   /*!< OCREF_CLR 连接到 ETRF 输入 */
/**
  * @}
  */

/** @defgroup TIM_DMA_Base_address TIM DMA 基地址
  * @{
  */
#define TIM_DMABASE_CR1                    0x00000000U
#define TIM_DMABASE_CR2                    0x00000001U
#define TIM_DMABASE_SMCR                   0x00000002U
#define TIM_DMABASE_DIER                   0x00000003U
#define TIM_DMABASE_SR                     0x00000004U
#define TIM_DMABASE_EGR                    0x00000005U
#define TIM_DMABASE_CCMR1                  0x00000006U
#define TIM_DMABASE_CCMR2                  0x00000007U
#define TIM_DMABASE_CCER                   0x00000008U
#define TIM_DMABASE_CNT                    0x00000009U
#define TIM_DMABASE_PSC                    0x0000000AU
#define TIM_DMABASE_ARR                    0x0000000BU
#define TIM_DMABASE_RCR                    0x0000000CU
#define TIM_DMABASE_CCR1                   0x0000000DU
#define TIM_DMABASE_CCR2                   0x0000000EU
#define TIM_DMABASE_CCR3                   0x0000000FU
#define TIM_DMABASE_CCR4                   0x00000010U
#define TIM_DMABASE_BDTR                   0x00000011U
#define TIM_DMABASE_DCR                    0x00000012U
#define TIM_DMABASE_DMAR                   0x00000013U
/**
  * @}
  */

/** @defgroup TIM_Event_Source TIM 事件源
  * @{
  */
#define TIM_EVENTSOURCE_UPDATE              TIM_EGR_UG     /*!< 重新初始化计数器并生成寄存器更新事件 */
#define TIM_EVENTSOURCE_CC1                 TIM_EGR_CC1G   /*!< 通道 1 上生成捕获/比较事件 */
#define TIM_EVENTSOURCE_CC2                 TIM_EGR_CC2G   /*!< 通道 2 上生成捕获/比较事件 */
#define TIM_EVENTSOURCE_CC3                 TIM_EGR_CC3G   /*!< A capture/compare event is generated on channel 3 */
#define TIM_EVENTSOURCE_CC4                 TIM_EGR_CC4G   /*!< A capture/compare event is generated on channel 4 */
#define TIM_EVENTSOURCE_COM                 TIM_EGR_COMG   /*!< A commutation event is generated */
#define TIM_EVENTSOURCE_TRIGGER             TIM_EGR_TG     /*!< A trigger event is generated */
#define TIM_EVENTSOURCE_BREAK               TIM_EGR_BG     /*!< A break event is generated */
/**
  * @}
  */

/** @defgroup TIM_Input_Channel_Polarity TIM Input Channel polarity
  * @{
  */
#define  TIM_INPUTCHANNELPOLARITY_RISING      0x00000000U                       /*!< Polarity for TIx source */
#define  TIM_INPUTCHANNELPOLARITY_FALLING     TIM_CCER_CC1P                     /*!< Polarity for TIx source */
#define  TIM_INPUTCHANNELPOLARITY_BOTHEDGE    (TIM_CCER_CC1P | TIM_CCER_CC1NP)  /*!< Polarity for TIx source */
/**
  * @}
  */

/** @defgroup TIM_ETR_Polarity TIM ETR Polarity
  * @{
  */
#define TIM_ETRPOLARITY_INVERTED              TIM_SMCR_ETP                      /*!< Polarity for ETR source */
#define TIM_ETRPOLARITY_NONINVERTED           0x00000000U                       /*!< Polarity for ETR source */
/**
  * @}
  */

/** @defgroup TIM_ETR_Prescaler TIM ETR Prescaler
  * @{
  */
#define TIM_ETRPRESCALER_DIV1                 0x00000000U                       /*!< No prescaler is used */
#define TIM_ETRPRESCALER_DIV2                 TIM_SMCR_ETPS_0                   /*!< ETR input source is divided by 2 */
#define TIM_ETRPRESCALER_DIV4                 TIM_SMCR_ETPS_1                   /*!< ETR input source is divided by 4 */
#define TIM_ETRPRESCALER_DIV8                 TIM_SMCR_ETPS                     /*!< ETR input source is divided by 8 */
/**
  * @}
  */

/** @defgroup TIM_Counter_Mode TIM Counter Mode
  * @{
  */
#define TIM_COUNTERMODE_UP                 0x00000000U                          /*!< Counter used as up-counter   */
#define TIM_COUNTERMODE_DOWN               TIM_CR1_DIR                          /*!< Counter used as down-counter */
#define TIM_COUNTERMODE_CENTERALIGNED1     TIM_CR1_CMS_0                        /*!< Center-aligned mode 1        */
#define TIM_COUNTERMODE_CENTERALIGNED2     TIM_CR1_CMS_1                        /*!< Center-aligned mode 2        */
#define TIM_COUNTERMODE_CENTERALIGNED3     TIM_CR1_CMS                          /*!< Center-aligned mode 3        */
/**
  * @}
  */

/** @defgroup TIM_ClockDivision TIM Clock Division
  * @{
  */
#define TIM_CLOCKDIVISION_DIV1             0x00000000U                          /*!< Clock division: tDTS=tCK_INT   */
#define TIM_CLOCKDIVISION_DIV2             TIM_CR1_CKD_0                        /*!< Clock division: tDTS=2*tCK_INT */
#define TIM_CLOCKDIVISION_DIV4             TIM_CR1_CKD_1                        /*!< Clock division: tDTS=4*tCK_INT */
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_State TIM Output Compare State
  * @{
  */
#define TIM_OUTPUTSTATE_DISABLE            0x00000000U                          /*!< Capture/Compare 1 output disabled */
#define TIM_OUTPUTSTATE_ENABLE             TIM_CCER_CC1E                        /*!< Capture/Compare 1 output enabled */
/**
  * @}
  */

/** @defgroup TIM_AutoReloadPreload TIM Auto-Reload Preload
  * @{
  */
#define TIM_AUTORELOAD_PRELOAD_DISABLE                0x00000000U               /*!< TIMx_ARR register is not buffered */
#define TIM_AUTORELOAD_PRELOAD_ENABLE                 TIM_CR1_ARPE              /*!< TIMx_ARR register is buffered */

/**
  * @}
  */

/** @defgroup TIM_Output_Fast_State TIM Output Fast State
  * @{
  */
#define TIM_OCFAST_DISABLE                 0x00000000U                          /*!< Output Compare fast disable */
#define TIM_OCFAST_ENABLE                  TIM_CCMR1_OC1FE                      /*!< Output Compare fast enable  */
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_N_State TIM Complementary Output Compare State
  * @{
  */
#define TIM_OUTPUTNSTATE_DISABLE           0x00000000U                          /*!< OCxN is disabled  */
#define TIM_OUTPUTNSTATE_ENABLE            TIM_CCER_CC1NE                       /*!< OCxN is enabled   */
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_Polarity TIM Output Compare Polarity
  * @{
  */
#define TIM_OCPOLARITY_HIGH                0x00000000U                          /*!< Capture/Compare output polarity  */
#define TIM_OCPOLARITY_LOW                 TIM_CCER_CC1P                        /*!< Capture/Compare output polarity  */
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_N_Polarity TIM Complementary Output Compare Polarity
  * @{
  */
#define TIM_OCNPOLARITY_HIGH               0x00000000U                          /*!< Capture/Compare complementary output polarity */
#define TIM_OCNPOLARITY_LOW                TIM_CCER_CC1NP                       /*!< Capture/Compare complementary output polarity */
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_Idle_State TIM Output Compare Idle State
  * @{
  */
#define TIM_OCIDLESTATE_SET                TIM_CR2_OIS1                         /*!< Output Idle state: OCx=1 when MOE=0 */
#define TIM_OCIDLESTATE_RESET              0x00000000U                          /*!< Output Idle state: OCx=0 when MOE=0 */
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_N_Idle_State TIM Complementary Output Compare Idle State
  * @{
  */
#define TIM_OCNIDLESTATE_SET               TIM_CR2_OIS1N                        /*!< Complementary output Idle state: OCxN=1 when MOE=0 */
#define TIM_OCNIDLESTATE_RESET             0x00000000U                          /*!< Complementary output Idle state: OCxN=0 when MOE=0 */
/**
  * @}
  */

/** @defgroup TIM_Input_Capture_Polarity TIM Input Capture Polarity
  * @{
  */
#define  TIM_ICPOLARITY_RISING             TIM_INPUTCHANNELPOLARITY_RISING      /*!< Capture triggered by rising edge on timer input                  */
#define  TIM_ICPOLARITY_FALLING            TIM_INPUTCHANNELPOLARITY_FALLING     /*!< Capture triggered by falling edge on timer input                 */
#define  TIM_ICPOLARITY_BOTHEDGE           TIM_INPUTCHANNELPOLARITY_BOTHEDGE    /*!< Capture triggered by both rising and falling edges on timer input*/
/**
  * @}
  */

/** @defgroup TIM_Encoder_Input_Polarity TIM Encoder Input Polarity
  * @{
  */
#define  TIM_ENCODERINPUTPOLARITY_RISING   TIM_INPUTCHANNELPOLARITY_RISING      /*!< Encoder input with rising edge polarity  */
#define  TIM_ENCODERINPUTPOLARITY_FALLING  TIM_INPUTCHANNELPOLARITY_FALLING     /*!< Encoder input with falling edge polarity */
/**
  * @}
  */

/** @defgroup TIM_Input_Capture_Selection TIM Input Capture Selection
  * @{
  */
#define TIM_ICSELECTION_DIRECTTI           TIM_CCMR1_CC1S_0                     /*!< TIM Input 1, 2, 3 or 4 is selected to be connected to IC1, IC2, IC3 or IC4, respectively */
#define TIM_ICSELECTION_INDIRECTTI         TIM_CCMR1_CC1S_1                     /*!< TIM Input 1, 2, 3 or 4 is selected to be connected to IC2, IC1, IC4 or IC3, respectively */
#define TIM_ICSELECTION_TRC                TIM_CCMR1_CC1S                       /*!< TIM Input 1, 2, 3 or 4 is selected to be connected to TRC */
/**
  * @}
  */

/** @defgroup TIM_Input_Capture_Prescaler TIM Input Capture Prescaler
  * @{
  */
#define TIM_ICPSC_DIV1                     0x00000000U                          /*!< Capture performed each time an edge is detected on the capture input */
#define TIM_ICPSC_DIV2                     TIM_CCMR1_IC1PSC_0                   /*!< Capture performed once every 2 events                                */
#define TIM_ICPSC_DIV4                     TIM_CCMR1_IC1PSC_1                   /*!< Capture performed once every 4 events                                */
#define TIM_ICPSC_DIV8                     TIM_CCMR1_IC1PSC                     /*!< Capture performed once every 8 events                                */
/**
  * @}
  */

/** @defgroup TIM_One_Pulse_Mode TIM One Pulse Mode
  * @{
  */
#define TIM_OPMODE_SINGLE                  TIM_CR1_OPM                          /*!< Counter stops counting at the next update event */
#define TIM_OPMODE_REPETITIVE              0x00000000U                          /*!< Counter is not stopped at update event          */
/**
  * @}
  */

/** @defgroup TIM_Encoder_Mode TIM Encoder Mode
  * @{
  */
#define TIM_ENCODERMODE_TI1                      TIM_SMCR_SMS_0                                                      /*!< 正交编码器模式 1（2倍频模式），在 TI1FP1 边沿计数，根据 TI2FP2 电平决定递增/递减  */
#define TIM_ENCODERMODE_TI2                      TIM_SMCR_SMS_1                                                      /*!< 正交编码器模式 2（2倍频模式），在 TI2FP2 边沿计数，根据 TI1FP1 电平决定递增/递减。 */
#define TIM_ENCODERMODE_TI12                     (TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0)                                   /*!< 正交编码器模式 3（4倍频模式），在 TI1FP1 和 TI2FP2 的边沿均计数，根据另一路输入的电平决定递增/递减。 */
/**
  * @}
  */

/** @defgroup TIM_Interrupt_definition TIM 中断定义
  * @{
  */
#define TIM_IT_UPDATE                      TIM_DIER_UIE                         /*!< 更新中断                    */
#define TIM_IT_CC1                         TIM_DIER_CC1IE                       /*!< 捕获/比较 1 中断            */
#define TIM_IT_CC2                         TIM_DIER_CC2IE                       /*!< 捕获/比较 2 中断            */
#define TIM_IT_CC3                         TIM_DIER_CC3IE                       /*!< 捕获/比较 3 中断            */
#define TIM_IT_CC4                         TIM_DIER_CC4IE                       /*!< 捕获/比较 4 中断            */
#define TIM_IT_COM                         TIM_DIER_COMIE                       /*!< 换相中断                    */
#define TIM_IT_TRIGGER                     TIM_DIER_TIE                         /*!< 触发中断                    */
#define TIM_IT_BREAK                       TIM_DIER_BIE                         /*!< 刹车中断                    */
/**
  * @}
  */

/** @defgroup TIM_Commutation_Source  TIM 换相源
  * @{
  */
#define TIM_COMMUTATION_TRGI              TIM_CR2_CCUS                          /*!< 当捕获/比较控制位具有预装载功能时，通过置位 COMG 位或在触发输入端出现上升沿时对其进行更新 */
#define TIM_COMMUTATION_SOFTWARE          0x00000000U                           /*!< 当捕获/比较控制位具有预装载功能时，通过置位 COMG 位对其进行更新 */
/**
  * @}
  */

/** @defgroup TIM_DMA_sources TIM DMA 源
  * @{
  */
#define TIM_DMA_UPDATE                     TIM_DIER_UDE                         /*!< DMA 请求由更新事件触发 */
#define TIM_DMA_CC1                        TIM_DIER_CC1DE                       /*!< DMA 请求由捕获/比较匹配 1 事件触发 */
#define TIM_DMA_CC2                        TIM_DIER_CC2DE                       /*!< DMA 请求由捕获/比较匹配 2 事件触发 */
#define TIM_DMA_CC3                        TIM_DIER_CC3DE                       /*!< DMA 请求由捕获/比较匹配 3 事件触发 */
#define TIM_DMA_CC4                        TIM_DIER_CC4DE                       /*!< DMA 请求由捕获/比较匹配 4 事件触发 */
#define TIM_DMA_COM                        TIM_DIER_COMDE                       /*!< DMA 请求由换相事件触发 */
#define TIM_DMA_TRIGGER                    TIM_DIER_TDE                         /*!< DMA 请求由触发事件触发 */
/**
  * @}
  */

/** @defgroup TIM_CC_DMA_Request CCx DMA 请求选择
  * @{
  */
#define TIM_CCDMAREQUEST_CC                 0x00000000U                         /*!< 当发生捕获或比较匹配事件时发送 CCx DMA 请求 */
#define TIM_CCDMAREQUEST_UPDATE             TIM_CR2_CCDS                        /*!< 当发生更新事件时发送 CCx DMA 请求 */
/**
  * @}
  */

/** @defgroup TIM_Flag_definition TIM 标志定义
  * @{
  */
#define TIM_FLAG_UPDATE                    TIM_SR_UIF                           /*!< 更新中断标志                 */
#define TIM_FLAG_CC1                       TIM_SR_CC1IF                         /*!< 捕获/比较 1 中断标志         */
#define TIM_FLAG_CC2                       TIM_SR_CC2IF                         /*!< 捕获/比较 2 中断标志         */
#define TIM_FLAG_CC3                       TIM_SR_CC3IF                         /*!< 捕获/比较 3 中断标志         */
#define TIM_FLAG_CC4                       TIM_SR_CC4IF                         /*!< 捕获/比较 4 中断标志         */
#define TIM_FLAG_COM                       TIM_SR_COMIF                         /*!< 换相中断标志                 */
#define TIM_FLAG_TRIGGER                   TIM_SR_TIF                           /*!< 触发中断标志                 */
#define TIM_FLAG_BREAK                     TIM_SR_BIF                           /*!< 刹车中断标志                 */
#define TIM_FLAG_CC1OF                     TIM_SR_CC1OF                         /*!< 捕获 1 过捕获标志            */
#define TIM_FLAG_CC2OF                     TIM_SR_CC2OF                         /*!< 捕获 2 过捕获标志            */
#define TIM_FLAG_CC3OF                     TIM_SR_CC3OF                         /*!< 捕获 3 过捕获标志            */
#define TIM_FLAG_CC4OF                     TIM_SR_CC4OF                         /*!< 捕获 4 过捕获标志            */
/**
  * @}
  */

/** @defgroup TIM_Channel TIM Channel
  * @{
  */
#define TIM_CHANNEL_1                      0x00000000U                          /*!< Capture/compare channel 1 identifier      */
#define TIM_CHANNEL_2                      0x00000004U                          /*!< Capture/compare channel 2 identifier      */
#define TIM_CHANNEL_3                      0x00000008U                          /*!< Capture/compare channel 3 identifier      */
#define TIM_CHANNEL_4                      0x0000000CU                          /*!< Capture/compare channel 4 identifier      */
#define TIM_CHANNEL_ALL                    0x0000003CU                          /*!< Global Capture/compare channel identifier  */
/**
  * @}
  */

/** @defgroup TIM_Clock_Source TIM Clock Source
  * @{
  */
#define TIM_CLOCKSOURCE_INTERNAL    TIM_SMCR_ETPS_0      /*!< Internal clock source                                 */
#define TIM_CLOCKSOURCE_ETRMODE1    TIM_TS_ETRF          /*!< External clock source mode 1 (ETRF)                   */
#define TIM_CLOCKSOURCE_ETRMODE2    TIM_SMCR_ETPS_1      /*!< External clock source mode 2                          */
#define TIM_CLOCKSOURCE_TI1ED       TIM_TS_TI1F_ED       /*!< External clock source mode 1 (TTI1FP1 + edge detect.) */
#define TIM_CLOCKSOURCE_TI1         TIM_TS_TI1FP1        /*!< External clock source mode 1 (TTI1FP1)                */
#define TIM_CLOCKSOURCE_TI2         TIM_TS_TI2FP2        /*!< External clock source mode 1 (TTI2FP2)                */
#define TIM_CLOCKSOURCE_ITR0        TIM_TS_ITR0          /*!< External clock source mode 1 (ITR0)                   */
#define TIM_CLOCKSOURCE_ITR1        TIM_TS_ITR1          /*!< External clock source mode 1 (ITR1)                   */
#define TIM_CLOCKSOURCE_ITR2        TIM_TS_ITR2          /*!< External clock source mode 1 (ITR2)                   */
#define TIM_CLOCKSOURCE_ITR3        TIM_TS_ITR3          /*!< External clock source mode 1 (ITR3)                   */
/**
  * @}
  */

/** @defgroup TIM_Clock_Polarity TIM Clock Polarity
  * @{
  */
#define TIM_CLOCKPOLARITY_INVERTED           TIM_ETRPOLARITY_INVERTED           /*!< Polarity for ETRx clock sources */
#define TIM_CLOCKPOLARITY_NONINVERTED        TIM_ETRPOLARITY_NONINVERTED        /*!< Polarity for ETRx clock sources */
#define TIM_CLOCKPOLARITY_RISING             TIM_INPUTCHANNELPOLARITY_RISING    /*!< Polarity for TIx clock sources */
#define TIM_CLOCKPOLARITY_FALLING            TIM_INPUTCHANNELPOLARITY_FALLING   /*!< Polarity for TIx clock sources */
#define TIM_CLOCKPOLARITY_BOTHEDGE           TIM_INPUTCHANNELPOLARITY_BOTHEDGE  /*!< Polarity for TIx clock sources */
/**
  * @}
  */

/** @defgroup TIM_Clock_Prescaler TIM Clock Prescaler
  * @{
  */
#define TIM_CLOCKPRESCALER_DIV1                 TIM_ETRPRESCALER_DIV1           /*!< No prescaler is used                                                     */
#define TIM_CLOCKPRESCALER_DIV2                 TIM_ETRPRESCALER_DIV2           /*!< Prescaler for External ETR Clock: Capture performed once every 2 events. */
#define TIM_CLOCKPRESCALER_DIV4                 TIM_ETRPRESCALER_DIV4           /*!< Prescaler for External ETR Clock: Capture performed once every 4 events. */
#define TIM_CLOCKPRESCALER_DIV8                 TIM_ETRPRESCALER_DIV8           /*!< Prescaler for External ETR Clock: Capture performed once every 8 events. */
/**
  * @}
  */

/** @defgroup TIM_ClearInput_Polarity TIM Clear Input Polarity
  * @{
  */
#define TIM_CLEARINPUTPOLARITY_INVERTED           TIM_ETRPOLARITY_INVERTED      /*!< Polarity for ETRx pin */
#define TIM_CLEARINPUTPOLARITY_NONINVERTED        TIM_ETRPOLARITY_NONINVERTED   /*!< Polarity for ETRx pin */
/**
  * @}
  */

/** @defgroup TIM_ClearInput_Prescaler TIM Clear Input Prescaler
  * @{
  */
#define TIM_CLEARINPUTPRESCALER_DIV1              TIM_ETRPRESCALER_DIV1         /*!< No prescaler is used                                                   */
#define TIM_CLEARINPUTPRESCALER_DIV2              TIM_ETRPRESCALER_DIV2         /*!< Prescaler for External ETR pin: Capture performed once every 2 events. */
#define TIM_CLEARINPUTPRESCALER_DIV4              TIM_ETRPRESCALER_DIV4         /*!< Prescaler for External ETR pin: Capture performed once every 4 events. */
#define TIM_CLEARINPUTPRESCALER_DIV8              TIM_ETRPRESCALER_DIV8         /*!< Prescaler for External ETR pin: Capture performed once every 8 events. */
/**
  * @}
  */

/** @defgroup TIM_OSSR_Off_State_Selection_for_Run_mode_state TIM OSSR OffState Selection for Run mode state
  * @{
  */
#define TIM_OSSR_ENABLE                          TIM_BDTR_OSSR                  /*!< When inactive, OC/OCN outputs are enabled (still controlled by the timer)           */
#define TIM_OSSR_DISABLE                         0x00000000U                    /*!< When inactive, OC/OCN outputs are disabled (not controlled any longer by the timer) */
/**
  * @}
  */

/** @defgroup TIM_OSSI_Off_State_Selection_for_Idle_mode_state TIM OSSI OffState Selection for Idle mode state
  * @{
  */
#define TIM_OSSI_ENABLE                          TIM_BDTR_OSSI                  /*!< When inactive, OC/OCN outputs are enabled (still controlled by the timer)           */
#define TIM_OSSI_DISABLE                         0x00000000U                    /*!< When inactive, OC/OCN outputs are disabled (not controlled any longer by the timer) */
/**
  * @}
  */
/** @defgroup TIM_Lock_level  TIM Lock level
  * @{
  */
#define TIM_LOCKLEVEL_OFF                  0x00000000U                          /*!< LOCK OFF     */
#define TIM_LOCKLEVEL_1                    TIM_BDTR_LOCK_0                      /*!< LOCK Level 1 */
#define TIM_LOCKLEVEL_2                    TIM_BDTR_LOCK_1                      /*!< LOCK Level 2 */
#define TIM_LOCKLEVEL_3                    TIM_BDTR_LOCK                        /*!< LOCK Level 3 */
/**
  * @}
  */

/** @defgroup TIM_Break_Input_enable_disable TIM Break Input Enable
  * @{
  */
#define TIM_BREAK_ENABLE                   TIM_BDTR_BKE                         /*!< Break input BRK is enabled  */
#define TIM_BREAK_DISABLE                  0x00000000U                          /*!< Break input BRK is disabled */
/**
  * @}
  */

/** @defgroup TIM_Break_Polarity TIM Break Input Polarity
  * @{
  */
#define TIM_BREAKPOLARITY_LOW              0x00000000U                          /*!< Break input BRK is active low  */
#define TIM_BREAKPOLARITY_HIGH             TIM_BDTR_BKP                         /*!< Break input BRK is active high */
/**
  * @}
  */

/** @defgroup TIM_AOE_Bit_Set_Reset TIM Automatic Output Enable
  * @{
  */
#define TIM_AUTOMATICOUTPUT_DISABLE        0x00000000U                          /*!< MOE can be set only by software */
#define TIM_AUTOMATICOUTPUT_ENABLE         TIM_BDTR_AOE                         /*!< MOE can be set by software or automatically at the next update event (if none of the break inputs BRK and BRK2 is active) */
/**
  * @}
  */

/** @defgroup TIM_Master_Mode_Selection TIM Master Mode Selection
  * @{
  */
#define TIM_TRGO_RESET            0x00000000U                                      /*!< TIMx_EGR.UG bit is used as trigger output (TRGO)              */
#define TIM_TRGO_ENABLE           TIM_CR2_MMS_0                                    /*!< TIMx_CR1.CEN bit is used as trigger output (TRGO)             */
#define TIM_TRGO_UPDATE           TIM_CR2_MMS_1                                    /*!< Update event is used as trigger output (TRGO)                 */
#define TIM_TRGO_OC1              (TIM_CR2_MMS_1 | TIM_CR2_MMS_0)                  /*!< Capture or a compare match 1 is used as trigger output (TRGO) */
#define TIM_TRGO_OC1REF           TIM_CR2_MMS_2                                    /*!< OC1REF signal is used as trigger output (TRGO)                */
#define TIM_TRGO_OC2REF           (TIM_CR2_MMS_2 | TIM_CR2_MMS_0)                  /*!< OC2REF signal is used as trigger output(TRGO)                 */
#define TIM_TRGO_OC3REF           (TIM_CR2_MMS_2 | TIM_CR2_MMS_1)                  /*!< OC3REF signal is used as trigger output(TRGO)                 */
#define TIM_TRGO_OC4REF           (TIM_CR2_MMS_2 | TIM_CR2_MMS_1 | TIM_CR2_MMS_0)  /*!< OC4REF signal is used as trigger output(TRGO)                 */
/**
  * @}
  */

/** @defgroup TIM_Master_Slave_Mode TIM Master/Slave Mode
  * @{
  */
#define TIM_MASTERSLAVEMODE_ENABLE         TIM_SMCR_MSM                         /*!< No action */
#define TIM_MASTERSLAVEMODE_DISABLE        0x00000000U                          /*!< Master/slave mode is selected */
/**
  * @}
  */

/** @defgroup TIM_Slave_Mode TIM Slave mode
  * @{
  */
#define TIM_SLAVEMODE_DISABLE                0x00000000U                                        /*!< Slave mode disabled           */
#define TIM_SLAVEMODE_RESET                  TIM_SMCR_SMS_2                                     /*!< Reset Mode                    */
#define TIM_SLAVEMODE_GATED                  (TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0)                  /*!< Gated Mode                    */
#define TIM_SLAVEMODE_TRIGGER                (TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1)                  /*!< Trigger Mode                  */
#define TIM_SLAVEMODE_EXTERNAL1              (TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0) /*!< External Clock Mode 1         */
/**
  * @}
  */

/** @defgroup TIM_Output_Compare_and_PWM_modes TIM Output Compare and PWM Modes
  * @{
  */
#define TIM_OCMODE_TIMING                   0x00000000U                                              /*!< Frozen                                 */
#define TIM_OCMODE_ACTIVE                   TIM_CCMR1_OC1M_0                                         /*!< Set channel to active level on match   */
#define TIM_OCMODE_INACTIVE                 TIM_CCMR1_OC1M_1                                         /*!< Set channel to inactive level on match */
#define TIM_OCMODE_TOGGLE                   (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0)                    /*!< Toggle                                 */
#define TIM_OCMODE_PWM1                     (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1)                    /*!< PWM mode 1                             */
#define TIM_OCMODE_PWM2                     (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0) /*!< PWM mode 2                             */
#define TIM_OCMODE_FORCED_ACTIVE            (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0)                    /*!< Force active level                     */
#define TIM_OCMODE_FORCED_INACTIVE          TIM_CCMR1_OC1M_2                                         /*!< Force inactive level                   */
/**
  * @}
  */

/** @defgroup TIM_Trigger_Selection TIM Trigger Selection
  * @{
  */
#define TIM_TS_ITR0          0x00000000U                                                       /*!< Internal Trigger 0 (ITR0)              */
#define TIM_TS_ITR1          TIM_SMCR_TS_0                                                     /*!< Internal Trigger 1 (ITR1)              */
#define TIM_TS_ITR2          TIM_SMCR_TS_1                                                     /*!< Internal Trigger 2 (ITR2)              */
#define TIM_TS_ITR3          (TIM_SMCR_TS_0 | TIM_SMCR_TS_1)                                   /*!< Internal Trigger 3 (ITR3)              */
#define TIM_TS_TI1F_ED       TIM_SMCR_TS_2                                                     /*!< TI1 Edge Detector (TI1F_ED)            */
#define TIM_TS_TI1FP1        (TIM_SMCR_TS_0 | TIM_SMCR_TS_2)                                   /*!< Filtered Timer Input 1 (TI1FP1)        */
#define TIM_TS_TI2FP2        (TIM_SMCR_TS_1 | TIM_SMCR_TS_2)                                   /*!< Filtered Timer Input 2 (TI2FP2)        */
#define TIM_TS_ETRF          (TIM_SMCR_TS_0 | TIM_SMCR_TS_1 | TIM_SMCR_TS_2)                   /*!< Filtered External Trigger input (ETRF) */
#define TIM_TS_NONE          0x0000FFFFU                                                       /*!< No trigger selected                    */
/**
  * @}
  */

/** @defgroup TIM_Trigger_Polarity TIM Trigger Polarity
  * @{
  */
#define TIM_TRIGGERPOLARITY_INVERTED           TIM_ETRPOLARITY_INVERTED               /*!< Polarity for ETRx trigger sources             */
#define TIM_TRIGGERPOLARITY_NONINVERTED        TIM_ETRPOLARITY_NONINVERTED            /*!< Polarity for ETRx trigger sources             */
#define TIM_TRIGGERPOLARITY_RISING             TIM_INPUTCHANNELPOLARITY_RISING        /*!< Polarity for TIxFPx or TI1_ED trigger sources */
#define TIM_TRIGGERPOLARITY_FALLING            TIM_INPUTCHANNELPOLARITY_FALLING       /*!< Polarity for TIxFPx or TI1_ED trigger sources */
#define TIM_TRIGGERPOLARITY_BOTHEDGE           TIM_INPUTCHANNELPOLARITY_BOTHEDGE      /*!< Polarity for TIxFPx or TI1_ED trigger sources */
/**
  * @}
  */

/** @defgroup TIM_Trigger_Prescaler TIM Trigger Prescaler
  * @{
  */
#define TIM_TRIGGERPRESCALER_DIV1             TIM_ETRPRESCALER_DIV1             /*!< No prescaler is used                                                       */
#define TIM_TRIGGERPRESCALER_DIV2             TIM_ETRPRESCALER_DIV2             /*!< Prescaler for External ETR Trigger: Capture performed once every 2 events. */
#define TIM_TRIGGERPRESCALER_DIV4             TIM_ETRPRESCALER_DIV4             /*!< 外部 ETR 触发预分频器：每 4 个事件执行一次捕获。 */
#define TIM_TRIGGERPRESCALER_DIV8             TIM_ETRPRESCALER_DIV8             /*!< 外部 ETR 触发预分频器：每 8 个事件执行一次捕获。 */
/**
  * @}
  */

/** @defgroup TIM_TI1_Selection TIM TI1 输入选择
  * @{
  */
#define TIM_TI1SELECTION_CH1               0x00000000U                          /*!< TIMx_CH1 引脚连接到 TI1 输入 */
#define TIM_TI1SELECTION_XORCOMBINATION    TIM_CR2_TI1S                         /*!< TIMx_CH1、CH2 和 CH3 引脚连接到 TI1 输入（异或组合） */
/**
  * @}
  */

/** @defgroup TIM_DMA_Burst_Length TIM DMA 突发传输长度
  * @{
  */
#define TIM_DMABURSTLENGTH_1TRANSFER       0x00000000U                          /*!< 传输到 1 个寄存器，从 TIMx_CR1 + TIMx_DCR.DBA 开始   */
#define TIM_DMABURSTLENGTH_2TRANSFERS      0x00000100U                          /*!< 传输到 2 个寄存器，从 TIMx_CR1 + TIMx_DCR.DBA 开始  */
#define TIM_DMABURSTLENGTH_3TRANSFERS      0x00000200U                          /*!< 传输到 3 个寄存器，从 TIMx_CR1 + TIMx_DCR.DBA 开始  */
#define TIM_DMABURSTLENGTH_4TRANSFERS      0x00000300U                          /*!< 传输到 4 个寄存器，从 TIMx_CR1 + TIMx_DCR.DBA 开始  */
#define TIM_DMABURSTLENGTH_5TRANSFERS      0x00000400U                          /*!< 传输到 5 个寄存器，从 TIMx_CR1 + TIMx_DCR.DBA 开始  */
#define TIM_DMABURSTLENGTH_6TRANSFERS      0x00000500U                          /*!< 传输到 6 个寄存器，从 TIMx_CR1 + TIMx_DCR.DBA 开始  */
#define TIM_DMABURSTLENGTH_7TRANSFERS      0x00000600U                          /*!< 传输到 7 个寄存器，从 TIMx_CR1 + TIMx_DCR.DBA 开始  */
#define TIM_DMABURSTLENGTH_8TRANSFERS      0x00000700U                          /*!< 传输至从 TIMx_CR1 + TIMx_DCR.DBA 开始的 8 个寄存器  */
#define TIM_DMABURSTLENGTH_9TRANSFERS      0x00000800U                          /*!< 传输至从 TIMx_CR1 + TIMx_DCR.DBA 开始的 9 个寄存器  */
#define TIM_DMABURSTLENGTH_10TRANSFERS     0x00000900U                          /*!< 传输至从 TIMx_CR1 + TIMx_DCR.DBA 开始的 10 个寄存器 */
#define TIM_DMABURSTLENGTH_11TRANSFERS     0x00000A00U                          /*!< 传输至从 TIMx_CR1 + TIMx_DCR.DBA 开始的 11 个寄存器 */
#define TIM_DMABURSTLENGTH_12TRANSFERS     0x00000B00U                          /*!< 传输至从 TIMx_CR1 + TIMx_DCR.DBA 开始的 12 个寄存器 */
#define TIM_DMABURSTLENGTH_13TRANSFERS     0x00000C00U                          /*!< 传输至从 TIMx_CR1 + TIMx_DCR.DBA 开始的 13 个寄存器 */
#define TIM_DMABURSTLENGTH_14TRANSFERS     0x00000D00U                          /*!< 传输至从 TIMx_CR1 + TIMx_DCR.DBA 开始的 14 个寄存器 */
#define TIM_DMABURSTLENGTH_15TRANSFERS     0x00000E00U                          /*!< 传输至从 TIMx_CR1 + TIMx_DCR.DBA 开始的 15 个寄存器 */
#define TIM_DMABURSTLENGTH_16TRANSFERS     0x00000F00U                          /*!< 传输至从 TIMx_CR1 + TIMx_DCR.DBA 开始的 16 个寄存器 */
#define TIM_DMABURSTLENGTH_17TRANSFERS     0x00001000U                          /*!< 传输至从 TIMx_CR1 + TIMx_DCR.DBA 开始的 17 个寄存器 */
#define TIM_DMABURSTLENGTH_18TRANSFERS     0x00001100U                          /*!< 传输至从 TIMx_CR1 + TIMx_DCR.DBA 开始的 18 个寄存器 */
/**
  * @}
  */

/** @defgroup DMA_Handle_index TIM DMA 句柄索引
  * @{
  */
#define TIM_DMA_ID_UPDATE                ((uint16_t) 0x0000)       /*!< 用于更新 (Update) DMA 请求的 DMA 句柄索引 */
#define TIM_DMA_ID_CC1                   ((uint16_t) 0x0001)       /*!< 用于捕获/比较 1 (Capture/Compare 1) DMA 请求的 DMA 句柄索引 */
#define TIM_DMA_ID_CC2                   ((uint16_t) 0x0002)       /*!< 用于捕获/比较 2 DMA 请求的 DMA 句柄索引 */
#define TIM_DMA_ID_CC3                   ((uint16_t) 0x0003)       /*!< 用于捕获/比较 3 DMA 请求的 DMA 句柄索引 */
#define TIM_DMA_ID_CC4                   ((uint16_t) 0x0004)       /*!< 用于捕获/比较 4 DMA 请求的 DMA 句柄索引 */
#define TIM_DMA_ID_COMMUTATION           ((uint16_t) 0x0005)       /*!< 用于换相 (Commutation) DMA 请求的 DMA 句柄索引 */
#define TIM_DMA_ID_TRIGGER               ((uint16_t) 0x0006)       /*!< 用于触发 DMA 请求的 DMA 句柄索引 */
/**
  * @}
  */

/** @defgroup Channel_CC_State TIM Capture/Compare Channel State
  * @{
  */
#define TIM_CCx_ENABLE                   0x00000001U                            /*!< 输入或输出通道已使能 */
#define TIM_CCx_DISABLE                  0x00000000U                            /*!< 输入或输出通道已禁用 */
#define TIM_CCxN_ENABLE                  0x00000004U                            /*!< 互补输出通道已使能 */
#define TIM_CCxN_DISABLE                 0x00000000U                            /*!< 互补输出通道已使能 */
/**
  * @}
  */

/**
  * @}
  */
/* 导出常量结束 -------------------------------------------------*/

/* 导出宏 -----------------------------------------------------------*/
/** @defgroup TIM_Exported_Macros TIM 导出宏
  * @{
  */

/** @brief  重置 TIM 句柄状态。
  * @param  __HANDLE__ TIM 句柄。
  * @retval 无
  */
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
#define __HAL_TIM_RESET_HANDLE_STATE(__HANDLE__) do {                                                               \
                                                      (__HANDLE__)->State            = HAL_TIM_STATE_RESET;         \
                                                      (__HANDLE__)->ChannelState[0]  = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelState[1]  = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelState[2]  = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelState[3]  = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelNState[0] = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelNState[1] = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelNState[2] = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelNState[3] = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->DMABurstState    = HAL_DMA_BURST_STATE_RESET;   \
                                                      (__HANDLE__)->Base_MspInitCallback         = NULL;            \
                                                      (__HANDLE__)->Base_MspDeInitCallback       = NULL;            \
                                                      (__HANDLE__)->IC_MspInitCallback           = NULL;            \
                                                      (__HANDLE__)->IC_MspDeInitCallback         = NULL;            \
                                                      (__HANDLE__)->OC_MspInitCallback           = NULL;            \
                                                      (__HANDLE__)->OC_MspDeInitCallback         = NULL;            \
                                                      (__HANDLE__)->PWM_MspInitCallback          = NULL;            \
                                                      (__HANDLE__)->PWM_MspDeInitCallback        = NULL;            \
                                                      (__HANDLE__)->OnePulse_MspInitCallback     = NULL;            \
                                                      (__HANDLE__)->OnePulse_MspDeInitCallback   = NULL;            \
                                                      (__HANDLE__)->Encoder_MspInitCallback      = NULL;            \
                                                      (__HANDLE__)->Encoder_MspDeInitCallback    = NULL;            \
                                                      (__HANDLE__)->HallSensor_MspInitCallback   = NULL;            \
                                                      (__HANDLE__)->HallSensor_MspDeInitCallback = NULL;            \
                                                     } while(0)
#else
#define __HAL_TIM_RESET_HANDLE_STATE(__HANDLE__) do {                                                               \
                                                      (__HANDLE__)->State            = HAL_TIM_STATE_RESET;         \
                                                      (__HANDLE__)->ChannelState[0]  = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelState[1]  = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelState[2]  = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelState[3]  = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelNState[0] = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelNState[1] = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelNState[2] = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->ChannelNState[3] = HAL_TIM_CHANNEL_STATE_RESET; \
                                                      (__HANDLE__)->DMABurstState    = HAL_DMA_BURST_STATE_RESET;   \
                                                     } while(0)
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

/**
  * @brief  使能 TIM 外设。
  * @param  __HANDLE__ TIM 句柄
  * @retval 无
  */
#define __HAL_TIM_ENABLE(__HANDLE__)                 ((__HANDLE__)->Instance->CR1|=(TIM_CR1_CEN))

/**
  * @brief  使能 TIM 主输出。
  * @param  __HANDLE__ TIM 句柄
  * @retval 无
  */
#define __HAL_TIM_MOE_ENABLE(__HANDLE__)             ((__HANDLE__)->Instance->BDTR|=(TIM_BDTR_MOE))

/**
  * @brief  禁用 TIM 外设。
  * @param  __HANDLE__ TIM 句柄
  * @retval 无
  */
#define __HAL_TIM_DISABLE(__HANDLE__) \
  do { \
    if (((__HANDLE__)->Instance->CCER & TIM_CCER_CCxE_MASK) == 0UL) \
    { \
      if(((__HANDLE__)->Instance->CCER & TIM_CCER_CCxNE_MASK) == 0UL) \
      { \
        (__HANDLE__)->Instance->CR1 &= ~(TIM_CR1_CEN); \
      } \
    } \
  } while(0)

/**
  * @brief  禁用 TIM 主输出。
  * @param  __HANDLE__ TIM 句柄
  * @retval 无
  * @note 仅当所有 CCx 和 CCxN 通道均已禁用时，定时器实例的主输出使能才会被禁用
  */
#define __HAL_TIM_MOE_DISABLE(__HANDLE__) \
  do { \
    if (((__HANDLE__)->Instance->CCER & TIM_CCER_CCxE_MASK) == 0UL) \
    { \
      if(((__HANDLE__)->Instance->CCER & TIM_CCER_CCxNE_MASK) == 0UL) \
      { \
        (__HANDLE__)->Instance->BDTR &= ~(TIM_BDTR_MOE); \
      } \
    } \
  } while(0)

/**
  * @brief  禁用 TIM 主输出。
  * @param  __HANDLE__ TIM 句柄
  * @retval 无
  * @note 无条件禁用定时器实例的主输出使能
  */
#define __HAL_TIM_MOE_DISABLE_UNCONDITIONALLY(__HANDLE__)  (__HANDLE__)->Instance->BDTR &= ~(TIM_BDTR_MOE)

/** @brief  使能指定的 TIM 中断。
  * @param  __HANDLE__ 指定 TIM 句柄。
  * @param  __INTERRUPT__ 指定要使能的 TIM 中断源。
  *          该参数可以是以下值之一：
  *            @arg TIM_IT_UPDATE: 更新中断
  *            @arg TIM_IT_CC1:   捕获/比较 1 中断
  *            @arg TIM_IT_CC2:  捕获/比较 2 中断
  *            @arg TIM_IT_CC3:  捕获/比较 3 中断
  *            @arg TIM_IT_CC4:  捕获/比较 4 中断
  *            @arg TIM_IT_COM:   换相中断
  *            @arg TIM_IT_TRIGGER: 触发中断
  *            @arg TIM_IT_BREAK: 刹车中断
  * @retval 无
  */
#define __HAL_TIM_ENABLE_IT(__HANDLE__, __INTERRUPT__)    ((__HANDLE__)->Instance->DIER |= (__INTERRUPT__))

/** @brief  禁用指定的 TIM 中断。
  * @param  __HANDLE__ 指定 TIM 句柄。
  * @param  __INTERRUPT__ 指定要禁用的 TIM 中断源。
  *          该参数可以是以下值之一：
  *            @arg TIM_IT_UPDATE: 更新中断
  *            @arg TIM_IT_CC1:   捕获/比较 1 中断
  *            @arg TIM_IT_CC2:  捕获/比较 2 中断
  *            @arg TIM_IT_CC3:  捕获/比较 3 中断
  *            @arg TIM_IT_CC4:  捕获/比较 4 中断
  *            @arg TIM_IT_COM:   换相中断
  *            @arg TIM_IT_TRIGGER: 触发中断
  *            @arg TIM_IT_BREAK: 刹车中断
  * @retval 无
  */
#define __HAL_TIM_DISABLE_IT(__HANDLE__, __INTERRUPT__)   ((__HANDLE__)->Instance->DIER &= ~(__INTERRUPT__))

/** @brief  使能指定的 DMA 请求。
  * @param  __HANDLE__ 指定 TIM 句柄。
  * @param  __DMA__ 指定要使能的 TIM DMA 请求。
  *          该参数可以是以下值之一：
  *            @arg TIM_DMA_UPDATE: 更新 DMA 请求
  *            @arg TIM_DMA_CC1:   捕获/比较 1 DMA 请求
  *            @arg TIM_DMA_CC2:  捕获/比较 2 DMA 请求
  *            @arg TIM_DMA_CC3:  捕获/比较 3 DMA 请求
  *            @arg TIM_DMA_CC4:  捕获/比较 4 DMA 请求
  *            @arg TIM_DMA_COM:   换相 DMA 请求
  *            @arg TIM_DMA_TRIGGER: 触发 DMA 请求
  * @retval 无
  */
#define __HAL_TIM_ENABLE_DMA(__HANDLE__, __DMA__)         ((__HANDLE__)->Instance->DIER |= (__DMA__))

/** @brief  禁用指定的 DMA 请求。
  * @param  __HANDLE__ 指定 TIM 句柄。
  * @param  __DMA__ 指定要禁用的 TIM DMA 请求。
  *          该参数可以是以下值之一：
  *            @arg TIM_DMA_UPDATE: 更新 DMA 请求
  *            @arg TIM_DMA_CC1:   捕获/比较 1 DMA 请求
  *            @arg TIM_DMA_CC2:  捕获/比较 2 DMA 请求
  *            @arg TIM_DMA_CC3:  捕获/比较 3 DMA 请求
  *            @arg TIM_DMA_CC4:  捕获/比较 4 DMA 请求
  *            @arg TIM_DMA_COM:   换相 DMA 请求
  *            @arg TIM_DMA_TRIGGER: 触发 DMA 请求
  * @retval 无
  */
#define __HAL_TIM_DISABLE_DMA(__HANDLE__, __DMA__)        ((__HANDLE__)->Instance->DIER &= ~(__DMA__))

/** @brief  检查指定的 TIM 中断标志位是否置位。
  * @param  __HANDLE__ 指定 TIM 句柄。
  * @param  __FLAG__ 指定要检查的 TIM 中断标志位。
  *        该参数可以是以下值之一：
  *            @arg TIM_FLAG_UPDATE: 更新中断标志位
  *            @arg TIM_FLAG_CC1: 捕获/比较 1 中断标志位
  *            @arg TIM_FLAG_CC2: 捕获/比较 2 中断标志位
  *            @arg TIM_FLAG_CC3: 捕获/比较 3 中断标志位
  *            @arg TIM_FLAG_CC4: 捕获/比较 4 中断标志位
  *            @arg TIM_FLAG_COM:  换相中断标志位
  *            @arg TIM_FLAG_TRIGGER: 触发中断标志位
  *            @arg TIM_FLAG_BREAK: 刹车中断标志位
  *            @arg TIM_FLAG_CC1OF: 捕获/比较 1 过捕获标志位
  *            @arg TIM_FLAG_CC2OF: 捕获/比较 2 过捕获标志位
  *            @arg TIM_FLAG_CC3OF: 捕获/比较 3 过捕获标志位
  *            @arg TIM_FLAG_CC4OF: 捕获/比较 4 过捕获标志位
  * @retval __FLAG__ 的新状态（TRUE 或 FALSE）。
  */
#define __HAL_TIM_GET_FLAG(__HANDLE__, __FLAG__)          (((__HANDLE__)->Instance->SR &(__FLAG__)) == (__FLAG__))

/** @brief  清除指定的 TIM 中断标志位。
  * @param  __HANDLE__ 指定 TIM 句柄。
  * @param  __FLAG__ 指定要清除的 TIM 中断标志位。
  *        该参数可以是以下值之一：
  *            @arg TIM_FLAG_UPDATE: 更新中断标志位
  *            @arg TIM_FLAG_CC1: 捕获/比较 1 中断标志位
  *            @arg TIM_FLAG_CC2: 捕获/比较 2 中断标志位
  *            @arg TIM_FLAG_CC3: 捕获/比较 3 中断标志位
  *            @arg TIM_FLAG_CC4: 捕获/比较 4 中断标志位
  *            @arg TIM_FLAG_COM:  换相中断标志位
  *            @arg TIM_FLAG_TRIGGER: 触发中断标志位
  *            @arg TIM_FLAG_BREAK: 刹车中断标志位
  *            @arg TIM_FLAG_CC1OF: 捕获/比较 1 过捕获标志位
  *            @arg TIM_FLAG_CC2OF: 捕获/比较 2 过捕获标志位
  *            @arg TIM_FLAG_CC3OF: 捕获/比较 3 过捕获标志位
  *            @arg TIM_FLAG_CC4OF: 捕获/比较 4 过捕获标志位
  * @retval __FLAG__ 的新状态（TRUE 或 FALSE）。
  */
#define __HAL_TIM_CLEAR_FLAG(__HANDLE__, __FLAG__)        ((__HANDLE__)->Instance->SR = ~(__FLAG__))

/**
  * @brief  检查指定的 TIM 中断源是否使能。
  * @param  __HANDLE__ TIM 句柄
  * @param  __INTERRUPT__ 指定要检查的 TIM 中断源。
  *          该参数可以是以下值之一：
  *            @arg TIM_IT_UPDATE: 更新中断
  *            @arg TIM_IT_CC1:   捕获/比较 1 中断
  *            @arg TIM_IT_CC2:  捕获/比较 2 中断
  *            @arg TIM_IT_CC3:  捕获/比较 3 中断
  *            @arg TIM_IT_CC4:  捕获/比较 4 中断
  *            @arg TIM_IT_COM:   换相中断
  *            @arg TIM_IT_TRIGGER: 触发中断
  *            @arg TIM_IT_BREAK: 刹车中断
  * @retval TIM_IT 的状态（SET 或 RESET）。
  */
#define __HAL_TIM_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) ((((__HANDLE__)->Instance->DIER & (__INTERRUPT__)) \
                                                             == (__INTERRUPT__)) ? SET : RESET)

/** @brief 清除 TIM 中断挂起标志位。
  * @param  __HANDLE__ TIM 句柄
  * @param  __INTERRUPT__ 指定待清除的中断挂起标志位。
  *          该参数可以是以下值之一：
  *            @arg TIM_IT_UPDATE: 更新中断
  *            @arg TIM_IT_CC1:   捕获/比较 1 中断
  *            @arg TIM_IT_CC2:  捕获/比较 2 中断
  *            @arg TIM_IT_CC3:  捕获/比较 3 中断
  *            @arg TIM_IT_CC4:  捕获/比较 4 中断
  *            @arg TIM_IT_COM:   换相中断
  *            @arg TIM_IT_TRIGGER: 触发中断
  *            @arg TIM_IT_BREAK: 刹车中断
  * @retval 无
  */
#define __HAL_TIM_CLEAR_IT(__HANDLE__, __INTERRUPT__)      ((__HANDLE__)->Instance->SR = ~(__INTERRUPT__))

/**
  * @brief  指示 TIM 计数器是否用作向下计数器。
  * @param  __HANDLE__ TIM 句柄。
  * @retval False（计数器用作向上计数器）或 True（计数器用作向下计数器）
  * @note 该宏在定时器工作于中央对齐模式或编码器模式下获取计数方向时特别有用。
  */
#define __HAL_TIM_IS_TIM_COUNTING_DOWN(__HANDLE__)    (((__HANDLE__)->Instance->CR1 &(TIM_CR1_DIR)) == (TIM_CR1_DIR))

/**
  * @brief  在运行时设置 TIM 预分频器值。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __PRESC__ 指定预分频器的新值。
  * @retval 无
  */
#define __HAL_TIM_SET_PRESCALER(__HANDLE__, __PRESC__)       ((__HANDLE__)->Instance->PSC = (__PRESC__))

/**
  * @brief  在运行时设置 TIM 计数器寄存器值。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __COUNTER__ 指定计数器寄存器的新值。
  * @retval 无
  */
#define __HAL_TIM_SET_COUNTER(__HANDLE__, __COUNTER__)  ((__HANDLE__)->Instance->CNT = (__COUNTER__))

/**
  * @brief  在运行时获取 TIM 计数器寄存器值。
  * @param  __HANDLE__ TIM 句柄。
  * @retval 定时器计数器寄存器 (TIMx_CNT) 的 16 位或 32 位值
  */
#define __HAL_TIM_GET_COUNTER(__HANDLE__)  ((__HANDLE__)->Instance->CNT)

/**
  * @brief  在运行时设置 TIM 自动重装载寄存器值，无需再次调用任何初始化函数。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __AUTORELOAD__ 指定计数器寄存器的新值。
  * @retval 无
  */
#define __HAL_TIM_SET_AUTORELOAD(__HANDLE__, __AUTORELOAD__) \
  do{                                                    \
    (__HANDLE__)->Instance->ARR = (__AUTORELOAD__);  \
    (__HANDLE__)->Init.Period = (__AUTORELOAD__);    \
  } while(0)

/**
  * @brief  在运行时获取 TIM 自动重装载寄存器值。
  * @param  __HANDLE__ TIM 句柄。
  * @retval 定时器自动重装载寄存器 (TIMx_ARR) 的 16 位或 32 位值
  */
#define __HAL_TIM_GET_AUTORELOAD(__HANDLE__)  ((__HANDLE__)->Instance->ARR)

/**
  * @brief  在运行时设置 TIM 时钟分频值，无需再次调用任何初始化函数。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __CKD__ 指定时钟分频值。
  *          该参数可以是以下值之一：
  *            @arg TIM_CLOCKDIVISION_DIV1: tDTS=tCK_INT
  *            @arg TIM_CLOCKDIVISION_DIV2: tDTS=2*tCK_INT
  *            @arg TIM_CLOCKDIVISION_DIV4: tDTS=4*tCK_INT
  * @retval 无
  */
#define __HAL_TIM_SET_CLOCKDIVISION(__HANDLE__, __CKD__) \
  do{                                                   \
    (__HANDLE__)->Instance->CR1 &= (~TIM_CR1_CKD);  \
    (__HANDLE__)->Instance->CR1 |= (__CKD__);       \
    (__HANDLE__)->Init.ClockDivision = (__CKD__);   \
  } while(0)

/**
  * @brief  在运行时获取 TIM 时钟分频值。
  * @param  __HANDLE__ TIM 句柄。
  * @retval 时钟分频值可以是以下值之一：
  *            @arg TIM_CLOCKDIVISION_DIV1: tDTS=tCK_INT
  *            @arg TIM_CLOCKDIVISION_DIV2: tDTS=2*tCK_INT
  *            @arg TIM_CLOCKDIVISION_DIV4: tDTS=4*tCK_INT
  */
#define __HAL_TIM_GET_CLOCKDIVISION(__HANDLE__)  ((__HANDLE__)->Instance->CR1 & TIM_CR1_CKD)

/**
  * @brief  在运行时设置 TIM 输入捕获预分频器，无需再次调用 HAL_TIM_IC_ConfigChannel()
  *         函数。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __CHANNEL__ 待配置的 TIM 通道。
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选中 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选中 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选中 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选中 TIM 通道 4
  * @param  __ICPSC__ 指定输入捕获预分频器的新值。
  *          该参数可以是以下值之一：
  *            @arg TIM_ICPSC_DIV1: 无预分频
  *            @arg TIM_ICPSC_DIV2: 每 2 个事件捕获一次
  *            @arg TIM_ICPSC_DIV4: 每 4 个事件捕获一次
  *            @arg TIM_ICPSC_DIV8: 每 8 个事件捕获一次
  * @retval 无
  */
#define __HAL_TIM_SET_ICPRESCALER(__HANDLE__, __CHANNEL__, __ICPSC__) \
  do{                                                    \
    TIM_RESET_ICPRESCALERVALUE((__HANDLE__), (__CHANNEL__));  \
    TIM_SET_ICPRESCALERVALUE((__HANDLE__), (__CHANNEL__), (__ICPSC__)); \
  } while(0)

/**
  * @brief  在运行时获取 TIM 输入捕获预分频器值。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __CHANNEL__ 待配置的 TIM 通道。
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 获取输入捕获 1 的预分频器值
  *            @arg TIM_CHANNEL_2: 获取输入捕获 2 的预分频器值
  *            @arg TIM_CHANNEL_3: 获取输入捕获 3 的预分频器值
  *            @arg TIM_CHANNEL_4: 获取输入捕获 4 的预分频器值
  * @retval 输入捕获预分频器值可以是以下值之一：
  *            @arg TIM_ICPSC_DIV1: 无预分频
  *            @arg TIM_ICPSC_DIV2: 每 2 个事件捕获一次
  *            @arg TIM_ICPSC_DIV4: 每 4 个事件捕获一次
  *            @arg TIM_ICPSC_DIV8: 每 8 个事件捕获一次
  */
#define __HAL_TIM_GET_ICPRESCALER(__HANDLE__, __CHANNEL__)  \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCMR1 & TIM_CCMR1_IC1PSC) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? (((__HANDLE__)->Instance->CCMR1 & TIM_CCMR1_IC2PSC) >> 8U) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCMR2 & TIM_CCMR2_IC3PSC) :\
   (((__HANDLE__)->Instance->CCMR2 & TIM_CCMR2_IC4PSC)) >> 8U)

/**
  * @brief  在运行时设置 TIM 捕获/比较寄存器值，无需再次调用 ConfigChannel 函数。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __CHANNEL__ 待配置的 TIM 通道。
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选中 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选中 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选中 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选中 TIM 通道 4
  * @param  __COMPARE__ 指定捕获/比较寄存器的新值。
  * @retval 无
  */
#define __HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__) \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCR1 = (__COMPARE__)) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCR2 = (__COMPARE__)) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCR3 = (__COMPARE__)) :\
   ((__HANDLE__)->Instance->CCR4 = (__COMPARE__)))

/**
  * @brief  在运行时获取 TIM 捕获/比较寄存器值。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __CHANNEL__ 与捕获/比较寄存器关联的 TIM 通道
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 获取捕获/比较 1 寄存器值
  *            @arg TIM_CHANNEL_2: 获取捕获/比较 2 寄存器值
  *            @arg TIM_CHANNEL_3: 获取捕获/比较 3 寄存器值
  *            @arg TIM_CHANNEL_4: 获取捕获/比较 4 寄存器值
  * @retval 捕获/比较寄存器 (TIMx_CCRy) 的 16 位或 32 位值
  */
#define __HAL_TIM_GET_COMPARE(__HANDLE__, __CHANNEL__) \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCR1) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCR2) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCR3) :\
   ((__HANDLE__)->Instance->CCR4))

/**
  * @brief  使能 TIM 输出比较预装载。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __CHANNEL__ 待配置的 TIM 通道。
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选中 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选中 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选中 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选中 TIM 通道 4
  * @retval 无
  */
#define __HAL_TIM_ENABLE_OCxPRELOAD(__HANDLE__, __CHANNEL__)    \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCMR1 |= TIM_CCMR1_OC1PE) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCMR1 |= TIM_CCMR1_OC2PE) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCMR2 |= TIM_CCMR2_OC3PE) :\
   ((__HANDLE__)->Instance->CCMR2 |= TIM_CCMR2_OC4PE))

/**
  * @brief  禁用 TIM 输出比较预装载。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __CHANNEL__ 待配置的 TIM 通道。
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选中 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选中 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选中 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选中 TIM 通道 4
  * @retval 无
  */
#define __HAL_TIM_DISABLE_OCxPRELOAD(__HANDLE__, __CHANNEL__)    \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCMR1 &= ~TIM_CCMR1_OC1PE) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCMR1 &= ~TIM_CCMR1_OC2PE) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCMR2 &= ~TIM_CCMR2_OC3PE) :\
   ((__HANDLE__)->Instance->CCMR2 &= ~TIM_CCMR2_OC4PE))

/**
  * @brief  使能指定通道的快速模式。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __CHANNEL__ 待配置的 TIM 通道。
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选中 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选中 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选中 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选中 TIM 通道 4
  * @note  使能快速模式后，触发输入上的有效边沿的作用相当于 CCx 输出上的比较匹配。
  *        对触发输入进行采样并激活 CCx 输出的延迟缩短至 3 个时钟周期。
  * @note  仅当通道配置为 PWM1 或 PWM2 模式时，快速模式才起作用。
  * @retval 无
  */
#define __HAL_TIM_ENABLE_OCxFAST(__HANDLE__, __CHANNEL__)    \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCMR1 |= TIM_CCMR1_OC1FE) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCMR1 |= TIM_CCMR1_OC2FE) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCMR2 |= TIM_CCMR2_OC3FE) :\
   ((__HANDLE__)->Instance->CCMR2 |= TIM_CCMR2_OC4FE))

/**
  * @brief  禁用指定通道的快速模式。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __CHANNEL__ 待配置的 TIM 通道。
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选中 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选中 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选中 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选中 TIM 通道 4
  * @note  当禁用快速模式时，即使触发处于开启状态，CCx 输出也根据计数器和 CCRx 的值表现正常。
  *        当触发输入出现有效边沿时，激活 CCx 输出的最小延迟为 5 个时钟周期。
  * @retval 无
  */
#define __HAL_TIM_DISABLE_OCxFAST(__HANDLE__, __CHANNEL__)    \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCMR1 &= ~TIM_CCMR1_OC1FE) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCMR1 &= ~TIM_CCMR1_OC2FE) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCMR2 &= ~TIM_CCMR2_OC3FE) :\
   ((__HANDLE__)->Instance->CCMR2 &= ~TIM_CCMR2_OC4FE))

/**
  * @brief  置位 TIMx_CR1 寄存器的更新请求源 (URS) 位。
  * @param  __HANDLE__ TIM 句柄。
  * @note  当置位 TIMx_CR1 寄存器的 URS 位时，仅计数器上溢/下溢会产生更新中断或 DMA 请求（如果已使能）。
  * @retval 无
  */
#define __HAL_TIM_URS_ENABLE(__HANDLE__)  ((__HANDLE__)->Instance->CR1|= TIM_CR1_URS)

/**
  * @brief  复位 TIMx_CR1 寄存器的更新请求源 (URS) 位。
  * @param  __HANDLE__ TIM 句柄。
  * @note  当复位 TIMx_CR1 寄存器的 URS 位时，以下任何事件都会产生更新中断或 DMA 请求（如果已使能）：
  *           _ 计数器上溢或下溢
  *           _ 置位 UG 位
  *           _ 通过从模式控制器生成更新
  * @retval 无
  */
#define __HAL_TIM_URS_DISABLE(__HANDLE__)  ((__HANDLE__)->Instance->CR1&=~TIM_CR1_URS)

/**
  * @brief  运行时设置 TIM 输入捕获 x 的极性。
  * @param  __HANDLE__ TIM 句柄。
  * @param  __CHANNEL__ 待配置的 TIM 通道。
  *          该参数可以是以下值之一：
  *            @arg TIM_CHANNEL_1: 选中 TIM 通道 1
  *            @arg TIM_CHANNEL_2: 选中 TIM 通道 2
  *            @arg TIM_CHANNEL_3: 选中 TIM 通道 3
  *            @arg TIM_CHANNEL_4: 选中 TIM 通道 4
  * @param  __POLARITY__ TIx 源的极性
  *            @arg TIM_INPUTCHANNELPOLARITY_RISING: 上升沿
  *            @arg TIM_INPUTCHANNELPOLARITY_FALLING: 下降沿
  *            @arg TIM_INPUTCHANNELPOLARITY_BOTHEDGE: 双边沿
  * @retval 无
  */
#define __HAL_TIM_SET_CAPTUREPOLARITY(__HANDLE__, __CHANNEL__, __POLARITY__)    \
  do{                                                                     \
    TIM_RESET_CAPTUREPOLARITY((__HANDLE__), (__CHANNEL__));               \
    TIM_SET_CAPTUREPOLARITY((__HANDLE__), (__CHANNEL__), (__POLARITY__)); \
  }while(0)

/** @brief  选择捕获/比较 DMA 请求源。
  * @param  __HANDLE__ 指定 TIM 句柄。
  * @param  __CCDMA__ 指定捕获/比较 DMA 请求源
  *          该参数可以是以下值之一：
  *            @arg TIM_CCDMAREQUEST_CC: 在捕获/比较事件时生成 CCx DMA 请求
  *            @arg TIM_CCDMAREQUEST_UPDATE: 在更新事件时生成 CCx DMA 请求
  * @retval 无
  */
#define __HAL_TIM_SELECT_CCDMAREQUEST(__HANDLE__, __CCDMA__)    \
  MODIFY_REG((__HANDLE__)->Instance->CR2, TIM_CR2_CCDS, (__CCDMA__))

/**
  * @}
  */
/* 导出的宏结束 ----------------------------------------------------*/

/* 私有常量 ---------------------------------------------------------*/
/** @defgroup TIM_Private_Constants TIM 私有常量
  * @{
  */
/* 仅当所有 CCx 和 CCxN 通道均已禁用时，定时器实例的计数器才会被禁用 */
#define TIM_CCER_CCxE_MASK  ((uint32_t)(TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E))
#define TIM_CCER_CCxNE_MASK ((uint32_t)(TIM_CCER_CC1NE | TIM_CCER_CC2NE | TIM_CCER_CC3NE))
/**
  * @}
  */
/* 私有常量结束 --------------------------------------------------*/

/* 私有宏 ------------------------------------------------------------*/
/** @defgroup TIM_Private_Macros TIM 私有宏
  * @{
  */
#define IS_TIM_CLEARINPUT_SOURCE(__MODE__)  (((__MODE__) == TIM_CLEARINPUTSOURCE_NONE)      || \
                                             ((__MODE__) == TIM_CLEARINPUTSOURCE_ETR))

#define IS_TIM_DMA_BASE(__BASE__) (((__BASE__) == TIM_DMABASE_CR1)   || \
                                   ((__BASE__) == TIM_DMABASE_CR2)   || \
                                   ((__BASE__) == TIM_DMABASE_SMCR)  || \
                                   ((__BASE__) == TIM_DMABASE_DIER)  || \
                                   ((__BASE__) == TIM_DMABASE_SR)    || \
                                   ((__BASE__) == TIM_DMABASE_EGR)   || \
                                   ((__BASE__) == TIM_DMABASE_CCMR1) || \
                                   ((__BASE__) == TIM_DMABASE_CCMR2) || \
                                   ((__BASE__) == TIM_DMABASE_CCER)  || \
                                   ((__BASE__) == TIM_DMABASE_CNT)   || \
                                   ((__BASE__) == TIM_DMABASE_PSC)   || \
                                   ((__BASE__) == TIM_DMABASE_ARR)   || \
                                   ((__BASE__) == TIM_DMABASE_RCR)   || \
                                   ((__BASE__) == TIM_DMABASE_CCR1)  || \
                                   ((__BASE__) == TIM_DMABASE_CCR2)  || \
                                   ((__BASE__) == TIM_DMABASE_CCR3)  || \
                                   ((__BASE__) == TIM_DMABASE_CCR4)  || \
                                   ((__BASE__) == TIM_DMABASE_BDTR))

#define IS_TIM_EVENT_SOURCE(__SOURCE__) ((((__SOURCE__) & 0xFFFFFF00U) == 0x00000000U) && ((__SOURCE__) != 0x00000000U))

#define IS_TIM_COUNTER_MODE(__MODE__)      (((__MODE__) == TIM_COUNTERMODE_UP)              || \
                                            ((__MODE__) == TIM_COUNTERMODE_DOWN)            || \
                                            ((__MODE__) == TIM_COUNTERMODE_CENTERALIGNED1)  || \
                                            ((__MODE__) == TIM_COUNTERMODE_CENTERALIGNED2)  || \
                                            ((__MODE__) == TIM_COUNTERMODE_CENTERALIGNED3))

#define IS_TIM_CLOCKDIVISION_DIV(__DIV__)  (((__DIV__) == TIM_CLOCKDIVISION_DIV1) || \
                                            ((__DIV__) == TIM_CLOCKDIVISION_DIV2) || \
                                            ((__DIV__) == TIM_CLOCKDIVISION_DIV4))

#define IS_TIM_AUTORELOAD_PRELOAD(PRELOAD) (((PRELOAD) == TIM_AUTORELOAD_PRELOAD_DISABLE) || \
                                            ((PRELOAD) == TIM_AUTORELOAD_PRELOAD_ENABLE))

#define IS_TIM_FAST_STATE(__STATE__)       (((__STATE__) == TIM_OCFAST_DISABLE) || \
                                            ((__STATE__) == TIM_OCFAST_ENABLE))

#define IS_TIM_OC_POLARITY(__POLARITY__)   (((__POLARITY__) == TIM_OCPOLARITY_HIGH) || \
                                            ((__POLARITY__) == TIM_OCPOLARITY_LOW))

#define IS_TIM_OCN_POLARITY(__POLARITY__)  (((__POLARITY__) == TIM_OCNPOLARITY_HIGH) || \
                                            ((__POLARITY__) == TIM_OCNPOLARITY_LOW))

#define IS_TIM_OCIDLE_STATE(__STATE__)     (((__STATE__) == TIM_OCIDLESTATE_SET) || \
                                            ((__STATE__) == TIM_OCIDLESTATE_RESET))

#define IS_TIM_OCNIDLE_STATE(__STATE__)    (((__STATE__) == TIM_OCNIDLESTATE_SET) || \
                                            ((__STATE__) == TIM_OCNIDLESTATE_RESET))

#define IS_TIM_ENCODERINPUT_POLARITY(__POLARITY__)   (((__POLARITY__) == TIM_ENCODERINPUTPOLARITY_RISING)   || \
                                                      ((__POLARITY__) == TIM_ENCODERINPUTPOLARITY_FALLING))

#define IS_TIM_IC_POLARITY(__POLARITY__)   (((__POLARITY__) == TIM_ICPOLARITY_RISING)   || \
                                            ((__POLARITY__) == TIM_ICPOLARITY_FALLING)  || \
                                            ((__POLARITY__) == TIM_ICPOLARITY_BOTHEDGE))

#define IS_TIM_IC_SELECTION(__SELECTION__) (((__SELECTION__) == TIM_ICSELECTION_DIRECTTI) || \
                                            ((__SELECTION__) == TIM_ICSELECTION_INDIRECTTI) || \
                                            ((__SELECTION__) == TIM_ICSELECTION_TRC))

#define IS_TIM_IC_PRESCALER(__PRESCALER__) (((__PRESCALER__) == TIM_ICPSC_DIV1) || \
                                            ((__PRESCALER__) == TIM_ICPSC_DIV2) || \
                                            ((__PRESCALER__) == TIM_ICPSC_DIV4) || \
                                            ((__PRESCALER__) == TIM_ICPSC_DIV8))

#define IS_TIM_OPM_MODE(__MODE__)          (((__MODE__) == TIM_OPMODE_SINGLE) || \
                                            ((__MODE__) == TIM_OPMODE_REPETITIVE))

#define IS_TIM_ENCODER_MODE(__MODE__)      (((__MODE__) == TIM_ENCODERMODE_TI1) || \
                                            ((__MODE__) == TIM_ENCODERMODE_TI2) || \
                                            ((__MODE__) == TIM_ENCODERMODE_TI12))

#define IS_TIM_DMA_SOURCE(__SOURCE__) ((((__SOURCE__) & 0xFFFF80FFU) == 0x00000000U) && ((__SOURCE__) != 0x00000000U))

#define IS_TIM_CHANNELS(__CHANNEL__)       (((__CHANNEL__) == TIM_CHANNEL_1) || \
                                            ((__CHANNEL__) == TIM_CHANNEL_2) || \
                                            ((__CHANNEL__) == TIM_CHANNEL_3) || \
                                            ((__CHANNEL__) == TIM_CHANNEL_4) || \
                                            ((__CHANNEL__) == TIM_CHANNEL_ALL))

#define IS_TIM_OPM_CHANNELS(__CHANNEL__)   (((__CHANNEL__) == TIM_CHANNEL_1) || \
                                            ((__CHANNEL__) == TIM_CHANNEL_2))

#define IS_TIM_PERIOD(__PERIOD__)      (((__PERIOD__) > 0U) && ((__PERIOD__) <= 0xFFFFU))

#define IS_TIM_COMPLEMENTARY_CHANNELS(__CHANNEL__) (((__CHANNEL__) == TIM_CHANNEL_1) || \
                                                    ((__CHANNEL__) == TIM_CHANNEL_2) || \
                                                    ((__CHANNEL__) == TIM_CHANNEL_3))

#define IS_TIM_CLOCKSOURCE(__CLOCK__) (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL) || \
                                       ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1) || \
                                       ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2) || \
                                       ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)    || \
                                       ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)      || \
                                       ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)      || \
                                       ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)     || \
                                       ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)     || \
                                       ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)     || \
                                       ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3))

#define IS_TIM_CLOCKPOLARITY(__POLARITY__) (((__POLARITY__) == TIM_CLOCKPOLARITY_INVERTED)    || \
                                            ((__POLARITY__) == TIM_CLOCKPOLARITY_NONINVERTED) || \
                                            ((__POLARITY__) == TIM_CLOCKPOLARITY_RISING)      || \
                                            ((__POLARITY__) == TIM_CLOCKPOLARITY_FALLING)     || \
                                            ((__POLARITY__) == TIM_CLOCKPOLARITY_BOTHEDGE))

#define IS_TIM_CLOCKPRESCALER(__PRESCALER__) (((__PRESCALER__) == TIM_CLOCKPRESCALER_DIV1) || \
                                              ((__PRESCALER__) == TIM_CLOCKPRESCALER_DIV2) || \
                                              ((__PRESCALER__) == TIM_CLOCKPRESCALER_DIV4) || \
                                              ((__PRESCALER__) == TIM_CLOCKPRESCALER_DIV8))

#define IS_TIM_CLOCKFILTER(__ICFILTER__)      ((__ICFILTER__) <= 0xFU)

#define IS_TIM_CLEARINPUT_POLARITY(__POLARITY__) (((__POLARITY__) == TIM_CLEARINPUTPOLARITY_INVERTED) || \
                                                  ((__POLARITY__) == TIM_CLEARINPUTPOLARITY_NONINVERTED))

#define IS_TIM_CLEARINPUT_PRESCALER(__PRESCALER__) (((__PRESCALER__) == TIM_CLEARINPUTPRESCALER_DIV1) || \
                                                    ((__PRESCALER__) == TIM_CLEARINPUTPRESCALER_DIV2) || \
                                                    ((__PRESCALER__) == TIM_CLEARINPUTPRESCALER_DIV4) || \
                                                    ((__PRESCALER__) == TIM_CLEARINPUTPRESCALER_DIV8))

#define IS_TIM_CLEARINPUT_FILTER(__ICFILTER__) ((__ICFILTER__) <= 0xFU)

#define IS_TIM_OSSR_STATE(__STATE__)       (((__STATE__) == TIM_OSSR_ENABLE) || \
                                            ((__STATE__) == TIM_OSSR_DISABLE))

#define IS_TIM_OSSI_STATE(__STATE__)       (((__STATE__) == TIM_OSSI_ENABLE) || \
                                            ((__STATE__) == TIM_OSSI_DISABLE))

#define IS_TIM_LOCK_LEVEL(__LEVEL__)       (((__LEVEL__) == TIM_LOCKLEVEL_OFF) || \
                                            ((__LEVEL__) == TIM_LOCKLEVEL_1)   || \
                                            ((__LEVEL__) == TIM_LOCKLEVEL_2)   || \
                                            ((__LEVEL__) == TIM_LOCKLEVEL_3))

#define IS_TIM_BREAK_FILTER(__BRKFILTER__) ((__BRKFILTER__) <= 0xFUL)

#define IS_TIM_BREAK_STATE(__STATE__)      (((__STATE__) == TIM_BREAK_ENABLE) || \
                                            ((__STATE__) == TIM_BREAK_DISABLE))

#define IS_TIM_BREAK_POLARITY(__POLARITY__) (((__POLARITY__) == TIM_BREAKPOLARITY_LOW) || \
                                             ((__POLARITY__) == TIM_BREAKPOLARITY_HIGH))

#define IS_TIM_AUTOMATIC_OUTPUT_STATE(__STATE__) (((__STATE__) == TIM_AUTOMATICOUTPUT_ENABLE) || \
                                                  ((__STATE__) == TIM_AUTOMATICOUTPUT_DISABLE))

#define IS_TIM_TRGO_SOURCE(__SOURCE__) (((__SOURCE__) == TIM_TRGO_RESET)  || \
                                        ((__SOURCE__) == TIM_TRGO_ENABLE) || \
                                        ((__SOURCE__) == TIM_TRGO_UPDATE) || \
                                        ((__SOURCE__) == TIM_TRGO_OC1)    || \
                                        ((__SOURCE__) == TIM_TRGO_OC1REF) || \
                                        ((__SOURCE__) == TIM_TRGO_OC2REF) || \
                                        ((__SOURCE__) == TIM_TRGO_OC3REF) || \
                                        ((__SOURCE__) == TIM_TRGO_OC4REF))

#define IS_TIM_MSM_STATE(__STATE__)      (((__STATE__) == TIM_MASTERSLAVEMODE_ENABLE) || \
                                          ((__STATE__) == TIM_MASTERSLAVEMODE_DISABLE))

#define IS_TIM_SLAVE_MODE(__MODE__) (((__MODE__) == TIM_SLAVEMODE_DISABLE)   || \
                                     ((__MODE__) == TIM_SLAVEMODE_RESET)     || \
                                     ((__MODE__) == TIM_SLAVEMODE_GATED)     || \
                                     ((__MODE__) == TIM_SLAVEMODE_TRIGGER)   || \
                                     ((__MODE__) == TIM_SLAVEMODE_EXTERNAL1))

#define IS_TIM_PWM_MODE(__MODE__) (((__MODE__) == TIM_OCMODE_PWM1)               || \
                                   ((__MODE__) == TIM_OCMODE_PWM2))

#define IS_TIM_OC_MODE(__MODE__)  (((__MODE__) == TIM_OCMODE_TIMING)             || \
                                   ((__MODE__) == TIM_OCMODE_ACTIVE)             || \
                                   ((__MODE__) == TIM_OCMODE_INACTIVE)           || \
                                   ((__MODE__) == TIM_OCMODE_TOGGLE)             || \
                                   ((__MODE__) == TIM_OCMODE_FORCED_ACTIVE)      || \
                                   ((__MODE__) == TIM_OCMODE_FORCED_INACTIVE))

#define IS_TIM_TRIGGER_SELECTION(__SELECTION__) (((__SELECTION__) == TIM_TS_ITR0)    || \
                                                 ((__SELECTION__) == TIM_TS_ITR1)    || \
                                                 ((__SELECTION__) == TIM_TS_ITR2)    || \
                                                 ((__SELECTION__) == TIM_TS_ITR3)    || \
                                                 ((__SELECTION__) == TIM_TS_TI1F_ED) || \
                                                 ((__SELECTION__) == TIM_TS_TI1FP1)  || \
                                                 ((__SELECTION__) == TIM_TS_TI2FP2)  || \
                                                 ((__SELECTION__) == TIM_TS_ETRF))

#define IS_TIM_INTERNAL_TRIGGEREVENT_SELECTION(__SELECTION__) (((__SELECTION__) == TIM_TS_ITR0) || \
                                                               ((__SELECTION__) == TIM_TS_ITR1) || \
                                                               ((__SELECTION__) == TIM_TS_ITR2) || \
                                                               ((__SELECTION__) == TIM_TS_ITR3) || \
                                                               ((__SELECTION__) == TIM_TS_NONE))

#define IS_TIM_TRIGGERPOLARITY(__POLARITY__)   (((__POLARITY__) == TIM_TRIGGERPOLARITY_INVERTED   ) || \
                                                ((__POLARITY__) == TIM_TRIGGERPOLARITY_NONINVERTED) || \
                                                ((__POLARITY__) == TIM_TRIGGERPOLARITY_RISING     ) || \
                                                ((__POLARITY__) == TIM_TRIGGERPOLARITY_FALLING    ) || \
                                                ((__POLARITY__) == TIM_TRIGGERPOLARITY_BOTHEDGE   ))

#define IS_TIM_TRIGGERPRESCALER(__PRESCALER__) (((__PRESCALER__) == TIM_TRIGGERPRESCALER_DIV1) || \
                                                ((__PRESCALER__) == TIM_TRIGGERPRESCALER_DIV2) || \
                                                ((__PRESCALER__) == TIM_TRIGGERPRESCALER_DIV4) || \
                                                ((__PRESCALER__) == TIM_TRIGGERPRESCALER_DIV8))

#define IS_TIM_TRIGGERFILTER(__ICFILTER__) ((__ICFILTER__) <= 0xFU)

#define IS_TIM_TI1SELECTION(__TI1SELECTION__)  (((__TI1SELECTION__) == TIM_TI1SELECTION_CH1) || \
                                                ((__TI1SELECTION__) == TIM_TI1SELECTION_XORCOMBINATION))

#define IS_TIM_DMA_LENGTH(__LENGTH__)      (((__LENGTH__) == TIM_DMABURSTLENGTH_1TRANSFER)   || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_2TRANSFERS)  || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_3TRANSFERS)  || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_4TRANSFERS)  || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_5TRANSFERS)  || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_6TRANSFERS)  || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_7TRANSFERS)  || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_8TRANSFERS)  || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_9TRANSFERS)  || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_10TRANSFERS) || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_11TRANSFERS) || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_12TRANSFERS) || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_13TRANSFERS) || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_14TRANSFERS) || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_15TRANSFERS) || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_16TRANSFERS) || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_17TRANSFERS) || \
                                            ((__LENGTH__) == TIM_DMABURSTLENGTH_18TRANSFERS))

#define IS_TIM_DMA_DATA_LENGTH(LENGTH) (((LENGTH) >= 0x1U) && ((LENGTH) < 0x10000U))

#define IS_TIM_IC_FILTER(__ICFILTER__)   ((__ICFILTER__) <= 0xFU)

#define IS_TIM_DEADTIME(__DEADTIME__)    ((__DEADTIME__) <= 0xFFU)

#define IS_TIM_SLAVEMODE_TRIGGER_ENABLED(__TRIGGER__) ((__TRIGGER__) == TIM_SLAVEMODE_TRIGGER)

#define TIM_SET_ICPRESCALERVALUE(__HANDLE__, __CHANNEL__, __ICPSC__) \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCMR1 |= (__ICPSC__)) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCMR1 |= ((__ICPSC__) << 8U)) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCMR2 |= (__ICPSC__)) :\
   ((__HANDLE__)->Instance->CCMR2 |= ((__ICPSC__) << 8U)))

#define TIM_RESET_ICPRESCALERVALUE(__HANDLE__, __CHANNEL__) \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCMR1 &= ~TIM_CCMR1_IC2PSC) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCMR2 &= ~TIM_CCMR2_IC3PSC) :\
   ((__HANDLE__)->Instance->CCMR2 &= ~TIM_CCMR2_IC4PSC))

#define TIM_SET_CAPTUREPOLARITY(__HANDLE__, __CHANNEL__, __POLARITY__) \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCER |= (__POLARITY__)) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCER |= ((__POLARITY__) << 4U)) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCER |= ((__POLARITY__) << 8U)) :\
   ((__HANDLE__)->Instance->CCER |= (((__POLARITY__) << 12U))))

#define TIM_RESET_CAPTUREPOLARITY(__HANDLE__, __CHANNEL__) \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP)) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCER &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP)) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCER &= ~(TIM_CCER_CC3P)) :\
   ((__HANDLE__)->Instance->CCER &= ~(TIM_CCER_CC4P)))

#define TIM_CHANNEL_STATE_GET(__HANDLE__, __CHANNEL__)\
  (((__CHANNEL__) == TIM_CHANNEL_1) ? (__HANDLE__)->ChannelState[0] :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? (__HANDLE__)->ChannelState[1] :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? (__HANDLE__)->ChannelState[2] :\
   (__HANDLE__)->ChannelState[3])

#define TIM_CHANNEL_STATE_SET(__HANDLE__, __CHANNEL__, __CHANNEL_STATE__) \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->ChannelState[0] = (__CHANNEL_STATE__)) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->ChannelState[1] = (__CHANNEL_STATE__)) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->ChannelState[2] = (__CHANNEL_STATE__)) :\
   ((__HANDLE__)->ChannelState[3] = (__CHANNEL_STATE__)))

#define TIM_CHANNEL_STATE_SET_ALL(__HANDLE__,  __CHANNEL_STATE__) do { \
                                                                       (__HANDLE__)->ChannelState[0]  = (__CHANNEL_STATE__);  \
                                                                       (__HANDLE__)->ChannelState[1]  = (__CHANNEL_STATE__);  \
                                                                       (__HANDLE__)->ChannelState[2]  = (__CHANNEL_STATE__);  \
                                                                       (__HANDLE__)->ChannelState[3]  = (__CHANNEL_STATE__);  \
                                                                     } while(0)

#define TIM_CHANNEL_N_STATE_GET(__HANDLE__, __CHANNEL__)\
  (((__CHANNEL__) == TIM_CHANNEL_1) ? (__HANDLE__)->ChannelNState[0] :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? (__HANDLE__)->ChannelNState[1] :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? (__HANDLE__)->ChannelNState[2] :\
   (__HANDLE__)->ChannelNState[3])

#define TIM_CHANNEL_N_STATE_SET(__HANDLE__, __CHANNEL__, __CHANNEL_STATE__) \
  (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->ChannelNState[0] = (__CHANNEL_STATE__)) :\
   ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->ChannelNState[1] = (__CHANNEL_STATE__)) :\
   ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->ChannelNState[2] = (__CHANNEL_STATE__)) :\
   ((__HANDLE__)->ChannelNState[3] = (__CHANNEL_STATE__)))

#define TIM_CHANNEL_N_STATE_SET_ALL(__HANDLE__,  __CHANNEL_STATE__) do { \
    (__HANDLE__)->ChannelNState[0] = (__CHANNEL_STATE__);  \
    (__HANDLE__)->ChannelNState[1] = (__CHANNEL_STATE__);  \
    (__HANDLE__)->ChannelNState[2] = (__CHANNEL_STATE__);  \
    (__HANDLE__)->ChannelNState[3] = (__CHANNEL_STATE__);  \
                                                                       } while(0)

/**
  * @}
  */
/* 私有宏定义结束 ------------------------------------------------------------*/

/* 包含 TIM HAL 扩展模块头文件 */
#include "stm32f1xx_hal_tim_ex.h"

/* 导出函数 ------------------------------------------------------------------*/
/** @addtogroup TIM_Exported_Functions TIM 导出函数
  * @{
  */

/** @addtogroup TIM_Exported_Functions_Group1 TIM 时基函数
  *  @brief   时基功能函数
  * @{
  */
/* 时基函数 *******************************************************************/
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim);
/* 阻塞模式：轮询 */
HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim);
/* 非阻塞模式：中断 */
HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim);
/* 非阻塞模式：DMA */
HAL_StatusTypeDef HAL_TIM_Base_Start_DMA(TIM_HandleTypeDef *htim, const uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIM_Base_Stop_DMA(TIM_HandleTypeDef *htim);
/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group2 TIM 输出比较函数
  *  @brief   TIM 输出比较功能函数
  * @{
  */
/* 定时器输出比较函数 *********************************************************/
HAL_StatusTypeDef HAL_TIM_OC_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_OC_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef *htim);
/* 阻塞模式：轮询 */
HAL_StatusTypeDef HAL_TIM_OC_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_OC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
/* 非阻塞模式：中断 */
HAL_StatusTypeDef HAL_TIM_OC_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_OC_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
/* 非阻塞模式：DMA */
HAL_StatusTypeDef HAL_TIM_OC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, const uint32_t *pData,
                                       uint16_t Length);
HAL_StatusTypeDef HAL_TIM_OC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);
/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group3 TIM PWM functions
  *  @brief   TIM PWM 相关函数
  * @{
  */
/* 定时器 PWM 相关函数 ********************************************************/
HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_PWM_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim);
/* 阻塞模式：轮询 */
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
/* 非阻塞模式：中断 */
HAL_StatusTypeDef HAL_TIM_PWM_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
/* 非阻塞模式：DMA */
HAL_StatusTypeDef HAL_TIM_PWM_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, const uint32_t *pData,
                                        uint16_t Length);
HAL_StatusTypeDef HAL_TIM_PWM_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);
/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group4 TIM Input Capture functions
  *  @brief   TIM 输入捕获相关函数
  * @{
  */
/* 定时器输入捕获相关函数 ******************************************************/
HAL_StatusTypeDef HAL_TIM_IC_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_IC_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim);
/* 阻塞模式：轮询 */
HAL_StatusTypeDef HAL_TIM_IC_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_IC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
/* 非阻塞模式：中断 */
HAL_StatusTypeDef HAL_TIM_IC_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_IC_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
/* 非阻塞模式：DMA */
HAL_StatusTypeDef HAL_TIM_IC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIM_IC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);
/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group5 TIM One Pulse functions
  *  @brief   TIM 单脉冲相关函数
  * @{
  */
/* 定时器单脉冲功能函数 **************************************************/
HAL_StatusTypeDef HAL_TIM_OnePulse_Init(TIM_HandleTypeDef *htim, uint32_t OnePulseMode);
HAL_StatusTypeDef HAL_TIM_OnePulse_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_OnePulse_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_OnePulse_MspDeInit(TIM_HandleTypeDef *htim);
/* 阻塞模式：轮询 */
HAL_StatusTypeDef HAL_TIM_OnePulse_Start(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
HAL_StatusTypeDef HAL_TIM_OnePulse_Stop(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
/* 非阻塞模式：中断 */
HAL_StatusTypeDef HAL_TIM_OnePulse_Start_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
HAL_StatusTypeDef HAL_TIM_OnePulse_Stop_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group6 TIM Encoder functions
  *  @brief   TIM 编码器功能函数
  * @{
  */
/* 定时器编码器功能函数 ****************************************************/
HAL_StatusTypeDef HAL_TIM_Encoder_Init(TIM_HandleTypeDef *htim, const TIM_Encoder_InitTypeDef *sConfig);
HAL_StatusTypeDef HAL_TIM_Encoder_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef *htim);
/* 阻塞模式：轮询 */
HAL_StatusTypeDef HAL_TIM_Encoder_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_Encoder_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
/* 非阻塞模式：中断 */
HAL_StatusTypeDef HAL_TIM_Encoder_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_Encoder_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
/* 非阻塞模式：DMA */
HAL_StatusTypeDef HAL_TIM_Encoder_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData1,
                                            uint32_t *pData2, uint16_t Length);
HAL_StatusTypeDef HAL_TIM_Encoder_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);
/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group7 TIM IRQ handler management
  *  @brief   IRQ 中断处理程序管理
  * @{
  */
/* 中断处理函数  ***********************************************/
void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim);
/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group8 TIM Peripheral Control functions
  *  @brief   外设控制功能函数
  * @{
  */
/* 控制功能函数  *********************************************************/
HAL_StatusTypeDef HAL_TIM_OC_ConfigChannel(TIM_HandleTypeDef *htim, const TIM_OC_InitTypeDef *sConfig,
                                           uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, const TIM_OC_InitTypeDef *sConfig,
                                            uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_IC_ConfigChannel(TIM_HandleTypeDef *htim, const TIM_IC_InitTypeDef *sConfig,
                                           uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_OnePulse_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OnePulse_InitTypeDef *sConfig,
                                                 uint32_t OutputChannel,  uint32_t InputChannel);
HAL_StatusTypeDef HAL_TIM_ConfigOCrefClear(TIM_HandleTypeDef *htim,
                                           const TIM_ClearInputConfigTypeDef *sClearInputConfig,
                                           uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_ConfigClockSource(TIM_HandleTypeDef *htim, const TIM_ClockConfigTypeDef *sClockSourceConfig);
HAL_StatusTypeDef HAL_TIM_ConfigTI1Input(TIM_HandleTypeDef *htim, uint32_t TI1_Selection);
HAL_StatusTypeDef HAL_TIM_SlaveConfigSynchro(TIM_HandleTypeDef *htim, const TIM_SlaveConfigTypeDef *sSlaveConfig);
HAL_StatusTypeDef HAL_TIM_SlaveConfigSynchro_IT(TIM_HandleTypeDef *htim, const TIM_SlaveConfigTypeDef *sSlaveConfig);
HAL_StatusTypeDef HAL_TIM_DMABurst_WriteStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress,
                                              uint32_t BurstRequestSrc, const uint32_t  *BurstBuffer,
                                              uint32_t  BurstLength);
HAL_StatusTypeDef HAL_TIM_DMABurst_MultiWriteStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress,
                                                   uint32_t BurstRequestSrc, const uint32_t *BurstBuffer,
                                                   uint32_t BurstLength,  uint32_t DataLength);
HAL_StatusTypeDef HAL_TIM_DMABurst_WriteStop(TIM_HandleTypeDef *htim, uint32_t BurstRequestSrc);
HAL_StatusTypeDef HAL_TIM_DMABurst_ReadStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress,
                                             uint32_t BurstRequestSrc, uint32_t  *BurstBuffer, uint32_t  BurstLength);
HAL_StatusTypeDef HAL_TIM_DMABurst_MultiReadStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress,
                                                  uint32_t BurstRequestSrc, uint32_t  *BurstBuffer,
                                                  uint32_t  BurstLength, uint32_t  DataLength);
HAL_StatusTypeDef HAL_TIM_DMABurst_ReadStop(TIM_HandleTypeDef *htim, uint32_t BurstRequestSrc);
HAL_StatusTypeDef HAL_TIM_GenerateEvent(TIM_HandleTypeDef *htim, uint32_t EventSource);
uint32_t HAL_TIM_ReadCapturedValue(const TIM_HandleTypeDef *htim, uint32_t Channel);
/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group9 TIM 回调函数
  *  @brief   TIM 回调函数
  * @{
  */
/* 非阻塞模式（中断和 DMA）下的回调函数 *************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_CaptureHalfCpltCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_TriggerHalfCpltCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim);

/* 回调函数注册/注销函数  ***********************************/
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
HAL_StatusTypeDef HAL_TIM_RegisterCallback(TIM_HandleTypeDef *htim, HAL_TIM_CallbackIDTypeDef CallbackID,
                                           pTIM_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_TIM_UnRegisterCallback(TIM_HandleTypeDef *htim, HAL_TIM_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group10 TIM 外设状态函数
  *  @brief  外设状态函数
  * @{
  */
/* 外设状态函数  ************************************************/
HAL_TIM_StateTypeDef HAL_TIM_Base_GetState(const TIM_HandleTypeDef *htim);
HAL_TIM_StateTypeDef HAL_TIM_OC_GetState(const TIM_HandleTypeDef *htim);
HAL_TIM_StateTypeDef HAL_TIM_PWM_GetState(const TIM_HandleTypeDef *htim);
HAL_TIM_StateTypeDef HAL_TIM_IC_GetState(const TIM_HandleTypeDef *htim);
HAL_TIM_StateTypeDef HAL_TIM_OnePulse_GetState(const TIM_HandleTypeDef *htim);
HAL_TIM_StateTypeDef HAL_TIM_Encoder_GetState(const TIM_HandleTypeDef *htim);

/* 外设通道状态函数  ************************************************/
HAL_TIM_ActiveChannel HAL_TIM_GetActiveChannel(const TIM_HandleTypeDef *htim);
HAL_TIM_ChannelStateTypeDef HAL_TIM_GetChannelState(const TIM_HandleTypeDef *htim,  uint32_t Channel);
HAL_TIM_DMABurstStateTypeDef HAL_TIM_DMABurstState(const TIM_HandleTypeDef *htim);
/**
  * @}
  */

/**
  * @}
  */
/* 导出函数结束 -------------------------------------------------*/

/* 私有函数----------------------------------------------------------*/
/** @defgroup TIM_Private_Functions TIM 私有函数
  * @{
  */
void TIM_Base_SetConfig(TIM_TypeDef *TIMx, const TIM_Base_InitTypeDef *Structure);
void TIM_TI1_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t TIM_ICFilter);
void TIM_OC2_SetConfig(TIM_TypeDef *TIMx, const TIM_OC_InitTypeDef *OC_Config);
void TIM_ETR_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ExtTRGPrescaler,
                       uint32_t TIM_ExtTRGPolarity, uint32_t ExtTRGFilter);

void TIM_DMADelayPulseHalfCplt(DMA_HandleTypeDef *hdma);
void TIM_DMAError(DMA_HandleTypeDef *hdma);
void TIM_DMACaptureCplt(DMA_HandleTypeDef *hdma);
void TIM_DMACaptureHalfCplt(DMA_HandleTypeDef *hdma);
void TIM_CCxChannelCmd(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t ChannelState);

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
void TIM_ResetCallback(TIM_HandleTypeDef *htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

/**
  * @}
  */
/* 私有函数结束 --------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32F1xx_HAL_TIM_H */

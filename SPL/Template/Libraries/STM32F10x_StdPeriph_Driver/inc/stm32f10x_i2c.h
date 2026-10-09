/**
  ******************************************************************************
  * @file    stm32f10x_i2c.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 I2C 固件库的所有函数原型。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供指导之用，旨在为客户提供有关其产品的编码信息，以节省他们的时间。
  * 因此，对于因本固件的内容和/或客户将此处包含的编码信息
  * 与其产品结合使用而提出的任何索赔所造成的任何直接、间接或后果性损害，
  * STMicroelectronics 概不承担任何责任。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 定义以下宏，以防止本头文件被递归包含 -------------------------------------*/
#ifndef __STM32F10x_I2C_H
#define __STM32F10x_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup I2C   I2C 外设
  * @{
  */

/** @defgroup I2C_Exported_Types   I2C 导出类型
  * @{
  */

/**
  * @brief  I2C 初始化结构体定义
  */

typedef struct
{
  uint32_t I2C_ClockSpeed;          /*!< 指定时钟频率。
                                         该参数必须设置为低于 400kHz 的值 */

  uint16_t I2C_Mode;                /*!< 指定 I2C 模式。
                                         该参数可取 @ref I2C_mode 的值 */

  uint16_t I2C_DutyCycle;           /*!< 指定 I2C 快速模式占空比。
                                         该参数可取 @ref I2C_duty_cycle_in_fast_mode 的值 */

  uint16_t I2C_OwnAddress1;         /*!< 指定第一个设备自身地址。
                                         该参数可以是 7 位或 10 位地址。 */

  uint16_t I2C_Ack;                 /*!< 使能或关闭应答。
                                         该参数可取 @ref I2C_acknowledgement 的值 */

  uint16_t I2C_AcknowledgedAddress; /*!< 指定应答 7 位还是 10 位地址。
                                         该参数可取 @ref I2C_acknowledged_address 的值 */
}I2C_InitTypeDef;

/**
  * @}
  */ 


/** @defgroup I2C_Exported_Constants   I2C 导出常量
  * @{
  */

#define IS_I2C_ALL_PERIPH(PERIPH) (((PERIPH) == I2C1) || \
                                   ((PERIPH) == I2C2))
/** @defgroup I2C_mode   I2C 模式
  * @{
  */

#define I2C_Mode_I2C                    ((uint16_t)0x0000)
#define I2C_Mode_SMBusDevice            ((uint16_t)0x0002)  
#define I2C_Mode_SMBusHost              ((uint16_t)0x000A)
#define IS_I2C_MODE(MODE) (((MODE) == I2C_Mode_I2C) || \
                           ((MODE) == I2C_Mode_SMBusDevice) || \
                           ((MODE) == I2C_Mode_SMBusHost))
/**
  * @}
  */

/** @defgroup I2C_duty_cycle_in_fast_mode   快速模式下的 I2C 占空比
  * @{
  */

#define I2C_DutyCycle_16_9              ((uint16_t)0x4000) /*!< I2C 快速模式 Tlow/Thigh = 16/9 */
#define I2C_DutyCycle_2                 ((uint16_t)0xBFFF) /*!< I2C 快速模式 Tlow/Thigh = 2 */
#define IS_I2C_DUTY_CYCLE(CYCLE) (((CYCLE) == I2C_DutyCycle_16_9) || \
                                  ((CYCLE) == I2C_DutyCycle_2))
/**
  * @}
  */ 

/** @defgroup I2C_acknowledgement   I2C 应答
  * @{
  */

#define I2C_Ack_Enable                  ((uint16_t)0x0400)
#define I2C_Ack_Disable                 ((uint16_t)0x0000)
#define IS_I2C_ACK_STATE(STATE) (((STATE) == I2C_Ack_Enable) || \
                                 ((STATE) == I2C_Ack_Disable))
/**
  * @}
  */

/** @defgroup I2C_transfer_direction   I2C 传输方向
  * @{
  */

#define  I2C_Direction_Transmitter      ((uint8_t)0x00)
#define  I2C_Direction_Receiver         ((uint8_t)0x01)
#define IS_I2C_DIRECTION(DIRECTION) (((DIRECTION) == I2C_Direction_Transmitter) || \
                                     ((DIRECTION) == I2C_Direction_Receiver))
/**
  * @}
  */

/** @defgroup I2C_acknowledged_address   I2C 应答地址
  * @{
  */

#define I2C_AcknowledgedAddress_7bit    ((uint16_t)0x4000)
#define I2C_AcknowledgedAddress_10bit   ((uint16_t)0xC000)
#define IS_I2C_ACKNOWLEDGE_ADDRESS(ADDRESS) (((ADDRESS) == I2C_AcknowledgedAddress_7bit) || \
                                             ((ADDRESS) == I2C_AcknowledgedAddress_10bit))
/**
  * @}
  */ 

/** @defgroup I2C_registers   I2C 寄存器
  * @{
  */

#define I2C_Register_CR1                ((uint8_t)0x00)
#define I2C_Register_CR2                ((uint8_t)0x04)
#define I2C_Register_OAR1               ((uint8_t)0x08)
#define I2C_Register_OAR2               ((uint8_t)0x0C)
#define I2C_Register_DR                 ((uint8_t)0x10)
#define I2C_Register_SR1                ((uint8_t)0x14)
#define I2C_Register_SR2                ((uint8_t)0x18)
#define I2C_Register_CCR                ((uint8_t)0x1C)
#define I2C_Register_TRISE              ((uint8_t)0x20)
#define IS_I2C_REGISTER(REGISTER) (((REGISTER) == I2C_Register_CR1) || \
                                   ((REGISTER) == I2C_Register_CR2) || \
                                   ((REGISTER) == I2C_Register_OAR1) || \
                                   ((REGISTER) == I2C_Register_OAR2) || \
                                   ((REGISTER) == I2C_Register_DR) || \
                                   ((REGISTER) == I2C_Register_SR1) || \
                                   ((REGISTER) == I2C_Register_SR2) || \
                                   ((REGISTER) == I2C_Register_CCR) || \
                                   ((REGISTER) == I2C_Register_TRISE))
/**
  * @}
  */

/** @defgroup I2C_SMBus_alert_pin_level   I2C SMBus 警报引脚电平
  * @{
  */

#define I2C_SMBusAlert_Low              ((uint16_t)0x2000)
#define I2C_SMBusAlert_High             ((uint16_t)0xDFFF)
#define IS_I2C_SMBUS_ALERT(ALERT) (((ALERT) == I2C_SMBusAlert_Low) || \
                                   ((ALERT) == I2C_SMBusAlert_High))
/**
  * @}
  */

/** @defgroup I2C_PEC_position   I2C PEC 位置
  * @{
  */

#define I2C_PECPosition_Next            ((uint16_t)0x0800)
#define I2C_PECPosition_Current         ((uint16_t)0xF7FF)
#define IS_I2C_PEC_POSITION(POSITION) (((POSITION) == I2C_PECPosition_Next) || \
                                       ((POSITION) == I2C_PECPosition_Current))
/**
  * @}
  */ 

/** @defgroup I2C_NCAK_position   I2C NACK 位置
  * @{
  */

#define I2C_NACKPosition_Next           ((uint16_t)0x0800)
#define I2C_NACKPosition_Current        ((uint16_t)0xF7FF)
#define IS_I2C_NACK_POSITION(POSITION)  (((POSITION) == I2C_NACKPosition_Next) || \
                                         ((POSITION) == I2C_NACKPosition_Current))
/**
  * @}
  */ 

/** @defgroup I2C_interrupts_definition   I2C 中断定义
  * @{
  */

#define I2C_IT_BUF                      ((uint16_t)0x0400)
#define I2C_IT_EVT                      ((uint16_t)0x0200)
#define I2C_IT_ERR                      ((uint16_t)0x0100)
#define IS_I2C_CONFIG_IT(IT) ((((IT) & (uint16_t)0xF8FF) == 0x00) && ((IT) != 0x00))
/**
  * @}
  */ 

/** @defgroup I2C_interrupts_definition   I2C 中断定义
  * @{
  */

#define I2C_IT_SMBALERT                 ((uint32_t)0x01008000)
#define I2C_IT_TIMEOUT                  ((uint32_t)0x01004000)
#define I2C_IT_PECERR                   ((uint32_t)0x01001000)
#define I2C_IT_OVR                      ((uint32_t)0x01000800)
#define I2C_IT_AF                       ((uint32_t)0x01000400)
#define I2C_IT_ARLO                     ((uint32_t)0x01000200)
#define I2C_IT_BERR                     ((uint32_t)0x01000100)
#define I2C_IT_TXE                      ((uint32_t)0x06000080)
#define I2C_IT_RXNE                     ((uint32_t)0x06000040)
#define I2C_IT_STOPF                    ((uint32_t)0x02000010)
#define I2C_IT_ADD10                    ((uint32_t)0x02000008)
#define I2C_IT_BTF                      ((uint32_t)0x02000004)
#define I2C_IT_ADDR                     ((uint32_t)0x02000002)
#define I2C_IT_SB                       ((uint32_t)0x02000001)

#define IS_I2C_CLEAR_IT(IT) ((((IT) & (uint16_t)0x20FF) == 0x00) && ((IT) != (uint16_t)0x00))

#define IS_I2C_GET_IT(IT) (((IT) == I2C_IT_SMBALERT) || ((IT) == I2C_IT_TIMEOUT) || \
                           ((IT) == I2C_IT_PECERR) || ((IT) == I2C_IT_OVR) || \
                           ((IT) == I2C_IT_AF) || ((IT) == I2C_IT_ARLO) || \
                           ((IT) == I2C_IT_BERR) || ((IT) == I2C_IT_TXE) || \
                           ((IT) == I2C_IT_RXNE) || ((IT) == I2C_IT_STOPF) || \
                           ((IT) == I2C_IT_ADD10) || ((IT) == I2C_IT_BTF) || \
                           ((IT) == I2C_IT_ADDR) || ((IT) == I2C_IT_SB))
/**
  * @}
  */

/** @defgroup I2C_flags_definition   I2C 标志定义
  * @{
  */

/**
  * @brief  SR2 寄存器标志
  */

#define I2C_FLAG_DUALF                  ((uint32_t)0x00800000)
#define I2C_FLAG_SMBHOST                ((uint32_t)0x00400000)
#define I2C_FLAG_SMBDEFAULT             ((uint32_t)0x00200000)
#define I2C_FLAG_GENCALL                ((uint32_t)0x00100000)
#define I2C_FLAG_TRA                    ((uint32_t)0x00040000)
#define I2C_FLAG_BUSY                   ((uint32_t)0x00020000)
#define I2C_FLAG_MSL                    ((uint32_t)0x00010000)

/**
  * @brief  SR1 寄存器标志
  */

#define I2C_FLAG_SMBALERT               ((uint32_t)0x10008000)
#define I2C_FLAG_TIMEOUT                ((uint32_t)0x10004000)
#define I2C_FLAG_PECERR                 ((uint32_t)0x10001000)
#define I2C_FLAG_OVR                    ((uint32_t)0x10000800)
#define I2C_FLAG_AF                     ((uint32_t)0x10000400)
#define I2C_FLAG_ARLO                   ((uint32_t)0x10000200)
#define I2C_FLAG_BERR                   ((uint32_t)0x10000100)
#define I2C_FLAG_TXE                    ((uint32_t)0x10000080)
#define I2C_FLAG_RXNE                   ((uint32_t)0x10000040)
#define I2C_FLAG_STOPF                  ((uint32_t)0x10000010)
#define I2C_FLAG_ADD10                  ((uint32_t)0x10000008)
#define I2C_FLAG_BTF                    ((uint32_t)0x10000004)
#define I2C_FLAG_ADDR                   ((uint32_t)0x10000002)
#define I2C_FLAG_SB                     ((uint32_t)0x10000001)

#define IS_I2C_CLEAR_FLAG(FLAG) ((((FLAG) & (uint16_t)0x20FF) == 0x00) && ((FLAG) != (uint16_t)0x00))

#define IS_I2C_GET_FLAG(FLAG) (((FLAG) == I2C_FLAG_DUALF) || ((FLAG) == I2C_FLAG_SMBHOST) || \
                               ((FLAG) == I2C_FLAG_SMBDEFAULT) || ((FLAG) == I2C_FLAG_GENCALL) || \
                               ((FLAG) == I2C_FLAG_TRA) || ((FLAG) == I2C_FLAG_BUSY) || \
                               ((FLAG) == I2C_FLAG_MSL) || ((FLAG) == I2C_FLAG_SMBALERT) || \
                               ((FLAG) == I2C_FLAG_TIMEOUT) || ((FLAG) == I2C_FLAG_PECERR) || \
                               ((FLAG) == I2C_FLAG_OVR) || ((FLAG) == I2C_FLAG_AF) || \
                               ((FLAG) == I2C_FLAG_ARLO) || ((FLAG) == I2C_FLAG_BERR) || \
                               ((FLAG) == I2C_FLAG_TXE) || ((FLAG) == I2C_FLAG_RXNE) || \
                               ((FLAG) == I2C_FLAG_STOPF) || ((FLAG) == I2C_FLAG_ADD10) || \
                               ((FLAG) == I2C_FLAG_BTF) || ((FLAG) == I2C_FLAG_ADDR) || \
                               ((FLAG) == I2C_FLAG_SB))
/**
  * @}
  */

/** @defgroup I2C_Events   I2C 事件
  * @{
  */

/*========================================

                     I2C 主设备事件（事件按通信顺序分组）
                                                        ==========================================*/
/**
  * @brief  通信开始
  *
  * 在发送 START 条件（I2C_GenerateSTART() 函数）后，主设备必须等待此事件。
  * 它意味着 Start 条件已在 I2C 总线上正确释放
  * （总线空闲，没有其它设备正在通信）。
  *
  */
/* --EV5 */
#define  I2C_EVENT_MASTER_MODE_SELECT                      ((uint32_t)0x00030001)  /* BUSY、MSL 和 SB 标志 */

/**
  * @brief  地址应答
  *
  * 在检查 EV5（Start 条件已在总线上正确释放）之后，主设备发送与之通信的
  * 从设备地址（I2C_Send7bitAddress() 函数，它同时确定通信方向：
  * 主发送器或主接收器）。随后主设备必须等待从设备应答其地址。
  * 如果总线上发送了应答，则将置位以下事件之一：
  *
  *  1) 主接收器（7 位寻址）情况下：置位
  *     I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED 事件。
  *
  *  2) 主发送器（7 位寻址）情况下：置位
  *     I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED
  *
  *  3) 10 位寻址模式情况下，主设备（在产生 START 并检查 EV5 之后）必须发送
  *  10 位寻址模式的头（I2C_SendData() 函数）。然后主设备应等待 EV9。
  *  它意味着 10 位寻址头已在总线上正确发送。随后主设备应使用
  *  I2C_Send7bitAddress() 函数发送 10 位地址的第二部分（LSB）。然后主设备
  *  应等待事件 EV6。
  *
  */

/* --EV6 */
#define  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED        ((uint32_t)0x00070082)  /* BUSY、MSL、ADDR、TXE 和 TRA 标志 */
#define  I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED           ((uint32_t)0x00030002)  /* BUSY、MSL 和 ADDR 标志 */
/* --EV9 */
#define  I2C_EVENT_MASTER_MODE_ADDRESS10                   ((uint32_t)0x00030008)  /* BUSY、MSL 和 ADD10 标志 */

/**
  * @brief 通信事件
  *
  * 如果通信已建立（START 条件已产生且从设备地址已被应答），则主设备必须
  * 检查以下事件之一以进行通信过程：
  *
  * 1) 主接收器模式：主设备必须等待事件 EV7，然后读取从设备发来的数据
  *    （I2C_ReceiveData() 函数）。
  *
  * 2) 主发送器模式：主设备必须发送数据（I2C_SendData() 函数），
  *    然后等待事件 EV8 或 EV8_2。
  *    这两个事件类似：
  *     - EV8 表示数据已写入数据寄存器并正在移出。
  *     - EV8_2 表示数据已物理移出并输出到总线上。
  *     大多数情况下，使用 EV8 对应用来说已经足够。
  *     使用 EV8_2 会使通信变慢，但能确保更可靠的测试。
  *     对于最后一次数据传输（在产生 Stop 条件之前）的测试，EV8_2 也比 EV8
  *     更合适。
  *
  *  @note 如果用户软件不能保证在当前字节传输结束前处理此事件 EV7，
  *  则用户可以同时检查 EV7 和 BTF 标志
  *  （即 (I2C_EVENT_MASTER_BYTE_RECEIVED | I2C_FLAG_BTF)）。
  *  在这种情况下通信可能会变慢。
  *
  */

/* 主接收器模式 -----------------------------*/ 
/* --EV7 */
#define  I2C_EVENT_MASTER_BYTE_RECEIVED                    ((uint32_t)0x00030040)  /* BUSY、MSL 和 RXNE 标志 */

/* 主发送器模式 --------------------------*/
/* --EV8 */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTING                 ((uint32_t)0x00070080) /* TRA、BUSY、MSL、TXE 标志 */
/* --EV8_2 */
#define  I2C_EVENT_MASTER_BYTE_TRANSMITTED                 ((uint32_t)0x00070084)  /* TRA、BUSY、MSL、TXE 和 BTF 标志 */


/*========================================

                     I2C 从设备事件（事件按通信顺序分组）
                                                        ==========================================*/

/**
  * @brief  通信开始事件
  *
  * 在通信开始时等待这些事件之一。它意味着 I2C 外设检测到总线上由主设备
  * 产生的 Start 条件以及随后的外设地址。该外设会在总线上产生 ACK 条件
  * （如果通过 I2C_AcknowledgeConfig() 函数使能了应答功能），
  * 并且置位上面列出的事件：
  *
  * 1) 正常情况下（从设备只管理一个地址），当主设备发送的地址与外设自身
  *   地址（由 I2C_OwnAddress1 字段配置）匹配时，置位
  *   I2C_EVENT_SLAVE_XXX_ADDRESS_MATCHED 事件
  *   （其中 XXX 可以是 TRANSMITTER 或 RECEIVER）。
  *
  * 2) 当主设备发送的地址与外设的第二个地址（由 I2C_OwnAddress2Config()
  *   函数配置并由 I2C_DualAddressCmd() 函数使能）匹配时，置位事件
  *   I2C_EVENT_SLAVE_XXX_SECONDADDRESS_MATCHED
  *   （其中 XXX 可以是 TRANSMITTER 或 RECEIVER）。
  *
  * 3) 当主设备发送的地址为通用呼叫（地址 0x00）且外设使能了通用呼叫
  *   （使用 I2C_GeneralCallCmd() 函数）时，置位以下事件
  *   I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED。
  *
  */

/* --EV1  （以下所有事件都是 EV1 的变体） */   
/* 1) 从设备只管理单个地址的情况 */
#define  I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED          ((uint32_t)0x00020002) /* BUSY 和 ADDR 标志 */
#define  I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED       ((uint32_t)0x00060082) /* TRA、BUSY、TXE 和 ADDR 标志 */

/* 2) 从设备管理双地址的情况 */
#define  I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED    ((uint32_t)0x00820000)  /* DUALF 和 BUSY 标志 */
#define  I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED ((uint32_t)0x00860080)  /* DUALF、TRA、BUSY 和 TXE 标志 */

/* 3) 从设备使能通用呼叫的情况 */
#define  I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED        ((uint32_t)0x00120000)  /* GENCALL 和 BUSY 标志 */

/**
  * @brief  通信事件
  *
  * 当已检查过 EV1 后，等待以下事件之一：
  *
  * - 从接收器模式：
  *     - EV2：当应用期望接收到一个数据字节时。
  *     - EV4：当应用期望通信结束时：主设备发送停止条件，数据传输停止。
  *
  * - 从发送器模式：
  *    - EV3：当从设备已发送一个字节且应用期望该字节传输结束时。
  *      事件 I2C_EVENT_SLAVE_BYTE_TRANSMITTED 和
  *      I2C_EVENT_SLAVE_BYTE_TRANSMITTING 类似。当用户软件不能保证
  *      在当前字节传输结束前处理 EV3 时，可以选用第二个事件。
  *    - EV3_2：当主设备发送 NACK 以告知从设备数据传输应结束时
  *      （在发送 STOP 条件之前）。在这种情况下，从设备必须停止发送
  *      数据字节并等待总线上的 Stop 条件。
  *
  *  @note 如果用户软件不能保证在当前字节传输结束前处理事件 EV2，
  *  则用户可以同时检查 EV2 和 BTF 标志
  *  （即 (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_BTF)）。
  * 在这种情况下通信可能会变慢。
  *
  */

/* 从接收器模式 --------------------------*/ 
/* --EV2 */
#define  I2C_EVENT_SLAVE_BYTE_RECEIVED                     ((uint32_t)0x00020040)  /* BUSY 和 RXNE 标志 */
/* --EV4  */
#define  I2C_EVENT_SLAVE_STOP_DETECTED                     ((uint32_t)0x00000010)  /* STOPF 标志 */

/* 从发送器模式 -----------------------*/
/* --EV3 */
#define  I2C_EVENT_SLAVE_BYTE_TRANSMITTED                  ((uint32_t)0x00060084)  /* TRA、BUSY、TXE 和 BTF 标志 */
#define  I2C_EVENT_SLAVE_BYTE_TRANSMITTING                 ((uint32_t)0x00060080)  /* TRA、BUSY 和 TXE 标志 */
/* --EV3_2 */
#define  I2C_EVENT_SLAVE_ACK_FAILURE                       ((uint32_t)0x00000400)  /* AF 标志 */

/*===========================      事件描述结束           ==========================================*/

#define IS_I2C_EVENT(EVENT) (((EVENT) == I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_BYTE_RECEIVED) || \
                             ((EVENT) == (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_DUALF)) || \
                             ((EVENT) == (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_GENCALL)) || \
                             ((EVENT) == I2C_EVENT_SLAVE_BYTE_TRANSMITTED) || \
                             ((EVENT) == (I2C_EVENT_SLAVE_BYTE_TRANSMITTED | I2C_FLAG_DUALF)) || \
                             ((EVENT) == (I2C_EVENT_SLAVE_BYTE_TRANSMITTED | I2C_FLAG_GENCALL)) || \
                             ((EVENT) == I2C_EVENT_SLAVE_STOP_DETECTED) || \
                             ((EVENT) == I2C_EVENT_MASTER_MODE_SELECT) || \
                             ((EVENT) == I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) || \
                             ((EVENT) == I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) || \
                             ((EVENT) == I2C_EVENT_MASTER_BYTE_RECEIVED) || \
                             ((EVENT) == I2C_EVENT_MASTER_BYTE_TRANSMITTED) || \
                             ((EVENT) == I2C_EVENT_MASTER_BYTE_TRANSMITTING) || \
                             ((EVENT) == I2C_EVENT_MASTER_MODE_ADDRESS10) || \
                             ((EVENT) == I2C_EVENT_SLAVE_ACK_FAILURE))
/**
  * @}
  */

/** @defgroup I2C_own_address1   I2C 自身地址 1
  * @{
  */

#define IS_I2C_OWN_ADDRESS1(ADDRESS1) ((ADDRESS1) <= 0x3FF)
/**
  * @}
  */

/** @defgroup I2C_clock_speed   I2C 时钟速度
  * @{
  */

#define IS_I2C_CLOCK_SPEED(SPEED) (((SPEED) >= 0x1) && ((SPEED) <= 400000))
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup I2C_Exported_Macros   I2C 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup I2C_Exported_Functions   I2C 导出函数
  * @{
  */

void I2C_DeInit(I2C_TypeDef* I2Cx);
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct);
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct);
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_DMACmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_DMALastTransferCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_OwnAddress2Config(I2C_TypeDef* I2Cx, uint8_t Address);
void I2C_DualAddressCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_GeneralCallCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_ITConfig(I2C_TypeDef* I2Cx, uint16_t I2C_IT, FunctionalState NewState);
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data);
uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx);
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction);
uint16_t I2C_ReadRegister(I2C_TypeDef* I2Cx, uint8_t I2C_Register);
void I2C_SoftwareResetCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_NACKPositionConfig(I2C_TypeDef* I2Cx, uint16_t I2C_NACKPosition);
void I2C_SMBusAlertConfig(I2C_TypeDef* I2Cx, uint16_t I2C_SMBusAlert);
void I2C_TransmitPEC(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_PECPositionConfig(I2C_TypeDef* I2Cx, uint16_t I2C_PECPosition);
void I2C_CalculatePEC(I2C_TypeDef* I2Cx, FunctionalState NewState);
uint8_t I2C_GetPEC(I2C_TypeDef* I2Cx);
void I2C_ARPCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_StretchClockCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_FastModeDutyCycleConfig(I2C_TypeDef* I2Cx, uint16_t I2C_DutyCycle);

/**
 * @brief
 ****************************************************************************************
 *
 *                         I2C 状态监控函数
 *
 ****************************************************************************************
 * 本 I2C 驱动根据应用需求和约束，提供三种不同的 I2C 状态监控方式：
 *
 *
 * 1) 基本状态监控：
 *    使用 I2C_CheckEvent() 函数：
 *    它将状态寄存器（SR1 和 SR2）的内容与给定事件
 *    （可以是一个或多个标志的组合）进行比较。
 *    如果当前状态包含给定标志则返回 SUCCESS，
 *    如果当前状态缺少一个或多个标志则返回 ERROR。
 *    - 适用场合：
 *      - 由于事件在产品参考手册（RM0008）中有完整描述，本函数适用于大多数
 *        应用以及启动阶段的活动。
 *      - 也适用于需要定义自己事件的用户。
 *    - 局限性：
 *      - 如果发生错误（即除被监控的标志外还有错误标志被置位），
 *        即使通信保持挂起或实际状态已损坏，I2C_CheckEvent() 函数也可能
 *        返回 SUCCESS。
 *        在这种情况下，建议使用错误中断来监控错误事件，
 *        并在中断 IRQ 处理函数中处理它们。
 *
 *        @note
 *        对于错误管理，建议使用以下函数：
 *          - I2C_ITConfig() 用于配置并使能错误中断（I2C_IT_ERR）。
 *          - I2Cx_ER_IRQHandler() 在发生错误中断时被调用。
 *            其中 x 是外设实例（I2C1、I2C2 ...）
 *          - I2C_GetFlagStatus() 或 I2C_GetITStatus() 在 I2Cx_ER_IRQHandler()
 *            中调用，以确定发生了哪个错误。
 *          - I2C_ClearFlag() 或 I2C_ClearITPendingBit() 和/或
 *            I2C_SoftwareResetCmd() 和/或 I2C_GenerateStop() 用于清除错误
 *            标志和错误源，并恢复到正确的通信状态。
 *
 *
 *  2) 高级状态监控：
 *     使用 I2C_GetLastEvent() 函数，它在一个字（uint32_t）中返回两个状态
 *     寄存器的镜像（状态寄存器 2 的值左移 16 位后与状态寄存器 1 拼接）。
 *     - 适用场合：
 *       - 本函数适用于上述相同的应用，但它可以克服 I2C_GetFlagStatus()
 *         函数的局限性（见下文）。
 *         返回值可以与库中已定义的事件（stm32f10x_i2c.h）
 *         或用户自定义的值进行比较。
 *       - 本函数适用于同时监控多个标志的场合。
 *       - 与 I2C_CheckEvent() 函数相反，本函数允许用户选择何时接受一个
 *         事件（当所有事件标志都置位且没有其它标志置位时，
 *         或者像 I2C_CheckEvent() 函数那样只要求所需标志置位时）。
 *     - 局限性：
 *       - 用户可能需要定义自己的事件。
 *       - 如果用户决定只检查常规通信标志（而忽略错误标志），
 *         那么关于错误管理的同样说明也适用于本函数。
 *
 *
 *  3) 基于标志的状态监控：
 *     使用 I2C_GetFlagStatus() 函数，它仅返回单个标志的状态
 *     （即 I2C_FLAG_RXNE ...）。
 *     - 适用场合：
 *        - 本函数可用于特定应用或调试阶段。
 *        - 适用于只需检查一个标志的场合（大多数 I2C 事件需要通过多个
 *          标志监控）。
 *     - 局限性：
 *        - 调用本函数时会访问状态寄存器。某些标志在访问状态寄存器时会
 *          被清除。因此检查一个标志的状态可能会清除其它标志。
 *        - 为了监控单个事件，可能需要调用本函数两次或更多次。
 *
 *  关于事件的详细描述，请参阅 stm32f10x_i2c.h 文件中的
 *  I2C_Events 一节。
 *
 */

/**
 *
 *  1) 基本状态监控
 *******************************************************************************
 */
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
/**
 *
 *  2) 高级状态监控
 *******************************************************************************
 */
uint32_t I2C_GetLastEvent(I2C_TypeDef* I2Cx);
/**
 *
 *  3) 基于标志的状态监控
 *******************************************************************************
 */
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG);
/**
 *
 *******************************************************************************
 */

void I2C_ClearFlag(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG);
ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cx, uint32_t I2C_IT);
void I2C_ClearITPendingBit(I2C_TypeDef* I2Cx, uint32_t I2C_IT);

#ifdef __cplusplus
}
#endif

#endif /*__STM32F10x_I2C_H */
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

/**
  ******************************************************************************
  * @file    stm32f10x_sdio.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 SDIO 固件库的所有函数原型。
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 定义以下宏，以防止本头文件被递归包含 -------------------------------------*/
#ifndef __STM32F10x_SDIO_H
#define __STM32F10x_SDIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup SDIO  SDIO 外设驱动模块
  * @{
  */

/** @defgroup SDIO_Exported_Types  SDIO 导出的类型定义
  * @{
  */

typedef struct
{
  uint32_t SDIO_ClockEdge;            /*!< 指定进行位采样时所依据的时钟跳变。
                                           该参数可取 @ref SDIO_Clock_Edge 的值 */

  uint32_t SDIO_ClockBypass;          /*!< 指定 SDIO 时钟分频旁路是使能还是关闭。
                                           该参数可取 @ref SDIO_Clock_Bypass 的值 */

  uint32_t SDIO_ClockPowerSave;       /*!< 指定总线空闲时 SDIO 时钟输出是使能还是关闭。
                                           该参数可取 @ref SDIO_Clock_Power_Save 的值 */

  uint32_t SDIO_BusWide;              /*!< 指定 SDIO 总线宽度。
                                           该参数可取 @ref SDIO_Bus_Wide 的值 */

  uint32_t SDIO_HardwareFlowControl;  /*!< 指定 SDIO 硬件流控制是使能还是关闭。
                                           该参数可取 @ref SDIO_Hardware_Flow_Control 的值 */

  uint8_t SDIO_ClockDiv;              /*!< 指定 SDIO 控制器的时钟频率。
                                           该参数可取 0x00 到 0xFF 之间的值。 */
                                           
} SDIO_InitTypeDef;

typedef struct
{
  uint32_t SDIO_Argument;  /*!< 指定作为命令消息一部分发送给卡的 SDIO 命令参数。
                                如果命令带有参数，必须在将命令写入命令寄存器
                                之前把该参数装载到此寄存器中 */

  uint32_t SDIO_CmdIndex;  /*!< 指定 SDIO 命令索引。它必须小于 0x40。 */

  uint32_t SDIO_Response;  /*!< 指定 SDIO 响应类型。
                                该参数可取 @ref SDIO_Response_Type 的值 */

  uint32_t SDIO_Wait;      /*!< 指定 SDIO 等待中断请求是使能还是关闭。
                                该参数可取 @ref SDIO_Wait_Interrupt_State 的值 */

  uint32_t SDIO_CPSM;      /*!< 指定 SDIO 命令通路状态机（CPSM）是使能还是关闭。
                                该参数可取 @ref SDIO_CPSM_State 的值 */
} SDIO_CmdInitTypeDef;

typedef struct
{
  uint32_t SDIO_DataTimeOut;    /*!< 指定以卡总线时钟周期为单位的数据超时时间。 */

  uint32_t SDIO_DataLength;     /*!< 指定待传输的数据字节数。 */
 
  uint32_t SDIO_DataBlockSize;  /*!< 指定块传输的数据块大小。
                                     该参数可取 @ref SDIO_Data_Block_Size 的值 */
 
  uint32_t SDIO_TransferDir;    /*!< 指定数据传输方向，即传输是读还是写。
                                     该参数可取 @ref SDIO_Transfer_Direction 的值 */
 
  uint32_t SDIO_TransferMode;   /*!< 指定数据传输采用流模式还是块模式。
                                     该参数可取 @ref SDIO_Transfer_Type 的值 */
 
  uint32_t SDIO_DPSM;           /*!< 指定 SDIO 数据通路状态机（DPSM）是使能还是关闭。
                                     该参数可取 @ref SDIO_DPSM_State 的值 */
} SDIO_DataInitTypeDef;

/**
  * @}
  */ 

/** @defgroup SDIO_Exported_Constants  SDIO 导出的常量
  * @{
  */

/** @defgroup SDIO_Clock_Edge  SDIO 时钟边沿
  * @{
  */

#define SDIO_ClockEdge_Rising               ((uint32_t)0x00000000)
#define SDIO_ClockEdge_Falling              ((uint32_t)0x00002000)
#define IS_SDIO_CLOCK_EDGE(EDGE) (((EDGE) == SDIO_ClockEdge_Rising) || \
                                  ((EDGE) == SDIO_ClockEdge_Falling))
/**
  * @}
  */

/** @defgroup SDIO_Clock_Bypass  SDIO 时钟分频旁路
  * @{
  */

#define SDIO_ClockBypass_Disable             ((uint32_t)0x00000000)
#define SDIO_ClockBypass_Enable              ((uint32_t)0x00000400)    
#define IS_SDIO_CLOCK_BYPASS(BYPASS) (((BYPASS) == SDIO_ClockBypass_Disable) || \
                                     ((BYPASS) == SDIO_ClockBypass_Enable))
/**
  * @}
  */ 

/** @defgroup SDIO_Clock_Power_Save  SDIO 时钟省电
  * @{
  */

#define SDIO_ClockPowerSave_Disable         ((uint32_t)0x00000000)
#define SDIO_ClockPowerSave_Enable          ((uint32_t)0x00000200) 
#define IS_SDIO_CLOCK_POWER_SAVE(SAVE) (((SAVE) == SDIO_ClockPowerSave_Disable) || \
                                        ((SAVE) == SDIO_ClockPowerSave_Enable))
/**
  * @}
  */

/** @defgroup SDIO_Bus_Wide  SDIO 总线宽度
  * @{
  */

#define SDIO_BusWide_1b                     ((uint32_t)0x00000000)
#define SDIO_BusWide_4b                     ((uint32_t)0x00000800)
#define SDIO_BusWide_8b                     ((uint32_t)0x00001000)
#define IS_SDIO_BUS_WIDE(WIDE) (((WIDE) == SDIO_BusWide_1b) || ((WIDE) == SDIO_BusWide_4b) || \
                                ((WIDE) == SDIO_BusWide_8b))

/**
  * @}
  */

/** @defgroup SDIO_Hardware_Flow_Control  SDIO 硬件流控制
  * @{
  */

#define SDIO_HardwareFlowControl_Disable    ((uint32_t)0x00000000)
#define SDIO_HardwareFlowControl_Enable     ((uint32_t)0x00004000)
#define IS_SDIO_HARDWARE_FLOW_CONTROL(CONTROL) (((CONTROL) == SDIO_HardwareFlowControl_Disable) || \
                                                ((CONTROL) == SDIO_HardwareFlowControl_Enable))
/**
  * @}
  */

/** @defgroup SDIO_Power_State  SDIO 电源状态
  * @{
  */

#define SDIO_PowerState_OFF                 ((uint32_t)0x00000000)
#define SDIO_PowerState_ON                  ((uint32_t)0x00000003)
#define IS_SDIO_POWER_STATE(STATE) (((STATE) == SDIO_PowerState_OFF) || ((STATE) == SDIO_PowerState_ON)) 
/**
  * @}
  */ 


/** @defgroup SDIO_Interrupt_sources  SDIO 中断源
  * @{
  */

#define SDIO_IT_CCRCFAIL                    ((uint32_t)0x00000001)
#define SDIO_IT_DCRCFAIL                    ((uint32_t)0x00000002)
#define SDIO_IT_CTIMEOUT                    ((uint32_t)0x00000004)
#define SDIO_IT_DTIMEOUT                    ((uint32_t)0x00000008)
#define SDIO_IT_TXUNDERR                    ((uint32_t)0x00000010)
#define SDIO_IT_RXOVERR                     ((uint32_t)0x00000020)
#define SDIO_IT_CMDREND                     ((uint32_t)0x00000040)
#define SDIO_IT_CMDSENT                     ((uint32_t)0x00000080)
#define SDIO_IT_DATAEND                     ((uint32_t)0x00000100)
#define SDIO_IT_STBITERR                    ((uint32_t)0x00000200)
#define SDIO_IT_DBCKEND                     ((uint32_t)0x00000400)
#define SDIO_IT_CMDACT                      ((uint32_t)0x00000800)
#define SDIO_IT_TXACT                       ((uint32_t)0x00001000)
#define SDIO_IT_RXACT                       ((uint32_t)0x00002000)
#define SDIO_IT_TXFIFOHE                    ((uint32_t)0x00004000)
#define SDIO_IT_RXFIFOHF                    ((uint32_t)0x00008000)
#define SDIO_IT_TXFIFOF                     ((uint32_t)0x00010000)
#define SDIO_IT_RXFIFOF                     ((uint32_t)0x00020000)
#define SDIO_IT_TXFIFOE                     ((uint32_t)0x00040000)
#define SDIO_IT_RXFIFOE                     ((uint32_t)0x00080000)
#define SDIO_IT_TXDAVL                      ((uint32_t)0x00100000)
#define SDIO_IT_RXDAVL                      ((uint32_t)0x00200000)
#define SDIO_IT_SDIOIT                      ((uint32_t)0x00400000)
#define SDIO_IT_CEATAEND                    ((uint32_t)0x00800000)
#define IS_SDIO_IT(IT) ((((IT) & (uint32_t)0xFF000000) == 0x00) && ((IT) != (uint32_t)0x00))
/**
  * @}
  */ 

/** @defgroup SDIO_Command_Index  SDIO 命令索引
  * @{
  */

#define IS_SDIO_CMD_INDEX(INDEX)            ((INDEX) < 0x40)
/**
  * @}
  */

/** @defgroup SDIO_Response_Type  SDIO 响应类型
  * @{
  */

#define SDIO_Response_No                    ((uint32_t)0x00000000)
#define SDIO_Response_Short                 ((uint32_t)0x00000040)
#define SDIO_Response_Long                  ((uint32_t)0x000000C0)
#define IS_SDIO_RESPONSE(RESPONSE) (((RESPONSE) == SDIO_Response_No) || \
                                    ((RESPONSE) == SDIO_Response_Short) || \
                                    ((RESPONSE) == SDIO_Response_Long))
/**
  * @}
  */

/** @defgroup SDIO_Wait_Interrupt_State  SDIO 等待中断状态
  * @{
  */

#define SDIO_Wait_No                        ((uint32_t)0x00000000) /*!< SDIO 不等待，使能超时 */
#define SDIO_Wait_IT                        ((uint32_t)0x00000100) /*!< SDIO 等待中断请求 */
#define SDIO_Wait_Pend                      ((uint32_t)0x00000200) /*!< SDIO 等待传输结束 */
#define IS_SDIO_WAIT(WAIT) (((WAIT) == SDIO_Wait_No) || ((WAIT) == SDIO_Wait_IT) || \
                            ((WAIT) == SDIO_Wait_Pend))
/**
  * @}
  */

/** @defgroup SDIO_CPSM_State  SDIO 命令通路状态机状态
  * @{
  */

#define SDIO_CPSM_Disable                    ((uint32_t)0x00000000)
#define SDIO_CPSM_Enable                     ((uint32_t)0x00000400)
#define IS_SDIO_CPSM(CPSM) (((CPSM) == SDIO_CPSM_Enable) || ((CPSM) == SDIO_CPSM_Disable))
/**
  * @}
  */ 

/** @defgroup SDIO_Response_Registers  SDIO 响应寄存器
  * @{
  */

#define SDIO_RESP1                          ((uint32_t)0x00000000)
#define SDIO_RESP2                          ((uint32_t)0x00000004)
#define SDIO_RESP3                          ((uint32_t)0x00000008)
#define SDIO_RESP4                          ((uint32_t)0x0000000C)
#define IS_SDIO_RESP(RESP) (((RESP) == SDIO_RESP1) || ((RESP) == SDIO_RESP2) || \
                            ((RESP) == SDIO_RESP3) || ((RESP) == SDIO_RESP4))
/**
  * @}
  */

/** @defgroup SDIO_Data_Length  SDIO 数据长度
  * @{
  */

#define IS_SDIO_DATA_LENGTH(LENGTH) ((LENGTH) <= 0x01FFFFFF)
/**
  * @}
  */

/** @defgroup SDIO_Data_Block_Size  SDIO 数据块大小
  * @{
  */

#define SDIO_DataBlockSize_1b               ((uint32_t)0x00000000)
#define SDIO_DataBlockSize_2b               ((uint32_t)0x00000010)
#define SDIO_DataBlockSize_4b               ((uint32_t)0x00000020)
#define SDIO_DataBlockSize_8b               ((uint32_t)0x00000030)
#define SDIO_DataBlockSize_16b              ((uint32_t)0x00000040)
#define SDIO_DataBlockSize_32b              ((uint32_t)0x00000050)
#define SDIO_DataBlockSize_64b              ((uint32_t)0x00000060)
#define SDIO_DataBlockSize_128b             ((uint32_t)0x00000070)
#define SDIO_DataBlockSize_256b             ((uint32_t)0x00000080)
#define SDIO_DataBlockSize_512b             ((uint32_t)0x00000090)
#define SDIO_DataBlockSize_1024b            ((uint32_t)0x000000A0)
#define SDIO_DataBlockSize_2048b            ((uint32_t)0x000000B0)
#define SDIO_DataBlockSize_4096b            ((uint32_t)0x000000C0)
#define SDIO_DataBlockSize_8192b            ((uint32_t)0x000000D0)
#define SDIO_DataBlockSize_16384b           ((uint32_t)0x000000E0)
#define IS_SDIO_BLOCK_SIZE(SIZE) (((SIZE) == SDIO_DataBlockSize_1b) || \
                                  ((SIZE) == SDIO_DataBlockSize_2b) || \
                                  ((SIZE) == SDIO_DataBlockSize_4b) || \
                                  ((SIZE) == SDIO_DataBlockSize_8b) || \
                                  ((SIZE) == SDIO_DataBlockSize_16b) || \
                                  ((SIZE) == SDIO_DataBlockSize_32b) || \
                                  ((SIZE) == SDIO_DataBlockSize_64b) || \
                                  ((SIZE) == SDIO_DataBlockSize_128b) || \
                                  ((SIZE) == SDIO_DataBlockSize_256b) || \
                                  ((SIZE) == SDIO_DataBlockSize_512b) || \
                                  ((SIZE) == SDIO_DataBlockSize_1024b) || \
                                  ((SIZE) == SDIO_DataBlockSize_2048b) || \
                                  ((SIZE) == SDIO_DataBlockSize_4096b) || \
                                  ((SIZE) == SDIO_DataBlockSize_8192b) || \
                                  ((SIZE) == SDIO_DataBlockSize_16384b)) 
/**
  * @}
  */

/** @defgroup SDIO_Transfer_Direction  SDIO 传输方向
  * @{
  */

#define SDIO_TransferDir_ToCard             ((uint32_t)0x00000000)
#define SDIO_TransferDir_ToSDIO             ((uint32_t)0x00000002)
#define IS_SDIO_TRANSFER_DIR(DIR) (((DIR) == SDIO_TransferDir_ToCard) || \
                                   ((DIR) == SDIO_TransferDir_ToSDIO))
/**
  * @}
  */

/** @defgroup SDIO_Transfer_Type  SDIO 传输类型
  * @{
  */

#define SDIO_TransferMode_Block             ((uint32_t)0x00000000)
#define SDIO_TransferMode_Stream            ((uint32_t)0x00000004)
#define IS_SDIO_TRANSFER_MODE(MODE) (((MODE) == SDIO_TransferMode_Stream) || \
                                     ((MODE) == SDIO_TransferMode_Block))
/**
  * @}
  */

/** @defgroup SDIO_DPSM_State  SDIO 数据通路状态机状态
  * @{
  */

#define SDIO_DPSM_Disable                    ((uint32_t)0x00000000)
#define SDIO_DPSM_Enable                     ((uint32_t)0x00000001)
#define IS_SDIO_DPSM(DPSM) (((DPSM) == SDIO_DPSM_Enable) || ((DPSM) == SDIO_DPSM_Disable))
/**
  * @}
  */

/** @defgroup SDIO_Flags  SDIO 标志
  * @{
  */

#define SDIO_FLAG_CCRCFAIL                  ((uint32_t)0x00000001)
#define SDIO_FLAG_DCRCFAIL                  ((uint32_t)0x00000002)
#define SDIO_FLAG_CTIMEOUT                  ((uint32_t)0x00000004)
#define SDIO_FLAG_DTIMEOUT                  ((uint32_t)0x00000008)
#define SDIO_FLAG_TXUNDERR                  ((uint32_t)0x00000010)
#define SDIO_FLAG_RXOVERR                   ((uint32_t)0x00000020)
#define SDIO_FLAG_CMDREND                   ((uint32_t)0x00000040)
#define SDIO_FLAG_CMDSENT                   ((uint32_t)0x00000080)
#define SDIO_FLAG_DATAEND                   ((uint32_t)0x00000100)
#define SDIO_FLAG_STBITERR                  ((uint32_t)0x00000200)
#define SDIO_FLAG_DBCKEND                   ((uint32_t)0x00000400)
#define SDIO_FLAG_CMDACT                    ((uint32_t)0x00000800)
#define SDIO_FLAG_TXACT                     ((uint32_t)0x00001000)
#define SDIO_FLAG_RXACT                     ((uint32_t)0x00002000)
#define SDIO_FLAG_TXFIFOHE                  ((uint32_t)0x00004000)
#define SDIO_FLAG_RXFIFOHF                  ((uint32_t)0x00008000)
#define SDIO_FLAG_TXFIFOF                   ((uint32_t)0x00010000)
#define SDIO_FLAG_RXFIFOF                   ((uint32_t)0x00020000)
#define SDIO_FLAG_TXFIFOE                   ((uint32_t)0x00040000)
#define SDIO_FLAG_RXFIFOE                   ((uint32_t)0x00080000)
#define SDIO_FLAG_TXDAVL                    ((uint32_t)0x00100000)
#define SDIO_FLAG_RXDAVL                    ((uint32_t)0x00200000)
#define SDIO_FLAG_SDIOIT                    ((uint32_t)0x00400000)
#define SDIO_FLAG_CEATAEND                  ((uint32_t)0x00800000)
#define IS_SDIO_FLAG(FLAG) (((FLAG)  == SDIO_FLAG_CCRCFAIL) || \
                            ((FLAG)  == SDIO_FLAG_DCRCFAIL) || \
                            ((FLAG)  == SDIO_FLAG_CTIMEOUT) || \
                            ((FLAG)  == SDIO_FLAG_DTIMEOUT) || \
                            ((FLAG)  == SDIO_FLAG_TXUNDERR) || \
                            ((FLAG)  == SDIO_FLAG_RXOVERR) || \
                            ((FLAG)  == SDIO_FLAG_CMDREND) || \
                            ((FLAG)  == SDIO_FLAG_CMDSENT) || \
                            ((FLAG)  == SDIO_FLAG_DATAEND) || \
                            ((FLAG)  == SDIO_FLAG_STBITERR) || \
                            ((FLAG)  == SDIO_FLAG_DBCKEND) || \
                            ((FLAG)  == SDIO_FLAG_CMDACT) || \
                            ((FLAG)  == SDIO_FLAG_TXACT) || \
                            ((FLAG)  == SDIO_FLAG_RXACT) || \
                            ((FLAG)  == SDIO_FLAG_TXFIFOHE) || \
                            ((FLAG)  == SDIO_FLAG_RXFIFOHF) || \
                            ((FLAG)  == SDIO_FLAG_TXFIFOF) || \
                            ((FLAG)  == SDIO_FLAG_RXFIFOF) || \
                            ((FLAG)  == SDIO_FLAG_TXFIFOE) || \
                            ((FLAG)  == SDIO_FLAG_RXFIFOE) || \
                            ((FLAG)  == SDIO_FLAG_TXDAVL) || \
                            ((FLAG)  == SDIO_FLAG_RXDAVL) || \
                            ((FLAG)  == SDIO_FLAG_SDIOIT) || \
                            ((FLAG)  == SDIO_FLAG_CEATAEND))

#define IS_SDIO_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFF3FF800) == 0x00) && ((FLAG) != (uint32_t)0x00))

#define IS_SDIO_GET_IT(IT) (((IT)  == SDIO_IT_CCRCFAIL) || \
                            ((IT)  == SDIO_IT_DCRCFAIL) || \
                            ((IT)  == SDIO_IT_CTIMEOUT) || \
                            ((IT)  == SDIO_IT_DTIMEOUT) || \
                            ((IT)  == SDIO_IT_TXUNDERR) || \
                            ((IT)  == SDIO_IT_RXOVERR) || \
                            ((IT)  == SDIO_IT_CMDREND) || \
                            ((IT)  == SDIO_IT_CMDSENT) || \
                            ((IT)  == SDIO_IT_DATAEND) || \
                            ((IT)  == SDIO_IT_STBITERR) || \
                            ((IT)  == SDIO_IT_DBCKEND) || \
                            ((IT)  == SDIO_IT_CMDACT) || \
                            ((IT)  == SDIO_IT_TXACT) || \
                            ((IT)  == SDIO_IT_RXACT) || \
                            ((IT)  == SDIO_IT_TXFIFOHE) || \
                            ((IT)  == SDIO_IT_RXFIFOHF) || \
                            ((IT)  == SDIO_IT_TXFIFOF) || \
                            ((IT)  == SDIO_IT_RXFIFOF) || \
                            ((IT)  == SDIO_IT_TXFIFOE) || \
                            ((IT)  == SDIO_IT_RXFIFOE) || \
                            ((IT)  == SDIO_IT_TXDAVL) || \
                            ((IT)  == SDIO_IT_RXDAVL) || \
                            ((IT)  == SDIO_IT_SDIOIT) || \
                            ((IT)  == SDIO_IT_CEATAEND))

#define IS_SDIO_CLEAR_IT(IT) ((((IT) & (uint32_t)0xFF3FF800) == 0x00) && ((IT) != (uint32_t)0x00))

/**
  * @}
  */

/** @defgroup SDIO_Read_Wait_Mode  SDIO 读等待模式
  * @{
  */

#define SDIO_ReadWaitMode_CLK               ((uint32_t)0x00000001)
#define SDIO_ReadWaitMode_DATA2             ((uint32_t)0x00000000)
#define IS_SDIO_READWAIT_MODE(MODE) (((MODE) == SDIO_ReadWaitMode_CLK) || \
                                     ((MODE) == SDIO_ReadWaitMode_DATA2))
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup SDIO_Exported_Macros  SDIO 导出的宏
  * @{
  */

/**
  * @}
  */

/** @defgroup SDIO_Exported_Functions  SDIO 导出的函数
  * @{
  */

void SDIO_DeInit(void);
void SDIO_Init(SDIO_InitTypeDef* SDIO_InitStruct);
void SDIO_StructInit(SDIO_InitTypeDef* SDIO_InitStruct);
void SDIO_ClockCmd(FunctionalState NewState);
void SDIO_SetPowerState(uint32_t SDIO_PowerState);
uint32_t SDIO_GetPowerState(void);
void SDIO_ITConfig(uint32_t SDIO_IT, FunctionalState NewState);
void SDIO_DMACmd(FunctionalState NewState);
void SDIO_SendCommand(SDIO_CmdInitTypeDef *SDIO_CmdInitStruct);
void SDIO_CmdStructInit(SDIO_CmdInitTypeDef* SDIO_CmdInitStruct);
uint8_t SDIO_GetCommandResponse(void);
uint32_t SDIO_GetResponse(uint32_t SDIO_RESP);
void SDIO_DataConfig(SDIO_DataInitTypeDef* SDIO_DataInitStruct);
void SDIO_DataStructInit(SDIO_DataInitTypeDef* SDIO_DataInitStruct);
uint32_t SDIO_GetDataCounter(void);
uint32_t SDIO_ReadData(void);
void SDIO_WriteData(uint32_t Data);
uint32_t SDIO_GetFIFOCount(void);
void SDIO_StartSDIOReadWait(FunctionalState NewState);
void SDIO_StopSDIOReadWait(FunctionalState NewState);
void SDIO_SetSDIOReadWaitMode(uint32_t SDIO_ReadWaitMode);
void SDIO_SetSDIOOperation(FunctionalState NewState);
void SDIO_SendSDIOSuspendCmd(FunctionalState NewState);
void SDIO_CommandCompletionCmd(FunctionalState NewState);
void SDIO_CEATAITCmd(FunctionalState NewState);
void SDIO_SendCEATACmd(FunctionalState NewState);
FlagStatus SDIO_GetFlagStatus(uint32_t SDIO_FLAG);
void SDIO_ClearFlag(uint32_t SDIO_FLAG);
ITStatus SDIO_GetITStatus(uint32_t SDIO_IT);
void SDIO_ClearITPendingBit(uint32_t SDIO_IT);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_SDIO_H */
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

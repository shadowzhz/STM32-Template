/**
  ******************************************************************************
  * @file    stm32f10x_sdio.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供 SDIO 的所有固件函数。
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

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_sdio.h"
#include "stm32f10x_rcc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup SDIO
  * @brief SDIO 驱动模块
  * @{
  */ 

/** @defgroup SDIO_Private_TypesDefinitions   SDIO 私有类型定义
  * @{
  */ 

/* ------------ SDIO 寄存器在别名区中的位地址 ----------- */
#define SDIO_OFFSET                (SDIO_BASE - PERIPH_BASE)

/* --- CLKCR 寄存器 ---*/

/* CLKEN 位的别名写字地址 */
#define CLKCR_OFFSET              (SDIO_OFFSET + 0x04)
#define CLKEN_BitNumber           0x08
#define CLKCR_CLKEN_BB            (PERIPH_BB_BASE + (CLKCR_OFFSET * 32) + (CLKEN_BitNumber * 4))

/* --- CMD 寄存器 ---*/

/* SDIOSUSPEND 位的别名写字地址 */
#define CMD_OFFSET                (SDIO_OFFSET + 0x0C)
#define SDIOSUSPEND_BitNumber     0x0B
#define CMD_SDIOSUSPEND_BB        (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (SDIOSUSPEND_BitNumber * 4))

/* ENCMDCOMPL 位的别名写字地址 */
#define ENCMDCOMPL_BitNumber      0x0C
#define CMD_ENCMDCOMPL_BB         (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (ENCMDCOMPL_BitNumber * 4))

/* NIEN 位的别名写字地址 */
#define NIEN_BitNumber            0x0D
#define CMD_NIEN_BB               (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (NIEN_BitNumber * 4))

/* ATACMD 位的别名写字地址 */
#define ATACMD_BitNumber          0x0E
#define CMD_ATACMD_BB             (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (ATACMD_BitNumber * 4))

/* --- DCTRL 寄存器 ---*/

/* DMAEN 位的别名写字地址 */
#define DCTRL_OFFSET              (SDIO_OFFSET + 0x2C)
#define DMAEN_BitNumber           0x03
#define DCTRL_DMAEN_BB            (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (DMAEN_BitNumber * 4))

/* RWSTART 位的别名写字地址 */
#define RWSTART_BitNumber         0x08
#define DCTRL_RWSTART_BB          (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (RWSTART_BitNumber * 4))

/* RWSTOP 位的别名写字地址 */
#define RWSTOP_BitNumber          0x09
#define DCTRL_RWSTOP_BB           (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (RWSTOP_BitNumber * 4))

/* RWMOD 位的别名写字地址 */
#define RWMOD_BitNumber           0x0A
#define DCTRL_RWMOD_BB            (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (RWMOD_BitNumber * 4))

/* SDIOEN 位的别名写字地址 */
#define SDIOEN_BitNumber          0x0B
#define DCTRL_SDIOEN_BB           (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (SDIOEN_BitNumber * 4))

/* ---------------------- SDIO 寄存器位掩码 ------------------------ */

/* --- CLKCR 寄存器 ---*/

/* CLKCR 寄存器清除掩码 */
#define CLKCR_CLEAR_MASK         ((uint32_t)0xFFFF8100) 

/* --- PWRCTRL 寄存器 ---*/

/* SDIO PWRCTRL 掩码 */
#define PWR_PWRCTRL_MASK         ((uint32_t)0xFFFFFFFC)

/* --- DCTRL 寄存器 ---*/

/* SDIO DCTRL 清除掩码 */
#define DCTRL_CLEAR_MASK         ((uint32_t)0xFFFFFF08)

/* --- CMD 寄存器 ---*/

/* CMD 寄存器清除掩码 */
#define CMD_CLEAR_MASK           ((uint32_t)0xFFFFF800)

/* SDIO RESP 寄存器地址 */
#define SDIO_RESP_ADDR           ((uint32_t)(SDIO_BASE + 0x14))

/**
  * @}
  */

/** @defgroup SDIO_Private_Defines   SDIO 私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup SDIO_Private_Macros   SDIO 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup SDIO_Private_Variables   SDIO 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup SDIO_Private_FunctionPrototypes   SDIO 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup SDIO_Private_Functions   SDIO 私有函数
  * @{
  */

/**
  * @brief  将 SDIO 外设寄存器反初始化为它们的默认复位值。
  * @param  None
  * @retval 无
  */
void SDIO_DeInit(void)
{
  SDIO->POWER = 0x00000000;
  SDIO->CLKCR = 0x00000000;
  SDIO->ARG = 0x00000000;
  SDIO->CMD = 0x00000000;
  SDIO->DTIMER = 0x00000000;
  SDIO->DLEN = 0x00000000;
  SDIO->DCTRL = 0x00000000;
  SDIO->ICR = 0x00C007FF;
  SDIO->MASK = 0x00000000;
}

/**
  * @brief  根据 SDIO_InitStruct 中指定的参数初始化 SDIO 外设。
  * @param  SDIO_InitStruct : 指向 SDIO_InitTypeDef 结构的指针，
  *         该结构包含 SDIO 外设的配置信息。
  * @retval 无
  */
void SDIO_Init(SDIO_InitTypeDef* SDIO_InitStruct)
{
  uint32_t tmpreg = 0;
    
  /* 检查参数 */
  assert_param(IS_SDIO_CLOCK_EDGE(SDIO_InitStruct->SDIO_ClockEdge));
  assert_param(IS_SDIO_CLOCK_BYPASS(SDIO_InitStruct->SDIO_ClockBypass));
  assert_param(IS_SDIO_CLOCK_POWER_SAVE(SDIO_InitStruct->SDIO_ClockPowerSave));
  assert_param(IS_SDIO_BUS_WIDE(SDIO_InitStruct->SDIO_BusWide));
  assert_param(IS_SDIO_HARDWARE_FLOW_CONTROL(SDIO_InitStruct->SDIO_HardwareFlowControl)); 
   
/*---------------------------- SDIO CLKCR 配置 ------------------------*/  
  /* 获取 SDIO CLKCR 值 */
  tmpreg = SDIO->CLKCR;
  
  /* 清除 CLKDIV、PWRSAV、BYPASS、WIDBUS、NEGEDGE、HWFC_EN 位 */
  tmpreg &= CLKCR_CLEAR_MASK;
  
  /* 根据 SDIO_ClockDiv 值设置 CLKDIV 位 */
  /* 根据 SDIO_ClockPowerSave 值设置 PWRSAV 位 */
  /* 根据 SDIO_ClockBypass 值设置 BYPASS 位 */
  /* 根据 SDIO_BusWide 值设置 WIDBUS 位 */
  /* 根据 SDIO_ClockEdge 值设置 NEGEDGE 位 */
  /* 根据 SDIO_HardwareFlowControl 值设置 HWFC_EN 位 */
  tmpreg |= (SDIO_InitStruct->SDIO_ClockDiv  | SDIO_InitStruct->SDIO_ClockPowerSave |
             SDIO_InitStruct->SDIO_ClockBypass | SDIO_InitStruct->SDIO_BusWide |
             SDIO_InitStruct->SDIO_ClockEdge | SDIO_InitStruct->SDIO_HardwareFlowControl); 
  
  /* 写入 SDIO CLKCR */
  SDIO->CLKCR = tmpreg;
}

/**
  * @brief  将 SDIO_InitStruct 的每个成员填充为默认值。
  * @param  SDIO_InitStruct: 指向将被初始化的 SDIO_InitTypeDef 结构的指针。
  * @retval 无
  */
void SDIO_StructInit(SDIO_InitTypeDef* SDIO_InitStruct)
{
  /* SDIO_InitStruct 成员默认值 */
  SDIO_InitStruct->SDIO_ClockDiv = 0x00;
  SDIO_InitStruct->SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStruct->SDIO_ClockBypass = SDIO_ClockBypass_Disable;
  SDIO_InitStruct->SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
  SDIO_InitStruct->SDIO_BusWide = SDIO_BusWide_1b;
  SDIO_InitStruct->SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
}

/**
  * @brief  使能或关闭 SDIO 时钟。
  * @param  NewState: SDIO 时钟的新状态。该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void SDIO_ClockCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) CLKCR_CLKEN_BB = (uint32_t)NewState;
}

/**
  * @brief  设置控制器的电源状态。
  * @param  SDIO_PowerState: 电源状态的新状态。
  *   该参数可取以下值之一：
  *     @arg SDIO_PowerState_OFF
  *     @arg SDIO_PowerState_ON
  * @retval 无
  */
void SDIO_SetPowerState(uint32_t SDIO_PowerState)
{
  /* 检查参数 */
  assert_param(IS_SDIO_POWER_STATE(SDIO_PowerState));
  
  SDIO->POWER &= PWR_PWRCTRL_MASK;
  SDIO->POWER |= SDIO_PowerState;
}

/**
  * @brief  获取控制器的电源状态。
  * @param  None
  * @retval 控制器的电源状态。返回值可以是以下之一：
  * - 0x00: 电源关闭
  * - 0x02: 电源上电
  * - 0x03: 电源开启
  */
uint32_t SDIO_GetPowerState(void)
{
  return (SDIO->POWER & (~PWR_PWRCTRL_MASK));
}

/**
  * @brief  使能或关闭 SDIO 中断。
  * @param  SDIO_IT: 指定要使能或关闭的 SDIO 中断源。
  *   该参数可取以下值之一或其组合：
  *     @arg SDIO_IT_CCRCFAIL: 收到命令响应（CRC 校验失败）中断
  *     @arg SDIO_IT_DCRCFAIL: 数据块发送/接收（CRC 校验失败）中断
  *     @arg SDIO_IT_CTIMEOUT: 命令响应超时中断
  *     @arg SDIO_IT_DTIMEOUT: 数据超时中断
  *     @arg SDIO_IT_TXUNDERR: 发送 FIFO 下溢错误中断
  *     @arg SDIO_IT_RXOVERR:  接收 FIFO 上溢错误中断
  *     @arg SDIO_IT_CMDREND:  收到命令响应（CRC 校验通过）中断
  *     @arg SDIO_IT_CMDSENT:  命令已发送（无需响应）中断
  *     @arg SDIO_IT_DATAEND:  数据结束（数据计数器 SDIDCOUNT 为零）中断
  *     @arg SDIO_IT_STBITERR: 宽总线模式下所有数据信号上均未检测到
  *                            起始位中断
  *     @arg SDIO_IT_DBCKEND:  数据块发送/接收（CRC 校验通过）中断
  *     @arg SDIO_IT_CMDACT:   命令传输进行中中断
  *     @arg SDIO_IT_TXACT:    数据发送进行中中断
  *     @arg SDIO_IT_RXACT:    数据接收进行中中断
  *     @arg SDIO_IT_TXFIFOHE: 发送 FIFO 半空中断
  *     @arg SDIO_IT_RXFIFOHF: 接收 FIFO 半满中断
  *     @arg SDIO_IT_TXFIFOF:  发送 FIFO 满中断
  *     @arg SDIO_IT_RXFIFOF:  接收 FIFO 满中断
  *     @arg SDIO_IT_TXFIFOE:  发送 FIFO 空中断
  *     @arg SDIO_IT_RXFIFOE:  接收 FIFO 空中断
  *     @arg SDIO_IT_TXDAVL:   发送 FIFO 中有数据可用中断
  *     @arg SDIO_IT_RXDAVL:   接收 FIFO 中有数据可用中断
  *     @arg SDIO_IT_SDIOIT:   收到 SD I/O 中断
  *     @arg SDIO_IT_CEATAEND: 收到 CMD61 的 CE-ATA 命令完成信号中断
  * @param  NewState: 指定 SDIO 中断的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void SDIO_ITConfig(uint32_t SDIO_IT, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_SDIO_IT(SDIO_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 使能 SDIO 中断 */
    SDIO->MASK |= SDIO_IT;
  }
  else
  {
    /* 关闭 SDIO 中断 */
    SDIO->MASK &= ~SDIO_IT;
  } 
}

/**
  * @brief  使能或关闭 SDIO DMA 请求。
  * @param  NewState: 所选 SDIO DMA 请求的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void SDIO_DMACmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) DCTRL_DMAEN_BB = (uint32_t)NewState;
}

/**
  * @brief  根据 SDIO_CmdInitStruct 中指定的参数初始化 SDIO 命令并发送该命令。
  * @param  SDIO_CmdInitStruct : 指向 SDIO_CmdInitTypeDef 结构的指针，
  *         该结构包含 SDIO 命令的配置信息。
  * @retval 无
  */
void SDIO_SendCommand(SDIO_CmdInitTypeDef *SDIO_CmdInitStruct)
{
  uint32_t tmpreg = 0;
  
  /* 检查参数 */
  assert_param(IS_SDIO_CMD_INDEX(SDIO_CmdInitStruct->SDIO_CmdIndex));
  assert_param(IS_SDIO_RESPONSE(SDIO_CmdInitStruct->SDIO_Response));
  assert_param(IS_SDIO_WAIT(SDIO_CmdInitStruct->SDIO_Wait));
  assert_param(IS_SDIO_CPSM(SDIO_CmdInitStruct->SDIO_CPSM));
  
/*---------------------------- SDIO ARG 配置 ------------------------*/
  /* 设置 SDIO 参数值 */
  SDIO->ARG = SDIO_CmdInitStruct->SDIO_Argument;
  
/*---------------------------- SDIO CMD 配置 ------------------------*/  
  /* 获取 SDIO CMD 的值 */
  tmpreg = SDIO->CMD;
  /* 清零 CMDINDEX、WAITRESP、WAITINT、WAITPEND、CPSMEN 位 */
  tmpreg &= CMD_CLEAR_MASK;
  /* 根据 SDIO_CmdIndex 的值设置 CMDINDEX 位 */
  /* 根据 SDIO_Response 的值设置 WAITRESP 位 */
  /* 根据 SDIO_Wait 的值设置 WAITINT 和 WAITPEND 位 */
  /* 根据 SDIO_CPSM 的值设置 CPSMEN 位 */
  tmpreg |= (uint32_t)SDIO_CmdInitStruct->SDIO_CmdIndex | SDIO_CmdInitStruct->SDIO_Response
           | SDIO_CmdInitStruct->SDIO_Wait | SDIO_CmdInitStruct->SDIO_CPSM;
  
  /* 写入 SDIO CMD 寄存器 */
  SDIO->CMD = tmpreg;
}

/**
  * @brief  将 SDIO_CmdInitStruct 的每个成员填充为默认值。
  * @param  SDIO_CmdInitStruct: 指向将被初始化的 SDIO_CmdInitTypeDef
  *         结构的指针。
  * @retval None
  */
void SDIO_CmdStructInit(SDIO_CmdInitTypeDef* SDIO_CmdInitStruct)
{
  /* SDIO_CmdInitStruct 成员的默认值 */
  SDIO_CmdInitStruct->SDIO_Argument = 0x00;
  SDIO_CmdInitStruct->SDIO_CmdIndex = 0x00;
  SDIO_CmdInitStruct->SDIO_Response = SDIO_Response_No;
  SDIO_CmdInitStruct->SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStruct->SDIO_CPSM = SDIO_CPSM_Disable;
}

/**
  * @brief  返回最后一个收到响应的命令的命令索引。
  * @param  None
  * @retval 返回最后一个收到命令响应的命令索引。
  */
uint8_t SDIO_GetCommandResponse(void)
{
  return (uint8_t)(SDIO->RESPCMD);
}

/**
  * @brief  返回最后一个命令从卡接收到的响应。
  * @param  SDIO_RESP: 指定 SDIO 响应寄存器。
  *   该参数可取以下值之一：
  *     @arg SDIO_RESP1: 响应寄存器 1
  *     @arg SDIO_RESP2: 响应寄存器 2
  *     @arg SDIO_RESP3: 响应寄存器 3
  *     @arg SDIO_RESP4: 响应寄存器 4
  * @retval 对应的响应寄存器值。
  */
uint32_t SDIO_GetResponse(uint32_t SDIO_RESP)
{
  __IO uint32_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_SDIO_RESP(SDIO_RESP));

  tmp = SDIO_RESP_ADDR + SDIO_RESP;
  
  return (*(__IO uint32_t *) tmp); 
}

/**
  * @brief  根据 SDIO_DataInitStruct 中指定的参数初始化 SDIO 数据通路。
  * @param  SDIO_DataInitStruct : 指向 SDIO_DataInitTypeDef 结构的指针，
  *   该结构包含 SDIO 命令的配置信息。
  * @retval None
  */
void SDIO_DataConfig(SDIO_DataInitTypeDef* SDIO_DataInitStruct)
{
  uint32_t tmpreg = 0;
  
  /* 检查参数 */
  assert_param(IS_SDIO_DATA_LENGTH(SDIO_DataInitStruct->SDIO_DataLength));
  assert_param(IS_SDIO_BLOCK_SIZE(SDIO_DataInitStruct->SDIO_DataBlockSize));
  assert_param(IS_SDIO_TRANSFER_DIR(SDIO_DataInitStruct->SDIO_TransferDir));
  assert_param(IS_SDIO_TRANSFER_MODE(SDIO_DataInitStruct->SDIO_TransferMode));
  assert_param(IS_SDIO_DPSM(SDIO_DataInitStruct->SDIO_DPSM));

/*---------------------------- SDIO DTIMER 配置 ---------------------*/
  /* 设置 SDIO 数据超时值 */
  SDIO->DTIMER = SDIO_DataInitStruct->SDIO_DataTimeOut;

/*---------------------------- SDIO DLEN 配置 -----------------------*/
  /* 设置 SDIO 数据长度值 */
  SDIO->DLEN = SDIO_DataInitStruct->SDIO_DataLength;

/*---------------------------- SDIO DCTRL 配置 ----------------------*/  
  /* 获取 SDIO DCTRL 的值 */
  tmpreg = SDIO->DCTRL;
  /* 清零 DEN、DTMODE、DTDIR 和 DBCKSIZE 位 */
  tmpreg &= DCTRL_CLEAR_MASK;
  /* 根据 SDIO_DPSM 的值设置 DEN 位 */
  /* 根据 SDIO_TransferMode 的值设置 DTMODE 位 */
  /* 根据 SDIO_TransferDir 的值设置 DTDIR 位 */
  /* 根据 SDIO_DataBlockSize 的值设置 DBCKSIZE 位 */
  tmpreg |= (uint32_t)SDIO_DataInitStruct->SDIO_DataBlockSize | SDIO_DataInitStruct->SDIO_TransferDir
           | SDIO_DataInitStruct->SDIO_TransferMode | SDIO_DataInitStruct->SDIO_DPSM;

  /* 写入 SDIO DCTRL 寄存器 */
  SDIO->DCTRL = tmpreg;
}

/**
  * @brief  将 SDIO_DataInitStruct 的每个成员填充为默认值。
  * @param  SDIO_DataInitStruct: 指向将被初始化的 SDIO_DataInitTypeDef
  *         结构的指针。
  * @retval None
  */
void SDIO_DataStructInit(SDIO_DataInitTypeDef* SDIO_DataInitStruct)
{
  /* SDIO_DataInitStruct 成员的默认值 */
  SDIO_DataInitStruct->SDIO_DataTimeOut = 0xFFFFFFFF;
  SDIO_DataInitStruct->SDIO_DataLength = 0x00;
  SDIO_DataInitStruct->SDIO_DataBlockSize = SDIO_DataBlockSize_1b;
  SDIO_DataInitStruct->SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStruct->SDIO_TransferMode = SDIO_TransferMode_Block;  
  SDIO_DataInitStruct->SDIO_DPSM = SDIO_DPSM_Disable;
}

/**
  * @brief  返回剩余待传输的数据字节数。
  * @param  None
  * @retval 剩余待传输的数据字节数
  */
uint32_t SDIO_GetDataCounter(void)
{ 
  return SDIO->DCOUNT;
}

/**
  * @brief  从 Rx FIFO 读取一个数据字。
  * @param  None
  * @retval 接收到的数据
  */
uint32_t SDIO_ReadData(void)
{ 
  return SDIO->FIFO;
}

/**
  * @brief  向 Tx FIFO 写入一个数据字。
  * @param  Data: 要写入的 32 位数据字。
  * @retval None
  */
void SDIO_WriteData(uint32_t Data)
{ 
  SDIO->FIFO = Data;
}

/**
  * @brief  返回 FIFO 中剩余待写入或待读取的字数。
  * @param  None
  * @retval 剩余字数。
  */
uint32_t SDIO_GetFIFOCount(void)
{ 
  return SDIO->FIFOCNT;
}

/**
  * @brief  启动 SD I/O 读等待操作。
  * @param  NewState: 启动 SDIO 读等待操作的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void SDIO_StartSDIOReadWait(FunctionalState NewState)
{ 
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) DCTRL_RWSTART_BB = (uint32_t) NewState;
}

/**
  * @brief  停止 SD I/O 读等待操作。
  * @param  NewState: 停止 SDIO 读等待操作的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void SDIO_StopSDIOReadWait(FunctionalState NewState)
{ 
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) DCTRL_RWSTOP_BB = (uint32_t) NewState;
}

/**
  * @brief  设置插入读等待间隔的两个选项之一。
  * @param  SDIO_ReadWaitMode: SD I/O 读等待操作模式。
  *   该参数可取：
  *     @arg SDIO_ReadWaitMode_CLK: 通过停止 SDIOCLK 进行读等待控制
  *     @arg SDIO_ReadWaitMode_DATA2: 使用 SDIO_DATA2 进行读等待控制
  * @retval None
  */
void SDIO_SetSDIOReadWaitMode(uint32_t SDIO_ReadWaitMode)
{
  /* 检查参数 */
  assert_param(IS_SDIO_READWAIT_MODE(SDIO_ReadWaitMode));
  
  *(__IO uint32_t *) DCTRL_RWMOD_BB = SDIO_ReadWaitMode;
}

/**
  * @brief  使能或关闭 SD I/O 模式操作。
  * @param  NewState: SDIO 特定操作的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void SDIO_SetSDIOOperation(FunctionalState NewState)
{ 
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) DCTRL_SDIOEN_BB = (uint32_t)NewState;
}

/**
  * @brief  使能或关闭 SD I/O 模式挂起命令的发送。
  * @param  NewState: SD I/O 模式挂起命令的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void SDIO_SendSDIOSuspendCmd(FunctionalState NewState)
{ 
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) CMD_SDIOSUSPEND_BB = (uint32_t)NewState;
}

/**
  * @brief  使能或关闭命令完成信号。
  * @param  NewState: 命令完成信号的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void SDIO_CommandCompletionCmd(FunctionalState NewState)
{ 
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) CMD_ENCMDCOMPL_BB = (uint32_t)NewState;
}

/**
  * @brief  使能或关闭 CE-ATA 中断。
  * @param  NewState: CE-ATA 中断的新状态。该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void SDIO_CEATAITCmd(FunctionalState NewState)
{ 
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) CMD_NIEN_BB = (uint32_t)((~((uint32_t)NewState)) & ((uint32_t)0x1));
}

/**
  * @brief  发送 CE-ATA 命令（CMD61）。
  * @param  NewState: CE-ATA 命令的新状态。该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void SDIO_SendCEATACmd(FunctionalState NewState)
{ 
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) CMD_ATACMD_BB = (uint32_t)NewState;
}

/**
  * @brief  检查指定的 SDIO 标志是否置位。
  * @param  SDIO_FLAG: 指定要检查的标志。
  *   该参数可取以下值之一：
  *     @arg SDIO_FLAG_CCRCFAIL: 收到命令响应（CRC 校验失败）
  *     @arg SDIO_FLAG_DCRCFAIL: 数据块已发送/接收（CRC 校验失败）
  *     @arg SDIO_FLAG_CTIMEOUT: 命令响应超时
  *     @arg SDIO_FLAG_DTIMEOUT: 数据超时
  *     @arg SDIO_FLAG_TXUNDERR: 发送 FIFO 下溢错误
  *     @arg SDIO_FLAG_RXOVERR:  接收 FIFO 上溢错误
  *     @arg SDIO_FLAG_CMDREND:  收到命令响应（CRC 校验通过）
  *     @arg SDIO_FLAG_CMDSENT:  命令已发送（无需响应）
  *     @arg SDIO_FLAG_DATAEND:  数据传输结束（数据计数器 SDIDCOUNT 为零）
  *     @arg SDIO_FLAG_STBITERR: 宽总线模式下，并非所有数据信号上
  *                              都检测到起始位
  *     @arg SDIO_FLAG_DBCKEND:  数据块已发送/接收（CRC 校验通过）
  *     @arg SDIO_FLAG_CMDACT:   命令传输进行中
  *     @arg SDIO_FLAG_TXACT:    数据发送进行中
  *     @arg SDIO_FLAG_RXACT:    数据接收进行中
  *     @arg SDIO_FLAG_TXFIFOHE: 发送 FIFO 半空
  *     @arg SDIO_FLAG_RXFIFOHF: 接收 FIFO 半满
  *     @arg SDIO_FLAG_TXFIFOF:  发送 FIFO 满
  *     @arg SDIO_FLAG_RXFIFOF:  接收 FIFO 满
  *     @arg SDIO_FLAG_TXFIFOE:  发送 FIFO 空
  *     @arg SDIO_FLAG_RXFIFOE:  接收 FIFO 空
  *     @arg SDIO_FLAG_TXDAVL:   发送 FIFO 中有可用数据
  *     @arg SDIO_FLAG_RXDAVL:   接收 FIFO 中有可用数据
  *     @arg SDIO_FLAG_SDIOIT:   收到 SD I/O 中断
  *     @arg SDIO_FLAG_CEATAEND: 收到 CMD61 的 CE-ATA 命令完成信号
  * @retval SDIO_FLAG 的新状态（SET 或 RESET）。
  */
FlagStatus SDIO_GetFlagStatus(uint32_t SDIO_FLAG)
{ 
  FlagStatus bitstatus = RESET;
  
  /* 检查参数 */
  assert_param(IS_SDIO_FLAG(SDIO_FLAG));
  
  if ((SDIO->STA & SDIO_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  清零 SDIO 的挂起标志。
  * @param  SDIO_FLAG: 指定要清零的标志。
  *   该参数可取以下值之一或其组合：
  *     @arg SDIO_FLAG_CCRCFAIL: 收到命令响应（CRC 校验失败）
  *     @arg SDIO_FLAG_DCRCFAIL: 数据块已发送/接收（CRC 校验失败）
  *     @arg SDIO_FLAG_CTIMEOUT: 命令响应超时
  *     @arg SDIO_FLAG_DTIMEOUT: 数据超时
  *     @arg SDIO_FLAG_TXUNDERR: 发送 FIFO 下溢错误
  *     @arg SDIO_FLAG_RXOVERR:  接收 FIFO 上溢错误
  *     @arg SDIO_FLAG_CMDREND:  收到命令响应（CRC 校验通过）
  *     @arg SDIO_FLAG_CMDSENT:  命令已发送（无需响应）
  *     @arg SDIO_FLAG_DATAEND:  数据传输结束（数据计数器 SDIDCOUNT 为零）
  *     @arg SDIO_FLAG_STBITERR: 宽总线模式下，并非所有数据信号上
  *                              都检测到起始位
  *     @arg SDIO_FLAG_DBCKEND:  数据块已发送/接收（CRC 校验通过）
  *     @arg SDIO_FLAG_SDIOIT:   收到 SD I/O 中断
  *     @arg SDIO_FLAG_CEATAEND: 收到 CMD61 的 CE-ATA 命令完成信号
  * @retval None
  */
void SDIO_ClearFlag(uint32_t SDIO_FLAG)
{ 
  /* 检查参数 */
  assert_param(IS_SDIO_CLEAR_FLAG(SDIO_FLAG));
   
  SDIO->ICR = SDIO_FLAG;
}

/**
  * @brief  检查指定的 SDIO 中断是否发生。
  * @param  SDIO_IT: 指定要检查的 SDIO 中断源。
  *   该参数可取以下值之一：
  *     @arg SDIO_IT_CCRCFAIL: 收到命令响应（CRC 校验失败）中断
  *     @arg SDIO_IT_DCRCFAIL: 数据块已发送/接收（CRC 校验失败）中断
  *     @arg SDIO_IT_CTIMEOUT: 命令响应超时中断
  *     @arg SDIO_IT_DTIMEOUT: 数据超时中断
  *     @arg SDIO_IT_TXUNDERR: 发送 FIFO 下溢错误中断
  *     @arg SDIO_IT_RXOVERR:  接收 FIFO 上溢错误中断
  *     @arg SDIO_IT_CMDREND:  收到命令响应（CRC 校验通过）中断
  *     @arg SDIO_IT_CMDSENT:  命令已发送（无需响应）中断
  *     @arg SDIO_IT_DATAEND:  数据传输结束（数据计数器 SDIDCOUNT 为零）中断
  *     @arg SDIO_IT_STBITERR: 宽总线模式下，并非所有数据信号上
  *                            都检测到起始位中断
  *     @arg SDIO_IT_DBCKEND:  数据块已发送/接收（CRC 校验通过）中断
  *     @arg SDIO_IT_CMDACT:   命令传输进行中中断
  *     @arg SDIO_IT_TXACT:    数据发送进行中中断
  *     @arg SDIO_IT_RXACT:    数据接收进行中中断
  *     @arg SDIO_IT_TXFIFOHE: 发送 FIFO 半空中断
  *     @arg SDIO_IT_RXFIFOHF: 接收 FIFO 半满中断
  *     @arg SDIO_IT_TXFIFOF:  发送 FIFO 满中断
  *     @arg SDIO_IT_RXFIFOF:  接收 FIFO 满中断
  *     @arg SDIO_IT_TXFIFOE:  发送 FIFO 空中断
  *     @arg SDIO_IT_RXFIFOE:  接收 FIFO 空中断
  *     @arg SDIO_IT_TXDAVL:   发送 FIFO 中有可用数据中断
  *     @arg SDIO_IT_RXDAVL:   接收 FIFO 中有可用数据中断
  *     @arg SDIO_IT_SDIOIT:   收到 SD I/O 中断
  *     @arg SDIO_IT_CEATAEND: 收到 CMD61 的 CE-ATA 命令完成信号中断
  * @retval SDIO_IT 的新状态（SET 或 RESET）。
  */
ITStatus SDIO_GetITStatus(uint32_t SDIO_IT)
{ 
  ITStatus bitstatus = RESET;
  
  /* 检查参数 */
  assert_param(IS_SDIO_GET_IT(SDIO_IT));
  if ((SDIO->STA & SDIO_IT) != (uint32_t)RESET)  
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  清零 SDIO 的中断挂起位。
  * @param  SDIO_IT: 指定要清零的中断挂起位。
  *   该参数可取以下值之一或其组合：
  *     @arg SDIO_IT_CCRCFAIL: 收到命令响应（CRC 校验失败）中断
  *     @arg SDIO_IT_DCRCFAIL: 数据块已发送/接收（CRC 校验失败）中断
  *     @arg SDIO_IT_CTIMEOUT: 命令响应超时中断
  *     @arg SDIO_IT_DTIMEOUT: 数据超时中断
  *     @arg SDIO_IT_TXUNDERR: 发送 FIFO 下溢错误中断
  *     @arg SDIO_IT_RXOVERR:  接收 FIFO 上溢错误中断
  *     @arg SDIO_IT_CMDREND:  收到命令响应（CRC 校验通过）中断
  *     @arg SDIO_IT_CMDSENT:  命令已发送（无需响应）中断
  *     @arg SDIO_IT_DATAEND:  数据传输结束（数据计数器 SDIDCOUNT 为零）中断
  *     @arg SDIO_IT_STBITERR: 宽总线模式下，并非所有数据信号上
  *                            都检测到起始位中断
  *     @arg SDIO_IT_SDIOIT:   收到 SD I/O 中断
  *     @arg SDIO_IT_CEATAEND: 收到 CMD61 的 CE-ATA 命令完成信号
  * @retval None
  */
void SDIO_ClearITPendingBit(uint32_t SDIO_IT)
{ 
  /* 检查参数 */
  assert_param(IS_SDIO_CLEAR_IT(SDIO_IT));
   
  SDIO->ICR = SDIO_IT;
}

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

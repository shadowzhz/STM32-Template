/**
  ******************************************************************************
  * @file    stm32f10x_fsmc.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供所有 FSMC 固件函数。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供指引之用，旨在为客户提供有关其产品的编码信息，
  * 以便客户节省时间。因此，对于因本固件内容及/或客户将本文
  * 所含编码信息用于其产品而产生的任何索赔所导致的任何直接、
  * 间接或后果性损害，STMicroelectronics 概不负责。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_fsmc.h"
#include "stm32f10x_rcc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup FSMC
  * @brief FSMC 驱动模块
  * @{
  */ 

/** @defgroup FSMC_Private_TypesDefinitions   FSMC 私有类型定义
  * @{
  */ 
/**
  * @}
  */

/** @defgroup FSMC_Private_Defines   FSMC 私有宏定义
  * @{
  */

/* --------------------- FSMC 寄存器位掩码 ---------------------------- */

/* FSMC BCRx 掩码 */
#define BCR_MBKEN_Set                       ((uint32_t)0x00000001)
#define BCR_MBKEN_Reset                     ((uint32_t)0x000FFFFE)
#define BCR_FACCEN_Set                      ((uint32_t)0x00000040)

/* FSMC PCRx 掩码 */
#define PCR_PBKEN_Set                       ((uint32_t)0x00000004)
#define PCR_PBKEN_Reset                     ((uint32_t)0x000FFFFB)
#define PCR_ECCEN_Set                       ((uint32_t)0x00000040)
#define PCR_ECCEN_Reset                     ((uint32_t)0x000FFFBF)
#define PCR_MemoryType_NAND                 ((uint32_t)0x00000008)
/**
  * @}
  */

/** @defgroup FSMC_Private_Macros   FSMC 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup FSMC_Private_Variables   FSMC 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup FSMC_Private_FunctionPrototypes   FSMC 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup FSMC_Private_Functions   FSMC 私有函数
  * @{
  */

/**
  * @brief  将 FSMC NOR/SRAM Bank 寄存器反初始化为其默认
  *         复位值。
  * @param  FSMC_Bank: 指定要使用的 FSMC Bank
  *   该参数可取以下值之一：
  *     @arg FSMC_Bank1_NORSRAM1: FSMC Bank1 NOR/SRAM1
  *     @arg FSMC_Bank1_NORSRAM2: FSMC Bank1 NOR/SRAM2
  *     @arg FSMC_Bank1_NORSRAM3: FSMC Bank1 NOR/SRAM3
  *     @arg FSMC_Bank1_NORSRAM4: FSMC Bank1 NOR/SRAM4
  * @retval 无
  */
void FSMC_NORSRAMDeInit(uint32_t FSMC_Bank)
{
  /* 检查参数 */
  assert_param(IS_FSMC_NORSRAM_BANK(FSMC_Bank));
  
  /* FSMC_Bank1_NORSRAM1 */
  if(FSMC_Bank == FSMC_Bank1_NORSRAM1)
  {
    FSMC_Bank1->BTCR[FSMC_Bank] = 0x000030DB;    
  }
  /* FSMC_Bank1_NORSRAM2、FSMC_Bank1_NORSRAM3 或 FSMC_Bank1_NORSRAM4 */
  else
  {   
    FSMC_Bank1->BTCR[FSMC_Bank] = 0x000030D2; 
  }
  FSMC_Bank1->BTCR[FSMC_Bank + 1] = 0x0FFFFFFF;
  FSMC_Bank1E->BWTR[FSMC_Bank] = 0x0FFFFFFF;  
}

/**
  * @brief  将 FSMC NAND Bank 寄存器反初始化为其默认复位值。
  * @param  FSMC_Bank: 指定要使用的 FSMC Bank
  *   该参数可取以下值之一：
  *     @arg FSMC_Bank2_NAND: FSMC Bank2 NAND
  *     @arg FSMC_Bank3_NAND: FSMC Bank3 NAND
  * @retval 无
  */
void FSMC_NANDDeInit(uint32_t FSMC_Bank)
{
  /* 检查参数 */
  assert_param(IS_FSMC_NAND_BANK(FSMC_Bank));
  
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    /* 将 FSMC_Bank2 寄存器设置为复位值 */
    FSMC_Bank2->PCR2 = 0x00000018;
    FSMC_Bank2->SR2 = 0x00000040;
    FSMC_Bank2->PMEM2 = 0xFCFCFCFC;
    FSMC_Bank2->PATT2 = 0xFCFCFCFC;  
  }
  /* FSMC_Bank3_NAND */  
  else
  {
    /* 将 FSMC_Bank3 寄存器设置为复位值 */
    FSMC_Bank3->PCR3 = 0x00000018;
    FSMC_Bank3->SR3 = 0x00000040;
    FSMC_Bank3->PMEM3 = 0xFCFCFCFC;
    FSMC_Bank3->PATT3 = 0xFCFCFCFC; 
  }  
}

/**
  * @brief  将 FSMC PCCARD Bank 寄存器反初始化为其默认复位值。
  * @param  无
  * @retval 无
  */
void FSMC_PCCARDDeInit(void)
{
  /* 将 FSMC_Bank4 寄存器设置为复位值 */
  FSMC_Bank4->PCR4 = 0x00000018; 
  FSMC_Bank4->SR4 = 0x00000000;	
  FSMC_Bank4->PMEM4 = 0xFCFCFCFC;
  FSMC_Bank4->PATT4 = 0xFCFCFCFC;
  FSMC_Bank4->PIO4 = 0xFCFCFCFC;
}

/**
  * @brief  根据 FSMC_NORSRAMInitStruct 中指定的参数初始化
  *         FSMC NOR/SRAM Bank。
  * @param  FSMC_NORSRAMInitStruct : 指向 FSMC_NORSRAMInitTypeDef
  *         结构的指针，该结构包含指定 FSMC NOR/SRAM
  *        Bank 的配置信息。
  * @retval 无
  */
void FSMC_NORSRAMInit(FSMC_NORSRAMInitTypeDef* FSMC_NORSRAMInitStruct)
{ 
  /* 检查参数 */
  assert_param(IS_FSMC_NORSRAM_BANK(FSMC_NORSRAMInitStruct->FSMC_Bank));
  assert_param(IS_FSMC_MUX(FSMC_NORSRAMInitStruct->FSMC_DataAddressMux));
  assert_param(IS_FSMC_MEMORY(FSMC_NORSRAMInitStruct->FSMC_MemoryType));
  assert_param(IS_FSMC_MEMORY_WIDTH(FSMC_NORSRAMInitStruct->FSMC_MemoryDataWidth));
  assert_param(IS_FSMC_BURSTMODE(FSMC_NORSRAMInitStruct->FSMC_BurstAccessMode));
  assert_param(IS_FSMC_ASYNWAIT(FSMC_NORSRAMInitStruct->FSMC_AsynchronousWait));
  assert_param(IS_FSMC_WAIT_POLARITY(FSMC_NORSRAMInitStruct->FSMC_WaitSignalPolarity));
  assert_param(IS_FSMC_WRAP_MODE(FSMC_NORSRAMInitStruct->FSMC_WrapMode));
  assert_param(IS_FSMC_WAIT_SIGNAL_ACTIVE(FSMC_NORSRAMInitStruct->FSMC_WaitSignalActive));
  assert_param(IS_FSMC_WRITE_OPERATION(FSMC_NORSRAMInitStruct->FSMC_WriteOperation));
  assert_param(IS_FSMC_WAITE_SIGNAL(FSMC_NORSRAMInitStruct->FSMC_WaitSignal));
  assert_param(IS_FSMC_EXTENDED_MODE(FSMC_NORSRAMInitStruct->FSMC_ExtendedMode));
  assert_param(IS_FSMC_WRITE_BURST(FSMC_NORSRAMInitStruct->FSMC_WriteBurst));  
  assert_param(IS_FSMC_ADDRESS_SETUP_TIME(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressSetupTime));
  assert_param(IS_FSMC_ADDRESS_HOLD_TIME(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressHoldTime));
  assert_param(IS_FSMC_DATASETUP_TIME(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataSetupTime));
  assert_param(IS_FSMC_TURNAROUND_TIME(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_BusTurnAroundDuration));
  assert_param(IS_FSMC_CLK_DIV(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_CLKDivision));
  assert_param(IS_FSMC_DATA_LATENCY(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataLatency));
  assert_param(IS_FSMC_ACCESS_MODE(FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AccessMode)); 
  
  /* Bank1 NOR/SRAM 控制寄存器配置 */ 
  FSMC_Bank1->BTCR[FSMC_NORSRAMInitStruct->FSMC_Bank] = 
            (uint32_t)FSMC_NORSRAMInitStruct->FSMC_DataAddressMux |
            FSMC_NORSRAMInitStruct->FSMC_MemoryType |
            FSMC_NORSRAMInitStruct->FSMC_MemoryDataWidth |
            FSMC_NORSRAMInitStruct->FSMC_BurstAccessMode |
            FSMC_NORSRAMInitStruct->FSMC_AsynchronousWait |
            FSMC_NORSRAMInitStruct->FSMC_WaitSignalPolarity |
            FSMC_NORSRAMInitStruct->FSMC_WrapMode |
            FSMC_NORSRAMInitStruct->FSMC_WaitSignalActive |
            FSMC_NORSRAMInitStruct->FSMC_WriteOperation |
            FSMC_NORSRAMInitStruct->FSMC_WaitSignal |
            FSMC_NORSRAMInitStruct->FSMC_ExtendedMode |
            FSMC_NORSRAMInitStruct->FSMC_WriteBurst;

  if(FSMC_NORSRAMInitStruct->FSMC_MemoryType == FSMC_MemoryType_NOR)
  {
    FSMC_Bank1->BTCR[FSMC_NORSRAMInitStruct->FSMC_Bank] |= (uint32_t)BCR_FACCEN_Set;
  }
  
  /* Bank1 NOR/SRAM 时序寄存器配置 */
  FSMC_Bank1->BTCR[FSMC_NORSRAMInitStruct->FSMC_Bank+1] = 
            (uint32_t)FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressSetupTime |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressHoldTime << 4) |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataSetupTime << 8) |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_BusTurnAroundDuration << 16) |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_CLKDivision << 20) |
            (FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataLatency << 24) |
             FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AccessMode;
            
    
  /* 若使用扩展模式，为写操作配置 Bank1 NOR/SRAM 时序寄存器 */
  if(FSMC_NORSRAMInitStruct->FSMC_ExtendedMode == FSMC_ExtendedMode_Enable)
  {
    assert_param(IS_FSMC_ADDRESS_SETUP_TIME(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressSetupTime));
    assert_param(IS_FSMC_ADDRESS_HOLD_TIME(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressHoldTime));
    assert_param(IS_FSMC_DATASETUP_TIME(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataSetupTime));
    assert_param(IS_FSMC_CLK_DIV(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_CLKDivision));
    assert_param(IS_FSMC_DATA_LATENCY(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataLatency));
    assert_param(IS_FSMC_ACCESS_MODE(FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AccessMode));
    FSMC_Bank1E->BWTR[FSMC_NORSRAMInitStruct->FSMC_Bank] = 
              (uint32_t)FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressSetupTime |
              (FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressHoldTime << 4 )|
              (FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataSetupTime << 8) |
              (FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_CLKDivision << 20) |
              (FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataLatency << 24) |
               FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AccessMode;
  }
  else
  {
    FSMC_Bank1E->BWTR[FSMC_NORSRAMInitStruct->FSMC_Bank] = 0x0FFFFFFF;
  }
}

/**
  * @brief  根据 FSMC_NANDInitStruct 中指定的参数初始化
  *         FSMC NAND Bank。
  * @param  FSMC_NANDInitStruct : 指向 FSMC_NANDInitTypeDef
  *         结构的指针，该结构包含指定 FSMC
  *         NAND Bank 的配置信息。
  * @retval 无
  */
void FSMC_NANDInit(FSMC_NANDInitTypeDef* FSMC_NANDInitStruct)
{
  uint32_t tmppcr = 0x00000000, tmppmem = 0x00000000, tmppatt = 0x00000000; 
    
  /* 检查参数 */
  assert_param( IS_FSMC_NAND_BANK(FSMC_NANDInitStruct->FSMC_Bank));
  assert_param( IS_FSMC_WAIT_FEATURE(FSMC_NANDInitStruct->FSMC_Waitfeature));
  assert_param( IS_FSMC_MEMORY_WIDTH(FSMC_NANDInitStruct->FSMC_MemoryDataWidth));
  assert_param( IS_FSMC_ECC_STATE(FSMC_NANDInitStruct->FSMC_ECC));
  assert_param( IS_FSMC_ECCPAGE_SIZE(FSMC_NANDInitStruct->FSMC_ECCPageSize));
  assert_param( IS_FSMC_TCLR_TIME(FSMC_NANDInitStruct->FSMC_TCLRSetupTime));
  assert_param( IS_FSMC_TAR_TIME(FSMC_NANDInitStruct->FSMC_TARSetupTime));
  assert_param(IS_FSMC_SETUP_TIME(FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime));
  assert_param(IS_FSMC_WAIT_TIME(FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime));
  assert_param(IS_FSMC_HOLD_TIME(FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime));
  assert_param(IS_FSMC_HIZ_TIME(FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime));
  assert_param(IS_FSMC_SETUP_TIME(FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime));
  assert_param(IS_FSMC_WAIT_TIME(FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime));
  assert_param(IS_FSMC_HOLD_TIME(FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime));
  assert_param(IS_FSMC_HIZ_TIME(FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime));
  
  /* 根据 FSMC_NANDInitStruct 参数设置 tmppcr 值 */
  tmppcr = (uint32_t)FSMC_NANDInitStruct->FSMC_Waitfeature |
            PCR_MemoryType_NAND |
            FSMC_NANDInitStruct->FSMC_MemoryDataWidth |
            FSMC_NANDInitStruct->FSMC_ECC |
            FSMC_NANDInitStruct->FSMC_ECCPageSize |
            (FSMC_NANDInitStruct->FSMC_TCLRSetupTime << 9 )|
            (FSMC_NANDInitStruct->FSMC_TARSetupTime << 13);
            
  /* 根据 FSMC_CommonSpaceTimingStructure 参数设置 tmppmem 值 */
  tmppmem = (uint32_t)FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime |
            (FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
            (FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
            (FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime << 24); 
            
  /* 根据 FSMC_AttributeSpaceTimingStructure 参数设置 tmppatt 值 */
  tmppatt = (uint32_t)FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime |
            (FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
            (FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
            (FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime << 24);
  
  if(FSMC_NANDInitStruct->FSMC_Bank == FSMC_Bank2_NAND)
  {
    /* FSMC_Bank2_NAND 寄存器配置 */
    FSMC_Bank2->PCR2 = tmppcr;
    FSMC_Bank2->PMEM2 = tmppmem;
    FSMC_Bank2->PATT2 = tmppatt;
  }
  else
  {
    /* FSMC_Bank3_NAND 寄存器配置 */
    FSMC_Bank3->PCR3 = tmppcr;
    FSMC_Bank3->PMEM3 = tmppmem;
    FSMC_Bank3->PATT3 = tmppatt;
  }
}

/**
  * @brief  根据 FSMC_PCCARDInitStruct 中指定的参数初始化
  *         FSMC PCCARD Bank。
  * @param  FSMC_PCCARDInitStruct : 指向 FSMC_PCCARDInitTypeDef
  *         结构的指针，该结构包含 FSMC
  *         PCCARD Bank 的配置信息。
  * @retval 无
  */
void FSMC_PCCARDInit(FSMC_PCCARDInitTypeDef* FSMC_PCCARDInitStruct)
{
  /* 检查参数 */
  assert_param(IS_FSMC_WAIT_FEATURE(FSMC_PCCARDInitStruct->FSMC_Waitfeature));
  assert_param(IS_FSMC_TCLR_TIME(FSMC_PCCARDInitStruct->FSMC_TCLRSetupTime));
  assert_param(IS_FSMC_TAR_TIME(FSMC_PCCARDInitStruct->FSMC_TARSetupTime));
 
  assert_param(IS_FSMC_SETUP_TIME(FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime));
  assert_param(IS_FSMC_WAIT_TIME(FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime));
  assert_param(IS_FSMC_HOLD_TIME(FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime));
  assert_param(IS_FSMC_HIZ_TIME(FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime));
  
  assert_param(IS_FSMC_SETUP_TIME(FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime));
  assert_param(IS_FSMC_WAIT_TIME(FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime));
  assert_param(IS_FSMC_HOLD_TIME(FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime));
  assert_param(IS_FSMC_HIZ_TIME(FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime));
  assert_param(IS_FSMC_SETUP_TIME(FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_SetupTime));
  assert_param(IS_FSMC_WAIT_TIME(FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_WaitSetupTime));
  assert_param(IS_FSMC_HOLD_TIME(FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HoldSetupTime));
  assert_param(IS_FSMC_HIZ_TIME(FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HiZSetupTime));
  
  /* 根据 FSMC_PCCARDInitStruct 参数设置 PCR4 寄存器值 */
  FSMC_Bank4->PCR4 = (uint32_t)FSMC_PCCARDInitStruct->FSMC_Waitfeature |
                     FSMC_MemoryDataWidth_16b |  
                     (FSMC_PCCARDInitStruct->FSMC_TCLRSetupTime << 9) |
                     (FSMC_PCCARDInitStruct->FSMC_TARSetupTime << 13);
            
  /* 根据 FSMC_CommonSpaceTimingStructure 参数设置 PMEM4 寄存器值 */
  FSMC_Bank4->PMEM4 = (uint32_t)FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime |
                      (FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
                      (FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
                      (FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime << 24); 
            
  /* 根据 FSMC_AttributeSpaceTimingStructure 参数设置 PATT4 寄存器值 */
  FSMC_Bank4->PATT4 = (uint32_t)FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime |
                      (FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
                      (FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
                      (FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime << 24);	
            
  /* 根据 FSMC_IOSpaceTimingStructure 参数设置 PIO4 寄存器值 */
  FSMC_Bank4->PIO4 = (uint32_t)FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_SetupTime |
                     (FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_WaitSetupTime << 8) |
                     (FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HoldSetupTime << 16)|
                     (FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HiZSetupTime << 24);             
}

/**
  * @brief  将 FSMC_NORSRAMInitStruct 的每个成员填充为默认值。
  * @param  FSMC_NORSRAMInitStruct: 指向将被初始化的
  *         FSMC_NORSRAMInitTypeDef 结构的指针。
  * @retval 无
  */
void FSMC_NORSRAMStructInit(FSMC_NORSRAMInitTypeDef* FSMC_NORSRAMInitStruct)
{  
  /* 复位 NOR/SRAM 初始化结构的参数值 */
  FSMC_NORSRAMInitStruct->FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStruct->FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
  FSMC_NORSRAMInitStruct->FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStruct->FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
  FSMC_NORSRAMInitStruct->FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStruct->FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStruct->FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStruct->FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStruct->FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStruct->FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStruct->FSMC_WaitSignal = FSMC_WaitSignal_Enable;
  FSMC_NORSRAMInitStruct->FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStruct->FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressSetupTime = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AddressHoldTime = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataSetupTime = 0xFF;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_BusTurnAroundDuration = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_CLKDivision = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_DataLatency = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_ReadWriteTimingStruct->FSMC_AccessMode = FSMC_AccessMode_A; 
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressSetupTime = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AddressHoldTime = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataSetupTime = 0xFF;
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_BusTurnAroundDuration = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_CLKDivision = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_DataLatency = 0xF;
  FSMC_NORSRAMInitStruct->FSMC_WriteTimingStruct->FSMC_AccessMode = FSMC_AccessMode_A;
}

/**
  * @brief  将 FSMC_NANDInitStruct 的每个成员填充为默认值。
  * @param  FSMC_NANDInitStruct: 指向将被初始化的
  *         FSMC_NANDInitTypeDef 结构的指针。
  * @retval 无
  */
void FSMC_NANDStructInit(FSMC_NANDInitTypeDef* FSMC_NANDInitStruct)
{ 
  /* 复位 NAND 初始化结构的参数值 */
  FSMC_NANDInitStruct->FSMC_Bank = FSMC_Bank2_NAND;
  FSMC_NANDInitStruct->FSMC_Waitfeature = FSMC_Waitfeature_Disable;
  FSMC_NANDInitStruct->FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
  FSMC_NANDInitStruct->FSMC_ECC = FSMC_ECC_Disable;
  FSMC_NANDInitStruct->FSMC_ECCPageSize = FSMC_ECCPageSize_256Bytes;
  FSMC_NANDInitStruct->FSMC_TCLRSetupTime = 0x0;
  FSMC_NANDInitStruct->FSMC_TARSetupTime = 0x0;
  FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime = 0xFC;
  FSMC_NANDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime = 0xFC;	  
}

/**
  * @brief  将 FSMC_PCCARDInitStruct 的每个成员填充为默认值。
  * @param  FSMC_PCCARDInitStruct: 指向将被初始化的
  *         FSMC_PCCARDInitTypeDef 结构的指针。
  * @retval 无
  */
void FSMC_PCCARDStructInit(FSMC_PCCARDInitTypeDef* FSMC_PCCARDInitStruct)
{
  /* 复位 PCCARD 初始化结构的参数值 */
  FSMC_PCCARDInitStruct->FSMC_Waitfeature = FSMC_Waitfeature_Disable;
  FSMC_PCCARDInitStruct->FSMC_TCLRSetupTime = 0x0;
  FSMC_PCCARDInitStruct->FSMC_TARSetupTime = 0x0;
  FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_SetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_WaitSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HoldSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_CommonSpaceTimingStruct->FSMC_HiZSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_SetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_WaitSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HoldSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_AttributeSpaceTimingStruct->FSMC_HiZSetupTime = 0xFC;	
  FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_SetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_WaitSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HoldSetupTime = 0xFC;
  FSMC_PCCARDInitStruct->FSMC_IOSpaceTimingStruct->FSMC_HiZSetupTime = 0xFC;
}

/**
  * @brief  使能或关闭指定的 NOR/SRAM 存储块。
  * @param  FSMC_Bank: 指定要使用的 FSMC Bank
  *   该参数可取以下值之一：
  *     @arg FSMC_Bank1_NORSRAM1: FSMC Bank1 NOR/SRAM1
  *     @arg FSMC_Bank1_NORSRAM2: FSMC Bank1 NOR/SRAM2
  *     @arg FSMC_Bank1_NORSRAM3: FSMC Bank1 NOR/SRAM3
  *     @arg FSMC_Bank1_NORSRAM4: FSMC Bank1 NOR/SRAM4
  * @param  NewState: FSMC_Bank 的新状态。该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void FSMC_NORSRAMCmd(uint32_t FSMC_Bank, FunctionalState NewState)
{
  assert_param(IS_FSMC_NORSRAM_BANK(FSMC_Bank));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 通过置位 BCRx 寄存器中的 PBKEN 位使能所选 NOR/SRAM Bank */
    FSMC_Bank1->BTCR[FSMC_Bank] |= BCR_MBKEN_Set;
  }
  else
  {
    /* 通过清零 BCRx 寄存器中的 PBKEN 位关闭所选 NOR/SRAM Bank */
    FSMC_Bank1->BTCR[FSMC_Bank] &= BCR_MBKEN_Reset;
  }
}

/**
  * @brief  使能或关闭指定的 NAND 存储块。
  * @param  FSMC_Bank: 指定要使用的 FSMC Bank
  *   该参数可取以下值之一：
  *     @arg FSMC_Bank2_NAND: FSMC Bank2 NAND
  *     @arg FSMC_Bank3_NAND: FSMC Bank3 NAND
  * @param  NewState: FSMC_Bank 的新状态。该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void FSMC_NANDCmd(uint32_t FSMC_Bank, FunctionalState NewState)
{
  assert_param(IS_FSMC_NAND_BANK(FSMC_Bank));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 通过置位 PCRx 寄存器中的 PBKEN 位使能所选 NAND Bank */
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      FSMC_Bank2->PCR2 |= PCR_PBKEN_Set;
    }
    else
    {
      FSMC_Bank3->PCR3 |= PCR_PBKEN_Set;
    }
  }
  else
  {
    /* 通过清零 PCRx 寄存器中的 PBKEN 位关闭所选 NAND Bank */
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      FSMC_Bank2->PCR2 &= PCR_PBKEN_Reset;
    }
    else
    {
      FSMC_Bank3->PCR3 &= PCR_PBKEN_Reset;
    }
  }
}

/**
  * @brief  使能或关闭 PCCARD 存储块。
  * @param  NewState: PCCARD 存储块的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void FSMC_PCCARDCmd(FunctionalState NewState)
{
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 通过置位 PCR4 寄存器中的 PBKEN 位使能 PCCARD Bank */
    FSMC_Bank4->PCR4 |= PCR_PBKEN_Set;
  }
  else
  {
    /* 通过清零 PCR4 寄存器中的 PBKEN 位关闭 PCCARD Bank */
    FSMC_Bank4->PCR4 &= PCR_PBKEN_Reset;
  }
}

/**
  * @brief  使能或关闭 FSMC NAND ECC 功能。
  * @param  FSMC_Bank: 指定要使用的 FSMC Bank
  *   该参数可取以下值之一：
  *     @arg FSMC_Bank2_NAND: FSMC Bank2 NAND
  *     @arg FSMC_Bank3_NAND: FSMC Bank3 NAND
  * @param  NewState: FSMC NAND ECC 功能的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void FSMC_NANDECCCmd(uint32_t FSMC_Bank, FunctionalState NewState)
{
  assert_param(IS_FSMC_NAND_BANK(FSMC_Bank));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 通过置位 PCRx 寄存器中的 ECCEN 位使能所选 NAND Bank ECC 功能 */
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      FSMC_Bank2->PCR2 |= PCR_ECCEN_Set;
    }
    else
    {
      FSMC_Bank3->PCR3 |= PCR_ECCEN_Set;
    }
  }
  else
  {
    /* 通过清零 PCRx 寄存器中的 ECCEN 位关闭所选 NAND Bank ECC 功能 */
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      FSMC_Bank2->PCR2 &= PCR_ECCEN_Reset;
    }
    else
    {
      FSMC_Bank3->PCR3 &= PCR_ECCEN_Reset;
    }
  }
}

/**
  * @brief  返回纠错码寄存器值。
  * @param  FSMC_Bank: 指定要使用的 FSMC Bank
  *   该参数可取以下值之一：
  *     @arg FSMC_Bank2_NAND: FSMC Bank2 NAND
  *     @arg FSMC_Bank3_NAND: FSMC Bank3 NAND
  * @retval 纠错码 (ECC) 值。
  */
uint32_t FSMC_GetECC(uint32_t FSMC_Bank)
{
  uint32_t eccval = 0x00000000;
  
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    /* 获取 ECCR2 寄存器值 */
    eccval = FSMC_Bank2->ECCR2;
  }
  else
  {
    /* 获取 ECCR3 寄存器值 */
    eccval = FSMC_Bank3->ECCR3;
  }
  /* 返回纠错码值 */
  return(eccval);
}

/**
  * @brief  使能或关闭指定的 FSMC 中断。
  * @param  FSMC_Bank: 指定要使用的 FSMC Bank
  *   该参数可取以下值之一：
  *     @arg FSMC_Bank2_NAND: FSMC Bank2 NAND
  *     @arg FSMC_Bank3_NAND: FSMC Bank3 NAND
  *     @arg FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
  * @param  FSMC_IT: 指定要使能或关闭的 FSMC 中断源。
  *   该参数可取以下值的任意组合：
  *     @arg FSMC_IT_RisingEdge: 上升沿检测中断。
  *     @arg FSMC_IT_Level: 电平边沿检测中断。
  *     @arg FSMC_IT_FallingEdge: 下降沿检测中断。
  * @param  NewState: 指定 FSMC 中断的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void FSMC_ITConfig(uint32_t FSMC_Bank, uint32_t FSMC_IT, FunctionalState NewState)
{
  assert_param(IS_FSMC_IT_BANK(FSMC_Bank));
  assert_param(IS_FSMC_IT(FSMC_IT));	
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 使能所选的 FSMC_Bank2 中断 */
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      FSMC_Bank2->SR2 |= FSMC_IT;
    }
    /* 使能所选的 FSMC_Bank3 中断 */
    else if (FSMC_Bank == FSMC_Bank3_NAND)
    {
      FSMC_Bank3->SR3 |= FSMC_IT;
    }
    /* 使能所选的 FSMC_Bank4 中断 */
    else
    {
      FSMC_Bank4->SR4 |= FSMC_IT;    
    }
  }
  else
  {
    /* 关闭所选的 FSMC_Bank2 中断 */
    if(FSMC_Bank == FSMC_Bank2_NAND)
    {
      
      FSMC_Bank2->SR2 &= (uint32_t)~FSMC_IT;
    }
    /* 关闭所选的 FSMC_Bank3 中断 */
    else if (FSMC_Bank == FSMC_Bank3_NAND)
    {
      FSMC_Bank3->SR3 &= (uint32_t)~FSMC_IT;
    }
    /* 关闭所选的 FSMC_Bank4 中断 */
    else
    {
      FSMC_Bank4->SR4 &= (uint32_t)~FSMC_IT;    
    }
  }
}

/**
  * @brief  检查指定的 FSMC 标志是否置位。
  * @param  FSMC_Bank: 指定要使用的 FSMC Bank
  *   该参数可取以下值之一：
  *     @arg FSMC_Bank2_NAND: FSMC Bank2 NAND
  *     @arg FSMC_Bank3_NAND: FSMC Bank3 NAND
  *     @arg FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
  * @param  FSMC_FLAG: 指定要检查的标志。
  *   该参数可取以下值之一：
  *     @arg FSMC_FLAG_RisingEdge: 上升沿检测标志。
  *     @arg FSMC_FLAG_Level: 电平检测标志。
  *     @arg FSMC_FLAG_FallingEdge: 下降沿检测标志。
  *     @arg FSMC_FLAG_FEMPT: FIFO 空标志。
  * @retval FSMC_FLAG 的新状态（SET 或 RESET）。
  */
FlagStatus FSMC_GetFlagStatus(uint32_t FSMC_Bank, uint32_t FSMC_FLAG)
{
  FlagStatus bitstatus = RESET;
  uint32_t tmpsr = 0x00000000;
  
  /* 检查参数 */
  assert_param(IS_FSMC_GETFLAG_BANK(FSMC_Bank));
  assert_param(IS_FSMC_GET_FLAG(FSMC_FLAG));
  
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    tmpsr = FSMC_Bank2->SR2;
  }  
  else if(FSMC_Bank == FSMC_Bank3_NAND)
  {
    tmpsr = FSMC_Bank3->SR3;
  }
  /* FSMC_Bank4_PCCARD*/
  else
  {
    tmpsr = FSMC_Bank4->SR4;
  } 
  
  /* 获取标志状态 */
  if ((tmpsr & FSMC_FLAG) != (uint16_t)RESET )
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* 返回标志状态 */
  return bitstatus;
}

/**
  * @brief  清除 FSMC 的挂起标志。
  * @param  FSMC_Bank: 指定要使用的 FSMC Bank
  *   该参数可取以下值之一：
  *     @arg FSMC_Bank2_NAND: FSMC Bank2 NAND
  *     @arg FSMC_Bank3_NAND: FSMC Bank3 NAND
  *     @arg FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
  * @param  FSMC_FLAG: 指定要清除的标志。
  *   该参数可取以下值的任意组合：
  *     @arg FSMC_FLAG_RisingEdge: 上升沿检测标志。
  *     @arg FSMC_FLAG_Level: 电平检测标志。
  *     @arg FSMC_FLAG_FallingEdge: 下降沿检测标志。
  * @retval 无
  */
void FSMC_ClearFlag(uint32_t FSMC_Bank, uint32_t FSMC_FLAG)
{
 /* 检查参数 */
  assert_param(IS_FSMC_GETFLAG_BANK(FSMC_Bank));
  assert_param(IS_FSMC_CLEAR_FLAG(FSMC_FLAG)) ;
    
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    FSMC_Bank2->SR2 &= ~FSMC_FLAG; 
  }  
  else if(FSMC_Bank == FSMC_Bank3_NAND)
  {
    FSMC_Bank3->SR3 &= ~FSMC_FLAG;
  }
  /* FSMC_Bank4_PCCARD*/
  else
  {
    FSMC_Bank4->SR4 &= ~FSMC_FLAG;
  }
}

/**
  * @brief  检查指定的 FSMC 中断是否发生。
  * @param  FSMC_Bank: 指定要使用的 FSMC Bank
  *   该参数可取以下值之一：
  *     @arg FSMC_Bank2_NAND: FSMC Bank2 NAND
  *     @arg FSMC_Bank3_NAND: FSMC Bank3 NAND
  *     @arg FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
  * @param  FSMC_IT: 指定要检查的 FSMC 中断源。
  *   该参数可取以下值之一：
  *     @arg FSMC_IT_RisingEdge: 上升沿检测中断。
  *     @arg FSMC_IT_Level: 电平边沿检测中断。
  *     @arg FSMC_IT_FallingEdge: 下降沿检测中断。
  * @retval FSMC_IT 的新状态（SET 或 RESET）。
  */
ITStatus FSMC_GetITStatus(uint32_t FSMC_Bank, uint32_t FSMC_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t tmpsr = 0x0, itstatus = 0x0, itenable = 0x0; 
  
  /* 检查参数 */
  assert_param(IS_FSMC_IT_BANK(FSMC_Bank));
  assert_param(IS_FSMC_GET_IT(FSMC_IT));
  
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    tmpsr = FSMC_Bank2->SR2;
  }  
  else if(FSMC_Bank == FSMC_Bank3_NAND)
  {
    tmpsr = FSMC_Bank3->SR3;
  }
  /* FSMC_Bank4_PCCARD*/
  else
  {
    tmpsr = FSMC_Bank4->SR4;
  } 
  
  itstatus = tmpsr & FSMC_IT;
  
  itenable = tmpsr & (FSMC_IT >> 3);
  if ((itstatus != (uint32_t)RESET)  && (itenable != (uint32_t)RESET))
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
  * @brief  清除 FSMC 的中断挂起位。
  * @param  FSMC_Bank: 指定要使用的 FSMC Bank
  *   该参数可取以下值之一：
  *     @arg FSMC_Bank2_NAND: FSMC Bank2 NAND
  *     @arg FSMC_Bank3_NAND: FSMC Bank3 NAND
  *     @arg FSMC_Bank4_PCCARD: FSMC Bank4 PCCARD
  * @param  FSMC_IT: 指定要清除的中断挂起位。
  *   该参数可取以下值的任意组合：
  *     @arg FSMC_IT_RisingEdge: 上升沿检测中断。
  *     @arg FSMC_IT_Level: 电平边沿检测中断。
  *     @arg FSMC_IT_FallingEdge: 下降沿检测中断。
  * @retval 无
  */
void FSMC_ClearITPendingBit(uint32_t FSMC_Bank, uint32_t FSMC_IT)
{
  /* 检查参数 */
  assert_param(IS_FSMC_IT_BANK(FSMC_Bank));
  assert_param(IS_FSMC_IT(FSMC_IT));
    
  if(FSMC_Bank == FSMC_Bank2_NAND)
  {
    FSMC_Bank2->SR2 &= ~(FSMC_IT >> 3); 
  }  
  else if(FSMC_Bank == FSMC_Bank3_NAND)
  {
    FSMC_Bank3->SR3 &= ~(FSMC_IT >> 3);
  }
  /* FSMC_Bank4_PCCARD*/
  else
  {
    FSMC_Bank4->SR4 &= ~(FSMC_IT >> 3);
  }
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

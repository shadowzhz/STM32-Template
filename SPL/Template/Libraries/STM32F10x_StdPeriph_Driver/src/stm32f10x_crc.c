/**
  ******************************************************************************
  * @file    stm32f10x_crc.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供所有 CRC 固件函数。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供参考，旨在为客户提供有关其产品的编码信息，以便客户节省时间。
  * 因此，对于因本固件的内容和/或客户将本文所含编码信息用于其产品
  * 而产生的任何索赔所导致的任何直接、间接或后果性损害，
  * 意法半导体概不承担责任。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_crc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup CRC
  * @brief CRC 驱动模块
  * @{
  */

/** @defgroup CRC_Private_TypesDefinitions   CRC 私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Private_Defines   CRC 私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Private_Macros   CRC 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Private_Variables   CRC 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Private_FunctionPrototypes   CRC 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Private_Functions   CRC 私有函数
  * @{
  */

/**
  * @brief  复位 CRC 数据寄存器 (DR)。
  * @param  无
  * @retval 无
  */
void CRC_ResetDR(void)
{
  /* 复位 CRC 生成器 */
  CRC->CR = CRC_CR_RESET;
}

/**
  * @brief  计算给定数据字 (32 位) 的 32 位 CRC。
  * @param  Data: 要计算其 CRC 的数据字 (32 位)
  * @retval 32 位 CRC
  */
uint32_t CRC_CalcCRC(uint32_t Data)
{
  CRC->DR = Data;
  
  return (CRC->DR);
}

/**
  * @brief  计算给定数据字 (32 位) 缓冲区的 32 位 CRC。
  * @param  pBuffer: 指向包含待计算数据的缓冲区的指针
  * @param  BufferLength: 待计算的缓冲区长度
  * @retval 32 位 CRC
  */
uint32_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength)
{
  uint32_t index = 0;
  
  for(index = 0; index < BufferLength; index++)
  {
    CRC->DR = pBuffer[index];
  }
  return (CRC->DR);
}

/**
  * @brief  返回当前 CRC 值。
  * @param  无
  * @retval 32 位 CRC
  */
uint32_t CRC_GetCRC(void)
{
  return (CRC->DR);
}

/**
  * @brief  在独立数据 (ID) 寄存器中存储 8 位数据。
  * @param  IDValue: 要存储在 ID 寄存器中的 8 位数值
  * @retval 无
  */
void CRC_SetIDRegister(uint8_t IDValue)
{
  CRC->IDR = IDValue;
}

/**
  * @brief  返回存储在独立数据 (ID) 寄存器中的 8 位数据
  * @param  无
  * @retval ID 寄存器的 8 位数值
  */
uint8_t CRC_GetIDRegister(void)
{
  return (CRC->IDR);
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

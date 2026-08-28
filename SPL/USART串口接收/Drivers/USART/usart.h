#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

void USART1_Init(void);

void USART_SendByte(uint8_t Byte);

void USART_SendString(char *String);

#endif

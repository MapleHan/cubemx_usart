/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "stdio.h"
#include "stdarg.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define TX_TIMEOUT  1000
typedef struct
{
	UART_HandleTypeDef* huart_handle;
	uint8_t init;
  uint8_t recv;
  uint8_t* rx_buff;
  uint8_t* tx_buff;
	uint16_t buff_size;
  uint16_t rx_tail;
  uint16_t tx_head;
  volatile uint16_t rx_head;
  volatile uint16_t tx_tail;
}UART_BufferTypeDef;
extern UART_BufferTypeDef uart1_buffer;
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void UART_Buffer_Start(UART_BufferTypeDef *obj,UART_HandleTypeDef* huart);
void UART_Buffer_End(UART_BufferTypeDef *obj);
int UART_Buffer_Peek(UART_BufferTypeDef *obj);
int UART_Buffer_Read(UART_BufferTypeDef *obj);
size_t UART_Buffer_Write(UART_BufferTypeDef *obj,uint8_t c);
size_t UART_Buffer_Write_Arr(UART_BufferTypeDef* obj,const uint8_t *buffer, size_t size);
int UART_Buffer_Available(UART_BufferTypeDef *obj);
int UART_Buffer_Available_For_Write(UART_BufferTypeDef *obj);
void UART_Buffer_Flush(UART_BufferTypeDef *obj);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

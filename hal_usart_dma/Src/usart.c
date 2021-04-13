/**
  ******************************************************************************
  * File Name          : USART.c
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
UART_BufferTypeDef uart1_buffer;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
/**
 * Attempts to determine if the serial peripheral is already in use for RX
 *
 * @param obj The serial object
 * @return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
static uint8_t UART_RX_Active(UART_HandleTypeDef *huart)
{
  return ((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX);
}

/**
 * Attempts to determine if the serial peripheral is already in use for TX
 *
 * @param obj The serial object
 * @return Non-zero if the TX transaction is ongoing, 0 otherwise
 */
static uint8_t UART_TX_Active(UART_HandleTypeDef *huart)
{
  return ((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX);
}

/**
  * @brief  Read receive byte from uart
  * @param  obj : pointer to UART_BufferTypeDef structure
  * @retval last character received
  */
static int UART_Buffer_Rx_Complate(UART_BufferTypeDef *obj)
{
  if (UART_RX_Active(obj->huart_handle)) {
    return -1; /* Transaction ongoing */
  }
  unsigned char c = (unsigned char)(obj->recv);
	uint16_t i = (unsigned int)(obj->rx_head + 1) % obj->rx_buff_size;
	if (i != obj->rx_tail)
	{
		obj->rx_buff[obj->rx_head] = c;
		obj->rx_head = i;
	}
  /* Restart RX irq */
  HAL_UART_Receive_DMA(obj->huart_handle, &(obj->recv), 1);

  return 0;
}

static int UART_Buffer_TX_Complate(UART_BufferTypeDef *obj)
{
	obj->tx_tail = (obj->tx_tail + 1) % obj->tx_buff_size;
  if (obj->tx_head == obj->tx_tail)
	{
    return -1;
  }
	if (HAL_UART_Transmit_DMA(obj->huart_handle, &obj->tx_buff[obj->tx_tail], 1) != HAL_OK)
	{
		return -1;
	}
	return 0;
}
static IRQn_Type UART_Get_IRQ(USART_TypeDef* Instance)
{
	if(Instance==USART1)
	{
		return(USART1_IRQn);
	}
	else if(Instance==USART2)
	{
		return(USART2_IRQn);
	}
	else if(Instance==USART3)
	{
		return(USART3_IRQn);
	}
	else if(Instance==UART4)
	{
		return(UART4_IRQn);
	}
	else if(Instance==UART5)
	{
		return(UART5_IRQn);
	}
	return(0xFF);
}

static void UART_Transmit_DMA_Init(UART_BufferTypeDef *obj)
{
	HAL_NVIC_DisableIRQ(UART_Get_IRQ(obj->huart_handle->Instance));
	HAL_UART_Transmit_DMA(obj->huart_handle, &obj->tx_buff[obj->tx_tail], 1);
	HAL_NVIC_SetPriority(UART_Get_IRQ(obj->huart_handle->Instance), 0, 0);
	HAL_NVIC_EnableIRQ(UART_Get_IRQ(obj->huart_handle->Instance));
}
void UART_Buffer_Start(UART_BufferTypeDef *obj,UART_HandleTypeDef* huart,uint16_t tx_buffer_size,uint16_t rx_buffer_size)
{
		if(obj->init==1)
		{
			return;
		}
		obj->huart_handle = huart;
		obj->tx_buff_size = tx_buffer_size;
		obj->rx_buff_size = rx_buffer_size;
		obj->tx_buff = (uint8_t*)malloc(obj->tx_buff_size*sizeof(uint8_t));
		obj->rx_buff = (uint8_t*)malloc(obj->rx_buff_size*sizeof(uint8_t));
		if((obj->tx_buff==NULL)||(obj->rx_buff==NULL))
		{
			while(1)
				;
		}
		obj->init=1;
		if (UART_RX_Active(obj->huart_handle))
		{
			return;
		}
		MX_USART1_UART_Init();
		/* Must disable interrupt to prevent handle lock contention */
		HAL_NVIC_DisableIRQ(UART_Get_IRQ(obj->huart_handle->Instance));
		HAL_UART_Receive_DMA(obj->huart_handle, &(obj->recv), 1);
		/* USART1 interrupt Init */
		HAL_NVIC_SetPriority(UART_Get_IRQ(obj->huart_handle->Instance), 0, 0);
		HAL_NVIC_EnableIRQ(UART_Get_IRQ(obj->huart_handle->Instance));
}

void UART_Buffer_End(UART_BufferTypeDef *obj)
{
	if(obj->init!=1)
	{
		return;
	}
	UART_Buffer_Flush(obj);
	HAL_UART_DeInit(obj->huart_handle);
	if(obj->tx_buff!=NULL)
		free(obj->tx_buff);
	if(obj->rx_buff!=NULL)
		free(obj->rx_buff);
	obj->init=0;
	obj->tx_buff_size = 0;
	obj->rx_buff_size = 0;
	obj->rx_head = obj->rx_tail = 0;
}

int UART_Buffer_Peek(UART_BufferTypeDef *obj)
{
	if (obj->rx_head == obj->rx_tail) {
    return -1;
  } else {
    return obj->rx_buff[obj->rx_tail];
  }
}
int UART_Buffer_Read(UART_BufferTypeDef *obj)
{
	if (obj->rx_head == obj->rx_tail) {
    return -1;
  } else {
    unsigned char c = obj->rx_buff[obj->rx_tail];
    obj->rx_tail = (uint16_t)(obj->rx_tail + 1) % obj->rx_buff_size;
    return c;
  }
}
size_t UART_Buffer_Write_Arr(UART_BufferTypeDef* obj,const uint8_t *buffer, size_t size)
{
  size_t n = 0;
  while (size--) {
    if (UART_Buffer_Write(obj,*buffer++)) {
      n++;
    } else {
      break;
    }
  }
  return n;
}
size_t UART_Buffer_Write(UART_BufferTypeDef* obj,uint8_t c)
{
  uint16_t i = (obj->tx_head + 1) % obj->tx_buff_size;
  while (i == obj->tx_tail) {
    // nop, the interrupt handler will free up space for us
  }

  obj->tx_buff[obj->tx_head] = c;
	obj->tx_head = i;
  if (!UART_TX_Active(obj->huart_handle))
	{
    UART_Transmit_DMA_Init(obj);
  }
  return 1;
}


/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle pointer on the uart reference
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==(&huart1))
	{
		UART_Buffer_Rx_Complate(&uart1_buffer);
	}
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle pointer on the uart reference
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==(&huart1))
	{
		UART_Buffer_TX_Complate(&uart1_buffer);
	}
}

int UART_Buffer_Available(UART_BufferTypeDef *obj)
{
	return ((unsigned int)(obj->rx_buff_size + obj->rx_head - obj->rx_tail)) % obj->rx_buff_size;
}

int UART_Buffer_Available_For_Write(UART_BufferTypeDef *obj)
{
	uint16_t head = obj->tx_head;
  uint16_t tail = obj->tx_tail;

  if (head >= tail) {
    return obj->tx_buff_size - 1 - head + tail;
  }
  return tail - head - 1;
}
void UART_Buffer_Flush(UART_BufferTypeDef *obj)
{
	while ((obj->tx_head != obj->tx_tail)) {
    // nop, the interrupt handler will free up space for us
  }
}



/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

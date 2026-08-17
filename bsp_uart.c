#include "stm32f10x.h"                  // Device header
#include <string.h>
#include "bsp_uart.h"
#include <stdio.h>
#define RINGBUF_SIZE       128
typedef struct {
	uint8_t buf[RINGBUF_SIZE];
	volatile uint16_t head;
	volatile uint16_t tail;
}RingBuf_t;
static RingBuf_t rb;
static uint8_t RingBuf_empty(RingBuf_t *rb)
{
	return rb->head == rb->tail;
}
static uint8_t RingBuf_full(RingBuf_t *rb)
{	
	return ((rb->head + 1) % RINGBUF_SIZE) == rb->tail;
}
static void RCC_Uart_Cmd(void)
{
	RCC_APB2PeriphClockCmd(UART_GPIORCC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_UART,ENABLE);
}

void BSP_Uart_Init(void)
{
	RCC_Uart_Cmd();
	/*RX*/
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_GPIO,&GPIO_InitStructure);
	/*TX*/

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_RX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_GPIO,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_Uer;
	USART_Uer.USART_BaudRate = 9600;              							// 波特率
	USART_Uer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   // 硬件流控制
	USART_Uer.USART_Mode = USART_Mode_Tx | USART_Mode_Rx ;                 	// 串口模式
	USART_Uer.USART_Parity = USART_Parity_No;                				// 校验位
	USART_Uer.USART_StopBits = USART_StopBits_1;              				// 停止位
	USART_Uer.USART_WordLength = USART_WordLength_8b;  
	USART_Init(USART1,&USART_Uer);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_Uer;
	NVIC_Uer.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Uer.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Uer.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Uer.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_Uer);
	USART_Cmd(USART1,ENABLE);
	memset(rb.buf, 0, RINGBUF_SIZE);
	rb.head = 0;
	rb.tail = 0;
}

void BSP_Uart_Sendbyte(uint8_t byte)
{
	USART_SendData(USART1,byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}
uint8_t BSP_UART_Available(void)
{
	return !RingBuf_empty(&rb);
}

uint8_t BSP_UART_ReadByte(void)
{
	if(RingBuf_empty(&rb))
		return 0xff;
	__disable_irq();
	uint8_t ch = rb.buf[rb.tail];
	rb.tail = (rb.tail + 1) % RINGBUF_SIZE;
	__enable_irq();
	return ch;
}

void BSP_UART_SendString(const char *String)
{
	uint16_t i;
	for(i = 0; String[i] != '\0';i++)
	{
		BSP_Uart_Sendbyte(String[i]);
	}
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE))
    {
        uint8_t ch = USART_ReceiveData(USART1);
		if(!RingBuf_full(&rb))
		{
			rb.buf[rb.head] = ch;
			rb.head = (rb.head + 1) % RINGBUF_SIZE;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
int fputc(int ch, FILE *f)
{
	BSP_Uart_Sendbyte(ch);
	return ch;
}

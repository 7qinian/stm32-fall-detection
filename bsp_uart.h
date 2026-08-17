#ifndef  __BSP_UART_H
#define  __BSP_UART_H
#define RCC_UART      RCC_APB2Periph_USART1
#define UART_GPIORCC  RCC_APB2Periph_GPIOA
#define UART_GPIO     GPIOA
#define GPIO_TX       GPIO_Pin_9
#define GPIO_RX       GPIO_Pin_10




void BSP_Uart_Init(void);
void BSP_Uart_Sendbyte(uint8_t byte); //发送字节
uint8_t BSP_UART_Available(void);  //判断是否有数据
uint8_t BSP_UART_ReadByte(void);   //接收数据
void BSP_UART_SendString(const char *String); //发送字符串

#endif

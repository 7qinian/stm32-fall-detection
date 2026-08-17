#ifndef  __BSP_I2C_H
#define  __BSP_I2C_H

#define IIC_GPIORCC  RCC_APB2Periph_GPIOA
#define IIC_GPIO  GPIOA
#define GPIO_SCL  GPIO_Pin_6
#define GPIO_SDA  GPIO_Pin_7


void BSP_IIC_Init(void);
uint8_t BSP_IIC_Recover(void);
void BSP_IIC_Stop(void);
void BSP_IIC_Start(void);
void BSP_IIC_Send_Byte(unsigned char data);
uint8_t BSP_IIC_ReceiveByte(void);
void BSP_IIC_SendAck(uint8_t AckBit);
uint8_t BSP_IIC_ReceiveAck(void);

#endif

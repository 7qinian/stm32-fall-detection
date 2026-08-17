#include "stm32f10x.h"                  // Device header
#include "BSP_I2c.h"
static void delay(int us)
{
	us *= (SystemCoreClock / 1000000);
	volatile int cunt = us;
	while(cunt--);
}

static void RCC_IIC_cmd(void)
{
	RCC_APB2PeriphClockCmd(IIC_GPIORCC,ENABLE);	
}
static void IIC_SCL_Out(int x)
{
	if(x == 0)
	{
		GPIO_WriteBit(IIC_GPIO,GPIO_SCL,Bit_RESET);
		delay(2);
	}
	else
	{
		GPIO_WriteBit(IIC_GPIO,GPIO_SCL,Bit_SET);
		delay(2);
	}
}
static void IIC_SDA_Out(int x)
{
	if(x == 0)
	{
		GPIO_WriteBit(IIC_GPIO,GPIO_SDA,Bit_RESET);
		delay(2);
	}
	else
	{
		GPIO_WriteBit(IIC_GPIO,GPIO_SDA,Bit_SET);
		delay(2);
	}
}
static uint8_t IIC_Receive(void)
{
	uint8_t Value;
	Value = GPIO_ReadInputDataBit(IIC_GPIO,GPIO_SDA);
	return Value;
}
static uint8_t BSP_IIC_IsSDA_Low(void)
{
	return GPIO_ReadInputDataBit(IIC_GPIO,GPIO_SDA) == 0;
}

void BSP_IIC_Init(void)
{
	RCC_IIC_cmd();
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_SCL | GPIO_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(IIC_GPIO,&GPIO_InitStructure);
	IIC_SDA_Out(1);
	IIC_SCL_Out(1);
	BSP_IIC_Recover();
}

void BSP_IIC_Stop(void)
{
	IIC_SDA_Out(0);
	IIC_SCL_Out(1);
	IIC_SDA_Out(1);
}
void BSP_IIC_Start(void)
{
	IIC_SDA_Out(1);
	IIC_SCL_Out(1);
	IIC_SDA_Out(0);
	IIC_SCL_Out(0);
}
void BSP_IIC_Send_Byte(unsigned char data)
{
	uint8_t i;
	for(i = 0; i < 8 ;i++)
	{
		IIC_SDA_Out(data & 0x80);
		data <<= 1;
		IIC_SCL_Out(1);
		IIC_SCL_Out(0);
	}
	IIC_SDA_Out(1);
}

uint8_t BSP_IIC_ReceiveByte(void)
{
	uint8_t i;uint8_t byte = 0x00;
	IIC_SDA_Out(1);
	for(i = 0; i < 8;i++)
	{
		byte <<= 1;
		IIC_SCL_Out(1);
		if(1 == IIC_Receive()){byte |= 0x01;}
		IIC_SCL_Out(0);
	}
	return byte;
}
void BSP_IIC_SendAck(uint8_t AckBit)
{
	IIC_SDA_Out(AckBit);
	IIC_SCL_Out(1);
	IIC_SCL_Out(0);
	IIC_SDA_Out(1);
}

uint8_t BSP_IIC_ReceiveAck(void)
{
	uint8_t Ackbit;
	IIC_SDA_Out(1);
	delay(1);
	IIC_SCL_Out(1);
	delay(1);
	Ackbit = IIC_Receive();
	IIC_SCL_Out(0);
	return Ackbit;
}
uint8_t BSP_IIC_Recover(void)
{
	uint8_t i;
	if(!BSP_IIC_IsSDA_Low())
		return 0;
	
	IIC_SDA_Out(1);
	for(i = 0 ;i < 9;i++)
	{
		if(!BSP_IIC_IsSDA_Low())
			break;
		IIC_SCL_Out(1);	
		delay(2);			
		IIC_SCL_Out(0);
		delay(2);	
	}
		IIC_SCL_Out(1);
		delay(2);
		IIC_SDA_Out(0);
		IIC_SDA_Out(1);
		
	return BSP_IIC_IsSDA_Low() ? 1 : 0;
}

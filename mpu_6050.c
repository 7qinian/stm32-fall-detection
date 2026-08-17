#include "stm32f10x.h"                  // Device header
#include "BSP_I2c.h"
#include "mpu6050_reg.h"

#define MPU6050_ADDRESS   0XD0
typedef struct GetData{
	int16_t AccX;
	int16_t AccY;
	int16_t AccZ;
	int16_t GyroX;
	int16_t GyroY;
	int16_t GyroZ;
}MPU6050_t;
uint8_t MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)
{
	BSP_IIC_Start();
	BSP_IIC_Send_Byte(MPU6050_ADDRESS);
	if(BSP_IIC_ReceiveAck())
	{
		BSP_IIC_Stop();
		return 1;
	}
	BSP_IIC_Send_Byte(RegAddress);
	if(BSP_IIC_ReceiveAck())
	{
		BSP_IIC_Stop();
		return 2;
	}
	BSP_IIC_Send_Byte(Data);
	if(BSP_IIC_ReceiveAck())
	{
		BSP_IIC_Stop();
		return 3;
	}
	BSP_IIC_Stop();
	return 0;
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress,uint8_t *data)
{
	*data = 0;
	BSP_IIC_Start();
	BSP_IIC_Send_Byte(MPU6050_ADDRESS);
	if(BSP_IIC_ReceiveAck())
	{
		BSP_IIC_Stop();
		return 1;
	}
	BSP_IIC_Send_Byte(RegAddress);
	if(BSP_IIC_ReceiveAck())
	{
		BSP_IIC_Stop();
		return 2;
	}
	BSP_IIC_Start();
	BSP_IIC_Send_Byte(MPU6050_ADDRESS | 0x01);
	if(BSP_IIC_ReceiveAck())
	{
		BSP_IIC_Stop();
		return 3;
	}
	*data = BSP_IIC_ReceiveByte();
	BSP_IIC_SendAck(1);
	BSP_IIC_Stop();
	return 0;
	
}
void MPU6050_Init(void)
{
	BSP_IIC_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01); 
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x09);
	MPU6050_WriteReg(MPU6050_CONFIG,0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);
	
}

uint8_t MPU6050_CheckConnection(void)
{
    BSP_IIC_Start();
    BSP_IIC_Send_Byte(MPU6050_ADDRESS);   // ??????
    uint8_t ack = BSP_IIC_ReceiveAck();
    BSP_IIC_Stop();
    return ack;
}
void MPU6050_GetData(MPU6050_t *Data)
{
	uint8_t DataH,DataL;
	MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H,&DataH);
	MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L,&DataL); 
	Data->AccX = (DataH << 8) | DataL;
	
	MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H,&DataH);
	MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L,&DataL); 
	Data->AccY = (DataH << 8) | DataL;
	
	MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H,&DataH);
	MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L,&DataL); 
	Data->AccZ = (DataH << 8) | DataL;
	
	MPU6050_ReadReg(MPU6050_GYRO_XOUT_H,&DataH);
	MPU6050_ReadReg(MPU6050_GYRO_XOUT_L,&DataL); 
	Data->GyroX = (DataH << 8) | DataL;
	
	MPU6050_ReadReg(MPU6050_GYRO_YOUT_H,&DataH);
	MPU6050_ReadReg(MPU6050_GYRO_YOUT_L,&DataL); 
	Data->GyroY = (DataH << 8) | DataL;
	
	MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H,&DataH);
	MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L,&DataL); 
	Data->GyroZ = (DataH << 8) | DataL;
}


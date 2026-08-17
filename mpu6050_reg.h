#ifndef __MPU6050_REG_H
#define __MPU6050_REG_H

// 采样率分频寄存器
#define MPU6050_SMPLRT_DIV    0x19   // 采样率 = 陀螺仪输出率 / (1 + SMPLRT_DIV)

// 配置寄存器
#define MPU6050_CONFIG        0x1A   // 配置外部帧同步、数字低通滤波器(DLPF)

// 陀螺仪配置寄存器
#define MPU6050_GYRO_CONFIG   0x1B   // 设置陀螺仪量程(±250/500/1000/2000 °/s)及自测

// 加速度计配置寄存器
#define MPU6050_ACCEL_CONFIG  0x1C   // 设置加速度计量程(±2/4/8/16 g)及自测

// 加速度计 X 轴数据高8位
#define MPU6050_ACCEL_XOUT_H  0x3B
// 加速度计 X 轴数据低8位
#define MPU6050_ACCEL_XOUT_L  0x3C
// 加速度计 Y 轴数据高8位
#define MPU6050_ACCEL_YOUT_H  0x3D
// 加速度计 Y 轴数据低8位
#define MPU6050_ACCEL_YOUT_L  0x3E
// 加速度计 Z 轴数据高8位
#define MPU6050_ACCEL_ZOUT_H  0x3F
// 加速度计 Z 轴数据低8位
#define MPU6050_ACCEL_ZOUT_L  0x40

// 温度传感器数据高8位
#define MPU6050_TEMP_OUT_H    0x41
// 温度传感器数据低8位
#define MPU6050_TEMP_OUT_L    0x42

// 陀螺仪 X 轴数据高8位
#define MPU6050_GYRO_XOUT_H   0x43
// 陀螺仪 X 轴数据低8位
#define MPU6050_GYRO_XOUT_L   0x44
// 陀螺仪 Y 轴数据高8位
#define MPU6050_GYRO_YOUT_H   0x45
// 陀螺仪 Y 轴数据低8位
#define MPU6050_GYRO_YOUT_L   0x46
// 陀螺仪 Z 轴数据高8位
#define MPU6050_GYRO_ZOUT_H   0x47
// 陀螺仪 Z 轴数据低8位
#define MPU6050_GYRO_ZOUT_L   0x48

// 电源管理寄存器1（时钟源、休眠控制）
#define MPU6050_PWR_MGMT_1    0x6B   // 默认 0x40(休眠)，写 0x00 唤醒
// 电源管理寄存器2（各轴独立开关）
#define MPU6050_PWR_MGMT_2    0x6C

// 器件 ID 寄存器（固定值 0x68）
#define MPU6050_WHO_AM_I      0x75

#endif

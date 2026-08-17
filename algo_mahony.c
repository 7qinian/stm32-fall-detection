#include "stm32f10x.h"                  // Device header
#include "mpu_6050.h"
#include "bsp_timer.h"
#include "algo_mahony.h"
#include <math.h>
#define FALL_ACC_MIN       0.6f    // 失重 < 0.6g²
#define FALL_ACC_IMPACT    2.8f    // 撞击 > 2.8g²
#define FALL_GYRO_TH       200.0f  // 旋转 > 200°/s
#define FALL_ANGLE_TH      45.0f   // 姿态 > 45°
#define FALL_STILL_TIME    100     // 1s (10ms * 100)
#define M_PI               3.14159
MPU6050_t rb;

typedef struct{
	float pitch ;
	float roll;
}Attitude_t;
static void fall_detect(MPU6050_t *adder, Attitude_t *att);
typedef enum{
	FALL_ST_NORMAL = 0,
    FALL_ST_FREEFALL,
    FALL_ST_IMPACT,
    FALL_ST_POSTURE,
    FALL_ST_STILL,
}FallState_e;
static uint8_t fall_detected = 0;
static FallState_e fall_state = FALL_ST_NORMAL;
static uint16_t still_cnt = 0;
static Attitude_t att;
static float Acceler(MPU6050_t *adder)
{
	float ax = adder->AccX / 2048.0f;
    float ay = adder->AccY / 2048.0f;
    float az = adder->AccZ / 2048.0f;
    return ax*ax + ay*ay + az*az;
}
static float angular_velocity(MPU6050_t *adder)
{
	float gx = adder->GyroX / 16.4f;
    float gy = adder->GyroY / 16.4f;
    float gz = adder->GyroZ / 16.4f;
    return gx*gx + gy*gy + gz*gz;
}
static void Attitude_angle(MPU6050_t *adder, Attitude_t *a)
{
    float ax = adder->AccX / 2048.0f;
    float ay = adder->AccY / 2048.0f;
    float az = adder->AccZ / 2048.0f;

    a->pitch = atan2f(-ax, az) * 180.0f / (float)M_PI;
    a->roll  = atan2f(ay, az) * 180.0f / (float)M_PI;
}
uint8_t Fall_IsDetected(void) 
{
	 static uint32_t last_tick = 0;
	if(Timer_10msGetFlag() - last_tick >=1)
	{
		last_tick = Timer_10msGetFlag();
		MPU6050_GetData(&rb);
		fall_detect(&rb,&att);
	}
	uint8_t a;

	a = fall_detected;
	fall_detected = 0;

	return a;
}


static void fall_detect(MPU6050_t *adder, Attitude_t *att)
{
	float acc = Acceler(adder);
	float gyro = angular_velocity(adder);
	
	switch (fall_state)
	{
		case FALL_ST_NORMAL:
			if(acc < FALL_ACC_MIN) fall_state = FALL_ST_FREEFALL;
			break;
		case FALL_ST_FREEFALL:
			if (acc > FALL_ACC_IMPACT)  fall_state = FALL_ST_IMPACT;
			break;
		case FALL_ST_IMPACT:
			 if (gyro > FALL_GYRO_TH) fall_state = FALL_ST_POSTURE;
			break;
		case FALL_ST_POSTURE:
			Attitude_angle(adder,att);
			if (fabsf(att->pitch) > FALL_ANGLE_TH ||
            fabsf(att->roll)  > FALL_ANGLE_TH)
        {
            fall_state = FALL_ST_STILL;
            still_cnt = 0;
        }
			break;
		case FALL_ST_STILL:
			still_cnt++;

        // 还在动，取消摔倒
        if (acc > 1.5f || gyro > 50.0f)
        {
            fall_state = FALL_ST_NORMAL;
            still_cnt = 0;
            break;
        }

        if (still_cnt >= FALL_STILL_TIME)
        {
           // fall_alarm();   /*后期封装*/
			
            fall_state = FALL_ST_NORMAL;
            still_cnt = 0;
			fall_detected = 1;
        }
			break;	
	}

}

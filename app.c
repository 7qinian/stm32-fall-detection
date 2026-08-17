#include "stm32f10x.h"                  // Device header
#include "algo_mahony.h"
#include "bsp_timer.h"
#include "bsp_uart.h"
#include "mpu_6050.h"
#include "protocol.h"
static uint32_t heartbeat_counter = 0;
static Location_t current_location = {30.1234f, 120.5678f};/*测试*/
static void App_HandleFallDetection(void)
{
	
	if(Fall_IsDetected())
	{
		Protocol_ReportFall(&current_location);
	}
}

static void App_HandleHeartbeat(void)
{
	heartbeat_counter++;
	if(heartbeat_counter >= 300)
	{
		heartbeat_counter = 0;
		Protocol_ReportHeartbeat();
	}		
}
void App_Init(void)
{
/* 1️⃣ 先关中断（防 Boot 残留） */
    __disable_irq();

    /* 2️⃣ 系统时钟初始化（它会改 VTOR） */
    SystemInit();

    /* 3️⃣ 切向量表（放在 SystemInit 后面！） */
    SCB->VTOR = 0x08004000;

    /* 4️⃣ 配置中断优先级分组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* 5️⃣ 关掉 Boot 可能开的 SysTick */
    SysTick->CTRL = 0;

    /* 6️⃣ 现在才能开中断 */
    __enable_irq();
	BSP_Uart_Init();
	MPU6050_Init();
	bsp_timer_Init();
	
}
void App_Process(void)
{
	 static uint32_t last_tick = 0;
		if (Timer_10msGetFlag() - last_tick >= 1) 
		{
			last_tick = Timer_10msGetFlag();
			App_HandleFallDetection();   // 摔倒检测（内部会读 MPU6050）
			App_HandleHeartbeat();       // 心跳上报
		}
}

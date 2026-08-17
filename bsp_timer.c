#include "stm32f10x.h"                  // Device header

#define RCC_TIMR2  RCC_APB1Periph_TIM2
#define TIMPASSAGE  TIM2
static volatile uint8_t g_10ms_flag = 0;
static void RCC_Timer(void)
{
	RCC_APB1PeriphClockCmd(RCC_TIMR2,ENABLE);
	TIM_InternalClockConfig(TIMPASSAGE);
}
void bsp_timer_Init(void)
{
	RCC_Timer();
	//配置时基单元
	TIM_TimeBaseInitTypeDef Tim_Uer;
	Tim_Uer.TIM_ClockDivision = TIM_CKD_DIV1;        //指定时钟分频
	Tim_Uer.TIM_CounterMode = TIM_CounterMode_Up;    //计数器模式
	Tim_Uer.TIM_Period = 10000-1;                    //ARR自动重装器
	Tim_Uer.TIM_Prescaler = 72-1;					 //PSC预分频器				
	Tim_Uer.TIM_RepetitionCounter = 0;				 //重复计数器
	TIM_TimeBaseInit(TIMPASSAGE,&Tim_Uer);
	
	TIM_ITConfig(TIMPASSAGE,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_Uer;
	NVIC_Uer.NVIC_IRQChannel = TIM2_IRQn;                      //中断通道
	NVIC_Uer.NVIC_IRQChannelCmd = ENABLE;                      //使能
	NVIC_Uer.NVIC_IRQChannelPreemptionPriority = 2;            //抢占优先级  
	NVIC_Uer.NVIC_IRQChannelSubPriority = 1;                   //响应优先级
	
	NVIC_Init(&NVIC_Uer);
	
	//启动定时器
	TIM_Cmd(TIM2,ENABLE);
}
uint8_t Timer_10msGetFlag(void)
{
	/*
	uint8_t num = g_10ms_flag;
	g_10ms_flag = 0;
	*/
	return g_10ms_flag;
}
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIMPASSAGE,TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIMPASSAGE,TIM_IT_Update);
		g_10ms_flag++;
		
	}
}

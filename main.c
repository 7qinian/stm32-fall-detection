#include "stm32f10x.h"                  // Device header

#include "ota.h"
void Boot_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpio_init.GPIO_Pin   = GPIO_Pin_1;
    gpio_init.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &gpio_init);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}
void delay(int n)
{
	while(n--)
	{
		for(int i = 8050; i > 0;i--);
	}
}
int main(void)
{
	Boot_GPIO_Init();
	delay(1000);
	if(OTA_NeedUpgrade())
	{
		OTA_Run();
	}
	else
	{
		JUMPtoAPP();
	}
	while(1)
	{
		
	}
}

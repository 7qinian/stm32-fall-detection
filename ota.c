
#include "stm32f10x.h"                  // Device header

#define  APP_PAGES        16
#define  OTA_PAGES        16
#include "bsp_falsh.h"
#include "ota.h"
int OTA_NeedUpgrade(void)
{
	uint32_t  flag  = Falsh_ReadWord(PARAMETER);
	return (flag == 0x12345678);
}

void OTA_Run(void)
{
	for(int i = 0; i < APP_PAGES;i++)
	{
		BSP_Falsh_Erasepages(FALSH_APP + i * FLASH_PAGE_SIZE);
	}
	for(int i = 0; i < APP_PAGES;i++)
	{
		for(int j = 0 ; j < FLASH_PAGE_SIZE / 4;j++)
		{
			uint32_t addr = i * FLASH_PAGE_SIZE + j * 4;
			uint32_t data  = Falsh_ReadWord(FALSH_OTA + addr);
			BSP_Falsh_pagesword(FALSH_APP + addr,data);
		}
	}
	BSP_Falsh_Erasepages(PARAMETER);
	JUMPtoAPP();
}


void JUMPtoAPP(void)
{
	typedef void (*pFun)(void);
	
	uint32_t jumpapp;
	
	pFun jumptoapplication;
	
	__disable_irq();
	
	jumpapp = *(__IO uint32_t*)(FALSH_APP + 4);
	
	jumptoapplication = (pFun)jumpapp;
	
	__set_MSP(*(__IO uint32_t*)FALSH_APP);
	
	SCB->VTOR = FALSH_APP;
	
	jumptoapplication();
}
		
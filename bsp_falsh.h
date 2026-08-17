#ifndef  __BSP_FALSH_H
#define  __BSP_FALSH_H

#include "stm32f10x.h"                  // Device header
#define  FLASH_PAGE_SIZE   1024
#define  PARAMETER        0x0800fc00
#define	 FALSH_APP        0x08004000
#define  FALSH_OTA        0x08008000

uint32_t Falsh_ReadWord(uint32_t Address);

void BSP_Falsh_Erasepages(uint32_t pageAddress);
void BSP_Falsh_pagesword(uint32_t pageAddress,uint32_t data);
#endif 

#include "stm32f10x.h"                  // Device header
#include "bsp_falsh.h"

uint32_t Falsh_ReadWord(uint32_t Address)
{
	if(Address % FLASH_PAGE_SIZE != 0)
		return 0XFFFFFFFF;
	return *((__IO uint32_t *)(Address));
}

uint16_t Falsh_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t *)(Address));
}
uint8_t Falsh_ReadByteWord(uint32_t Address)
{
	return *((__IO uint8_t *)(Address));
}

void BSP_Falsh_Erasepages(uint32_t pageAddress)
{
	if(pageAddress % FLASH_PAGE_SIZE != 0)
		return ;
	FLASH_Unlock();   //解锁
	FLASH_ErasePage(pageAddress);  //擦除
	FLASH_Lock();  //加锁
}
void BSP_Falsh_pagesword(uint32_t pageAddress,uint32_t data)
{
	if(pageAddress % FLASH_PAGE_SIZE != 0)
		return ;
	FLASH_Unlock();   //解锁
	FLASH_ProgramWord(pageAddress,data);  //写入32位数据
	FLASH_Lock();  //加锁
}

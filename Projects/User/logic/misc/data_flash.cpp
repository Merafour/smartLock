/************************ (C) COPYLEFT 2018 Merafour *************************

* File Name          : data_flash.cpp
* Author             : Merafour
* Last Modified Date : 04/17/2018
* Description        : This file provides the data flash library.

******************************************************************************/
#include "data_flash.h"
#include "eeprom.h"
#include "ssd1306syp.h"

#define _FORMAT   0xCCEBF985
void DATA_FLASH::Init(void)
{
	uint16_t i=0;

	//const uint8_t *str=(const uint8_t *)"ÃÌº””√ªß";
	for(i=0; i<sizeof(VirtAddVarTab)/sizeof(VirtAddVarTab[0]); i++) VirtAddVarTab[i]=i+1000;
#if 1
//	uint32_t p0=0,p1=0;
//	uint16_t temp_add=0;
//	p0 = *(__IO uint32_t*)PAGE0_BASE_ADDRESS;
//	p1 = *(__IO uint32_t*)PAGE1_BASE_ADDRESS;
	if (((*(__IO uint32_t*)PAGE0_BASE_ADDRESS) == 0xFFFFFFFF)&&((*(__IO uint32_t*)PAGE1_BASE_ADDRESS) == 0xFFFFFFFF))
	//if ((p0 != _FORMAT)&&(p1 != _FORMAT))
	{
		/* Unlock the Flash Program Erase controller */
		FLASH_Unlock();
		/* EEPROM Init */
		EE_Init();
		FLASH_Lock();
		/* --- Store successively many values of the three variables in the EEPROM ---*/
		FLASH_Unlock();
		EE_WriteVariable(VirtAddVarTab[0], 0);
		FLASH_Lock();
	}
#if 0
	/* Unlock the Flash Program Erase controller */
	//FLASH_Unlock();
	//EE_ReadVariable(VirtAddVarTab[0],&temp_add);
	read(0, &temp_add, 1);
	temp_add += 10;
//	FLASH_Unlock();
//	EE_WriteVariable(VirtAddVarTab[0],temp_add);
//	FLASH_Lock();
	write(0, &temp_add, 1);
	//EE_ReadVariable(VirtAddVarTab[0],&temp_add);
	read(0, &temp_add, 1);
	display.clear(0);
	display.setCursor(0,8);
	display.printf("FLASH:%5d ", temp_add);
	display.update();
	while(1);
#endif
#endif
	DATA::Init();
#if 0
	/* Unlock the Flash Program Erase controller */
	//FLASH_Unlock();
	//EE_ReadVariable(VirtAddVarTab[0],&temp_add);
	read(0, &temp_add, 1);
	temp_add += 10;
//	FLASH_Unlock();
//	EE_WriteVariable(VirtAddVarTab[0],temp_add);
//	FLASH_Lock();
	write(0, &temp_add, 1);
	//EE_ReadVariable(VirtAddVarTab[0],&temp_add);
	read(0, &temp_add, 1);
	display.clear(0);
	display.setCursor(0,8);
	display.printf("FLASH:%5d ", temp_add);
	display.update();
	while(1);
#endif
}
uint8_t DATA_FLASH::erase_all(void)
{
//	uint16_t index=0;
//	uint16_t data=0;
//	/* Unlock the Flash Program Erase controller */
//	FLASH_Unlock();
//	/* EEPROM Init */
//	EE_Init();
//	FLASH_Lock();
//	FLASH_Unlock();
//	for(index=0; index<NB_OF_VAR; index++)
//	{
//		EE_WriteVariable(VirtAddVarTab[index], 0);
//	}
//	FLASH_Lock();
//	// check
//	for(index=0; index<NB_OF_VAR; index++)
//	{
//		read(index, &data, 1);
//		if(0xFFFF!=data) return 1;
//	}
	return 0;
}

uint8_t DATA_FLASH::read(const uint16_t Addr, uint16_t *buf, const uint16_t len)
{
	uint16_t index=0;
	if(len>NB_OF_VAR) return 0;
	//  EE_ReadVariable(VirtAddVarTab[0],&temp_add);
	for(index=0; index<len; index++)
	{
		EE_ReadVariable(VirtAddVarTab[Addr+index], &buf[index]);
	}
	return index;
}

uint8_t DATA_FLASH::write(const uint16_t Addr, const uint16_t *buf, const uint16_t len)
{
	uint16_t index=0;
	if(len>NB_OF_VAR) return 0;
	FLASH_Unlock();
	//  EE_WriteVariable(VirtAddVarTab[0],temp_add);
	for(index=0; index<len; index++)
	{
		EE_WriteVariable(VirtAddVarTab[Addr+index], buf[index]);
	}
	FLASH_Lock();
	return index;
}

uint16_t DATA_FLASH::lenght(void)
{
	return NB_OF_VAR<<1;
}


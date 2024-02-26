/*
 * flash_data.cpp
 *
 *  Created on: Jul 28, 2022
 *      Author: Sky
 */

#include "flash_data.h"

uint32_t flashWriteData(TermoplastConfigTypeDef *configData)
{
	static FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = FLASH_SECTOR_7;
	EraseInitStruct.NbSectors = 1;
	//EraseInitStruct.Banks = FLASH_BANK_1;
	uint32_t Address = START_ADDR;
	uint32_t SectorError = 0;
	HAL_FLASH_Unlock();
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
		uint32_t er = HAL_FLASH_GetError();
		return er;
	}
	osDelay(20);
	for (int i = 0; i < sizeof(*configData)/4; i++) {
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, *(((uint32_t*)configData)+i)) != HAL_OK)
		{
			uint32_t er = HAL_FLASH_GetError();
			osDelay(50);
			return er;
		}
		Address += 4;
		osDelay(2);
	}
	osDelay(10);
	HAL_FLASH_Lock();
	return 0;
}
uint32_t flashReadData(TermoplastConfigTypeDef *configData)
{
	uint32_t Address = START_ADDR;
	for (int i = 0; i < sizeof(*configData)/4; i++) {
		*(((uint32_t*)configData)+i) = *(uint32_t*)Address;
		Address += 4;
	}
	osDelay(10);
	return 0;
}
uint32_t GetSectorSize(uint32_t Sector)
{
  uint32_t sectorsize = 0x00;

  if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3))
  {
    sectorsize = 16 * 1024;
  }
  else if(Sector == FLASH_SECTOR_4)
  {
    sectorsize = 64 * 1024;
  }
  else
  {
    sectorsize = 128 * 1024;
  }
  return sectorsize;
}

/*
 * flash_data.h
 *
 *  Created on: Jul 28, 2022
 *      Author: Sky
 */

#ifndef INC_FLASH_DATA_H_
#define INC_FLASH_DATA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "cmsis_os2.h"
#include "termoplast_config.h"

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#define START_ADDR ADDR_FLASH_SECTOR_7
#define END_ADDR     START_ADDR  +  GetSectorSize(ADDR_FLASH_SECTOR_7) -1

uint32_t flashWriteData(TermoplastConfigTypeDef *configData);
uint32_t flashReadData(TermoplastConfigTypeDef *configData);
uint32_t GetSectorSize(uint32_t Sector);

#ifdef __cplusplus
}
#endif

#endif /* INC_FLASH_DATA_H_ */

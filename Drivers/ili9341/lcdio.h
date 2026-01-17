#pragma once

#include <stdint.h>

#define LCD_DATA	(0x60000000)
#define LCD_CMD		(0x60000000)


void            LCD_IO_Init(void);
void            LCD_IO_WriteData(uint16_t Data); 
void            LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size);
void            LCD_IO_WriteReg(uint8_t Reg);
uint16_t        LCD_IO_ReadData(uint16_t Reg);

void LCD_Delay(uint32_t delay);

#include "lcdio.h"

void LCD_IO_Init(void) {
}

void LCD_IO_WriteData(uint16_t Data) {
	LCD_DATA = Data;
}

void LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size);

void LCD_IO_WriteReg(uint8_t Reg) {
	LCD_CMD = Reg;
}
uint16_t LCD_IO_ReadData(void){
	return LCD_DATA;
}

void LCD_Delay(uint32_t delay){
	HAL_Delay(delay);
}


#define LCD_REG LCD_CMD
#define LCD_RAM LCD_DATA

void LCD_OpenWin(u16 x0, u16 y0, u16 x1, u16 y1)
{

    LCD_REG = (ILI_CASET);
    LCD_RAM = (x0 >> 8);
    LCD_RAM = (x0);
    LCD_RAM = (x1 >> 8);
    LCD_RAM = (x1);
    LCD_REG = (ILI_PASET);
    LCD_RAM = (y0 >> 8);
    LCD_RAM = (y0);
    LCD_RAM = (y1 >> 8);
    LCD_RAM = (y1);
    LCD_REG = (ILI_RAMWR);
}

void LCD_Put_Pixel(u16 x0, u16 y0, u16 Color)
{

    LCD_REG = (ILI_CASET);
    LCD_RAM = (x0 >> 8);
    LCD_RAM = (x0);
    LCD_RAM = (x0 >> 8);
    LCD_RAM = (x0);
    LCD_REG = (ILI_PASET);
    LCD_RAM = (y0 >> 8);
    LCD_RAM = (y0);
    LCD_RAM = (y0 >> 8);
    LCD_RAM = (y0);
    LCD_REG = (ILI_RAMWR);
    LCD_RAM = (Color);
}

void LCD_ClrScr(u16 BackColor)
{
    uint32_t i;
    LCD_OpenWin(0, 0, 240, 320);
    for(i = 0; i < (240*320); i++)
        LCD_RAM = BackColor ;
}



#include "ili9341.h"
#include "stdlib.h"

uint8_t LCD_WORK_ORIENTATION;
uint8_t LCD_NOT_WORK_ORIENTATION;
uint8_t LCD_PORTRAIT_WORK_ORIENTATION;
uint8_t LCD_PORTRAIT_NOT_WORK_ORIENTATION;

static const uint8_t init_tab[] = {
ILI_PCB, 3, 0x00, 0xC1, 0X30, /* 0xCF */
ILI_POSC, 4, 0x64, 0x03, 0X12, 0X81, /* 0xED */
ILI_DTCA_ic, 3, 0x85, 0x10, 0x7A, /* 0xE8 */
ILI_PCA, 5, 0x39, 0x2C, 0x00, 0x34, 0x02, /* 0xCB */
ILI_PRC, 1, 0x20, /* 0xF7 */
ILI_DTCB, 2, 0x00, 0x00, /* 0xEA */
ILI_PWCTRL1, 1, 0x1B, /* 0xC0 */
ILI_PWCTRL2, 1, 0x01, /* 0xC1 */
ILI_VMCTRL1, 2, 0x30, 0x30, /* 0xC5 */
ILI_VMCTRL2, 1, 0XB7, /* 0xC7 */
ILI_MADCTL, 1, 0x48, /* 0x36 */
ILI_PIXSET, 1, 0x55, /* 0x3A */
//      ILI_FRMCTR1, 2, 0x00, 0x1A,  0xB1
		ILI_DISCTRL, 2, 0x0A, 0xA2, /* 0xB6 oryg , A2 = 60hz */
		ILI_E3G, 1, 0x00, /* 0xF2 */
		ILI_GAMSET, 1, 0x01, /* 0x26 */
		ILI_PGAMCTRL, 15, 0x0F, 0x2A, 0x28, 0x08, 0x0E, 0x08, 0x54, 0XA9, 0x43,
		0x0A, 0x0F, 0x00, 0x00, 0x00, 0x00,
		ILI_NGAMCTRL, 15, 0x00, 0x15, 0x17, 0x07, 0x11, 0x06, 0x2B, 0x56, 0x3C,
		0x05, 0x10, 0x0F, 0x3F, 0x3F, 0x0F,
		ILI_PASET, 4, 0x00, 0x00, 0x01, 0x3f, /* 0x2B */
		ILI_CASET, 4, 0x00, 0x00, 0x00, 0xef, /* 0x2A */
		ILI_SLPOUT, 120, /* 0x11 */
		ILI_DISPON, 0, /* 0x29 */
		ILI_FRMCTR1, 2, 0x01, 0x12, /* 0xB1 */
		0x00 };
//-------------------------------------------------------------------------------------------
volatile uint32_t OS_TimeMS = 0; //!!! delete and declare as extern ! when use STemWIN !
//-------------------------------------------------------------------------------------------

lcdPropertiesTypeDef lcdProperties = { ILI9341_PIXEL_WIDTH,
		ILI9341_PIXEL_HEIGHT, LCD_ORIENTATION_PORTRAIT, true, true, 0x08 };

void LCD_OpenWin(u16 x0, u16 y0, u16 x1, u16 y1) {
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

void LCD_Put_Pixel(u16 x0, u16 y0, u16 Color) {
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

void LCD_ClrScr(u16 BackColor) {
	uint32_t i;
	LCD_OpenWin(0, 0, lcdProperties.width, lcdProperties.height);
	for (i = 0; i < (SCREEN_PIXELS); i++)
		LCD_RAM = BackColor;
}

void LCD_WriteReg(u16 data) {
	LCD_REG = data;
}
void LCD_WriteData(u16 data) {
	LCD_RAM = data;
}


uint32_t LCD_ILI9341_readID(void) {
	uint8_t d[4];
	LCD_WriteReg(0xd3);
	d[0] = LCD_RAM;
	d[1] = LCD_RAM;
	d[2] = LCD_RAM;
	d[3] = LCD_RAM;
	return (d[0]<<24) | (d[1]<<16) | (d[2]<<8) | (d[3]<<0);
}
void LCD_ILI9341_init(void) {
	u32 i = 0;
	u32 i2;
	u8 counter;

	LCD_REG = 1;
	HAL_Delay(1);

	//--------------
	while (init_tab[i] != 0) {
		LCD_REG = (u16) init_tab[i];
		counter = init_tab[i + 1];
		i += 2;
		if (counter > 15) {
			HAL_Delay(counter);
		} else if (counter > 0) {
			for (i2 = 0; i2 < counter; i2++) {
				LCD_RAM = init_tab[i + i2];
			}
			i += counter;
		}
	}

	lcdSetOrientation(LCD_ORIENTATION_PORTRAIT);
	LCD_ClrScr(COLOR_565_BLUE);  // init screen as black

}
static const uint8_t tab_orientations[] = { 0x68, 0xe8, 0xa8, 0x28, 0x08, 0x48,
		0x88, 0xc8 };


void lcdSetOrientation(lcdOrientationTypeDef value) {
	uint16_t x, y;
	if (value > 7) {
		value = 0;
	}

	lcdProperties.orientation = value;
	lcdProperties.MADCTL_VAL = tab_orientations[value];

	y = ILI9341_PIXEL_WIDTH;
	x = ILI9341_PIXEL_HEIGHT;

	if (value < 4) {
		x = y;
		y = ILI9341_PIXEL_HEIGHT;
	}

	lcdProperties.width = x;
	lcdProperties.height = y;
	LCD_WriteReg(ILI9341_MEMCONTROL);
	LCD_WriteData(lcdProperties.MADCTL_VAL);

	LCD_WORK_ORIENTATION = lcdProperties.orientation;
	if ((LCD_WORK_ORIENTATION & 3) != 0) {
		LCD_WORK_ORIENTATION = LCD_ORIENTATION_LANDSCAPE_ROTATE;
		LCD_NOT_WORK_ORIENTATION = LCD_ORIENTATION_LANDSCAPE;
		LCD_PORTRAIT_WORK_ORIENTATION = LCD_ORIENTATION_PORTRAIT_ROTATE_MIRROR;
		LCD_PORTRAIT_NOT_WORK_ORIENTATION = LCD_ORIENTATION_PORTRAIT;
	} else {
		LCD_WORK_ORIENTATION = LCD_ORIENTATION_LANDSCAPE;
		LCD_NOT_WORK_ORIENTATION = LCD_ORIENTATION_LANDSCAPE_ROTATE;
		LCD_PORTRAIT_WORK_ORIENTATION = LCD_ORIENTATION_PORTRAIT;
		LCD_PORTRAIT_NOT_WORK_ORIENTATION =
				LCD_ORIENTATION_PORTRAIT_ROTATE_MIRROR;
	}

}

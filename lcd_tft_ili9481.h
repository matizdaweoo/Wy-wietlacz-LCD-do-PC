/*
 * lcd_tft_ili9481.h
 *
 *  Created on: 15.03.2020
 *      Author: Mateusz
 */

#include "stm32f10x_gpio.h"

/* RS  - Register Select Command/Data selection (0-command, 1-data) */
#define TFT_ILI9481_RS_PORT 		GPIOB
#define TFT_ILI9481_RS_PIN			GPIO_Pin_7
	#define TFT_ILI9481_RS_Set		GPIO_SetBits(TFT_ILI9481_RS_PORT, TFT_ILI9481_RS_PIN)
	#define TFT_ILI9481_RS_Reset	GPIO_ResetBits(TFT_ILI9481_RS_PORT, TFT_ILI9481_RS_PIN)

/* CS  - Chip Selection (Low active)*/
#define TFT_ILI9481_CS_PORT 		GPIOB
#define TFT_ILI9481_CS_PIN			GPIO_Pin_6
	#define TFT_ILI9481_CS_Set 		GPIO_SetBits(TFT_ILI9481_CS_PORT, TFT_ILI9481_CS_PIN)
	#define TFT_ILI9481_CS_Reset	GPIO_ResetBits(TFT_ILI9481_CS_PORT, TFT_ILI9481_CS_PIN)

/* WR  - Write to TFT LCD */
#define TFT_ILI9481_WR_PORT 		GPIOB
#define TFT_ILI9481_WR_PIN			GPIO_Pin_5
	#define TFT_ILI9481_WR_Set		GPIO_SetBits(TFT_ILI9481_WR_PORT, TFT_ILI9481_WR_PIN);
	#define TFT_ILI9481_WR_Reset	GPIO_ResetBits(TFT_ILI9481_WR_PORT, TFT_ILI9481_WR_PIN);

/* RST - Reset LCD (Low level enable)*/
#define TFT_ILI9481_RST_PORT		GPIOB
#define TFT_ILI9481_RST_PIN			GPIO_Pin_4
	#define TFT_ILI9481_RST_Set		GPIO_SetBits(TFT_ILI9481_RST_PORT, TFT_ILI9481_RST_PIN)
	#define TFT_ILI9481_RST_Reset	GPIO_ResetBits(TFT_ILI9481_RST_PORT, TFT_ILI9481_RST_PIN)

/* LCD settings */
#define TFT_ILI9481_WIDTH		320
#define TFT_ILI9481_HEIGHT		480
#define TFT_ILI9481_PIXELS		TFT_ILI9481_WIDTH * TFT_ILI9481_HEIGHT
#define TFT_ILI9481_ORIENTATION	0

/* Colors */
#define TFT_ILI9481_COLOR_WHITE			0xFFFF
#define TFT_ILI9481_COLOR_BLACK			0x0000
#define TFT_ILI9481_COLOR_RED       	0xF800
#define TFT_ILI9481_COLOR_GREEN			0x07E0
#define TFT_ILI9481_COLOR_GREEN2		0xB723
#define TFT_ILI9481_COLOR_DARKGREEN   	0x03E0
#define TFT_ILI9481_COLOR_GREENYELLOW 	0xAFE5
#define TFT_ILI9481_COLOR_BLUE			0x001F
#define TFT_ILI9481_COLOR_BLUE2			0x051D
#define TFT_ILI9481_COLOR_YELLOW		0xFFE0
#define TFT_ILI9481_COLOR_ORANGE		0xFBE4
#define TFT_ILI9481_COLOR_ORANGE2      	0xFD20
#define TFT_ILI9481_COLOR_CYAN			0x07FF
#define TFT_ILI9481_COLOR_DARKCYAN    	0x03EF
#define TFT_ILI9481_COLOR_MAGENTA		0xA254
#define TFT_ILI9481_COLOR_MAGENTA2      0xF81F
#define TFT_ILI9481_COLOR_DARKGREY		0x7BEF
#define TFT_ILI9481_COLOR_LIGHTGREY   	0xC618
#define TFT_ILI9481_COLOR_BROWN			0xBBCA
#define TFT_ILI9481_COLOR_NAVY        	0x000F
#define TFT_ILI9481_COLOR_MAROON      	0x7800
#define TFT_ILI9481_COLOR_PURPLE      	0x780F
#define TFT_ILI9481_COLOR_OLIVE       	0x7BE0
#define TFT_ILI9481_COLOR_PINK        	0xF81F

/* Transparent background, only for strings and chars */	//TODO: check this!
#define TFT_ILI9481_TRANSPARENT			0x80000000

/* Command */
#define TFT_ILI9481_CMD_ENTER_SLEEP_MODE					0x10
#define TFT_ILI9481_CMD_EXIT_SLEEP_MODE						0x11

#define TFT_ILI9481_CMD_SET_DISPLAY_OFF						0x28
#define TFT_ILI9481_CMD_SET_DISPLAY_ON						0x29
#define TFT_ILI9481_CMD_SET_COLUMN_ADDRESS					0x2A
#define TFT_ILI9481_CMD_SET_PAGE_ADDRESS					0x2B
#define TFT_ILI9481_CMD_WRITE_MEMORY_START					0x2C

#define TFT_ILI9481_CMD_SET_ADDRESS_MODE					0x36
#define TFT_ILI9481_CMD_SET_PIXEL_FORMAT					0x3A

#define TFT_ILI9481_CMD_PANEL_DRIVING_SETTING				0xC0
#define TFT_ILI9481_CMD_FRAME_RATE_AND_INVERSION_CONTROL	0xC5
#define TFT_ILI9481_CMD_GAMMA_SETTING						0xC8

#define TFT_ILI9481_CMD_POWER_SETTING						0xD0
#define TFT_ILI9481_CMD_VCOM_CONTROL						0xD1
#define TFT_ILI9481_CMD_POWER_SETTING_FOR_NORMAL_MODE		0xD2

/* Orientacja LCD (domyslne: Piny do gory, SW na dole) */
typedef enum {
	TFT_ILI9481_Orientation_0deg,  	// Portrait orientation mode 1
	TFT_ILI9481_Orientation_90deg,  // Portrait orientation mode 2
	TFT_ILI9481_Orientation_180deg, // Landscape orientation mode 1
	TFT_ILI9481_Orientation_270deg  // Landscape orientation mode 2
} _lcdOrientation;

//Parametry LCD TFT
typedef struct {
	uint16_t width;
	uint16_t height;
	_lcdOrientation orientation;
}_lcdParams;

extern _lcdParams lcdParams;
extern _lcdOrientation lcdOrientation;

void TFT_ILI9481_SetActiveWindow(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);

void TFT_ILI9481_Write(uint16_t data);
void TFT_ILI9481_WriteCommand(uint16_t command);
void TFT_ILI9481_WriteData(uint16_t data);

void TFT_ILI9481_SetCursor(uint16_t Xpos, uint16_t Ypos);
void TFT_ILI9481_SetOrientationLCD(uint8_t orientation);
void TFT_ILI9481_ClearLCD(uint16_t Color);
void TFT_ILI9481_ResetLCD(void);
void TFT_ILI9481_InitLCD(uint16_t width, uint16_t height, _lcdOrientation orientation, uint16_t color);

/*
 * lcd_tft_ili9481.c
 *
 *  Created on: 15.03.2020
 *      Author: Mateusz
 */

#include "lcd_tft_ili9481.h"
#include "lcd_tft_graphics.h"
#include "delay.h"

_lcdParams lcdParams;
_lcdOrientation lcdOrientation;
//extern _graphicParams;



void TFT_ILI9481_SetActiveWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_SET_COLUMN_ADDRESS);
	TFT_ILI9481_WriteData(xStart>>8);
	TFT_ILI9481_WriteData(0x00FF&xStart);
	TFT_ILI9481_WriteData(xEnd>>8);
	TFT_ILI9481_WriteData(0x00FF&xEnd);

	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_SET_PAGE_ADDRESS);
	TFT_ILI9481_WriteData(yStart>>8);
	TFT_ILI9481_WriteData(0x00FF&yStart);
	TFT_ILI9481_WriteData(yEnd>>8);
	TFT_ILI9481_WriteData(0x00FF&yEnd);

	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_WRITE_MEMORY_START);
}

void TFT_ILI9481_Write(uint16_t data) {
	TFT_ILI9481_CS_Reset;

	uint16_t starszy = data & 0xFF00;
	uint16_t mlodszy = GPIO_ReadOutputData(GPIOB) & 0x00FF;

	GPIO_Write(GPIOA, data);
	GPIO_Write(GPIOB, (starszy | mlodszy));

	TFT_ILI9481_WR_Reset;
	TFT_ILI9481_WR_Set;

	TFT_ILI9481_CS_Set;
}

void TFT_ILI9481_WriteCommand(uint16_t command) {
	TFT_ILI9481_RS_Reset;
	TFT_ILI9481_Write(command);
}

void TFT_ILI9481_WriteData(uint16_t data) {
	TFT_ILI9481_RS_Set;
	TFT_ILI9481_Write(data);
}



void TFT_ILI9481_SetCursor(uint16_t Xpos, uint16_t Ypos) {
	TFT_ILI9481_SetActiveWindow(Xpos, Ypos, Xpos, Ypos);
}

void TFT_ILI9481_SetOrientationLCD(uint8_t orientation) {
	orientation = orientation % 4;	//Zabezpieczenie (wartosci od 0 do 3)

	switch(orientation){
		case 0: //Portrait	(Portret)
			lcdParams.width  = TFT_ILI9481_WIDTH;
			lcdParams.height = TFT_ILI9481_HEIGHT;
			TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_SET_ADDRESS_MODE);
			TFT_ILI9481_WriteData((1<<3)|(1<<6));				//0 degree PAO=0,CAO=1,P/CO=0,VO=0,RGBO=1,DO=0,HF=0,VF=0
		break;
		case 1:
			lcdParams.width  = TFT_ILI9481_HEIGHT;
			lcdParams.height = TFT_ILI9481_WIDTH;
			TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_SET_ADDRESS_MODE);
			TFT_ILI9481_WriteData((1<<3)|(1<<5));				//90 degree PAO=0,CAO=0,P/CO=1,VO=0,RGBO=1,DO=0,HF=0,VF=0
		break;
		case 2:
			lcdParams.width  = TFT_ILI9481_WIDTH;
			lcdParams.height = TFT_ILI9481_HEIGHT;
			TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_SET_ADDRESS_MODE);
			TFT_ILI9481_WriteData((1<<3)|(1<<7));				//180 degree PAO=1,CAO=0,P/CO=0,VO=0,RGBO=1,DO=0,HF=0,VF=0
		break;
		case 3: // Landscape (Krajobraz)
			lcdParams.width  = TFT_ILI9481_HEIGHT;
			lcdParams.height = TFT_ILI9481_WIDTH;
			TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_SET_ADDRESS_MODE);
			TFT_ILI9481_WriteData((1<<3)|(1<<5)|(1<<6)|(1<<7));	//270 degree PAO=1,CAO=1,P/CO=1,VO=0,RGBO=1,DO=0,HF=0,VF=0
		break;
		default:break;
	}
}

void TFT_ILI9481_ClearLCD(uint16_t color) {
	unsigned int i;

	TFT_ILI9481_SetActiveWindow(0, 0, lcdParams.width-1, lcdParams.height-1);

	for(i = 0; i < TFT_ILI9481_PIXELS; i++) {
		TFT_ILI9481_WriteData(color);
	}
}

void TFT_ILI9481_ResetLCD() {
	TFT_ILI9481_RST_Reset;
	delay_ms(100);
	TFT_ILI9481_RST_Set;
	delay_ms(50);
}

void TFT_ILI9481_InitLCD(uint16_t width, uint16_t height, _lcdOrientation orientation, uint16_t color) {
	lcdParams.width  = width;
	lcdParams.height = height;
	lcdParams.orientation = orientation;

	resetGraphicParams();

	TFT_ILI9481_ResetLCD();			//Hardware reset
	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_EXIT_SLEEP_MODE);	//Exit sleep mode
	delay_ms(20);
	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_POWER_SETTING);	//Power Setting
	TFT_ILI9481_WriteData(0x07);			//VC -
	TFT_ILI9481_WriteData(0x42);			//
	TFT_ILI9481_WriteData(0X18);			//
	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_VCOM_CONTROL);	//VCOM Control
	TFT_ILI9481_WriteData(0x00);			//
	TFT_ILI9481_WriteData(0x07);			//
	TFT_ILI9481_WriteData(0x10);			//
	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_POWER_SETTING_FOR_NORMAL_MODE);	//Power setting for Normal Mode
	TFT_ILI9481_WriteData(0x01);
	TFT_ILI9481_WriteData(0x02);
	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_PANEL_DRIVING_SETTING);	//Panel Driving Setting
	TFT_ILI9481_WriteData(0x10);
	TFT_ILI9481_WriteData(0x3B);
	TFT_ILI9481_WriteData(0x00);
	TFT_ILI9481_WriteData(0x02);
	TFT_ILI9481_WriteData(0x11);
	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_FRAME_RATE_AND_INVERSION_CONTROL);	//Frame Rate and Inversion Control
	TFT_ILI9481_WriteData(0x03);			//Frame rate = 75Hz
	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_GAMMA_SETTING);	//Gamma Setting
	TFT_ILI9481_WriteData(0x00);
	TFT_ILI9481_WriteData(0x32);
	TFT_ILI9481_WriteData(0x36);
	TFT_ILI9481_WriteData(0x45);
	TFT_ILI9481_WriteData(0x06);
	TFT_ILI9481_WriteData(0x16);
	TFT_ILI9481_WriteData(0x37);
	TFT_ILI9481_WriteData(0x75);
	TFT_ILI9481_WriteData(0x77);
	TFT_ILI9481_WriteData(0x54);
	TFT_ILI9481_WriteData(0x0C);
	TFT_ILI9481_WriteData(0x00);
	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_SET_ADDRESS_MODE);	//Set address mode
	TFT_ILI9481_WriteData(0x0A);
	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_SET_PIXEL_FORMAT);	//Set pixel format
	TFT_ILI9481_WriteData(0x55);			//16bit/pixel (65,536 colors)
	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_SET_COLUMN_ADDRESS);	//Set column address
	TFT_ILI9481_WriteData(0X00);
	TFT_ILI9481_WriteData(0X00);
	TFT_ILI9481_WriteData(0X01);
	TFT_ILI9481_WriteData(0X3F);
	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_SET_PAGE_ADDRESS);	//Set page address
	TFT_ILI9481_WriteData(0x00);
	TFT_ILI9481_WriteData(0x00);
	TFT_ILI9481_WriteData(0x01);
	TFT_ILI9481_WriteData(0xE0);
	delay_ms(120);

	TFT_ILI9481_SetOrientationLCD(lcdParams.orientation);
	TFT_ILI9481_ClearLCD(color);

	TFT_ILI9481_WriteCommand(TFT_ILI9481_CMD_SET_DISPLAY_ON); //Set display on
}

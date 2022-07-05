
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "lcd_tft_graphics.h"
#include "lcd_tft_ili9481.h"



#define min(X,Y) (((X) < (Y)) ? (X) : (Y))
#define max(X,Y) (((X) > (Y)) ? (X) : (Y))

extern _lcdParams lcdParams;
_alignVertical alignV;
_alignHorizontal alignH;
_graphicParams graphicParams;
_pointParams point;


//typedef struct {
//	uint16_t width;
//	uint16_t height;
//	TM_ILI9341_Orientation orientation; // 1 = portrait; 0 = landscape
//} TM_ILI931_Options_t;

/* Pin functions */
uint16_t ILI9341_x;
uint16_t ILI9341_y;

/*******************************************************************
 * @ Nazwa:
 * @ Data:		03.04.2020
 * @ Opis:
 * @ Parametry:	1) x:		Wspolrzedna punktu 'x'
 * @ Wynik:		Brak
********************************************************************/

/*******************************************************************
 * @ Nazwa:		void TFT_ILI9481_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
 * @ Data:		03.04.2020
 * @ Opis:		Rysuje piksel w zadanym punkcie i kolorze na LCD
 * @ Parametry:	1) x:		Wspolrzedna punktu 'x'
                2) y:		Wspolrzedna punktu 'y'
				3) color:	Kolor punktu
 * @ Wynik:		Brak
********************************************************************/
void TFT_ILI9481_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
	TFT_ILI9481_SetCursor(x, y);
	TFT_ILI9481_WriteData(color);
}

void TFT_ILI9481_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	int CurrentX, CurrentY, Xinc, Yinc,
		Dx, Dy, TwoDx, TwoDy,
		TwoDxAccumulatedError, TwoDyAccumulatedError;

	Dx = (x2-x1); Dy = (y2-y1);

	TwoDx = Dx + Dx; TwoDy = Dy + Dy;

	CurrentX = x1; CurrentY = y1;

	Xinc = 1; Yinc = 1;

	if(Dx < 0) {
		Xinc = -1;
		Dx = -Dx;
		TwoDx = -TwoDx;
	}

	if (Dy < 0) {
		Yinc = -1;
		Dy = -Dy;
		TwoDy = -TwoDy;
	}

	TFT_ILI9481_DrawPixel(x1, y1, color);

	if ((Dx != 0) || (Dy != 0)) {

		if (Dy <= Dx) {
			TwoDxAccumulatedError = 0;
			do {
			    CurrentX += Xinc;
			    TwoDxAccumulatedError += TwoDy;
			    if(TwoDxAccumulatedError > Dx) {
			        CurrentY += Yinc;
			        TwoDxAccumulatedError -= TwoDx;
			    }
			    TFT_ILI9481_DrawPixel(CurrentX, CurrentY, color);
			  } while (CurrentX != x2);
		  } else {
			  TwoDyAccumulatedError = 0;
			  do {
			      CurrentY += Yinc;
			      TwoDyAccumulatedError += TwoDx;
			      if(TwoDyAccumulatedError>Dy) {
				      CurrentX += Xinc;
				      TwoDyAccumulatedError -= TwoDy;
			      }
			      TFT_ILI9481_DrawPixel(CurrentX, CurrentY, color);
			  } while (CurrentY != y2);
		  }
	}
}

void TFT_ILI9481_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
	// (x1,y1) *------------* (x2,y2)
	//		   |			|
	//		   |			|
	// (x4,y4) *------------* (x3,y3)

	TFT_ILI9481_DrawLine(x, y, x+width-1, y, color);					//(x1,y1) <-> (x2,y2)
	TFT_ILI9481_DrawLine(x+width-1, y, x+width-1, y+height-1, color);		//(x2,y2) <-> (x3,y3)
	TFT_ILI9481_DrawLine(x+width-1, y+height-1, x, y+height-1, color);	//(x3,y3) <-> (x4,y4)
	TFT_ILI9481_DrawLine(x, y+height-1, x, y, color);					//(x4,y4) <-> (x1,y1)
}

void TFT_ILI9481_DrawCircle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t color) {
	   int X, Y, Xchange, YChange, RadiusError;

	   X = r;
	   Y = 0;
	   Xchange = 1 - 2*r;
	   YChange = 1;
	   RadiusError = 0;
	   while( X >= Y ) {
		   TFT_ILI9481_DrawPixel(xc+X, yc+Y, color);
		   TFT_ILI9481_DrawPixel(xc-X, yc+Y, color);
		   TFT_ILI9481_DrawPixel(xc-X, yc-Y, color);
		   TFT_ILI9481_DrawPixel(xc+X, yc-Y, color);
		   TFT_ILI9481_DrawPixel(xc+Y, yc+X, color);
		   TFT_ILI9481_DrawPixel(xc-Y, yc+X, color);
		   TFT_ILI9481_DrawPixel(xc-Y, yc-X, color);
		   TFT_ILI9481_DrawPixel(xc+Y, yc-X, color);

	        Y++;
	        RadiusError += YChange;
	        YChange+=2;
	        if ( 2*RadiusError + Xchange > 0 ) {
	              X--;
	              RadiusError+=Xchange;
	              Xchange+=2;
	        }
	   }
}

////////////////////////////////////////////////
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
	uint16_t i,j;
	uint16_t width=ex-sx+1;
	uint16_t height=ey-sy+1;
	TFT_ILI9481_SetActiveWindow(sx,sy,ex,ey);
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
			TFT_ILI9481_WriteData(color);
	}
	TFT_ILI9481_SetActiveWindow(0,0,TFT_ILI9481_WIDTH-1,TFT_ILI9481_HEIGHT-1);
}

static void _swap(uint16_t *a, uint16_t *b)
{
	uint16_t tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}
void Fill_Triangel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color)
{
	uint16_t a, b, y, last;
	int dx01, dy01, dx02, dy02, dx12, dy12;
	long sa = 0;
	long sb = 0;
 	if (y0 > y1) {
 		_swap(&y0,&y1);
		_swap(&x0,&x1);
 	}
 	if (y1 > y2) {
 		_swap(&y2,&y1);
		_swap(&x2,&x1);
 	}
 	if (y0 > y1) {
 		_swap(&y0,&y1);
		_swap(&x0,&x1);
 	}
	if(y0 == y2) {
		a = b = x0;
		if(x1 < a) {
			a = x1;
		}
		else if(x1 > b) {
			b = x1;
		}
		if(x2 < a) {
			a = x2;
		}
		else if(x2 > b) {
			b = x2;
		}
			LCD_Fill(a,y0,b,y0,color);
		return;
	}
	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;

	if(y1 == y2) {
		last = y1;
	}
	else {
		last = y1-1;
	}
	for(y=y0; y<=last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		if(a > b) {
			_swap(&a,&b);
		}
		LCD_Fill(a,y,b,y,color);
	}
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if(a > b) {
			_swap(&a,&b);
		}
		LCD_Fill(a,y,b,y,color);
	}
}


long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// #########################################################################
//  Draw the meter on the screen, returns x coord of righthand side
// #########################################################################
int ringMeter(int value, int vmin, int vmax, int x, int y, int r, uint8_t scheme,int sp, int segm, int wid)
{
	// Minimum value of r is about 52 before value text intrudes on ring
	// drawing the text first is an option

	x += r; y += r;   // Calculate coords of centre of ring

	int w = r / wid;    // Width of outer ring is 1/4 of radius
	sp = sp/2;
	int angle = sp;  // Half the sweep angle of meter (300 degrees)

	int v = map(value, vmin, vmax, -angle, angle); // Map the value to an angle v

	uint8_t seg = 5; // Segments are 5 degrees wide = 60 segments for 300 degrees
	uint8_t inc = 5*segm; // Draw segments every 5 degrees, increase to 10 for segmented ring

	// Draw colour blocks every inc degrees
	for(int i = -angle; i < angle; i += inc) {
		int colour = 0;	// Choose colour from scheme
		switch (scheme) {
    		case 0:  colour = TFT_ILI9481_COLOR_RED; break;						//Fixed colour
    		case 1:  colour = TFT_ILI9481_COLOR_GREEN; break;					//Fixed colour
    		case 2:  colour = TFT_ILI9481_COLOR_BLUE; break; 					//Fixed colour
    		case 3:  colour = rainbow(map(i, -angle, angle,   0, 127)); break;	//Full spectrum blue to red
    		case 4:  colour = rainbow(map(i, -angle, angle,  63, 127)); break;	//Green to red (high temperature etc)
    		case 5:  colour = rainbow(map(i, -angle, angle, 127,  63)); break;	//Red to green (low battery etc)
    		default: colour = TFT_ILI9481_COLOR_BLUE; break;					//Fixed colour
		}

		// Calculate pair of coordinates for segment start
		float sx = cos((i - 90) * 0.0174532925);
		float sy = sin((i - 90) * 0.0174532925);
		uint16_t x0 = sx * (r - w) + x;
		uint16_t y0 = sy * (r - w) + y;
		uint16_t x1 = sx * r + x;
		uint16_t y1 = sy * r + y;

		// Calculate pair of coordinates for segment end
		float sx2 = cos((i + seg - 90) * 0.0174532925);
		float sy2 = sin((i + seg - 90) * 0.0174532925);
		int x2 = sx2 * (r - w) + x;
		int y2 = sy2 * (r - w) + y;
		int x3 = sx2 * r + x;
		int y3 = sy2 * r + y;

		if (i < v) { // Fill in coloured segments with 2 triangles
			Fill_Triangel(x0, y0, x1, y1, x2, y2, colour);
			Fill_Triangel(x1, y1, x2, y2, x3, y3, colour);
		}
		else { // Fill in blank segments
			Fill_Triangel(x0, y0, x1, y1, x2, y2, TFT_ILI9481_COLOR_DARKGREY);
			Fill_Triangel(x1, y1, x2, y2, x3, y3, TFT_ILI9481_COLOR_DARKGREY);
		}
	}
  return x + r;	//Calculate and return right hand side x coordinate
}


void ringMeterTemp(float value, float vmin, float vmax, char *name, uint16_t x, uint16_t y, _alignHorizontal alignH, _alignVertical alignV) {
	char tab[10];
	uint16_t xRingMeter;
	uint16_t yRingMeter;

	//Wyrownaj w poziomie
	switch(alignH) {
		case alignHLeft:	xRingMeter = x; 	  break;
		case alignHCenter:	xRingMeter = x -  79; break;
		case alignHRight:	xRingMeter = x - 159; break;
	}
	//Wyrownaj w pionie
	switch(alignV) {
		case alignVTop:		yRingMeter = y;		  break;
		case alignVMiddle:	yRingMeter = y -  79; break;
		case alignVBottom:	yRingMeter = y - 159; break;
	}

	if(value >= 100.0) { value = 99.9; }
	sprintf(tab, "%2.1f", value);

	TFT_ILI9481_DrawRectangle(xRingMeter, yRingMeter, 160, 160, TFT_ILI9481_COLOR_DARKGREY);
	TM_ILI9341_Puts(xRingMeter+80, yRingMeter+ 76,    name, &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(xRingMeter+80, yRingMeter+112, "     ", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(xRingMeter+80, yRingMeter+112,     tab, &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(xRingMeter+80, yRingMeter+148,  "o", &TM_Font_11x18, alignHRight, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(xRingMeter+80, yRingMeter+153,  "C", &TM_Font_11x18, alignHLeft, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	ringMeter((int)value, vmin, vmax, xRingMeter+4, yRingMeter+4, 76, 3, 310, 1, 5);
}

void ringMeterTempRPM(float value1, float vmin1, float vmax1, float value2, float vmin2, float vmax2, char *name, uint16_t x, uint16_t y, _alignHorizontal alignH, _alignVertical alignV) {
	char tab1[10];
	char tab2[10];
	uint16_t xRingMeter;
	uint16_t yRingMeter;

	//Wyrownaj w poziomie
	switch(alignH) {
		case alignHLeft:	xRingMeter = x; 	  break;
		case alignHCenter:	xRingMeter = x -  79; break;
		case alignHRight:	xRingMeter = x - 159; break;
	}
	//Wyrownaj w pionie
	switch(alignV) {
		case alignVTop:		yRingMeter = y;		  break;
		case alignVMiddle:	yRingMeter = y -  79; break;
		case alignVBottom:	yRingMeter = y - 159; break;
	}

	if(value2 >= 100.0) { value2 = 99.9; }
	sprintf(tab1, "%.0f", value1);
	sprintf(tab2, "%2.1f", value2);

	TFT_ILI9481_DrawRectangle(xRingMeter, yRingMeter, 160, 160, TFT_ILI9481_COLOR_DARKGREY);
	TM_ILI9341_Puts(xRingMeter+ 80, yRingMeter+ 68, name, &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(xRingMeter+ 80, yRingMeter+ 94, "     ", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(xRingMeter+ 80, yRingMeter+120, "    ", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(xRingMeter+ 80, yRingMeter+ 94, tab1, &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(xRingMeter+ 80, yRingMeter+120, tab2, &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(xRingMeter+ 80, yRingMeter+140, "RPM", &TM_Font_11x18, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(xRingMeter+80, yRingMeter+153,  "o", &TM_Font_11x18, alignHRight, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(xRingMeter+80, yRingMeter+158,  "C", &TM_Font_11x18, alignHLeft, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	//TM_ILI9341_Puts(xRingMeter+ 80, yRingMeter+158, "TEMP", &TM_Font_11x18, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	//TM_ILI9341_Puts(xRingMeter+ 41, yRingMeter+158, "0.0V", &TM_Font_7x10, alignHRight, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	//TM_ILI9341_Puts(xRingMeter+154, yRingMeter+158, "0.00A", &TM_Font_7x10, alignHRight, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	ringMeter(value1, 0, 100, xRingMeter+ 4, yRingMeter+ 4, 76, 3, 310, 1, 8);
	ringMeter(value2, 0, 100, xRingMeter+20, yRingMeter+20, 60, 4, 310, 1, 7);
}

void ringMeterInit1() {
	//Frame
//	TFT_ILI9481_DrawLine(79, 0, 79, 159, TFT_ILI9481_COLOR_DARKGREY);
//	TFT_ILI9481_DrawRectangle( 80, 0, 160, 160, TFT_ILI9481_COLOR_DARKGREY);
//	TFT_ILI9481_DrawLine(400, 0, 400, 159, TFT_ILI9481_COLOR_DARKGREY);
//	TFT_ILI9481_DrawRectangle(240, 0, 160, 160, TFT_ILI9481_COLOR_DARKGREY);
//	TFT_ILI9481_DrawRectangle(  0, 160, 160, 160, TFT_ILI9481_COLOR_DARKGREY);
//	TFT_ILI9481_DrawRectangle(160, 160, 160, 160, TFT_ILI9481_COLOR_DARKGREY);
//	TFT_ILI9481_DrawRectangle(320, 160, 160, 160, TFT_ILI9481_COLOR_DARKGREY);
//	TFT_ILI9481_DrawLine(0, 159, 78, 159, TFT_ILI9481_COLOR_DARKGREY);
//	TFT_ILI9481_DrawLine(401, 159, 479, 159, TFT_ILI9481_COLOR_DARKGREY);
	//
	TM_ILI9341_Puts(160,  68,   "CPU", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(160,  94,     "0", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(160, 120,   "0.0", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(160, 153,     "o", &TM_Font_11x18, alignHRight,  alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(160, 158,     "C", &TM_Font_11x18, alignHLeft,   alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(160, 139,   "RPM", &TM_Font_11x18, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	//
	TM_ILI9341_Puts(320,  68,   "GPU", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(320,  94,     "0", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(320, 120,   "0.0", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(320, 153,     "o", &TM_Font_11x18, alignHRight,  alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(320, 158,     "C", &TM_Font_11x18, alignHLeft,   alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(320, 139,   "RPM", &TM_Font_11x18, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	//
	TM_ILI9341_Puts( 80, 236,   "SSD", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts( 80, 272,   "0.0", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts( 80, 308,     "o", &TM_Font_11x18, alignHRight,  alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts( 80, 313,     "C", &TM_Font_11x18, alignHLeft,   alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	//
	TM_ILI9341_Puts(240, 236,   "HDD", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(240, 272,   "0.0", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(240, 308,     "o", &TM_Font_11x18, alignHRight,  alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(240, 313,     "C", &TM_Font_11x18, alignHLeft,   alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	//
	TM_ILI9341_Puts(400, 236, "BOARD", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(400, 272,   "0.0", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(400, 308,     "o", &TM_Font_11x18, alignHRight,  alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(400, 313,     "C", &TM_Font_11x18, alignHLeft,   alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);

	ringMeter(0, 20, 100,  84,   4, 76, 3, 310, 1, 8);	//Temp
	ringMeter(0,  0, 100, 100,  20, 60, 4, 310, 1, 7);	//RPM
	ringMeter(0, 20, 100, 244,   4, 76, 3, 310, 1, 8);	//Temp
	ringMeter(0,  0, 100, 260,  20, 60, 4, 310, 1, 7);	//RPM
	ringMeter(0, 20, 100,   4, 164, 76, 3, 310, 1, 5);	//Temp
	ringMeter(0, 20, 100, 164, 164, 76, 3, 310, 1, 5);	//Temp
	ringMeter(0, 20, 100, 324, 164, 76, 3, 310, 1, 5);	//Temp
}

void ringMeterSetParams1(float rpm1, float temp1, float rpm2, float temp2, float temp3, float temp4, float temp5) {
	char tab1[10], tab2[10], tab3[10], tab4[10], tab5[10], tab6[10], tab7[10];

	if(temp1 >= 100.0) { temp1 = 99.9; }
	if(temp2 >= 100.0) { temp2 = 99.9; }
	if(temp3 >= 100.0) { temp3 = 99.9; }
	if(temp4 >= 100.0) { temp4 = 99.9; }
	if(temp5 >= 100.0) { temp5 = 99.9; }

	sprintf(tab1, "%.0f",   rpm1);
	sprintf(tab2, "%2.1f", temp1);
	sprintf(tab3, "%.0f",   rpm2);
	sprintf(tab4, "%2.1f", temp2);
	sprintf(tab5, "%2.1f", temp3);
	sprintf(tab6, "%2.1f", temp4);
	sprintf(tab7, "%2.1f", temp5);

	ringMeter(temp1, 20,  100,  84,   4, 76, 3, 310, 1, 8);	//Temp
	ringMeter(rpm1,   0, 3000, 100,  20, 60, 4, 310, 1, 7);	//RPM
	TM_ILI9341_Puts(160,  94, "    ", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(160,  94, tab1, &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(160, 120, "    ", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(160, 120, tab2, &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);

	ringMeter(temp2, 20,  100, 244,   4, 76, 3, 310, 1, 8);	//Temp
	ringMeter(rpm2,   0, 3000, 260,  20, 60, 4, 310, 1, 7);	//RPM
	TM_ILI9341_Puts(320,  94, "    ", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(320,  94, tab3, &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(320, 120, "    ", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(320, 120, tab4, &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);

	ringMeter(temp3, 20,  100,   4, 164, 76, 3, 310, 1, 5);	//Temp
	TM_ILI9341_Puts( 80, 272, "    ", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts( 80, 272, tab5, &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);

	ringMeter(temp4, 20,  100, 164, 164, 76, 3, 310, 1, 5);	//Temp
	TM_ILI9341_Puts(240, 272, "    ", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(240, 272, tab6, &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);

	ringMeter(temp5, 20,  100, 324, 164, 76, 3, 310, 1, 5);	//Temp
	TM_ILI9341_Puts(400, 272, "    ", &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
	TM_ILI9341_Puts(400, 272, tab7, &TM_Font_16x26, alignHCenter, alignVBottom, TFT_ILI9481_COLOR_WHITE, TFT_ILI9481_COLOR_BLACK);
}

// #########################################################################
// Return a 16 bit rainbow colour
// #########################################################################
unsigned int rainbow(uint8_t value) {
	// Value is expected to be in range 0-127
	// The value is converted to a spectrum colour from 0 = blue through to 127 = red

	uint8_t red = 0; // Red is the top 5 bits of a 16 bit colour value
	uint8_t green = 0;// Green is the middle 6 bits
	uint8_t blue = 0; // Blue is the bottom 5 bits

	uint8_t quadrant = value / 32;

	if (quadrant == 0) {
		blue = 31;
		green = 2 * (value % 32);
		red = 0;
	}
	if (quadrant == 1) {
		blue = 31 - (value % 32);
		green = 63;
		red = 0;
	}
	if (quadrant == 2) {
		blue = 0;
		green = 63;
		red = value % 32;
	}
	if (quadrant == 3) {
		blue = 0;
		green = 63 - 2 * (value % 32);
		red = 31;
	}
	return (red << 11) + (green << 5) + blue;
}



//void TFT_ILI9481_Bitmap(int x, int y, const uint8_t * glyph, int width, int height) {
//	uint8_t cred, cgreen, cblue;
//	uint16_t idx = 0;
//    y += frame_ptr;
//    TFT_ILI9481_SetActiveWindow(x, y, x+width-1, y+height-1);
//    //TFT_ILI9481_WriteCommand(0x2c);
//
//	while( idx < width*height*3 ) {
//		cred = &glyph[ idx++ ];
//		cgreen = &glyph[ idx++ ];
//		cblue = &glyph[ idx++ ];
//		Set_color(cred, cgreen, cblue);
//		Draw_pixel();
//	}
//}

//void TFT_ILI9481_DrawLine3(int x0, int y0, int x1, int y1, float thickness, uint16_t color)
//{
//	int dx = abs(x1-x0), sx = x0 < x1 ? 1 : -1;
//	int dy = abs(y1-y0), sy = y0 < y1 ? 1 : -1;
//	int err = dx-dy, e2, x2, y2;                          /* error value e_xy */
//	float ed = dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);
//
//	for (thickness = (thickness+1)/2; ; ) {                                   /* pixel loop */
//		//if(max(0,255*(abs(err-dx+dy)/ed-thickness+1)) > 128) {
//			TFT_ILI9481_DrawPixel(x0,y0, color);
//		//}
//		//setPixelColor(x0,y0,max(0,255*(abs(err-dx+dy)/ed-thickness+1)));
//		e2 = err; x2 = x0;
//		if (2*e2 >= -dx) {                                           /* x step */
//			for (e2 += dy, y2 = y0; e2 < ed*thickness && (y1 != y2 || dx > dy); e2 += dx)
//
//				//if(max(0,255*(abs(e2)/ed-thickness+1)) > 128) {
//					TFT_ILI9481_DrawPixel(x0, y2 += sy, color);
//				//}
//				//setPixelColor(x0, y2 += sy, max(0,255*(abs(e2)/ed-thickness+1)));
//				if (x0 == x1) break;
//				e2 = err; err -= dy; x0 += sx;
//		}
//		if (2*e2 <= dy) {                                            /* y step */
//			for (e2 = dx-e2; e2 < ed*thickness && (x1 != x2 || dx < dy); e2 += dy)
//
//				//if(max(0,255*(abs(e2)/ed-thickness+1)) > 128) {
//					TFT_ILI9481_DrawPixel(x2 += sx, y0, color);
//				//}
//				//setPixelColor(x2 += sx, y0, max(0,255*(abs(e2)/ed-thickness+1)));
//				if (y0 == y1) break;
//				err += dx; y0 += sy;
//		}
//	}
//}
//
//void TFT_ILI9481_GenerateBrush(uint16_t *brush, uint16_t thickness, uint8_t typeOfBrusch) {
///***********************************************************
// *						THICKNESS
// *   1    2    3      4      5        6        7
// * CIRCLE BRUSH
// *									 11		  111
// *					 11		 1		1111	 11111
// *		 11	   1	1111	111	   111111	1111111
// *   x   1x   1x1   11x1   11x11   111x11   111x111
// *	 		   1	 11		111		1111	1111111
// *							 1		 11		 11111
// *							 				  111
// * RECTANGLE
// *								   111111   1111111
// *					1111   11111   111111	1111111
// *		 11	  111	1111   11111   111111	1111111
// *   x   1x   1x1   11x1   11x11   111x11   111x111
// *	 		  111	1111   11111   111111	1111111
// *						   11111   111111	1111111
// *							 				1111111
// *
// * x - punkt poczatkowy (0, 0)
//*/
//
//
//
//}
//
//
//void TFT_ILI9481_DrawLine2(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t thickness, uint16_t color) {
//	point.x = 0;
//	point.y = 0;
//	_pointParams brush[thickness][thickness];	//Maska glowna w ukladzie lokalnym
//	_pointParams lastBrush[thickness][thickness];	//Maska poprzednia w ukladzie globalnym
//
//	//Generowanie pustej maski
//		for(int8_t r = 0; r < thickness; r++) {
//			for(int8_t c = 0; c < thickness; c++) {
//				brush[r][c] = point;
//			}
//		}
//
//		float helpValue = ((float)thickness - 1.0) / 2.0;
//		int16_t brushTabStart = (int16_t)ceil(helpValue);
//		//int16_t brushTabStop  = (int16_t)floor(helpValue);
//
//	//Generacja maski kwadratu
//		for(int8_t r = 0; r < thickness; r++) {
//			for(int8_t c = 0; c < thickness; c++) {
//				point.x = (int16_t)c - brushTabStart;
//				point.y = (int16_t)r - brushTabStart;
//
//				brush[r][c] = point;
//			}
//		}
//
//
//
//	int16_t x1_c = x1, y1_c = y1, x2_c = x2, y2_c = y2;
//	//int x3, y3;
//	//int16_t x1_l, y1_l, x2_l, y2_l, x3_l, y3_l;	//Wektor pomocniczy do rysowania grubosci linii (zawsze prostopadly do linii)
//	float theta = 0.0;
//	int16_t a = thickness / 2; //pomocnicza
//
//	int dx, dy, sx, sy, err, e2;
//
//	for(uint16_t i = 0; i < thickness; i++) {
//		//Przejœcie (x1,y1) i (x2,y2) do lokalnego ukladu wsp.
////		x1_l =  0; y1_l =  0;
////		x2_l = x2; y2_l = y2;
////		pointToLocal(&x2_l, &y2_l, x1, y1);
//		//Wyznaczenie kata prostej w lokalnym ukladzie wsp.
//		//theta = atan2f((float)y2_l, (float)x2_l);
////		theta = atan2f((float)y2-(float)y1, (float)x2-(float)x1);
////
////		if(theta <= 0.0 && theta >= degToRad(-45.0)) {
////			x1_c = x1; y1_c = (int16_t)y1 - a;
////			x2_c = x2; y2_c = (int16_t)y2 - a;
////		}
//
////		x3_l = a; y3_l = 0;	//Wektor pomocniczy (jako grubosc lini)
////		rotatePoint(&x3_l, &y3_l, theta+M_PI_2);
////
////		x1_c = x1 + x3_l; y1_c = y1 - y3_l;
////		x2_c = x2 + x3_l; y2_c = y2 - y3_l;
//
//		dx =  abs(x2_c-x1_c); sx = x1_c < x2_c ? 1 : -1;
//		dy = -abs(y2_c-y1_c); sy = y1_c < y2_c ? 1 : -1;
//		err = dx+dy, e2 = 0;
//
//		for(int8_t r = 0; r < thickness; r++) {
//			for(int8_t c = 0; c < thickness; c++) {
//				point = brush[r][c];
//
//				int16_t xNew = point.x;
//				int16_t yNew = point.y;
//
//				pointToGlobal(&xNew, &yNew, x1_c, y1_c);
//
//				if( (xNew >= 0 && xNew <= TFT_ILI9481_WIDTH) && (yNew >= 0 && yNew <= TFT_ILI9481_HEIGHT) ) {
//					TFT_ILI9481_DrawPixel(xNew, yNew, color);	//Rysuj px tylko jesli jest w aktywnym oknie LCD
//				}
//
//				point.x = xNew;
//				point.y = yNew;
//				lastBrush[r][c] = point;
//			}
//		}
//
//
//		for(;;) {
//			e2 = 2*err;
//			if (e2 >= dy) {	/* e_xy+e_x > 0 */
//				if (x1_c == x2_c) break;
//				err += dy;
//				x1_c  += sx;
//			}
//			if (e2 <= dx) {	/* e_xy+e_y < 0 */
//				if (y1_c == y2_c) break;
//				err += dx;
//				y1_c  += sy;
//			}
//
//			for(int8_t r = 0; r < thickness; r++) {
//				for(int8_t c = 0; c < thickness; c++) {
//					point = brush[r][c];
//
//					int16_t xNew = point.x;
//					int16_t yNew = point.y;
//
//					pointToGlobal(&xNew, &yNew, x1_c, y1_c);
//
//					if( (xNew >= 0 && xNew <= TFT_ILI9481_WIDTH) && (yNew >= 0 && yNew <= TFT_ILI9481_HEIGHT) ) {
//						TFT_ILI9481_DrawPixel(xNew, yNew, color);	//Rysuj px tylko jesli jest w aktywnym oknie LCD
//					}
//
//				}
//			}
//		}
//	}
//}


//void TFT_ILI9481_DrawLine2(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t thickness, uint16_t color) {
//
//
//	int16_t x1_c = x1, y1_c = y1, x2_c = x2, y2_c = y2;
//	//int x3, y3;
//	int16_t x1_l, y1_l, x2_l, y2_l, x3_l, y3_l;	//Wektor pomocniczy do rysowania grubosci linii (zawsze prostopadly do linii)
//	float theta = 0.0;
//	int16_t a = thickness / 2; //pomocnicza
//
//	int dx, dy, sx, sy, err, e2;
//
//	for(uint16_t i = 0; i < thickness; i++) {
//		//Przejœcie (x1,y1) i (x2,y2) do lokalnego ukladu wsp.
//		x1_l =  0; y1_l =  0;
//		x2_l = x2; y2_l = y2;
//		pointToLocal(&x2_l, &y2_l, x1, y1);
//		//Wyznaczenie kata prostej w lokalnym ukladzie wsp.
//		theta = atan2f((float)y2_l, (float)x2_l);
//
//		x3_l = a; y3_l = 0;	//Wektor pomocniczy (jako grubosc lini)
//		rotatePoint(&x3_l, &y3_l, theta+M_PI_2);
//
//		x1_c = x1 + x3_l; y1_c = y1 - y3_l;
//		x2_c = x2 + x3_l; y2_c = y2 - y3_l;
//
//
//		dx =  abs(x2_c-x1_c); sx = x1_c < x2_c ? 1 : -1;
//		dy = -abs(y2_c-y1_c); sy = y1_c < y2_c ? 1 : -1;
//		err = dx+dy, e2 = 0;
//
//		for(;;) {
//
//			if(a ==  2) { color = TFT_ILI9481_COLOR_BLUE; }
//			if(a ==  1) { color = TFT_ILI9481_COLOR_GREEN2; }
//			if(a ==  0) { color = TFT_ILI9481_COLOR_WHITE; }
//			if(a == -1) { color = TFT_ILI9481_COLOR_RED; }
//
//
//			TFT_ILI9481_DrawPixel(x1_c, y1_c, color);
//			e2 = 2*err;
//			if (e2 >= dy) {	/* e_xy+e_x > 0 */
//				if (x1_c == x2_c) break;
//				err += dy;
//				x1_c  += sx;
//			}
//			if (e2 <= dx) {	/* e_xy+e_y < 0 */
//				if (y1_c == y2_c) break;
//				err += dx;
//				y1_c  += sy;
//			}
//		}
//
//		--a;
//	}
//}

/*******************************************************************
 * @ Nazwa:		void TFT_ILI9481_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
 * @ Data:		03.04.2020
 * @ Opis:	Rysuje piksel w zadanym punkcie i kolorze na LCD
 * @ Parametry:	1) x:		Wspolrzedna punktu 'x'
                2) y:		Wspolrzedna punktu 'y'
				3) color:	Kolor punktu
 * @ Wynik:		Brak
********************************************************************/



//void TFT_ILI9481_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, _graphicParams graphParams)
//{
//	uint16_t points[4][2];	//Definicja 4 punktow (x,y)
//
//		points[0][0] = x;			points[0][1] = y;			// (x1,y1) *------------* (x2,y2)
//		points[1][0] = x + width;	points[1][1] = y;			//		   |			|
//		points[2][0] = x + width;	points[2][1] = y + height;	//		   |			|
//		points[3][0] = x;			points[3][1] = y + height;	// (x4,y4) *------------* (x3,y3)
//
//	uint16_t x1, y1, x2, y2, x3, y3, x4, y4;	//Nowe pkt
//
//
//
//	//Wyznaczenie nowych punktow wyrownania w pionie i poziomie
//	switch(graphParams.alignH) {
//		uint16_t w_2 = 0;
//
//		case alignHLeft:
//			points[0][0] = x;
//			points[1][0] = x + width;
//			points[2][0] = points[1][0];
//			points[3][0] = points[0][0];
//			break;
//		case alignHCenter:
//			w_2 = (uint16_t)round((float)width / 2.0);
//			points[0][0] = x - w_2;
//			points[1][0] = points[0][0] + width;
//			points[2][0] = points[1][0];
//			points[3][0] = points[0][0];
//			break;
//		case alignHRight:
//			points[0][0] = x - width;
//			points[1][0] = x;
//			points[2][0] = points[1][0];
//			points[3][0] = points[0][0];
//			break;
//	}
//
//	//Wyznaczenie nowych punktow wyrownania w pionie i poziomie
//	switch(graphParams.alignV) {
//		uint16_t h_2 = 0;
//
//		case alignVTop:
//			points[0][1] = y;
//			points[1][1] = points[0][1];
//			points[2][1] = y + height;
//			points[3][1] = points[2][1];
//			break;
//		case alignVMiddle:
//			h_2 = (uint16_t)round((float)height / 2.0);
//			points[0][1] = y - h_2;
//			points[1][1] = points[0][1];
//			points[2][1] = points[0][1] + height;
//			points[3][1] = points[2][1];
//			break;
//		case alignVBottom:
//			points[0][1] = y - height;
//			points[1][1] = points[0][1];
//			points[2][1] = y;
//			points[3][1] = points[2][1];
//			break;
//	}
//
//	//Wyznaczenie nowych punktow po obrocie
//	graphParams.rotation = setPrecision(graphParams.rotation, 5);
//	if(graphParams.rotation != 0) {	//Obracam
//		int16_t x1_l=x1, y1_l=y1, x2_l=x2, y2_l=y2, x3_l=x3, y3_l=y3, x4_l=x4, y4_l=y4;	//Nowe pkt lokalne wzgledem pkt (0,0)
//
//		pointToLocal(&x1_l, &y1_l, x, y);
//		pointToLocal(&x2_l, &y2_l, x, y);
//		pointToLocal(&x3_l, &y3_l, x, y);
//		pointToLocal(&x4_l, &y4_l, x, y);
//
//		rotatePoint(&x1_l, &y1_l, graphParams.rotation);
//		rotatePoint(&x2_l, &y2_l, graphParams.rotation);
//		rotatePoint(&x3_l, &y3_l, graphParams.rotation);
//		rotatePoint(&x4_l, &y4_l, graphParams.rotation);
//
//		pointToGlobal(&x1_l, &y1_l, x, y);
//		pointToGlobal(&x2_l, &y2_l, x, y);
//		pointToGlobal(&x3_l, &y3_l, x, y);
//		pointToGlobal(&x4_l, &y4_l, x, y);
//
//		x1=x1_l; y1=y1_l;
//		x2=x2_l; y2=y2_l;
//		x3=x3_l; y3=y3_l;
//		x4=x4_l; y4=y4_l;
//	}
//
////	TFT_ILI9481_DrawLine(x1, y1, x2, y2, graphParams.thickness, color);
////	TFT_ILI9481_DrawLine(x2, y2, x3, y3, graphParams.thickness, color);
////	TFT_ILI9481_DrawLine(x3, y3, x4, y4, graphParams.thickness, color);
////	TFT_ILI9481_DrawLine(x4, y4, x1, y1, graphParams.thickness, color);
//}



void TM_ILI9341_Putc(uint16_t x, uint16_t y, char c, TM_FontDef_t *font, uint32_t foreground, uint32_t background) {
	uint32_t i, b, j;
	/* Set coordinates */
	ILI9341_x = x;
	ILI9341_y = y;

	if ((ILI9341_x + font->FontWidth) > lcdParams.width) {
		/* If at the end of a line of display, go to new line and set x to 0 position */
		ILI9341_y += font->FontHeight;
		ILI9341_x = 0;
	}

	/* Draw rectangle for background */
	LCD_Fill(ILI9341_x, ILI9341_y, ILI9341_x + font->FontWidth, ILI9341_y + font->FontHeight, background);

	/* Draw font data */
	for (i = 0; i < font->FontHeight; i++) {
		b = font->data[(c - 32) * font->FontHeight + i];
		for (j = 0; j < font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				TFT_ILI9481_DrawPixel(ILI9341_x + j, (ILI9341_y + i), foreground);
			}
		}
	}

	/* Set new pointer */
	ILI9341_x += font->FontWidth;
}

void TM_ILI9341_Puts(uint16_t x, uint16_t y, char *str, TM_FontDef_t *font, _alignHorizontal alignH, _alignVertical alignV, uint32_t foreground, uint32_t background) {
	uint16_t startX = x;
	TM_FONTS_SIZE_t fontsSize;

	ILI9341_x = x;
	ILI9341_y = y;

	TM_FONTS_GetStringSize(str, &fontsSize, font);

	//Wyrownaj w poziomie
	switch(alignH) {
		uint16_t w_2 = 0;

		case alignHLeft:
			ILI9341_x = x;
			break;
		case alignHCenter:
			w_2 = (uint16_t)round((float)fontsSize.Length / 2.0);
			ILI9341_x = x - w_2;
			break;
		case alignHRight:
			ILI9341_x = x - fontsSize.Length;
			break;
	}

	//Wyrownaj w pionie
	switch(alignV) {
		uint16_t h_2 = 0;

		case alignVTop:
			ILI9341_y = y;
			break;
		case alignVMiddle:
			h_2 = (uint16_t)round((float)fontsSize.Height / 2.0);
			ILI9341_y = y - h_2;
			break;
		case alignVBottom:
			ILI9341_y = y - fontsSize.Height;
			break;
	}

	while (*str) {
		/* New line */
		if (*str == '\n') {
			ILI9341_y += font->FontHeight + 1;
			/* if after \n is also \r, than go to the left of the screen */
			if (*(str + 1) == '\r') {
				ILI9341_x = 0;
				str++;
			} else {
				ILI9341_x = startX;
			}
			str++;
			continue;
		} else if (*str == '\r') {
			str++;
			continue;
		}

		/* Put character to LCD */
		TM_ILI9341_Putc(ILI9341_x, ILI9341_y, *str++, font, foreground, background);
	}
}


void pointToLocal(int16_t *x_l, int16_t *y_l, uint16_t x, uint16_t y) {
	*x_l = *x_l - x;
	*y_l = *y_l - y;
}

void pointToGlobal(int16_t *x_g, int16_t *y_g, uint16_t x, uint16_t y) {
	*x_g = *x_g + x;
	*y_g = *y_g + y;
}

void rotatePoint(int16_t *x, int16_t *y, float rotation) {
	rotation = setPrecision(rotation, 5);

	if(rotation != 0.0) {
		int16_t x1 = *x;
		int16_t y1 = *y;

		float a = (float)x1*cos(rotation);
		float b = (float)y1*sin(rotation);
		float c = (float)x1*sin(rotation);
		float d = (float)y1*cos(rotation);

		float e = a - b;
		float f = c + d;

		*x = (int16_t)round(e);
		*y = (int16_t)round(f);

		a = 0;

//		*x = (int16_t)((float)x1*cos(rotation) - (float)y1*sin(rotation));
//		*y = (int16_t)((float)x1*sin(rotation) + (float)y1*cos(rotation));
	}	//else do nothing
}

void movePoint(int16_t *x, int16_t *y, int16_t xMove, int16_t yMove) {
	*x = *x + xMove;
	*y = *y + yMove;
}

void rotateAndMovePoint(int16_t *x, int16_t *y, _graphicParams graphicParams) {
	rotatePoint(x, y, graphicParams.rotation);
	movePoint(x, y, graphicParams.xOffset, graphicParams.yOffset);
}

float setPrecision(float num, uint16_t precision) {
	return floor((float)(10*precision) * num) / (float)(10*precision);
}

float degToRad(float deg) {
	return deg * (M_PI / 180.0);
}

float radToDeg(float rad) {
	return rad * (180.0 / M_PI);
}

void resetGraphicParams(void) {
	graphicParams.thickness = 1;
	graphicParams.rotation = 0.0;
	graphicParams.alignH = alignHLeft;
	graphicParams.alignV = alignVTop;
	graphicParams.xOffset = 0;
	graphicParams.yOffset = 0;
}

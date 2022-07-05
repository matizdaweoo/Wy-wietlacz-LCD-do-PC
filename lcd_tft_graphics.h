#include <stdint.h>
#include "lcd_tft_fonts.h"

typedef enum {TRUE = 1, FALSE = 0} bool;

/* Wyrownanie w poziomie */
typedef enum {
	alignHLeft,  	//Wyrownaj do lewej
	alignHCenter,  	//Wyrownaj do srodka
	alignHRight  	//Wyrownaj do prawej
} _alignHorizontal;

/* Wyrownanie w pionie */
typedef enum {
	alignVTop,  		//Wyrownaj do gory
	alignVMiddle,  	//Wyrownaj do srodka
	alignVBottom  	//Wyrownaj do dolu
} _alignVertical;

//Parametry obiektu graficznego
typedef struct {
	uint16_t thickness;
	float rotation;
	_alignHorizontal alignH;
	_alignVertical alignV;
	uint16_t xOffset;
	uint16_t yOffset;
}_graphicParams;

//Punkt w ukladzie wsp.
typedef struct {
	int16_t x;
	int16_t y;
	bool isEmpty;
}_pointParams;

void TFT_ILI9481_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void TFT_ILI9481_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT_ILI9481_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void TFT_ILI9481_DrawCircle(uint16_t xc, uint16_t yc, uint16_t r, uint16_t color);


void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
//static void _swap(uint16_t *a, uint16_t *b);
void Fill_Triangel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color);
long map(long x, long in_min, long in_max, long out_min, long out_max);
int ringMeter(int value, int vmin, int vmax, int x, int y, int r, uint8_t scheme,int sp, int segm, int wid);
void ringMeterTemp(float value, float vmin, float vmax, char *name, uint16_t x, uint16_t y, _alignHorizontal alignH, _alignVertical alignV);
void ringMeterTempRPM(float value1, float vmin1, float vmax1, float value2, float vmin2, float vmax2, char *name, uint16_t x, uint16_t y, _alignHorizontal alignH, _alignVertical alignV);

void ringMeterInit1();
void ringMeterSetParams1(float rpm1, float temp1, float rpm2, float temp2, float temp3, float temp4, float temp5);

unsigned int rainbow(uint8_t value);

//void TFT_ILI9481_Bitmap(int x, int y, const uint8_t * glyph, int width, int height);


//void TFT_ILI9481_DrawLine2(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t thickness, uint16_t color);
//void TFT_ILI9481_DrawLine3(int x0, int y0, int x1, int y1, float thickness, uint16_t color);

//void TFT_ILI9481_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, _graphicParams graphicParams);

/**
 * @brief  Puts single character to LCD
 * @param  x: X position of top left corner
 * @param  y: Y position of top left corner
 * @param  c: Character to be displayed
 * @param  *font: Pointer to @ref TM_FontDef_t used font
 * @param  foreground: Color for char
 * @param  background: Color for char background
 * @retval None
 */
void TM_ILI9341_Putc(uint16_t x, uint16_t y, char c, TM_FontDef_t* font, uint32_t foreground, uint32_t background);

/**
 * @brief  Puts string to LCD
 * @param  x: X position of top left corner of first character in string
 * @param  y: Y position of top left corner of first character in string
 * @param  *str: Pointer to first character
 * @param  *font: Pointer to @ref TM_FontDef_t used font
 * @param  foreground: Color for string
 * @param  background: Color for string background
 * @retval None
 */
void TM_ILI9341_Puts(uint16_t x, uint16_t y, char* str, TM_FontDef_t *font, _alignHorizontal alignH, _alignVertical alignV, uint32_t foreground, uint32_t background);


void pointToLocal(int16_t *x_l, int16_t *y_l, uint16_t x, uint16_t y);
void pointToGlobal(int16_t *x_g, int16_t *y_g, uint16_t x, uint16_t y);
void rotatePoint(int16_t *x, int16_t *y, float rotation);
float setPrecision(float num, uint16_t precision);
float degToRad(float deg);
float radToDeg(float rad);

void resetGraphicParams(void);

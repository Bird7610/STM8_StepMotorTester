#ifndef LCD_HPP
#define LCD_HPP

#define LEFT 0
#define RIGHT 84
#define CENTER 42

#define LCD_COMMAND   0
#define LCD_DATA      1

// PCD8544 Commandset
// ------------------
// General commands
#define PCD8544_POWERDOWN		0x04
#define PCD8544_ENTRYMODE		0x02
#define PCD8544_EXTENDEDINSTRUCTION	0x01
#define PCD8544_DISPLAYBLANK		0x00
#define PCD8544_DISPLAYNORMAL		0x04
#define PCD8544_DISPLAYALLON		0x01
#define PCD8544_DISPLAYINVERTED		0x05
// Normal instruction set
#define PCD8544_FUNCTIONSET		0x20
#define PCD8544_DISPLAYCONTROL		0x08
#define PCD8544_SETYADDR		0x40
#define PCD8544_SETXADDR		0x80
// Extended instruction set
#define PCD8544_SETTEMP			0x04
#define PCD8544_SETBIAS			0x10
#define PCD8544_SETVOP			0x80
// Display presets
#define LCD_BIAS			0x03	// Range: 0-7 (0x00-0x07)
#define LCD_TEMP			0x02	// Range: 0-3 (0x00-0x03)
#define LCD_CONTRAST			0x46	// Range: 0-127 (0x00-0x7F)
#define byte 				unsigned char

//#define fontbyte(x) pgm_read_byte(&cfont.font[x])  
//#define bitmapbyte(x) pgm_read_byte(&bitmap[x])  
#define fontbyte(x) cfont.font[x]  
#define bitmapbyte(x) bitmap[x]  

#define regtype volatile unsigned char
#define regsize volatile unsigned char
#define bitmapdatatype unsigned char*
#define TEMP_BUFFER_SIZE 5


#define and &&


struct _current_font
{
	unsigned char* font;
	unsigned char x_size;
	unsigned char y_size;
	unsigned char offset;
	unsigned char numchars;
	unsigned char inverted;
};


//char* ToUTF8(char* NonUTF8String);
void InitLCD(unsigned char contrast);
void setContrast(unsigned char contrast);
//void enableSleep(void);
//void disableSleep(void);
void update(void);
void clrScr(void);
void fillScr(void);
void invert(bool mode);
void setPixel(unsigned char x, unsigned char y);
void clrPixel(unsigned char x, unsigned char y);
void invPixel(unsigned char x, unsigned char y);
void invertText(bool mode);
void print(char *st, unsigned char x, unsigned char y);
void setFont(unsigned char* font);
//void drawBitmap(int x, int y, unsigned char* bitmap, int sx, int sy);
//void _LCD_Write(unsigned char data, unsigned char mode);
//void _print_char(char c, char x, char row);
void printNumI(int num, unsigned char x, unsigned char y, unsigned char length, char filler);
void printNumF(float &num, unsigned char dec, unsigned char x, unsigned char y, char divider, unsigned char length, char filler);
void printNumF(float &num, unsigned char dec, unsigned char x, unsigned char y, unsigned char divider, unsigned char length, char filler, unsigned char selpos); // divider = '.' filler=' '
void drawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void clrLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void drawRect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void drawfillRect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void drawCircle(unsigned char x, unsigned char y, unsigned char radius);
void clrCircle(unsigned char x, unsigned char y, unsigned char radius);
void drawRoundRect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
//void _LCD_Reset(void);
//void clrRoundRect(int x1, int y1, int x2, int y2);
//void PrintCurrentScreen(bool state);

/*
class LCD5110
{
	public:
		LCD5110();
		void InitLCD(int contrast=LCD_CONTRAST);
		void setContrast(int contrast);
		void enableSleep();
		void disableSleep();
		void update();
		void clrScr();
		void fillScr();
		void invert(bool mode);
		void setPixel(unsigned short x, unsigned short y);
		void clrPixel(unsigned short x, unsigned short y);
		void invPixel(unsigned short x, unsigned short y);
		void invertText(bool mode);
		void print(char *st, int x, int y);
		//void print(String st, int x, int y);
		void printNumI(long num, int x, int y, int length=0, char filler=' ');
		//void printNumF(double num, byte dec, int x, int y, char divider='.', int length=0, char filler=' ');
		void setFont(unsigned char* font);
		void drawBitmap(int x, int y, unsigned char* bitmap, int sx, int sy);
		//void drawLine(int x1, int y1, int x2, int y2);
		//void clrLine(int x1, int y1, int x2, int y2);
		//void drawRect(int x1, int y1, int x2, int y2);
		//void clrRect(int x1, int y1, int x2, int y2);
		//void drawRoundRect(int x1, int y1, int x2, int y2);
		//void clrRoundRect(int x1, int y1, int x2, int y2);
		//void drawCircle(int x, int y, int radius);
		//void clrCircle(int x, int y, int radius);
		void _pulseClock();

	protected:
		regtype			*P_SCK, *P_MOSI, *P_DC, *P_RST, *P_CS;
		regsize			B_SCK, B_MOSI, B_DC, B_RST, B_CS;
		unsigned char			SCK_Pin, RST_Pin;			
		_current_font	cfont;
		unsigned char			scrbuf[504];
		bool			_sleep;
		int				_contrast;

		void _LCD_Write(unsigned char data, unsigned char mode);
		void _print_char(unsigned char c, int x, int row);
		void _convert_float(char *buf, double num, int width, byte prec);
		void drawHLine(int x, int y, int l);
		void clrHLine(int x, int y, int l);
		void drawVLine(int x, int y, int l);
		void clrVLine(int x, int y, int l);
};
*/

#endif

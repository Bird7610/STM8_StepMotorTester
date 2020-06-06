//
// FILE: OLED_F103F3.h
//

#ifndef OLED_F103F3_HPP
#define OLED_F103F3_HPP

#include <cstring>                        // для strlen
#include <string.h>

// compile options
#define USESBUF_          // enable this line if use graphic drowing
//#define ROT180           // enable this line if display rotate 180 deg
#define SSH1106_          // enable this line if 1.3 inch OLED
#define OLED_ad  0x3c   // OLED I2C bus address

// x postion offset
#ifdef SSH1106
 #define XOFSET   2      // for 1.3 inch OLED (SSH1106)
#else
 #define XOFSET   32      // for 0.66 inch OLED (SSD1306)
#endif

// command / data definition
#define CMD    true
#define DATA   false

#define TEMP_BUFFER_SIZE 5

// draw option
#define  NORMAL       false
#define  NEGATIVE     true
#define  TEXTAREA     0
#define  GRAPHICAREA  1
#define  LEFTHALF     0
#define  RIGHTHALF    1

// Scrolling option
#define ACTIVATE_SCROLL 0x2F
#define DEACTIVATE_SCROLL 0x2E
#define SET_VERTICAL_SCROLL_AREA 0xA3
#define RIGHT_HORIZONTAL_SCROLL 0x26
#define LEFT_HORIZONTAL_SCROLL 0x27
#define VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A


// extern reference from main routine
extern bool I2CdataTX(unsigned char address, unsigned char *txbuf_in, int txnum);
extern bool I2CdataRX(unsigned char address, unsigned char *rxbuf_in, int rxnum);
extern void delay(unsigned long msdly);

// OLED driving class
class OLED_F103F3
{
  public:
      char* ToUTF8(char* NonUTF8String);
      bool initOLED(void);
      void displayON(bool onflg);
      void invert(bool mode);
      void WriteCharacter(char character, bool neg);
      void SetYX(unsigned char row, unsigned char col);
      void SetPixelXY(unsigned char x, unsigned char y);
      void WriteString(char *characters, bool neg);
      void clearOLED(bool neg);
      void DrawFrame(bool neg);
      void WriteExFont(char *data, bool neg);
      void drawAllScreenPic(char *bitmap, bool neg);
      void startscrollright(unsigned char start, unsigned char stop);
      void startscrollleft(unsigned char start, unsigned char stop);
      void startscrolldiagright(unsigned char start, unsigned char stop);
      void startscrolldiagleft(unsigned char start, unsigned char stop);
      void stopscroll(void);
      void printNumF(float& num, unsigned char dec, unsigned char x, unsigned char y, char divider, unsigned char length, char filler,  bool neg);
      void setFont(unsigned char* font);
      
#ifdef USESBUF
      void clearArea(unsigned char areakind, bool neg);
      void selectGarea(unsigned char side){gxstpt = (side == LEFTHALF) ? 0 : 64;};
      void SetOvrWflg(bool ovwflg){this->ovwflg = ovwflg;}
      void SetGraphicXY(unsigned char x, unsigned char y);
      void setPixel(unsigned char gx, unsigned char y, bool neg);
      void invPixel(unsigned char gx, unsigned char y);
      void drawLine(unsigned char gx1, unsigned char y1, unsigned char gx2, unsigned char y2, bool neg);
      void drawRect(unsigned char gx1, unsigned char y1, unsigned char gx2, unsigned char y2, bool neg);
      void drawRoundRect(unsigned char gx1, unsigned char y1, unsigned char gx2, unsigned char y2, bool neg);
      void drawCircle(unsigned char gx, unsigned char y, unsigned char radius, bool neg);
      void drawBitmap(unsigned char gx, unsigned char y, unsigned char *bitmap, unsigned char sx, unsigned char sy, bool neg);
      void refreshGraph(void);
#endif

  private:
      void SendData(unsigned char idata, bool neg);
      bool SendCmd(unsigned char cmd);
      unsigned char snprintf_fp( char* destination, unsigned char available_chars, unsigned char decimal_digits, float source_number );
      bool ovwflg;
      unsigned char xpos;
      unsigned char ypos;
      unsigned char xofset;
#ifdef USESBUF
      void drawHLine(unsigned char x, unsigned char y, unsigned char l, bool neg);
      void drawVLine(unsigned char x, unsigned char y, unsigned char l, bool neg);
      unsigned char gxstpt;
#endif
};
static const char ASCII[][5] = {
  {0x00,0x00,0x00,0x00,0x00}, // 20 space           
  {0x00,0x00,0x5f,0x00,0x00}, // 21 ! 
  {0x00,0x07,0x00,0x07,0x00}, // 22 " 
  {0x14,0x7f,0x14,0x7f,0x14}, // 23 # 
  {0x24,0x2a,0x7f,0x2a,0x12}, // 24 $ 
  {0x23,0x13,0x08,0x64,0x62}, // 25 % 
  {0x36,0x49,0x55,0x22,0x50}, // 26 & 
  {0x00,0x05,0x03,0x00,0x00}, // 27 ' 
  {0x00,0x1c,0x22,0x41,0x00}, // 28 ( 
  {0x00,0x41,0x22,0x1c,0x00}, // 29 ) 
  {0x14,0x08,0x3e,0x08,0x14}, // 2a * 
  {0x08,0x08,0x3e,0x08,0x08}, // 2b + 
  {0x00,0x50,0x30,0x00,0x00}, // 2c , 
  {0x08,0x08,0x08,0x08,0x08}, // 2d - 
  {0x00,0x60,0x60,0x00,0x00}, // 2e . 
  {0x20,0x10,0x08,0x04,0x02}, // 2f / 
  {0x3e,0x51,0x49,0x45,0x3e}, // 30 0 
  {0x00,0x42,0x7f,0x40,0x00}, // 31 1 
  {0x42,0x61,0x51,0x49,0x46}, // 32 2 
  {0x21,0x41,0x45,0x4b,0x31}, // 33 3 
  {0x18,0x14,0x12,0x7f,0x10}, // 34 4 
  {0x27,0x45,0x45,0x45,0x39}, // 35 5 
  {0x3c,0x4a,0x49,0x49,0x30}, // 36 6 
  {0x01,0x71,0x09,0x05,0x03}, // 37 7 
  {0x36,0x49,0x49,0x49,0x36}, // 38 8 
  {0x06,0x49,0x49,0x29,0x1e}, // 39 9 
  {0x00,0x36,0x36,0x00,0x00}, // 3a : 
  {0x00,0x56,0x36,0x00,0x00}, // 3b ; 
  {0x08,0x14,0x22,0x41,0x00}, // 3c < 
  {0x14,0x14,0x14,0x14,0x14}, // 3d = 
  {0x00,0x41,0x22,0x14,0x08}, // 3e > 
  {0x02,0x01,0x51,0x09,0x06}, // 3f ? 
  {0x32,0x49,0x79,0x41,0x3e}, // 40 @ 
  {0x7e,0x11,0x11,0x11,0x7e}, // 41 A 
  {0x7f,0x49,0x49,0x49,0x36}, // 42 B 
  {0x3e,0x41,0x41,0x41,0x22}, // 43 C 
  {0x7f,0x41,0x41,0x22,0x1c}, // 44 D 
  {0x7f,0x49,0x49,0x49,0x41}, // 45 E 
  {0x7f,0x09,0x09,0x09,0x01}, // 46 F 
  {0x3e,0x41,0x49,0x49,0x7a}, // 47 G 
  {0x7f,0x08,0x08,0x08,0x7f}, // 48 H 
  {0x00,0x41,0x7f,0x41,0x00}, // 49 I 
  {0x20,0x40,0x41,0x3f,0x01}, // 4a J 
  {0x7f,0x08,0x14,0x22,0x41}, // 4b K 
  {0x7f,0x40,0x40,0x40,0x40}, // 4c L 
  {0x7f,0x02,0x0c,0x02,0x7f}, // 4d M 
  {0x7f,0x04,0x08,0x10,0x7f}, // 4e N 
  {0x3e,0x41,0x41,0x41,0x3e}, // 4f O 
  {0x7f,0x09,0x09,0x09,0x06}, // 50 P 
  {0x3e,0x41,0x51,0x21,0x5e}, // 51 Q 
  {0x7f,0x09,0x19,0x29,0x46}, // 52 R 
  {0x46,0x49,0x49,0x49,0x31}, // 53 S 
  {0x01,0x01,0x7f,0x01,0x01}, // 54 T 
  {0x3f,0x40,0x40,0x40,0x3f}, // 55 U 
  {0x1f,0x20,0x40,0x20,0x1f}, // 56 V 
  {0x3f,0x40,0x38,0x40,0x3f}, // 57 W 
  {0x63,0x14,0x08,0x14,0x63}, // 58 X 
  {0x07,0x08,0x70,0x08,0x07}, // 59 Y 
  {0x61,0x51,0x49,0x45,0x43}, // 5a Z 
  {0x00,0x7f,0x41,0x41,0x00}, // 5b [ 
  {0x02,0x04,0x08,0x10,0x20}, // 5c 
  {0x00,0x41,0x41,0x7f,0x00}, // 5d 
  {0x04,0x02,0x01,0x02,0x04}, // 5e 
  {0x40,0x40,0x40,0x40,0x40}, // 5f 
  {0x00,0x01,0x02,0x04,0x00}, // 60 
  {0x20,0x54,0x54,0x54,0x78}, // 61 a 
  {0x7f,0x48,0x44,0x44,0x38}, // 62 b 
  {0x38,0x44,0x44,0x44,0x20}, // 63 c 
  {0x38,0x44,0x44,0x48,0x7f}, // 64 d 
  {0x38,0x54,0x54,0x54,0x18}, // 65 e 
  {0x08,0x7e,0x09,0x01,0x02}, // 66 f 
  {0x0c,0x52,0x52,0x52,0x3e}, // 67 g 
  {0x7f,0x08,0x04,0x04,0x78}, // 68 h 
  {0x00,0x44,0x7d,0x40,0x00}, // 69 i 
  {0x20,0x40,0x44,0x3d,0x00}, // 6a j 
  {0x7f,0x10,0x28,0x44,0x00}, // 6b k 
  {0x00,0x41,0x7f,0x40,0x00}, // 6c l 
  {0x7c,0x04,0x18,0x04,0x78}, // 6d m 
  {0x7c,0x08,0x04,0x04,0x78}, // 6e n 
  {0x38,0x44,0x44,0x44,0x38}, // 6f o 
  {0x7c,0x14,0x14,0x14,0x08}, // 70 p 
  {0x08,0x14,0x14,0x18,0x7c}, // 71 q 
  {0x7c,0x08,0x04,0x04,0x08}, // 72 r 
  {0x48,0x54,0x54,0x54,0x20}, // 73 s 
  {0x04,0x3f,0x44,0x40,0x20}, // 74 t 
  {0x3c,0x40,0x40,0x20,0x7c}, // 75 u 
  {0x1c,0x20,0x40,0x20,0x1c}, // 76 v 
  {0x3c,0x40,0x30,0x40,0x3c}, // 77 w 
  {0x44,0x28,0x10,0x28,0x44}, // 78 x 
  {0x0c,0x50,0x50,0x50,0x3c}, // 79 y 
  {0x44,0x64,0x54,0x4c,0x44}, // 7a z 
  {0x00,0x08,0x36,0x41,0x00}, // 7b 
  {0x00,0x00,0x7f,0x00,0x00}, // 7c 
  {0x00,0x41,0x36,0x08,0x00}, // 7d 
  {0x10,0x08,0x08,0x10,0x08}, // 7e 
  {0x78,0x46,0x41,0x46,0x78},  // 7f 
  {0x7E,0x11,0x11,0x11,0x7E}, //c0 A  0x80
  {0x7F,0x49,0x49,0x49,0x30}, //c1 Б 
  {0x7F,0x49,0x49,0x49,0x36}, //c2 B 
  {0x7F,0x01,0x01,0x01,0x01}, //c3 Г
  {0x60,0x3E,0x21,0x3F,0x60}, //c4 Д
  {0x7F,0x49,0x49,0x49,0x41}, //c5 Е
  {0x67,0x18,0x7F,0x18,0x67}, //c6 Ж
  {0x22,0x41,0x49,0x49,0x36}, //c7 З
  {0x7F,0x10,0x08,0x04,0x7F}, //c8 И
  {0x7F,0x10,0x09,0x04,0x7F}, //c9 Й
  {0x7F,0x08,0x14,0x22,0x41}, //ca К
  {0x40,0x3E,0x01,0x01,0x7E}, //cb Л
  {0x7F,0x02,0x04,0x02,0x7F}, //cc М
  {0x7F,0x08,0x08,0x08,0x7F}, //cd Н
  {0x3E,0x41,0x41,0x41,0x3E}, //ce О
  {0x7F,0x01,0x01,0x01,0x7F}, //cf П
  {0x7F,0x09,0x09,0x09,0x06}, //d0 Р
  {0x3E,0x41,0x41,0x41,0x22}, //d1 С
  {0x01,0x01,0x7F,0x01,0x01}, //d2 Т
  {0x27,0x48,0x48,0x48,0x3F}, //d3 У
  {0x1E,0x21,0x7F,0x21,0x1E}, //d4 Ф
  {0x63,0x14,0x08,0x14,0x63}, //d5 Х
  {0x3F,0x20,0x20,0x3F,0x40}, //d6 Ц
  {0x1F,0x10,0x10,0x10,0x7F}, //d7 Ч
  {0x7F,0x40,0x7F,0x40,0x7F}, //d8 Ш
  {0x3F,0x20,0x3F,0x20,0x7F}, //d9 Щ
  {0x01,0x7F,0x44,0x44,0x38}, //da Ъ
  {0x7F,0x44,0x7C,0x00,0x7F}, //db Ы
  {0x7F,0x44,0x44,0x44,0x38}, //dc Ь
  {0x22,0x41,0x49,0x49,0x3E}, //dd Э
  {0x7F,0x08,0x7F,0x41,0x7F}, //de Ю
  {0x46,0x29,0x19,0x09,0x7F}, //df Я
  {0x20,0x54,0x54,0x54,0x78}, //e0 а
  {0x3C,0x4A,0x4A,0x4B,0x30}, //e1 б
  {0x7C,0x54,0x54,0x58,0x20}, //e2 в
  {0x7C,0x04,0x04,0x04,0x04}, //e3 г
  {0x60,0x38,0x24,0x3C,0x60}, //e4 д
  {0x38,0x54,0x54,0x54,0x18}, //e5 е
  {0x74,0x08,0x7C,0x08,0x74}, //e6 ж
  {0x28,0x44,0x54,0x54,0x28}, //e7 з
  {0x7C,0x20,0x10,0x08,0x7C}, //e8 и
  {0x7C,0x21,0x12,0x09,0x7C}, //e9 й
  {0x7C,0x10,0x10,0x28,0x44}, //ea к
  {0x40,0x78,0x04,0x04,0x7C}, //eb л
  {0x7C,0x08,0x10,0x08,0x7C}, //ec м
  {0x7C,0x10,0x10,0x10,0x7C}, //ed н
  {0x38,0x44,0x44,0x44,0x38}, //ee о
  {0x7C,0x04,0x04,0x04,0x7C}, //ef п
  {0x7C,0x14,0x14,0x14,0x08}, //f0 р
  {0x38,0x44,0x44,0x44,0x28}, //f1 с
  {0x04,0x04,0x7C,0x04,0x04}, //f2 т
  {0x0C,0x50,0x50,0x50,0x3C}, //f3 у
  {0x18,0x24,0x7C,0x24,0x18}, //f4 ф
  {0x44,0x28,0x10,0x28,0x44}, //f5 х
  {0x3C,0x20,0x20,0x3C,0x40}, //f6 ц
  {0x1C,0x20,0x20,0x20,0x7C}, //f7 ч
  {0x7C,0x40,0x7C,0x40,0x7C}, //f8 ш
  {0x3C,0x20,0x3C,0x20,0x7C}, //f9 щ
  {0x04,0x7C,0x48,0x48,0x30}, //fa ъ
  {0x7C,0x48,0x48,0x30,0x7C}, //fb ы
  {0x7C,0x48,0x48,0x48,0x30}, //fc ь
  {0x28,0x44,0x54,0x54,0x38}, //fd э
  {0x7C,0x38,0x44,0x44,0x38}, //fe ю
  {0x48,0x34,0x14,0x14,0x7C}  //ff я
};  

//
//fontdatatype MediumNumbers[] =
//{
//0x0c, 0x10, 0x2d, 0x0d,
//0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00,   // -
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,   // .
//0x00, 0x00, 0x02, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x02, 0x00, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x81, 0x00, 0x00,   // /
//0x00, 0xfc, 0x7a, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7a, 0xfc, 0x00, 0x00, 0x7e, 0xbc, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xbc, 0x7e, 0x00,   // 0
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0x00,   // 1
//0x00, 0x00, 0x02, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x7a, 0xfc, 0x00, 0x00, 0x7e, 0xbd, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x81, 0x00, 0x00,   // 2
//0x00, 0x00, 0x02, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x7a, 0xfc, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00,   // 3
//0x00, 0xfc, 0x78, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x78, 0xfc, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3d, 0x7e, 0x00,   // 4
//0x00, 0xfc, 0x7a, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x02, 0x00, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00,   // 5
//0x00, 0xfc, 0x7a, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x02, 0x00, 0x00, 0x00, 0x7e, 0xbd, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00,   // 6
//0x00, 0x00, 0x02, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7a, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0x00,   // 7
//0x00, 0xfc, 0x7a, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x7a, 0xfc, 0x00, 0x00, 0x7e, 0xbd, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00,   // 8
//0x00, 0xfc, 0x7a, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x7a, 0xfc, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00,   // 9
//};
//*/
//
//fontdatatype BigNumbers[]  =
//{
//0x0e, 0x18, 0x2d, 0x0d,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // -
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xe0, 0xe0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   // .
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // /
//0x00, 0xfc, 0xfa, 0xf6, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0xf6, 0xfa, 0xfc, 0x00, 0x00, 0xef, 0xc7, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xc7, 0xef, 0x00, 0x00, 0x7f, 0xbf, 0xdf, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdf, 0xbf, 0x7f, 0x00,   // 0
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xc7, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x3f, 0x7f, 0x00,   // 1
//0x00, 0x00, 0x02, 0x06, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0xf6, 0xfa, 0xfc, 0x00, 0x00, 0xe0, 0xd0, 0xb8, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x3b, 0x17, 0x0f, 0x00, 0x00, 0x7f, 0xbf, 0xdf, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0x80, 0x00, 0x00,   // 2
//0x00, 0x00, 0x02, 0x06, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0xf6, 0xfa, 0xfc, 0x00, 0x00, 0x00, 0x10, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xbb, 0xd7, 0xef, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdf, 0xbf, 0x7f, 0x00,   // 3
//0x00, 0xfc, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0xfc, 0x00, 0x00, 0x0f, 0x17, 0x3b, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xbb, 0xd7, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x3f, 0x7f, 0x00,   // 4
//0x00, 0xfc, 0xfa, 0xf6, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x06, 0x02, 0x00, 0x00, 0x00, 0x0f, 0x17, 0x3b, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xb8, 0xd0, 0xe0, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdf, 0xbf, 0x7f, 0x00,   // 5
//0x00, 0xfc, 0xfa, 0xf6, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x06, 0x02, 0x00, 0x00, 0x00, 0xef, 0xd7, 0xbb, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xb8, 0xd0, 0xe0, 0x00, 0x00, 0x7f, 0xbf, 0xdf, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdf, 0xbf, 0x7f, 0x00,   // 6
//0x00, 0x00, 0x02, 0x06, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0xf6, 0xfa, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xc7, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x3f, 0x7f, 0x00,   // 7
//0x00, 0xfc, 0xfa, 0xf6, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0xf6, 0xfa, 0xfc, 0x00, 0x00, 0xef, 0xd7, 0xbb, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xbb, 0xd7, 0xef, 0x00, 0x00, 0x7f, 0xbf, 0xdf, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdf, 0xbf, 0x7f, 0x00,   // 8
//0x00, 0xfc, 0xfa, 0xf6, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0xf6, 0xfa, 0xfc, 0x00, 0x00, 0x0f, 0x17, 0x3b, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xbb, 0xd7, 0xef, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdf, 0xbf, 0x7f, 0x00,   // 9
//};

//
//fontdatatype TABLE5[]  =  //    
// {0x06,0x08,0x20,0xA0,
//  0x00,0x00,0x00,0x00,0x00,0x00, // 20 space           
//  0x00,0x00,0x00,0x5f,0x00,0x00, // 21 ! 
//  0x00,0x00,0x07,0x00,0x07,0x00, // 22 " 
//  0x00,0x14,0x7f,0x14,0x7f,0x14, // 23 # 
//  0x00,0x24,0x2a,0x7f,0x2a,0x12, // 24 $ 
//  0x00,0x23,0x13,0x08,0x64,0x62, // 25 % 
//  0x00,0x36,0x49,0x55,0x22,0x50, // 26 & 
//  0x00,0x00,0x05,0x03,0x00,0x00, // 27 ' 
//  0x00,0x00,0x1c,0x22,0x41,0x00, // 28 ( 
//  0x00,0x00,0x41,0x22,0x1c,0x00, // 29 ) 
//  0x00,0x14,0x08,0x3e,0x08,0x14, // 2a * 
//  0x00,0x08,0x08,0x3e,0x08,0x08, // 2b + 
//  0x00,0x00,0x50,0x30,0x00,0x00, // 2c , 
//  0x00,0x08,0x08,0x08,0x08,0x08, // 2d - 
//  0x00,0x00,0x60,0x60,0x00,0x00, // 2e . 
//  0x00,0x20,0x10,0x08,0x04,0x02, // 2f / 
//  0x00,0x3e,0x51,0x49,0x45,0x3e, // 30 0 
//  0x00,0x00,0x42,0x7f,0x40,0x00, // 31 1 
//  0x00,0x42,0x61,0x51,0x49,0x46, // 32 2 
//  0x00,0x21,0x41,0x45,0x4b,0x31, // 33 3 
//  0x00,0x18,0x14,0x12,0x7f,0x10, // 34 4 
//  0x00,0x27,0x45,0x45,0x45,0x39, // 35 5 
//  0x00,0x3c,0x4a,0x49,0x49,0x30, // 36 6 
//  0x00,0x01,0x71,0x09,0x05,0x03, // 37 7 
//  0x00,0x36,0x49,0x49,0x49,0x36, // 38 8 
//  0x00,0x06,0x49,0x49,0x29,0x1e, // 39 9 
//  0x00,0x00,0x36,0x36,0x00,0x00, // 3a : 
//  0x00,0x00,0x56,0x36,0x00,0x00, // 3b ; 
//  0x00,0x08,0x14,0x22,0x41,0x00, // 3c < 
//  0x00,0x14,0x14,0x14,0x14,0x14, // 3d = 
//  0x00,0x00,0x41,0x22,0x14,0x08, // 3e > 
//  0x00,0x02,0x01,0x51,0x09,0x06, // 3f ? 
//  0x00,0x32,0x49,0x79,0x41,0x3e, // 40 @ 
//  0x00,0x7e,0x11,0x11,0x11,0x7e, // 41 A 
//  0x00,0x7f,0x49,0x49,0x49,0x36, // 42 B 
//  0x00,0x3e,0x41,0x41,0x41,0x22, // 43 C 
//  0x00,0x7f,0x41,0x41,0x22,0x1c, // 44 D 
//  0x00,0x7f,0x49,0x49,0x49,0x41, // 45 E 
//  0x00,0x7f,0x09,0x09,0x09,0x01, // 46 F 
//  0x00,0x3e,0x41,0x49,0x49,0x7a, // 47 G 
//  0x00,0x7f,0x08,0x08,0x08,0x7f, // 48 H 
//  0x00,0x00,0x41,0x7f,0x41,0x00, // 49 I 
//  0x00,0x20,0x40,0x41,0x3f,0x01, // 4a J 
//  0x00,0x7f,0x08,0x14,0x22,0x41, // 4b K 
//  0x00,0x7f,0x40,0x40,0x40,0x40, // 4c L 
//  0x00,0x7f,0x02,0x0c,0x02,0x7f, // 4d M 
//  0x00,0x7f,0x04,0x08,0x10,0x7f, // 4e N 
//  0x00,0x3e,0x41,0x41,0x41,0x3e, // 4f O 
//  0x00,0x7f,0x09,0x09,0x09,0x06, // 50 P 
//  0x00,0x3e,0x41,0x51,0x21,0x5e, // 51 Q 
//  0x00,0x7f,0x09,0x19,0x29,0x46, // 52 R 
//  0x00,0x46,0x49,0x49,0x49,0x31, // 53 S 
//  0x00,0x01,0x01,0x7f,0x01,0x01, // 54 T 
//  0x00,0x3f,0x40,0x40,0x40,0x3f, // 55 U 
//  0x00,0x1f,0x20,0x40,0x20,0x1f, // 56 V 
//  0x00,0x3f,0x40,0x38,0x40,0x3f, // 57 W 
//  0x00,0x63,0x14,0x08,0x14,0x63, // 58 X 
//  0x00,0x07,0x08,0x70,0x08,0x07, // 59 Y 
//  0x00,0x61,0x51,0x49,0x45,0x43, // 5a Z 
//  0x00,0x00,0x7f,0x41,0x41,0x00, // 5b [ 
//  0x00,0x02,0x04,0x08,0x10,0x20, // 5c 
//  0x00,0x00,0x41,0x41,0x7f,0x00, // 5d 
//  0x00,0x04,0x02,0x01,0x02,0x04, // 5e 
//  0x00,0x40,0x40,0x40,0x40,0x40, // 5f 
//  0x00,0x00,0x01,0x02,0x04,0x00, // 60 
//  0x00,0x20,0x54,0x54,0x54,0x78, // 61 a 
//  0x00,0x7f,0x48,0x44,0x44,0x38, // 62 b 
//  0x00,0x38,0x44,0x44,0x44,0x20, // 63 c 
//  0x00,0x38,0x44,0x44,0x48,0x7f, // 64 d 
//  0x00,0x38,0x54,0x54,0x54,0x18, // 65 e 
//  0x00,0x08,0x7e,0x09,0x01,0x02, // 66 f 
//  0x00,0x0c,0x52,0x52,0x52,0x3e, // 67 g 
//  0x00,0x7f,0x08,0x04,0x04,0x78, // 68 h 
//  0x00,0x00,0x44,0x7d,0x40,0x00, // 69 i 
//  0x00,0x20,0x40,0x44,0x3d,0x00, // 6a j 
//  0x00,0x7f,0x10,0x28,0x44,0x00, // 6b k 
//  0x00,0x00,0x41,0x7f,0x40,0x00, // 6c l 
//  0x00,0x7c,0x04,0x18,0x04,0x78, // 6d m 
//  0x00,0x7c,0x08,0x04,0x04,0x78, // 6e n 
//  0x00,0x38,0x44,0x44,0x44,0x38, // 6f o 
//  0x00,0x7c,0x14,0x14,0x14,0x08, // 70 p 
//  0x00,0x08,0x14,0x14,0x18,0x7c, // 71 q 
//  0x00,0x7c,0x08,0x04,0x04,0x08, // 72 r 
//  0x00,0x48,0x54,0x54,0x54,0x20, // 73 s 
//  0x00,0x04,0x3f,0x44,0x40,0x20, // 74 t 
//  0x00,0x3c,0x40,0x40,0x20,0x7c, // 75 u 
//  0x00,0x1c,0x20,0x40,0x20,0x1c, // 76 v 
//  0x00,0x3c,0x40,0x30,0x40,0x3c, // 77 w 
//  0x00,0x44,0x28,0x10,0x28,0x44, // 78 x 
//  0x00,0x0c,0x50,0x50,0x50,0x3c, // 79 y 
//  0x00,0x44,0x64,0x54,0x4c,0x44, // 7a z 
//  0x00,0x00,0x08,0x36,0x41,0x00, // 7b 
//  0x00,0x00,0x00,0x7f,0x00,0x00, // 7c 
//  0x00,0x00,0x41,0x36,0x08,0x00, // 7d 
//  0x00,0x10,0x08,0x08,0x10,0x08, // 7e 
//  0x00,0x78,0x46,0x41,0x46,0x78,  // 7f 
//  0x00,0x7E,0x11,0x11,0x11,0x7E, //c0 A  0x80
//  0x00,0x7F,0x49,0x49,0x49,0x30, //c1 Б 
//  0x00,0x7F,0x49,0x49,0x49,0x36, //c2 B 
//  0x00,0x7F,0x01,0x01,0x01,0x01, //c3 Г
//  0x00,0x60,0x3E,0x21,0x3F,0x60, //c4 Д
//  0x00,0x7F,0x49,0x49,0x49,0x41, //c5 Е
//  0x00,0x67,0x18,0x7F,0x18,0x67, //c6 Ж
//  0x00,0x22,0x41,0x49,0x49,0x36, //c7 З
//  0x00,0x7F,0x10,0x08,0x04,0x7F, //c8 И
//  0x00,0x7F,0x10,0x09,0x04,0x7F, //c9 Й
//  0x00,0x7F,0x08,0x14,0x22,0x41, //ca К
//  0x00,0x40,0x3E,0x01,0x01,0x7E, //cb Л
//  0x00,0x7F,0x02,0x04,0x02,0x7F, //cc М
//  0x00,0x7F,0x08,0x08,0x08,0x7F, //cd Н
//  0x00,0x3E,0x41,0x41,0x41,0x3E, //ce О
//  0x00,0x7F,0x01,0x01,0x01,0x7F, //cf П
//  0x00,0x7F,0x09,0x09,0x09,0x06, //d0 Р
//  0x00,0x3E,0x41,0x41,0x41,0x22, //d1 С
//  0x00,0x01,0x01,0x7F,0x01,0x01, //d2 Т
//  0x00,0x27,0x48,0x48,0x48,0x3F, //d3 У
//  0x00,0x1E,0x21,0x7F,0x21,0x1E, //d4 Ф
//  0x00,0x63,0x14,0x08,0x14,0x63, //d5 Х
//  0x00,0x3F,0x20,0x20,0x3F,0x40, //d6 Ц
//  0x00,0x1F,0x10,0x10,0x10,0x7F, //d7 Ч
//  0x00,0x7F,0x40,0x7F,0x40,0x7F, //d8 Ш
//  0x00,0x3F,0x20,0x3F,0x20,0x7F, //d9 Щ
//  0x00,0x01,0x7F,0x44,0x44,0x38, //da Ъ
//  0x00,0x7F,0x44,0x7C,0x00,0x7F, //db Ы
//  0x00,0x7F,0x44,0x44,0x44,0x38, //dc Ь
//  0x00,0x22,0x41,0x49,0x49,0x3E, //dd Э
//  0x00,0x7F,0x08,0x7F,0x41,0x7F, //de Ю
//  0x00,0x46,0x29,0x19,0x09,0x7F, //df Я
//  0x00,0x20,0x54,0x54,0x54,0x78, //e0 а
//  0x00,0x3C,0x4A,0x4A,0x4B,0x30, //e1 б
//  0x00,0x7C,0x54,0x54,0x58,0x20, //e2 в
//  0x00,0x7C,0x04,0x04,0x04,0x04, //e3 г
//  0x00,0x60,0x38,0x24,0x3C,0x60, //e4 д
//  0x00,0x38,0x54,0x54,0x54,0x18, //e5 е
//  0x00,0x74,0x08,0x7C,0x08,0x74, //e6 ж
//  0x00,0x28,0x44,0x54,0x54,0x28, //e7 з
//  0x00,0x7C,0x20,0x10,0x08,0x7C, //e8 и
//  0x00,0x7C,0x21,0x12,0x09,0x7C, //e9 й
//  0x00,0x7C,0x10,0x10,0x28,0x44, //ea к
//  0x00,0x40,0x78,0x04,0x04,0x7C, //eb л
//  0x00,0x7C,0x08,0x10,0x08,0x7C, //ec м
//  0x00,0x7C,0x10,0x10,0x10,0x7C, //ed н
//  0x00,0x38,0x44,0x44,0x44,0x38, //ee о
//  0x00,0x7C,0x04,0x04,0x04,0x7C, //ef п
//  0x00,0x7C,0x14,0x14,0x14,0x08, //f0 р
//  0x00,0x38,0x44,0x44,0x44,0x28, //f1 с
//  0x00,0x04,0x04,0x7C,0x04,0x04, //f2 т
//  0x00,0x0C,0x50,0x50,0x50,0x3C, //f3 у
//  0x00,0x18,0x24,0x7C,0x24,0x18, //f4 ф
//  0x00,0x44,0x28,0x10,0x28,0x44, //f5 х
//  0x00,0x3C,0x20,0x20,0x3C,0x40, //f6 ц
//  0x00,0x1C,0x20,0x20,0x20,0x7C, //f7 ч
//  0x00,0x7C,0x40,0x7C,0x40,0x7C, //f8 ш
//  0x00,0x3C,0x20,0x3C,0x20,0x7C, //f9 щ
//  0x00,0x04,0x7C,0x48,0x48,0x30, //fa ъ
//  0x00,0x7C,0x48,0x48,0x30,0x7C, //fb ы
//  0x00,0x7C,0x48,0x48,0x48,0x30, //fc ь
//  0x00,0x28,0x44,0x54,0x54,0x38, //fd э
//  0x00,0x7C,0x38,0x44,0x44,0x38, //fe ю
//  0x00,0x48,0x34,0x14,0x14,0x7C  //ff я
//  };  



static const char motor_4x_Image[384] = {
0x00, 0x24, 0x3e, 0x20, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x3a, 0x2a, 0x2e, 
0x00, 0x06, 0xf9, 0xf9, 0x06, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xf9, 
0xf9, 0x06, 0x00, 0x07, 0x0f, 0x18, 0x30, 
0x60, 0x60, 0x30, 0x18, 0x0c, 0x07, 0x07, 
0x0c, 0x18, 0x30, 0x60, 0x60, 0x30, 0x18, 
0x0c, 0x07, 0x07, 0x0c, 0x18, 0x30, 0x60, 
0x60, 0x30, 0x18, 0x0f, 0x07, 0x01, 0x01, 
0x07, 0x0f, 0x18, 0x30, 0x60, 0x60, 0x30, 
0x18, 0x0c, 0x07, 0x07, 0x0c, 0x18, 0x30, 
0x60, 0x60, 0x30, 0x18, 0x0c, 0x07, 0x07, 
0x0c, 0x18, 0x30, 0x60, 0x60, 0x30, 0x18, 
0x0f, 0x07, 0x00, 0x00, 0xe0, 0xf0, 0x18, 
0x0c, 0x06, 0x06, 0x0c, 0x18, 0x30, 0xe0, 
0xe0, 0x30, 0x18, 0x0c, 0x06, 0x06, 0x0c, 
0x18, 0x30, 0xe0, 0xe0, 0x30, 0x18, 0x0c, 
0x06, 0x06, 0x0c, 0x18, 0xf0, 0xe0, 0x80, 
0x80, 0xe0, 0xf0, 0x18, 0x0c, 0x06, 0x06, 
0x0c, 0x18, 0x30, 0xe0, 0xe0, 0x30, 0x18, 
0x0c, 0x06, 0x06, 0x0c, 0x18, 0x30, 0xe0, 
0xe0, 0x30, 0x18, 0x0c, 0x06, 0x06, 0x0c, 
0x18, 0xf0, 0xe0, 0x00, 0x60, 0x9f, 0x9f, 
0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x60, 0x9f, 0x9f, 0x60, 0x00, 0x54, 
0x54, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x1c, 0x10, 0x7c, 0x00
};

static const char motor_6x_Image[384] = {
     0x00,    0x24,    0x3e,    0x20,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x3a,    0x2a,    
    0x2e,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x2a,    0x2a,    0x3e,    0x00,    
    0x06,    0xf9,    0xf9,    0x06,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x06,    0xf9,    
    0xf9,    0x06,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x06,    0xf9,    0xf9,    0x06,    
    0x00,    0x07,    0x0f,    0x18,    0x30,    0x60,    0x60,    0x30,    
    0x18,    0x0c,    0x07,    0x07,    0x0c,    0x18,    0x30,    0x60,    
    0x60,    0x30,    0x18,    0x0c,    0x07,    0x07,    0x0c,    0x18,    
    0x30,    0x60,    0x60,    0x30,    0x18,    0x0f,    0x07,    0x00,    
    0x00,    0x07,    0x0f,    0x18,    0x30,    0x60,    0x60,    0x30,    
    0x18,    0x0c,    0x07,    0x07,    0x0c,    0x18,    0x30,    0x60,    
    0x60,    0x30,    0x18,    0x0c,    0x07,    0x07,    0x0c,    0x18,    
    0x30,    0x60,    0x60,    0x30,    0x18,    0x0f,    0x07,    0x00,    
    0x00,    0xe0,    0xf0,    0x18,    0x0c,    0x06,    0x06,    0x0c,    
    0x18,    0x30,    0xe0,    0xe0,    0x30,    0x18,    0x0c,    0x06,    
    0x06,    0x0c,    0x18,    0x30,    0xe0,    0xe0,    0x30,    0x18,    
    0x0c,    0x06,    0x06,    0x0c,    0x18,    0xf0,    0xe0,    0x00,    
    0x00,    0xe0,    0xf0,    0x18,    0x0c,    0x06,    0x06,    0x0c,    
    0x18,    0x30,    0xe0,    0xe0,    0x30,    0x18,    0x0c,    0x06,    
    0x06,    0x0c,    0x18,    0x30,    0xe0,    0xe0,    0x30,    0x18,    
    0x0c,    0x06,    0x06,    0x0c,    0x18,    0xf0,    0xe0,    0x00,    
    0x60,    0x9f,    0x9f,    0x60,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x60,    0x9f,    
    0x9f,    0x60,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x60,    0x9f,    0x9f,    0x60,    
    0x00,    0x1c,    0x10,    0x7c,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x5c,    0x54,    
    0x74,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    
    0x00,    0x00,    0x00,    0x00,    0x7c,    0x54,    0x74,    0x00
};

extern OLED_F103F3 OLED;
#endif  // end of #define OLED_F103F3_H
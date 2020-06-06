#include "main.hpp"
#include "spi.hpp"
#include "lcd.hpp"

//#include <avr/pgmspace.h>


struct _current_font	cfont;
static unsigned char		scrbuf[504];
//bool		_sleep;
unsigned char	_contrast;

char* ToUTF8(char* NonUTF8String)
{
  
  if (NonUTF8String == NULL){return(NonUTF8String);}
  
  for (unsigned char  i = 0; i < strlen(NonUTF8String); ++i)
    {
        if (NonUTF8String[i] != 0xD0 && NonUTF8String[i] != 0xD1 )
         {
          if (NonUTF8String[i] >= 0x80 && NonUTF8String[i] < 0x90)
	   {
	   NonUTF8String[i]= (NonUTF8String[i] + 0x40);
	   } else if (NonUTF8String[i] >= 0xC0)// && (unsigned char)NonUTF8String[i] <= (unsigned char)0xFF) 
            {
              NonUTF8String[i] = (NonUTF8String[i] - 0x40);
            }
          else
          {
            NonUTF8String[i]= NonUTF8String[i];
          }
         }
    }
    return(NonUTF8String);
} 

unsigned char  snprintf_fp( char* destination, unsigned char available_chars, unsigned char decimal_digits,
                   float source_number )
{
    unsigned char   chars_used  = 0;    // This will be returned.
    unsigned char   lenght  = available_chars;    
    unsigned char   i = 0;
      
    if ( available_chars > 0 )
    {
        // Handle a negative sign.
        if ( source_number < 0 )
        {
            // Make it positive
            source_number = 0 - source_number;
            //source_number *= -1;
            destination[ chars_used ] = '-';
            ++chars_used;
        }

        // Handle rounding
        unsigned long int zeros = 1;
        for ( i = decimal_digits; i > 0; --i )
        {
            zeros *=10;
        }

        unsigned long int source_num = (unsigned long int)(source_number * zeros + 0.5f );

        // Determine sliding divider max position.
        unsigned long int  div_amount = zeros;       // Give it a head start
        while ( (div_amount*10 ) <= source_num )
        {
            div_amount *= 10;
        }
        // Process the digits
        while ( div_amount > 0 )
        {
            unsigned long int whole_number = source_num / div_amount;
            if ( chars_used < available_chars )
            {
                destination[ chars_used ] = '0' + (char)whole_number;
                ++chars_used;

                if ( ( div_amount == zeros ) && ( zeros > 10 ) )
                {
                    destination[ chars_used ] = '.';
                    ++chars_used;
                    available_chars = chars_used + 1;
                }
            }
            source_num -= ( whole_number * div_amount );
            div_amount /= 10;
        }

        if (chars_used < lenght)
        {
          for (i = 0; i < chars_used; ++i)
          {
            destination[lenght-1 - i] = destination[chars_used - 1 - i];
          }
          for (i = 0; i < (lenght - chars_used); ++i)
          {
            destination[i] = '/'; // store the space
          }
            
        }
        // Store the zero.
        destination[ lenght ] = 0;

    }

    return lenght;
}


void _LCD_Write(unsigned char data, unsigned char mode)
{   
  SS_ACTIVE;

  if (mode==LCD_COMMAND)
  {
    DC_COMMAND;
  }
  else
  {
    DC_DATA;
  }

  SPISendByte(data);
  
  SS_INACTIVE;
}

void _LCD_Reset(void)
{ 
    
  SPI_DC = 1;
  SPI_MOSI = 1;
  SPI_SCK = 1;
  SPI_CE = 0;
  SPI_RST = 0;
  _delay_ms(10);
  SPI_RST = 1;
}

void InitLCD(unsigned char contrast)
{
   SPIInit();

//	if (contrast>0x7F)
//		contrast=0x7F;
//	if (contrast<0)
//		contrast=0;

        _LCD_Reset();
        
	_LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
	_LCD_Write(PCD8544_SETVOP | contrast, LCD_COMMAND);
	_LCD_Write(PCD8544_SETTEMP | LCD_TEMP, LCD_COMMAND);
	_LCD_Write(PCD8544_SETBIAS | LCD_BIAS, LCD_COMMAND);
	_LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
	_LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
	_LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
	_LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);
        setContrast(contrast);
        //
	//clrScr();
	//update();

	cfont.font=0;
	//_sleep=false;
	_contrast=contrast;

}

void setContrast(unsigned char contrast)
{
	if (contrast>0x7F)
        {
            contrast=0x7F;
        }
//	if (contrast<0)
//		contrast=0;
	_LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
	_LCD_Write(PCD8544_SETVOP | contrast, LCD_COMMAND);
	_LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
	_contrast=contrast;
}
/*
void enableSleep(void)
{
	_sleep = true;
	 unsigned short b;
	_LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
	_LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
	for (b=0; b<504; b++)
		_LCD_Write(0, LCD_DATA);
	_LCD_Write(PCD8544_FUNCTIONSET | PCD8544_POWERDOWN, LCD_COMMAND);
}

void disableSleep(void)
{
	_sleep = false;
	_LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
	_LCD_Write(PCD8544_SETVOP | _contrast, LCD_COMMAND);
	_LCD_Write(PCD8544_SETTEMP | LCD_TEMP, LCD_COMMAND);
	_LCD_Write(PCD8544_SETBIAS | LCD_BIAS, LCD_COMMAND);
	_LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
	_LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);
	update();
}
*/
void update(void)
{
   unsigned short b;
//	if (_sleep==false)
//	{
		_LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
		_LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
		for (b=0; b<504; ++b)
                {
                    _LCD_Write(scrbuf[b], LCD_DATA);
                }
//	}
}

void clrScr(void)
{
   unsigned short c=0;
	for (c=0; c<504; ++c)
        {
            scrbuf[c]=0;
        }
}

void fillScr(void)
{
   unsigned short c=0;
	for (c=0; c<504; ++c)
        {
            scrbuf[c]=0xFF;
        }
	//update();
}

void invert(bool mode)
{
	if (mode==true)
        {_LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYINVERTED, LCD_COMMAND);}
	else
        {_LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);}
}

void setPixel(unsigned char x, unsigned char y)
{
	unsigned short by, bi;

	if ((x<84) && (y<48))
	{
		by=((y/8)*84)+x;
		bi=y % 8;

		scrbuf[by]=scrbuf[by] | (1<<bi);
	}
}

void clrPixel(unsigned char x, unsigned char y)
{
	unsigned short by, bi;

	if ((x<84) && (y<48))
	{
		by=((y/8)*84)+x;
		bi=y % 8;

		scrbuf[by]=scrbuf[by] & ~(1<<bi);
	}
}

void invPixel(unsigned char x, unsigned char y)
{
	unsigned short by, bi;

	if ((x<84) && (y<48))
	{
		by=((y/8)*84)+x;
		bi=y % 8;

		if ((scrbuf[by] & (1<<bi))==0)
                {
                  scrbuf[by]=scrbuf[by] | (1<<bi);
                }
		else
                {
                  scrbuf[by]=scrbuf[by] & ~(1<<bi);
                }
	}
}

void invertText(bool mode)
{
	if (mode==true)
        {
          cfont.inverted=1;
        }
	else
	{
          cfont.inverted=0;
        }
}

void _print_char(unsigned char c, unsigned char x, unsigned char y)
{
   unsigned char rowcnt, cnt, b, cx, cy;
   
//        if ((unsigned char) c != 0xD0 && (unsigned char) c != 0xD1 )
//         {
          if ((unsigned char)c >= 0x80 && (unsigned char)c < 0x90)
	   {
	   c= ((unsigned char)c + 0x40);
	   } else if ( c >= 0xC0 ) 
                    {
                      c = ((unsigned char)c - 0x40);             
                    }
          else
          {
            c= c;
          }
//         }
	if ((cfont.y_size % 8) == 0)
	{
		int font_idx = ((c - cfont.offset)*(cfont.x_size*(cfont.y_size/8)))+4;
		for ( rowcnt=0; rowcnt<(cfont.y_size/8); ++rowcnt)
		{
			for( cnt=0; cnt<cfont.x_size; ++cnt)
			{
				for ( b=0; b<8; ++b)
					if ((fontbyte(font_idx+cnt+(rowcnt*cfont.x_size)) & (1<<b))!=0)
						if (cfont.inverted==0)
                                                  {setPixel(x+cnt, y+(rowcnt*8)+b);}
						else
                                                  {clrPixel(x+cnt, y+(rowcnt*8)+b);}
					else
						if (cfont.inverted==0)
                                                {clrPixel(x+cnt, y+(rowcnt*8)+b);}
						else
                                                {setPixel(x+cnt, y+(rowcnt*8)+b);}
			}
		}
	}
	else
	{
		unsigned char font_idx = ((c - cfont.offset)*((cfont.x_size*cfont.y_size/8)))+4;
		unsigned char cbyte=fontbyte(font_idx);
		signed char cbit=7;
		for ( cx=0; cx<cfont.x_size; ++cx)
		{
			for ( cy=0; cy<cfont.y_size; ++cy)
			{
				if ((cbyte & (1<<cbit)) != 0)
					if (cfont.inverted==0)
                                        {setPixel(x+cx, y+cy);}
					else
                                        {clrPixel(x+cx, y+cy);}
				else
					if (cfont.inverted==0)
                                        {clrPixel(x+cx, y+cy);}
					else
                                        {setPixel(x+cx, y+cy);}
				--cbit;
				if (cbit<0)
				{
					cbit=7;
					font_idx++;
					cbyte=fontbyte(font_idx);
				}
			}
		}
	}
}

void print(char *st, unsigned char x, unsigned char y)
{
	//st = ToUTF8(st);
	//unsigned char ch;
	unsigned char stl;
	unsigned char cnt;
	stl = strlen(st);
	if (x == RIGHT)
          {x = 84-(stl*cfont.x_size);}
	if (x == CENTER)
          {x = (84-(stl*cfont.x_size))/2;}

	for (cnt=0; cnt<stl; ++cnt)
        {
          _print_char(*st++, x + (cnt*(cfont.x_size)), y);
        }
}
/*
void print(String st, int x, int y)
{
	char buf[st.length()+1];

	st.toCharArray(buf, st.length()+1);
	print(buf, x, y);
}
*/

void printNumI(int num, unsigned char x, unsigned char y, unsigned char length, char filler)
{
	
	char buf[4];
	char st[4];
	bool neg=false;
	unsigned char c=0, f=0, i=0;
  
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); ++c)
                        {
                          st[c]=filler;
                        }
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg))
		{
			for (i=0; i<(length-c-neg); ++i)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (i=0; i<c; ++i)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y);
}



void printNumF(float& num, unsigned char dec, unsigned char x, unsigned char y, char divider, unsigned char length, char filler)
{

        char st[5];
        unsigned char i = 0;
//	bool neg=false;
//        
//	if (num<0)
//		neg = true;
        unsigned int lenround=1;
        for (i = 0; i < (length - dec); ++i)
        {
          lenround *= 10;
        }
        
        //num = (float)round(num*lenround)/lenround;
        snprintf_fp( st, TEMP_BUFFER_SIZE, dec, num );

//	if (divider != '.')
//	{
//		for ( i=0; i<sizeof(st); i++)
//			if (st[i]=='.')
//				st[i]=divider;
//	}
//
//	if (filler != ' ')
//	{
//		if (neg)
//		{
//			st[0]='-';
//			for ( i=1; i<sizeof(st); i++)
//				if ((st[i]==' ') || (st[i]=='-'))
//					st[i]=filler;
//		}
//		else
//		{
//			for ( i=0; i<sizeof(st); i++)
//				if (st[i]==' ')
//					st[i]=filler;
//		}
//	}

	print(st,x,y);
}


/*
void printNumF(float& num, unsigned char dec, unsigned char x, unsigned char y, unsigned char divider, unsigned char length, char filler, unsigned char selpos)
{
//double num            - значение
//unsigned char dec,          - целая часть, количество знаков
//unsigned char x,            - x
//unsigned char y,            - y
//unsigned char divider,      - положение десятичной точки
//unsigned char length,       - общая длина числа в символах
//char filler,          - заполняющий знак, типа 12,300
//unsigned char selpos        - выбор позиции в числе
  
	char st[14];
        char *str = st;
	bool neg=false;

	if (num<0)
		neg = true;

	//_convert_float(st, num, length, dec);
	//dtostrf(num,length,dec,st);
        //snprintf(st, sizeof(st), "%.*f", dec,num);
	//str = _ftoa(num, str);
        //snprintf_fp( st, TEMP_BUFFER_SIZE, 3, num );
        
        if (divider != '.')
	{
		for (unsigned char i=0; i<sizeof(st); i++)
			if (st[i]=='.')
				st[i]=divider;
	}

	if (filler != ' ')
	{
		if (neg)
		{
			st[0]='-';
			for (unsigned char i=1; i<sizeof(st); i++)
				if ((st[i]==' ') || (st[i]=='-'))
					st[i]=filler;
		}
		else
		{
			for (unsigned char i=0; i<sizeof(st); i++)
				if (st[i]==' ')
					st[i]=filler;
		}
	}
	invertText(false);
	print(st,x,y);
	invertText(true);
	
	if (selpos == 1)
	{
		_print_char(st[4-selpos],14*(4-selpos)+x,y);
	}else if (selpos >1)
	{
		_print_char(st[3-selpos],14*(3-selpos)+x,y);
	}

//	switch (selpos)
//	{
////		case 0:
////			print(st,x,y);
//		case 1:
//			print(&st[4-selpos],24*(4-selpos)+x,y);
//		case 2:
//			print(&st[3-selpos],24*(3-selpos)+x,y);
//		case 3:
//			print(&st[3-selpos],24*(3-selpos)+x,y);
//		
//	}
//	
	
	invertText(false);
	
}

*/



void setFont(unsigned char* font)
{
  cfont.font=font;
  cfont.x_size=fontbyte(0);
  cfont.y_size=fontbyte(1);
  cfont.offset=fontbyte(2);
  cfont.numchars=fontbyte(3);
  cfont.inverted=0;
}

void drawHLine(unsigned char x, unsigned char y, unsigned char l)
{
	unsigned short by, bi, cx;

	if ( (x<84) && (y<48))
	{
		for (cx=0; cx<l; ++cx)
		{
			by=((y/8)*84)+x;
			bi=y % 8;

			scrbuf[by+cx] |= (1<<bi);
		}
	}
}

void clrHLine(unsigned char x, unsigned char y, unsigned char l)
{
	unsigned short by, bi;
        char cx=0;

	if ((x<84) && (y<48))
	{
		for (cx=0; cx<l; ++cx)
		{
			by=((y/8)*84)+x;
			bi=y % 8;

			scrbuf[by+cx] &= ~(1<<bi);
		}
	}
}

void drawVLine(unsigned char x, unsigned char y, unsigned char l)
{
  unsigned char cy=0;
  if ((x<84) && (y<48))
  {
    for (cy=0; cy<l; ++cy)
    {
      setPixel(x, y+cy);
    }
  }
}

void clrVLine(unsigned char x, unsigned char y, unsigned char l)
{
  unsigned char cy=0;
  if ((x<84) && (y<48))
  {
    for (cy=0; cy<l; ++cy)
    {
      clrPixel(x, y+cy);
    }
  }
}


/*
void drawLine(int x1, int y1, int x2, int y2)
{
	int tmp, i;
	double delta, tx, ty;
	//double m, b, dx, dy;
	
	if (((x2-x1)<0))
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
		tmp=y1;
		y1=y2;
		y2=tmp;
	}
    if (((y2-y1)<0))
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
		tmp=y1;
		y1=y2;
		y2=tmp;
	}

	if (y1==y2)
	{
		if (x1>x2)
		{
			tmp=x1;
			x1=x2;
			x2=tmp;
		}
		drawHLine(x1, y1, x2-x1);
	}
	else if (x1==x2)
	{
		if (y1>y2)
		{
			tmp=y1;
			y1=y2;
			y2=tmp;
		}
		drawVLine(x1, y1, y2-y1);
	}
	else if (abs(x2-x1)>abs(y2-y1))
	{
		delta=((double)(y2-y1)/(double)(x2-x1));
		ty=(double)(y1);
		if (x1>x2)
		{
			for (i =x1; i>=x2; i--)
			{
				setPixel(i, (int)(ty+0.5));
        		ty=ty-delta;
			}
		}
		else
		{
			for (i=x1; i<=x2; i++)
			{
				setPixel(i, (int)(ty+0.5));
        		ty=ty+delta;
			}
		}
	}
	else
	{
		delta=((float)(x2-x1)/(float)(y2-y1));
		tx=(float)(x1);
        if (y1>y2)
        {
			for (i=y2+1; i>y1; i--)
			{
		 		setPixel((int)(tx+0.5), i);
        		tx=tx+delta;
			}
        }
        else
        {
			for (i=y1; i<y2+1; i++)
			{
		 		setPixel((int)(tx+0.5), i);
        		tx=tx+delta;
			}
        }
	}

}
*/
void drawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
//void drawLine(x1, y1, x2, y2) //int x1, y1, x2, y2;
{
	unsigned char t, distance;	
	unsigned char xerr=0, yerr=0, delta_x, delta_y;
	char incx, incy;
	
	delta_x = x2-x1;
	delta_y = y2-y1;
	
	if (delta_x > 0) {incx = 1;}
	else if(delta_x == 0) {incx = 0;}
	else {incx = -1;}
	
	if (delta_y > 0) {incy = 1;}
	else if(delta_y == 0) {incy = 0;}
	else {incy = -1;}

	delta_x = abs(delta_x);
	delta_y = abs(delta_y);
	
	if (delta_x > delta_y) {distance = delta_x;}
	else {distance = delta_y;}
	
	for ( t = 0; t <= distance+1; ++t )
	{
		setPixel(x1, y1);
		xerr += delta_x;
		yerr += delta_y;
		
		if (xerr > distance)
		{
			xerr -= distance;
			x1 +=incx;
		}
		
		if (yerr > distance)
		{
			yerr -= distance;
			y1 += incy;
		}
	}
}

/*
void clrLine(int x1, int y1, int x2, int y2)
{
	int tmp, i;
	double delta, tx, ty;
//	double m, b, dx, dy;
	
	if (((x2-x1)<0))
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
		tmp=y1;
		y1=y2;
		y2=tmp;
	}
    if (((y2-y1)<0))
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
		tmp=y1;
		y1=y2;
		y2=tmp;
	}

	if (y1==y2)
	{
		if (x1>x2)
		{
			tmp=x1;
			x1=x2;
			x2=tmp;
		}
		clrHLine(x1, y1, x2-x1);
	}
	else if (x1==x2)
	{
		if (y1>y2)
		{
			tmp=y1;
			y1=y2;
			y2=tmp;
		}
		clrVLine(x1, y1, y2-y1);
	}
	else if (abs(x2-x1)>abs(y2-y1))
	{
		delta=((double)(y2-y1)/(double)(x2-x1));
		ty=(double)(y1);
		if (x1>x2)
		{
			for (i=x1; i>=x2; i--)
			{
				clrPixel(i, (int)(ty+0.5));
        		ty=ty-delta;
			}
		}
		else
		{
			for (i=x1; i<=x2; i++)
			{
				clrPixel(i, (int)(ty+0.5));
        		ty=ty+delta;
			}
		}
	}
	else
	{
		delta=((float)(x2-x1)/(float)(y2-y1));
		tx=(float)(x1);
        if (y1>y2)
        {
			for (i=y2+1; i>y1; i--)
			{
		 		clrPixel((int)(tx+0.5), i);
        		tx=tx+delta;
			}
        }
        else
        {
			for (i=y1; i<y2+1; i++)
			{
		 		clrPixel((int)(tx+0.5), i);
        		tx=tx+delta;
			}
        }
	}

}

*/
void clrLine(unsigned char x1, unsigned char y1,unsigned char x2, unsigned char y2)
//void clrLine(x1, y1, x2, y2) int x1, y1, x2, y2;
{
	unsigned char t, distance;
	unsigned char xerr=0, yerr=0, delta_x, delta_y;
	char incx, incy;
	
	delta_x = x2-x1;
	delta_y = y2-y1;
	
	if (delta_x > 0) {incx = 1;}
	else if(delta_x == 0) {incx = 0;}
	else {incx = -1;}
	
	if (delta_y > 0) {incy = 1;}
	else if(delta_y == 0) {incy = 0;}
	else {incy = -1;}

	delta_x = abs(delta_x);
	delta_y = abs(delta_y);
	
	if (delta_x > delta_y) {distance = delta_x;}
	else {distance = delta_y;}
	
	for (t = 0; t <= distance+1; t++ )
	{
		clrPixel(x1, y1);
		xerr += delta_x;
		yerr += delta_y;
		
		if (xerr > distance)
		{
			xerr -= distance;
			x1 +=incx;
		}
		
		if (yerr > distance)
		{
			yerr -= distance;
			y1 += incy;
		}
	}
}


void drawRect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
	unsigned char tmp;

	if (x1>x2)
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
	}
	if (y1>y2)
	{
		tmp=y1;
		y1=y2;
		y2=tmp;
	}

	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1+1);
}
/*
void clrRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
	}
	if (y1>y2)
	{
		tmp=y1;
		y1=y2;
		y2=tmp;
	}

	clrHLine(x1, y1, x2-x1);
	clrHLine(x1, y2, x2-x1);
	clrVLine(x1, y1, y2-y1);
	clrVLine(x2, y1, y2-y1+1);
}
*/

void drawRoundRect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
	unsigned char tmp;

	if (x1>x2)
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
	}
	if (y1>y2)
	{
		tmp=y1;
		y1=y2;
		y2=tmp;
	}
	if ((x2-x1)>4 && (y2-y1)>4)
	{
		setPixel(x1+1,y1+1);
		setPixel(x2-1,y1+1);
		setPixel(x1+1,y2-1);
		setPixel(x2-1,y2-1);
		drawHLine(x1+2, y1, x2-x1-3);
		drawHLine(x1+2, y2, x2-x1-3);
		drawVLine(x1, y1+2, y2-y1-3);
		drawVLine(x2, y1+2, y2-y1-3);
	}
}
/*
void clrRoundRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
	}
	if (y1>y2)
	{
		tmp=y1;
		y1=y2;
		y2=tmp;
	}
	if ((x2-x1)>4 && (y2-y1)>4)
	{
		clrPixel(x1+1,y1+1);
		clrPixel(x2-1,y1+1);
		clrPixel(x1+1,y2-1);
		clrPixel(x2-1,y2-1);
		clrHLine(x1+2, y1, x2-x1-3);
		clrHLine(x1+2, y2, x2-x1-3);
		clrVLine(x1, y1+2, y2-y1-3);
		clrVLine(x2, y1+2, y2-y1-3);
	}
}

*/
/*
void drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
//	char ch, cl;
	
	setPixel(x, y + radius);
	setPixel(x, y - radius);
	setPixel(x + radius, y);
	setPixel(x - radius, y);
 
	while(x1 < y1)
	{
		if(f >= 0) 
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;    
		setPixel(x + x1, y + y1);
		setPixel(x - x1, y + y1);
		setPixel(x + x1, y - y1);
		setPixel(x - x1, y - y1);
		setPixel(x + y1, y + x1);
		setPixel(x - y1, y + x1);
		setPixel(x + y1, y - x1);
		setPixel(x - y1, y - x1);
	}
}
*/

/*
void clrCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
//	char ch, cl;
	
	clrPixel(x, y + radius);
	clrPixel(x, y - radius);
	clrPixel(x + radius, y);
	clrPixel(x - radius, y);
 
	while(x1 < y1)
	{
		if(f >= 0) 
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;    
		clrPixel(x + x1, y + y1);
		clrPixel(x - x1, y + y1);
		clrPixel(x + x1, y - y1);
		clrPixel(x - x1, y - y1);
		clrPixel(x + y1, y + x1);
		clrPixel(x - y1, y + x1);
		clrPixel(x + y1, y - x1);
		clrPixel(x - y1, y - x1);
	}
}

*/
/*
void drawBitmap(int x, int y, unsigned char* bitmap, int sx, int sy)
{
	int bit, cy, cx;
	byte data;

	for (cy=0; cy<sy; cy++)
	{
		bit= cy % 8;
		for(cx=0; cx<sx; cx++)
		{
			data=bitmapbyte(cx+((cy/8)*sx));
			if ((data & (1<<bit))>0)
				setPixel(x+cx, y+cy);
			else
				clrPixel(x+cx, y+cy);
		}
	}      
}
*/
/*
void PrintCurrentScreen(bool state)
{
	
	clrScr();
	setFont(TABLE5);

	drawRect(0, 0, 18,10); //int x1, int y1, int x2, int y2
	drawRect(20, 0, 44,10);
	drawRect(46, 0, 83,10);

	drawRoundRect(0, 12, 59, 38); //int x1, int y1, int x2, int y2

	invertText(!state);
	print ("Ток ", 61, 14);
	invertText(state);
	print ("Напр", 61, 24);
	drawHLine(61, 39, 22);
	
	if (state)
	{
		invertText(false);
		print ("U", 5, 2);
		invertText(true);
		print ("0-10", 20, 2);
		print ("Вых: V", 47, 2);
		invertText(false);
		//print ("Вх: V", 0, 41);
	}else{
		invertText(false);
		print ("I", 5, 2);
		invertText(true);
		print ("0-20", 20, 2);
		print ("Вых:mA", 47, 2);
		invertText(false);
		//print ("Вх:mA", 0, 41);
	}
		drawLine(20, 1, 44,1);
		drawLine(46, 1, 83,1);

  	//print ("88.88", 48, 41);

    setFont(BigNumbers);
    invertText(false);
	print ("00.0", 2, 13);
	setFont(TABLE5);
	update();
}
*/

void drawfillRect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
	unsigned char i = 0;
	drawRect(x1, y1, x2, y2);
	for (i = y1+1; i < y2; ++i)
	{
		drawHLine(x1, i, x2-x1);
	}
	
}

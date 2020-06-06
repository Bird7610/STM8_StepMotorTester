/*
 *  Control code for both SSD1306 0.96" and SSH1106 1.3" with I2C wiring
 *
 */

#include "OLED_F103F3.hpp"


#ifdef USESBUF
// global variables
unsigned char scrnbuf[512];   // screen buffer 64x64 (64x8バイト）
#endif

/////////////////////////////////////////////////////
//
// PUBLIC
//


char* OLED_F103F3::ToUTF8(char* NonUTF8String)
{
  
  if (NonUTF8String == ""){return(NonUTF8String);}
  
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


// initial set (return  true:error, false:ok)
bool OLED_F103F3::initOLED(void){
  const unsigned char tbl1[] = {
    0xae, 
    0xa6, 
    0xae, 
    0xd5, 
    0x80, 
    0xa8, 
    0x3f, 
    0xd3, 
    0x00,
    0x40, 
    0x8d, 
    0x14, 
    0x20, 
    0x00,
#ifdef ROT180  // rotation 180 degree
    0xa0, 
    0xc0,
#else          // normal
    0xa1, 
    0xc8,
#endif
    0xda, 
    0x12, 
    0x81, 
    0xcf,
    0xd9, 
    0xf1, 
    0xdb, 
    0x40, 
    0xa4, 
    0xa6
  };
  const unsigned char tbl2[] = {0x2e, 0x20, 0};                              
  bool errflg = false;
                              
  // set command
  for(int i = 0 ; !errflg && (i < (sizeof(tbl1) + sizeof(tbl2))) ; i++){
    if(i == sizeof(tbl1)) clearOLED(false);
    errflg = SendCmd((i >= sizeof(tbl1)) ? tbl2[i - sizeof(tbl1)] : tbl1[i]);
  }
  
  // set initial value
  xpos = 0; ypos = 0; ovwflg = false;

#ifdef USESBUF
  // graphic start point
  gxstpt = 64;
#endif
  // end of process
  return errflg;
}

// display invert
void OLED_F103F3::invert(bool mode){
  SendCmd(mode ? 0xa7 : 0xa6);
}

// display ON/OFF
void OLED_F103F3::displayON(bool onflg){
  SendCmd(onflg ? 0xaf : 0xae);
}

// display character
void OLED_F103F3::WriteCharacter(char character, bool neg){
  // variables
  unsigned char data[9] = {0};

  // set data
  data[0] = 0x40; //data[1] = neg ? 0xff : 0; data[7] = data[1]; data[8] = data[1];
  for(int i = 1 ; i < 9 ; i++){
    if(character < 0x80)
      if((i > 0) && (i < 6)) data[i] = ASCII[character - 0x20][i - 1];
    if(character > 0xBF)  
      if((i > 0) && (i < 6)) data[i] = ASCII[character - 0x60][i - 1];
    if(neg) data[i] = ~data[i];
  }
  I2CdataTX(OLED_ad, data, 7);

  // update position
  xpos += 8;
  if(xpos >= 128){
    xpos = 0;
    ypos++;
    SetYX(ypos, 0);
  }
}

// set total screen position 
void OLED_F103F3::SetPixelXY(unsigned char x, unsigned char y){
    unsigned char xr = x + XOFSET;
    SendCmd(0xb2 + y);
    SendCmd(xr & 0x0f);
    SendCmd(0x10 + ((xr >> 4) & 0x0f));
    xpos = x; ypos = y;
}

// set row and colum (X Y)
void OLED_F103F3::SetYX(unsigned char row, unsigned char col){
    SetPixelXY(col * 8, row);
}

// display string
void OLED_F103F3::WriteString(char *characters, bool neg){
  // write string
  //characters = ToUTF8(characters);
  
  while(*characters){
    WriteCharacter(*characters, neg);
    characters++;
  }
}

// clear display
void OLED_F103F3::clearOLED(bool neg){
  // clear overwrite flag
  ovwflg = false;
  // clear screen
#ifdef USESBUF
  unsigned char pos = 0;
#endif
  for(int j = 0 ; j < 8 ; j++){
    SetPixelXY(0, j);
    for(int i = 0 ; i < 132 ; i++){
      SendData(0, neg);
#ifdef USESBUF
      if((i >= gxstpt) && (i < (gxstpt + 64))) scrnbuf[pos++] = (neg ? 0xff : 0);
#endif
    }
  }
  SetYX(0, 0);
}

// draw frame
void OLED_F103F3::DrawFrame(bool neg){
  // variables
  char frmdat[8];
  
  // top line
  SetPixelXY(0, 0);
  for(int i = 0 ; i < 8 ; i++) frmdat[i] = 0x01;
  for(int i = 0 ; i < 8 ; i++) WriteExFont(frmdat, neg);
  // bottom line
  SetPixelXY(0, 5);
  for(int i = 0 ; i < 8 ; i++) frmdat[i] = 0x80;
  for(int i = 0 ; i < 8 ; i++) WriteExFont(frmdat, neg);
  // left line
  for(int i = 0 ; i < 8 ; i++){
    SetPixelXY(0, i);
    SendData(0xff, neg);
  }
  // right line
  for(int i = 0 ; i < 8 ; i++){
    SetPixelXY(63, i);
    SendData(0xff, neg);
  }  

}

// write extra-font data (data : 8 bytes data)
void OLED_F103F3::WriteExFont(char *data, bool neg){
  // variables
  unsigned char ldata[9];

  // write data
  ldata[0] = 0x40;
  for(int i = 1 ; i < 9 ; i++) ldata[i] = neg ? ~data[i - 1] : data[i - 1];
  I2CdataTX(OLED_ad, ldata, 9);

  // update position
  xpos += 8;
  if(xpos >= 64){
    xpos = 0;
    ypos++;
    SetYX(ypos, 0);
  }
}

void OLED_F103F3::drawAllScreenPic(char *bitmap, bool neg)
{
  SetPixelXY(0, 0);
//  for(int j = 0 ; j < 2 ; j++)
//  {
 //   SetPixelXY(0, j*8);
    for(int i = 0 ; i < 48 ; i++) 
    {
      WriteExFont(bitmap, neg);
      bitmap+=8;
    }
 // }
};





// startscrollright
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void OLED_F103F3::startscrollright(unsigned char start, unsigned char stop){
  SendCmd(RIGHT_HORIZONTAL_SCROLL);
  SendCmd(0X00);
  SendCmd(start);
  SendCmd(0X00);
  SendCmd(stop);
  SendCmd(0X00);
  SendCmd(0XFF);
  SendCmd(ACTIVATE_SCROLL);
}

// startscrollleft
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void OLED_F103F3::startscrollleft(unsigned char start, unsigned char stop){
  SendCmd(LEFT_HORIZONTAL_SCROLL);
  SendCmd(0X00);
  SendCmd(start);
  SendCmd(0X00);
  SendCmd(stop);
  SendCmd(0X00);
  SendCmd(0XFF);
  SendCmd(ACTIVATE_SCROLL);
}

// startscrolldiagright
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void OLED_F103F3::startscrolldiagright(unsigned char start, unsigned char stop){
  SendCmd(SET_VERTICAL_SCROLL_AREA);
  SendCmd(0X00);
  SendCmd(0x3f);
  SendCmd(VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  SendCmd(0X00);
  SendCmd(start);
  SendCmd(0X00);
  SendCmd(stop);
  SendCmd(0X01);
  SendCmd(ACTIVATE_SCROLL);
}

// startscrolldiagleft
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void OLED_F103F3::startscrolldiagleft(unsigned char start, unsigned char stop){
  SendCmd(SET_VERTICAL_SCROLL_AREA);
  SendCmd(0X00);
  SendCmd(0x3f);
  SendCmd(VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  SendCmd(0X00);
  SendCmd(start);
  SendCmd(0X00);
  SendCmd(stop);
  SendCmd(0X01);
  SendCmd(ACTIVATE_SCROLL);
}

void OLED_F103F3::stopscroll(void){
  SendCmd(DEACTIVATE_SCROLL);
}

unsigned char OLED_F103F3::snprintf_fp( char* destination, unsigned char available_chars, unsigned char decimal_digits,
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
            destination[i] = ' '; // store the space
          }
            
        }
        // Store the zero.
        destination[ lenght ] = 0;

    }

    return lenght;
}


void OLED_F103F3::printNumF(float& num, unsigned char dec, unsigned char x, unsigned char y, char divider, unsigned char length, char filler,  bool neg)
{
  char st[5] ;
  unsigned char i = 0;
  unsigned int lenround=1;
  for (i = 0; i < (length - dec); ++i)
  {
    lenround *= 10;
  }

  snprintf_fp( st, TEMP_BUFFER_SIZE, dec, num );
  //print(st,x,y);
  SetYX(y, x);
  WriteString(st, neg);
        
}









#ifdef USESBUF
//
// Public functions for graphic drawing
//
// set graphic position on screen
void OLED_F103F3::SetGraphicXY(unsigned char gx, unsigned char y){
    SetPixelXY(gx + gxstpt, y);
}

// clear text or graphic area (areakind 0:text(left half), 1:graphic(right)
void OLED_F103F3::clearArea(unsigned char areakind, bool neg){
  // variabales
  unsigned char xstpt = (areakind == TEXTAREA) ? ((gxstpt == 0) ? 64 : 0) : gxstpt;

  // clear overwrite flag
  if(areakind != TEXTAREA) ovwflg = false;

  // clear screen
  for(int j = 0 ; j < 8 ; j++){
    SetPixelXY(xstpt, j);
    for(int i = xstpt ; i < (xstpt + 64) ; i++){
      SendData(0, neg);
      xpos++;
    }
  }
}

// set pixel (gx:graphic area x axis)
void OLED_F103F3::setPixel(unsigned char gx, unsigned char y, bool neg){
  // check input parameter
  if((gx < 64) && (y < 64)){
    ovwflg = true;
    SetGraphicXY(gx, y >> 3);
    SendData((1 << (y % 8)), neg);
  }
}

// invert pixel (gx:graphic area x axis)
void OLED_F103F3::invPixel(unsigned char gx, unsigned char y){
  // check input parameter
  if((gx < 64) && (y < 64)){
    ovwflg = true;
    int bi = y % 8;
    SetGraphicXY(gx, y >> 3);
    SendData((1 << bi), ((scrnbuf[ypos * 64 + xpos - gxstpt] & (1 << bi)) != 0));
  }
}

// draw line (gx1,gx2:graphic area x axis)
void OLED_F103F3::drawLine(unsigned char gx1, unsigned char y1, unsigned char gx2, unsigned char y2, bool neg){
	unsigned char tmp;
	double delta, tx, ty;

    // draw horizontal line
	if (y1 == y2){
		if(gx1 > gx2){
			tmp=gx1;
			gx1=gx2;
			gx2=tmp;
		}
		drawHLine(gx1, y1, gx2 - gx1, neg);
	
	// draw vertiacl line
	} else if (gx1==gx2){
		if (y1 > y2){
			tmp=y1;
			y1=y2;
			y2=tmp;
		}
		drawVLine(gx1, y1, y2-y1, neg);
	
	// draw longer x axis tilt line
	} else if(((gx2 >= gx1) ? gx2 - gx1 : gx1 - gx2) > ((y2 >= y1) ? y2 - y1 : y1 - y2)){
		delta = (double(y2 - y1) / double(gx2 - gx1));
		ty = double(y1);
		if(gx1 > gx2){
			for (int i = gx1 ; i >= gx2 ; i--){
				setPixel((unsigned char)i, (unsigned char)(ty + 0.5), neg);
        		ty = ty - delta;
			}

		} else {
			for (int i = gx1 ; i <= gx2 ; i++){
				setPixel((unsigned char)i, (unsigned char)(ty + 0.5), neg);
        		ty = ty + delta;
			}
		}
	
	// draw longer y axis tilt line
	} else {
		delta = (float(gx2 -gx1) / float(y2 - y1));
		tx = float(gx1);
        if(y1 > y2){
	        for (int i = y1 ; i >= y2 ; i--){
                setPixel((unsigned char)(tx + 0.5), (unsigned char)i, neg);
                tx = tx - delta;
			}
        } else {
			for (int i = y1 ; i <= y2 ; i++){
		 		setPixel((unsigned char)(tx + 0.5), (unsigned char)i, neg);
        		tx = tx + delta;
			}
        }
	}
}

// draw rectangle (gx1,gx2:graphic area x axis)
void OLED_F103F3::drawRect(unsigned char gx1, unsigned char y1, unsigned char gx2, unsigned char y2, bool neg){
	unsigned char tmp;

    // exchange x and y start and end
	if (gx1 > gx2){
		tmp = gx1; gx1 = gx2; gx2 = tmp;
	}
	if (y1 > y2){
		tmp = y1; y1 = y2; y2 = tmp;
	}

    // draw rect angle
#if 0
	drawHLine(gx1, y1, gx2 - gx1, neg);      // ng
	drawHLine(gx1, y2, gx2 - gx1, neg);      // ng
#else
	drawHLine(gx1, y2, gx2 - gx1, neg);      // ng
	drawHLine(gx1, y1, gx2 - gx1, neg);      // ng
#endif
	drawVLine(gx1, y1, y2 - y1, neg);
	drawVLine(gx2, y1, y2 - y1 + 1, neg);
}

// draw round rect (gx1,gx2:graphic area x axis)
void OLED_F103F3::drawRoundRect(unsigned char gx1, unsigned char y1, unsigned char gx2, unsigned char y2, bool neg){
	unsigned char tmp;

	if (gx1 > gx2){
		tmp = gx1; gx1 = gx2; gx2 = tmp;
	}
	if (y1 > y2){
		tmp = y1; y1 = y2; y2 = tmp;
	}
	if ((gx2 - gx1) > 4 && (y2 - y1) > 4)
	{
		setPixel(gx1 + 1, y1 + 1, neg);
		setPixel(gx2 - 1, y1 + 1, neg);
		setPixel(gx1 + 1, y2 - 1, neg);
		setPixel(gx2 - 1, y2 - 1, neg);
		drawHLine(gx1 + 2, y1, gx2 - gx1 - 3, neg);
		drawHLine(gx1 + 2, y2, gx2 - gx1 - 3, neg);
		drawVLine(gx1, y1 + 2, y2 - y1 - 3, neg);
		drawVLine(gx2, y1 + 2, y2 - y1 - 3, neg);
	}
}

// draw circle (gx:graphic area x axis)
void OLED_F103F3::drawCircle(unsigned char gx, unsigned char y, unsigned char radius, bool neg){
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
	
	setPixel(gx, y + radius, neg);
	setPixel(gx, y - radius, neg);
	setPixel(gx + radius, y, neg);
	setPixel(gx - radius, y, neg);
 
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
		setPixel(gx + x1, y + y1, neg);
		setPixel(gx - x1, y + y1, neg);
		setPixel(gx + x1, y - y1, neg);
		setPixel(gx - x1, y - y1, neg);
		setPixel(gx + y1, y + x1, neg);
		setPixel(gx - y1, y + x1, neg);
		setPixel(gx + y1, y - x1, neg);
		setPixel(gx - y1, y - x1, neg);
	}
}

// draw bitmap data (gx:graphic area x axis)
void OLED_F103F3::drawBitmap(unsigned char gx, unsigned char y, unsigned char *bitmap, unsigned char sx, unsigned char sy, bool neg){
	unsigned char bit, data;

	for (int cy = 0 ; cy < sy ; cy++)
	{
		bit= cy % 8;
		for(int cx = 0 ; cx < sx ; cx++)
		{
			data = bitmap[cx + ((cy / 8) * sx)];
			if(data & (1 << bit)) setPixel(gx + cx, y + cy, neg);
		}
	}
}

// refresh graphic area
void OLED_F103F3::refreshGraph(void){
  // variables
  unsigned char ldata[2];

  // send data
  for(int i = 0 ; i < 8 ; i++){     // row
    for(int j = 0 ; j < 64 ; j++){  // colum
      // set position
      SetGraphicXY(j, i);
      // send data
      ldata[0] = 0x40; ldata[1] = scrnbuf[i * 64 + j];
      I2CdataTX(OLED_ad, ldata, 2);      
    }
  }
}

#endif


/////////////////////////////////////////////////////
//
// PRIVATE
//
// send command/character  (cmdflg true:command, false:character)
void OLED_F103F3::SendData(unsigned char idata, bool neg){
  // variables
  unsigned char ldata[2];
  unsigned char data = neg ? ~idata : idata;

#ifdef USESBUF
  // check over write for graphic drawing
  if((xpos >= gxstpt) && (xpos < (gxstpt + 64))){
    unsigned int bpt = ypos * 64 + (xpos - gxstpt);
    if(ovwflg){
      if(neg) data &= scrnbuf[bpt];
      else data |= scrnbuf[bpt];
    }
    scrnbuf[bpt] = data;
  }
#endif

  // send command
  ldata[0] = 0x40; ldata[1] = data;
  I2CdataTX(OLED_ad, ldata, 2);
}

// send command
bool OLED_F103F3::SendCmd(unsigned char cmd){
  unsigned char ldata[2];
  ldata[0] = 0x80; ldata[1] = cmd;
  return I2CdataTX(OLED_ad, ldata, 2);
}

#ifdef USESBUF
//
// Private functions for graphic drawing
//
// draw horizontal line
void OLED_F103F3::drawHLine(unsigned char gx, unsigned char y, unsigned char l, bool neg){
  if ((gx < 64) && (y < 64)){
    for (int cx = 0 ; (cx < l) && (cx < 64) && ((gx + cx) < (gxstpt + 64))  ; cx++) setPixel(gx + cx, y, neg);
  }
}

// draw vertical line
void OLED_F103F3::drawVLine(unsigned char gx, unsigned char y, unsigned char l, bool neg){
  if ((gx < 64) && (y < 64)){
    for (int cy = 0 ; (cy < l) && (cy < 64) && ((y + cy) < 64) ; cy++) setPixel(gx, y + cy, neg);
  }
}
#endif

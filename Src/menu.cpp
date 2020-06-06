/**
  ******************************************************************************
  * @file     menu.c
  * @author   MCD Application Team
  * @version  V1.1.0
  * @date     11-March-2011
  * @brief    This file contains all the functions for the menu demo.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYBTN_RIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "menu.hpp"
//#include "functions.h"

/* Private typedef -----------------------------------------------------------*/

extern	unsigned char tmpbyte;

	struct PackedVars { 
		unsigned char _show_menu      :1; 
		unsigned char _running        :1; 
		unsigned char _sensor_type    :1; 
		unsigned char _control_type   :1; 
		unsigned char _working        :1;
                unsigned char _update         :1;
                unsigned char _clear          :1;
                PackedVars():
                  _show_menu(0),
		  _running(0),
		  _sensor_type(0), 
		  _control_type(0), 
		  _working(0),
		  _update(0),
		  _clear(0)
                  {}
	};


	//static PackedVars tmpbyte;
	#define ShowMenu        ( (volatile PackedVars*)(&tmpbyte) )->_show_menu
	#define Running         ( (volatile PackedVars*)(&tmpbyte) )->_running
	#define SensorType      ( (volatile PackedVars*)(&tmpbyte) )->_sensor_type
	#define ControlType     ( (volatile PackedVars*)(&tmpbyte) )->_control_type
	#define Working         ( (volatile PackedVars*)(&tmpbyte) )->_working
	#define Update          ( (volatile PackedVars*)(&tmpbyte) )->_update
	#define Clear           ( (volatile PackedVars*)(&tmpbyte) )->_clear

    
    
extern  unsigned char Brightness_value;
extern  unsigned char Contrast_value;
//extern    bool SensorType;
//extern    bool ControlType;
//extern  bool ShowMenu;

/* Private define ------------------------------------------------------------*/
#define MENU_TOP_TEXT        (" Настройки ")
#define HELP_TEXT            ("   ПОМОЩЬ  ")
#define CONTRAST_TEXT        ("  КОНТРАСТ ")
#define BRIGHTNESS_TEXT      ("  ЯРКОСТЬ  ")
#define SENSOR_TEXT          ("  ТИП PXX  ")
#define CONTROL_TEXT         (" ВЫБОР РХХ ")
#define SENSOR_4X_TEXT       ("  4-х PXX  ")
#define SENSOR_6X_TEXT       ("  6-x РХХ  ")
#define EXIT_TEXT            ("   ВЫХОД   ")

const MENU_NEXTSTATE_T MENU_NEXTSTATE[] =
    {

        /* STATE        */           /* INPUT*/         /* NEXT STATE */
        /* MENU Level 1 */           /* Key  */
      /*
        { HELP,                      BTN_DOWN,          EXIT },
        { HELP,                      BTN_UP,            SET_CONTRAST },
        { HELP,                      BTN_SET,           EXE_HELP },
      */
      /*
        { SET_CONTRAST,              BTN_UP,            EXIT },
        { SET_CONTRAST,              BTN_DOWN,          SET_BRIGHTNESS },
        { SET_CONTRAST,              BTN_SET,           EXE_SET_CONTRAST },

        { SET_BRIGHTNESS,            BTN_UP,            SET_CONTRAST },
        { SET_BRIGHTNESS,            BTN_DOWN,          SET_TEMP_SENSOR },
        { SET_BRIGHTNESS,            BTN_SET,           EXE_SET_BRIGHTNESS },
*/
        { SET_4X_SENSOR,             BTN_UP,            EXIT },
        { SET_4X_SENSOR,             BTN_DOWN,          SET_6X_SENSOR },
        { SET_4X_SENSOR,             BTN_SET,           EXE_SET_4X },

        { SET_6X_SENSOR,             BTN_UP,            SET_4X_SENSOR },
        { SET_6X_SENSOR,             BTN_DOWN,          EXIT },
        { SET_6X_SENSOR,             BTN_SET,           EXE_SET_6X },

        { EXIT,                      BTN_UP,            SET_6X_SENSOR },
        { EXIT,                      BTN_DOWN,          SET_4X_SENSOR },
        { EXIT,                      BTN_SET,           EXE_EXIT },
        
        
        
        /* EXE menu */
        //{ EXE_HELP,                  BTN_SET,           HELP },
 /*       
        { EXE_SET_CONTRAST,          BTN_UP,            EXE_CONTRAST_inc },
        { EXE_SET_CONTRAST,          BTN_DOWN,          EXE_CONTRAST_dec },
        { EXE_SET_CONTRAST,          BTN_SET,           SET_CONTRAST },
        
        { EXE_SET_BRIGHTNESS,        BTN_UP,            EXE_BRIGHTNESS_inc },
        { EXE_SET_BRIGHTNESS,        BTN_DOWN,          EXE_BRIGHTNESS_dec },
        { EXE_SET_BRIGHTNESS,        BTN_SET,           SET_BRIGHTNESS },

        { EXE_BRIGHTNESS_dec,        BTN_UP,            EXE_BRIGHTNESS_inc },
        { EXE_BRIGHTNESS_dec,        BTN_DOWN,          EXE_BRIGHTNESS_dec },
        { EXE_BRIGHTNESS_dec,        BTN_SET,           SET_BRIGHTNESS },

        { EXE_BRIGHTNESS_inc,        BTN_UP,            EXE_BRIGHTNESS_inc },
        { EXE_BRIGHTNESS_inc,        BTN_DOWN,          EXE_BRIGHTNESS_dec },
        { EXE_BRIGHTNESS_inc,        BTN_SET,           SET_BRIGHTNESS },

        { EXE_CONTRAST_dec,          BTN_UP,            EXE_CONTRAST_inc },
        { EXE_CONTRAST_dec,          BTN_DOWN,          EXE_CONTRAST_dec },
        { EXE_CONTRAST_dec,          BTN_SET,           SET_CONTRAST },

        { EXE_CONTRAST_inc,          BTN_UP,            EXE_CONTRAST_inc },
        { EXE_CONTRAST_inc,          BTN_DOWN,          EXE_CONTRAST_dec },
        { EXE_CONTRAST_inc,          BTN_SET,           SET_CONTRAST },
       */
//        { EXE_SET_4X,       BTN_UP,            EXE_TEMP_SENSOR_analog },
//        { EXE_SET_4X,       BTN_DOWN,          EXE_TEMP_SENSOR_digital },
//        { EXE_SET_4X,       BTN_SET,           SET_TEMP_SENSOR },
//
//        { EXE_TEMP_SENSOR_analog,    BTN_UP,            EXE_TEMP_SENSOR_digital },
//        { EXE_TEMP_SENSOR_analog,    BTN_DOWN,          EXE_TEMP_SENSOR_analog },
//        { EXE_TEMP_SENSOR_analog,    BTN_SET,           SET_TEMP_SENSOR },
//
//        { EXE_TEMP_SENSOR_digital,   BTN_UP,            EXE_TEMP_SENSOR_analog },
//        { EXE_TEMP_SENSOR_digital,   BTN_DOWN,          EXE_TEMP_SENSOR_digital },
//        { EXE_TEMP_SENSOR_digital,   BTN_SET,           SET_TEMP_SENSOR },
//
//        { EXE_SET_6X,      BTN_UP,            EXE_SET_CONTROL_pulse },
//        { EXE_SET_6X,      BTN_DOWN,          EXE_SET_CONTROL_wide },
//        { EXE_SET_6X,      BTN_SET,           SET_CONTROL_TYPE },
//
//        { EXE_SET_CONTROL_pulse,     BTN_UP,            EXE_SET_CONTROL_pulse },
//        { EXE_SET_CONTROL_pulse,     BTN_DOWN,          EXE_SET_CONTROL_wide },
//        { EXE_SET_CONTROL_pulse,     BTN_SET,           SET_CONTROL_TYPE },
//
//        { EXE_SET_CONTROL_wide,      BTN_UP,            EXE_SET_CONTROL_wide },
//        { EXE_SET_CONTROL_wide,      BTN_DOWN,          EXE_SET_CONTROL_pulse },
//        { EXE_SET_CONTROL_wide,      BTN_SET,           SET_CONTROL_TYPE },

        { EXE_EXIT,                  BTN_SET,           EXE_EXIT },
        
        /* End of table */
        { (STATE_ID_T)0,      (BTNState_TypeDef)0,      (STATE_ID_T)0, }
    };

const MENU_STATE_T MENU_STATE[] =
    {
        /* STATE                     STATE TEXT            FUNCTION */
        /* MENU Level 1 */
        //{ HELP,                      HELP_TEXT,                 0 },
//        { SET_CONTRAST,              CONTRAST_TEXT,             0 },
//        { SET_BRIGHTNESS,            BRIGHTNESS_TEXT,           0 },
        { SET_4X_SENSOR,           SENSOR_4X_TEXT,               0 },
        { SET_6X_SENSOR,           SENSOR_6X_TEXT,              0 },
        { EXIT,                      EXIT_TEXT,                 0 },

        /* EXE menu */
        //{ EXE_HELP,                  0,                         ABOUT_Exec },
//        { EXE_SET_CONTRAST,          0,                         SET_CONTRAST_Exec },
//        { EXE_SET_BRIGHTNESS,        0,                         SET_BRIGHTNESS_Exec },
        { EXE_SET_4X,       0,                         SET_4X_Exec },
        { EXE_SET_6X,      0,                         SET_6X_Exec },
        { EXE_EXIT,                  0,                         EXIT_MENU_Exec },

//        { EXE_BRIGHTNESS_dec,        BRIGHTNESS_TEXT,           SET_BRIGHTNESS_dec_Exec },
//        { EXE_BRIGHTNESS_inc,        BRIGHTNESS_TEXT,           SET_BRIGHTNESS_inc_Exec },
//        
//        { EXE_CONTRAST_dec,          CONTRAST_TEXT,             SET_CONTRAST_dec_Exec },
//        { EXE_CONTRAST_inc,          CONTRAST_TEXT,             SET_CONTRAST_inc_Exec },
//        
//        { EXE_TEMP_SENSOR_analog,    SENSOR_TEXT,               SET_TEMP_SENSOR_analog_Exec },
//        { EXE_TEMP_SENSOR_digital,   SENSOR_TEXT,               SET_TEMP_SENSOR_digital_Exec },
//        
//        { EXE_SET_CONTROL_pulse,     CONTROL_TEXT,              SET_CONTROL_pulse_Exec },
//        { EXE_SET_CONTROL_wide,      CONTROL_TEXT,              SET_CONTROL_wide_Exec },
//
//        
        /* End of table */
        { (STATE_ID_T)0,             0,                         0 }
    };

//EXIT_MENU

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
STATE_ID_T PreviousTopState, State;
void (*pStateFunc)(void);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void print(char *characters, bool neg, unsigned char x, unsigned char y)
{
  OLED.SetYX(y, x);
  OLED.WriteString(characters, neg);
}

void clrScr(void)
{
  OLED.clearOLED(NORMAL);
}
/**
  * @brief  Initializes the navigation menu.
  * @param  None
  * @retval None
  */
void MenuInit(void)
{
  clrScr();
  PreviousTopState = (STATE_ID_T)0;
  State = SET_4X_SENSOR;
  pStateFunc = 0;
  Update = 1;
}

/**
  * @brief  Updates the menu states and displays the new menu on the LCD.
  * @param  uButton: new Button state
  * @retval None
  */
void MenuUpdate(BTNState_TypeDef uButton)
{
  unsigned char i = 0;
  STATE_ID_T NextState = (STATE_ID_T)0;
    /* Clear LCD */
  //clrScr();
  
  /* Search if the next menu state exists */
  for (i = 0; MENU_NEXTSTATE[i].State; i++)
  {
    if (MENU_NEXTSTATE[i].State == State && MENU_NEXTSTATE[i].Key == uButton)
    {
      NextState = MENU_NEXTSTATE[i].NextState;
      break;
    }
  }

  /* if next state exists */
  if (NextState)
  {
    /* Update the previous top state */
    switch (uButton)
    {
      case BTN_UP:
        PreviousTopState = (STATE_ID_T)0;
        for (i = 0; MENU_NEXTSTATE[i].State; i++)
        {
          if ((MENU_NEXTSTATE[i].State == NextState) && (MENU_NEXTSTATE[i].Key == BTN_UP))
          {
            PreviousTopState = MENU_NEXTSTATE[i].NextState;
            break;
          }
        }
        break;

      case BTN_SET:
        /* If a sub-menu is entered, the PreviousTopState is the current state */
        PreviousTopState = State;
        Update = 1;
        break;

      default:
        break;
    }
    State = NextState; /* Update current state */

    MenuDisplay(); /* Display the new menu on LCD */

    /* Execute the function of state if there is one*/
    for (i = 0; MENU_STATE[i].State; i++)
    {
      if (MENU_STATE[i].State == State)
      {
        pStateFunc = MENU_STATE[i].pFunc;
        if (pStateFunc) /* Check if there is a state function to be executed */
        {
          pStateFunc(); /* Execute the state function */
          pStateFunc = 0;
        }
      }
    }
  }
}

/**
  * @brief  Displays the current menu level on the LCD
  * @param  None
  * @retval None
  */
void MenuDisplay(void)
{
  unsigned char i = 0, j = 0;

  /* Clear LCD */
//  clrScr();//LCD_Clear();
  
//  _delay_ms(80);
  /* Display menu line 1 */
  if (PreviousTopState)
  {
    for (i = 0; MENU_STATE[i].State; i++)
    {
      if (MENU_STATE[i].State == PreviousTopState)
      {
        if (MENU_STATE[i].pText)
        {
          //LCD_SetCursorPos(LCD_LINE1, 0);
          //LCD_Print((unsigned char *)MENU_STATE[i].pText);
          print(MENU_STATE[i].pText, NORMAL, 0, LCD_LINE1);
          
        }
        break;
      }
    }
  }
  else
  {
    /* Menu top level reached */
//    LCD_SetCursorPos(LCD_LINE1, 0);
//    LCD_Print(MENU_TOP_TEXT);
//    invertText(0);
//    print(MENU_TOP_TEXT, 0, LCD_LINE1);
    print(MENU_TOP_TEXT, NORMAL, 0, LCD_LINE1);
  }
  
  /* Display menu line 2 */
  for (i = 0; MENU_STATE[i].State; i++)
  {
    if (MENU_STATE[i].State == State)
    {
      if (MENU_STATE[i].pText)
      {
//        LCD_SetCursorPos(LCD_LINE2, 0);
//        LCD_Print((unsigned char *)MENU_STATE[i].pText);
//        invertText(0);
//        print(MENU_TOP_TEXT, 0, LCD_LINE1);
        print(MENU_TOP_TEXT, NORMAL, 0, LCD_LINE1);
        for (j = 0; j<LCD_MENU_LINEs; ++j)
        {
//            if (j == i)
//            {
//              invertText(1);
//            }else{
//              invertText(0);
//            }
//              
//            print(MENU_STATE[j].pText, 0, LCD_LINE2+LCD_LINE*j);
            print(MENU_STATE[j].pText, j == i, 0, LCD_LINE2+LCD_LINE*j);
        }
        
      }
      break;
    }
  }
}

/******************* (C) COPYBTN_RIGHT 2009 STMicroelectronics *****END OF FILE****/

void _printBrightnessBar(unsigned char in_value)
{
  unsigned char c = 0;
  //setFont(TABLE5);
//  print("<", 0,  LCD_LINE2);
  print("<", NORMAL, 0,  LCD_LINE2);
//  print(">", 9*LCD_CHAR,  LCD_LINE2);
  print(">", NORMAL, 9*LCD_CHAR,  LCD_LINE2);
  for (c=0; c < 8; ++c )
  {
    if(in_value > (25 + 25*c))
    {
//      invertText(1);
//      print(" ", LCD_CHAR + c*LCD_CHAR,  LCD_LINE2);
      print(" ", INVERT, LCD_CHAR + c*LCD_CHAR,  LCD_LINE2);
    }else{
//      invertText(0);
//      print("_", LCD_CHAR + c*LCD_CHAR,  LCD_LINE2);
      print("_", NORMAL, LCD_CHAR + c*LCD_CHAR,  LCD_LINE2);
    }
//    invertText(0);
  }
}

void _printContrastBar(unsigned char in_value)
{
  unsigned char c = 0;
  //setFont(TABLE5);
//  print("<", 0,  LCD_LINE2);
  print("<", NORMAL, 0,  LCD_LINE2);
//  print(">", 9*LCD_CHAR,  LCD_LINE2);
  print(">", NORMAL, 9*LCD_CHAR,  LCD_LINE2);
  for (c=0; c < 8; ++c )
  {
    if(in_value > (59 + 3*c))
    {
//      invertText(1);
//      print(" ", LCD_CHAR + c*LCD_CHAR,  LCD_LINE2);
      print(" ", INVERT, LCD_CHAR + c*LCD_CHAR,  LCD_LINE2);
    }else{
//      invertText(0);
//      print("_", LCD_CHAR + c*LCD_CHAR,  LCD_LINE2);
      print("_", NORMAL, LCD_CHAR + c*LCD_CHAR,  LCD_LINE2);      
    }
//    invertText(0);
  }
}

void EXIT_MENU_Exec(void){
  MenuInit();
  ShowMenu = 0;
};

void SET_4X_Exec(void){
  MenuInit();
  SensorType = 0;
  ShowMenu = 0;
};

void SET_6X_Exec(void){
  MenuInit();
  SensorType = 1;
  ShowMenu = 0;
};


/*
void SET_CONTRAST_Exec(void){
  ShowMenu = 1;
  _printContrastBar(Contrast_value);
};

void SET_BRIGHTNESS_Exec(void){
  ShowMenu = 1;
  clrScr();
  _printBrightnessBar(Brightness_value);
  
};

void SET_BRIGHTNESS_dec_Exec(void){
  clrScr();
  ShowMenu = 1;
  State = EXE_SET_BRIGHTNESS;

  

  Brightness_value-= 25;
  if (Brightness_value <= 25)
  {
    Brightness_value = 25;
  }else if (Brightness_value >= 250){
    Brightness_value = 225;
  }
  _printBrightnessBar(Brightness_value);
  SetBrightness(Brightness_value*4);
};


void SET_BRIGHTNESS_inc_Exec(void){
  clrScr();
  ShowMenu = 1;
  State = EXE_SET_BRIGHTNESS;

  Brightness_value+= 25;
  if (Brightness_value <= 25)
  {
    Brightness_value = 25;
  }else if (Brightness_value >= 250){
    Brightness_value = 225;
  }
  _printBrightnessBar(Brightness_value);  
  SetBrightness(Brightness_value*4);
};


void SET_CONTRAST_dec_Exec(void){
  clrScr();
  ShowMenu = 1;
  State = EXE_SET_CONTRAST;

  Contrast_value-= 3;
  if (Contrast_value <= 59)
  {
    Contrast_value = 59;
  }else if (Contrast_value >= 83){
    Contrast_value = 83;
  }
  _printContrastBar(Contrast_value);
  setContrast(Contrast_value);
  
};


void SET_CONTRAST_inc_Exec(void){
  clrScr();
  ShowMenu = 1;
  State = EXE_SET_CONTRAST;

  Contrast_value+= 3;
  if (Contrast_value <= 59)
  {
    Contrast_value = 59;
  }else if (Contrast_value >= 83){
    Contrast_value = 83;
  }
  _printContrastBar(Contrast_value);
  setContrast(Contrast_value);
};
*/

//void SET_TEMP_SENSOR_Exec(void){
//  ShowMenu = 1;
//  clrScr();
////  invertText(!SensorType);
//  print("Аналоговый",!SensorType, 0,  LCD_LINE2);
////  invertText(SensorType);
//  print("Цифровой", SensorType, 0,  LCD_LINE3);
////  invertText(0);
//
//};

//void SET_CONTROL_TYPE_Exec(void){
//  ShowMenu = 1;
//  clrScr();
////  invertText(!ControlType);
//  print("Фазовое", !ControlType, 0,  LCD_LINE2);
////  invertText(ControlType);
//  print("Пропуск", ControlType, 0,  LCD_LINE3);
////  invertText(0);
//};

//void SET_CONTROL_pulse_Exec(void){ //фазовое
//  clrScr();
//  ShowMenu = 1;
//  State = EXE_SET_6X;
//  ControlType = 0;
////  invertText(!ControlType);
//  print("Фазовое",!ControlType , 0,  LCD_LINE2);
////  invertText(ControlType);
//  print("Пропуск", ControlType, 0,  LCD_LINE3);
////  invertText(0);
//  
//};

//void SET_CONTROL_wide_Exec(void){ // Пропуск
//  clrScr();
//  ShowMenu = 1;
//  State = EXE_SET_6X;
//  ControlType = 1;
////  invertText(!ControlType);
//  print("Фазовое", !ControlType, 0,  LCD_LINE2);
////  invertText(ControlType);
//  print("Пропуск", !ControlType, 0,  LCD_LINE3);
////  invertText(0);
//  
//};

//void SET_TEMP_SENSOR_analog_Exec(void){
//  clrScr();
//  ShowMenu = 1;
//  State = EXE_SET_4X;
//  SensorType = 0;
////  invertText(!SensorType);
//  print("Аналоговый", !SensorType, 0,  LCD_LINE2);
////  invertText(SensorType);
//  print("Цифровой", !SensorType, 0,  LCD_LINE3);
////  invertText(0);
//};

//void SET_TEMP_SENSOR_digital_Exec(void){
//  clrScr();
//  ShowMenu = 1;
//  State = EXE_SET_4X;
//  SensorType = 1;
////  invertText(!SensorType);
//  print("Аналоговый",!SensorType , 0,  LCD_LINE2);
////  invertText(SensorType);
//  print("Цифровой",SensorType , 0,  LCD_LINE3);
////  invertText(0);
//};

//void ABOUT_Exec(void){
////  ShowMenu = 1;
////  setFont(TABLE5);
////  print("Вторая", 0,  LCD_LINE2);
////  print("Третья", 0,  LCD_LINE3);
////  print("Четвертая", 0,  LCD_LINE4);
////  print("Пятая", 0,  LCD_LINE5);
////  print("Шестая", 0,  LCD_LINE6);
////  
//};


//void EXIT_MENU(void)
//{
//  clrScr();
//  ShowMenu = 0;
//};
//


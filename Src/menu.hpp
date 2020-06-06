/**
  ********************************************************************************
  * @file     menu.h
  * @author   MCD Application Team
  * @version  V1.1.0
  * @date     11-March-2011
  * @brief    TThis file contains functions prototypes for the menu.
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
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MENU_HPP
#define __MENU_HPP

/* Includes ------------------------------------------------------------------*/
#include "..\main.hpp"
/* Exported types ------------------------------------------------------------*/
/** State ID
  * @note Table element must not be equal to 0 as this element is
  * used to detect the end of table.
 */
#define LCD_LINE  1
#define LCD_CHAR  1 //10
#define LCD_LINE1 0
#define LCD_LINE2 1
#define LCD_LINE3 2
#define LCD_LINE4 3
#define LCD_LINE5 4
#define LCD_LINE6 5
#define LCD_MENU_LINEs 5

#define  NORMAL       false
#define  INVERT       true

typedef enum {
    /* Level 1 */
    //HELP = 1,
//    SET_CONTRAST=1,
//    SET_BRIGHTNESS,
//    SET_TEMP_SENSOR = 1,
//    SET_CONTROL_TYPE,
    SET_4X_SENSOR = 1,
    SET_6X_SENSOR,
    EXIT,
    
//    SET_CONTRAST_inc,
//    SET_CONTRAST_dec,
//    SET_BRIGHTNESS_inc,
//    SET_BRIGHTNESS_dec,
//    SET_TEMP_SENSOR_analog,
//    SET_TEMP_SENSOR_digital,
//    SET_CONTROL_pulse,
//    SET_CONTROL_wide,
    
    /* Level 2 */
//    CONTRAST,
//    BRIGHTNESS,
//    SENSOR,
//    CONTROL,

    /* Execution */
    //EXE_HELP,
//    EXE_SET_CONTRAST,
//    EXE_CONTRAST_inc,
//    EXE_CONTRAST_dec,
//    EXE_SET_BRIGHTNESS,
//    EXE_BRIGHTNESS_inc,
//    EXE_BRIGHTNESS_dec,
//    EXE_SET_TEMP_SENSOR,
//    EXE_TEMP_SENSOR_analog,
//    EXE_TEMP_SENSOR_digital,
//    EXE_SET_CONTROL_TYPE,
//    EXE_SET_CONTROL_pulse,
//    EXE_SET_CONTROL_wide,

    EXE_SET_4X,
    EXE_SET_6X,
    
    EXE_EXIT
      
} STATE_ID_T;

/** Menu NextState */
typedef struct
{
    STATE_ID_T State;
    BTNState_TypeDef Key;
    STATE_ID_T NextState;
}
MENU_NEXTSTATE_T;

/** Menu state */
typedef struct
{
    STATE_ID_T State;
    char* pText;
    void (*pFunc)(void);
}
MENU_STATE_T;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern STATE_ID_T State;
/* Exported functions --------------------------------------------------------*/
void MenuInit(void);
void MenuUpdate(BTNState_TypeDef uBTNstick);
void MenuDisplay(void);
void clrScr(void);
void print(char *characters, bool neg, unsigned char x, unsigned char y);
//void ABOUT_Exec(void);
void EXIT_MENU_Exec(void);
void SET_4X_Exec(void);
void SET_6X_Exec(void);

//void SET_CONTRAST_Exec(void);
//void SET_BRIGHTNESS_Exec(void);
//void SET_TEMP_SENSOR_Exec(void);
//void SET_CONTROL_TYPE_Exec(void);
//void SET_BRIGHTNESS_dec_Exec(void);
//void SET_BRIGHTNESS_inc_Exec(void);
//void SET_CONTRAST_dec_Exec(void);
//void SET_CONTRAST_inc_Exec(void);
//void SET_CONTROL_pulse_Exec(void);
//void SET_CONTROL_wide_Exec(void);
//void SET_TEMP_SENSOR_analog_Exec(void);
//void SET_TEMP_SENSOR_digital_Exec(void);
#endif /* __MENU_HPP */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

#ifndef KEYS_HPP
#define KEYS_HPP

//#define BTN_UP                  (1<<0)
//#define BTN_DOWN                (1<<1)
//#define BTN_SET		  (1<<2)

typedef enum
{
  BTN_NONE = 0,
  BTN_UP = (1<<0),   //выход штока
  BTN_DOWN = (1<<1), //вход штока
  BTN_SET = (1<<2),  //меню/старт
  BTN_LEFT = (1<<3),   //резерв
  BTN_RIGHT = (1<<4)   //резерв

} BTNState_TypeDef;

#define KEY_CW BTN_UP
#define KEY_CCW BTN_DOWN


//#define STATE_BTN_SET 		!PD_IDR_bit.IDR4
//#define STATE_BTN_UP		!PA_IDR_bit.IDR1
//#define STATE_BTN_DOWN 		!PA_IDR_bit.IDR2
//

#define T_BTN_REPT_START    	1000    /* задержка начала повторений */
#define T_BTN_REPT_PERIOD   	300    /* период повторений */
#define T_BTN_READ_PERIOD   	50     /* период опроса */

void initKeys(void);
unsigned short readkeys(void);


#endif

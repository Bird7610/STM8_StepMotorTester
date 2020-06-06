#include "..\main.hpp"
#include "soft_timers.hpp"
#include "keys.hpp"
#include "adc.hpp"

//set = 53760-53824
//up = 38144-38208
//down = 26496-26432

void initKeys(void)
{
  ADCInit();
}



unsigned short readkeys(void)
{
    static unsigned int adc = 0;
    static unsigned char cur_key_state;
    static unsigned long t_last_poll;
    static unsigned short t_rept;//, t_rept_down, t_rept_left, t_rept_right;
    
    unsigned char new_key_state, key_push = 0;
    
    /* Опрашиваем кнопочки каждые 50 мс */
    if(MainTimerGetMs() - t_last_poll >= T_BTN_READ_PERIOD)
    {
        t_last_poll = MainTimerGetMs();

        /* Читаем текущее состояние кнопочек */
        new_key_state = 0;
        
         adc = ReadADC(BTN_ADC_CH);
         if((adc < 55000) && (adc > 53000)) new_key_state |= BTN_SET;
         if((adc < 38250) && (adc > 38050)) new_key_state |= BTN_UP;
         if((adc < 26550) && (adc > 26350)) new_key_state |= BTN_DOWN;
         
        /* Выделяем фронты нажатия */
        key_push = new_key_state & ~cur_key_state;
        cur_key_state = new_key_state;
    }

    /* Повторение кнопочки UP */
    if(key_push & BTN_UP)
        t_rept = MainTimerGetMs() + T_BTN_REPT_START;
    if((cur_key_state & BTN_UP) && ((signed short)(MainTimerGetMs() - t_rept) >= 0)) {
        t_rept += T_BTN_REPT_PERIOD;
        key_push |= BTN_UP;
    }

    /* Повторение кнопочки DOWN */
    if(key_push & BTN_DOWN)
        t_rept = MainTimerGetMs() + T_BTN_REPT_START;
    if((cur_key_state & BTN_DOWN) && ((signed short)(MainTimerGetMs() - t_rept) >= 0)) {
        t_rept += T_BTN_REPT_PERIOD;
        key_push |= BTN_DOWN;
    }
	
    // /* Повторение кнопочки LEFT */
    // if(key_push & BTN_LEFT)
        // t_rept = MainTimerGetMs() + T_BTN_REPT_START;
    // if((cur_key_state & BTN_LEFT) && ((signed short)(MainTimerGetMs() - t_rept) >= 0)) {
        // t_rept += T_BTN_REPT_PERIOD;
        // key_push |= BTN_LEFT;
    // }

    // /* Повторение кнопочки RIGHT */
    // if(key_push & BTN_RIGHT)
        // t_rept = MainTimerGetMs() + T_BTN_REPT_START;
    // if((cur_key_state & BTN_RIGHT) && ((signed short)(MainTimerGetMs() - t_rept) >= 0)) {
        // t_rept += T_BTN_REPT_PERIOD;
        // key_push |= BTN_RIGHT;
    // }

    
//    /* Повторение кнопочки UP */
//    if(key_push & BTN_UP)
//    {
//        key_push |= BTN_UP;
//    }
//
//    /* Повторение кнопочки DOWN */
//    if(key_push & BTN_DOWN)
//    {
//        key_push |= BTN_DOWN;
//    }

    
    return key_push | (cur_key_state << 8);
}




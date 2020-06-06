#include "soft_timers.hpp"



void TimerInit(void)
{
  //CLK_PCKENR1 |= 32; // Включение тактирования TIM2
  //CLK_PCKENR1_bit.PCKEN15 = 1;
  TIM2_PSCR_bit.PSC = 7;  // Делитель 128, Частота 16 МГц
  
  TIM2_CR1_bit.CEN = 1; // Start!
  
  TIM2_ARRH = (ARR & 0xFF)>>8; // 
  TIM2_ARRL = (ARR & 0xFF);    // 
  TIM2_CR1_bit.URS = 1; // Прерывание только по переполнению счетчика

  TIM2_EGR_bit.UG = 1; // Вызываем Update Event
  TIM2_IER_bit.UIE = 1; // Разрешаем прерывание


}


void MainTimerInit(void)
{
	TimeMs = 0;
	TimerInit();
}

unsigned long MainTimerGetMs(void)
{
	return TimeMs;
}

ISR (TIM2_UIF_interrupt, TIM2_OVR_UIF_vector)
{
  TimeMs++;
  
  //TIM2_CCR3H = (unsigned char)(TIM2_CH3_CCR>>8); // 
  //TIM2_CCR3L = (unsigned char)(TIM2_CH3_CCR);    // 
   
  TIM2_SR1_bit.UIF = 0; //Сброс флага прерывания
}

ISR (TIM2_CP_interrupt, TIM2_CAPCOM_CC3IF_vector)
{
  TimeMs++;
  TIM2_SR1_bit.CC3IF = 0; //Сброс флага прерывания
}



char MainTimerIsExpired(const unsigned long Timer)
{
	return (Timer <= TimeMs);
}

/*
static unsigned long MainTimerRemainingMs(const unsigned long Timer)
{
	if ((TimeMs - Timer) < (1UL << 31))
		return (TimeMs - Timer);
	else
		return 0;
}
*/
unsigned long MainTimerSet(const unsigned long AddTimeMs)
{
	return (TimeMs + AddTimeMs);
}


void _delay_ms(const unsigned long DelayMs)
{
  unsigned long TimeDelay = MainTimerSet(DelayMs);
  
  while (!MainTimerIsExpired(TimeDelay)) {};  
}

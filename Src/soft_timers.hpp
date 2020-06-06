//========================================================================
#ifndef SOFT_TIMERS_HPP
#define SOFT_TIMERS_HPP
#include "..\main.hpp"

#define ARR 125 // Значение счета до переполнения

static unsigned long TimeMs = 0;

//extern volatile unsigned long TimeMs;

void MainTimerInit(void);
unsigned long MainTimerGetMs(void);
char MainTimerIsExpired(const unsigned long Timer);
//unsigned long MainTimerRemainingMs(const unsigned long Timer);
unsigned long MainTimerSet(const unsigned long AddTimeMs);
void TimerInit(void);

void _delay_ms(const unsigned long DelayMs);
#endif
#ifndef ADC_HPP
#define ADC_HPP
#include "..\main.hpp"
 
void ADCInit(void);
//unsigned short ReadBTNADC(void);
//unsigned short ReadRTDADC(void);
unsigned short ReadADC(unsigned char CH);

#define BTN_ADC_CH 2
#define RTD_ADC_CH 3


#endif


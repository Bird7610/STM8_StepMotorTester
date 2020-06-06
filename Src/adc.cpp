#include "adc.hpp"

static unsigned short ADC_value = 0;
void ADCInit(void)
{
    PC_DDR_bit.DDR4 = 0;  // Set input Floating without interrupt
//    PC_CR1_bit.C14 = 0;   // 
//    PC_CR2_bit.C24 = 0;   // 
//   
//    PD_DDR_bit.DDR2 = 0;  // Set input Floating without interrupt
//    PD_CR1_bit.C12 = 0;   // 
//    PD_CR2_bit.C22 = 0;   // 
//   
    //CLK_PCKENR2_bit.PCKEN23 = 1;        // Вкл тактировани
    ADC_CR1_bit.SPSEL = 4 ;             // Предделитель 
    ADC_CR1_bit.CONT = 0;               // Одиночное преобразование
    
    ADC_CR2_bit.ALIGN = 0;              // Выравнивание влево
    ADC_CR2_bit.SCAN = 0;               // Сканирование
    ADC_CSR_bit.CH = 2;              //Выбор канала AIN2
    //ADC_CSR_bit.EOCIE = 1;              // Вкл. прерывание
    ADC_CR3_bit.DBUF = 0;
    
    ADC_CR1_bit.ADON = 1; // Вкл. АЦП Начать перобразование
}
//
//unsigned short ReadBTNADC(void)
//{
//      ADC_CSR_bit.CH = BTN_ADC_CH;              //Выбор канала AIN2
//      ADC_CR1_bit.ADON = 1; // Вкл. АЦП Начать перобразование
//      return (ADC_value);
//}
//
//unsigned short ReadRTDADC(void)
//{
//      ADC_CSR_bit.CH = RTD_ADC_CH;              //Выбор канала AIN2
//      ADC_CR1_bit.ADON = 1; // Вкл. АЦП Начать перобразование
//      return (ADC_value);
//}

unsigned short ReadADC(unsigned char CH)
{
    unsigned short temph = 0;
    unsigned char templ = 0;
      ADC_CSR_bit.CH = CH;              //Выбор канала 
      ADC_CR1_bit.ADON = 1; // Вкл. АЦП Начать перобразование

      while (!ADC_CSR_bit.EOC){};
//   /* Read MSB firts*/
//    temph = ADC_DRH;
//    /* Then read LSB */
//    templ = ADC_DRL;
//
//    ADC_value = (unsigned short)((unsigned short)((unsigned short)templ << 6) | (unsigned short)((unsigned short)temph << 8));
      ADC_CSR_bit.EOC = 0;
      
      if (CH == BTN_ADC_CH){ //2
       /* Read MSB firts*/
        temph = ADC_DRH;
        /* Then read LSB */
        templ = ADC_DRL;

        ADC_value = (unsigned short)((unsigned short)((unsigned short)templ << 6) | (unsigned short)((unsigned short)temph << 8));
      }
        if(CH == RTD_ADC_CH){ //3
         /* Read MSB firts*/
          temph = ADC_DRH;
          /* Then read LSB */
          templ = ADC_DRL;

          ADC_value = (unsigned short)((unsigned short)((unsigned short)templ << 6) | (unsigned short)((unsigned short)temph << 8));
        }
      return (ADC_value);
}

void ADC1_DeInit(void)
{
//    
//    ADC1->CSR  = ADC1_CSR_RESET_VALUE;
//    ADC1->CR1  = ADC1_CR1_RESET_VALUE;
//    ADC1->CR2  = ADC1_CR2_RESET_VALUE;
//    ADC1->CR3  = ADC1_CR3_RESET_VALUE;
//    ADC1->TDRH = ADC1_TDRH_RESET_VALUE;
//    ADC1->TDRL = ADC1_TDRL_RESET_VALUE;
//    ADC1->HTRH = ADC1_HTRH_RESET_VALUE;
//    ADC1->HTRL = ADC1_HTRL_RESET_VALUE;
//    ADC1->LTRH = ADC1_LTRH_RESET_VALUE;
//    ADC1->LTRL = ADC1_LTRL_RESET_VALUE;
//    ADC1->AWCRH = ADC1_AWCRH_RESET_VALUE;
//    ADC1->AWCRL = ADC1_AWCRL_RESET_VALUE;
}

ISR (ADC_EOF, ADC1_AWS2_vector)
{
  //unsigned short clear = 0;
//  The correct way to clear the EOC flag in continuous scan mode is to load a byte in the
//ADC_CSR register from a RAM variable, clearing the EOC flag and reloading the last
//channel number for the scan sequence
  
//    unsigned short temph = 0;
//    unsigned char templ = 0;


//   /* Read MSB firts*/
//    temph = ADC_DRH;
//    /* Then read LSB */
//    templ = ADC_DRL;
//
//    ADC_value = (unsigned short)((unsigned short)((unsigned short)templ << 6) | (unsigned short)((unsigned short)temph << 8));
  
    ADC_CSR_bit.EOC = 0; //Сброс флага прерывания

}

#include "main.hpp"
//====================================================
unsigned char tmpbyte;

struct PackedByte1 { 
        unsigned char _show_menu      :1; 
        unsigned char _running        :1; 
        unsigned char _sensor_type    :1; 
        unsigned char _control_type   :1; 
        unsigned char _working        :1;
        unsigned char _update         :1;
        unsigned char _clear          :1;
        PackedByte1():
          _show_menu(0),
          _running(0),
          _sensor_type(0), 
          _control_type(0), 
          _working(0),
          _update(0),
          _clear(0)
          {}
};



//static PackedByte tmpbyte;
#define ShowMenu        ( (volatile PackedByte1*)(&tmpbyte) )->_show_menu
#define Running         ( (volatile PackedByte1*)(&tmpbyte) )->_running
#define SensorType      ( (volatile PackedByte1*)(&tmpbyte) )->_sensor_type
#define ControlType     ( (volatile PackedByte1*)(&tmpbyte) )->_control_type
#define Working         ( (volatile PackedByte1*)(&tmpbyte) )->_working
#define Update          ( (volatile PackedByte1*)(&tmpbyte) )->_update
#define Clear           ( (volatile PackedByte1*)(&tmpbyte) )->_clear

//====================================================

//====================================================
static unsigned char tmpbyte2;

struct PackedByte2{ 
        unsigned char _Keypressed:1; // Флаг нажатия
        unsigned char _BFRotation:6; // Количество нажатий
        unsigned char _Direction :1; // Направление
        PackedByte2():
          _Keypressed(0),
          _BFRotation(0),
          _Direction(0)
          {}
} ;

#define Keypressed ( (volatile PackedByte2*)(&tmpbyte2) )->_Keypressed
#define BFRotation ( (volatile PackedByte2*)(&tmpbyte2) )->_BFRotation
#define Direction  ( (volatile PackedByte2*)(&tmpbyte2) )->_Direction //CW - по часовой стрелке,  CCW - против часовой стрелке
//====================================================
        
  BTNState_TypeDef key = BTN_NONE; 

unsigned short adcValue = 0;

unsigned char PINB = 0;

float Eval(float OutMin, float OutMax, unsigned short ADCMin, unsigned short ADCMax, unsigned short InputValue)
{
    return (OutMax-OutMin)/(ADCMax-ADCMin)*(InputValue - ADCMin) + OutMin;
}

float filter(float val) 
{ //функция фильтрации
  // переменные для калмана
  static float varVolt = 0; // среднее отклонение (расчет в программе)
  static float varProcess = 200; // скорость реакции на изменение (подбирается вручную)
  static float Pc = 0.0;
  static float G = 0.0;
  static float P = 1.0;
  static float Xp = 0.0;
  static float Zp = 0.0;
  static float Xe = 0.0;
    
  Pc = P + varProcess;
  G = Pc/(Pc + varVolt);
  P = (1-G)*Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G*(val-Zp)+Xp; // "фильтрованное" значение
  return(Xe);
}

void setup()
{
  //=====
  asm("SIM"); // Interrupt off
  //=====
  // SetUp Clock CPU
    CLK_ICKR = 0;                       // Reset the Internal Clock Register.
    CLK_ICKR_HSIEN = 1;                 // Enable the HSI.
    CLK_ECKR = 0;                       // Disable the external clock.
    while (CLK_ICKR_HSIRDY == 0);       // Wait for the HSI to be ready for use.
    CLK_CKDIVR = 0;                     // Ensure the clocks are running at full speed.
    CLK_PCKENR1 = 0xff;                 // Enable all peripheral clocks.
    CLK_PCKENR2 = 0xff;                 // Ditto.
    CLK_CCOR = 0;                       // Turn off CCO.
    CLK_HSITRIMR = 0;                   // Turn off any HSIU trimming.
    CLK_SWIMCCR = 0;                    // Set SWIM to run at clock / 2.
    CLK_SWR = 0xe1;                     // Use HSI as the clock source.
    CLK_SWCR = 0;                       // Reset the clock switch control register.
    CLK_SWCR_SWEN = 1;                  // Enable switching.
    while (CLK_SWCR_SWBSY != 0);        // Pause while the clock switch is busy.
  //=====
  
  
  //=====Setup User Modules
  MainTimerInit();
  //=====
  asm("RIM");// Interrupt on
  //=====
  //=====Setup User Modules
  initKeys();
  SetupI2C();
  MenuInit();

  //=====Setup Peripheral
  PA_DDR_bit.DDR3 = 1;  // set output
  PA_CR1_bit.C13  = 1;  // set output
  //=====

}

OLED_F103F3 OLED;        // OLED drive instance

int main()
{
  //=====SETUP=====//
  setup();
  //==============//

  Working = true;
  WorkLED = 0;
  Running = 1;
  
  unsigned long Delay = MainTimerSet(WORKCYCLE), DelayMenuBTN = 0, BaseCycleDelay =  MainTimerSet(BASECYCLE), BtnDelay = 0; // in ms
  static unsigned short key_state = 0, step_key_state = 0;
  //static unsigned short rtd_adc = 0;
  static float Temperature = 0.0;
  signed char OutPower = 0;

  static unsigned char Step = 0;
  unsigned char StepCount = 0; // Количество шагов в режиме Туда-Сюда
  
  
  _delay_ms(1000);
  OLED.initOLED();  // enable after tx rx complete
  _delay_ms(100);
  OLED.displayON(true);  // enable after tx rx complete
  _delay_ms(100);
//  

        // display title
      OLED.clearOLED(NORMAL);
      OLED.SetYX(0, 0);
      OLED.WriteString("ИСПЫТАТЕЛЬ ", NEGATIVE);
      OLED.SetYX(1, 0);
      OLED.WriteString("    PXX    ", NEGATIVE);
      OLED.SetYX(2, 0);
      OLED.WriteString("всех типов", NORMAL);
      OLED.SetYX(3, 0);
      OLED.WriteString("подключения", NORMAL);
      OLED.SetYX(5, 0);
      OLED.WriteString("  ver 0.1b", NORMAL);
      
  _delay_ms(5000);
OLED.clearOLED(NEGATIVE);
  _delay_ms(150);
OLED.clearOLED(NORMAL);

//if (SensorType)
//{
//      OLED.clearOLED(NORMAL);
//      OLED.drawAllScreenPic(( char*)&motor_6x_Image, NORMAL);
//      OLED.SetYX(0, 1);
//      OLED.WriteString("обр", NORMAL);
//      OLED.SetYX(0, 5);
//      OLED.WriteString("обр", NORMAL);
//      OLED.SetYX(5, 1);
//      OLED.WriteString("обр", NORMAL);
//      OLED.SetYX(5, 5);
//      OLED.WriteString("обр", NORMAL);      
//
//}else{
//      OLED.clearOLED(NORMAL);
//      OLED.drawAllScreenPic(( char*)&motor_4x_Image, NORMAL);
//      OLED.SetYX(0, 3);
//      OLED.WriteString("обр", NORMAL);
//      OLED.SetYX(5, 3);
//      OLED.WriteString("обр", NORMAL);  
//
//}
  
  while(Working)
  {
//    if (!ShowMenu && MainTimerIsExpired(BaseCycleDelay) )
//    {   
//        BaseCycleDelay = MainTimerSet(BaseCycle) ;
//        rtd_adc = ReadADC(RTD_ADC_CH);
//        Temperature = (Eval(0.0f, 150.0f, 0, 57728, rtd_adc));
//        if (!SensorType)
//        {
//          OLED.printNumF(Temperature ,3, 2, 0, '.', 5, ' ', NORMAL);
//        }
//    }

//    if ( MainTimerIsExpired(Delay) )
//    {   
//        Delay = MainTimerSet(WorkCycle) ;
//        if (Running) WorkLED = WorkLED^1;
//        //adcValue = ReadADC();
//    }

  key_state = readkeys();

  // ============================ вход в меню ====================================    
          if (!ShowMenu && ((key_state>>8) == BTN_SET) && (MainTimerIsExpired(DelayMenuBTN))) 
  //        if (((key_state>>8) == BTN_SET) && (MainTimerIsExpired(DelayMenuBTN))) 
          {
            ShowMenu = 1;
            Update = 1;
            clrScr();
            if (ShowMenu){
              /* Display the menu */
              //setFont(TABLE5);
              MenuDisplay();
            }
            DelayMenuBTN = MainTimerSet(DELAYMENUBTN);
          }else if((key_state>>8) != BTN_SET) {
            DelayMenuBTN = MainTimerSet(DELAYMENUBTN);
          }
          
  // ==================Обработка кнопок===========================================        
//          if ((key_state&0x0F) == BTN_SET)
//          {
//            Running ^= 1;
//          }
          
          if ((key_state&0x0F) == BTN_UP)
          {
//            Update = 1;
//            Running = 0;
//            OutPower += 1;
//            if (OutPower > 100)
//            {
//              OutPower = 100;
//            }
          }
          
          if ((key_state&0x0F) == BTN_DOWN)
          {
//            Update = 1;
//            Running = 0;
//            OutPower -= 1;
//            if (OutPower < 0)
//            {
//              OutPower = 0;
//            }
          }
          
  //==============================================================================
          if (ShowMenu){ // ShowMenu
  //================обработка меню================================================
            Running = 0;

            key = (BTNState_TypeDef) (key_state & 0x00FF);
            if (key != BTN_NONE)
            {
              if (Update)
              {
                /* Display the menu */
                //clrScr();
                //MenuDisplay();
                Update = 0;
                MenuUpdate(key);
              }
            }else{
              Update = 1;
            }
            
          }else{// !ShowMenu
  //=============обрабатываем шаги================================================
            step_key_state = key_state&0xFBFB;
            
             if ((!MainTimerIsExpired(BtnDelay)) || (step_key_state > 0))
             {
                  if ((step_key_state > 0) && (!Keypressed)) 
                  {
                              BtnDelay = MainTimerSet(KEYPRESSEDTIMEOUT);
                              Keypressed = 1;
                  }
                      if ((!step_key_state) && (Keypressed))
                      {
                              Keypressed = 0;
                              BFRotation++;
                              if ( BFRotation > (KEYPRESSEDCOUNT-1)) 
                              {
                                    Direction = CW;
                                    StepCount = 0;
                              }
                  }
             }else {
                    if ((!step_key_state) && (Keypressed))
                    {
                            Keypressed = 0;
                    }
                    if (BFRotation < KEYPRESSEDCOUNT)
                    {
                            BFRotation = 0;
                    }
              }
               
             if ( MainTimerIsExpired(Delay) && ((step_key_state > 0)  || ( BFRotation >= KEYPRESSEDCOUNT )))
             {
              //Время базового цикла
              
                    if ( (BFRotation > KEYPRESSEDCOUNT))
                    {
                       BFRotation = 0;
                    }
                    
                    if((step_key_state & KEY_CW))// && (BFRotation<KEYPRESSEDCOUNT)) 
                    {
                       Direction = CW;
                    }else if((step_key_state & KEY_CCW) )// && (BFRotation<KEYPRESSEDCOUNT)) 
                    {
                       Direction = CCW;
                    }

                    if (( BFRotation > (KEYPRESSEDCOUNT-1)))
                    {
                            if (StepCount > MOTORSTEPCOUNT)
                            {
                                    Direction^=1;
                                    StepCount = 0;
                            }
                                    StepCount++;
                    }
                    
                    PINB=(0x0F & PINB);
                    Delay = MainTimerSet(PULSE_HUGH);
                                            
                    switch(Step)
                     {
                                case 0:
                                            PINB = (1<<PIN3);
                                        //Delay = MainTimerSet(PULSE_HUGH);
                                        Step+=(1+((2)*Direction));
                                break;

                                case 1:
                                            PINB = (1<<PIN0);
                                        //Delay = MainTimerSet(PULSE_HUGH);
                                        Step+=(1+((-2)*Direction));
                                break;

                                case 2:
                                            PINB = (1<<PIN2);
                                        //Delay = MainTimerSet(PULSE_HUGH);
                                        Step+=(1+((-2)*Direction));
                                break;

                                case 3:
                                            PINB = (1<<PIN1);
                                        //Delay = MainTimerSet(PULSE_HUGH);
                                        Step-=(3+((-2)*Direction));
                                break;
                                    
                    }

            }else if ((!step_key_state) && (!BFRotation))
            {
               PINB=(0x0F & PINB);
               StepCount = 0;
            }

//=================// Обновляем картинку на экране==============================
            if (Update) // Обновляем картинку на экране
            {
              Update = 0;
              clrScr();
              if (SensorType)
              {
                    OLED.clearOLED(NORMAL);
                    OLED.drawAllScreenPic(( char*)&motor_6x_Image, NORMAL);
                    OLED.SetYX(0, 1);
                    OLED.WriteString("38", NORMAL);
                    OLED.SetYX(0, 5);
                    OLED.WriteString("35", NORMAL);
                    OLED.SetYX(5, 1);
                    OLED.WriteString("38", NORMAL);
                    OLED.SetYX(5, 5);
                    OLED.WriteString("35", NORMAL);      

              }else{
                    OLED.clearOLED(NORMAL);
                    OLED.drawAllScreenPic(( char*)&motor_4x_Image, NORMAL);
  //                  OLED.SetYX(0, 3);
  //                  OLED.WriteString("38", NORMAL);
                    OLED.printNumF(Temperature ,3, 2, 0, '.', 5, ' ', NORMAL);
                    OLED.SetYX(5, 3);
                    OLED.WriteString("35", NORMAL);  

              }
            }
            
          }// !ShowMenu
    
  }
  
  return 0;
}

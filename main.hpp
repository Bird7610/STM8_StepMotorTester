#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostm8s103f3.h>

#include <stdlib.h>     /* abs */

#include <cstring>                        // для strlen

#include "lib\inc\stm8s_type.h"
#include <intrinsics.h>

#include "src\soft_timers.hpp"
#include "src\adc.hpp"
//#include "src\pwm.hpp"
//#include "src\spi.hpp"
//#include "src\lcd.hpp"
#include "src\i2c.hpp"
#include "src\keys.hpp"
#include "src\menu.hpp"

#include <intrinsics.h>
#include <iostm8s103f3.h>

#include "src\OLED_F103F3.hpp"

 #define STRINGVECTOR(x) #x
 #define VECTOR_ID(x) STRINGVECTOR( vector = (x) )
 #define ISR( a, b )  \
 _Pragma( VECTOR_ID( (b) ) )        \
 __interrupt void (a)( void )
   
 #define ISR_TRAP(a) \
 _Pragma( VECTOR_ID( 1 ) ) \
 __interrupt void (a) (void)  
   

#define WorkLED PA_ODR_bit.ODR3
#define BASECYCLE 1000 // ms
#define SOMEDELAY 500 // ms
#define UPDATEPWM 50 // ms
#define WORKCYCLE 500 // ms
#define DELAYMENUBTN 3000 // ms

   
#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
   
#define KEYPRESSEDTIMEOUT 500
#define KEYPRESSEDCOUNT 3
#define MOTORSTEPCOUNT 200
#define PULSE_HUGH 6UL
#define PULSE_LOW 0UL

#define CW 1    /* по часовой (ClockWise)  */
#define CCW 0   /* против часовой (CounterClockWise) */

   
// I2C device related constants.
#define I2C_READ                1
#define I2C_WRITE               0

   
   
   
#endif

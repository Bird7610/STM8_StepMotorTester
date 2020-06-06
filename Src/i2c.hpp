#ifndef I2C_HPP
#define I2C_HPP
#include "..\main.hpp"

// global variables
//static unsigned long elpstime;  // elapse timer (mS)
//static unsigned long elpstend;  // elapse timer check end
//static unsigned char i2cerr;    // I2C error status

void SetupI2C(void);
bool I2CdataTX(unsigned char address, unsigned char *txbuf, int txnum);
bool I2CdataRX(unsigned char address, unsigned char *rxbuf, int rxnum);

#ifdef USESBUF
// Sine wave table (%) (0 to 90 degree range : value 0 to 100)
const unsigned char Sine[] = {0, 11, 22, 33, 43, 53, 62, 71, 78, 85, 90, 94, 97, 99, 100};
#endif

// delay wait (dly mS)
//#define DELAY(dly) {elpstime = MainTimerGetMs(); elpstend = elpstime + dly; while(elpstend > elpstime);}

#endif
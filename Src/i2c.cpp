/*       --- Setup I2C master mode ---      */
#include "i2c.hpp"


/*       --- Timer common process ---       */
// report system clock (mS)
inline unsigned long millis(void){
 // elpstime = MainTimerGetMs();
  return MainTimerGetMs();
}


//  Initialise the I2C system.
void SetupI2C(void)
{
    // Setup the I2C clock information.
    I2C_FREQR = 16;                     // Set the internal clock frequency (MHz).
    I2C_CCRH_F_S = 1;                   // I2C running is high speed mode.
    I2C_CCRH_DUTY = 1;                  // I2C pulse duty Low/High=16/9
    I2C_CCRL = 2;                       // SCL clock speed is 320KHz (400 KHz).
    I2C_CCRH_CCR = 0x00;

    //  Set the address mode of this device.
    I2C_OARH_ADDMODE = 0;               // 7 bit address mode.
    I2C_OARH_ADDCONF = 1;               // Docs say this must always be 1.

    //  Setup the bus characteristics.
    I2C_TRISER = 17;                    // setting I2C_FREQR + 1

    //  Setup I2C port pullup
    PB_CR1_C14 = 1;
    PB_CR1_C15 = 1;
}

/*       --- Data TX / RX with I2C line ---      */
// I2C data tx (return error)
bool I2CdataTX(unsigned char address, unsigned char *txbuf, int txnum){
  // variables
  volatile unsigned char dum;   // read dummy buffer
  unsigned long waitlmt;        // wait limit time (mS)
  #define  CHKWAIT  5           // check wait time (mS)
  bool rlt = false;             // return

  // enable I2C
  I2C_CR1_PE = 1;               // enable I2C

  // start I2C
  I2C_SR2 &= 0xf0;              // clear error flag
  I2C_CR2_ACK = 0;              // ack return clear
  I2C_CR2_STOP = 0;             // stop bit set clear
  I2C_CR2_START = 1;            // set start bit
  waitlmt = MainTimerGetMs() + CHKWAIT; // set timeout
  while(!I2C_SR1_SB && (MainTimerGetMs() < waitlmt));
  if(MainTimerGetMs() >= waitlmt) rlt = true;
  dum = I2C_SR1;                // clear start bit set flag

  // set I2C address
  if(!rlt){
    I2C_DR = (address << 1) | I2C_WRITE;  // set device address
    waitlmt = MainTimerGetMs() + CHKWAIT;         // set timeout
    while(!I2C_SR1_ADDR && (MainTimerGetMs() < waitlmt)); 
    if(MainTimerGetMs() >= waitlmt) rlt = true;   // timeout error
    dum = I2C_SR1;              // clear addres transmitted flag
    dum = I2C_SR3;
  }

  // set data
  for(int i = 0 ; !rlt && (i < txnum) ; i++){
    waitlmt = MainTimerGetMs() + CHKWAIT; // set timeout
    while(!I2C_SR1_TXE && (MainTimerGetMs() < waitlmt)); // wait tx buffer enmpty
    if(MainTimerGetMs() >= waitlmt) rlt = true;   // timeout error
    I2C_DR = txbuf[i];          // set tx data
  }

  // wait last data tx end
  if(!rlt){
    waitlmt = MainTimerGetMs() + CHKWAIT; // set timeout
    while((!I2C_SR1_TXE || !I2C_SR1_BTF) && (MainTimerGetMs() < waitlmt));
    if(MainTimerGetMs() >= waitlmt) rlt = true;   // timeout error
  }
  I2C_CR2_STOP = 1;             // set stop bit

  // end of process
  while(I2C_SR3_BUSY && !rlt);  // wait I2C bus end
  I2C_CR1_PE = 0;               // disable I2C
  return rlt;
}

// I2C data rx (return error)
bool I2CdataRX(unsigned char address, unsigned char *rxbuf, int rxnum){
  // variables
  volatile unsigned char dum;   // read dummy buffer
  unsigned long waitlmt;        // wait limit time (mS)
  bool rlt = false;             // return

  // enable I2C
  I2C_CR1_PE = 1;               // enable I2C

  // start I2C
  I2C_SR2 &= 0xf0;              // clear error flag
  I2C_CR2_ACK = 1;              // ack return set
  I2C_CR2_STOP = 0;             // stop bit set clear
  I2C_CR2_START = 1;            // set start bit
  waitlmt = MainTimerGetMs() + CHKWAIT; // set timeout
  while(!I2C_SR1_SB && (MainTimerGetMs() < waitlmt));
  if(MainTimerGetMs() >= waitlmt) rlt = true;
  dum = I2C_SR1;                // clear start bit set flag

  // set I2C address
  if(!rlt){
    I2C_DR = (address << 1) | I2C_READ;   // set device address
    waitlmt = MainTimerGetMs() + CHKWAIT;         // set timeout
    while(!I2C_SR1_ADDR && (MainTimerGetMs() < waitlmt)); 
    if(MainTimerGetMs() >= waitlmt) rlt = true;   // timeout error
    dum = I2C_SR1;              // clear addres transmitted flag
    dum = I2C_SR3;
  }

  // get data
  for(int i = 0 ; !rlt && (i < rxnum) ; i++){
    if(i >= (rxnum - 1)){
      I2C_CR2_ACK = 0;
      I2C_CR2_STOP = 1;
    }
    waitlmt = MainTimerGetMs() + CHKWAIT;         // set timeout
    while(!I2C_SR1_RXNE && (MainTimerGetMs() < waitlmt)); // wait rx buffer enmpty
    if(MainTimerGetMs() >= waitlmt) rlt = true;   // timeout error
    dum = I2C_DR;
    if(!rlt) rxbuf[i] = dum;
  }

  // end of process
  while(I2C_SR3_BUSY && !rlt);  // wait I2C bus end
  I2C_CR2_ACK = 0;
  I2C_CR1_PE = 0;               // disable I2C
  return rlt; 
}


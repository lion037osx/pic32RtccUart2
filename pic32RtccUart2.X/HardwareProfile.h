/* 
 * File:   HardwareProfile.h
 * Author: leo
 *
 * Created on July 2, 2017, 1:21 AM
 */

#ifndef HARDWAREPROFILE_H
#define	HARDWAREPROFILE_H

#include "p32xxxx.h"
#include "xc.h"
#include "plib.h"
#include <GenericTypeDefs.h>
#include "uart2.h"
//#include "ds1307rtcc.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
#define debug UART2PrintString    

#define GetSystemClock()            (80000000ul)
#define GetPeripheralClock()        (GetSystemClock()/2) 
#define GetInstructionClock()       (GetSystemClock())

// I/O definitions for the Explorer16
#define CTS     _RF12               // Cleart To Send, input
#define RTS     _RF13               // Request To Send, output
#define TRTS    TRISFbits.TRISF13   // tris control for RTS pin
#define BAUDRATE2       115200UL
#define BRG_DIV2        8 
#define BRGH2           0 
#define Fsck	100000
#define BRG_VAL 	((GetPeripheralClock()/2/Fsck)-2)

// timing and baud rate calculations
#define BAUDRATEREG2        ((GetPeripheralClock()+(BRG_DIV2/2*BAUDRATE2))/BRG_DIV2/BAUDRATE2-1)

#define I2C_CLOCK_FREQ              100000
    
// EEPROM Constants
#define EEPROM_I2C_BUS              I2C1
#define EEPROM_ADDRESS              0x50        // 0b1010000 Serial EEPROM address

#define I2C_BUS              I2C1
#define ADDRESS              0x50        // 0b1010000 Serial EEPROM address
        
//Config PIN OUTS SDA SCL    
#define   TRIS__RG3_SDA TRISGbits.TRISG3
#define TRIS__RG2_SCL     TRISGbits.TRISG2

#define init_sda_scl()    {TRISGbits.TRISG2=1;TRISGbits.TRISG3=1;}
    
#define TPI2CCON    I2C1CON
#define TPCONbits   I2C1CONbits
#define TPI2CSTAT   I2C1STAT
#define TPSTATbits  I2C1STATbits
#define TPI2CMSK     I2C1MSK
#define TPI2CRCV     I2C1RCV
#define TPI2CTRN     I2C1TRN
#define TPI2CADD     I2C1ADD
#define TPI2CBRG    I2C1BRG

#define C_TP_ADDR   0x3E
#define C_TP_READ   0x7D
#define C_TP_WRITE  0x7C    
    
#define LIS_I2C_ADDRESS             0x19
#define LIS_ID_REG                  0x0F
#define LIS_CNTRL_REG1              0x20 

#define LED_A0 LATAbits.LATA0
#define LED_A1 LATAbits.LATA1
#define LED_A2 LATAbits.LATA2
#define LED_A3  LATAbits.LATA3    
    
#define config_leds() {TRISAbits.TRISA0=0;TRISAbits.TRISA1=0;TRISAbits.TRISA2=0;TRISAbits.TRISA3=0;}    
    
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* HARDWAREPROFILE_H */


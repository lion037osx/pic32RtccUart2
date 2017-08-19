//#include "HardwareProfile.h"
#include "uart2.h"


// initialize the UART2 serial port
void initU2( void)
{   
    U2BRG = BAUDRATEREG2;
    U2MODE = 0;
    U2MODEbits.BRGH = BRGH2;
    U2STA = 0;
    U2MODEbits.UARTEN = 1;
    U2STAbits.UTXEN = 1;
    IFS1bits.U2RXIF = 0;
    
    #if defined (__PIC32MX__)
        U2STAbits.URXEN = 1;
    #endif
} // initU2




        void UART2PutChar( char ch )
{
    U2TXREG = ch;
    #if !defined(__PIC32MX__)
        Nop();
    #endif
    while(U2STAbits.TRMT == 0);
}
        
void UART2PrintString( char *str )
{
    unsigned char c;

    while( (c = *str++) )
        UART2PutChar(c);
}

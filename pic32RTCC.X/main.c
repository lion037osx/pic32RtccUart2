/*********************************************************************
 *
 *      Example Binky LEDs
 *
 *********************************************************************
 * FileName:        main.c
 * Dependencies:    plib.h
 *
 * Processor:       PIC32MX
 *
 * Complier:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PIC32MX Microcontroller is intended
 * and supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *********************************************************************
 * This program uses Explorer-16 to blink all of its LEDs at once.
 *
 * Platform: 	Explorer-16 with PIC32MX PIM
 *
 ********************************************************************/
#include <plib.h>
#include <xc.h>
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8


#include "HardwareProfile.h"

//#define Nop() asm( "nop" )                 //No-operation; asm stands for assembly, using an assembly command in C.  Cool!

#define INPUT_F5       PORTFbits.RF5
#define INPUT_F6       PORTFbits.RF6
UINT8 err_flg=0x00;

UINT8 i2c_rtcc_wr(UINT8 rtcc_reg,UINT8 time_var );
int RcvDataI2C(unsigned int address) ;
void SendDataI2C (int data, unsigned int address);
void DelayMs( unsigned t);
void i2c_wait(unsigned int cnt);


int main(void)
{
    int i;
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);  
    AD1PCFG = 0xFFFF;
    set_tris_sda_scl();
    initUART2();
    puts_UART2("Project pic32 with RTCC DS1307...\r\n");
    
mJTAGPortEnable(DEBUG_JTAGPORT_OFF);
mPORTAClearBits(BIT_7 | BIT_6 | BIT_5 | BIT_5 | BIT_4 | BIT_3 | BIT_2 | BIT_1 | BIT_0 );
mPORTASetPinsDigitalOut( BIT_7 | BIT_6 | BIT_5 | BIT_5 | BIT_4 | BIT_3 | BIT_2 | BIT_1 | BIT_0 );

//Enable I2C channel and set the baud rate to BRG_VAL)
//OpenI2C1 ( I2C_EN, BRG_VAL );
OpenI2C1(I2C_EN|I2C_ACK_EN,BRG_VAL);
I2CSetFrequency(I2C1,SYS_FREQ,50000);


    while(1)
    {
            mPORTAToggleBits( BIT_0 );
            i = 1024*1024;
            while(i--);
    }
}

int RcvDataI2C(unsigned int address) {
	StartI2C1();				//Send line start condition
	IdleI2C1();			        //Wait to complete
	MasterWriteI2C1((address << 1) | 1);	//Write out slave address OR 1 (read command)
	IdleI2C1();				//Wait to complete
	int rcv = MasterReadI2C1();		//Read in a value
	StopI2C1();				//Send line stop condition
	IdleI2C1();				//Wait co complete
	return rcv;				//Return read value
}

void SendDataI2C (int data, unsigned int address){
	StartI2C1();	        //Send the Start Bit
	IdleI2C1();		//Wait to complete

	MasterWriteI2C1((address << 1) | 0);  //Sends the slave address over the I2C line.  This must happen first so the 
                                             //proper slave is selected to receive data.
	IdleI2C1();	        //Wait to complete

	MasterWriteI2C1(data);  //Sends data byte over I2C line
	IdleI2C1();		//Wait to complete

	StopI2C1();	        //Send the Stop condition
	IdleI2C1();	        //Wait to complete

} //end function


void DelayMs( unsigned t)
// This uses Timer 1, can be changed to another timer. Assumes FPB = SYS_FREQ
{
    OpenTimer1(T1_ON | T1_PS_1_256, 0xFFFF);
    while (t--)
    {  // t x 1ms loop
        WriteTimer1(0);
        while (ReadTimer1() < SYS_FREQ/256/1000);
	}
	CloseTimer1();
} // Delayms

void i2c_wait(unsigned int cnt)
{
	while(--cnt)
	{
		Nop();
		Nop();
	}
}


UINT8 i2c_rtcc_wr(UINT8 rtcc_reg,UINT8 time_var ){
	StartI2C1();	        //Send the Start Bit
	IdleI2C1();		//Wait to complete
 	MasterWriteI2C1((ADDR_RTCC_WRITE << 1) | 0x00);     
    	IdleI2C1();	
 	MasterWriteI2C1((rtcc_reg << 1) | 0);     
    	IdleI2C1();	
 	MasterWriteI2C1((time_var << 1) | 0);     
    	IdleI2C1();	        
    
	StopI2C1();	        //Send the Stop condition
	IdleI2C1();	 
}


UINT8 i2c_rtcc_rd(UINT8  rtcc_reg){ // reads a data byte from the I2C RTCC
static UINT8 rtcc_buf=0x00;  // general data buffer for the i2c rtcc 
int err_flg;
StartI2C1();  // start I2C communication
MasterWriteI2C1(ADDR_RTCC_WRITE) ;  // write DEVICE ADDR for RTCC WRITES
if(err_flg){                // if an error occured at a PICTAIL removal,
     return rtcc_buf   ;   
}  
// leave fast the function 
MasterWriteI2C1(rtcc_reg);  // write the register ADDRESS
if(err_flg)                // if an error occured at a PICTAIL removal, 
{ return rtcc_buf   ;   }  // leave fast the function    
StartI2C1()           ;  // RESTART for READS 
MasterWriteI2C1(ADDR_RTCC_READ)  ;  // send the DEVICE ADDRESS for RTCC READS.
if(err_flg){                // if an error occured at a PICTAIL removal,
    return rtcc_buf   ;   
    }  // leave fast the function 
rtcc_buf=MasterReadI2C1();  // read register (stored in 'rtcc_buf')
IdleI2C1() ;  // NOACK from MASTER (last read byte)
StopI2C1() ;  // stop I2C communication
return rtcc_buf    	;  
}  // return the read byte, stored in the general rtcc buffer
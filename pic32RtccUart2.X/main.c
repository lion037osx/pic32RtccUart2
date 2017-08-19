/*
** UART2 RS232 asynchronous communication demonstration code
*/

// configuration bit settings, Fcy=80MHz, Fpb=40MHz
#include "HardwareProfile.h"
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2	
#pragma config FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL
//#pragma config DEBUG = ON

#include "mcp79401.h"

#define CARRIER 0
/*************  FUNCTIONS *****************/
UINT8 err_flg=0x00;

//void i2c_rtcc_wr(UINT8 register_rtcc,UINT8 time_var );
//UINT8  i2c_rtcc_rd(UINT8  register_rtcc);
void DelayMs(unsigned t);
void startBatteryMcp79401(void);
void initMcp79401(void);
void sram_byte_write_i2c1(UINT8 address,UINT8 data );
UINT8 random_read(UINT8 address);

void ptime(void);


/*************** MAIN ***************/
int main(void)
{
static UINT8 time,tmp;
char str[20];    
int dec ,uni;
SYSTEMConfigPerformance(GetSystemClock());

// 1. init the UART2 serial port 
initU2();
config_leds();
UART2PrintString("\r\n\t\t\t******************** PROJECT  PIC32 UART2  I2C RTCC MCP79410 ********************\r\n\r\n\r\n");
init_sda_scl();
OpenI2C1(I2C_EN|I2C_ACK_EN,BRG_VAL);
I2CSetFrequency(I2C1,GetSystemClock(),I2C_CLOCK_FREQ);

LED_A0=LED_A1=LED_A2=LED_A3=0;
//UART2PrintString("step1\r\n");
initMcp79401();
//UART2PrintString("step2\r\n");
startBatteryMcp79401();

    while(1){     
            
            time=random_read(ADDR_SEC);
            if(time!=tmp){
            ptime();
            tmp=time;
            }
            /*
            if(time!=tmp){
           dec =(int) ((time>>4)&0x07)*10;
            uni=(int)0x0f&time;
            sprintf(str,"time : %d",dec+uni);
            UART2PrintString(str);
            UART2PrintString("\r\n");
            tmp=time;
            }
             */
           
             
            DelayMs(100);
        } 
}// end main











void DelayMs( unsigned t)
// This uses Timer 1, can be changed to another timer. Assumes FPB = SYS_FREQ
{
    OpenTimer1(T1_ON | T1_PS_1_256, 0xFFFF);
    while (t--)
    {  // t x 1ms loop
        WriteTimer1(0);
        while (ReadTimer1() < GetSystemClock()/256/1000);
	}
	CloseTimer1();
} // Delayms





void ack_i2c1(void){
IdleI2C1();
}

void no_ack_i2c1(void){
IdleI2C1();
}










void sram_byte_write_i2c1(UINT8 address,UINT8 data ){
    StartI2C1();	        //Send the Start Bit
    IdleI2C1();
    MasterWriteI2C1(ADDR_RTCC_WRITE );     
    ack_i2c1();
    MasterWriteI2C1(address);    
    ack_i2c1();
    MasterWriteI2C1(data);
    ack_i2c1();
    StopI2C1();	        //Send the Stop condition
    IdleI2C1();	
}

UINT8 current_address_read(void ){
static UINT8 rtcc_buf=0x00;

    StartI2C1();	  
    ack_i2c1();
    MasterWriteI2C1(ADDR_RTCC_READ);       
    ack_i2c1();
    rtcc_buf=MasterReadI2C1();  
    ack_i2c1();
    StopI2C1();	
    ack_i2c1();
    return (rtcc_buf);
}


UINT8 random_read(UINT8 address){
    UINT8 buff;

    StartI2C1();	
    IdleI2C1();

    MasterWriteI2C1(ADDR_RTCC_WRITE);       
    ack_i2c1();   

    MasterWriteI2C1(address);
    ack_i2c1();    

    buff=current_address_read();
    return (buff);
}






void startBatteryMcp79401(void){             // initialization of the I2C RTCC: enables the battery circuit                                     
// START bit is located in the Sec register
                                     // time/date will be set in 'ini_i2c_time()' 

unsigned char day=0; 				 // local variable (stores the RTCC DAY register)     
day = random_read(ADDR_DAY); 		 // read day + OSCON bit   
sram_byte_write_i2c1(ADDR_DAY,day|VBATEN);
}	 // enable the battery back-up  

void initMcp79401(void){             // initialization of time/date vars on the I2C RTCC
unsigned char day=0;   				 // local variable (stores the RTCC DAY register) 

day = random_read(ADDR_DAY);   		 // read day + OSCON bit 

if((day&OSCON)==OSCON);            // if oscillator = already running, do nothing. 
else{                                // if oscillator = not running, set time/date(arbitrary)
    
	sram_byte_write_i2c1(ADDR_YEAR,0x17);         // initialize YEAR  register : (20)11           
	sram_byte_write_i2c1(ADDR_MNTH,0x07);   	     // initialize MONTH register : november   
	sram_byte_write_i2c1(ADDR_DATE,0x06);   	     // initialize DATE  register : date = 01  
	sram_byte_write_i2c1(ADDR_HOUR,0x18);   	     // initialize HOUR  register : hour = 00  
	sram_byte_write_i2c1(ADDR_MIN,0x56) ;   	     // initialize MIN   register : min  = 00  
	sram_byte_write_i2c1(ADDR_SEC,START_32KHZ);
	}


} // init SEC register and start the 32khz oscillator 
 

void ptime(void){
    CHAR str[60];
    int year,month,day,hour,min,sec;
    
year=random_read(ADDR_YEAR);      
month=random_read(ADDR_MNTH);   	 
day=random_read(ADDR_DATE);   
hour=random_read(ADDR_HOUR); 
min=random_read(ADDR_MIN) ;   	   
sec=random_read(ADDR_SEC);
year=2000+(((year>>4)&0x7)*10)+(year&0xf);

    sprintf(str," date %d/%1d%1d/%1d%1d",year,(month>>4)&0x7,month&0xf,(day>>4)&0x7,day&0xf);
    UART2PrintString(str);
        sprintf(str,"  %1d%1d:%1d%1d:%1d%1d\r\n",(hour>>4)&0x7,hour&0xf,(min>>4)&0x7,min&0xf,(sec>>4)&0x7,sec&0xf);
    UART2PrintString(str);
}


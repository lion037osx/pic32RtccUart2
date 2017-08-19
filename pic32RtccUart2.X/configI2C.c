

#include "HardwareProfile.h"
#include "ds1307rtcc.h"


int  RcvDataI2C(unsigned int address) {
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


void i2c_wait(unsigned int cnt)
{
	while(--cnt)
	{
		Nop();
		Nop();
	}
}

void i2c_rtcc_wr(UINT8 register_rtcc,UINT8 time ){
	StartI2C1();	        //Send the Start Bit
	IdleI2C1();		//Wait to complete
 	MasterWriteI2C1(*(unsigned char *)(ADDR_RTCC_WRITE << 1) | 0x00);     
    	IdleI2C1();	
 	MasterWriteI2C1((register_rtcc << 1) | 0);     
    	IdleI2C1();	
 	MasterWriteI2C1((time << 1) | 0);     
    	IdleI2C1();	        
	StopI2C1();	        //Send the Stop condition
	IdleI2C1();	 
}


UINT8  i2c_rtcc_rd(UINT8  register_rtcc){ // reads a data byte from the I2C RTCC
static UINT8 rtcc_buf=0x00;  // general data buffer for the i2c rtcc 
int err_flg;

StartI2C1();  // start I2C communication
MasterWriteI2C1(ADDR_RTCC_WRITE) ;  // write DEVICE ADDR for RTCC WRITES
    if(err_flg){                // if an error occured at a PICTAIL removal,
         return rtcc_buf   ;   
    }  
// leave fast the function 
MasterWriteI2C1(register_rtcc);  // write the register ADDRESS
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

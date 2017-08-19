#include "HardwareProfile.h"
#include "ds1307.h"



void I2C_Master_Init(const unsigned long c)
{
    //SSPCON = 0b00101000;            //SSP Module as Master
//  SSPCON2 = 0;
//  SSPADD = (_XTAL_FREQ/(4*c))-1; //Setting Clock Speed
 // SSPSTAT = 0;
  
    
  TRIS__RG3_SDA = 1;                   //Setting as input as given in datasheet
  TRIS__RG2_SCL = 1;                   //Setting as input as given in datasheet
}

void I2C_Master_Wait(void)
{
//  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}


void I2C_Master_Start(void)
{
  I2C_Master_Wait();    
  //SEN = 1;             //Initiate start condition
}

void I2C_Master_Stop(void)
{
  I2C_Master_Wait();
 // PEN = 1;           //Initiate stop condition
}

void I2C_Master_Write(unsigned d)
{
  I2C_Master_Wait();
  //SSPBUF = d;         //Write data to SSPBUF
}

unsigned short I2C_Master_Read(unsigned short a)
{
  unsigned short temp;
  I2C_Master_Wait();
  //RCEN = 1;
  //I2C_Master_Wait();
  //temp = SSPBUF;      //Read data from SSPBUF
  //I2C_Master_Wait();
//  ACKDT = (a)?0:1;    //Acknowledge bit
 // ACKEN = 1;          //Acknowledge sequence
  return temp;
}


/*******************************************************************************
  Function:
    BOOL StartTransfer( BOOL restart )

  Summary:
    Starts (or restarts) a transfer to/from the EEPROM.

  Description:
    This routine starts (or restarts) a transfer to/from the EEPROM, waiting (in
    a blocking loop) until the start (or re-start) condition has completed.

  Precondition:
    The I2C module must have been initialized.

  Parameters:
    restart - If FALSE, send a "Start" condition
            - If TRUE, send a "Restart" condition
    
  Returns:
    TRUE    - If successful
    FALSE   - If a collision occured during Start signaling
    
  Example:
    <code>
    StartTransfer(FALSE);
    </code>

  Remarks:
    This is a blocking routine that waits for the bus to be idle and the Start
    (or Restart) signal to complete.
  *****************************************************************************/

BOOL StartTransfer( BOOL restart )
{
    I2C_STATUS  status;

    // Send the Start (or Restart) signal
    if(restart)
    {
        I2CRepeatStart(EEPROM_I2C_BUS);
    }
    else
    {
        // Wait for the bus to be idle, then start the transfer
        while( !I2CBusIsIdle(EEPROM_I2C_BUS) );

        if(I2CStart(EEPROM_I2C_BUS) != I2C_SUCCESS)
        {
            debug("Error: Bus collision during transfer Start\n");
            return FALSE;
        }
    }

    // Wait for the signal to complete
    do
    {
        status = I2CGetStatus(EEPROM_I2C_BUS);

    } while ( !(status & I2C_START) );

    return TRUE;
}


/*******************************************************************************
  Function:
    BOOL TransmitOneByte( UINT8 data )

  Summary:
    This transmits one byte to the EEPROM.

  Description:
    This transmits one byte to the EEPROM, and reports errors for any bus
    collisions.

  Precondition:
    The transfer must have been previously started.

  Parameters:
    data    - Data byte to transmit

  Returns:
    TRUE    - Data was sent successfully
    FALSE   - A bus collision occured

  Example:
    <code>
    TransmitOneByte(0xAA);
    </code>

  Remarks:
    This is a blocking routine that waits for the transmission to complete.
  *****************************************************************************/

BOOL TransmitOneByte( UINT8 data )
{
    // Wait for the transmitter to be ready
    while(!I2CTransmitterIsReady(EEPROM_I2C_BUS));

    // Transmit the byte
    if(I2CSendByte(EEPROM_I2C_BUS, data) == I2C_MASTER_BUS_COLLISION)
    {
        debug("Error: I2C Master Bus Collision\n");
        return FALSE;
    }

    // Wait for the transmission to finish
    while(!I2CTransmissionHasCompleted(EEPROM_I2C_BUS));

    return TRUE;
}


/*******************************************************************************
  Function:
    void StopTransfer( void )

  Summary:
    Stops a transfer to/from the EEPROM.

  Description:
    This routine Stops a transfer to/from the EEPROM, waiting (in a 
    blocking loop) until the Stop condition has completed.

  Precondition:
    The I2C module must have been initialized & a transfer started.

  Parameters:
    None.
    
  Returns:
    None.
    
  Example:
    <code>
    StopTransfer();
    </code>

  Remarks:
    This is a blocking routine that waits for the Stop signal to complete.
  *****************************************************************************/

void StopTransfer( void )
{
    I2C_STATUS  status;

    // Send the Stop signal
    I2CStop(EEPROM_I2C_BUS);

    // Wait for the signal to complete
    do
    {
        status = I2CGetStatus(EEPROM_I2C_BUS);

    } while ( !(status & I2C_STOP) );
}
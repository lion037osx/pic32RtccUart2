/* 
 * File:   ds1307.h
 * Author: leo
 *
 * Created on July 2, 2017, 1:19 AM
 */

#ifndef DS1307_H
#define	DS1307_H

#ifdef	__cplusplus
extern "C" {
#endif

void I2C_Master_Init(const unsigned long c);
void I2C_Master_Start(void);
void I2C_Master_Stop(void);
void I2C_Master_Write(unsigned d);
unsigned short I2C_Master_Read(unsigned short a);

BOOL StartTransfer( BOOL restart );
BOOL TransmitOneByte( UINT8 data );
void StopTransfer( void );


#ifdef	__cplusplus
}
#endif

#endif	/* DS1307_H */


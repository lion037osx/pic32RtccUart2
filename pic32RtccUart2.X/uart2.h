/* 
 * File:   uart2.h
 * Author: leo
 *
 * Created on July 2, 2017, 1:41 AM
 */
#include "HardwareProfile.h"

#ifndef UART2_H
#define UART2_H

#ifdef	__cplusplus
extern "C" {
#endif

void initU2( void);
void UART2Puts( char *str );


#ifdef	__cplusplus
}
#endif

#endif	/* UART2_H */


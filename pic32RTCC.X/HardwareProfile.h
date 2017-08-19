/* 
 * File:   HardwareProfile.h
 * Author: leo
 *
 * Created on July 3, 2017, 10:56 PM
 */

#ifndef HARDWAREPROFILE_H
#define	HARDWAREPROFILE_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "uart2.h"
#include "ds1307.h"

#define set_tris_sda_scl() {TRISFbits.TRISF5=0;TRISFbits.TRISF6=0;}    
    
#define TX_TRIS		TRISFbits.TRISF5
#define RX_TRIS 		TRISFbits.TRISF4

#define SYS_FREQ 			(80000000L)
#define getClock() (80000000ul)    
#define Fsck	50000
#define BRG_VAL 	((SYS_FREQ/2/Fsck)-2)
    
#ifdef	__cplusplus
}
#endif

#endif	/* HARDWAREPROFILE_H */


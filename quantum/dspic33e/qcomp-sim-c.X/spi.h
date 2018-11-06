/*
 * File: spi.h
 * 
 * Description: SPI communication functions
 *
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "p33EP512MU810.h"
#include "xc.h"

// Set up serial peripheral interface
int setup_spi(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */


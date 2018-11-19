/**
 * @file spi.h
 * 
 * @brief Description: SPI communication functions
  * @authors J Scott, O Thomas
 * @date Nov 2018
*
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "xc.h"

/// Set up serial peripheral interface
int setup_spi(void);

/// Send a byte to the SPI1 peripheral
/// @param data byte to be sent to SPI1
int send_byte_spi_1(int data);

/// Recieve a byte from the SPI3 peripheral
int read_byte_spi_3();

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */


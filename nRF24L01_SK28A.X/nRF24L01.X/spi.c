/*******************************************************************************
* This file provides the functions for the SPI module.
*
* Author: Kong Wai Weng @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/



#include <htc.h>



/*******************************************************************************
* PUBLIC FUNCTION: vInitSpi
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the SPI module.
*
*******************************************************************************/
void vInitSpi(void)
{
	// Input data sampled at middle of data output time.
	SSPSTATbits.SMP = 0;
	
	// Select SPI mode 0, 0.
	SSPSTATbits.CKE = 1;	// Transmit occurs on transition from active to Idle clock state.
	SSPCONbits.CKP = 0;		// Idle state for clock is a low level.
	
	// SPI Master mode, clock = FOSC/64.
	SSPCONbits.SSPM3 = 0;
	SSPCONbits.SSPM2 = 0;
	SSPCONbits.SSPM1 = 1;
	SSPCONbits.SSPM0 = 0;
	
	// Clear the Write Collision Detect bit.
	SSPCONbits.WCOL = 0;
	
	// Enables serial port and configures SCK, SDO, SDI and SS as the source of the serial port pins.
	SSPCONbits.SSPEN = 1;
}



/*******************************************************************************
* PUBLIC FUNCTION: ucTransferSpi
*
* PARAMETERS:
* ~ ucData		- The data to send.
*
* RETURN:
* ~ The data received from the SPI.
*
* DESCRIPTIONS:
* Send and receive the data via SPI.
*
*******************************************************************************/
unsigned char ucTransferSpi(unsigned char ucData)
{
	// Clear the SPI buffer.
	unsigned char TempVar = SSPBUF;
	
	// Clear the interrupt flag.
	PIR1bits.SSPIF = 0;
  
	// Send the data
	SSPBUF = ucData;
	
	// Wait for the SPI module to finish transfering data.
	while(SSPSTATbits.BF == 0);
	
	// Return the received data.
	return SSPBUF;
}

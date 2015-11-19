#ifndef _NRF24L01_H
#define _NRF24L01_H

/*******************************************************************************
* PUBLIC FUNCTION: vInitNrf24l01
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the nRF24L01 wireless transceiver.
*
*******************************************************************************/
void vInitNrf24l01(void);


/*******************************************************************************
* PUBLIC FUNCTION: vNrf24l01EnableTx
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Change to Tx Mode.
*
*******************************************************************************/
void vNrf24l01EnableTx(void);


/*******************************************************************************
* PUBLIC FUNCTION: vNrf24l01EnableRx
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Change to Rx Mode.
*
*******************************************************************************/
void vNrf24l01EnableRx(void);



/*******************************************************************************
* PUBLIC FUNCTION: ucNrf24l01TransmitPacket
*
* PARAMETERS:
* ~ pucBuffer	- Pointer to the data that we want to transmit.
* ~ ucLength	- Number of bytes we want to transmit.
*
* RETURN:
* ~ The status register of nRF24L01.
*
* DESCRIPTIONS:
* Transmit a packet via the nRF24L01.
*
*******************************************************************************/
unsigned char ucNrf24l01TransmitPacket(const unsigned char* pucBuffer, unsigned char ucLength);


/*******************************************************************************
* PUBLIC FUNCTION: ucNrf24l01ReceivePacket
*
* PARAMETERS:
* ~ pucBuffer	- Pointer to the buffer to store the received data.
* ~ ucLength	- Number of bytes we want to read.
*
* RETURN:
* ~ Return 0 if Rx FIFO is empty.
* ~ Return 1 if data was read successfully.
*
* DESCRIPTIONS:
* Read the received packet.
*
*******************************************************************************/
unsigned char ucNrf24l01ReceivePacket(unsigned char* pucBuffer, unsigned char ucLength);


/*******************************************************************************
* PUBLIC FUNCTION: vReadNrf24l01
*
* PARAMETERS:
* ~ ucCommand	- Command.
* ~ ucAddress	- Register address we wish to read from.
* ~ pucBuffer	- Buffer to store the data.
* ~ ucLength	- Number of bytes to read.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Read from nRF24L01.
*
*******************************************************************************/
void vReadNrf24l01(unsigned char ucCommand, unsigned char ucAddress,
				   unsigned char* pucBuffer, unsigned char ucLength);


/*******************************************************************************
* PUBLIC FUNCTION: vWriteNrf24l01
*
* PARAMETERS:
* ~ ucCommand	- Command.
* ~ ucAddress	- Register address we wish to write.
* ~ pucBuffer	- Buffer for the data.
* ~ ucLength	- Number of bytes to write.
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Write to nRF24L01.
*
*******************************************************************************/
void vWriteNrf24l01(unsigned char ucCommand, unsigned char ucAddress,
				    const unsigned char* pucBuffer, unsigned char ucLength);


/*******************************************************************************
* PUBLIC FUNCTION: ucReadNrf24l01Status
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ Value of Status register.
*
* DESCRIPTIONS:
* Read from the Status register of nRF24L01.
*
*******************************************************************************/
unsigned char ucReadNrf24l01Status(void);


/*******************************************************************************
* PUBLIC FUNCTION: ucReadNrf24l01FifoStatus
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ Value of FIFO Status register.
*
* DESCRIPTIONS:
* Read from the FIFO Status register of nRF24L01.
*
*******************************************************************************/
unsigned char ucReadNrf24l01FifoStatus(void);


#endif

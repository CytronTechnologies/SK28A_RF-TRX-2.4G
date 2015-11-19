#include <htc.h>
#include "system.h"
#include "spi.h"
#include "nrf24l01.h"



/*******************************************************************************
* PRIVATE CONSTANT                                                             *
*******************************************************************************/

// Command.
#define R_REGISTER			0b00000000
#define W_REGISTER			0b00100000
#define R_RX_PAYLOAD		0b01100001
#define W_TX_PAYLOAD		0b10100000
#define FLUSH_TX			0b11100001
#define FLUSH_RX			0b11100010
#define REUSE_TX_PL			0b11100011
#define ACTIVATE			0b01010000
#define R_RX_PL_WID			0b01100000
#define W_ACK_PAYLOAD		0b10101000
#define W_TX_PAYLOAD_NOACK	0b10110000
#define CMD_NOP				0b11111111



// Address.
#define ADD_CONFIG		0x00
#define ADD_EN_AA		0x01
#define ADD_EN_RXADDR	0x02
#define ADD_SETUP_AW	0x03
#define ADD_SETUP_RETR	0x04
#define ADD_RF_CH		0x05
#define ADD_RF_SETUP	0x06
#define ADD_STATUS		0x07
#define ADD_OBSERVE_TX	0x08
#define ADD_CD			0x09
#define ADD_RX_ADDR_P0	0x0a
#define ADD_RX_ADDR_P1	0x0b
#define ADD_RX_ADDR_P2	0x0c
#define ADD_RX_ADDR_P3	0x0d
#define ADD_RX_ADDR_P4	0x0e
#define ADD_RX_ADDR_P5	0x0f
#define ADD_TX_ADDR		0x10
#define ADD_RX_PW_P0	0x11
#define ADD_RX_PW_P1	0x12
#define ADD_RX_PW_P2	0x13
#define ADD_RX_PW_P3	0x14
#define ADD_RX_PW_P4	0x15
#define ADD_RX_PW_P5	0x16
#define ADD_FIFO_STATUS	0x17
#define ADD_DYNPD		0x1c
#define ADD_FEATURE		0x1d



// RF channel.
#define RF_CHANNEL		50

// Number of bytes to send and receive.
#define DATA_LENGTH		1



/*******************************************************************************
* PRIVATE FUNCTION PROTOTYPES                                                  *
*******************************************************************************/



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
void vInitNrf24l01(void)
{
	unsigned char aucBuffer[5];
	
	__delay_ms(100);
	
	// Flush Tx and Rx FIFO.
	vWriteNrf24l01(FLUSH_TX, 0, 0, 0);
	vWriteNrf24l01(FLUSH_RX, 0, 0, 0);
	
	
	// Setting Tx address.
	// Rx must have the same address too.
	aucBuffer[0] = 0xaa;
	aucBuffer[1] = 0xbb;
	aucBuffer[2] = 0xcc;
	aucBuffer[3] = 0xdd;
	aucBuffer[4] = 0xee;
	vWriteNrf24l01(W_REGISTER, ADD_TX_ADDR, &aucBuffer, 5);
	
	// Setting same address for Rx P0.
	vWriteNrf24l01(W_REGISTER, ADD_RX_ADDR_P0, &aucBuffer, 5);
	
	// Enable Auto Ack on Pipe 0.
	aucBuffer[0] = 0x01;
	vWriteNrf24l01(W_REGISTER, ADD_EN_AA, &aucBuffer, 1);
	
	// Enable Pipe 0.
	aucBuffer[0] = 0x01;
	vWriteNrf24l01(W_REGISTER, ADD_EN_RXADDR, &aucBuffer, 1);
	
	// Rx Payload.
	aucBuffer[0] = DATA_LENGTH;
	vWriteNrf24l01(W_REGISTER, ADD_RX_PW_P0, &aucBuffer, 1);
	
	// Auto Retransmit Delay = 500uS.
	// Auto Retransmit Count = 15.
	aucBuffer[0] = 0b00011111;
	vWriteNrf24l01(W_REGISTER, ADD_SETUP_RETR, &aucBuffer, 1);
	
	// Select RF channel.
	aucBuffer[0] = RF_CHANNEL;
	vWriteNrf24l01(W_REGISTER, ADD_RF_CH, &aucBuffer, 1);
	
	// Select Air Data Rate.
	// Tx Power = 0dB.
//	aucBuffer[0] = 0b00100110;		// 250kbps.
	aucBuffer[0] = 0b00000110;		// 1Mbps.
	vWriteNrf24l01(W_REGISTER, ADD_RF_SETUP, &aucBuffer, 1);
	
	// Enable RX_DR, TX_DS and MAX_RT interrupt.
	// Enable CRC.
	// CRC encoding scheme = 2 bytes.
	// Power up.
	// Prim = PRX.
	aucBuffer[0] = 0b00001111;
	vWriteNrf24l01(W_REGISTER, ADD_CONFIG, &aucBuffer, 1);
	
	// Enable Rx.
	CE = 1;
	
	// Delay a while.
	__delay_ms(2);
}



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
void vNrf24l01EnableTx(void)
{
	unsigned char ucBuffer;
	
	// Disable Rx.
	CE = 0;
	
	// Read the current config.
	vReadNrf24l01(R_REGISTER, ADD_CONFIG, &ucBuffer, 1);
	
	// Change to Tx mode.
	ucBuffer &= 0xfe;
	
	// Write the new config.
	vWriteNrf24l01(W_REGISTER, ADD_CONFIG, &ucBuffer, 1);
}



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
void vNrf24l01EnableRx(void)
{
	unsigned char ucBuffer;
	
	// Disable Tx.
	CE = 0;
	
	// Read the current config.
	vReadNrf24l01(R_REGISTER, ADD_CONFIG, &ucBuffer, 1);
	
	// Change to Rx mode.
	ucBuffer |= 0x01;
	
	// Write the new config.
	vWriteNrf24l01(W_REGISTER, ADD_CONFIG, &ucBuffer, 1);
	
	// Enable Rx.
	CE = 1;
}



/*******************************************************************************
* PUBLIC FUNCTION: ucNrf24l01TransmitPacket
*
* PARAMETERS:
* ~ pucBuffer	- Pointer to the data that we want to transmit.
* ~ ucLength	- Number of bytes we want to transmit.
*
* RETURN:
* ~ Return 0 if transmission failed and maximum retry reached.
* ~ Return 1 if data was transmitted successfully.
*
* DESCRIPTIONS:
* Transmit a packet via the nRF24L01.
*
*******************************************************************************/
unsigned char ucNrf24l01TransmitPacket(const unsigned char* pucBuffer, unsigned char ucLength)
{
	// Enable Tx.
	CE = 1;

	// Write Tx payload.
	vWriteNrf24l01(W_TX_PAYLOAD, 0, pucBuffer, ucLength);
	
	// Disable Tx.
	CE = 0;
	
	// Wait for interrupt.
	while (IRQ == 1);
	
	// Read the status.
	unsigned char ucStatus = ucReadNrf24l01Status();
	
	// Clear the interrupt flag.
	vWriteNrf24l01(W_REGISTER, ADD_STATUS, &ucStatus, 1);
		
	// Flush Tx FIFO.
	vWriteNrf24l01(FLUSH_TX, 0, 0, 0);
	
	
	// Check whether the transmission succeeded.
	if (((ucStatus & 0x20) != 0) && ((ucStatus & 0x10) == 0)) {
		return 1;
	}
	else {
		return 0;
	}	
}	



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
unsigned char ucNrf24l01ReceivePacket(unsigned char* pucBuffer, unsigned char ucLength)
{
	unsigned char ucStatus;
	unsigned char ucFifoStatus;
	unsigned char ucReturnCode;
	
	
	
	// Read the FIFO status.
	ucFifoStatus = ucReadNrf24l01FifoStatus();
	
	// Make sure there is data in the Rx FIFO.
	if ((ucFifoStatus & 0x01) == 0) {
		vReadNrf24l01(R_RX_PAYLOAD, 0, pucBuffer, ucLength);
		ucReturnCode = 1;
	}	
	else {
		ucReturnCode = 0;
	}	
	
	// Clear the interrupt flag.
	ucStatus = ucReadNrf24l01Status();
	vWriteNrf24l01(W_REGISTER, ADD_STATUS, &ucStatus, 1);
	
	return ucReturnCode;
}	



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
				   unsigned char* pucBuffer, unsigned char ucLength)
{
	// Get the command.
	ucCommand += ucAddress & 0b00011111;
	
	// Communicate with the nRF24L01.
	CSN = 0;
	ucTransferSpi(ucCommand);
	while (ucLength-- > 0) {
		*pucBuffer++ = ucTransferSpi(0xff);
	}
	CSN = 1;
}	



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
				    const unsigned char* pucBuffer, unsigned char ucLength)
{
	// Get the command.
	ucCommand += ucAddress & 0b00011111;
	
	// Communicate with the nRF24L01.
	CSN = 0;
	ucTransferSpi(ucCommand);
	while (ucLength-- > 0) {
		ucTransferSpi(*pucBuffer++);
	}
	CSN = 1;
}	



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
unsigned char ucReadNrf24l01Status(void)
{
	unsigned char ucStatus;
	
	// Communicate with the nRF24L01.
	CSN = 0;
	ucStatus = ucTransferSpi(CMD_NOP);
	CSN = 1;
	
	return ucStatus;
}	



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
unsigned char ucReadNrf24l01FifoStatus(void)
{
	unsigned char ucFifoStatus;
	
	vReadNrf24l01(R_REGISTER, ADD_FIFO_STATUS, &ucFifoStatus, 1);
	return ucFifoStatus;
}	

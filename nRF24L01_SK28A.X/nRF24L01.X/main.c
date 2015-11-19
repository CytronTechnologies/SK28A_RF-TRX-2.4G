#include <htc.h>
#include "system.h"
#include "spi.h"
#include "nrf24l01.h"
#include "lcd.h"
#include "keypad.h"

/*******************************************************************************
* Prototype function                                                           *
*******************************************************************************/
void initialzie(void);
/*******************************************************************************
* DEVICE CONFIGURATION WORDS                                                   *
*******************************************************************************/

__CONFIG(FOSC_HS &		// High Speed Crystal.
		 WDTE_OFF &		// Disable Watchdog Timer.
		 PWRTE_ON &		// Enable Power Up Timer.
		 BOREN_OFF &	// Disable Brown Out Reset.
		 LVP_OFF);		// Disable Low Voltage Programming.

/*******************************************************************************
* MAIN FUNCTION                                                                *
*******************************************************************************/
int main(void)
{
	unsigned char ucStatus;
	unsigned char ucData;
	unsigned char key_send;
        unsigned char key;
	
        initialzie();

	while (1)
        {
		// If data is received...
		if (IRQ == 0)
                {
			ucStatus = ucNrf24l01ReceivePacket(&ucData, 1);

			// Make sure data is read out successfully.
			if (ucStatus != 0)
                        {
                                LCD_goto(0, 0);
                                LCD_putstr("Receive ");
                                LCD_goto(1, 0);
				LCD_putchar(ucData);
				// Turn on/off LED1 according to the data received.
				if (ucData == '1')
                                {
					LED1 = 1;
				}
				else if (ucData == '2')
                                {
					LED1 = 0;
				}
                        }
                }

            while((key = keypad_read()) != 0xFF)
            {
 			// Enable Tx mode.
                        key_send = keypad_wait(); //ASCII_value(keypad_wait());
                        LED1 = ~LED1;
                        __delay_ms(30);
                          LCD_goto(0, 0);
                          LCD_putstr("Transmit");
                          LCD_goto(1,0);
                        LCD_putstr("Mode");
                        __delay_ms(40);
			vNrf24l01EnableTx();

			ucStatus = ucNrf24l01TransmitPacket(&key_send, 1);

			// Change back to Rx mode.
                        vNrf24l01EnableRx();
            }
        }
}

void initialzie(void)
{
    	// Set all pins on Port A as digital IO.
	ADCON1 = 0x06;

	// Configure the IO direction.
        TRISA0 = 0;
        TRISA1 = 0;
        TRISA2 = 0;
        TRISA3 = 0;
        TRISA4 = 1;
        TRISA5 = 1;
	TRISB = 0b00000001;
	TRISC = 0b11010100;

	// Initialize the IO.
	PORTA = 0;
	PORTB = 0;
	PORTC = 0;

	LED1 = 0;

	CE = 0;
	CSN = 1;

	// Initialize SPI.
	vInitSpi();
	// Initialize nRF24L01.
	vInitNrf24l01();
        // Initialize LCD.
        LCD_initialize();
}
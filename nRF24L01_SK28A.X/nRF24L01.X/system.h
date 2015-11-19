#ifndef _SYSTEM_H
#define _SYSTEM_H

// Oscillator Frequency.
#define	_XTAL_FREQ		20000000

/*
 * IO Connections
*/
//LED
#define LED1			PORTBbits.RB1

//Switch
#define SW1			PORTBbits.RB0

//nRF24L01
#define CE			PORTCbits.RC0
#define CSN			PORTCbits.RC1
#define IRQ			PORTCbits.RC2
#define SCK			PORTCbits.RC3
#define MISO			PORTCbits.RC4
#define MOSI			PORTCbits.RC5

//LCD
#define lcd_RS                  RB2
#define lcd_en                  RB3
#define lcd_P4                  RB4
#define lcd_P5                  RB5
#define lcd_P6                  RB6
#define lcd_P7                  RB7

//4x4 Keypad
#define KP_R1			RA3		// Row 1
#define KP_R2			RA2		// Row 2
#define KP_R3			RA1		// Row 3
#define KP_R4			RA0		// Row 4

#define KP_C1			RC6		// Colomn 1
#define KP_C2			RC7		// Colomn 2
#define KP_C3			RA5		// Colomn 3
#define KP_C4			RA4		// Colomn 4

#endif

#include <htc.h>
#include "lcd.h"
#include "system.h"

/**************************************************
* PUBLIC FUNCTION: LCD_command                    *
*                                                 *
* PARAMETERS:                                     *
* ~ var                                           *
*                                                 *
* RETURN:                                         *
* ~ void                                          *
*                                                 *
* DESCRIPTIONS:                                   *
* Routine to send command to LCD                  *
/**************************************************/
void LCD_command (unsigned char var)
{
    lcd_RS = 0;                 //Selected command register
    LCD_4bitsend(var);          //send command via 2 nibbles
}

/**************************************************
* PUBLIC FUNCTION: LCD_4bitsend                   *
*                                                 *
* PARAMETERS:                                     *
* ~ data                                          *
*                                                 *
* RETURN:                                         *
* ~ void                                          *
*                                                 *
* DESCRIPTIONS:                                   *
* 4bit send command                               *
/**************************************************/
void LCD_4bitsend(unsigned char data)
{
    lcd_P7 = 1 & (data >> 7);
    lcd_P6 = 1 & (data >> 6);
    lcd_P5 = 1 & (data >> 5);
    lcd_P4 = 1 & (data >> 4);
    lcd_en = 1;                 //Enable H->L
	__delay_ms(2);
    lcd_en = 0;


    lcd_P7 = 1 & (data >> 3);
    lcd_P6 = 1 & (data >> 2);
    lcd_P5 = 1 & (data >> 1);
    lcd_P4 = 1 & data;
    lcd_en = 1;//               //Enable H->L
	__delay_ms(2);
    lcd_en = 0;
    //__delay_us(400);            //wait for ~400us
}

/**************************************************
* PUBLIC FUNCTION: LCD_goto                       *
*                                                 *
* PARAMETERS:                                     *
* ~ void                                          *
*                                                 *
* RETURN:                                         *
* ~ void                                          *
*                                                 *
* DESCRIPTIONS:                                   *
* Initiaize LCD                   *
/**************************************************/
void LCD_initialize()
{
    __delay_ms(30);
	//set lcd to congfiguration mode
	lcd_en = 1;
	__delay_ms(20);
	lcd_en = 0;
    lcd_RS = 0;         //selected command register
    lcd_P7 = 0;
    lcd_P6 = 0;
    lcd_P5 = 1;
    lcd_P4 = 1;
	lcd_en = 1;
	__delay_ms(2);
    lcd_en = 0;
              //wait for ~5ms

    lcd_P7 = 0;
    lcd_P6 = 0;
    lcd_P5 = 1;
    lcd_P4 = 1;
    lcd_en = 1;                 //Enable H->L
	__delay_ms(2);
    lcd_en = 0;
    __delay_ms(2);         //wait for ~100us

    lcd_P7 = 0;
    lcd_P6 = 0;
    lcd_P5 = 1;
    lcd_P4 = 1;
    lcd_en = 1;                 //Enable H->L
	__delay_ms(2);
    lcd_en = 0;
    __delay_ms(2);          //wait for ~100us

    lcd_P7 = 0;
    lcd_P6 = 0;
    lcd_P5 = 1;
    lcd_P4 = 0;
    lcd_en = 1;                 //Enable H->L
	__delay_ms(2);
    lcd_en = 0;
    __delay_ms(2);            //wait for ~100us

    //LCD configuration
    LCD_command (0x28);         //Function set: 4 bit mode, 2 lines, 5x7 font
    LCD_command (0x0C);         //Display on, no cursor & no blinking
    LCD_command (0x01);         //Clear LCD
    LCD_command (0x06);         //Entry mode, auto increment with no shift
}

/**************************************************
* PUBLIC FUNCTION: LCD_senddata                   *
*                                                 *
* PARAMETERS:                                     *
* ~ var                                           *
*                                                 *
* RETURN:                                         *
* ~ void                                          *
*                                                 *
* DESCRIPTIONS:                                   *
*                                                 *
/**************************************************/
void LCD_senddata(unsigned char var)
{
    lcd_RS = 1;
    LCD_4bitsend(var);
}

/**************************************************
* PUBLIC FUNCTION: LCD_putstr                     *
*                                                 *
* PARAMETERS:                                     *
* ~ * csz_string                                  *
*                                                 *
* RETURN:                                         *
* ~ void                                          *
*                                                 *
* DESCRIPTIONS:                                   *
* display words on LCD                            *
/**************************************************/
void LCD_putstr(char* csz_string)
{
	// Loop until the end of string.
	while (*csz_string != '\0')
        {
		LCD_senddata(*csz_string);
		csz_string++;      //// Point to next character.
	}
}

/**************************************************
* PUBLIC FUNCTION: putchar                        *
*                                                 *
* PARAMETERS:                                     *
* ~ c_data                                        *
*                                                 *
* RETURN:                                         *
* ~ void                                          *
*                                                 *
* DESCRIPTIONS:                                   *
* dispaly a character on LCD                      *
/**************************************************/
void LCD_putchar(char c_data)
{
	// Send the data to display.
	LCD_senddata(c_data);
}


/**************************************************
* PUBLIC FUNCTION: LCD_clear                      *
*                                                 *
* PARAMETERS:                                     *
* ~void                                           *
*                                                 *
* RETURN:                                         *
* ~ void                                          *
*                                                 *
* DESCRIPTIONS:                                   *
* clear LCD                                       *
/**************************************************/
void LCD_clear()
{
    LCD_command(0b00000001);
}

/**************************************************
* PUBLIC FUNCTION: LCD_goto                       *
*                                                 *
* PARAMETERS:                                     *
* ~ row                                           *
* ~ column                                        *
*                                                 *
* RETURN:                                         *
* ~ void                                          *
*                                                 *
* DESCRIPTIONS:                                   *
* cursor go to certain position                   *
/**************************************************/
void LCD_goto(unsigned char row,unsigned char column)
{
    if(row == 0)
        LCD_command(0x80 | column);
    else
        LCD_command(0x80 | 0x40 | column);
}


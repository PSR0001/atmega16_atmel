/*
 * LCD.c
 *
 * This code is used for 8bit data transfer
 *
 * Created: 10-09-2022 11:21:38
 * Author : PSR
 */ 
#define F_CPU 16000000UL   /*Define CPU frequency 16MHz*/
#include <avr/io.h>
#include  <util/delay.h>
#include <string.h>

/*defiens the macros LCD settings*/
#define LCD_Data_Dir DDRD
#define LCD_Command_Dir DDRB
#define LCD_Data_Port PORTD
#define LCD_Command_Port PORTB
#define RS PB0
#define RW PB1
#define EN PB2

#define _LCD_CLEAR 0x01					//Clear Display
#define _LCD_TYPE 0x38					//Initialization of 16X2 LCD in 8bit mode
#define _LCD_AUTO_INCREMENT 0X06		//Auto Increment cursor
#define _LCD_COURSER_OFF 0x0C			//Display ON Cursor OFF
#define _LCD_HOME 0x80                  // Cursor at home position 


//Function Prototype
void LCD_Command(unsigned char cmnd);
void LCD_Char (unsigned char char_data);
void LCD_Init (void);
void LCD_String (char *str);
void LCD_Write(char row, char pos, char *str);
void LCD_Clear();

int main(void)
{

	LCD_Init();			/* Initialize LCD */

	LCD_String("PSR");	/* write string on 1st line of LCD*/
	LCD_Clear();
	LCD_String("Hello World");	/* Write string on 2nd line*/
	_delay_ms(500);
	LCD_Command(0xC0); //next line
	
	
	int k=0;
	while(k<3){
		//LCD_String((char*)k);
		LCD_String("A");
		k++;
		_delay_ms(1000);
	}
}

// Function Defination
void LCD_Command(unsigned char cmnd)
{
	LCD_Data_Port= cmnd;
	LCD_Command_Port &= ~(1<<RS);	/* RS=0 command reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 Write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(3);
}

void LCD_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}
void LCD_Clear() /*LCD clear Function*/
{
	LCD_Command (_LCD_CLEAR);	
	LCD_Command (_LCD_HOME);	
}

void LCD_Write(char row, char pos, char *str)/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		
}
void LCD_Init (void)			/* LCD Initialize function */
{
	LCD_Command_Dir = 0xFF;		/* Make LCD command port direction as o/p */
	LCD_Data_Dir = 0xFF;		/* Make LCD data port direction as o/p */
	_delay_ms(20);			//LCD Power ON delay always >15ms 
	
	LCD_Command (_LCD_TYPE);	
	LCD_Command (_LCD_COURSER_OFF);		
	LCD_Command (_LCD_AUTO_INCREMENT);		
	LCD_Command (_LCD_CLEAR);		
	LCD_Command (_LCD_HOME);		
}

void LCD_Char (unsigned char char_data)	/* LCD data write function */
{
	LCD_Data_Port= char_data;
	LCD_Command_Port |= (1<<RS);	/* RS=1 Data reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable Pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);   
	_delay_ms(1);
}
#include<LPC214X.h>
#include <stdio.h>
#define RS	((1<<18));   // P0.10(0000 0000 0000 0000 0000 0x00 0000 0000)				
#define EN	((1<<16));    // P0.13(0000 0000 0000 0000 00x0 0000 0000 0000) 

//Define LCD PinIO Mask 
#define  LCD_RS     0x00040000   			// P1.18(0000 0000 0000 0x00 0000 0000 0000 0000)
#define  LCD_EN     0x00010000   			// P1.16(0000 0000 0000 000x 0000 0000 0000 0000)

#define  LCD_GND    0x00080000				// P0.19(0000 0000 0000 X000 0000 0000 0000 0000)
#define  LCD_D4     0x00200000   			// P1.21(0000 0000 00x0 0000 0000 0000 0000 0000)
#define  LCD_D5     0x00400000  			// P1.22(0000 0000 0x00 0000 0000 0000 0000 0000)
#define  LCD_D6     0x00800000  			// P1.23(0000 0000 x000 0000 0000 0000 0000 0000)
#define  LCD_D7     0x01000000				// P1.24(0000 000x 0000 0000 0000 0000 0000 0000)   	

												  //(8421 8421 8421 8421 8421 8421 8421 8421)   

#define  LCD_DATA   (LCD_D7|LCD_D6|LCD_D5|LCD_D4)
#define  LCD_IOALL  (LCD_D7|LCD_D6|LCD_D5|LCD_D4|LCD_RS|LCD_EN)

#define  lcd_rs_set() IOSET1 = LCD_RS	 				// RS = 1 (Select Instruction)
#define  lcd_rs_clr() IOCLR1 = LCD_RS					// RS = 0 (Select Data)
#define  lcd_en_set() IOSET1 = LCD_EN					// EN = 1 (Enable)
#define  lcd_en_clr() IOCLR1 = LCD_EN					// EN = 0 (Disable)

// (0000 000x xxx0 0x0x 0000 0000 0000 0000)
#define  lcd_dir_write()  IODIR1 |= 0x01E50000			// LCD Data Bus = Write
//#define  lcd_dir_read()   IODIR1 &= 0xFFC3FFFF			// LCD Data Bus = Read 

#define  lcd_clear()          lcd_write_control(0x01)	// Clear Display
#define  lcd_cursor_home()    lcd_write_control(0x02)	// Set Cursor = 0
#define  lcd_display_on()     lcd_write_control(0x0E)	// LCD Display Enable
#define  lcd_display_off()    lcd_write_control(0x08)	// LCD Display Disable
#define  lcd_cursor_blink()   lcd_write_control(0x0F)	// Set Cursor = Blink
#define  lcd_cursor_on()      lcd_write_control(0x0E)	// Enable LCD Cursor
#define  lcd_cursor_off()     lcd_write_control(0x0C)	// Disable LCD Cursor
#define  lcd_cursor_left()    lcd_write_control(0x10)	// Shift Left Cursor
#define  lcd_cursor_right()   lcd_write_control(0x14)	// Shift Right Cursor
#define  lcd_display_sleft()  lcd_write_control(0x18)	// Shift Left Display
#define  lcd_display_sright() lcd_write_control(0x1C)	// Shift Right Display

void lcd_init(void);										// Initial LCD
void lcd_out_data4(unsigned char);						// Strobe 4-Bit Data to LCD
void lcd_write_byte(unsigned char);						// Write 1 Byte Data to LCD
void lcd_write_control(unsigned char); 					// Write Instruction
void lcd_write_ascii(unsigned char); 					// Write LCD Display(ASCII)
void goto_cursor(unsigned char);						// Set Position Cursor LCD
void lcd_print(unsigned char*);							// Print Display to LCD
//char busy_lcd(void);									// Read Busy LCD Status
void enable_lcd(void);	 								// Enable Pulse
void delay(unsigned long int);							// Delay Function
void LCD_Valuedisplay(unsigned int value_display);
void LCD_Valuedisplay_2(unsigned int value_display);
void LCD_Valuedisplay_5(unsigned int value_display);

				// Strobe 4-Bit Data to LCD 
void lcd_out_data4(unsigned char val)
				{			IOCLR1 = (LCD_DATA);	  		// Reset 4-Bit Pin Data
								IOSET1 = (val<<21); 		// EN,0,RW,RS:DDDD:0000:0000:0000:0000:0000:0000   	// doubt
				}

				// Write Data 1 Byte to LCD 
void lcd_write_byte(unsigned char val)
			{  
      
					lcd_out_data4((val>>4)&0x0F);									// Strobe 4-Bit High-Nibble to LCD
					enable_lcd();																									// Enable Pulse 
					lcd_out_data4(val&0x0F);				  								// Strobe 4-Bit Low-Nibble to LCD
					enable_lcd();																									// Enable Pulse  
					delay(100);
					// while(busy_lcd());      												// Wait LCD Execute Complete  
				}

/* Write Instruction to LCD */
void lcd_write_control(unsigned char val)
				{ 
					lcd_rs_clr();											// RS = 0 = Instruction Select
					lcd_write_byte(val);									// Strobe Command Byte	    
				}
				
/* Write Data(ASCII) to LCD */
void lcd_write_ascii(unsigned char c)
			{  
				lcd_rs_set();											// RS = 1 = Data Select
				lcd_write_byte(c);		   							// Strobe 1 Byte to LCD    
			}

///* Initial 4-Bit LCD Interface 
void lcd_init()
			{
				unsigned int i;															// LCD Initial Delay Count 
	/*Warning:Please dont touch the line*/
	/*Use in all LPC code*/
	PINSEL2 = 0xF7;								// GPIO1[31..26] = I/O Function
				lcd_dir_write();														// LCD Data Bus = Write

				for (i=0;i<100;i++);										// Power-On Delay (15 mS)

				IOCLR1 = (LCD_IOALL);									// Reset (RS,RW,EN,4-Bit Data) Pin
				IOSET1 = (LCD_D5|LCD_D4);					// DDDD:EN,RW,RS,0:0000:0000:0000:0000:0000:0000 
				enable_lcd();																	// Enable Pulse
				for (i=0;i<100;i++);										// Delay 4.1mS

				IOCLR1 = (LCD_IOALL);	  						// Reset (RS,RW,EN,4-Bit Data) Pin
				IOSET1 = (LCD_D5|LCD_D4);					// DDDD:EN,RW,RS,0:0000:0000:0000:0000:0000:0000 
				enable_lcd();																	// Enable Pulse
				for (i=0;i<100;i++);										// delay 100uS

				IOCLR1 = (LCD_IOALL);	  						// Reset (RS,RW,EN,4-Bit Data) Pin
				IOSET1 = (LCD_D5|LCD_D4);					// DDDD:EN,RW,RS,0:0000:0000:0000:0000:0000:0000 
				enable_lcd();																	// Enable Pulse
				delay(100);
  //while(busy_lcd());      				// Wait LCD Execute Complete
 
				IOCLR1 = (LCD_IOALL);	  						// Reset (RS,RW,EN,4-Bit Data) Pin
				IOSET1 = (LCD_D5);												// DDDD:EN,RW,RS,0:0000:0000:0000:0000:0000:0000 
				enable_lcd();																	// Enable Pulse
				delay(100);
		// while(busy_lcd());      				// Wait LCD Execute Complete

				lcd_write_control(0x28);  				// Function Set (DL=0 4-Bit,N=1 2 Line,F=0 5X7)
				lcd_write_control(0x0C);  				// Display on/off Control (Entry Display,Cursor off,Cursor not Blink)
				lcd_write_control(0x06);  				// Entry Mode Set (I/D=1 Increment,S=0 Cursor Shift)
				lcd_write_control(0x01);  				// Clear Display  (Clear Display,Set DD RAM Address=0) 
				for (i=0;i<100000;i++);							// Wait Command Ready
}

// Set LCD Position Cursor 
void goto_cursor(unsigned char i)
		{	
				i |= 0x80;											// Set DD-RAM Address Command
				lcd_write_control(i);
		}

// Print Display Data(ASCII) to LCD */
void lcd_print(unsigned char* str)
		{
				int i;
				for (i=0;i<16 && str[i]!=0;i++)  						// 16 Character Print
			{
				lcd_write_ascii(str[i]);							// Print Byte to LCD
			}
	}
		
		
// Display For Digits	
void LCD_Valuedisplay(unsigned int value_display)
{
	//	lcd_write_ascii(((value_display%10000)/1000)+0x30);
		lcd_write_ascii(((value_display%10000)/1000)+0x30);
		lcd_write_ascii(((value_display%1000)/100)+0x30);
		lcd_write_ascii(((value_display%100)/10)+0x30);
		lcd_write_ascii((value_display%10)+0x30);
}
void LCD_Valuedisplay_5(unsigned int value_display)
{
		lcd_write_ascii(((value_display%100000)/10000)+0x30);
		lcd_write_ascii(((value_display%10000)/1000)+0x30);
		lcd_write_ascii(((value_display%1000)/100)+0x30);
		lcd_write_ascii(((value_display%100)/10)+0x30);
		lcd_write_ascii((value_display%10)+0x30);
}

void LCD_Valuedisplay_2(unsigned int value_display)
{
	lcd_write_ascii(((value_display%100)/10)+0x30);
	lcd_write_ascii((value_display%10)+0x30);
}

/* Enable Pulse to LCD */
void enable_lcd(void)	 				// Enable Pulse
		{
				unsigned int i;										// Delay Count
				lcd_en_set();  										// Enable ON
				for (i=0;i<4000;i++);
				lcd_en_clr();  										// Enable OFF 
		}

/*    1-4294967296     */
void delay(unsigned long int count1)
		{
				while(count1 > 0) {count1--;}							// Loop Decrease Counter	
		}
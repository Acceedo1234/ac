extern void OLED_Init(void);
extern void OLED_Clear();
extern void OLED_SetCursor(unsigned char lineNumber,unsigned char cursorPosition);
extern void OLED_DisplayString(unsigned char *ptr);
extern void OLED_DisplayChar(unsigned char ch);
extern void  OLED_GoToNextLine();
extern void OLED_HorizontalGraph(unsigned char barGraphNumber, unsigned char percentageValue);
extern void OLED_VerticalGraph(unsigned char barGraphNumber, unsigned char percentageValue);
extern void OLED_ScrollMessage(unsigned char lineNum, unsigned char *strptr);
extern void OLED_DisplayNumber(unsigned char v_numericSystem_u8, unsigned int v_number_u32, unsigned char v_numOfDigitsToDisplay_u8);

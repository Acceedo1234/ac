#include<LPC214X.h>

void Init_PLL();

void Init_PLL()
{
    //System Clock
    //PLLCFG = 0x00100100;              //p=2 (PSEL=01) and m=5-1 (MSEL=00100)
//    PLL0CFG = 0x00;     //0x0010 0010   //48Mhz m = 4-1 = 3 i.e 00010
    PLL0CFG = 0x23;                   //60Mhz m=5-1=4 i.e 00011
    PLL0CON = 0x01;
    PLL0FEED = 0xaa;
    PLL0FEED = 0x55;
    while((PLL0STAT & 0x400) == 0);
    PLL0CON = 0x03;
    PLL0FEED = 0xaa;
    PLL0FEED = 0x55;

    //USB clock
    //PLL1CFG = 0x00100011;            //p=2 (PSEL=01) and m=4-1 (MSEL=00011)
    PLL1CFG = 0x23;
    PLL1CON = 0x01;     
    PLL1FEED = 0xaa;
    PLL1FEED = 0x55;    
    while((PLL1STAT & 0x400) == 0);    
    PLL1CON = 0x03;                    
    PLL1FEED = 0xaa;
    PLL1FEED = 0x55;            
}
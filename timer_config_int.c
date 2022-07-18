#include<LPC214X.h>

void Init_TIMER0(void);
void Init_TIMER1(void);
void Init_EXTINT0(void);


void Init_TIMER0(void)
{
    T0IR  = 0xff;         //clr the pending flags of interrupt
    //T0CTCR = 0x00;      //timer mode	   //5
    T0PR  = 250-1;        //prescalar maximum count value keep it 1000 and change T0MR0
    T0PC  = 0;        
    T0MCR = 0x0003;       //MR0I=interrupt when TC=MR and reset
    T0MR0 = 250-1;        //match the final value (using )
    //T0TC  = 0;
    T0TCR = 0x02;         //reset the timer
    T0TCR = 0x01;         //enable the timer
}

void Init_TIMER1(void)
{
    T1IR  = 0xff;         //clr the pending flags of interrupt
    //T1CTCR = 0x00;      //timer mode
    T1PR  = 1095-1;         //prescalar maximum count value keep it 1000 and change T0MR1
    T1PC  = 0;        
    T1MCR = 0x0003;       //MR0I=interrupt when TC=MR and reset
    T1MR0 = 1095-1;         //match the final value (using )
    //T1TC  = 0;
    T1TCR = 0x02;         //reset the timer
    T1TCR = 0x01;         //enable the timer
}

void Init_EXTINT0(void)
{
    EXTINT    = 0x01;     //clr interrupt bit
    INTWAKE   = 0x0000;
    EXTMODE   = 0x01;     //EINT0 edge sensitive
    EXTPOLAR  = 0x01;     //EINT0 rising edge
}
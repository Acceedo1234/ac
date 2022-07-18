#include<LPC214X.h>


/*Function Initialization*/
void IO_Init(void);


/*Function Definition*/
void IO_Init(void)
{
/*
	//INPUT
	IO0DIR &= ~((1<<17)) ; // explicitly making P0.17 as Input 1 
	IO0DIR &= ~((1<<18)) ; // explicitly making P0.18 as Input 2
	IO1DIR &= ~((1<<16)) ; // explicitly making P1.16 as Input 3
	//IO1DIR &= ~((1<<17)) ; // explicitly making P1.17 as Input 4
	IO0DIR &= ~((1<<11)) ; // explicitly making P0.11 as Input 5
	IO0DIR &= ~((1<<12)) ; // explicitly making P0.12 as Input 6
	IO1DIR &= ~((1<<22)) ; // explicitly making P1.22 as Input 7
	IO0DIR &= ~((1<<15)) ; // explicitly making P0.15 as Input 8
	IO1DIR &= ~((1<<18)) ; // explicitly making P1.18 as Input 9
	
	IO1DIR &= ~((1<<25)) ; // explicitly making P1.25 as Input
	IO1DIR &= ~((1<<20)) ; // explicitly making P1.20 as Input
	IO1DIR &= ~((1<<21)) ; // explicitly making P1.21 as Input
	IO1DIR &= ~((1<<23)) ; // explicitly making P1.23 as Input
	
	IO1DIR |= (1<<24); // Config P1.24 as Ouput
	
	//RELAY
	IO0DIR |= (1<<25); // Config P1.24 as Ouput
	IO0DIR |= (1<<28); // Config P1.24 as Ouput
	IO0DIR |= (1<<29); // Config P1.24 as Ouput
	IO0DIR |= (1<<30); // Config P1.24 as Ouput
	
	IO0SET |= !(1<<25); // Make ouput High for P1.24
	IO0SET |= !(1<<28); // Make ouput High for P1.24
	IO0SET |= !(1<<29); // Make ouput High for P1.24
	IO0SET |= !(1<<30); // Make ouput High for P1.24
	
	//IO0CLR |= (1<<25); // Output for P0.15 becomes Low
	//IO0CLR |= (1<<28); // Output for P0.15 becomes Low			
	//IO0CLR |= (1<<29); // Output for P0.15 becomes Low
	//IO0CLR |= (1<<30); // Output for P0.15 becomes Low
	IO1DIR &= ((1<<17)) ;
//	IO0DIR &= ((1<<28)) ; 
*/
    IO0DIR |= (1<<17); // Config P0.25 as Ouput
 	IO0DIR |= (1<<25); // Config P0.25 as Ouput
	IO0DIR |= (1<<28); // Config P0.28 as Ouput
	IO0DIR |= (1<<29); // Config P0.29 as Ouput
	IO1DIR |= (1<<17); // Config P1.17 as Ouput

	IO1DIR |= (1<<21); // Config P1.21 as Ouput
	IO1DIR |= (1<<22); // Config P1.22 as Ouput
	IO1DIR |= (1<<23); // Config P1.23 as Ouput
	
	IO1DIR |= (1<<18); // Config P1.18 as Ouput	  
	IO1DIR |= (1<<16); // Config P1.16 as Ouput	
	 
	IO0DIR |= (1<<19); // Config P0.19 as Ouput	  
	IO0DIR |= (1<<18); // Config P0.18 as Ouput
	IO1DIR |= (1<<24); // Config P1.24 as Ouput
	IO1DIR |= (1<<25); // Config P1.25 as Ouput
	PINSEL0 = 0;
	IO0DIR |= (1<<2); // Config P0.2 as Ouput     //SCL
	IO0DIR |= (1<<3); // Config P0.3 as Ouput     //SDA
	//IO0DIR |= (1<<15); // Config P0.15 as Ouput 
	IO0DIR &= ~((1<<15)) ; 
 	//INPUT
	IO0DIR &= ~((1<<10)) ; // explicitly making P0.17 as Input 1 //IO0PIN
	IO0DIR &= ~((1<<11)) ; // explicitly making P0.18 as Input 2
	IO0DIR &= ~((1<<12)) ; // explicitly making P1.16 as Input 3
	IO0DIR &= ~((1<<13)) ; // explicitly making P1.17 as Input 4
	




}

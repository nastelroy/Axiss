

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

//#include "gpio.c"

#define FASA1 BIT(21)
#define STEP BIT(11) //input step -->interrupt
#define DIR BIT(12)  //input direction

#define INPUT1 BIT(0) // port 1.0
#define INPUT2 BIT(1) // port 1.1
#define INPUT3 BIT(4) // port 1.4
#define INPUT4 BIT(8) // port 1.8 
#define INPUT5 BIT(9) // port 1.9

// gunakan dulu sebagai input biasa, seharusnya sebagai PWM
#define INH1 BIT(18) //port 1.18
#define INH2 BIT(20) //port 1.20
#define INH3 BIT(21) // port 1.21
#define INH4 BIT(23) // port 1.23
#define INH5 BIT(24) // port 1.24



void gpio_ISR_Wrapper( void ) __attribute__ ((naked));
void gpio_ISR_Handler( void );


void gpio_ISR_Wrapper( void )
{
	
	/* Save the context of the interrupted task. */
	portSAVE_CONTEXT();
	
	gpio_ISR_Handler();
	
	
	portRESTORE_CONTEXT();
}


extern void gpio_ISR_Handler( void )
{
	static portBASE_TYPE xHigherPriorityTaskWoken;
	
	
	int hitung = 0; 
	//hitung++;
	/*
	if (IO2_INT_STAT_F & DIR )
	{
		if (hitung ==1)
		{
			FIO1PIN = INPUT1 | INPUT2 ;
			FIO1PIN = INH1 | INH2 | INH4;
		}
		else if (hitung ==2)
		{
			FIO1PIN = INPUT1;
			FIO1PIN = INH1 | INH3 | INH4 ;
		}
		else if (hitung ==3)
		{
			FIO1PIN = INPUT1 | INPUT5 ;
			FIO1PIN = INH1 |INH3|INH5;
		}
		else if (hitung ==4)
		{
			FIO1PIN = INPUT5;
			FIO1PIN = INH2 | INH3 | INH5 ;
		}
		else if (hitung ==5)
		{
			FIO1PIN = INPUT4 | INPUT5 ;
			FIO1PIN = INH2 | INH4 | INH5;
		}
		else if(hitung ==6)
		{
			FIO1PIN = INPUT4 ;
			FIO1PIN = INH1 | INH2 | INH4;
		}
		else if (hitung ==7)
		{
			FIO1PIN = INPUT3 | INPUT4;
			FIO1PIN = INH1 | INH3 | INH4 ;
		}
		else if (hitung ==8)
		{
			FIO1PIN = INPUT3 ;
			FIO1PIN = INH1 | INH3 | INH5;
		}
		else if (hitung ==9)
		{
			FIO1PIN = INPUT2 | INPUT3;
			FIO1PIN = INH2 | INH3 | INH5 ;
		}
		else if (hitung ==10)
		{
			FIO1PIN = INPUT2;
			FIO1PIN = INH2 | INH4 | INH5;
			hitung = 0;
		}
		IO2_INT_CLR = DIR;
	}
	
	

	else 
	{
		FIO1SET = FASA1;
	
	}
	
	IO2_INT_CLR = DIR;
	VICVectAddr = 0; // Acknowledge Interrupt

	*/
}

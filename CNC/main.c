/*
	FreeRTOS.org V5.1.1 - Copyright (C) 2003-2008 Richard Barry.
	
	28 Juli 2009
	blink + serial + FreeRTOS demo
*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "gpio.c"

#define BAUD_RATE	( ( unsigned portLONG ) 115200 )
#define LED_UTAMA	BIT(26)
#define INH3 BIT(21)

#define STEP BIT(11) //input step -->interrupt

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

static int tog;

/*-----------------------------------------------------------*/
#define jalankan

int main( void )
{
	sysInit();
	
	PINMODE2 = 0xFFFFFFFF;
	PINMODE3 = 0xFFFFFFFF;
	PINMODE4 = 0xFFFFFFFF;
	
	FIO0DIR = LED_UTAMA;
	FIO0CLR = LED_UTAMA;	

	xSerialPortInitMinimal( BAUD_RATE, configMINIMAL_STACK_SIZE  );

#ifdef jalankan
	init_led_utama();
	init_task_serial();
	init_task_cnc();
	init_irq();
	
	vTaskStartScheduler();

    /* Will only get here if there was insufficient memory to create the idle
    task. */
	return 0;
#endif
}


void togle_led_utama(void)
{
	if (tog)
	{
		FIO0SET = LED_UTAMA;
		tog = 0;	
	}
	else
	{
		FIO0CLR = LED_UTAMA;
		tog = 1;		
	}
}

static portTASK_FUNCTION(task_led, pvParameters )
{
	tog = 0;
	
	for (;;)
	{
		togle_led_utama();
		vTaskDelay(500);
	}
}
void init_led_utama(void)
{
	xTaskCreate(task_led, ( signed portCHAR * ) "Led2",  (configMINIMAL_STACK_SIZE * 2) ,\
		 NULL, tskIDLE_PRIORITY - 2, ( xTaskHandle * ) NULL );
}


static portTASK_FUNCTION(task_serial, pvParameters )
{
	int c;
	vTaskDelay(1000);
	vSerialPutString(0, "Testing FreeRTOS serial\r\n");
	
	while(1)
    {
	  	if (xSerialGetChar(1, &c, 100 ) == pdTRUE)
	  	{
	  		vSerialPutString(0, "Tombol ditekan = ");
	  		xSerialPutChar(	0, (char ) c);
	  		vSerialPutString(0, " \r\n");
	  	}
	}
}

void init_task_serial(void)
{
	xTaskCreate(task_serial, ( signed portCHAR * ) "SERR",  (configMINIMAL_STACK_SIZE * 4) ,\
		 NULL, tskIDLE_PRIORITY, ( xTaskHandle * ) NULL );
}



static portTASK_FUNCTION(task_cnc, pvParameters )
{
	cnc();
	
	for (;;)
	{
		
		
		vTaskDelay(100);
	}
	
}

void init_task_cnc(void)
{
	xTaskCreate(task_cnc, ( signed portCHAR * ) "cnc_task",  (configMINIMAL_STACK_SIZE * 2) ,\
		 NULL, tskIDLE_PRIORITY - 2, ( xTaskHandle * ) NULL );
}





void cnc(void)
{
	int a=0;
	FIO1DIR |=  INPUT1 | INPUT2 | INPUT3 | INPUT4 | INPUT5;
	FIO1DIR |= INH1 | INH2 | INH3 | INH4 | INH5;
	
	
	while(1)
	{
		
		vTaskDelay(100);
		
	}
	
}



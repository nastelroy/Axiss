/*	Fana Akbarkan
 * 
 * 
 * 
 * Jakarta, 18 Maret 2011
 * Microstep , tapi masih sampe interrupt
 * 
 * 
 * 
 * 
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "gpio.c"

#define BAUD_RATE	( ( unsigned portLONG ) 115200 )
#define LED_UTAMA	BIT(26)

#define FASA1 BIT(21)
#define FASA2 BIT(11)
#define SWPIN 11

static int tog;

/*-----------------------------------------------------------*/




void dele(int dd)
{
	int g;
	int i;
	int dum;

	for (i=0; i<dd; i++)
	{
		dum = FIO0DIR;
	}
}





int main( void )
{
	sysInit();
	
	
	FIO0DIR = LED_UTAMA;
	FIO0CLR = LED_UTAMA;	

	xSerialPortInitMinimal( BAUD_RATE, configMINIMAL_STACK_SIZE  );

	init_led_utama();
	init_task_serial();
	init_task_cnc();
	
	
	vTaskStartScheduler();
	
    /* Will only get here if there was insufficient memory to create the idle
    task. */
	return 0;
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
		vTaskDelay(100);
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
	  		vSerialPutString(0, "dipencet = ");
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
		
		
		vTaskDelay(1);
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
	FIO1DIR = FASA1;

	FIO1SET = FASA1;
	//printf("blink \n\r");
	dele(1000000);
	
	FIO1CLR = FASA1;
	//printf("blink \n\r");
	dele(1000000);
	
	init_irq();
	
	while(1)
	{
		FIO1PIN ^= FASA1;
		dele(1000000);
		vTaskDelay(10);
	}
	
}


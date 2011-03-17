/*
	FreeRTOS.org V5.1.1 - Copyright (C) 2003-2008 Richard Barry.
	
	28 Juli 2009
	blink + FreeRTOS demo
*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define LED_UTAMA	BIT(26)

static int tog;

/*-----------------------------------------------------------*/
#define jalankan

int main( void )
{
	sysInit();

	FIO0DIR = LED_UTAMA;
	FIO0CLR = LED_UTAMA;


#ifdef jalankan
	init_led_utama();
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

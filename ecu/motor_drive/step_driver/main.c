/*
	17 Maret 2010
	
	Furkan Jadid
	Daun Biru Engineering
	
	Step Driver untuk CNC Fana
	
	Fungsi untuk generate PULSE dan Direction berdasarkan G-Code
	yang dibaca dari input (serial ?)
	
*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define BAUD_RATE	( ( unsigned portLONG ) 115200 )
#define LED_UTAMA	BIT(27)

static int tog;

void init_led_utama(void);
void init_task_serial(void);

/*-----------------------------------------------------------*/
#define jalankan

int main( void )
{
	sysInit();

	FIO0DIR = LED_UTAMA;
	FIO0CLR = LED_UTAMA;	

	xSerialPortInitMinimal( BAUD_RATE, configMINIMAL_STACK_SIZE  );

#ifdef jalankan
	init_led_utama();
	init_task_serial();
	init_task_step();
	
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
		vTaskDelay(5000);
	}
}
void init_led_utama(void)
{
	xTaskCreate(task_led, ( signed portCHAR * ) "Led2",  (configMINIMAL_STACK_SIZE * 2) ,\
		 NULL, tskIDLE_PRIORITY - 2, ( xTaskHandle * ) NULL );
}

void proses_c(char * c);

static portTASK_FUNCTION(task_serial, pvParameters )
{
	int c;
	vTaskDelay(1000);
	vSerialPutString(0, "Testing FreeRTOS serial\r\n");
	
	init_steppers();
	
	while(1)
    {
	  	if (xSerialGetChar(1, &c, 0xFFFF ) == pdTRUE)
	  	{
	  		proses_c( &c );
			//vSerialPutString(0, "Tombol ditekan = ");
	  		//xSerialPutChar(	0, (char ) c);
	  		//vSerialPutString(0, " \r\n");
	  	}
	}
}

void init_task_serial(void)
{
	xTaskCreate(task_serial, ( signed portCHAR * ) "SERR",  (configMINIMAL_STACK_SIZE * 10) ,\
		 NULL, tskIDLE_PRIORITY, ( xTaskHandle * ) NULL );
}

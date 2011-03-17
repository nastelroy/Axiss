/*

	PULSE dan DIRECTION
	
	furkan jadid
	daun biru engineering
	
	17 Maret 2010
	
	*/

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"	

typedef int byte;

#define wait(c)		vTaskDelay(c)

#define PULSE_X		BIT(24)		// P1
#define DIR_X		BIT(25)
#define PORT_X

#define PULSE_Y		BIT(26)
#define DIR_Y		BIT(27)
#define PORT_Y

#define PULSE_Z		BIT(28)
#define DIR_Z		BIT(29)
#define PORT_Z

#define DIR_HC245	BIT(23)

#define PULSE_X_HI()	FIO1SET = PULSE_X
#define PULSE_X_LO()	FIO1CLR = PULSE_X

#define PULSE_Y_HI()	FIO1SET = PULSE_Y
#define PULSE_Y_LO()	FIO1CLR = PULSE_Y

#define PULSE_Z_HI()	FIO1SET = PULSE_Z
#define PULSE_Z_LO()	FIO1CLR = PULSE_Z

xSemaphoreHandle step_sem;
//extern struct t_step *pstep;

extern FloatPoint current_steps;
extern FloatPoint target_steps;
extern FloatPoint delta_steps;

extern FloatPoint current_units;
extern FloatPoint target_units;
extern FloatPoint delta_units;

extern byte x_direction;
extern byte y_direction;
extern byte z_direction;

extern long max_delta;

void init_port_X_Y_Z(void)
{
	FIO1DIR |= PULSE_X | DIR_X;
	FIO1DIR |= PULSE_Y | DIR_Y;
	FIO1DIR |= PULSE_Z | DIR_Z;	
	
	FIO1DIR |= DIR_HC245;
	
	FIO1SET = DIR_HC245;
}

void set_port_X_direction(int d)
{
	if (d)
		FIO1SET = DIR_X;
	else
		FIO1CLR = DIR_X;
}

void set_port_Y_direction(int d)
{
	if (d)
		FIO1SET = DIR_Y;
	else
		FIO1CLR = DIR_Y;
}

void set_port_Z_direction(int d)
{
	if (d)
		FIO1SET = DIR_Z;
	else
		FIO1CLR = DIR_Z;
}

static portTASK_FUNCTION(task_stepper, pvParameters )
{
	int c;
	int i;
	int counter_x;
	int counter_y;
	int counter_z;
	
	
	init_port_X_Y_Z();
	
	vSemaphoreCreateBinary( step_sem );
    xSemaphoreTake( step_sem, 0 );
	
	vTaskDelay(1000);
	
	while(1)
    {
		if ( xSemaphoreTake( step_sem, 10000 ) == pdTRUE )
		{
			printf("%s(): CUR %f %f %f\r\n", __FUNCTION__, current_steps.x, \
				current_steps.y, current_steps.z);
			
			printf("%s(): TGT %f %f %f\r\n", __FUNCTION__, target_steps.x, \
				target_steps.y, target_steps.z);
			
			printf("%s(): DLT %f %f %f\r\n", __FUNCTION__, delta_steps.x, \
				delta_steps.y, delta_steps.z);
				
			printf("%s(): DIR %d, %d, %d\r\n", __FUNCTION__, x_direction, \
				y_direction, z_direction);
			
			printf("%s(): Max Delta %d\r\n", __FUNCTION__, max_delta);
			
			set_port_X_direction( x_direction );
			set_port_Y_direction( y_direction );
			set_port_Z_direction( z_direction );
			
			counter_x = 0;
			counter_y = 0;
			counter_z = 0;
			
			for (i=0; i<max_delta; i++)
			{			
				if ( delta_steps.x > counter_x )
					PULSE_X_HI();
				if ( delta_steps.y > counter_y )
					PULSE_Y_HI();
				if ( delta_steps.z > counter_z )
					PULSE_Z_HI();
					
				wait(1);
				
				if ( delta_steps.x > counter_x )
					PULSE_X_LO();
				if ( delta_steps.y > counter_y )
					PULSE_Y_LO();
				if ( delta_steps.z > counter_z )
					PULSE_Z_LO();
					
				wait(1);
				
				counter_x++;
				counter_y++;
				counter_z++;
			}
			
			current_units.x = target_units.x;
			current_units.y = target_units.y;
			current_units.z = target_units.z;
			calculate_deltas();
			printf("RUN OK\r\n");
		}
	}
}

void init_task_step(void)
{
	xTaskCreate(task_stepper, ( signed portCHAR * ) "STEPP",  (configMINIMAL_STACK_SIZE * 10) ,\
		 NULL, tskIDLE_PRIORITY - 1, ( xTaskHandle * ) NULL );
}

/*
	proses 
	G-code
	
	furkan jadid
	17 Maret 2010

*/
	
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void process_string(char instruction[], int size);

static char kar[128];
static int jum;

void ser_out(char *c)
{
	while ( *c )
	{
		xSerialPutChar(0, *c++);
	}
}

void proses_c(char *c)
{
	xSerialPutChar(0, *c);
	
	if (*c == 0x0D)  /* jika enter dari gtkterm */
	{		
		process_string( kar, jum );
		
		memset( kar, 0, sizeof (kar));
		jum = 0;
	}
	else if ( *c == 0x0A )
	{
		//xSerialPutChar(0, 'A');
		process_string( kar, jum );
		
		memset( kar, 0, sizeof (kar));
		jum = 0;
	}
	else
	{
		kar[ jum++ ] = (char *) *c;
		
		
		
		if (jum > 128) 
		{
			ser_out("Penuh !\r\n");
			jum = 0;
		}
	}
}

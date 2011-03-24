
#include "FreeRTOS.h"



#define STEP BIT(11)
#define DIR BIT(12)  //input direction

void init_irq(void)
{
extern void ( gpio_ISR_Wrapper)( void );



	portENTER_CRITICAL();
	
	//siapkan interrupt handler untuk GPIO
	VICIntSelect    &= ~VIC_CHAN_TO_MASK(VIC_CHAN_NUM_EINT3);
	VICIntEnClr      = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_EINT3);
	VICVectAddr17 = ( portLONG )gpio_ISR_Wrapper;
	VICVectPriority17 = 0x05;
	VICIntEnable = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_EINT3);

	// setup GPIO direction & interrupt
	FIO2DIR = FIO2DIR & ~(STEP | DIR) ;
	
	// enable rising edge interrupt
	IO2_INT_EN_R = STEP | DIR ; 

	printf("telah init irq\n\r");
	portEXIT_CRITICAL();
	
}


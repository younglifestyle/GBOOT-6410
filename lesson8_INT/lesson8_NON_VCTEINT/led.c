#ifdef MMU_ON
	#define GPKCON0 (*(volatile unsigned long *)0xA0008800)
	#define GPKDAT  (*(volatile unsigned long *)0xA0008808)
#else
	#define GPKCON0 (*(volatile unsigned long *)0x7F008800)
	#define GPKDAT  (*(volatile unsigned long *)0x7F008808)
#endif

void led_init()
{
	GPKCON0 = 0x11110000;
}

void led_on()
{	
	GPKDAT = 0x0;
}

void led_off()
{
	GPKDAT = 0xF0;
}
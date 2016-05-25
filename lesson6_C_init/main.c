#define GPKCON0 (*(volatile unsigned long *)0x7F008800)
#define GPKDAT  (*(volatile unsigned long *)0x7F008808)


int gboot_main()
{
//	GPKCON0 = 0x11110000;
//	GPKDAT = 0x0;
	light_led();
	
	return 0;
} 
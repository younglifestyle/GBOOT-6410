#define GPKCON0 0x7F008800
#define GPKDAT  0x7F008808
//#define GPKCON0 (*(volatile unsigned long *)0x7F008800)
//#define GPKDAT  (*(volatile unsigned long *)0x7F008808)

int gboot_main()
{
//	GPKCON0 = 0x11110000;
//	GPKDAT = 0x0;
//	light_led();
	
	__asm__ (
		"ldr r1, =0x11110000\n"
		"str r1, [%0]\n"
		"mov r1, #0x0\n"
		"str r1, [%1]\n"
		:
		:"r"(GPKCON0), "r"(GPKDAT)
		:"r1"
	);
	
	
	return 0;
} 
#define GPNCON (*(volatile unsigned long *)0x7F008830)
#define GPNDAT (*(volatile unsigned long *)0x7F008834)

#define GPN0_Clear (3 << 0)
#define GPN1_Clear (3 << 2)
#define GPN2_Clear (3 << 4)
#define GPN3_Clear (3 << 6)

#define GPN0_INT (2 << 0)
#define GPN1_INT (2 << 2)
#define GPN2_INT (2 << 4)
#define GPN3_INT (2 << 6)

void key_init()
{
	GPNCON &= ~(GPN0_Clear | GPN1_Clear | GPN2_Clear | GPN3_Clear);
	GPNCON |= GPN0_INT | GPN1_INT | GPN2_INT | GPN3_INT;
}
//#define GPKCON0 (*(volatile unsigned long *)0x7F008800)
//#define GPKDAT  (*(volatile unsigned long *)0x7F008808)

#define GPKCON0 (*(volatile unsigned long *)0xAF008800)
#define GPKDAT  (*(volatile unsigned long *)0xAF008808)

#define SECTION      (3 << 0)
#define BUFFERABLE   (1 << 2)
#define CACHEABLE    (1 << 3)
#define ONE          (1 << 4)
#define DOMAIN       (0 << 5)
#define AP           (3 << 10)
#define MMU_DESCR    (SECTION | ONE | DOMAIN | AP)
#define MMU_DESCR_WB (SECTION | ONE | DOMAIN | AP | BUFFERABLE | CACHEABLE)

void creat_page()
{
	unsigned long *ttb = (unsigned long *)0x50000000;
	unsigned long vaddr = 0xA0000000;
	unsigned long paddr = 0x7F000000;
	
	*(ttb + (vaddr >> 20)) = (paddr & 0xFFF00000) | MMU_DESCR;
	
	vaddr = 0x50000000;
	paddr = 0x50000000;
	while(vaddr <= 0x54000000)
	{
		*(ttb + (vaddr >> 20)) = (paddr & 0xFFF00000) | MMU_DESCR_WB;
		vaddr += 0x100000;
		paddr += 0x100000;
	}
}

void mmu_init()
{
   __asm__(
    
    /*设置TTB*/
    "ldr    r0, =0x50000000\n"                  
    "mcr    p15, 0, r0, c2, c0, 0\n"    
    
    /*不进行权限检查*/
    "mvn    r0, #0\n"                   
    "mcr    p15, 0, r0, c3, c0, 0\n"    
     
   /*使能MMU*/
    "mrc    p15, 0, r0, c1, c0, 0\n"    
    "orr    r0, r0, #0x0001\n"          
    "mcr    p15, 0, r0, c1, c0, 0\n"
    : 
    : 
  );
}

int gboot_main()
{
	creat_page();
	mmu_init();
	
	GPKCON0 = 0x11110000;
	GPKDAT = 0x0;
	
	return 0;
} 
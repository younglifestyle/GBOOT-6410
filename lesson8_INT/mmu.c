#define SECTION      (2 << 0)       /* 选择段转换方式 */
#define BUFFERABLE   (1 << 2)       /* 使用Write Buffer */
#define CACHEABLE    (1 << 3)       /* 使用Cache */ 
#define ONE          (1 << 4)       /* 必须是“1” */
#define DOMAIN       (0 << 5)       /* 选择16个域中的一个 */
#define AP           (3 << 10)      /* 设置域的控制权限 */
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
	while(vaddr < 0x54000000)
	{
		*(ttb + (vaddr >> 20)) = (paddr & 0xFFF00000) | MMU_DESCR_WB;
		vaddr += 0x100000;
		paddr += 0x100000;
	}
}

void mmu_init()
{
   unsigned long ttb = 0x50000000;
 
	__asm__(
		"mov    r0, #0\n"
		"mcr    p15, 0, r0, c7, c7, 0\n"    /* 使无效ICaches和DCaches */
		
		"mcr    p15, 0, r0, c7, c10, 4\n"   /* drain write buffer on v4 */
		"mcr    p15, 0, r0, c8, c7, 0\n"    /* 使无效指令、数据TLB */
		
		"mov    r4, %0\n"                   /* r4 = 页表基址 */
		"mcr    p15, 0, r4, c2, c0, 0\n"    /* 设置页表基址寄存器 */
		
		"mvn    r0, #0\n"                   
		"mcr    p15, 0, r0, c3, c0, 0\n"    /* 域访问控制寄存器设为0xFFFFFFFF，
		                                     * 不进行权限检查 
		                                     */    
		/* 
		 * 对于控制寄存器，先读出其值，在这基础上修改感兴趣的位，
		 * 然后再写入
		 */
		"mrc    p15, 0, r0, c1, c0, 0\n"    /* 读出控制寄存器的值 */
		
		/* 控制寄存器的低16位含义为：.RVI ..RS B... .CAM
		 * R : 表示换出Cache中的条目时使用的算法，
		 *     0 = Random replacement；1 = Round robin replacement
		 * V : 表示异常向量表所在的位置，
		 *     0 = Low addresses = 0x00000000；1 = High addresses = 0xFFFF0000
		 * I : 0 = 关闭ICaches；1 = 开启ICaches
		 * R、S : 用来与页表中的描述符一起确定内存的访问权限
		 * B : 0 = CPU为小字节序；1 = CPU为大字节序
		 * C : 0 = 关闭DCaches；1 = 开启DCaches
		 * A : 0 = 数据访问时不进行地址对齐检查；1 = 数据访问时进行地址对齐检查
		 * M : 0 = 关闭MMU；1 = 开启MMU
		 */
		
		/*  
		 * 先清除不需要的位，往下若需要则重新设置它们    
		 */
		                                    /* .RVI ..RS B... .CAM */ 
		"bic    r0, r0, #0x3000\n"          /* ..11 .... .... .... 清除V、I位 */
		"bic    r0, r0, #0x0300\n"          /* .... ..11 .... .... 清除R、S位 */
		"bic    r0, r0, #0x0087\n"          /* .... .... 1... .111 清除B/C/A/M */
		
		/*
		 * 设置需要的位
		 */
		"orr    r0, r0, #0x0002\n"          /* .... .... .... ..1. 开启对齐检查 */
		"orr    r0, r0, #0x0004\n"          /* .... .... .... .1.. 开启DCaches */
		"orr    r0, r0, #0x1000\n"          /* ...1 .... .... .... 开启ICaches */
		"orr    r0, r0, #0x0001\n"          /* .... .... .... ...1 使能MMU */
		
		"mcr    p15, 0, r0, c1, c0, 0\n"    /* 将修改的值写入控制寄存器 */
		: /* 无输出 */
		: "r" (ttb) 
    );
}

void mmu_start()
{
	creat_page();
	mmu_init();
}
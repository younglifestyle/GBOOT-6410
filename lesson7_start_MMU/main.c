//#define GPKCON0 (*(volatile unsigned long *)0x7F008800)
//#define GPKDAT  (*(volatile unsigned long *)0x7F008808)

#define GPKCON0 (*(volatile unsigned long *)0xA0008800)
#define GPKDAT  (*(volatile unsigned long *)0xA0008808)

#define SECTION      (2 << 0)
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
		"mcr    p15, 0, r0, c7, c7, 0\n"    /* ʹ��ЧICaches��DCaches */
		
		"mcr    p15, 0, r0, c7, c10, 4\n"   /* drain write buffer on v4 */
		"mcr    p15, 0, r0, c8, c7, 0\n"    /* ʹ��Чָ�����TLB */
		
		"mov    r4, %0\n"                   /* r4 = ҳ���ַ */
		"mcr    p15, 0, r4, c2, c0, 0\n"    /* ����ҳ���ַ�Ĵ��� */
		
		"mvn    r0, #0\n"                   
		"mcr    p15, 0, r0, c3, c0, 0\n"    /* ����ʿ��ƼĴ�����Ϊ0xFFFFFFFF��
		                                     * ������Ȩ�޼�� 
		                                     */    
		/* 
		 * ���ڿ��ƼĴ������ȶ�����ֵ������������޸ĸ���Ȥ��λ��
		 * Ȼ����д��
		 */
		"mrc    p15, 0, r0, c1, c0, 0\n"    /* �������ƼĴ�����ֵ */
		
		/* ���ƼĴ����ĵ�16λ����Ϊ��.RVI ..RS B... .CAM
		 * R : ��ʾ����Cache�е���Ŀʱʹ�õ��㷨��
		 *     0 = Random replacement��1 = Round robin replacement
		 * V : ��ʾ�쳣���������ڵ�λ�ã�
		 *     0 = Low addresses = 0x00000000��1 = High addresses = 0xFFFF0000
		 * I : 0 = �ر�ICaches��1 = ����ICaches
		 * R��S : ������ҳ���е�������һ��ȷ���ڴ�ķ���Ȩ��
		 * B : 0 = CPUΪС�ֽ���1 = CPUΪ���ֽ���
		 * C : 0 = �ر�DCaches��1 = ����DCaches
		 * A : 0 = ���ݷ���ʱ�����е�ַ�����飻1 = ���ݷ���ʱ���е�ַ������
		 * M : 0 = �ر�MMU��1 = ����MMU
		 */
		
		/*  
		 * ���������Ҫ��λ����������Ҫ��������������    
		 */
		                                    /* .RVI ..RS B... .CAM */ 
		"bic    r0, r0, #0x3000\n"          /* ..11 .... .... .... ���V��Iλ */
		"bic    r0, r0, #0x0300\n"          /* .... ..11 .... .... ���R��Sλ */
		"bic    r0, r0, #0x0087\n"          /* .... .... 1... .111 ���B/C/A/M */
		
		/*
		 * ������Ҫ��λ
		 */
		"orr    r0, r0, #0x0002\n"          /* .... .... .... ..1. ���������� */
		"orr    r0, r0, #0x0004\n"          /* .... .... .... .1.. ����DCaches */
		"orr    r0, r0, #0x1000\n"          /* ...1 .... .... .... ����ICaches */
		"orr    r0, r0, #0x0001\n"          /* .... .... .... ...1 ʹ��MMU */
		
		"mcr    p15, 0, r0, c1, c0, 0\n"    /* ���޸ĵ�ֵд����ƼĴ��� */
		: /* ����� */
		: "r" (ttb) 
    );
}

int gboot_main()
{
	creat_page();
	mmu_init();
	
	//light_led();
	
	GPKCON0 = 0x11110000;
	GPKDAT = 0x0;
	
	return 0;
}
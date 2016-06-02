#define EINT0CON0				(*(volatile unsigned long *)0x7F008900)
#define EINT0MASK 			(*(volatile unsigned long *)0x7F008920)
#define VIC0INTENABLE 	(*(volatile unsigned long *)0x71200010)
#define VIC0VECTADDR 		(*(volatile unsigned long *)0x71200100)
#define EINT0PEND 			(*(volatile unsigned long *)0x7F008924)
#define VIC0ADDRESS 		(*(volatile unsigned long *)0x71200F00)

void key_handle()
{
		int val = EINT0PEND&((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));

		__asm__(
					/* ���滷�� */
					"sub lr, lr, #4\n"
					"stmfd sp!, {r0-r12, lr}\n"
					:
					:
					);
		
		EINT0PEND = ~0x0;
		
		/* ������ */
		switch(val)
		{
			case (1):
				led_on();
			break;
			
			case (1 << 1):
				led_off();
			break;
			
			case (1 << 2):
				led_on();
			break;
			
			case (1 << 3):
				led_off();
			break;
			
			default:
			break;
		}
		VIC0ADDRESS = 0;
		
		__asm__(
					/* �ָ����� */
					"ldmfd sp!, {r0-r12, pc}^\n"
					:
					:
					);
}

void init_irq()
{
	// �����жϴ�����ʽΪ�½���,���ö�Ӧ�������жϹ��ܺ�
	EINT0CON0 &= ~0x0f;
	EINT0CON0 |= ~0x33;
	
	// ȥ���ж�����Interrupt Mask
	EINT0MASK = 0;
	
	// ʹ��VIC0�ж���  ʹ�ܰ�����Ӧ�ж�External interrupt 0 ~ 3
	VIC0INTENABLE |= 0b1;
	
	// ���������жϴ�����
	VIC0VECTADDR = (int)key_handle;
	
	
	// �����жϿ�������CPSR�йرյ�I��Fλ
	__asm__(
					/* ��֧��6410�����ж� */
					"mrc p15,0,r0,c1,c0,0\n"
    			"orr r0,r0,#(1<<24)\n"
    			"mcr p15,0,r0,c1,c0,0\n"
					
					
					/* ��CPSR�С� I �� λ */
					"mrs r0, cpsr\n"
					"bic r0, r0, #0x80\n"
					"msr cpsr_c, r0\n"
					:
					:
	);
}
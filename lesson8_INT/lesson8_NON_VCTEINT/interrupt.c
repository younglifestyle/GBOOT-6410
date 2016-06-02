#define EINT0CON0				(*(volatile unsigned long *)0x7F008900)
#define EINT0MASK 			(*(volatile unsigned long *)0x7F008920)
#define VIC0INTENABLE 	(*(volatile unsigned long *)0x71200010)
#define VIC0VECTADDR 		(*(volatile unsigned long *)0x71200100)
#define EINT0PEND 			(*(volatile unsigned long *)0x7F008924)
#define VIC0ADDRESS 		(*(volatile unsigned long *)0x71200F00)
#define VIC0IRQSTATUS 	(*(volatile unsigned long *)0x71200000)
 
void handle_int()
{
		int val = EINT0PEND&((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));

		EINT0PEND = ~0x0;
//		VIC0IRQSTATUS = ~0x0;  //  这里不清楚是写1还是写0清除状态,上一步已经清除了中断		

		/* 处理函数 */
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
}

void init_irq()
{
	// 设置中断触发方式为下降沿,设置对应的引脚中断功能后
	EINT0CON0 &= ~0x0f;
	EINT0CON0 |= ~0x33;
	
	// 去掉中断掩码Interrupt Mask
	EINT0MASK = 0;
	
	// 使能VIC0中断中  使能按键对应中断External interrupt 0 ~ 3
	VIC0INTENABLE |= 0b1;	
	
	// 打开总中断控制器，CPSR中关闭的I、F位
	__asm__(
					/* 打开CPSR中“ I ” 位 */
					"mrs r0, cpsr\n"
					"bic r0, r0, #0x80\n"
					"msr cpsr_c, r0\n"
					:
					:
	);
}
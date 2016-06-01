#define GPACON 		(*(volatile unsigned long *)0x7F008000)
#define ULCON0 		(*(volatile unsigned long *)0x7F005000)
#define UCON0  		(*(volatile unsigned long *)0x7F005004)
#define UTRSTAT0  (*(volatile unsigned long *)0x7F005010)
#define UTXH0  		(*(volatile unsigned char *)0x7F005020)
#define URXH0  		(*(volatile unsigned char *)0x7F005024)
#define UBRDIV0  	(*(volatile unsigned short *)0x7F005028)
#define UDIVSLOT0 (*(volatile unsigned short *)0x7F00502C)
#define UFCON0 		(*(volatile unsigned long *)0x7F005008)
#define UMCON0 		(*(volatile unsigned long *)0x7F00500C)
#define UFSTAT0 	(*(volatile unsigned long *)0x7F005018)

void uart_init()
{
	/* 1、设置引脚功能 */
	GPACON &= 0;
	GPACON = 0x22222222;
	
	/* 2. 设置数据格式 */
	ULCON0 = 0x3;
	
	/* 3.a 设置工作模式 */
	UCON0 = 0x5;
	
	/* 3.b 使能FIFO */
	UFCON0 = 0x1;
	
	/* 3.c 去流控 */
	UMCON0 = 0;
	
	/* 4. 设置波特率 */
	UBRDIV0 = 35;      /* 例如DIV_VAL = (PCLK / (baud x 16 ) )-1  66500000/(115200*16)-1 = 35.07 */
	UDIVSLOT0 = 0x1;   //UDISLOT0=保存该公式计算后的小数部分*16
}


/* 发送一个字符 */
void putc(char c)
{
	while ( (UFSTAT0 & (1 << 14)) ); 	 // 发送FIFO为空，则等待
	UTXH0 = c;                           // 写数据 
}

/* 接收一个字符 */
char getc(void)
{
	char ret;
		
	while ( (UFSTAT0&0x7F) == 0 );       // 接收FIFO为满，
	ret = URXH0;                         // 取数据 
	
	if((ret == 0x0d) || (ret == 0x0a))
	{
		putc(0x0d);
		putc(0x0a);
	}
	else
		putc(ret);
	
	return ret;                 
}

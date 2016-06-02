#define GPACON 		(*(volatile unsigned long *)0x7F008000)
#define ULCON0 		(*(volatile unsigned long *)0x7F005000)
#define UCON0  		(*(volatile unsigned long *)0x7F005004)
#define UTRSTAT0  (*(volatile unsigned long *)0x7F005010)
#define UTXH0  		(*(volatile unsigned long *)0x7F005020)
#define URXH0  		(*(volatile unsigned long *)0x7F005024)
#define UBRDIV0  	(*(volatile unsigned long *)0x7F005028)
#define UDIVSLOT0 (*(volatile unsigned long *)0x7F00502C)
#define UFCON0 		(*(volatile unsigned long *)0x7F005008)
#define UMCON0 		(*(volatile unsigned long *)0x7F00500C)
#define UFSTAT0 	(*(volatile unsigned long *)0x7F005018)

void uart_init()
{
	/* 1���������Ź��� */
	GPACON &= 0;
	GPACON = 0x22222222;
	
	/* 2. �������ݸ�ʽ */
	ULCON0 = 0x3;
	
	/* 3.a ���ù���ģʽ */
	UCON0 = 0x5;
	
	/* 3.b ʹ��FIFO */
	UFCON0 = 0x1;
	
	/* 3.c ȥ���� */
	UMCON0 = 0;
	
	/* 4. ���ò����� */
	UBRDIV0 = 35;      /* ����DIV_VAL = (PCLK / (baud x 16 ) )-1  66500000/(115200*16)-1 = 35.07 */
	UDIVSLOT0 = 0x1;   //UDISLOT0=����ù�ʽ������С������*16
}


/* ����һ���ַ� */
void putc(char c)
{
	while ( (UTRSTAT0 & (1 << 14)) ); 	 // ����FIFOΪ�գ���ȴ�
	UTXH0 = c;                           // д���� 
}

/* ����һ���ַ� */
char getc(void)
{
	char ret;
		
	while ( (UFSTAT0&0x7F) == 0 );       // ����FIFOΪ����
	ret = URXH0;                         // ȡ���� 
	
	if((ret == 0x0d) || (ret == 0x0a))
	{
		putc(0x0d);
		putc(0x0a);
	}
	else
		putc(ret);
	
	return ret;                 
}

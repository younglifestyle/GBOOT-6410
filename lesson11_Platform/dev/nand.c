#define NFCONF (*(volatile unsigned long *)0x70200000)
#define NFCONT (*(volatile unsigned long *)0x70200004)
#define NFCMMD (*(volatile unsigned char *)0x70200008)
#define NFADDR (*(volatile unsigned char *)0x7020000C)
#define NFDATA (*(volatile unsigned char *)0x70200010)
#define NFSTAT (*(volatile unsigned char *)0x70200028)

#define TACLS  0x2
#define TWRPH0 0xf
#define TWRPH1 0x7

#define PAGE_SIZE 2048

void select_chip()
{
	NFCONT &= ~(1 << 1);
}

void delete_chip()
{
	NFCONT |= (1 << 1);
}

void clear_RnB()
{
	NFSTAT |= (1 << 4);
}

void send_cmd(unsigned char cmd)
{
	volatile char i;
	
	NFCMMD = cmd;
	for(i=0; i < 10; i++);
}

void send_addr(unsigned char addr)
{
	NFADDR = addr;
}

void wait_RnB()
{
	while( !(NFSTAT&(0x1)) );
}

void reset()
{
	// ѡ��NandFlash
	select_chip();
	
	// ���RnB(NFæ�ź�)
	clear_RnB();
	
	// ����0xFF�ź�
	send_cmd(0xFF);
	
	// �ȴ�RnB
	wait_RnB();
	
	// ȡ��ѡ��
	delete_chip();
}

void nand_init()
{
	/* ����ʱ����� */
	NFCONF |= ( (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4) );
	
	/* ʹ�� nandflash controller*/
	NFCONT = 1 | (1<<1);
	
	// Reset
	reset();
}

void nand_addr(unsigned long addr)
{
	unsigned int col = addr % 2048;         // NF����
	unsigned int page = addr / 2048;        // NF����
	volatile char i = 0;
	
	NFADDR = col&0xFF;
	for(i=0; i < 10; i++);
	NFADDR = (col >> 8)&0xFF;
	for(i=0; i < 10; i++);
	
	NFADDR = page&0xFF;
	for(i=0; i < 10; i++);
	NFADDR = (page >> 8)&0xFF;
	for(i=0; i < 10; i++);
	NFADDR = (page >> 16)&0xFF;
	for(i=0; i < 10; i++);
}

void NF_Page_Read(unsigned long addr, unsigned char *buff, unsigned long size)
{
	int i; 
	unsigned long rest = size;
	unsigned long page;
	
	// ѡ��NF
	select_chip();
	
	while(rest > 0)
	{	
		// ���RnB
		clear_RnB();
		
		// ����0x00����
		send_cmd(0x00);
		
		// ���͵�ַ
		nand_addr(addr);
		
		// ����0x30����
		send_cmd(0x30);
		
		// �ȴ�RnB�ź�
		wait_RnB();
		
		/* �жϳ�һ�����ʵ�size */	
		page = rest>PAGE_SIZE?PAGE_SIZE:rest;

		// ��ȡ����
		for(i=0; i < page; i++)
		{
			*buff++ = NFDATA;
		}
		rest -= page;
		addr += page;	 
	}

	//ȡ��ѡ��
	delete_chip();
}

void copy_2ddr(unsigned long nand_start, unsigned char *buff, unsigned long size)
{
	unsigned long i;
	unsigned long rest = size;
	unsigned long len;
	
	/* NFǰ4ҳ��Ҫ����2K����ȡ */
	for(i = 0; i != 4; i++)
	{
		len = rest>PAGE_SIZE?PAGE_SIZE:rest;
			
		NF_Page_Read(PAGE_SIZE*i, buff, len);
		
		buff += 2048;
		rest -= len;
		if(len == 0)
			return;
	}
	
	/* NF����ǰ4ҳ, �����ÿҳΪ8K */
	NF_Page_Read(PAGE_SIZE*4, buff, rest);
} 

void NF_Page_Write(unsigned long addr, unsigned char *buff, unsigned long size)
{
	unsigned long count = 0;
	int ret; 
	unsigned long rest = size;
	unsigned long page;
	unsigned int col = addr % 2048;         // NF����
	
	// ѡ��NF
	select_chip();
	
	while(count < rest)
	{	
		// ���RnB
		clear_RnB();
		
		// ����0x80����
		send_cmd(0x80);
		
		// ���͵�ַ
		nand_addr(addr);
		
		// д������
		for(; (col < PAGE_SIZE)&&(count < rest); col++)
		{
			NFDATA = buff[count++];
			addr++;
		}
		col = 0;
		
		// ����0x10����
		send_cmd(0x10);
		
		// �ȴ�RnB�ź�
		wait_RnB();
		
		// ����0x70����
		send_cmd(0x70);
		
		// ��ȡд��״̬
		ret = NFDATA;
	}
	
	//ȡ��ѡ��
	delete_chip();
}

void NF_Page_Erase(unsigned long addr)
{
	int i;
	int ret;
	
	// ���������ĵ�ַ��ת��Ϊֻ���е�ַ
	addr = addr / 2048;
	
	// ѡ��NF
	select_chip();
	
	// ���RnB
	clear_RnB();
	
	// ����0x60����
	send_cmd(0x60);
	
	// ����3���е�ַ
	send_addr(addr & 0xFF);
	send_addr((addr >> 8) & (0xFF));
	send_addr((addr >> 16) & (0xFF));
	
	// ����0xD0����
	send_cmd(0xD0);
	
	// �ȴ�RnB�ź�
	wait_RnB();
	
	// ����0x70����
	send_cmd(0x70);
	
	// ��ȡд��״̬
	ret = NFDATA;
	
	//ȡ��ѡ��
	delete_chip();
}
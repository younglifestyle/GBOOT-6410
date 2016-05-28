#define NFCONF (*(volatile unsigned long *)0x70200000)
#define NFCONT (*(volatile unsigned long *)0x70200004)
#define NFCMMD (*(volatile unsigned char *)0x70200008)
#define NFADDR (*(volatile unsigned char *)0x7020000C)
#define NFDATA (*(volatile unsigned char *)0x70200010)
#define NFSTAT (*(volatile unsigned char *)0x70200028)

#define TACLS  0x2
#define TWRPH0 0xf
#define TWRPH1 0x7

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
	NFCMMD = cmd;
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

void NF_Page_Read(unsigned long addr, unsigned char *buff)
{
	int i;
	
	// ѡ��NF
	select_chip();
	
	// ���RnB
	clear_RnB();
	
	// ����0x00����
	send_cmd(0x00);
	
	// ����2���е�ַ
	send_addr(0x00);
	send_addr(0x00);
	
	// ����3���е�ַ
	send_addr(addr & 0xFF);
	send_addr((addr >> 8) & (0xFF));
	send_addr((addr >> 16) & (0xFF));
	
	// ����0x30����
	send_cmd(0x30);
	
	// �ȴ�RnB�ź�
	wait_RnB();
	
	// ��ȡ����
	for(i=0; i < 1024*2; i++)
	{
		*buff++ = NFDATA;
	}
	
	//ȡ��ѡ��
	delete_chip();
}

void copy_2ddr(unsigned long nand_start, unsigned char *buff, int size)
{
	unsigned long i;
	
	for(i = (nand_start >> 11); size > 0;)
	{
		NF_Page_Read(i, buff);
		buff += 2048;
		size -= 2048;
		i++;
	}
}


void NF_Page_Write(unsigned long addr, unsigned char *buff)
{
	int i;
	int ret;
	
	// ѡ��NF
	select_chip();
	
	// ���RnB
	clear_RnB();
	
	// ����0x80����
	send_cmd(0x80);
	
	// ����2���е�ַ
	send_addr(0x00);
	send_addr(0x00);
	
	// ����3���е�ַ
	send_addr(addr & 0xFF);
	send_addr((addr >> 8) & (0xFF));
	send_addr((addr >> 16) & (0xFF));
	
	// д������
	for(i=0; i < 1024*2; i++)
	{
		NFDATA = *buff++;
	}
	
	// ����0x10����
	send_cmd(0x10);
	
	// �ȴ�RnB�ź�
	wait_RnB();
	
	// ����0x70����
	send_cmd(0x70);
	
	// ��ȡд��״̬
	ret = NFDATA;
	
	//ȡ��ѡ��
	delete_chip();
}

void NF_Page_Erase(unsigned long addr)
{
	int i;
	int ret;
	
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
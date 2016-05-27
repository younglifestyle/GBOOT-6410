#define NFCONF (*(volatile unsigned long *)0x70200000)
#define NFCONT (*(volatile unsigned long *)0x70200004)
#define NFCMMD (*(volatile unsigned char *)0x70200008)
#define NFADDR (*(volatile unsigned char *)0x7020000C)
#define NFDATA (*(volatile unsigned char *)0x70200010)
#define NFSTAT (*(volatile unsigned char *)0x70200028)

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
	// 选中NandFlash
	select_chip();
	
	// 清除RnB(NF忙信号)
	clear_RnB();
	
	// 发送0xFF信号
	send_cmd(0xFF);
	
	// 等待RnB
	wait_RnB();
	
	// 取消选中
	delete_chip();
}

void nand_init()
{
	    /* 设置时间参数 */
#define TACLS  7
#define TWRPH0 7
#define TWRPH1 7
    
    NFCONF &= ~((7<<12)|(7<<8)|(7<<4));
    NFCONF |= (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
    
    /* 使能 nandflash controller*/
    NFCONT = 1 | (1<<1);
	
	// Reset
	reset();
}

void NF_Page_Read(unsigned long addr, unsigned char *buff)
{
	int i;
	
	// 选中NF
	select_chip();
	
	// 清除RnB
	clear_RnB();
	
	// 发送0x00命令
	send_cmd(0x00);
	
	// 发送2个列地址
	send_addr(0x00);
	send_addr(0x00);
	
	// 发送3个行地址
	send_addr(addr & 0xFF);
	send_addr((addr >> 8) & (0xFF));
	send_addr((addr >> 16) & (0xFF));
	
	// 发送0x30命令
	send_cmd(0x30);
	
	// 等待RnB信号
	wait_RnB();
	
	// 读取数据
	for(i=0; i < 1024*2; i++)
	{
		*buff++ = NFDATA;
	}
	
	//取消选中
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
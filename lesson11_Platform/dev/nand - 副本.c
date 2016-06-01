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
	NFCONF |= ( (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4) );
	
	/* 使能 nandflash controller*/
	NFCONT = 1 | (1<<1);
	
	// Reset
	reset();
}

void nand_addr(unsigned long addr)
{
	unsigned int col = addr % 2048;         // NF的列
	unsigned int page = addr / 2048;        // NF的行
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

int nand_read(unsigned long nand_start, unsigned char *ddr_start, unsigned long len)  
{  
    unsigned long rest = len;  
    unsigned long addr = nand_start;  
    unsigned long page;  
    unsigned char *dest = ddr_start;  
    int i;  
  
    select_chip();  
    while(rest > 0){  
        send_cmd(0x00);  
        nand_addr(addr);  
        send_cmd(0x30);  
        wait_RnB();  
  
        page = rest>PAGE_SIZE?PAGE_SIZE:rest;  
        for(i = 0; i != page; ++i){  
            *dest++ = NFDATA;  
        }  
        rest -= page;  
        addr += page;  
    }  
    delete_chip();  
  
    return 0;  
}  

void copy_2ddr(unsigned long nand_start, unsigned char *buff, unsigned long size)
{  
    unsigned long rest = size;  
    unsigned long len;  
    unsigned long i;  
  
    for(i = 0; i != 4; ++i)
    {  
        len = rest>2048?2048:rest;  
        nand_read(PAGE_SIZE*i, buff, len);
        buff += 2048;  
        rest -= len;  
        if(rest == 0)  
            return;  
    } 
  
    nand_read(PAGE_SIZE*4, buff, rest);  
}  

void NF_Page_Write(unsigned long addr, unsigned char *buff, int size)
{
	int i;
	int ret, len;
	
	// 选中NF
	select_chip();
	
	// 清除RnB
	clear_RnB();
	
	// 发送0x80命令
	send_cmd(0x80);
	
	// 发送2个列地址
	send_addr(0x00);
	send_addr(0x00);
	
	// 发送3个行地址
	send_addr(addr & 0xFF);
	send_addr((addr >> 8) & (0xFF));
	send_addr((addr >> 16) & (0xFF));
	
	/* 判断出一个合适的size */
	if(size > PAGE_SIZE)
		len = size > PAGE_SIZE*4 ? PAGE_SIZE*4 : size;
	else if(size < PAGE_SIZE)
		len = size;
	else
		len = PAGE_SIZE;
	
//	len = size > PAGE_SIZE ? size : PAGE_SIZE;
//	if(len > PAGE_SIZE*4)
//		len = PAGE_SIZE*4;	
	
	// 写入数据
	for(i=0; i < len; i++)
	{
		NFDATA = *buff++;
	}
	
	// 发送0x10命令
	send_cmd(0x10);
	
	// 等待RnB信号
	wait_RnB();
	
	// 发送0x70命令
	send_cmd(0x70);
	
	// 读取写入状态
	ret = NFDATA;
	
	//取消选中
	delete_chip();
}

void NF_Page_Erase(unsigned long addr)
{
	int i;
	int ret;
	
	// 选中NF
	select_chip();
	
	// 清除RnB
	clear_RnB();
	
	// 发送0x60命令
	send_cmd(0x60);
	
	// 发送3个行地址
	send_addr(addr & 0xFF);
	send_addr((addr >> 8) & (0xFF));
	send_addr((addr >> 16) & (0xFF));
	
	// 发送0xD0命令
	send_cmd(0xD0);
	
	// 等待RnB信号
	wait_RnB();
	
	// 发送0x70命令
	send_cmd(0x70);
	
	// 读取写入状态
	ret = NFDATA;
	
	//取消选中
	delete_chip();
}
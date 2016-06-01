#include "vsprintf.h"
#include "string.h"
#include "printf.h"

unsigned char outbuf[1024];
unsigned char inbuf[1024];

int printf (const char *fmt, ...)
{
	va_list args;
	unsigned int i, len;
	
	//1.将变参转化为字符串
	va_start (args, fmt);                 //利用fmt得到变参的个数，然后把变参放到args里面

	len = vsprintf (outbuf, fmt, args);   //将变参转换为字符串
	va_end (args);

	/* Print the string */
	for(i=0; i < strlen(outbuf); i++)
		putc(outbuf[i]);
		
	return len;
}

int scanf(const char * fmt, ...)
{
	int i = 0;
	unsigned char c;
	va_list args;
	
	//1. 获取输入的字符串
	while(1)
	{
		c = getc();
		if((c == 0x0d) || (c == 0x0a))
		{
			inbuf[i] = '\0';
			break;
		}
		else
		{
			inbuf[i++] = c;
		}
	}
	
	//2. 格式转换
	va_start(args,fmt);
	i = vsscanf(inbuf, fmt, args);
	va_end(args);
	
	return i;
}
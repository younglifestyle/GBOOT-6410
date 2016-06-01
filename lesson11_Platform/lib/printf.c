#include "vsprintf.h"
#include "string.h"
#include "printf.h"

unsigned char outbuf[1024];
unsigned char inbuf[1024];

int printf (const char *fmt, ...)
{
	va_list args;
	unsigned int i, len;
	
	//1.�����ת��Ϊ�ַ���
	va_start (args, fmt);                 //����fmt�õ���εĸ�����Ȼ��ѱ�ηŵ�args����

	len = vsprintf (outbuf, fmt, args);   //�����ת��Ϊ�ַ���
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
	
	//1. ��ȡ������ַ���
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
	
	//2. ��ʽת��
	va_start(args,fmt);
	i = vsscanf(inbuf, fmt, args);
	va_end(args);
	
	return i;
}
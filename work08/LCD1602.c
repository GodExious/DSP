#include<reg51.h>
#include<intrins.h>
#include<string.h>

#define INT8U unsigned char
#define INT16U unsigned int

sbit RS = P2^0;
sbit RW = P2^1;
sbit EN = P2^2;

void lcd1602_test_bf()
{
	INT8U lcd_status;
	do
	{
		P0 = 0xFF; //1111_1111
		EN = 0; RS = 0; RW = 1;
		EN = 1;
		lcd_status = P0;
		_nop_(); _nop_();
		EN = 0;
	}while(lcd_status&0x80);
}

void lcd1602_write_data(INT8U data8)
{
	lcd1602_test_bf();
	EN = 0; RS = 1; RW = 0;
	P0 = data8;
	EN = 1; _nop_(); EN = 0;
}

void lcd1602_write_cmd(INT8U cmd8)
{
	lcd1602_test_bf();
	EN = 0; RS = 0; RW = 0;
	P0 = cmd8;
	EN = 1; _nop_(); EN = 0;
}

void lcd1602_write_str(int r, int c, char *str)
{
	int i = 0;
	INT8U Addressx[] = {0x80, 0xC0};
	INT16U startAddress = (Addressx[r] | c);
	
	lcd1602_write_cmd(startAddress);
	
	for(i=0; i<16; i++)
	{
		if(str[i] == 0) break;
		lcd1602_write_data(str[i]);
	}
	
	for(; i<16; i++)
	{
		lcd1602_write_data(' ');
	}
}

void lcd1602_init()
{
	/*
	  0011_1000	�ù���
 	  DL=1 --> 	8λ����
	  N=0  --> 	������ʾ
	  F=0  -->	��ʾ5x7�ĵ����ַ�
	*/
	lcd1602_write_cmd(0x38);
	/*
	  0000_0001 ����ʾ
	  ָ����01H,��긴λ����ַ00Hλ��
	*/
	lcd1602_write_cmd(0x01);
	/*
	  0000_0110 ������ģʽ
	  I/D=1 --> �������
	  S=0   --> ��Ļ�������������ƻ�����
	*/
	lcd1602_write_cmd(0x06);
	/*
	  0000_1100	��ʾ��/�ؿ���
	  D=1 --> ����ʾ
	  C=0 --> �޹��
	  B=0 --> ��겻��˸
	*/
	lcd1602_write_cmd(0x0C);
}
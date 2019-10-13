#include<reg51.h>
#include<string.h>
#include<intrins.h>

#define Uint unsigned int
#define Uchar unsigned char

sbit CS=P1^0; // chip select
sbit CLK=P1^1; // clock
sbit DIO=P1^2; // data input and output
sbit RS=P2^0; // data register status register
sbit RW=P2^1; // read/write
sbit EN=P2^2; // enable

Uchar disp_buff1[]="VOLTAGE:  0.00V";
Uchar disp_buff2[16];

/*
* ��ʱ
*  
* @param x ʱ�䣨����ȷ��
* @return
*/
void delay_ms(Uint x)
{
	Uchar t;
	while(x--)
		for(t=0; t<120; t++);
}

/*
* ��ʱ2΢��
* 
* @return
*/
void delay2us()
{
	_nop_();
	_nop_();
}

/*
* ���BF(busy flag)λ״̬
* 
* @return
*/
bit test_BF()
{
	Uchar LCD_status;
	P0=0xFF;
	EN=0;RS=0;RW=1;
	EN=1;delay2us();LCD_status=P0;
	EN=0;
	return (LCD_status&0x80)?1:0;
}

/*
* д����
*
* @param cmd ��λ����
* @return
*/
void write_CMD(Uchar cmd)
{
	while(test_BF());
	EN=0; RS=0; RW=0;
	P0=cmd;
	EN=1; _nop_(); EN=0;
}

/*
* д���ݣ�һλһλ��д��
*
* @param data8 ��λ����
* @return
*/
void write_Data(Uchar data8)
{
	while(test_BF());
	EN=0; RS=1; RW=0;
	P0=data8;
	EN=1; _nop_(); EN=0;
}

/*
* ��ʼ��
*
* @return
*/
void init()
{
	write_CMD(0x38);delay_ms(1);
	write_CMD(0x01);delay_ms(1);
	write_CMD(0x06);delay_ms(1);
	write_CMD(0x0C);delay_ms(1);
}

/*
* д�ַ���
*
* @param r row
* @param c column
* @param str �ַ���
* @return
*/
void write_Str(int r, int c, char *str)
{
	int i=0;	
	unsigned char Addressx[] = {0x80, 0xC0};
	unsigned char StartAdd = (Addressx[r] | c);//��λ��

	write_CMD(StartAdd);
	
	for(i = 0; i < 16; i++){
		if(str[i]==0) break;
		write_Data(str[i]);
	}
	// �������16λ���ÿո����
	for(;i < 16; i++){
		write_Data(' '); 	
	}
}

/*
* ADת��
*
* @return �������
*/
Uchar get_AD_Res()
{
	Uchar i, data1=0, data2=0;
	CS=0;
	//��һ�����ڣ�ת����ʼ
	CLK=0;DIO=1;delay2us();
	CLK=1;delay2us();
	
	//�ڶ������ڣ�ѡ��ͨ������˫ͨ�� DIO=0˫ͨ����� �� DIO=1��ͨ��
	CLK=0;DIO=1;delay2us(); 
	CLK=1;delay2us();
	
	//���������ڣ�DIOѡ��CH1-->���DIO=0 ѡ��CH0
	CLK=0;DIO=0;delay2us();
	CLK=1;delay2us();
	
	//�ȴ�
	CLK=0;DIO=1;delay2us(); 
	
	//�Ƚ�����Ϊ���λ�������Ϊ���λ
	for(i=0; i<8; i++)
	{
		CLK=1;delay2us();
		CLK=0;delay2us();
		/*
		* 0000_0000|0000_000��=0000_000��
		* 0000_00��0|0000_000��=0000_00����
		* 0000_0����0|0000_000��=0000_0������
		*/
		data1=(data1<<1)|(Uchar)DIO; 
	}
	
	//�Ƚ�����Ϊ���λ�������Ϊ���λ
	for(i=0; i<8; i++)
	{
		/*
		* 0000_0000|0000_000��=0000_000��
		* 0000_000��|0000_00��0=0000_00����
		* 0000_00����|0000_0��00=0000_0������
		*/
		data2= data2|(Uchar)DIO<<i;
		CLK=1;delay2us();
		CLK=0;delay2us();
		 
	}
	
	//��ֹƬѡ
	CS=1;
	
	return (data1==data2)?data1:0;
}

void main()
{
	Uchar i=0,AD=0;
	Uint d=0;
	init();
	
	while(1)
	{
		AD=get_AD_Res(); 
		d=(AD*500.0/256.0);
		
		disp_buff1[10]=d/100+'0';
		disp_buff1[12]=(d/10)%10+'0';
		disp_buff1[13]=d%10+'0';
		
		write_Str(0,0,disp_buff1);
		
		//���ʵ�ĺͿ��ĵĿ��
		i=(Uint)AD*16/256;
		memset(disp_buff2,'\xFF',i);
		memset(disp_buff2+i,'\xDB',16-i);
		write_Str(1,0,disp_buff2);
	}
	
}


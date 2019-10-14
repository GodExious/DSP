#include<reg51.h>
#include<stdio.h>
#include<stdlib.h>
#include<intrins.h>
#include<string.h>

//��ַ����
sbit ADDC=P1^6;
sbit ADDB=P1^5;
sbit ADDA=P1^4;

sbit CLK=P1^3; //clock
sbit ST=P1^2; //start (ת�������ź�)
sbit EOC=P1^1; //end of conversion (ת�������ź�)
sbit OE=P1^0; //output enable (��������ź�,����)

//0~9
unsigned char code SEG7[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00};

/*
* ��ʱ
*  
* @param x ʱ�䣨����ȷ��
* @return
*/
void delay_ms(int i)
{
	int j=0;
	while(i--)
	{
		for(j=0;j<120;j++);
	}
}

/*
* ��ʾ
* 
* @param ADx ADת������İ�λ��ֵ
* @return
*/
void display(unsigned char ADx)
{
	int AD = ADx*500.0/255.0;
	P2=0xFD; P0=SEG7[AD/100]; delay_ms(5);//1111_1101 -> P2^1 
	P2=0xFB; P0=SEG7[(AD/10)%10]; delay_ms(5);//1111_1011 -> P2^2
	P2=0xF7; P0=SEG7[AD%10]; delay_ms(5);//1111_0111 -> P2^3
}

/*
* ��ʼ��
*
* @return
*/
void init()
{
	// ABC=110ѡ�����ͨ��
	ADDC=0;
	ADDB=1;
	ADDA=1;
	
	CLK=0;
	ST=1; //�ڲ��Ĵ�������
	EOC=1; // ת������(δ��ʼ)
	OE=0; 
}

void main()
{
	init();
	
	//TMOD(��ʱ��)
	TMOD=0x02; //0000_0010 8λ 0~255
	/*
	 �Զ�ʱ��T0��˵�Ƿֳ������Ĵ�����TH0Ϊ�߰�λ��TL0Ϊ�Ͱ�λ�������16λ�Ķ�ʱ����
	 ����λTL0���������λTH0��һ������Ȼ������
	 TH0��TL0��������Ƶ��(��ֵԽ��Ƶ��Խ��)
	*/
	TH0=200; 
	TL0=200; 
	EA=1; // ���жϿ���
	ET0=1; // T0��ʱ�����жϿ���
	TR0=1; // ����T0�Ŀ���ʱ��λ
	
	while(1)
	{
		ST=0;_nop_();ST=1;_nop_();ST=0;
		while(EOC==0);
		OE=1;
		display(P3);
		OE=0;
	}
	
}

/*
* �ж�
*
* @return
*/
void t0() interrupt 1
{
	CLK=!CLK;
}
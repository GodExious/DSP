#include<reg51.h>	   
#include<intrins.h>   
#include<string.h>     
#include<stdio.h>

#define INT8U  unsigned char
#define INT16U unsigned int
#define INT32U unsigned long

sbit CS=P0^2;	  
sbit CLK=P0^1;	     
sbit DIO=P0^0;

INT8U Get_AD_Result()
{	 
	INT8U i=0,dat1=0x00,dat2=0x00;
	CS=0;//Ƭѡ
	CLK=0;_nop_();_nop_();
	DIO=1;//����0832
	CLK=1;_nop_();_nop_();	


	CLK=0;_nop_();_nop_();
	DIO=1;//ѡ��ͨ������˫ͨ��
	CLK=1;_nop_();_nop_();

	CLK=0;_nop_();_nop_();
	DIO=0;//ѡ��0����1ͨ��
	CLK=1;_nop_();_nop_();

	CLK=0; DIO=1;_nop_();_nop_(); //�ͷ����ߣ�׼���ɼ�
	for(i=0;i<8;i++)//�½��زɼ����ݣ��Ӹ�λ����λ
	{
		CLK=1;_nop_();_nop_(); 
		CLK=0; _nop_();_nop_();
		dat1=(dat1<<1)|((INT8U)DIO);
	}	
	for(i=0;i<8;i++)//�½��زɼ����ݣ��ӵ�λ����λ
	{
		dat2=(dat2)|(((INT8U)DIO)<<i);	
		CLK=1;_nop_();_nop_(); 
		CLK=0; _nop_();_nop_();
	}
	_nop_();_nop_();
	CS=1;//0832Ƭѡ����
	if(dat2==dat1)	//�������������ȣ������
		return dat1;
	else
		return 0;
}//5v 255	126 3.24 324
/*
INT16U Digit_Convert_Analog(INT8U dat)
{
	INT16U AnalogValue=0x0000;//   x*500/256=y
	AnalogValue=(INT16U)((dat*500.0)/256.0);
	return AnalogValue;
}
*/
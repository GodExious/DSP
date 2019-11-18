#include<reg51.h>
#include<intrins.h>
#include<string.h>
#include<stdio.h>

#define INT8U unsigned char
#define INT16U unsigned int
#define INT32U unsigned long

sbit DS = P3^5; //���ݴ��������
sbit SH_CP = P3^6; //���ݴ���������λʱ���źţ���������Ч
sbit ST_CP = P3^7; //���ݲ������ʱ���źţ���������Ч

void serialInputData(INT8U dat)
{
	INT8U i = 0;
	for(i=0; i<8; i++)
	{
		if(dat&0x80) //1000_0000
			DS=1;
		else
			DS=0;
		
		dat=dat<<1;
		SH_CP=0; _nop_();_nop_();
		SH_CP=1; _nop_();_nop_();
	}
	SH_CP=0; _nop_();_nop_();
}

void paralledOutputData()
{
	ST_CP=0; _nop_();_nop_();
	ST_CP=1; _nop_();_nop_();
	ST_CP=0; _nop_();_nop_();	
}






#include<reg51.h>
#include<stdio.h>
#include<intrins.h>
#include<string.h>
#include<stdlib.h>

#define INT8U unsigned char
#define INT16U unsigned int

sbit DQ = P3^4; //special bit
INT8U tempValue[]={0x00, 0x00};
extern void delay_ms(INT16U x);

/**
* ��ʱ���� ���12Mhz�ľ���
* _nop_()�൱��һ���������ڣ�12�������ڣ� --> 1us
*
*@param x ��ʱʱ�䣨��λus��
*@return
*/
void delay_us(INT16U x)
{
	while(x--)
		_nop_();
}


/**
* ��ȷ��ʱ����
* delay(0):��ʱ518us ���:518-2*256=6
* delay(1):��ʱ7us
* delay(10):��ʱ25us    ���:25-20=5
* delay(20):��ʱ45us    ���:45-40=5
* delay(100):��ʱ205us ���:205-200=5
* delay(200):��ʱ405us ���:405-400=5
*
*@param x ��ʱʱ��
*@return 
*/
void delay(INT16U x)
{
	while(--x);
}

/**
* ��ʼ��
* ���͵�ƽ��Ҫ����480us���ߵ�ƽ��Ҫ����15us
*
*@return 0����ʼ���ɹ� ��1����ʼ��ʧ�� 
*/
INT8U init()
{
	INT8U status;
	DQ = 1; delay_us(15);	
	DQ = 0; delay_us(450);	
	DQ = 1; delay_us(15);
	status = DQ;
	delay_ms(100); 
	
	return status;
}

/**
* ��ȡDQ�е�����
*
* @return ��λ����
*/
INT8U read_byte()
{
	INT8U i, dat=0x00;
	
	for(i=0x01; i!=0x00; i<<=1) // �ӵ͵��߶�
	{
		DQ=0; delay_us(1);
		DQ=1; delay_us(7);
		if(DQ)
			dat = dat|i;
		delay_us(25); //����ʱ�����٣�ȷ����ʱ��ĳ���60us��
	}
	
	return dat;
}

/**
*д����
*
*@dat ��λ����
*@return
*/
void write(INT8U dat)
{
	INT8U i;
	
	for(i=0; i<8; i++)
	{
		DQ=0; delay_us(1);
		dat = dat>>1; //�����λ�Ƶ�PSW�Ľ�/��λCY
		DQ = CY;
		delay(8);
		DQ = 1;
	}
}

/**
*��ȡ�¶�����ֵ
*
*@return 1���ɹ���0��ʧ��
*/
INT8U read_temp()
{
	if(init()) return 0;
	else
	{
		write(0xCC); //�������� 
		write(0x44); //�����¶�ת��
		write(0xBE); //��ȡ�¶�����ֵ
		tempValue[0] = read_byte(); //��ȡRAM���8λ
		tempValue[1] = read_byte(); //��ȡRAM���8λ
		return 1;
	}
}
#include<reg51.h>
#include<intrins.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#define INT8U unsigned char
#define INT16U unsigned int

extern void delay_ms(INT16U x);
extern void LCD_Initialize();
extern void LCD_String(INT8U r,INT8U c,INT8U *str);
extern INT8U Temp_Value[];
extern void delayx(INT16U x);
extern INT8U Read_temperature();

sbit MA=P1^0;//ֱ�����A�� 
sbit MB=P1^1;//ֱ�����B�� 
sbit PWM=P1^2;//pulse width modification �����ȵ��� ���ܵ���

static int count = 0;
float tempv;
INT8U buff[17];

void T0_interrupt() interrupt 1 //INT0 0, T0 1, INT1 2, T1 3, Serial T 4 
{
	count++;
	TL0 = ((INT16U)(65536-(INT16U)(11.0592/12*500)))%256; //t0 low delay->500us
	TH0 = ((INT16U)(65536-(INT16U)(11.0592/12*500)))%256; //t0 high
	if(count==10)		//������ʱ -> 50ms
	{
		count=0;
		
		if(Read_temperature()==1) //˳�������ƻ��ִ�������Ϊ��ʱ
		{
			count=0;
			tempv=(((int)Temp_Value[1]<<8)|((int)Temp_Value[0]))*0.0625;
			sprintf(buff,"temp:  %5.1f\xDF\x43",tempv);  
			LCD_String(1,0,buff);
			if(tempv>=75)
				tempv=75;//��ת75����ȫ����  temp speed
			if(tempv<=0)
				tempv=0;//��ת0����ȫ����
			if(tempv>=45)
			{   
				MA=1;
				MB=0;
				if(tempv==45)
					PWM=0;
				else if(tempv==75)
					PWM=1;
				else
				{   
					PWM=1;delay_ms(tempv-45); //����
					PWM=0;delay_ms(75-tempv);//������
				}
			}
			else if(tempv<=10)
			{   
				MA=0;
				MB=1;
				if(tempv==10) 
					PWM=0;
				else if(tempv==0)  
					PWM=1;
				else
				{   
					PWM=1;delay_ms(10-tempv); //����
					PWM=0;delay_ms(tempv-0);//������
				}

			}
			else
			{   
				MA=0;
				MB=0;
			}
		}
	}
		
	
	
}

void main()
{ 
	LCD_Initialize();	
	LCD_String(0,0,"  recent temp");
	LCD_String(1,0,"  Waiting.......");
	Read_temperature();
	delay_ms(1500);
	TMOD=0x01; //��ʱ��16λ 65535
	EA = 1; //�ܿ���
	ET0 = 1; //�ֿ���
	TR0 = 1; //��ʱ������
	
	while(1);//�����ܷ�
}
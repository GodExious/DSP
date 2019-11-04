#include<reg51.h>//register
#include<intrins.h>//_nop_() _crol_()
#define INT8U unsigned char
#define INT16U unsigned int
#define delay4us();	{_nop_();_nop_();_nop_();_nop_();}

extern void delay_ms(INT16U x);

sbit DQ=P3^3;
INT8U Temp_Value[]={0x00,0x00};	//�¶����ݵĵ�8λ���8λ

void delayx(INT16U x)
{
	while(x--);
}

/***********AT89C51����Ƶһ��Ҫ��Ϊ11.0592MHz*****/
INT8U Init_DS18B20()//��ʼ���ͼ��DS18B20
{
	INT8U DQ_status;
	DQ=1; delayx(8);//delay77us
	DQ=0; delayx(90);//>delay480us
	DQ=1; delayx(5);//>delay15us
	DQ_status=DQ; delayx(90);
	
	return DQ_status;
}


INT8U Readonebyte()//��ȡDS18B20�е�1���ֽڵ�����
{
	INT8U i,datatemp=0x00;
	for(i=0x01;i!=0x00;i<<=1)//100000000
	{
		DQ=0;_nop_();  
		DQ=1;_nop_();
		if(DQ) datatemp=datatemp|i; 
		delayx(8);
	}
	return datatemp;

}

void Writeonebyte(INT8U dat)//dat>>1 00001010 1CY ACC
{ 
	INT8U i;
	for(i=0;i<8;i++)
	{
		DQ=1;_nop_(); 
		DQ=0;_nop_();
		dat=dat>>1;  //PSW
		DQ=CY;
		delayx(8); 
	 
	}
}

INT8U Read_temperature()
{
	if(Init_DS18B20()==1) return 0;
	else
	{
		Writeonebyte(0xCC);//�������к�
		Writeonebyte(0x44);//����ת��
		Init_DS18B20(); 
		Writeonebyte(0xCC);//�������к� ��Ϊֻ��һ��DS18B20
		Writeonebyte(0xBE);//���¶ȼĴ�����ĵ�0��1�ֽ�
		Temp_Value[0] = Readonebyte(); 
		Temp_Value[1] = Readonebyte();
		
		return 1;
	}
}

























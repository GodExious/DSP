#include<reg51.h>
#include<intrins.h>
#include<string.h>
#include<stdio.h>

#define INT8U unsigned char
#define INT16U unsigned int
#define INT32U unsigned long

sbit DS = P1^5; //���ݴ��������
sbit SH_CP = P1^6; //���ݴ���������λʱ���źţ���������Ч
sbit ST_CP = P1^7; //���ݲ������ʱ���źţ���������Ч

//�������߶�����ܵı���
INT8U code segment7[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};


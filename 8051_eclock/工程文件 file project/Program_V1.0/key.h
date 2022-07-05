#ifndef  __key_H
#define  __key_H
#include <STC8.H>

#define u8 unsigned char
#define u16 unsigned int
	
extern u8 mode,dis_format,temp;	//��ʾ�������
extern u8 hour_t,min_t,sec_t,mon_t,day_t,year_t,week_t;	//��ʱ������
extern bit dis_time,dis_temp;	//�����¶���ʾ����


sbit SW1 = P3^4;//SET
sbit SW2 = P3^5;//CHANGE
sbit SW3 = P3^6;//ENTER

extern void Key_Set();//SET���̰���������ʱ��(���Զ�����Ϊ30)���գ���ENTER��ȷ��
extern void Key_Enter();//ENTER��setʱ���̰�д��ʱ�䣬���޸Ķ���������ɶ̰������������棬��������̰���ʾ�¶ȣ������Ӻ��Զ���ʾʱ��
extern void Key_Change();//CHANGE��setʱ������ʱ�䣬��������̰���ʾ����

#endif
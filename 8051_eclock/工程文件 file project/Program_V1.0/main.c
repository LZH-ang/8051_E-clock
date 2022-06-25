#include <STC8.H>
#include "intrins.h"
#include "delay.h"
#include "3231.h"
#include "TM1637.h"
#include "key.h"

u8 const num[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7f,0x6F};
u8 sec_dot=0,cp=0,light=3;
bit light_con=0,TM0_flag=0;//�Զ����ȿ���
	
void get_Temperature();	 //�¶Ȼ�ȡ ��ʾ
void Timer0Init(void)		//5����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ʼֵ
	TH0 = 0x28;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
	EA = 1;
}

void main()
{
	P1M0 = 0x30;//��©
	P1M1 = 0x30;
	P3M1 = 0x00;//׼˫��
	P3M0 = 0x00;
	Timer0Init();
	while(cp<=20);cp = 0;//�ȴ������ȶ�
//	ModifyTime(DS3231_SECOND,30);_nop_();//������
//	ModifyTime(DS3231_MINUTE,8);_nop_();
//	ModifyTime(DS3231_HOUR,22);_nop_();
//	ModifyTime(DS3231_YEAR,22);_nop_();
//	ModifyTime(DS3231_MONTH,1);_nop_();
//	ModifyTime(DS3231_DAY,15);_nop_();
//	ModifyTime(DS3231_WEEK,7);_nop_();
	while(1)
	{
		Key_Set();
		Key_Change();
		Key_Enter();	
		if(cp ==70)//ÿ��Լ300ms��һ��ʱ��
		{
			cp = 0;
			year = read_random(DS3231_YEAR);_nop_();
			mon  = read_random(DS3231_MONTH);_nop_();
			day  = read_random(DS3231_DAY);_nop_();
			hour = read_random(DS3231_HOUR);_nop_();
			min  = read_random(DS3231_MINUTE);_nop_();
			sec  = read_random(DS3231_SECOND);_nop_();
			week = read_random(DS3231_WEEK);_nop_();
			if(light_con == 0)
			{
				if((hour >= 6)&&(hour < 23))light = 5;//23-6������ȣ�6-23�������
				else light = 1;
				light_con = 1;
			}
			else if((!min)&&(hour == 6 || hour == 23))light_con = 0;
			if(sec%2)sec_dot=0x80;else sec_dot=0;//С��������˸		
		}	
		if(TM0_flag)
		{
			if(mode == 0)//��������
			{	
				if(dis_temp == 1)get_Temperature();
				else
				{
					if(dis_time == 0)display_4seg(num[hour/10],num[hour%10]|sec_dot,num[min/10]|sec_dot,num[min%10],light);   //��ʾʱ��
					if(dis_time == 1)display_4seg(num[mon/10],num[mon%10]|0x80,num[day/10],num[day%10],light);	//��ʾ����
				}												
			}
			if(mode == 1)//���ý���
			{
				if(temp){temp = 0;hour_t = hour;min_t = min;sec_t = sec;mon_t = mon;day_t = day;}//ÿ�������ý����ȡ��ǰʱ��
				if(dis_format == 1)display_4seg(num[hour_t/10],num[hour_t%10],0x08,0x08,light);			 //ʱ��������
				if(dis_format == 2)display_4seg(0x08,0x08,num[min_t/10],num[min_t%10],light);				 //�ֵ�������
				if(dis_format == 3)display_4seg(num[mon_t/10],num[mon_t%10],0x08,0x08,light);				 //�µ�������
				if(dis_format == 4)display_4seg(0x08,0x08,num[day_t/10],num[day_t%10],light);				 //�յ�������
			}
		}
	}
}

void TM0_Isr() interrupt 1
{
	cp++;
	TM0_flag = ~TM0_flag;
}

void get_Temperature()	 //�¶Ȼ�ȡ ��ʾ
{
  uchar Tem_L; 
	uchar Tem_H;
	
  Tem_H=read_randomTmp(DS3231_TEMPERATUREH);    //�¶� ���ֽ� ��������
	Tem_L=read_randomTmp(DS3231_TEMPERATUREL);	 //�¶� ���ֽ� С������
	Tem_L = (Tem_L>>6)*0.25*100;	 
	
	if(!(Tem_H & 0x80))	//���¶�
	{ 
		/********��ʾ�¶�*********/
		if(Tem_H/10 > 0)
			display_1seg(num[Tem_H/10],0,light);//��ʾ�¶�ʮλ
		else
			display_1seg(0x00,0,light);//�����¶�ʮλΪ0����ʾ
		  display_1seg(num[Tem_H%10]|0x80,1,light);//��ʾ�¶ȸ�λ����ʾС����
		  display_1seg(num[Tem_L/10],2,light);//��ʾ�¶�С����1λ
		  display_1seg(num[Tem_L%10],3,light);//��ʾ�¶�С����2λ
	}
	else
	{ 
		if(Tem_H & 0x80)	 //���¶�
		{
			display_4seg(0x08,0x08,0x08,0x08,light);
		}
	}
}	

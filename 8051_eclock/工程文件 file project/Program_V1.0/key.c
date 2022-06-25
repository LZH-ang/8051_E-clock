#include "key.h"
#include "intrins.h"
#include "delay.h"
#include "3231.h"

u8 mode=0,dis_format=0,temp=1;	//��ʾ�������
u8 hour_t,min_t,sec_t,mon_t,day_t,year_t,week_t;	//��ʱ������
bit dis_time=0,dis_temp=0;	//�����¶���ʾ����
bit or_wr=0;	//�ж����ý����Ƿ���ʱ��Ķ�����
bit or_hour=0,or_min=0,or_mon=0,or_day=0;	//�ж����ý���ʱ�������Ƿ��иĶ�����

void Key_Set()
{
	if(SW1 == 0)
	{
		Delay10ms();
		if(SW1 == 0)	//����&���ý����л�
		{
			mode = 1;
			dis_format++;
			if(dis_format >= 5)
			{
				dis_format = 0;
				mode = 0;
				temp = 1;
				or_wr=0;
				or_hour=0;
				or_min=0;
				or_mon=0;
				or_day=0;
			}
		}
		Delay10ms();
		while(!SW1);	
	}
}

void Key_Change()
{
	if(SW2 == 0)
	{
		Delay10ms();
		if(SW2 == 0)
		{
			if(mode == 0)	//ʱ��&������ʾ�л�
			{
				dis_time = ~dis_time;
			}
			if(mode == 1)	//����ʱ��
			{
				if(dis_format == 1){hour_t++;if(hour_t >= 24)hour_t = 0;or_wr = 1;or_hour = 1;}
				if(dis_format == 2){min_t++;if(min_t >= 60)min_t = 0;or_wr = 1;or_min = 1;}
				if(dis_format == 3){mon_t++;if(mon_t >= 13)mon_t = 1;or_wr = 1;or_mon = 1;}
				if(dis_format == 4){day_t++;if(day_t >= 32)day_t = 1;or_wr = 1;or_day = 1;}
			}
		}
		Delay10ms();
		while(!SW2);
	}	
}

void Key_Enter()
{
	if(SW3 == 0)
	{
		Delay10ms();
		if(SW3 == 0)
		{
			if(mode == 0)
			{
				dis_temp = ~dis_temp;
			}
			if(mode == 1)
			{
				if(or_wr == 0)//�޲������˳�
				{
					mode = 0;
					dis_format=0;
					temp = 1;			
				}
				else if((or_wr == 1)&&(mode == 1))//�в�����д��ʱ��
				{
					if(or_hour){ModifyTime(DS3231_HOUR,hour_t);or_wr = 0;or_hour = 0;}
					if(or_min){ModifyTime(DS3231_MINUTE,min_t);_nop_();ModifyTime(DS3231_SECOND,30);or_wr = 0;or_min = 0;}
					if(or_mon){ModifyTime(DS3231_MONTH,mon_t);or_wr = 0;or_mon = 0;}
					if(or_day){ModifyTime(DS3231_DAY,day_t);or_wr = 0;or_day = 0;}				
				}
			}			
		}
		Delay10ms();
		while(!SW3);
	}	
}

#include "3231.h"
#include "delay.h"
#include "intrins.h"

uchar hour,min,sec,year,mon,day,week;
bit	ack;		//Ӧ���־λ

/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{
	SCL = 1;		
	SDA = 1;
	SDA = 0;
	_nop_();
	SCL = 0;
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
	SCL = 1;
	SDA = 0;
	SDA = 1;
}

/********************************************************************************************************
** 	3231
********************************************************************************************************/
uchar	BCD2HEX(uchar val)
{
	return	((val>>4)*10)+(val&0x0f);
}

uchar	HEX2BCD(uchar val)
{
	return	(((val%100)/10)<<4)|(val%10);
}


void SendByte(uchar c)
{
	uchar BitCnt;
	SCL=0;
	for(BitCnt=0;BitCnt<8;BitCnt++)         //Ҫ���͵����ݳ���Ϊ8λ
	{
		if((c<<BitCnt)&0x80)
			SDA=1;                          //�жϷ���λ
		else 
			SDA=0; 
		_nop_();
		SCL=1;                            //��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ
		Delay5us();                       //��֤ʱ�Ӹߵ�ƽ���ڴ���4��s   
		SCL=0;
	}
	SDA=1;                                  //8λ��������ͷ������ߣ�׼������Ӧ��λ
	_nop_();
	SCL=1;
	Delay5us();
	if(SDA==1)
			ack=0;    
	else
			ack=1;                              //�ж��Ƿ���յ�Ӧ���ź�
	SCL=0;
	Delay5us();
}	   

uchar RcvByte()
{
	uchar retc;
	uchar BitCnt;

	retc=0;
	SDA=1;                           //��������Ϊ���뷽ʽ
	for(BitCnt=0;BitCnt<8;BitCnt++)
	{
		SCL=0;                      //��ʱ����Ϊ�ͣ�׼����������λ       
		Delay5us();                 //ʱ�ӵ͵�ƽ���ڴ���4.7��s		       
		SCL=1;                      //��ʱ����Ϊ��ʹ��������������Ч
		Delay5us();
		retc=retc<<1;
		if(SDA==1)
				retc=retc+1;            //������λ,���յ�����λ����retc��
		Delay5us();
	}
	SCL=0;
	return(retc);
}			    
 
void Ack_I2C(bit a)
{
	SDA	=	a;  
	SCL=1;		      
	Delay5us();             //ʱ�ӵ͵�ƽ���ڴ���4us    
	SCL=0;                  //��ʱ���ߣ�ǯסI2C�����Ա��������
	Delay5us();   
}					    

uchar write_byte(uchar addr, uchar write_data)
{
	IIC_Start();
	SendByte(DS3231_WriteAddress);
	if (ack == 0)
			return 0;
 
	SendByte(addr);   
	if (ack == 0)
			return 0;
 
	SendByte(write_data);
	if (ack == 0)
			return 0;
 
	IIC_Stop();
	Delay5us();      
	Delay5us();      
	return 1;
}					  

uchar read_current()
{
	uchar read_data;
	IIC_Start();
	SendByte(DS3231_ReadAddress);
	if(ack==0)
		 return(0);	      
	read_data = RcvByte();
	Ack_I2C(1);
	IIC_Stop();
	return read_data;
}						 

uchar read_random(uchar random_addr)
{
	uchar Tmp;
	IIC_Start();
	SendByte(DS3231_WriteAddress);
	if(ack==0)
			return(0);	    
	SendByte(random_addr);
	if(ack==0)
			return(0);
	Tmp=read_current();
	if(random_addr==DS3231_HOUR) 
	Tmp&=0x3f;
						
	return(BCD2HEX(Tmp));//��ת10�������
}

/***********************/
uchar read_randomTmp(uchar random_addr)  //����Ƕ��¶Ⱥ���
{
	uchar Tmp;
	write_byte(0x0e,0x20);//0x0e�Ĵ�����CONVλ��1�����¶�ת����Ҫ��䣬�¶�ʵʱˢ�£���䲻Ҫ���¶�Ҫ64s��ˢ��1��
	IIC_Start();
	SendByte(DS3231_WriteAddress);
	if(ack==0)
			return(0);	    
	SendByte(random_addr);
	if(ack==0)
			return(0);
	Tmp=read_current();
	return Tmp;
}				  

void ModifyTime(uchar address,uchar num)
{
	uchar temp=0;
	if(address>6 && address <0) return;
	temp=HEX2BCD(num);
	write_byte(address,temp);
}

/*********************/	
//void get_Temperature()	 //�¶Ȼ�ȡ ��ʾ
//{
//  uchar Tem_L; 
//	uchar Tem_H;
//  Tem_H=read_randomTmp(DS3231_TEMPERATUREH);    //�¶� ���ֽ� ��������
//	Tem_L=read_randomTmp(DS3231_TEMPERATUREL);	 //�¶� ���ֽ� С������
//	
//	Tem_L = (Tem_L>>6)*0.25*100;	 //
//	
//	if(!(Tem_H & 0x80))	//���¶�
//	{ 
//		/********��ʾ�¶�*********/
//		if(Tem_H/10 > 0)
//			//��ʾ�¶�ʮλ
//		else
//			//�����¶�ʮλΪ0����ʾ
//		  //��ʾ�¶ȸ�λ
//		  //��ʾС����
//		  //��ʾ�¶�С����1λ
//		  //��ʾ�¶�С����2λ
//	}
//	else
//	{ 
//		if(Tem_H & 0x80)	 //���¶�
//		{
//			Tem_H =(~Tem_H)+1; //�����ʵ���¶�ֵ
//				/��ʾ��-������
//			if(Tem_H/10 > 0)
//				//��ʾ���¶�ʮλ
//			else
//				//���¶�ʮλΪ0����
//				//��ʾ���¶ȸ�λ
//		  	//��ʾС����
//			  //��ʾ�¶�С����1λ
//			  //��ʾ�¶�С����2λ
//		}
//	}
//	//��ʾ�����
//}	


#ifndef		__3231_H
#define		__3231_H

#include <STC8.H>
#define uint  unsigned int
#define uchar unsigned char
// ------------------------------------------------------------
// IO��ģ��I2Cͨ��
// ------------------------------------------------------------
sbit SCL=P1^0; //����ʱ��
sbit SDA=P1^1; //��������

extern char hour,min,sec,year,mon,day,week;
extern bit	ack;		//Ӧ���־λ
/********************************************************************************************************
** 	DS3231��������
********************************************************************************************************/
#define DS3231_WriteAddress 0xD0    //����д��ַ 
#define DS3231_ReadAddress  0xD1    //��������ַ
#define DS3231_SECOND       0x00    //��
#define DS3231_MINUTE       0x01    //��
#define DS3231_HOUR         0x02    //ʱ
#define DS3231_WEEK         0x03    //����
#define DS3231_DAY          0x04    //��
#define DS3231_MONTH        0x05    //��
#define DS3231_YEAR         0x06    //��

#define DS3231_TEMPERATUREH 0x11    //�¶ȼĴ������ֽ�(8λ)	��������
#define DS3231_TEMPERATUREL 0x12    //�¶ȼĴ������ֽ�(��2λ) С������ 

#define NACK    1
#define ACK     0

extern void IIC_Start();
extern void IIC_Stop();
extern uchar	BCD2HEX(uchar val);
extern uchar	HEX2BCD(uchar val);
extern void SendByte(uchar c);	
extern uchar RcvByte();	
extern void Ack_I2C(bit a);	
extern uchar write_byte(uchar addr, uchar write_data);	
extern uchar read_current();
extern uchar read_random(uchar random_addr);//��ȡʱ��
extern uchar read_randomTmp(uchar random_addr);
extern void ModifyTime(uchar address,uchar num);//�޸�ʱ��

#endif
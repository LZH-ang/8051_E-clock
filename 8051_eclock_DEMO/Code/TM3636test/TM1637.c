#include "TM1637.h"
#include <intrins.h>
void Delay2us()                //@11.0592MHz
{
	unsigned char i;
	_nop_();
	i = 5;
	while (--i);
}
void Delay3us()                //@11.0592MHz
{
	unsigned char i;
	i = 9;
	while (--i);
}
void Delay5us()                //@11.0592MHz
{
	unsigned char i;
	_nop_();
	i = 16;
	while (--i);
	}
void I2CStart()
{
	CLK = 1;
	DIO = 1;
	Delay2us();
	DIO = 0;
}
void I2CAsk()
{
	CLK = 0;
	Delay5us();
	while(DIO);
	CLK = 1;
	Delay2us();
	CLK = 0;
}
void I2CStop()
{
	CLK = 0;
	Delay2us();
	DIO = 0;
	Delay2us();
	CLK = 1;
	Delay2us();
	DIO = 1;
}
void I2CWriteByte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		CLK = 0;
		if(dat&0x01)
		{
			DIO = 1;
		}
		else
		{
			DIO = 0;
		}
		Delay3us();
		dat>>=1;
		CLK = 1;
		Delay3us();
	}
}
void Tm1636_write_display(u8 mode)//1:�Զ�;0;�̶�
{
	I2CStart();
	if(mode)I2CWriteByte(0x40);	//40H ��ַ�Զ���1 ģʽ,
	else I2CWriteByte(0x44);		//44H �̶���ַģʽ
	I2CAsk();										
	I2CStop();	
}
void Tm1636_set_segbit(u8 addr)//��λ:0-3,�Զ�ģʽ������0
{
	I2CStart();
	I2CWriteByte(0xC0+addr);		//�����׵�ַ C0 C1 C2 C3
	I2CAsk();
}
void Tm1636_display(u16 num)
{
	I2CWriteByte(num);
	I2CAsk();
}
void Tm1636_discon(u8 light)//����:0-7
{
	I2CStop();
	I2CStart();
	I2CWriteByte(0x88+light);//8F���� 88-8F 
	I2CAsk();
	I2CStop();
}
void display_4seg(u8 G1,u8 G2,u8 G3,u8 G4)//��λ�������ʾ
{
	Tm1636_write_display(1);
	Tm1636_set_segbit(0);
	Tm1636_display(G1);
	Tm1636_display(G2);
	Tm1636_display(G3);
	Tm1636_display(G4);
	Tm1636_discon(6);
}
void display_1seg(u8 seg_num,u8 segbit,u8 light)//1λ�������ʾ
{
	Tm1636_write_display(0);
	Tm1636_set_segbit(segbit);
	Tm1636_display(seg_num);
	Tm1636_discon(light);
}
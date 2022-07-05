#ifndef		__TM3637_H
#define		__TM3637_H
#include <STC8.H>

#define u8 unsigned char
#define u16 unsigned int

sbit DIO = P1^4;
sbit CLK = P1^5;

extern void I2CStart();
extern void I2CAsk();
extern void I2CStop();
extern void I2CWriteByte(u8 dat);
extern void Tm1636_write_display(u8 mode);//1:�Զ�;0;�̶�
extern void Tm1636_set_segbit(u8 addr);//��λ0-3,�Զ�ģʽ������0	
extern void Tm1636_display(u16 num);
extern void Tm1636_discon(u8 light);//����0-7
extern void display_4seg(u8 G1,u8 G2,u8 G3,u8 G4,u8 light);//4λ�������ʾ	
extern void display_1seg(u8 seg_num,u8 segbit,u8 light);//1λ�������ʾ

#endif
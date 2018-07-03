#include "reg52.h"

typedef unsigned char u8;
typedef unsigned int u16;

sbit IRIN=P3^2; // ���������

u8 IrValue[6];  // ��������
u8 Time;        // ��ʱ�ݴ�
u8 DisplayData[8];  // ��ʾ���ݻ���


u8 code smgduan[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0X76};
// 0��1��2��3��4��5��6��7��8��9��A��b��C��d��E��F��H����ʾ��

// ��ʱ������i=1ʱ����Լ��ʱ10us
void delay(u16 i) {
	while(i--);	
}


// �������ʾ����
void DigDisplay() {
    
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 // λѡ��ѡ�����������ܣ�
		{
            case(0):
                P2 = 0; break; // ��ʾ�� 1 λ
            case(1):
                P2 = 1; break; // ��ʾ�� 2 λ
            case(2):
                P2 = 2; break; // ��ʾ�� 3 λ
            case(3):
                P2 = 3; break; // ��ʾ�� 4 λ
            case(4):
                P2 = 4; break; // ��ʾ�� 5 λ
			case(5):
                P2 = 5; break; // ��ʾ�� 6 λ
			case(6):
				P2 = 6; break; // ��ʾ�� 7 λ
			case(7):
				P2 = 7; break; // ��ʾ�� 8 λ	
		}
		P0 = DisplayData[i]; // ��������
		delay(100); // ���һ��ʱ��ɨ��	
		P0 = 0x00;    // ����
	}		
}


// ��ʼ�������߽���
void IrInit() {
    
	IT0=1;//�½��ش���
	EX0=1;//���ж�0����
	EA=1;	//�����ж�

	IRIN=1;//��ʼ���˿�
}


// ������
void main() {
    
	IrInit();
	while(1) {
        DisplayData[0] = smgduan[IrValue[0]/16];    // �û����λ�߰��ֽ�
        DisplayData[1] = smgduan[IrValue[0]%16];    // �û����λ�Ͱ��ֽ�
        DisplayData[2] = smgduan[IrValue[1]/16];    // �û����λ�߰��ֽ�
        DisplayData[3] = smgduan[IrValue[1]%16];    // �û����λ�Ͱ��ֽ�
		DisplayData[5] = smgduan[IrValue[2]/16];    // ������߰��ֽ�
		DisplayData[6] = smgduan[IrValue[2]%16];    // ������Ͱ��ֽ�
		DisplayData[7] = smgduan[16];
	    DigDisplay();		
	}		
}

// ��ȡ������ֵ���жϺ���
void ReadIr() interrupt 0 {
    
	u8 j,k;
	u16 err;
	Time=0;					 
	delay(700);	//7ms
	if(IRIN==0)		//ȷ���Ƿ���Ľ��յ���ȷ���ź�
	{	 
		
		err=1000;				//1000*10us=10ms,����˵�����յ�������ź�
		/*������������Ϊ����ѭ���������һ������Ϊ�ٵ�ʱ������ѭ������ó�������ʱ
		�������������*/	
		while((IRIN==0)&&(err>0))	//�ȴ�ǰ��9ms�ĵ͵�ƽ��ȥ  		
		{			
			delay(1);
			err--;
		} 
		if(IRIN==1)			//�����ȷ�ȵ�9ms�͵�ƽ
		{
			err=500;
			while((IRIN==1)&&(err>0))		 //�ȴ�4.5ms����ʼ�ߵ�ƽ��ȥ
			{
				delay(1);
				err--;
			}
			for(k=0;k<4;k++)		//����4������
			{				
				for(j=0;j<8;j++)	//����һ������
				{

					err=60;		
					while((IRIN==0)&&(err>0))//�ȴ��ź�ǰ���560us�͵�ƽ��ȥ
					{
						delay(1);
						err--;
					}
					err=500;
					while((IRIN==1)&&(err>0))	 //����ߵ�ƽ��ʱ�䳤�ȡ�
					{
						delay(10);	 //0.1ms
						Time++;
						err--;
						if(Time>30)
						{
							return;
						}
					}
					IrValue[k]>>=1;	 //k��ʾ�ڼ�������
					if(Time>=8)			//����ߵ�ƽ���ִ���565us����ô��1
					{
						IrValue[k]|=0x80;
					}
					Time=0;		//����ʱ��Ҫ���¸�ֵ							
				}
			}
		}
		if(IrValue[2]!=~IrValue[3])
		{
			return;
		}
	}			
}


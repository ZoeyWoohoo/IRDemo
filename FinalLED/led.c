#include "reg52.h"
#include <intrins.h>

#define led P0

typedef unsigned char u8;
typedef unsigned int u16;

sbit IRIN=P3^2; // ���������

u8 IrValue[6];  // ��������
u8 Time;        // ��ʱ�ݴ�

u8 m;

// ��ʱ������i=1ʱ����Լ��ʱ10us
void delay(u16 i) {
	while(i--);	
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
    
    u8 data1;
    u8 data2;
    IrInit();
    
    
    led = 0x00;
    delay(50000); //��Լ��ʱ450ms	
    
	while(1) {
		data1 = IrValue[2]/16;    // ������߰��ֽ�
		data2 = IrValue[2]%16;    // ������Ͱ��ֽ�

        if (data1 == 0) {
            switch (data2) {
                case 1:
                    led = 0x01;
                    break;
                case 2:
                    led = 0x02;
                    break;
                case 3:
                    led = 0x04;
                    break;
                case 4:
                    led = 0x08;
                    break;
                case 5:
                    led = 0x10;
                    break;
                case 6:
                    led = 0x01;
                    delay(50000); //��Լ��ʱ450ms
                    // ��led����һλ
                    for(m=0; m<7; m++) {
                        led = _crol_(led, 1);
                        delay(5000); //��Լ��ʱ450ms	
                    }
                    // ��led����һλ
                    for(m=0; m<7; m++) {
                        led = _cror_(led, 1);
                        delay(5000); //��Լ��ʱ450ms
                    }
                    break;
            }
        }
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


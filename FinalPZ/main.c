#include "reg52.h"

typedef unsigned char u8;
typedef unsigned int u16;

sbit IRIN=P3^2; // 红外输出口

u8 IrValue[6];  // 数据数组
u8 Time;        // 计时暂存
u8 DisplayData[8];  // 显示数据缓存


u8 code smgduan[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0X76};
// 0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F、H的显示码

// 延时函数，i=1时，大约延时10us
void delay(u16 i) {
	while(i--);	
}


// 数码管显示函数
void DigDisplay() {
    
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 // 位选，选择点亮的数码管，
		{
            case(0):
                P2 = 0; break; // 显示第 1 位
            case(1):
                P2 = 1; break; // 显示第 2 位
            case(2):
                P2 = 2; break; // 显示第 3 位
            case(3):
                P2 = 3; break; // 显示第 4 位
            case(4):
                P2 = 4; break; // 显示第 5 位
			case(5):
                P2 = 5; break; // 显示第 6 位
			case(6):
				P2 = 6; break; // 显示第 7 位
			case(7):
				P2 = 7; break; // 显示第 8 位	
		}
		P0 = DisplayData[i]; // 发送数据
		delay(100); // 间隔一段时间扫描	
		P0 = 0x00;    // 消隐
	}		
}


// 初始化红外线接收
void IrInit() {
    
	IT0=1;//下降沿触发
	EX0=1;//打开中断0允许
	EA=1;	//打开总中断

	IRIN=1;//初始化端口
}


// 主函数
void main() {
    
	IrInit();
	while(1) {
        DisplayData[0] = smgduan[IrValue[0]/16];    // 用户码高位高半字节
        DisplayData[1] = smgduan[IrValue[0]%16];    // 用户码高位低半字节
        DisplayData[2] = smgduan[IrValue[1]/16];    // 用户码低位高半字节
        DisplayData[3] = smgduan[IrValue[1]%16];    // 用户码低位低半字节
		DisplayData[5] = smgduan[IrValue[2]/16];    // 数据码高半字节
		DisplayData[6] = smgduan[IrValue[2]%16];    // 数据码低半字节
		DisplayData[7] = smgduan[16];
	    DigDisplay();		
	}		
}

// 读取红外数值的中断函数
void ReadIr() interrupt 0 {
    
	u8 j,k;
	u16 err;
	Time=0;					 
	delay(700);	//7ms
	if(IRIN==0)		//确认是否真的接收到正确的信号
	{	 
		
		err=1000;				//1000*10us=10ms,超过说明接收到错误的信号
		/*当两个条件都为真是循环，如果有一个条件为假的时候跳出循环，免得程序出错的时
		侯，程序死在这里*/	
		while((IRIN==0)&&(err>0))	//等待前面9ms的低电平过去  		
		{			
			delay(1);
			err--;
		} 
		if(IRIN==1)			//如果正确等到9ms低电平
		{
			err=500;
			while((IRIN==1)&&(err>0))		 //等待4.5ms的起始高电平过去
			{
				delay(1);
				err--;
			}
			for(k=0;k<4;k++)		//共有4组数据
			{				
				for(j=0;j<8;j++)	//接收一组数据
				{

					err=60;		
					while((IRIN==0)&&(err>0))//等待信号前面的560us低电平过去
					{
						delay(1);
						err--;
					}
					err=500;
					while((IRIN==1)&&(err>0))	 //计算高电平的时间长度。
					{
						delay(10);	 //0.1ms
						Time++;
						err--;
						if(Time>30)
						{
							return;
						}
					}
					IrValue[k]>>=1;	 //k表示第几组数据
					if(Time>=8)			//如果高电平出现大于565us，那么是1
					{
						IrValue[k]|=0x80;
					}
					Time=0;		//用完时间要重新赋值							
				}
			}
		}
		if(IrValue[2]!=~IrValue[3])
		{
			return;
		}
	}			
}


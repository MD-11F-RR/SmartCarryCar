#include <reg52.h>

/*
void serial_break(void) interrupt 4 {
  RI = 0;
  flag = 1;
}
*/


void serial_init() {
	//T2MOD = 0X02;
  RCLK = 1;				//设置作为波特率发生器
	TCLK = 1;				//设置作为波特率发生器
	TH2 = 0xfF;			//波特率设置，此处为9600，下同
	TL2 = 0XDC;
	//T2CON = 0X34;
	//ffdc
	RCAP2H = 0xFF;	//波特率重载，此处为9600，下同
	RCAP2L = 0xDC;
	//使用定时器2的时候请格外注意
	//波特率计算公式
	//波特率=晶振频率/(32*(65536-n))
	//将n赋值给计数器

	TR2 = 1;
  //以上设置定时器2，先行测试。
  SM0 = 0;        //设置串口工作方式1
  SM1 = 1;        //同上
	REN = 1;        //允许串口接收
  ES = 1;         //串口中断开启
}

void serial_shutdown(){
  TR2 = 0;
  REN = 0;
  ES = 0;
}

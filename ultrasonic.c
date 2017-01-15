#include <reg52.h>

int ultrasonic_distance = 0;
unsigned int ultrasonic_time = 0;

void ulotasonic_init() {
  ET0 = 0;			//不允许中断，只计时
  TMOD &= 0XF0;
  TMOD |= 0X09;
  TR0 = 1;			//定时器0开始运行，待有高电平就开始计时
	//注意防止颜色传感器干扰
  //定时器0初始化

  IT0 = 1;		//下降沿触发
  EX0 = 1;		//允许中断
  //外部中断0初始化

  ultrasonic_time = 0;
  ultrasonic_distance = 0;
}

void ultrasonic_shutdown() {
  ET0 = 0;
  EX0 = 0;
  TR0 = 0;
}

void ultrasonic_T0interrupt() {
  EA = 0;
  TR0 = 0;
  //防止中断嵌套

  ultrasonic_time = 0;
  ultrasonic_distance = 0;
  //复位值

  ultrasonic_time = TH0 * 256 + TL0;
  //时间转换
  TH0 = 0;
  TL0 = 0;

  ultrasonic_distance = (ultrasonic_time * 1.87) / 100;
  //距离计算公式，输出单位：厘米

  TR0 = 1;
  EA = 1;
  //恢复中断
}

int ultrasonic_return() {
  if (!ultrasonic_distance) {
    return ultrasonic_distance;
  }
  else{
    return 9999;
  }
}

int ultrasonic_check(){
	ulotasonic_init();
	ultrasonic_time = 0;

}

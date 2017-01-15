#include <reg52.h>
#define COLOR_TIME 19456

unsigned int color_count = 0;

float color_pred, color_pgreen, color_pblue;
//白平衡参数，通过校准调节

//int color_true = 0;
//转换为256^3色的结果

int *color_avgP = 0;
//指针，指向储存数组，默认指向0，以免误操作

bit color_calibrated = 0;

sbit s0 = P2^3;
sbit s1 = P2^2;
sbit s2 = P2^6;
sbit s3 = P2^7;

void color_init(){
  s0 = 1;
  s1 = 1;
  {
    TMOD &= 0xF0;
    TMOD |= 0x01;
    //设置模式
    TH0 = COLOR_TIME >> 8;
    TL0 = COLOR_TIME;
    //设置定时器时间
    PT0=1;
    //优先级
    ET0=1;
    //允许定时器0中断
    TR0=1;
    //打开定时器0
  }//T0 initate
  {
    PX0 = 0;
    //优先级

    IT0 = 1;
    //下降沿触发

    EX0 = 1;
    //开外部中断0
  }//INT0 interrupt init
}
//初始化函数

void color_shutdown() {
  ET0 = 0;
  //关闭定时器0中断
  TR0 = 0;
  //关闭定时器0

  EX0 = 0;
  //关闭外部中断

	s0 = 0;
	s1 = 0;
	s2 = 0;
	s3 = 0;
	//关闭四个开关，防止干扰其他用到INT0中断的传感器
}
//停止函数

void color_T0interrupt() {
  static unsigned char color_stat = 0;

  TR0 = 0;
  //关掉定时器

  TH0 = COLOR_TIME >> 8;
  TL0 = COLOR_TIME;
  //重置定时器时间
/*
  switch (color_stat) {
    case 1:
    s2 = 0; s3 = 0; color_count = 0;
    //第一次用于选择红色滤镜，无获取颜色值，脉冲数清零
    break;

    case 2:
    color_count_store = color_count; s2 = 0; s3 = 1; color_count = 0;
    //获取红色数值并选择蓝色滤镜，脉冲数清零
    color_true[0] = (int)(color_count_store * color_pred);
    break;

    case 3:
    color_count_store = color_count; s2 = 1; s3 = 1; color_count = 0;
    //获取蓝色数值并选择绿色滤镜，脉冲数清零
    color_true[1] = (int)(color_count_store * color_pgreen);
    break;

    case 4:
    color_count_store = color_count; s2 = 1; s3 = 0; color_stat = 0;
    //获取绿色数值并选择无滤镜，脉冲数清零
    color_true[2] = (int)(color_count_store * color_pblue);
    break;
  }
  */
	if (color_avgP != 0) {
		for (color_stat = 0; color_stat < 30; color_stat++, color_avgP++) {
			switch (color_stat % 4) {
				case 0:
				s2 = 0; s3 = 0;
				color_count = 0;
				//第一次用于选择红色滤镜，无获取颜色值，脉冲数清零
				break;

				case 1:
				s2 = 0; s3 = 1;
				*color_avgP++ = color_count;
				color_count = 0;
				//获取红色数值并选择蓝色滤镜，脉冲数清零
				break;

				case 2:
				s2 = 1; s3 = 1;
				*color_avgP++ = color_count;
				color_count = 0;
				//获取蓝色数值并选择绿色滤镜，脉冲数清零
				break;

				case 3:
				s2 = 1; s3 = 0;
				*color_avgP++ = color_count;
				color_stat = 0;
				//获取绿色数值并选择无滤镜，脉冲数清零
				break;
			}
		}
	}



  TR0 = 1;
  //重新打开定时器
}
//写在定时器中断里面

void color_INT0interrupt(){
  ++color_count;
}
//写在外部中断里面

void color_calibrate(){
/*
  color_pred = 255.0 / color_count_store[0];
  color_pgreen = 255.0 / color_count_store[1];
  color_pblue = 255.0 / color_count_store[2];
  //校准256色阶
  color_calibrated = 1;
  */
  int color_avg[30] = (0);
  //用于储存
  char color_spot = 0;
  //下面for循环时使用，3个一组，定位

  color_avgP = &color_avg[0];
  //给指针定位
  color_init();
  while(color_avg[29] == 0);
  color_shutdown();

  for (color_avgP = &color_avg[0]; color_avgP < &color_avg[30]; color_avgP++) {
    switch (color_spot) {
      case 0:
      color_pred += *color_avgP;
      color_spot++;
      break;

      case 1:
      color_pgreen += *color_avgP;
      color_spot++;
      break;

      case 2:
      color_pblue = *color_avgP;
      color_spot = 0;
      break;

      default:
      break;
    }
  }
  color_pred = 2550.0 / color_pred;
  color_pblue = 2550.0 / color_pblue;
  color_pgreen = 2550.0 / color_pgreen;
  //白平衡校准

  color_calibrated = 1;
  color_avgP = 0;
  //取消指针定位，防止误触发函数造成野指针

}
//校准



int color_printf(char colors){
  int color_avg[30];
  int color_true[3];
  char color_spot = 0;

  color_avgP = &color_avg[0];
  color_init();
  while(color_avg[29] == 0);
  color_shutdown();

  for (color_avgP = &color_avg[0]; color_avgP < &color_avg[30]; color_avgP++) {
    switch (color_spot) {
      case 0:
      color_true[0] += *color_avgP;
      color_spot++;
      break;

      case 1:
      color_true[1] += *color_avgP;
      color_spot++;
      break;

      case 2:
      color_true[2] += *color_avgP;
      color_spot = 0;
      break;

      default:
      break;
    }
    color_true[0] *= color_pred / 10;
    color_true[1] *= color_pgreen / 10;
    color_true[2] *= color_pblue / 10;
  }
  switch (colors) {
    case 1:
    return color_true[0];
    break;

    case 2:
    return color_true[1];
    break;

    case 3:
    return color_true[2];
    break;

    default:
    return 9999;
    break;
  }

  color_avgP = 0;
  //清指针
}


char color_check(){
  int color_avg[30];
  int color_true[3];
  char color_spot = 0;

  color_avgP = &color_avg[0];
  color_init();
  while(color_avg[29] == 0);
  color_shutdown();

  for (color_avgP = &color_avg[0]; color_avgP < &color_avg[30]; color_avgP++) {
    switch (color_spot) {
      case 0:
      color_true[0] += *color_avgP;
      color_spot++;
      break;

      case 1:
      color_true[1] += *color_avgP;
      color_spot++;
      break;

      case 2:
      color_true[2] += *color_avgP;
      color_spot = 0;
      break;

      default:
      break;
    }
    color_true[0] *= color_pred / 10;
    color_true[1] *= color_pgreen / 10;
    color_true[2] *= color_pblue / 10;
  }
	return 'e';
}

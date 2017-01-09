#include <reg52.h>
//#include "serial.h"
#include <stdio.h>
#define COLOR_TIME 19456


unsigned int color_count_store[3] = {0,0,0};
//RGB for color[0-2];
unsigned int color_count = 0;
unsigned char stat = 0;

unsigned int pred, pgreen, pblue;
//白平衡参数，通过校准调节

unsigned int color_true[3] = {0,0,0};
//转换为256^3色的结果

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

void color_T0interrupt() {
  TR0 = 0;
  //关掉定时器
  TH0 = COLOR_TIME >> 8;
  TL0 = COLOR_TIME;
  //重置定时器时间
  ++stat;

  switch (stat) {
    case 1:
    s2 = 0; s3 = 0; color_count = 0;
    //第一次用于选择红色滤镜，无获取颜色值，脉冲数清零
    break;

    case 2:
    color_count_store[0] = color_count; s2 = 0; s3 = 1; color_count = 0;
    //获取红色数值并选择蓝色滤镜，脉冲数清零
    break;

    case 3:
    color_count_store[2] = color_count; s2 = 1; s3 = 1; color_count = 0;
    //获取蓝色数值并选择绿色滤镜，脉冲数清零
    break;

    case 4:
    color_count_store[1] = color_count; s2 = 1; s3 = 0; stat = 0;
    //获取绿色数值并选择无滤镜，脉冲数清零
    break;

  }

  color_true[0] = color_count_store[0] * pred;
  color_true[1] = color_count_store[1] * pgreen;
  color_true[2] = color_count_store[2] * pblue;

  TR0 = 1;
  //重新打开定时器
}

void color_INT0interrupt(){
  ++color_count;
}

void color_calibrate(){
  pred = 255 / color_count_store[0];
  pgreen = 255 / color_count_store[1];
  pblue = 255 / color_count_store[2];
  //校准256色阶
  color_calibrated = 1;
}

void color_printf(){
  if (color_calibrated == 1) {
    printf("R = %d\n",color_true[0]);
    printf("G = %d\n",color_true[1]);
    printf("B = %d\n",color_true[2]);
  }
  else{
    printf("not calibrated\n");
  }
}

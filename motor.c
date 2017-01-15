/*
tested version.
it works now.
renember to rezero the motor when it
doesn't stop while using stop function

Author: Martin.Wang
GDUT.ROBOT
*/

#define CYCLE 0xdc00
#define MID_POINT 0xfa9a
#include <AT89X52.H>
#include "motor.h"

sbit Lmotor = P1^1;
sbit Rmotor = P1^0;

//int CYCLE = 0xdc00;
//周期，10ms

unsigned int pwm_motorL = MID_POINT;
unsigned int pwm_motorR = MID_POINT;

//这两个变量直接控制左右电机pwm值
unsigned int step = 25;
//100us的步进，默认值

bit stat_motor = 1;
//状态量，左右电机的flag

bit pwm_high = 0;
//高电平指示

bit run_motor = 0;
//电机运行状态量，用于在shutdown时能够顺利的停止电机


void motor_init(){
  TMOD &= 0X0F;
  TMOD |= 0x10;
  //定时器选项

  TR1 = 1;
  ET1 = 1;
  //定时器开始并允许中断
  //使用定时器

  Lmotor = 0;
  Rmotor = 0;
  //电机初始化，两个电机控制信号置零


}
//初始化

void motor_shutdown() {
  motor_stop();
  run_motor = 1;

  while(run_motor != 0);
  //当执行一个中断周期后再停止计时器

  TR0 = 0;
  ET0 = 0;
  Lmotor = 0;
  Rmotor = 0;
}
//停止计时器相关，电机置零

void motor_speed(char speed) {
  switch (speed) {
    case 0:
    step = 10;
    break;

    case 1:
    step = 25;
    break;

    case 2:
    step = 50;
    break;

    case 3:
    step = 100;
    break;

    default:
    step = 25;
    }
}
//速度选择函数

void motor_stop(){
  pwm_motorL = MID_POINT;
  pwm_motorR = MID_POINT;
}
//电机停止

void motor_ccw() {
  pwm_motorL = MID_POINT + step;
  pwm_motorR = MID_POINT + step;
}
//电机逆时针

void motor_left() {
  pwm_motorL = MID_POINT;
  pwm_motorR = MID_POINT + step;
}
//左转弯，左侧电机停止，右侧电机以原先速度转动

void motor_right() {
  pwm_motorL = MID_POINT - step;
  pwm_motorR = MID_POINT;
}
//右转弯，右侧电机停止，左侧电机以原先速度转动

void motor_cw() {
  pwm_motorL = MID_POINT - step;
  pwm_motorR = MID_POINT - step;
}
//顺时针

void motor_backward(){
  pwm_motorL = MID_POINT + step;
  pwm_motorR = MID_POINT - step;
}
//向后转

void motor_forward () {
  pwm_motorL = MID_POINT - step;
  pwm_motorR = MID_POINT + step;
}
//前进

void motor_interrupt(){
  if (stat_motor == 1) {

    if (pwm_high == 0) {
      pwm_high = 1;
      Lmotor = 1;
      TH1 = pwm_motorL >> 8;
      TL1 = pwm_motorL;
    } else {
      pwm_high = 0;
      Lmotor = 0;
      TH1 = (65536 - pwm_motorL + CYCLE) >> 8;
      TL1 = 65536 - pwm_motorL + CYCLE;
      Lmotor = 0;
      stat_motor = 0;

      run_motor = 1;
    }

  } else {
    if (pwm_high == 0) {
      pwm_high = 1;
      Rmotor = 1;
      TH1 = pwm_motorR >> 8;
      TL1 = pwm_motorR;
    } else {
      pwm_high = 0;
      Rmotor = 0;
      TH1 = (65536 - pwm_motorR + CYCLE) >> 8;
      TL1 = 65536 - pwm_motorR + CYCLE;
      stat_motor = 1;

      run_motor = 0;
    }
  }
}
//放在中断函数里面，可以用函数指针

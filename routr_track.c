#include <reg52.h>
#include "motor.h"

void route_track(){
	if(P0 & 0x60){
	  motor_forward();
	}//全速前进

	else if(P0 & 0x30){
	  motor_right();
	}//向右转弯

	else if(P0 & 0xc0){
	  motor_left();
	}//向左转弯

	else if(P0 & 0x20){
	  motor_cw();
	}//顺时针微调车身

	else if(P0 & 0x40){
	 motor_ccw ();
	}
	//逆时针微调车身

  else{
    motor_forward();
  }

}

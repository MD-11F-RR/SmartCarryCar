#include <reg52.h>
#include <stdio.h>
#include "serial.h"
#include "motor.h"
#include "route_track.h"
#include "color_detect.h"

bit newinfo = 0;
char info;
void serialdeal() {
	switch (info) {
		case 'p':
		printf("RED:%d\nGREEN:%d\nBLUE:%d\n",(int)color_printf(1),(int)color_printf(2),(int)color_printf(3));
		break;

		case 'c':
		color_calibrate();
		break;
	}
}

int main() {
	serial_init();
	color_init();
	EA = 1;
	while(1){
	if(newinfo == 1){
		serialdeal();
		newinfo = 0;
		}
	}
	return 0;
}

void T1interrupt() interrupt 3 {
	EA = 0;
  motor_interrupt();
	EA = 1;
}

void T0interrupt() interrupt 1 {
	EA = 0;
  color_T0interrupt();
	EA = 1;
}

void INT0interrupt() interrupt 0 {
	EA = 0;
  color_INT0interrupt();
	EA = 1;
}


void serial_interrupt() interrupt 4 {
EA = 0;
	if (RI) {
		RI = 0;
		newinfo = 1;
		info = SBUF;
  }


EA = 1;
}

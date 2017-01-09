#include <reg52.h>
#include <stdio.h>
#include "serial.h"
#include "motor.h"
#include "route_track.h"
#include "color_detect.h"


int main() {

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
  color_INT0interrupt();
}


void serial_interrupt() interrupt 4 {

	if (RI) {
    switch (SBUF) {

      case 'c':
			print('a');
      break;

      case 'o':
      break;

      default:
			break;

    }
  }
  else{;}
  RI = TI = 0;

}

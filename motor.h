#ifndef MOTOR_H
#define MOTOR_H


void motor_init();
void motor_shutdown();
void motor_speed(char speed);
void motor_stop();
void motor_left();
void motor_right();
void motor_forward();
void motor_backward();
void motor_cw();
void motor_ccw();
void motor_interrupt();

#endif

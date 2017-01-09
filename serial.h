#ifndef SERIAL_H
#define SERIAL_H


//UART1 发送串口数据
void UART1_SendData(char dat){
    ES=0;           //关串口中断
    SBUF=dat;
    while(!TI);   //等待发送成功
    TI=0;           //清除发送中断标志
    ES=1;           //开串口中断
}

//UART1 发送字符串
void UART1_SendString(char *s){
    while(*s)//检测字符串结束符
    {
        UART1_SendData(*s++);//发送当前字符
    }
}

//重写putchar函数
char putchar(char c){
    UART1_SendData(c);
    return c;
}

void serial_init();
void serial_shutdown();

void print(char select) {
  switch (select) {
    case 'a':
    printf("hello world\n");
    break;

    default:
    break;
  }
}

#endif

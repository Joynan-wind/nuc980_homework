#ifndef USR_H_
#define USR_H_ //插旗

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#define SERVER_PORT 8888 //端口号不能发生冲突,不常用的端口号通常大于 5000
#define on 0
#define TRUE 1
#define off 1
#define FALSE 0

//buffer for uart
char Txbuf[101];
char Rxbuf[101];
int tty_fd;
/*----------------------------设备信息----------------------------*/
struct LEDinfo
{
    int val;
    char sta[2];
    int lcd_val;
} LED0;

/*------------------------IO口设置-------------------------------*/
char gpio_path[100];
int gpio_config(const char *attr, const char *val);
int gpio_set(const char *gpio_num, const char *gpio_val);

/*-----------------------------网络通信----------------------------*/
void *server(void* arg);
/*---------------------------串口通信------------------------------*/
int uart_send(int fd,void* buf);
int uart_receive(int fd,char* buf);
void reset_tty_atexit(void);
void reset_tty_handler(int signal);
int open_port(const char *portname);
void *uart(void*arg);

#endif
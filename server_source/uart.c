#include "usr.h"


//terminal setting value
static struct termios newtios,oldtios;
static int saved_portfd=-1;   //serial port fd

//rest fuction at exit
void reset_tty_atexit(void) 
{
    if(saved_portfd != -1)
    {
        tcsetattr(saved_portfd,TCSANOW,&oldtios);//reset tty with oldtios
    }
}

//cleanup signal handler
void reset_tty_handler(int signal)
{
    if(saved_portfd != -1)
    {
        tcsetattr(saved_portfd,TCSANOW,&oldtios);
    }
    _exit(EXIT_FAILURE);
}

//open uart serial port and setting
int open_port(const char *portname)
{
    struct sigaction sa;
    int portfd;
    printf("opening serial port:%s\n",portname);
    //open serial port 
    //O_NOCTTY 标志，该标志用于告知系统/dev/ttySTM2 它不会成为进程的控制终端。
    if(0 >(portfd=open(portname,O_RDWR|O_NOCTTY)))
    {
        printf("open serial port %s fail\n",portname);
        return portfd;
    }

    //get serial part parnms ,save in "oldtios"
    tcgetattr(portfd,&newtios);
    memcpy(&oldtios,&newtios,sizeof(newtios));

    //configure new values,setting port parnms
    cfmakeraw(&newtios);                //client fuction make raw.setting rew mode
    newtios.c_iflag |=IGNPAR;           //"IGNPAR" stand for ignore parity on input
    newtios.c_oflag &= ~(OPOST|ONLCR|OLCUC|OCRNL|ONOCR|ONLRET|OFILL);
    newtios.c_cflag=CS8|CLOCAL|CREAD;    //依次代表8位数据位，忽略调制解调控制线，接收使能
    newtios.c_cc[VMIN]=1;                 //block until 1 char received
    newtios.c_cc[VTIME]=0;                 //no inter-character timer
    //baund
    cfsetospeed(&newtios,B9600);
    cfsetispeed(&newtios,B9600);
    //register cleanup stuff
    atexit(reset_tty_atexit);
    memset(&sa,0,sizeof sa);
    sa.sa_handler = reset_tty_handler;
    sigaction(SIGHUP,&sa,NULL);
    sigaction(SIGINT,&sa,NULL);
    sigaction(SIGPIPE,&sa,NULL);
    sigaction(SIGTERM,&sa,NULL);
    //apply modified termios
    saved_portfd=portfd;
    tcflush(portfd,TCIOFLUSH);//清空緩衝區尚未處理的輸入輸出數據
    tcsetattr(portfd,TCSADRAIN,&newtios);//阻塞生效（配置在所有寫入fd的輸出都傳輸完畢後生效）
    return portfd;   //返回portfd，方便對端口讀寫操作
}

int uart_send(int fd,void* buf)
{
    int portfd =fd;
    int rev;

    rev = write(portfd,buf,13);
    if(rev>0)
    {
        printf("info %d update success!\n",rev);
    }
    else
    {
        printf("send error!\n");
    }
}

int uart_receive(int fd,char* buf)
{
    int portfd=fd;
    int rev; 
    memset(buf,0,100);
    rev=read(portfd,buf,100);
    if(rev>0)
    {
        printf("screen buttn %s",buf);
    }
}
void *uart(void*arg)
{    

    char *dev[10]={"/dev/ttyS1"};

    tty_fd=open_port(dev[0]);

    while(1)
    {
        uart_receive(tty_fd,Rxbuf);
        if(0 == strncmp("off",Rxbuf,3))
        {
            LED0.val=off;
            LED0.lcd_val=off;
        }
        if(0 == strncmp("on",Rxbuf,2))
        {
            LED0.val=on;
            LED0.lcd_val=on;
        }
    }
}
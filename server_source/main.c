/*本次课设的入口函数
小组成员：电子1194 翁宗南 陈富烨 陈雨康
硬件：NUC980开发板，TJC4827K_011串口屏
课设功能：网络通信实现点灯并串口屏上显示并控制led状态
*/
#include "usr.h"

int main(void)
{
    //初始化参数、设置变量
    LED0_set.val=0;
    PWM02_set.period=300000;
    int rev; 
    int i=0;
    char *dev[10]={"/dev/ttyS1"};
    tty_fd=open_port(dev[0]);
    pthread_t threads[10]; //threads id

    //两个通信线程，保持与客户端、串口屏的通信
    pthread_create(&threads[0],NULL,server,NULL);
    pthread_create(&threads[1],NULL,uart_process,NULL);
    
    //根据数据改变设备状态
    while(1)
    {   
        //LED0
        sprintf(LED0_set.sta,"%d",LED0_set.val);
        gpio_set("32",LED0_set.sta);
        //同步开发板上LED的状态到串口屏
        if(LED0_set.val != LED0_set.lcd_val)
        {   
            memset(Txbuf,0,100);
            sprintf(Txbuf,"click b0,0\xff\xff\xff");
            printf("%s",Txbuf);           //调试用：打印串口发送值
            uart_send(tty_fd,Txbuf);
            sleep(1);

        }
        //PWM02
        if(PWM02_set.rate>=0 && PWM02_set.rate<=100)                                 //加上后不能运行？
        {
            PWM02_set.duty_cycle=PWM02_set.period*PWM02_set.rate/100;//计算duty_cycle的值
            pwm_set(PWM02_set.period,PWM02_set.duty_cycle,1);

            //调试信息代码
            i=1+i;
            if(i==2000)
            {
                i=0;
                printf("set_rate:%d\nreal_sta:%s\n",PWM02_set.rate,PWM02_info);
            }
            if(PWM02_set.lcd_rate!=PWM02_set.rate)
            {
                memset(Txbuf,0,100);
                sprintf(Txbuf,"moto.val=%d\xff\xff\xff",PWM02_set.rate);
                printf("%s",Txbuf);         //调试用：打印串口发送值
                uart_send(tty_fd,Txbuf);
                uart_send(tty_fd,"click moto,0\xff\xff\xff");
                //usleep(500);
            }
        }
        
    }

    return 0;
}
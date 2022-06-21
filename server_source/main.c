/*本次课设的入口函数
小组成员：电子1194 翁宗南 陈富烨 周雨康
硬件：NUC980开发板，TJC4827K_011串口屏
课设功能：网络通信实现点灯并串口屏上显示并控制led状态
*/
#include "usr.h"

int main(void)
{
    LED0.val=0;
    int rev; 
    pthread_t threads[10]; //threads id

    
    pthread_create(&threads[0],NULL,server,NULL);
    pthread_create(&threads[1],NULL,uart,NULL);
    
    while(1)
    {
        sprintf(LED0.sta,"%d",LED0.val);
        gpio_set("32",LED0.sta);
        if(LED0.val != LED0.lcd_val)
        {   
            memset(Txbuf,0,100);
            sprintf(Txbuf,"\x63\x6C\x69\x63\x6B\x20\x62\x30\x2C\x30\xff\xff\xff");
            printf("%s",Txbuf);
            uart_send(tty_fd,Txbuf);
            sleep(2);
        }
    }

    return 0;
}
/*封装一个控制指定gpio输出高低电平的函数
usage：函数名（指定gpio编号，电平）
*/
#include "usr.h"
static rev=1;

int gpio_config(const char *attr,const char *val)
{
    char file_path[100];
    int len;
    int fd;
    sprintf(file_path,"%s/%s",gpio_path,attr);
    if(0 > (fd = open(file_path,O_WRONLY)))
    {
        printf("%s",file_path);
        perror("open1 error");
        return fd;
    }
    len = strlen(val);
    if(len != write(fd,val,len))
    {
        printf("%s",file_path);
        perror("write1 error");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}
int gpio_set(const char * gpio_num,const char *gpio_val)
{
    char file_path[100];
    int fd;
    sprintf(gpio_path,"/sys/class/gpio/gpio%s",gpio_num);
    if(access(gpio_path,F_OK))
    {
        int len;
        if(0 > (fd = open("/sys/class/gpio/export",O_WRONLY)))
        {
            perror("open2 error");
            exit(-1);
        }
        if(0 > write(fd,gpio_num,strlen(gpio_num)))
        {
            perror("write2 error");
            close(fd);
            exit(-1);
        }
        close(fd);
    }
    if(rev)
    {rev=gpio_config("direction", "out");}
    /* 极性设置 */
    gpio_config("value",gpio_val);
    /* 配置为非中断方式 */
    gpio_config("edge", "none");
    return 0;
}
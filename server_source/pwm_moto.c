/*
封装一个设置PWM的函数，用以控制电机/舵机
设置占空比、PWM频率、极性（忽略）、使能/失能
*/
#include "usr.h"
//PWM0通道2 chip2 channel 0
//属性文件：polarity、period、duty_cycle、enable

//pwm属性文件设置
int pwm_config(const char *attr,const char *val)
{
    char file_path[100];
    char current_val[15];
    int len;
    int fd;
    int rev;
    //利用字符拼接赋值给变量完整的路径名
    sprintf(file_path,"%s/%s",pwm_path,attr);
    //printf("%s\n",file_path);            //调试用
    //打开文件
    if(0>(fd=open(file_path,O_RDWR)))
    {
        perror("pwm_config open error");
        return fd;
    }
    //读取原属性数值
    memset(current_val,0,sizeof(current_val));
    do{
        if(0 >(rev=read(fd,&current_val,15)))
        {
            perror("read current error");
        }
        }while(rev);
    //如果需要改变数值，写入val
    if((0!=strcmp(current_val,val)))
    {
        len = strlen(val);
        if(len != write(fd,val,len))
        {
            perror("pwm_config write error");
            return -1;
        }
    }
    //关闭文件
    close(fd);
    return 0;
}

//pwm总设置
int pwm_set(int period,int duty_cycle,int SW)
{   
    char channel[]="0";
    char c_period[40];
    char c_duty_cycle[40];
    char c_SW[40];

    //数据由int型转换为字符型
    sprintf(c_period,"%d",period);
    sprintf(c_duty_cycle,"%d",duty_cycle);
    sprintf(c_SW,"%d",SW);
    
    /*调试信息
    printf("c_period=%s\n",c_period);    
    printf("c_duty_cycle=%s\n",c_duty_cycle);
    printf("c_SW=%s\n",c_SW);
    */
    
    //如果没有开启通道，导出PWM通道（本课设为PWM0通道2）
    if(access(pwm_path,F_OK))
    {
        int fd;
        int len;
        fd=open(pwm_export,O_WRONLY);
        if(fd<0)
        {
            printf("%d",fd);
            perror("pwm export open error");
            exit(-1);
        }
        len=strlen(channel);
        if(len!=write(fd,channel,len))
        {
            perror("pwm export write error");
            close(fd);
            exit(-1);
        }
        close(fd);
    }
    //调用pwm_config设置属性文件，period，duty_cycle
    pwm_config("period",c_period);//300us=300*1000ns;
    pwm_config("duty_cycle",c_duty_cycle);//占空比=1/3
    pwm_config("enable",c_SW);//开关
    sprintf(PWM02_info,"Per:%s,Duty:%s,En:%s",c_period,c_duty_cycle,c_SW);
    return 0;
}

/*调试用
int main(int argc, char *argv[])
{
    int a,b,c;
    sscanf(argv[1],"%6d",&a);
    sscanf(argv[2],"%6d",&b);
    sscanf(argv[3],"%d",&c);
    printf("a=%d\n",a);
    printf("b=%d\n",b);
    printf("c=%d\n",c);
    pwm_set(a,b,c);
    return 0;
}
*/
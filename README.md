# nuc980_homework
 Curriculum design of “Embedded operating system”

## 小组成员
  电子1194 翁宗南 陈富烨 陈雨康
## 课设功能
    网络通信与串口屏控制LED状态，并将信息显示在串口屏上。
## 说明
    本课设分为两个部分，客户端部分为上位机程序源码，服务器部分为NUC980程序源码
    （一）客户端部分
        实现网络通信客户端功能，发送的字符将在服务器端打印。指定字符“on”、“off”将控制LED灯的开关。
    （二）服务器端部分
        接收服务端发送的信息并打印到屏幕，接受到特定字符“on”“off”改变LED的状态，并将LED灯的状态通过串口传送给串口屏显示出来，同时串口屏屏幕按下时也会改变LED灯的状态。

## 各模块函数接口说明
### PWM模块
    > int pwm_set(int period,int duty_cycle,int SW);
    
#### 参数
| 参数       | 说明                  |
| ---------- | --------------------- |
| period     | PWM周期               |
| duty_cycle | 占空比的时长          |
| SW         | 使能开关 1：开  0：关 |
#### 返回值
int整型
成功返回0
失败返回-1 或 退出进程
### GPIO模块
### server模块
### uart模块
### usr头文件
### 主函数
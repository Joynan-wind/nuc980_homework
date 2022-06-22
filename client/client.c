#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define SERVER_PORT 8888          //服务器的端口号
#define SERVER_IP "172.20.10.8" //服务器的 IP 地址，本此课程设计开发板为服务器端

int main(void)
{
    struct sockaddr_in server_addr = {0};   //sockaddr_in 结构体：协议、IP、端口、保留位
    char buf[512];                          //数据缓存区
    int sockfd;                             //套接字描述符
    int ret;                                //函数返回值
    /* 打开套接字，得到套接字描述符 */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //AF_INET->ip4协议族，SOCK_STREAM->双向字节流，0->默认协议
    if (0 > sockfd)
    {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    /* 调用 connect 连接远端服务器 */
    server_addr.sin_family = AF_INET;                      //协议族ip4
    server_addr.sin_port = htons(SERVER_PORT);            //端口号，htons为宏定义
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr); // IP 地址转换为二进制
    ret = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));//连接，参数：套接字描述符、服务器地址（结构体），地址字节
    if (0 > ret)
    {
        perror("connect error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("服务器连接成功...\n\n");
    /* 向服务器发送数据 */
    for (;;)
    {
        // 清理缓冲区
        memset(buf, 0x0, sizeof(buf));
        // 接收用户输入的字符串数据
        printf("Please enter a string: ");
        fgets(buf, sizeof(buf), stdin);
        // 将用户输入的数据发送给服务器
        ret = send(sockfd, buf, strlen(buf), 0);
        if (0 > ret)
        {
            perror("send error");
            break;
        }
        //输入了"exit"，退出循环
        if (0 == strncmp(buf, "exit", 4))
            break;
    }
    close(sockfd);
    exit(EXIT_SUCCESS);
}
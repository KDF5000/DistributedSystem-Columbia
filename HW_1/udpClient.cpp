#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024

int main(){

    //向服务器（特定的ip和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(1234);

    int sock = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
    //发送数据
    char buffer[BUF_SIZE] = {0};
    struct sockaddr_in from_addr;
    socklen_t addr_len = sizeof(from_addr);
    while(1){

        printf("请输入一个字符串:");
        scanf("%s", buffer);
        //发送
        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        int strLen = recvfrom(sock, buffer, BUF_SIZE, 0, (struct sockaddr*)&from_addr, &addr_len);
        buffer[strLen] = 0;
        printf("Message form server: %s\n", buffer);

    }
    close(sock);

    return 0;
}

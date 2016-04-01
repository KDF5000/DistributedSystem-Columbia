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

    //发送数据
    char buffer[BUF_SIZE] = {0};
    while(1){
        //创建套接字
        int sock = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
        //连接
        connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        printf("请输入一个字符串:");
        scanf("%s", buffer);
        //发送
        write(sock, buffer, sizeof(buffer));

        //接受数据
        int  recbytes = read(sock, buffer, sizeof(buffer)-1);
        printf("Message from server: %d %s\n", recbytes,buffer);
        memset(buffer,0, sizeof(buffer));
        close(sock);
    }

    return 0;
}

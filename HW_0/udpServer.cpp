#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define BUF_SIZE 1024
int main(){
    //创建套接字
    int serv_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    //将套接字和IP，端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//自动获取ip地址
    serv_addr.sin_port = htons(1234);
    bind(serv_sock,(struct sockaddr *)&serv_addr, sizeof(serv_addr)); //绑定 ip，端口

    //接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);

    char buffer[BUF_SIZE] = {0};
    while(1){
        int strLen = recvfrom(serv_sock, buffer, BUF_SIZE, 0, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        sendto(serv_sock, buffer, strLen, 0, (struct sockaddr *)&clnt_addr, clnt_addr_size);
    }
    close(serv_sock);
    return 0;
}



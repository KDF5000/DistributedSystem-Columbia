#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024

int main(){
    char filename[100] = {0};
    printf("请输入保存的要保存的文件名: ");
    scanf("%s", filename);
    FILE *fp = fopen(filename, "wb");
    if(fp == NULL){
        printf("打开文件失败，俺任意键退出~");
        system("pause");
        exit(0);
    }
    //向服务器（特定的ip和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(1234);


    //创建套接字
    int sock = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
    //连接
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    //接收数据
    char buffer[BUF_SIZE] = {0};
    int nCount;
    while((nCount = read(sock,buffer,BUF_SIZE)) > 0){
        fwrite(buffer, nCount, 1, fp);
    }

    printf("文件传输完成\n");

    fclose(fp);
    close(sock);

    return 0;
}

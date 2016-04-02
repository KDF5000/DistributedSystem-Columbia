#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<fstream>

#define BUF_SIZE 1024
using namespace std;


int main(int argn, char *argv[]){
    if(argn < 4){
        cout<<"请输入正确的参数";
        exit(0);
    }
    char host_ip[30]= {0};
    char host_port[10] = {0};
    char file_name[100] = {0};

    memcpy(host_ip, argv[1],strlen(argv[1]));
    memcpy(host_port, argv[2],strlen(argv[2]));
    memcpy(file_name, argv[3],strlen(argv[3]));

//    cout<<host_ip<<endl<<host_port<<endl<<strlen(file_name)<<endl;

    //向服务器发送请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(host_ip);
    serv_addr.sin_port = htons(atoi(host_port));

    char buffer[BUF_SIZE] = {0};
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //连接
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    //发送请求的文件名
    write(sock, file_name, strlen(file_name));

    read(sock,buffer,BUF_SIZE);
//    cout<<buffer<<endl;
    int status = atoi(buffer);
    if(status == 0){
        cout<<"File "<<file_name<<" does not exist"<<endl;
    }else{
        int nCount;
        ofstream outFile(file_name, ios::binary);

        while((nCount = read(sock,buffer,BUF_SIZE)) > 0){
            outFile<<buffer;
        }
        outFile.close();
        cout<<file_name<<" saved"<<endl;
    }
    //接受数据
    close(sock);

}

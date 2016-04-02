#include<iostream>
#include<cstring>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<cstdlib>
#include<unistd.h>
#include<stdio.h>
#include<fstream>
#include<map>

using namespace std;
#define MAX_CACHE (64*1024*1024)
#define BUF_SIZE 1024
#define MAX_FULL_NAME 500

map<string, string> contentCache;//cache
int cacheSize = 0;
int main(int argc, char *argv[]){
    if(argc < 3){
        cout<<"请输入正确的参数"<<endl;
        exit(0);
    }
    char * portStr;//端口号
    char * fileDir;//文件目录
    portStr = argv[1];
    fileDir = argv[2];
    cout<<portStr<<endl;
    cout<<fileDir<<endl;
    int port = atoi(portStr);

    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //将套接字绑定ip,端口
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(port);
    bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    //listen
    listen(serv_sock, 20);

    //接收客户端请求
    struct sockaddr_in clnt_addr,peer_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    socklen_t peer_addr_size = sizeof(peer_addr);
    char ipAddr[16];//保存点分十进制的地址

    while(1){
        int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

        char *request_file = new char[BUF_SIZE];
        memset(request_file, 0, BUF_SIZE);
        int recvLen = read(clnt_sock, request_file, BUF_SIZE);
        //get ip address of client
        getpeername(clnt_sock, (struct sockaddr*)&peer_addr, &peer_addr_size);
        cout<<"Client "<<inet_ntop(AF_INET, &peer_addr.sin_addr, ipAddr, sizeof(ipAddr))<<" is requesting file "<<request_file<<endl;

        //
        char *full_file_name = new char[MAX_FULL_NAME];
        memset(full_file_name, 0, MAX_FULL_NAME);
        memcpy(full_file_name, fileDir, strlen(fileDir));
        strcat(full_file_name, "/");
        strcat(full_file_name, request_file);

        //find in cache;
        string filename(full_file_name);
        map<string,string>::iterator iter;
        if((iter = contentCache.find(filename)) != contentCache.end()){
            string content = contentCache[filename];
            write(clnt_sock,content.c_str(), content.length());
            close(clnt_sock);
//            cout<<content<<endl;
            cout<<"Cache hit."<<request_file<<" sent to the client"<<endl;
            continue;
        }

       //find in disk
//        cout<<full_file_name<<endl;
        ifstream file(full_file_name,ios::binary);
        char *buffer = new char[BUF_SIZE];
        if(file){
            write(clnt_sock, "1", strlen("1"));
            string content("");
            while(!file.eof()){
                file.read(buffer, BUF_SIZE);
                string tmp(buffer);
                content += tmp;
                write(clnt_sock,buffer, strlen(buffer));
            }
            contentCache[filename] = content;
//            cout<<content<<endl;

            if(cacheSize > MAX_CACHE){
                map<string,string>::iterator iter = contentCache.begin();
                contentCache.erase(iter->first);
            }
            contentCache.insert(make_pair(filename,content));
            cout<<"Cache miss."<<request_file<<" sent to the client"<<endl;
        }else{
            write(clnt_sock, "0", strlen("0"));
            cout<<"File "<< request_file<< " does not exist"<<endl;
        }
         //关闭套接字
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}



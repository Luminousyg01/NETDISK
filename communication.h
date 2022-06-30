#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#endif // COMMUNICATION_H
#include <iostream>
#include <string>
#include <winsock2.h>
#include<QDebug>
#include<fstream>
#include<errno.h>
using namespace std;

#define myOK 0
#define myERROR -1

#define SENDFILESIZE 1024 // 每次发送的文件大小
#define SENDSIZE 2048
#define usercorrect_begin 2 // 判断用户账号密码是否合法，包括注册、登录时都要用到
#define MAXMESSAGE 128
class Communication {
private:
    boolean message_count_use[MAXMESSAGE]; //标记该消息号是否用过
    string ip;
    int port;
    boolean ConnectError;

public:
    SOCKET sclient;//pritvate
    // 與服務端鏈接是否錯誤
    boolean connecterror();

    Communication(string ip,int port);
    string recv_string();
    int send_string(string str);
    // 鏈接服務端
    int connection();
    // 斷開服務器鏈接
    int disconnection();
    ~Communication();
    int send_buf(char *buf,int length);
    int REconnection();
    int recv_buf(char *buf);
};


#include "./communication.h"


int Communication::send_string(string str)
{
    string sendstr=str;
    if(send(sclient,sendstr.c_str(),sendstr.length(),0)<=0){
        cout<<"fail to send message, please check the network"<<endl;
        return myERROR;
    }
    return 1;
}
int Communication::send_buf(char *buf,int length)
{
    if(send(sclient,buf,length,0)<=0){
        cout<<"fail to send message, please check the network"<<endl;
        return myERROR;
    }
    return 1;
}
string Communication::recv_string()
{
    string recvstr;
    char buf[SENDSIZE]={0};
    if(recv(sclient,buf,SENDSIZE,0)<=0)
        return NULL;
    recvstr=string(buf);
    return recvstr;
}
int Communication::recv_buf(char *buf)
{
    int ret=0;
    if((ret=recv(sclient,buf,5000,0))<=0)
        return NULL;
    return ret;
}
Communication::Communication(string ip,int port)
{
    this->ip=ip;
    this->port=port;
    this->ConnectError=false;
    memset(message_count_use,0,MAXMESSAGE);
    // this->connection();
}
int Communication::connection(){
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }
    sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sclient == INVALID_SOCKET)
    {
        //qDebug()<<strerror(errno)<<endl;
        cout<<"invalid socket!"<<endl;
        this->ConnectError=true;
        return myERROR;
    }
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(this->port);
    serAddr.sin_addr.S_un.S_addr = inet_addr(this->ip.c_str());
    if(connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {  //连接失败
        cout<<"connect error !"<<endl;
        closesocket(sclient);
        this->ConnectError=true;
        return myERROR;
    }
    qDebug()<<"connect OK"<<endl;
    return myOK;
}

boolean Communication::connecterror(){
    return this->ConnectError;
}


int Communication::disconnection(){
    if(connecterror()==false){
        closesocket(sclient);
    }
    return myOK;
}

Communication::~Communication(){
    disconnection();
}

int Communication::REconnection()
{
    while(1){
        if(this->connection()==myOK)
            return myOK;
        Sleep(100);
    }
    return myERROR;
}

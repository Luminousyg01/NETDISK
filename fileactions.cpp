#define _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_SECURE_NO_WARNINGS
#include"fileactions.h"
#include<QDebug>
#include<QDateTime>
#include<QTime>
#include<QCoreApplication>
#include<QFile>
#include<stdio.h>
#include"communication.h"
#include<QCryptographicHash>
#include<QFileInfo>
#include<QFileInfoList>
#include<QDir>
#include<QMessageBox>
#include<fstream>
#include<cstring>
#include<stdlib.h>
using namespace std;
extern Communication com;
extern string clientname;
extern string uploading_file;
extern string downloading_file;
extern string logfile;
extern string l_p;
extern bool F_G;
extern int stop;
extern int mode;
QString msgtoshow;
string getMD5(QString filename)
{
    QFile theFile(filename);
    theFile.open(QIODevice::ReadOnly);
    QByteArray ba = QCryptographicHash::hash(theFile.readAll(), QCryptographicHash::Md5);
    theFile.close();
    //qDebug() << ba.toHex().constData();
    return ba.toHex().constData();
}
string getsize(QString filename)
{
    QFile theFile(filename);
    return to_string(theFile.size());
}
void senduploadrequest(string md5,string size,string path)
{
    string res="2?";
    res+=md5;
    res+="?";
    res+=size;
    res+="?";
    res+=path;
    string name="";
    int i=0;
    int j=0;
    for(i = 0;i<uploading_file.length();++i)
    {
        if(uploading_file[i]=='/')
            j=i;
    }
    int z=0;
    for(z=j+1;z<uploading_file.length();++z)
    {
        name.push_back(uploading_file[z]);
    }
    res+="?";
    res+=name;
    res+="?";
    res+=clientname;
    com.send_string(res);

}

int getuploadreply(long long & start,int & length,int &client_num)
{
    string rpy;
    string start_str="";
    string length_str="";
    rpy=com.recv_string();
    string cnum="";
    if(rpy[0]=='1')
    {
        return 1;
    }
    else if (rpy[0]=='0')
    {
        int i=0;
        for(i=2;i<rpy.length();i++)
        {
            start_str.push_back(rpy[i]);
            if(rpy[i]==';')
            {
                i++;
                break;
            }
        }
        for(;i<rpy.length();++i)
        {
            length_str.push_back(rpy[i]);
            if(rpy[i]==';')
            {
                i++;
                break;
            }
        }
        for(;i<rpy.length();++i)
        {
            cnum.push_back(rpy[i]);
            if(rpy[i]==';')
            {
                i++;
                break;
            }
        }
        start = stoll(start_str);
        length = atoi(length_str.c_str());
        client_num=atoi(cnum.c_str());
        return 0;
    }
    else{
        qDebug()<<"文件名重复"<<endl;
        return 2;
    }
}
void seperate_path_name(string in_path,string& out_path,string &out_name)
{
    out_path="";
    out_name="";
    int i=0;
    int j=0;
    for(i = 0;i<in_path.length();++i)
    {
        if(in_path[i]=='/')
            j=i;
    }
    for(int z=0;z<j;++z)
    {
        out_path.push_back(in_path[z]);
    }
    for(int z=j+1;z<in_path.length();++z)
    {
        out_name.push_back(in_path[z]);
    }
    return;
}
void upload(long long start, int length,int mode,string md5,string path)
{
    string name="";
    int i=0;
    int j=0;
    for(i = 0;i<uploading_file.length();++i)
    {
        if(uploading_file[i]=='/')
            j=i;
    }
    int z=0;
    qDebug()<<j<<endl;
    for(z=j+1;z<uploading_file.length();++z)
    {
        name.push_back(uploading_file[z]);
    }
    QFile theFile(QString::fromStdString(uploading_file));
    theFile.open( QIODevice::ReadOnly);
    theFile.seek(start);

    char buf[5000];
    string head;
    head+="3?";
    head+=(char)('0'+mode);
    head+="?";
    head+=md5;
    head+="?";
    head+=to_string(start);
    head+="?";
    head+=to_string(length);
    head+='?';
    head+=path;
    head+='?';
    head+=name;
    head+='?';
    head+=clientname;
    head+='?';
    memcpy(buf,head.c_str(),head.length());
    qDebug()<<QString::fromStdString(head)<<endl;
    theFile.read(buf+head.length(),length);
    com.send_buf(buf,head.length()+length);
    qDebug()<<head.length()+length<<endl;
    theFile.close();
}

void makenewdir(string path,string dirname)
{
    string str;
    str="4?";
    str+=path;
    str+="?";
    str+=dirname;
    str+="?";
    str+=clientname;
    str+="?";
    com.send_string(str);
    char buf[5000];
    com.recv_buf(buf);
}
void downloadfile(string localpath,string cloudpath)
{
    downloading_file=cloudpath;
    string send11="";
    string s_path,s_name;
    seperate_path_name(cloudpath,s_path,s_name);
    send11+="7?";
    send11+=s_path+'/';
    send11+="?";
    send11+=s_name;
    send11+="?";
    send11+=clientname;
    send11+="?";
    com.send_string(send11);
    string recv11="";
    string recv_md5="";
    recv11=com.recv_string();
    if(recv11[0]=='0')
    {
        qDebug()<<"path error"<<endl;
        return ;
    }
    else if(recv11[0]=='1')
    {
        for(int i=2;recv11[i]!='?';++i)
        {
            recv_md5.push_back(recv11[i]);
        }
    }
    QFile file(QString::fromStdString(localpath));
    if(file.exists())
    {
        string newmd5=getMD5(QString::fromStdString(localpath));
        if(newmd5==recv_md5)
        {
            qDebug()<<"秒下成功"<<endl;
            return;
        }
        else
        {
            if(F_G)
                file.remove();
            F_G=0;
        }
    }
    fstream out;
    out.open(localpath,ios::out|ios::binary|ios::app);

    if(!out.is_open())
    {
        qDebug()<<"file not open"<<endl;
        return;
    }
    int start = out.tellp();
    string name="";
    int i=0;
    int j=0;
    for(i = 0;i<downloading_file.length();++i)
    {
        if(downloading_file[i]=='/')
            j=i;
    }
    int z=0;
    //qDebug()<<j<<endl;
    for(z=j+1;z<downloading_file.length();++z)
    {
        name.push_back(downloading_file[z]);
    }
    string msg="";
    msg+="6?";
    for(z=0;z<=j;++z)
    {
       msg.push_back(downloading_file[z]);
    }
    msg+="?";
    msg+=name;
    msg+="?";
    msg+=to_string(start);
    msg+="?";
    msg+=clientname;
    msg+="?";
    int size_get=0;
    int ret=0;
    int length=0;
    com.send_string(msg);
    char buf[5000]={0};
    while(1)
    {
        ret=com.recv_buf(buf);
        i=0;
        if(buf[0]=='5')
        {
            qDebug()<<"path error"<<endl;
            return;
        }
        string str="";
        for(i=2;buf[i]!=';';++i)
        {
           str+=buf[i];
        }
        i++;
        qDebug()<<ret<<endl;
       // qDebug()<<"md5:"<<QString::fromStdString(str)<<endl;
        str="";
        for(;buf[i]!=';';++i)
        {
           str+=buf[i];
        }
        length=stoll(str);
        qDebug()<<"leg:"<<length<<endl;
       // qDebug()<<"length:"<<length<<endl;
        i+=1;
        //out.seekp(size_get,ios::beg);
        size_get+=ret-i;
        out.write(buf+i,ret-i);
        qDebug()<<"size"<<size_get<<endl;
        if(out.tellp()>=length || stop==1)
            break;
        msg="";
        msg+="6?";
        for(z=0;z<=j;++z)
        {
           msg.push_back(downloading_file[z]);
        }
        start = out.tellp();
        msg+="?";
        msg+=name;
        msg+="?";
        msg+=to_string(start);
        msg+="?";
        msg+=clientname;
        msg+="?";
        com.send_string(msg);
        int attime=(int)((length-start)*5/1024/1024);
        msgtoshow=QString::fromStdString(downloading_file)+"下载进度"+QString::number(start)+"/"+QString::number(length)+""+"预估时间"+QString::number(attime)+"s";
    }
    out.close();
    if(!stop)
    {
        msgtoshow=QString::fromStdString(downloading_file)+"下载完成";
        fstream outb;
        outb.open(clientname+"history.txt",ios::out|ios::app);
        outb<<msgtoshow.toStdString()<<endl;
        outb.close();
    }
}

void uploadafile(string localpath,string cloudpath)
{
    uploading_file = localpath;
    string md5=getMD5(QString::fromStdString(localpath));
    string size=getsize(QString::fromStdString(localpath));
    senduploadrequest(md5,size,cloudpath);
    int length,ret;
    long long start;
    int client_num;
    mode = 1;
    ret=getuploadreply(start,length,client_num);
    while(1)
    {
        if(ret >= 1 || stop==1)
            break;
        else
        {
            upload(start, length,mode,md5,cloudpath);
            ret=getuploadreply(start,length,client_num);
            int attime=(int)((stoll(size.c_str())-start)*5/1024/1024/client_num);
            msgtoshow=QString::fromStdString(uploading_file)+"上传进度"+QString::number(start)+"/"+QString::fromStdString(size)+"预估时间"+QString::number(attime)+"s";
        }
    }
    if(!stop)
    {
        msgtoshow=QString::fromStdString(uploading_file)+"上传完成";
        fstream outb;
        outb.open(clientname+"history.txt",ios::out|ios::app);
        outb<<msgtoshow.toStdString()<<endl;
        outb.close();
    }
}

void replacelocalbycloud(string &local,string localhead,string cloudhead)
{
    string local_cpy=local;
    int localheadnum=localhead.length();
    //int cloudheadnum=cloudhead.length();
    local_cpy.erase(0,localheadnum);
    local=cloudhead+local_cpy;

}
void uploadfolder(string localpath,string cloudpath)
{
        string localhead=l_p;
        string cloudhead=cloudpath;
        cloudhead.pop_back();
        QDir dir(QString::fromStdString(localpath));
        if (!dir.exists()) {
            qDebug()<<"目录不存在"<<endl;
            return;
        }
        dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
         dir.setSorting(QDir::DirsFirst);
        //QFileInfoList list = dir.entryInfoList();
        //int i = 0;

        foreach(QFileInfo fileInfo,dir.entryInfoList())
        {
            bool bisDir = fileInfo.isDir();
            if (bisDir) {
                string fpath;
                string fname;
                seperate_path_name(fileInfo.filePath().toStdString(),fpath,fname);
               //qDebug()<<QString::fromStdString(localhead)<<endl;
                replacelocalbycloud(fpath,localhead,cloudhead);
                fpath+='/';
                makenewdir(fpath,fname);
                Sleep(1);
                qDebug()<<QString::fromStdString(fpath)<<QString::fromStdString(fname)<<endl;
                //qDebug()<<"dir: "<<fileInfo.filePath()<<"/"<<fileInfo.fileName();
                uploadfolder(fileInfo.filePath().toStdString(),cloudpath);
            } else {
                //blog_file_paths_ << fileInfo.path();
                //blog_file_names_ << fileInfo.fileName();
                string cpath=fileInfo.path().toStdString();
                replacelocalbycloud(cpath,localhead,cloudhead);//
                cpath+='/';
                string lpath=fileInfo.filePath().toStdString();
                //string lpath1,lname1;
                //seperate_path_name(lpath,lpath1,lname1);
                uploadafile(lpath,cpath);
                qDebug() << QString::fromStdString(lpath)<<endl;
                //nFiles++;
            }
        }


        return;
}


void downloadfolder(string localpath,string cloudpath)
{
    string localhead=localpath;
    string cloudhead=cloudpath;
    cloudhead.pop_back();
    string path[256];
    int ret;
    ret=getpath(path,cloudpath);
    for(int i=0;i<ret;++i)
    {
        if(path[i].back()=='/')
        {
            path[i].pop_back();
            string localp=path[i];
            replacelocalbycloud(localp,cloudhead,localhead);
            createdir(localp);
            continue;
        }
        else
        {
            string local=path[i];
            replacelocalbycloud(local,cloudhead,localhead);
            downloadfile(local,path[i]);
        }
    }
}

int getpath(string *path,string in_path)
{
    string sen="";
    sen+="5?";

    sen+=in_path;
    sen+="?";
    sen+=clientname;
    sen+="?";
    com.send_string(sen);
    int ret=0;
    int all_length=0;
    char buf[5000]={0};
    string num="";
    ret=com.recv_buf(buf);
    int i=0;
    for(i=2;buf[i]!=';';++i)
    {
        num.push_back(buf[i]);
    }
    i++;
    all_length=atoi(num.c_str());
    all_length+=3;
    all_length+=num.length();
    //qDebug()<<"all"<<all_length;
    int index = 0;
    for(;i<ret;++i)
    {
        if(buf[i]=='\n')
        {
            index++;
            continue;
        }
        else
        {
            path[index].push_back(buf[i]);
        }
    }
    int now_length=ret;
    while(1)
    {
        //qDebug()<<"now"<<now_length<<endl;
        if(now_length>=all_length)
            break;
        else
        {
            ret=com.recv_buf(buf);
            for(i=0;i<ret;++i)
            {
                if(buf[i]=='\n')
                {
                    //qDebug()<<QString::fromStdString(path[i]);
                    index++;
                    continue;
                }
                else
                {
                    path[index].push_back(buf[i]);
                }
            }
            now_length+=ret;
        }
    }
    return index;
}
void createdir(string path)
{
    QDir dir(QString::fromStdString(path));
    if(!dir.exists()){
        QDir().mkdir(QString::fromStdString(path));
        /*
        if(!ismkdir)
            qDebug() << "Create path fail" << endl;
        else
            qDebug() << "Create fullpath success" << endl;
        */
    }
    else{
        ;//qDebug() << "fullpath exist" << endl;
    }
}
void deletefile(string filepath,string filename)
{
    string sendd="";
    sendd+="8?";
    sendd+=filepath;
    sendd+="?";
    sendd+=filename;
    sendd+="?";
    sendd+=clientname;
    sendd+="?";
    com.send_string(sendd);
    char buf[5000];
    com.recv_buf(buf);
}
void deletedir(string dirpath,string dirname)
{
    string sendd="";
    sendd+="9?";
    sendd+=dirpath;
    sendd+="?";
    sendd+=dirname;
    sendd+="?";
    sendd+=clientname;
    sendd+="?";
    com.send_string(sendd);
    char buf[5000];
    com.recv_buf(buf);
}
void copyfile(string path1,string path2)
{
    string truepath="";
    string name="";
    seperate_path_name(path1,truepath,name);
    string sendcpy="a?";
    sendcpy+=truepath;
    sendcpy+="?";
    sendcpy+=name;
    sendcpy+="?";
    sendcpy+=path2;
    sendcpy+="?";
    sendcpy+=clientname;
    sendcpy+="?";
    com.send_string(sendcpy);
    char buf[5000];
    com.recv_buf(buf);
}

void copydir(string path1,string path2)
{
    string truepath="";
    string name="";
    seperate_path_name(path1,truepath,name);
    string sendcpy="b?";
    sendcpy+=truepath;
    sendcpy+="?";
    sendcpy+=name;
    sendcpy+="?";
    sendcpy+=path2;
    sendcpy+="?";
    sendcpy+=clientname;
    sendcpy+="?";
    com.send_string(sendcpy);
    char buf[5000];
    com.recv_buf(buf);
}
void movefile(string path1,string path2)
{
    string truepath="";
    string name="";
    seperate_path_name(path1,truepath,name);
    string sendcpy="c?";
    sendcpy+=truepath;
    sendcpy+="?";
    sendcpy+=name;
    sendcpy+="?";
    sendcpy+=path2;
    sendcpy+="?";
    sendcpy+=clientname;
    sendcpy+="?";
    com.send_string(sendcpy);
    char buf[5000];
    com.recv_buf(buf);
    if(buf[0]=='0')
    {
        msgtoshow="操作不合理";
    }
}

void movedir(string path1,string path2)
{
    string truepath="";
    string name="";
    seperate_path_name(path1,truepath,name);
    string sendcpy="d?";
    sendcpy+=truepath;
    sendcpy+="?";
    sendcpy+=name;
    sendcpy+="?";
    sendcpy+=path2;
    sendcpy+="?";
    sendcpy+=clientname;
    sendcpy+="?";
    com.send_string(sendcpy);
    char buf[5000];
    com.recv_buf(buf);
    if(buf[0]=='0')
    {
        msgtoshow="操作不合理";
    }
}
void modifyfile(string localpath,string cloudpath)
{
    string truepath="";
    string name="";
    seperate_path_name(localpath,truepath,name);
    string sendcpy="e?";
    sendcpy+=truepath;
    sendcpy+="?";
    sendcpy+=name;
    sendcpy+="?";
    sendcpy+=cloudpath;
    sendcpy+="?";
    sendcpy+=clientname;
    sendcpy+="?";
    com.send_string(sendcpy);
    char buf[5000];
    com.recv_buf(buf);
}

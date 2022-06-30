#ifndef FILEACTIONS_H
#define FILEACTIONS_H
#endif // FILEACTIONS_H
#define _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<winsock2.h>
#include<Windows.h>
#include<cstdlib>
#include<thread>
#include<time.h>
#include<string>
#include<fstream>
#include<iomanip>
#include<sstream>
#include<cstring>
#include<direct.h>
#include <ImageHlp.h>
#include <QWidget>
#define CHECKMD5 1
#define SHOWTREE 0
using namespace std;
string getMD5(QString filename);
string getsize(QString filename);
void senduploadrequest(string md5,string size,string path);
int getuploadreply(long long & start,int & length,int &client_num);
void upload(long long start, int length,int mode,string md5,string path);
void makenewdir(string path,string dirname);
void downloadfile(string localpath,string cloudpath);
void uploadfolder(string localpath,string cloudpath);
void downloadfolder(string localpath,string cloudpath);
int getpath(string *path,string in_path);
void createdir(string path);
void deletefile(string filepath,string filename);
void deletedir(string dirpath,string dirname);
void copyfile(string path1,string path2);
void copydir(string path1,string path2);
void movefile(string path1,string path2);
void movedir(string path1,string path2);
void uploadafile(string localpath,string cloudpath);
void modifyfile(string localpath,string cloudpath);

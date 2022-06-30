#include "workthread.h"
#include"dialog_folder.h"
#include"fileactions.h"
#include<string>
#include<cstring>
using namespace std;
extern int confirm_mode;
extern string downloading_file;
extern string uploading_file;
extern string localpath;
extern string cloudpath;
extern int mode;
extern string l_p;
WorkerThread::WorkerThread(QObject *parent)
    :QThread(parent)
{

}
void WorkerThread::run()
{
    if(confirm_mode==1)
    {
       uploadafile(localpath,cloudpath);
    }
    else if(confirm_mode==2)
    {
        makenewdir(localpath,cloudpath);
    }
    else if(confirm_mode==3)
    {
        downloadfile(localpath,cloudpath);
    }
    else if(confirm_mode==4)
    {
        l_p=localpath;
        uploadfolder(localpath,cloudpath);
    }
    else if(confirm_mode==5)
    {
        downloadfolder(localpath,cloudpath);
    }
    else if(confirm_mode==6)
    {
        deletefile(localpath,cloudpath);
    }
    else if(confirm_mode==7)
    {
        deletedir(localpath,cloudpath);
    }
    else if(confirm_mode==8)
    {
        copyfile(localpath,cloudpath);
    }
    else if(confirm_mode==9)
    {
        copydir(localpath,cloudpath);
    }
    else if(confirm_mode==10)
    {
        movefile(localpath,cloudpath);
    }
    else if(confirm_mode==11)
    {
        movedir(localpath,cloudpath);
    }
    else if(confirm_mode==12)
    {
        modifyfile(localpath,cloudpath);
    }
}

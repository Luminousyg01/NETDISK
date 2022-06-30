#include "dialog_folder.h"
#include "ui_dialog_folder.h"
#include"fileactions.h"
#include"communication.h"
#include<QFileDialog>
#include<QDebug>
#include<QTimer>
#include<QMessageBox>
#include<fstream>
extern Communication com;
extern string clientname;
extern string downloading_file;
extern string uploading_file;
extern QString filetree;
extern string password;
string localpath;
string cloudpath;
string l_p;
int mode;
int confirm_mode;
extern QString msgtoshow;
bool F_G=0;
int stop;
Dialog_folder::Dialog_folder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_folder)
{
    ui->setupUi(this);
    ui->textBrowser->setText(filetree);
    QTimer *qt0=new QTimer();
    qt0->setInterval(100);
    qt0->start();
    connect(qt0,SIGNAL(timeout()),this,SLOT(onTimeOut()));
    ui->stop->setDisabled(true);
    string filename=clientname+"now.txt";
    fstream ina;
    ina.open(filename,ios::in|ios::binary);
    string content="";
    ina >> content;
    msgtoshow=QString::fromStdString(content);
    ina.close();
}
int status;
Dialog_folder::~Dialog_folder()
{
    delete ui;
}
void Dialog_folder::onTimeOut()
{
    ui->textmsg->setText(msgtoshow);
    fstream outa;
    string filename=clientname+"now.txt";
    outa.open(filename,ios::out|ios::binary);
    //qDebug()<<msgtoshow<<endl;
    outa<<msgtoshow.toStdString();
    outa.close();
}
void Dialog_folder::on_myfile_clicked()
{
    com.send_string("1;"+clientname+";"+password+";");
    //qDebug()<<msg.user_correct<<endl;
    string rcv_msg;
    rcv_msg=com.recv_string();
    //qDebug()<<QString::fromStdString(rcv_msg)<<endl;
    rcv_msg.erase(rcv_msg.begin());
    rcv_msg.erase(rcv_msg.begin());
    int i=0;
    for(i=0;rcv_msg[i]!=';';++i)
    {
        ;
    }
    for(int j=0;j<i;++j)
    {
        rcv_msg.erase(rcv_msg.begin());
    }
    rcv_msg.erase(rcv_msg.begin());
    filetree=QString::fromStdString(rcv_msg);
    ui->textBrowser->setText(filetree);
    //qDebug()<<"ok"<<endl;

}
void Dialog_folder::on_stop_clicked()
{
    stop=1;
}
void Dialog_folder::on_addpath_clicked()
{
    confirm_mode=1;
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Allfile(*.*)"));
    ui->localpath->setPlainText(fileName);
    ui->cloudpath->setDisabled(false);
    ui->confirm->setDisabled(false);
}

void Dialog_folder::on_newdir_clicked()
{
    confirm_mode=2;
    ui->localpath->setDisabled(false);
    ui->cloudpath->setDisabled(false);
    ui->confirm->setDisabled(false);
}
void Dialog_folder::on_confirm_clicked()
{
    stop=0;
    cloudpath = ui->cloudpath->toPlainText().toStdString();
    localpath = ui->localpath->toPlainText().toStdString();
    if(confirm_mode==3)
    {
        QFile f(QString::fromStdString(localpath));
        if(f.exists())
        {
            QMessageBox::StandardButton reply;
            reply=QMessageBox::question(this,
                   tr("文件已存在"),
                   tr("是否覆盖"),
                   QMessageBox::Yes | QMessageBox::No,
                   QMessageBox::Yes);
            if(reply==QMessageBox::Yes)
                F_G=1;
            else
                F_G=0;
        }
    }
     workerThread = new WorkerThread(this);
     connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
     workerThread->start();
     ui->textmsg->setText(msgtoshow);
     ui->stop->setDisabled(false);
}
void Dialog_folder::on_downloaddir_clicked()
{
    confirm_mode=5;
    ui->localpath->setDisabled(false);
    ui->cloudpath->setDisabled(false);
    ui->confirm->setDisabled(false);
}
void Dialog_folder::on_filearray_clicked()
{
    fstream inb;
    inb.open(clientname+"history.txt",ios::in);
    char str[200];
    QString show="";
    while( inb.getline(str,200) )
    {
        qDebug()<<str<<endl;
        show+=str;
    }

    ui->textBrowser->setText(show);
    inb.close();
}
void Dialog_folder::on_download_clicked()
{
    confirm_mode=3;
    ui->localpath->setDisabled(false);
    ui->cloudpath->setDisabled(false);
    ui->confirm->setDisabled(false);
}
void Dialog_folder::on_addpathdir_clicked()
{
    confirm_mode=4;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home");
    ui->localpath->setPlainText(dir);
    ui->cloudpath->setDisabled(false);
    ui->confirm->setDisabled(false);

}
void Dialog_folder::on_deletefile_clicked()
{
    confirm_mode=6;
    ui->localpath->setDisabled(false);
    ui->cloudpath->setDisabled(false);
    ui->confirm->setDisabled(false);
}
void Dialog_folder::on_deletedir_clicked()
{
    confirm_mode=7;
    ui->localpath->setDisabled(false);
    ui->cloudpath->setDisabled(false);
    ui->confirm->setDisabled(false);
}
void Dialog_folder::on_copyfile_clicked()
{
    confirm_mode=8;
    ui->localpath->setDisabled(false);
    ui->cloudpath->setDisabled(false);
    ui->confirm->setDisabled(false);
}
void Dialog_folder::on_copydir_clicked()
{
    confirm_mode=9;
    ui->localpath->setDisabled(false);
    ui->cloudpath->setDisabled(false);
    ui->confirm->setDisabled(false);
}
void Dialog_folder::on_movefile_clicked()
{
    confirm_mode=10;
    ui->localpath->setDisabled(false);
    ui->cloudpath->setDisabled(false);
    ui->confirm->setDisabled(false);
}
void Dialog_folder::on_movedir_clicked()
{
    confirm_mode=11;
    ui->localpath->setDisabled(false);
    ui->cloudpath->setDisabled(false);
    ui->confirm->setDisabled(false);
}
void Dialog_folder::on_modifyname_clicked()
{
    confirm_mode=12;
    ui->localpath->setDisabled(false);
    ui->cloudpath->setDisabled(false);
    ui->confirm->setDisabled(false);
}
void Dialog_folder::on_preview_clicked()
{
    QString file_full_path=ui->localpath->toPlainText();
    QString disp="";
    char buf[500];
    int i=0;
    ifstream inf;
    inf.open(file_full_path.toStdString(),ios::in);
    while(inf.getline(buf,100))
    {
       i++;
       if(i>30)
           break;
       disp+=buf;
    }
    ui->textBrowser->setText(disp);
    inf.close();
}

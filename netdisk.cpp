#include "netdisk.h"
#include "ui_netdisk.h"
#include "dialog_folder.h"
#include "dialog_regist.h"
#include "communication.h"
#include "fileactions.h"
extern QString filetree;
extern Communication com;
extern string clientname;
extern string uploading_file;
extern string downloading_file;
extern string logfile;
string password;
netdisk::netdisk(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::netdisk)
{
    ui->setupUi(this);
}

netdisk::~netdisk()
{
    delete ui;
}


void netdisk::on_login_clicked()
{

    password = ui->password->toPlainText().toStdString();
    clientname=ui->username->toPlainText().toStdString();
    com.send_string("1;"+clientname+";"+password+";");
    //qDebug()<<msg.user_correct<<endl;
    string rcv_msg;
    rcv_msg=com.recv_string();
    //qDebug()<<QString::fromStdString(rcv_msg)<<endl;
    if(rcv_msg[0]=='0'){
        ui->show->setText("账号不存在");
        //qDebug()<<msg.user_correct<<"wrong"<<endl;
        return;
    }
    else if(rcv_msg[0]=='2')
    {
        ui->show->setText("密码错误");
        return;
    }
    else
        ui->show->clear();
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
    //qDebug()<<"ok"<<endl;
    Dialog_folder df;
    df.show();
    df.exec();
    this->show();
}

void netdisk::on_regist_clicked()
{
    this->close();
    Dialog_regist dr;
    dr.show();
    dr.exec();
    this->show();
}

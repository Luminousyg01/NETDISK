#include "dialog_regist.h"
#include "ui_dialog_regist.h"
#include "communication.h"
#include "fileactions.h"
#include<QDebug>
extern Communication com;
extern string clientname;

Dialog_regist::Dialog_regist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_regist)
{
    ui->setupUi(this);
}

Dialog_regist::~Dialog_regist()
{
    delete ui;
}

bool Dialog_regist::check_patern(string in){
    if(in.length()<12){
        ui->msg->setText("密码至少长度为12");
        return false;
    }
    bool types[5]={false};
    int typenum=0;
    for(int i=0;i<(int)in.length();i++){
        if(in[i]<='z'&&in[i]>='a')
            types[0]=true;
        else if(in[i]<='Z'&&in[i]>='A')
            types[1]=true;
        else if(in[i]<='9'&&in[i]>='0')
            types[2]=true;
        else
            types[3]=true;
    }
    for(int i=0;i<4;++i)
    {
        if(types[i]==true)
            typenum++;
    }
    if(typenum<3){
        ui->msg->setText("密码需要包含三种字符");
        return false;
    }
    return true;
}

void Dialog_regist::on_finish_clicked()
{
    QString passwd1=ui->password1->toPlainText();
    QString passwd2=ui->password2->toPlainText();
    if(passwd1!=passwd2){
        ui->msg->setText("密码输入不一致");
        return;
    }

    if(!check_patern(passwd1.toStdString()))
        return ;
    ui->msg->clear();
    string password = passwd1.toStdString();
    string username = ui->username->toPlainText().toStdString();
    //qDebug()<<MD5passwd.c_str()<<endl;
    com.send_string("0;"+username+';'+password+';');
    string rcv_string;
    rcv_string=com.recv_string();
    qDebug()<<QString::fromStdString(rcv_string)<<endl;
    if(rcv_string[0]=='0'){
        ui->msg->setText("账号已经存在");
        return;
    }
    ui->msg->clear();
    if(rcv_string[0]=='1'){
        ui->msg->setText("注册成功,3s后自动跳转");
    }

    Sleep(3000);

    this->close();

}

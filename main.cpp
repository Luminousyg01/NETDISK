#include "netdisk.h"

#include <QApplication>
#include"communication.h"

Communication com("175.24.188.7",5000);
QString filetree;
string clientname;
string downloading_file;
string uploading_file;
string logfile;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    netdisk w;
    com.connection();
    w.show();
    return a.exec();
}

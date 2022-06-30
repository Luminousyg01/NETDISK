#ifndef DIALOG_FOLDER_H
#define DIALOG_FOLDER_H

#include <QDialog>
#include"workthread.h"
#define ADDPATH 1
#define DELETEPATH 2
#define BONDPATH 3
#define UNBONDPATH 4
namespace Ui {
class Dialog_folder;
}

class Dialog_folder : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_folder(QWidget *parent = nullptr);
    ~Dialog_folder();

private slots:
    void on_myfile_clicked();

    void on_addpath_clicked();

    void on_confirm_clicked();

    void on_filearray_clicked();
    void on_download_clicked();
    void on_newdir_clicked();
    void on_addpathdir_clicked();
    void on_downloaddir_clicked();
    void on_deletefile_clicked();
    void on_deletedir_clicked();
    void on_copyfile_clicked();
    void on_copydir_clicked();
    void on_movefile_clicked();
    void on_movedir_clicked();
    void on_stop_clicked();
    void on_modifyname_clicked();
    void on_preview_clicked();
private:
    Ui::Dialog_folder *ui;
    WorkerThread *workerThread;
protected slots:
    void onTimeOut();
};

#endif // DIALOG_FOLDER_H

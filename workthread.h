#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include<QThread>
class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread(QObject *parent = nullptr);
protected:
    void run();
signals:
    void resultReady(const QString &s);
};



#endif // WORKTHREAD_H

#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class WorkerThread : public QThread
{
        Q_OBJECT

    public:
        WorkerThread(QObject *parent);
        ~WorkerThread() override;

    protected:
        void run() override;

    private:
        QMutex mutex;
        QWaitCondition condition;
        volatile bool abort;
};

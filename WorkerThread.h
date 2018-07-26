#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

struct ExtractStatusMessage
{
};
Q_DECLARE_METATYPE(ExtractStatusMessage)

class WorkerThread : public QThread
{
        Q_OBJECT

    public:
        WorkerThread(QObject *parent);
        ~WorkerThread() override;
        void extract(const QString &archive, const QString &outputFolder);

    signals:
        void dieParent(const QString &message);
        void updateGUI(ExtractStatusMessage msg);

    protected:
        void run() override;

    private:
        QMutex mutex;
        QWaitCondition condition;
        volatile bool abort;
        QString archive;
        QString outputFolder;
};

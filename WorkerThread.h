#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

struct ExtractStatusMessage
{
    QString status = QString();
    QString currentFile = QString();
    QString currentArchive = QString();
    float currentFilePercent = std::numeric_limits<float>::infinity();
    float totalPercent = std::numeric_limits<float>::infinity();
};
Q_DECLARE_METATYPE(ExtractStatusMessage)

class WorkerThread : public QThread
{
        Q_OBJECT

    public:
        explicit WorkerThread(QObject *parent);
        virtual ~WorkerThread();
        void extract(const QString &archive, const QString &outputFolder);

    signals:
        void dieSignal(const QString &message);
        void finished();
        void updateGUI(ExtractStatusMessage msg);
        void log(const QString &message);

    protected:
        void run() override;

    private:
        QMutex mutex;
        QWaitCondition condition;
        volatile bool abort;
        QString archive;
        QString outputFolder;

    public:
        QString waitingArchive;
};

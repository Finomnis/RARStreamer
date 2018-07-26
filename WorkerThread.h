#pragma once

#include "ProgressTracker.h"

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

struct ExtractStatusMessage
{
    QString status = QString();
    QString currentFile = QString();
    QString currentArchive = QString();
    float currentFilePercent = std::numeric_limits<float>::infinity();
    float currentArchivePercent = std::numeric_limits<float>::infinity();
};
Q_DECLARE_METATYPE(ExtractStatusMessage)

class WorkerThread : public QThread
{
        Q_OBJECT

    public:
        explicit WorkerThread(QObject *parent);
        virtual ~WorkerThread();
        void extract(const QString &archive, const QString &outputFolder);
        inline bool needsAbort() {return abort;}
        float getFilePercent();
        float getArchivePercent();
        float addExtractedData();


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
        volatile bool abort = false;
        QString archive;
        QString outputFolder;
        ProgressTracker progressTracker;

    public:
        QString waitingArchive;
};

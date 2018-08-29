#pragma once

#include "ProgressTracker.h"

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

struct ExtractStatusMessage
{
    QString status = QString();
    QString statusColor = QString();
    QString currentFile = QString();
    QString currentArchive = QString();
    float currentFilePercent = std::numeric_limits<float>::infinity();
};
Q_DECLARE_METATYPE(ExtractStatusMessage)

class WorkerThread : public QThread
{
        Q_OBJECT

    public:
        explicit WorkerThread(QObject *parent);
        virtual ~WorkerThread();
        void extract(const QString &archive, const QString &outputFolder, const QString &password);

        bool needsAbort() const;
        void cancel();
        float getFilePercent() const;
        void addExtractedData(uint64_t dataSize);
        QString getPassword();
        void setPassword(const QString &password);

    signals:
        void dieSignal(const QString &message);
        void finished(bool success);
        void updateGUI(ExtractStatusMessage msg);
        void log(const QString &message);
        void requestPassword();

    protected:
        void run() override;

    private:
        QMutex mutex;
        QWaitCondition condition;
        volatile bool abort = false;
        QString archive;
        QString outputFolder;
        ProgressTracker progressTracker;
        QString password;
        QMutex passwordMutex;
        QWaitCondition passwordCondition;

    public:
        QString waitingArchive;
};

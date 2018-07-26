#include "WorkerThread.h"

#include "UnRarHelpers.h"

#include <QFileInfo>

#define die(msg, archive) { if(archive) RARCloseArchive(archive); emit dieSignal(msg); return; }

namespace
{
QString extractFilename(const wchar_t *name)
{
    QFileInfo file(QString::fromWCharArray(name));
    return file.fileName();
}
}

extern "C" {
    int CALLBACK UnrarCallback(UINT msg, LPARAM UserData, LPARAM P1, LPARAM P2)
    {
        WorkerThread *parent = (WorkerThread *)UserData;
        switch (msg)
        {
            case UCM_CHANGEVOLUMEW:
                if (P2 == RAR_VOL_NOTIFY)
                {
                    parent->waitingArchive = QString();

                    QString archive = extractFilename((wchar_t *)P1);

                    ExtractStatusMessage msg;
                    msg.status = "Extracting ...";
                    msg.currentArchive = archive;
                    emit parent->updateGUI(msg);

                    emit parent->log(QString("Reading archive '") + archive + QString("' ..."));
                }
                else if (P2 == RAR_VOL_ASK)
                {
                    QString archive = extractFilename((wchar_t *)P1);
                    if (parent->waitingArchive != archive)
                    {
                        parent->waitingArchive = archive;

                        ExtractStatusMessage msg;
                        msg.status = "Waiting for next part ...";
                        msg.currentArchive = archive;
                        emit parent->updateGUI(msg);

                        emit parent->log(QString("Waiting for archive '") + archive + QString("' ..."));
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                else
                    emit parent->log(QString("Warning: Signal UCM_CHANGEVOLUMEW caught with invalid parameter ") + QString::number(P2) + QString("."));
                break;
            case UCM_PROCESSDATA:
                parent->addExtractedData(uint64_t(P2));
                {
                    ExtractStatusMessage msg;
                    msg.currentFilePercent = parent->getFilePercent();
                    emit parent->updateGUI(msg);
                }
                break;
            case UCM_NEEDPASSWORDW:
                emit parent->dieSignal("Password archives not yet implemented. TODO.");
                return -1;
                break;
            case UCM_CHANGEVOLUME:
                break;
            case UCM_NEEDPASSWORD:
                break;
            default:
                emit parent->log(QString("Warning: Callback with unknown message ") + QString::number(msg) + QString(" caught."));
        }

        return parent->needsAbort() ? -1 : 1;
    }
}

WorkerThread::WorkerThread(QObject *parent)
    : QThread(parent)
{

}

WorkerThread::~WorkerThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}



void WorkerThread::extract(const QString &archive, const QString &outputFolder)
{
    if (isRunning())
        return;

    this->archive = archive;
    this->outputFolder = outputFolder;

    start(LowPriority);
}


void WorkerThread::run()
{
    emit log(QString("UnRAR Static Version: ") + QString::number(RAR_DLL_VERSION));
    emit log(QString("UnRAR Dynamic Version: ") + QString::number(RARGetDllVersion()));

    if (RARGetDllVersion() < RAR_DLL_VERSION)
        die("UnRAR.dll is too old!", nullptr);

    {
        ExtractStatusMessage msg;
        msg.status = "Opening ...";
        emit updateGUI(msg);
    }

    // Initialize UnRAR config
    RAROpenArchiveDataEx unrarConfig;
    memset(&unrarConfig, 0, sizeof(unrarConfig));

    // Fill Unrar Config
    std::wstring archiveName = archive.toStdWString();
    unrarConfig.ArcNameW = (wchar_t *)archiveName.c_str();
    unrarConfig.OpenMode = RAR_OM_EXTRACT;
    unrarConfig.Callback = UnrarCallback;
    unrarConfig.UserData = (LPARAM)this;

    // Open Archive
    emit log(QString("Opening archive '") + archive + QString("' ..."));
    HANDLE rarHandle = RAROpenArchiveEx(&unrarConfig);
    if (rarHandle == nullptr || unrarConfig.OpenResult != ERAR_SUCCESS)
    {
        if (unrarConfig.OpenResult == ERAR_SUCCESS)
        {
            die("Unable to open archive.", rarHandle);
        }
        else
        {
            die(rarOpenErrorToString(unrarConfig.OpenResult), rarHandle);
        }
    }

    // Log archive flags
    {
        QString flags;
        for (const QString &flag : parseArchiveFlags(unrarConfig.Flags))
        {
            if (!flags.isEmpty())
                flags += " | ";
            flags += flag;
        }
        emit log(QString("Archive Flags: ") + flags);
    }

    // Extract
    {
        ExtractStatusMessage msg;
        msg.status = "Extracting ...";
        msg.currentArchive = extractFilename((wchar_t *)archiveName.c_str());
        emit updateGUI(msg);
    }
    while (true)
    {
        // Read File Header
        RARHeaderDataEx fileHeader;
        {
            memset(&fileHeader, 0, sizeof(fileHeader));
            auto ret = RARReadHeaderEx(rarHandle, &fileHeader);
            if (ret != ERAR_SUCCESS)
            {
                if (ret == ERAR_END_ARCHIVE)
                {
                    break;
                }
                die(rarHeaderErrorToString(ret), rarHandle);
            }
        }

        emit log(QString("Extracting: ") + QString::fromWCharArray(fileHeader.FileNameW));

        uint64_t fileSize = (uint64_t(fileHeader.UnpSizeHigh) << 32) | uint64_t(fileHeader.UnpSize);
        progressTracker.addNewFile(fileSize);
        {
            ExtractStatusMessage msg;
            msg.currentFile = extractFilename(fileHeader.FileNameW);
            msg.currentFilePercent = 0.0f;
            emit updateGUI(msg);
        }

        // Extract file
        std::wstring outputFolderString = outputFolder.toStdWString();
        auto ret = RARProcessFileW(rarHandle, RAR_EXTRACT, (wchar_t *)outputFolderString.c_str(), nullptr);
        if (ret != ERAR_SUCCESS)
            die(rarProcessFileErrorToString(ret), rarHandle);
    }

    {
        ExtractStatusMessage msg;
        msg.status = "Closing archive ...";
        emit updateGUI(msg);
    }

    // Close Archive
    emit log(QString("Closing archive ..."));
    if (RARCloseArchive(rarHandle) != ERAR_SUCCESS)
        die("Archive close error", rarHandle);

    {
        ExtractStatusMessage msg;
        msg.status = "Finished.";
        emit updateGUI(msg);
    }
    emit log(QString("Finished."));

    emit finished();
}

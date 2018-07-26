#include "WorkerThread.h"

#include "UnRarHelpers.h"

#include <iostream>
#include <sstream>

#define die(msg, archive) { if(archive) RARCloseArchive(archive); emit dieSignal(msg); return; }

extern "C" {
    int CALLBACK UnrarCallback(UINT msg, LPARAM UserData, LPARAM P1, LPARAM P2)
    {
        WorkerThread *parent = (WorkerThread *)UserData;
        switch (msg)
        {
            case UCM_CHANGEVOLUMEW:
                if (P2 == RAR_VOL_NOTIFY)
                    std::wcout << L"Processing '" << (wchar_t *)P1 << "'" << std::endl;
                else if (P2 == RAR_VOL_ASK)
                {
                    std::wcout << L"Asked for '" << (wchar_t *)P1 << "'" << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                else
                    std::wcout << L"UCM_CHANGEVOLUMEW" << std::endl;
                break;
            case UCM_PROCESSDATA:
                break;
            case UCM_NEEDPASSWORDW:
                std::wcout << L"We need a password." << std::endl;
                break;
            case UCM_CHANGEVOLUME:
                break;
            case UCM_NEEDPASSWORD:
                break;
            default:
                std::wcout << L"Callback: " << msg << std::endl;
        }

        return 1;
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
    std::cout << "UnRAR Static Version: " << RAR_DLL_VERSION << std::endl;
    std::cout << "UnRAR Dynamic Version: " << RARGetDllVersion() << std::endl;

    if (RARGetDllVersion() < RAR_DLL_VERSION)
        die("UnRAR.dll is too old!", nullptr);

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
    log(QString("Opening archive '") + archive + QString("' ..."));
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
        log(QString("Archive Flags: ") + flags);
    }

    //emit updateGUI(ExtractStatusMessage());
    die("Not implemented yet.", rarHandle);
}

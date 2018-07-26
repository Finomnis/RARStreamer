#include "WorkerThread.h"

#include <iostream>
#include <thread>
#include <chrono>


#define die(msg) {emit dieParent(msg);return;}

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
    std::cout << "WorkerThread." << std::endl;

    emit updateGUI(ExtractStatusMessage());
    die("Not implemented yet.");
}

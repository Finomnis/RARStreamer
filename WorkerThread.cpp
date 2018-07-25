#include "WorkerThread.h"
#include <iostream>

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


void WorkerThread::run()
{
    std::cout << "WorkerThread." << std::endl;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    workerThread(this)
{
    ui->setupUi(this);
    workerThread.start(QThread::LowPriority);
}

MainWindow::~MainWindow()
{
    delete ui;
}

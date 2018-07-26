#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    workerThread(this)
{
    ui->setupUi(this);

    qRegisterMetaType<ExtractStatusMessage>();

    connect(&workerThread, SIGNAL(dieParent(QString)), this, SLOT(die(QString)));
    connect(&workerThread, SIGNAL(updateGUI(ExtractStatusMessage)), this, SLOT(updateStatus(ExtractStatusMessage)));

    workerThread.start(QThread::LowPriority);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::die(const QString &message)
{
    QMessageBox messageBox(QMessageBox::Icon::Critical, "Error", "Critical Error:\n" + message, QMessageBox::Ok, this);
    messageBox.exec();
    qApp->quit();
}

void MainWindow::updateStatus(const ExtractStatusMessage &message)
{

}

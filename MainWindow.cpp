#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    workerThread(this)
{
    ui->setupUi(this);

    qRegisterMetaType<ExtractStatusMessage>();

    connect(&workerThread, SIGNAL(dieSignal(QString)), this, SLOT(die(QString)));
    connect(&workerThread, SIGNAL(updateGUI(ExtractStatusMessage)), this, SLOT(updateStatus(ExtractStatusMessage)));
    connect(&workerThread, SIGNAL(log(QString)), this, SLOT(log(QString)));
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


void MainWindow::log(const QString &message)
{
    std::cout << "- " << message.toStdString() << std::endl;
}

void MainWindow::extract(const QString &archive)
{
    QFileInfo file(archive);

    if (!file.exists())
        die("Archive '" + archive + "' does not exist!");
    if (!file.isFile())
        die("'" + archive + "' is not a file!");

    QDir directory = file.dir();
    QString fileName = file.fileName();
    QString extractDirectory = directory.filePath("rarstream_"  + fileName);

    workerThread.extract(archive, extractDirectory);
}

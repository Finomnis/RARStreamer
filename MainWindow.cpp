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
    connect(&workerThread, SIGNAL(finished(bool)), this, SLOT(finished(bool)));
    connect(ui->cancelButton, SIGNAL(released()), this, SLOT(cancelButton()));
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
    //std::cout << "     Status update: "
    //          << message.status.toStdString() << " | "
    //          << message.currentArchive.toStdString() << " | "
    //          << message.currentFile.toStdString() << " | "
    //          << message.currentFilePercent << std::endl;
    if (!message.status.isEmpty())
        ui->label_status->setText(message.status);
    if (!message.currentArchive.isEmpty())
        ui->label_archive->setText(message.currentArchive);
    if (!message.currentFile.isEmpty())
        ui->label_file->setText(message.currentFile);
    if (message.currentFilePercent != std::numeric_limits<float>::infinity())
        ui->progressBar_currentFile->setValue(int(std::round(message.currentFilePercent) + 0.5f));
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

    ui->label_targetDirectory->setText(extractDirectory);

    workerThread.extract(archive, extractDirectory);
}

void MainWindow::finished(bool success)
{
    qApp->quit();
}

void MainWindow::cancelButton()
{
    ui->label_status->setText("Cancelling ...");
    workerThread.cancel();
}

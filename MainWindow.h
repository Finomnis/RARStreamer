#pragma once

#include <QMainWindow>
#include "WorkerThread.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        virtual ~MainWindow();
        void extract(const QString &archive, const QString &extractDirectory);

    private slots:
        void die(const QString &message);
        void updateStatus(const ExtractStatusMessage &msg);
        void log(const QString &message);
        void finished(bool success);
        void cancelButton();

    private:
        Ui::MainWindow *ui;
        WorkerThread workerThread;
};

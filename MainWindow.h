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

    private slots:
        void die(const QString &message);
        void updateStatus(const ExtractStatusMessage &msg);

    private:
        Ui::MainWindow *ui;
        WorkerThread workerThread;
};

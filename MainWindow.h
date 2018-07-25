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

    private:
        Ui::MainWindow *ui;
        WorkerThread workerThread;
};

#pragma once

#include <QDialog>

namespace Ui
{
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit PasswordDialog(QWidget *parent = nullptr);
        virtual ~PasswordDialog();

        QString getPassword();

    private slots:
        void okButton();

    private:
        Ui::PasswordDialog *ui;
        QString password;
};

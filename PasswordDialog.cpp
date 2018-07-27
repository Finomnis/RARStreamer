#include "PasswordDialog.h"
#include "ui_passworddialog.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <iostream>

PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);

    setFixedSize(size());
    ui->password->setFocus();

    connect(this, SIGNAL(accepted()), this, SLOT(okButton()));
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::okButton()
{
    this->password = ui->password->text();
}

QString PasswordDialog::getPassword()
{
    return password;
}

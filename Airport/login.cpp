#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_buttonBox_accepted()
{
    if (ui->lineEdit_login->text().length() == 0 ||
                ui->lineEdit_password->text().length() == 0) {
            QMessageBox::critical(this, "Ошибка в данных!", "Данные не заполнены, повторите попытку!");
    } else {
        this->login = ui->lineEdit_login->text();
        this->password = ui->lineEdit_password->text();
    }
}


void Login::on_buttonBox_rejected()
{
    this->close();
}


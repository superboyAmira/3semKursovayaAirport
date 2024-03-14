#include "registration.h"
#include "ui_registration.h"

Registration::Registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
}

Registration::~Registration()
{
    delete ui;
}

void Registration::on_buttonBox_accepted()
{
    if (ui->lineEdit_login->text().length() == 0 ||
                ui->lineEdit_password->text().length() == 0) {
            QMessageBox::critical(this, "Ошибка в данных!", "Данные не заполнены, повторите попытку!");
    } else {
        this->login = ui->lineEdit_login->text();
        this->password = ui->lineEdit_password->text();
        this->role = ui->comboBox_role->currentIndex();
    }
}


void Registration::on_buttonBox_rejected()
{
    this->close();
    this->login = "";
    this->password = "";
    this->role = 0;
}


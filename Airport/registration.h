#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();

    QString login;
    QString password;
    short role;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Registration *ui;
};

#endif // REGISTRATION_H

#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QString>
#include <QMessageBox>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    QString login;
    QString password;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H

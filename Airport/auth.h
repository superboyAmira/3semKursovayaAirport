#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>

#include <QDebug>
#include <QSqlError>

#include "databases.h"
#include "registration.h"
#include "login.h"
#include "adminmenu.h"
#include "passmenu.h"
#include "guardmenu.h"
#include "staffmenu.h"
#include "customermenu.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Auth;
}
QT_END_NAMESPACE

class Auth : public QMainWindow
{
    Q_OBJECT

public:
    Auth(QWidget *parent = nullptr);
    ~Auth();

private slots:
    void on_pushButton_regist_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_test_clicked();

private:
    Ui::Auth *ui;
    Registration reg;
    Login login;

    AdminMenu admin;
    PassMenu pass;
    DataBases bases;
    GuardMenu guard;
    StaffMenu staff;
    CustomerMenu customs;

    QSqlDatabase db;
    QSqlQuery *db_query;
    QSqlTableModel *db_model;

    void openDB();
    void closeDB();
    void createDB();
    void deleteDB();

    void regUser(QString login, QString password, short role);
    bool getUser(QString login);
    bool validateUser(QString login, QString password);
    QString getRole(QString login);
    void addSession(QString login);
    void delSession(QString login);
};
#endif // MAINWINDOW_H

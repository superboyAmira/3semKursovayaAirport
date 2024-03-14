#ifndef PASSMENU_H
#define PASSMENU_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QMessageBox>

#include <QSqlError>
#include <QDebug> // Временно

#include "ticket.h"
#include "route.h"

namespace Ui {
class PassMenu;
}

class PassMenu : public QDialog
{
    Q_OBJECT

public:
    explicit PassMenu(QWidget *parent = nullptr);
    ~PassMenu();

    void openDB();

private slots:
    void on_pushButton_exit_clicked();
    void on_pushButton_refresh_clicked();

private:
    Ui::PassMenu *ui;

    QSqlDatabase db;
    QSqlQuery *db_query;
    QSqlTableModel *db_model_users;
    QSqlTableModel *db_model_routes;
    QSqlTableModel *db_model_tickets;
    QSqlTableModel *db_model_seats;

    void outputAllRoutes();
    void outputMyTickets();
};

#endif // PASSMENU_H

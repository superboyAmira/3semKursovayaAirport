#ifndef ADMINMENU_H
#define ADMINMENU_H

/*
 * This is one of the main classes
 * describing the logic of the airport administrator's work.
 */

#include <chrono> // lib time sales report
#include <ctime> // lib time to str sales report
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include <QDialog>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>

namespace Ui {
class AdminMenu;
}

class AdminMenu : public QDialog
{
    Q_OBJECT

public:
    explicit AdminMenu(QWidget *parent = nullptr);
    ~AdminMenu();

    void openDB();

private slots:

    void on_pushButton_close_clicked();

    void on_pushButton_add_route_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pushButton_setDefault_route_clicked();

    void on_pushButton_clear_route_clicked();

    void on_pushButton_delete_route_clicked();

    void on_tableView_routes_clicked(const QModelIndex &index);

    void on_pushButton_correct_route_clicked();

    void on_pushButton_add_ticket_clicked();

    void on_tableView_tickets_clicked(const QModelIndex &index);

private:
    Ui::AdminMenu *ui;

    QSqlDatabase db;
    QSqlQuery *db_query;
    QSqlTableModel *db_model_routes;
    QSqlTableModel *db_model_tickets;
    QSqlTableModel *db_model_seats;

    /*
     * routes
     */
    int routes_id;
    bool update_routes;

    int getMaxIdRoutes();
    void hideModifiElementsRoutes();
    void showModifiElementsRoutes();

    /*
     * tickets
     */
    int tickets_id;
    bool update_tickets;

    int getMaxIdTickets();

};

#endif // ADMINMENU_H

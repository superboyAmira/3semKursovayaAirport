#ifndef DATABASES_H
#define DATABASES_H

/*
 * this is a class for demonstrating how to work with a database,
 * it does not relate to the functionality of the project in any way
 */

/*
 * https://habr.com/ru/articles/128836/
 * подробное описание взаимодействия qt + sqllite
 */


#include <QDialog>
#include "airport.h"

namespace Ui {
class DataBases;
}

class DataBases : public QDialog
{
    Q_OBJECT

public:
    explicit DataBases(QWidget *parent = nullptr);
    ~DataBases();


    /*
     * Connection DB
     */
    void connectAllDb();
    void closeAllDb();

private slots:

    /*
     * Взаимодействие с БД посредством мыши и
     * кнопок для демонстрации работы с БД
     */

    void on_pushButton_add_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_clear_clicked();

    void on_tableView_all_clicked(const QModelIndex &index);

    void on_pushButton_default_clicked();

    void on_pushButton_clicked();

private:
    Ui::DataBases *ui;
    int users_id;

    QSqlDatabase db;
    QSqlQuery *db_query;
    QSqlTableModel *db_model_users;
    QSqlTableModel *db_model_routes;
    QSqlTableModel *db_model_seats;
    QSqlTableModel *db_model_tickets;
    QSqlTableModel *db_model_users_active;

};

#endif // DATABASES_H

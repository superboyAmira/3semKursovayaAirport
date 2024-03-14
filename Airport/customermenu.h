#ifndef CUSTOMERMENU_H
#define CUSTOMERMENU_H

#include <QDialog>
#include "airport.h"

namespace Ui {
class CustomerMenu;
}

class CustomerMenu : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerMenu(QWidget *parent = nullptr);
    ~CustomerMenu();

    void openDB();

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_accept_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::CustomerMenu *ui;

    int id_table;
    QSqlDatabase db;
    QSqlQuery *db_query;
    QSqlTableModel *db_model;
};

#endif // CUSTOMERMENU_H

#ifndef STAFFMENU_H
#define STAFFMENU_H

#include <QDialog>
#include "airport.h"

namespace Ui {
class StaffMenu;
}

class StaffMenu : public QDialog
{
    Q_OBJECT

public:
    explicit StaffMenu(QWidget *parent = nullptr);
    ~StaffMenu();

    void openDB();

private slots:
    void on_pushButton__exit_clicked();

    void on_tableView_registr_clicked(const QModelIndex &index);

    void on_pushButton_reg_confirm_clicked();

    void on_tableView_fly_clicked(const QModelIndex &index);

    void on_pushButton_fly_confirm_clicked();

private:
    Ui::StaffMenu *ui;

    int id_ticket;
    int id_route;
    QSqlDatabase db;
    QSqlQuery *db_query;
    QSqlTableModel *db_model;
};

#endif // STAFFMENU_H

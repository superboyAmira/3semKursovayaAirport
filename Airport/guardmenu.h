#ifndef GUARDMENU_H
#define GUARDMENU_H

#include <QDialog>

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>

#include <QDebug>
#include <QSqlError>
#include "airport.h"

namespace Ui {
class GuardMenu;
}

class GuardMenu : public QDialog
{
    Q_OBJECT

public:
    explicit GuardMenu(QWidget *parent = nullptr);
    ~GuardMenu();

    void openDB();

private slots:
    void on_pushButton_exit_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_border_control_clicked();

private:
    Ui::GuardMenu *ui;

    int id_table;
    QSqlDatabase db;
    QSqlQuery *db_query;
    QSqlTableModel *db_model;

};

#endif // GUARDMENU_H

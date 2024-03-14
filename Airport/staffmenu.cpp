#include "staffmenu.h"
#include "ui_staffmenu.h"

StaffMenu::StaffMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StaffMenu)
{
    ui->setupUi(this);
}

StaffMenu::~StaffMenu()
{
    delete ui;
}

void StaffMenu::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "./db.db");
    db.setDatabaseName("./db.db");

    if (!db.isOpen()) {
        if (db.open()) {
            qDebug() << "db is open!";
            db_query = new QSqlQuery(db);
            db_model = new QSqlTableModel(this, db);
            db_model->setTable("tickets");
            db_model->select();
            ui->tableView_registr->setModel(db_model);
            ui->tableView_fly->setModel(db_model);
        } else {
            QMessageBox::critical(this, "Ошибка в базе данных!", "Обратитель к разработчику!<users>");
        }
    }
}

void StaffMenu::on_pushButton__exit_clicked()
{
    this->close();
}


void StaffMenu::on_tableView_registr_clicked(const QModelIndex &index)
{
    id_ticket = index.row() + 1;
}


void StaffMenu::on_pushButton_reg_confirm_clicked()
{
    db_query->exec("update tickets set registration_control = true where id = " + QString::number(id_ticket) + ";");
    db_model->setTable("tickets");
    db_model->select();
}


void StaffMenu::on_tableView_fly_clicked(const QModelIndex &index)
{
    id_route = index.row() + 1;
}


void StaffMenu::on_pushButton_fly_confirm_clicked()
{
    db_query->exec("update tickets set leave = true where id = " + QString::number(id_route) + ";");
    db_model->setTable("tickets");
    db_model->select();
}


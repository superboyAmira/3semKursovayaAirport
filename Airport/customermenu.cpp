#include "customermenu.h"
#include "ui_customermenu.h"

CustomerMenu::CustomerMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerMenu)
{
    ui->setupUi(this);
}

CustomerMenu::~CustomerMenu()
{
    delete ui;
}

void CustomerMenu::openDB()
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
            ui->tableView->setModel(db_model);
        } else {
            QMessageBox::critical(this, "Ошибка в базе данных!", "Обратитель к разработчику!<Guard>");
        }
    }
}

void CustomerMenu::on_tableView_clicked(const QModelIndex &index)
{
    id_table = index.row() + 1;
}


void CustomerMenu::on_pushButton_accept_clicked()
{
    db_query->exec("update tickets set baggage_control = true where id = " + QString::number(id_table) + ";");
    db_model->setTable("tickets");
    db_model->select();
}


void CustomerMenu::on_pushButton_exit_clicked()
{
    this->close();
}





#include "databases.h"
#include "ui_databases.h"

#include <QSqlError>

DataBases::DataBases(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataBases)
{
    ui->setupUi(this);
}

DataBases::~DataBases()
{
    delete ui;
}

void DataBases::closeAllDb()
{
    db.close();
}

void DataBases::connectAllDb()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "./db.db");
    db.setDatabaseName("./db.db");

    if (!db.isOpen()) {
        if (db.open()) {
            qDebug() << "db is open!";
            db_query = new QSqlQuery(db);

            db_model_users = new QSqlTableModel(this, db);
            db_model_routes = new QSqlTableModel(this, db);
            db_model_seats = new QSqlTableModel(this, db);
            db_model_tickets = new QSqlTableModel(this, db);
            db_model_users_active = new QSqlTableModel(this, db);
            db_model_users->setTable("users");
            db_model_users->select();
            db_model_seats->setTable("seats");
            db_model_seats->select();
            db_model_routes->setTable("routes");
            db_model_routes->select();
            db_model_tickets->setTable("tickets");
            db_model_tickets->select();
            db_model_users_active->setTable("users_active");
            db_model_users_active->select();

            ui->tableView_all->setModel(db_model_users);
            ui->tableView_routes->setModel(db_model_routes);
            ui->tableView_seats->setModel(db_model_seats);
            ui->tableView_tickets->setModel(db_model_tickets);
            ui->tableView_active->setModel(db_model_users_active);
        } else {
            QMessageBox::critical(this, "Ошибка в базе данных!", "Обратитель к разработчику!<routes>");
        }
    }
}

/*
 * Взаимодействие с БД посредством мыши и
 * кнопок для демонстрации работы с БД
 */

void DataBases::on_pushButton_add_clicked()
{
    db_model_users->insertRow(db_model_users->rowCount());
}

void DataBases::on_pushButton_delete_clicked()
{
    db_model_users->removeRow(users_id);
}


void DataBases::on_pushButton_clear_clicked()
{
    db_model_users->clear();
    db_query->exec("drop table users;");
    db_query->exec("create table users ( id integer primary key, login text, password text, role text );");
    db_model_users->setTable("users");
    db_model_users->select();
}

void DataBases::on_tableView_all_clicked(const QModelIndex &index)
{
    users_id = index.row();
}


void DataBases::on_pushButton_default_clicked()
{
    db_query->exec("INSERT INTO users (login, password, role) VALUES ('user2', 'password2', 'staff'), ('user3', 'password3', 'administrator'), ('user4', 'password4', 'passenger');");
    db_model_users->setTable("users");
    db_model_users->select();
}

void DataBases::on_pushButton_clicked()
{
    if (db_query->exec(ui->lineEdit->text())) {
        while (db_query->next());
            qDebug() << db_query->value(0).toString();
    } else {
        QMessageBox::critical(this, "12", db_query->lastError().text());
    }
}


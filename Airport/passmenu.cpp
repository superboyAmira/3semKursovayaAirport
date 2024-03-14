#include "passmenu.h"
#include "ui_passmenu.h"

PassMenu::PassMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PassMenu)
{
    ui->setupUi(this);
}

PassMenu::~PassMenu()
{
    delete ui;
}

/*
 * ---[DB Settings]---
 */

void PassMenu::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "./db.db");
    db.setDatabaseName("./db.db");

    if (!db.isOpen()) {
        if (db.open()) {
            qDebug() << "db is open!";
            db_query = new QSqlQuery(db);

            db_model_users = new QSqlTableModel(this, db);
            db_model_users->setTable("users");
            db_model_users->select();

            db_model_routes = new QSqlTableModel(this, db);
            db_model_routes->setTable("routes");
            db_model_routes->select();

            db_model_tickets = new QSqlTableModel(this, db);
            db_model_tickets->setTable("tickets");
            db_model_tickets->select();

            db_model_seats = new QSqlTableModel(this, db);
            db_model_seats->setTable("seats");
            db_model_seats->select();
        } else {
            QMessageBox::critical(this, "Ошибка в базе данных!", "Обратитель к разработчику!<routes>");
        }
    }
    outputAllRoutes();
    outputMyTickets();
}

/*
 * ------
 */

/*
 * ---[UI Settings & show dynamic widgets]---
 */

void PassMenu::outputAllRoutes()
{
    db_query->exec("select * from routes");
    while (db_query->next()) {
        Route *route = new Route();
        route->route_id = db_query->value(0).toInt();
        route->setCityFrom(db_query->value(1).toString());
        route->setCityTo(db_query->value(2).toString());
        route->setDateTime(db_query->value(3).toString() + " " + db_query->value(4).toString());
        route->setCntSeats(db_query->value(5).toInt());
        ui->verticalLayout->addWidget(route);
    }
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayout->addSpacerItem(verticalSpacer);
}

void PassMenu::outputMyTickets() {
    QSqlQuery *t_query = new QSqlQuery(db);
    db_query->exec("SELECT u.id "
                             "FROM users u "
                             "JOIN users_active ua ON u.login = ua.username "
                             "GROUP BY u.id "
                             "HAVING COUNT(*) = 1;");
    db_query->next();
    int id = db_query->value(0).toInt();

    QString select = "select * from tickets where id_user = " + QString::number(id) + ";";
    db_query->exec(select);
    while (db_query->next()) {
        QString id_ticket = db_query->value(0).toString();
        QString select_ticket = "SELECT * "
                                "FROM routes r "
                                "INNER JOIN seats s ON r.id = s.id_route "
                                "INNER JOIN tickets t ON s.id = t.id_seat "
                                "WHERE t.id = " + id_ticket + ";";
        t_query->exec(select_ticket);
        t_query->next();
        QString cities = t_query->value(1).toString() + " " + t_query->value(2).toString();
        QString date_time = t_query->value(3).toString() + " " + t_query->value(4).toString();
        Ticket *ticket = new Ticket();
        ticket->setSize(500,200);
        ticket->showMyTickets(cities, date_time);

        if (db_query->value(10).toBool() == true) {
            ticket->ticketAccepted();
        }
        if (db_query->value(8).toBool() == true) {
            ticket->borderControlAccepted();
        }
        if (db_query->value(9).toBool() == true) {
            ticket->customerControlAccepted();
        }
        if (db_query->value(11).toBool() == true) {
            ticket->fly();
        }

        ui->verticalLayout_2->addWidget(ticket);
    }

}
void PassMenu::on_pushButton_exit_clicked()
{
    QLayoutItem* item;
    while ((item = ui->verticalLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    while ((item = ui->verticalLayout_2->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    db.close();
    this->close();
}

void PassMenu::on_pushButton_refresh_clicked() {
    QLayoutItem* item;
    while ((item = ui->verticalLayout_2->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    outputMyTickets();
}

/*
 * ------
 */

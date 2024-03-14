#include "adminmenu.h"
#include "ui_adminmenu.h"


AdminMenu::AdminMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminMenu)
{
    ui->setupUi(this);
    hideModifiElementsRoutes();
}

AdminMenu::~AdminMenu()
{
    delete ui;
}

/*
 * ---[DB settings]---
 */

void AdminMenu::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "./db.db");
    db.setDatabaseName("./db.db");

    if (!db.isOpen()) {
        if (db.open()) {
            qDebug() << "db is open!";
            db_query = new QSqlQuery(db);
            db_model_routes = new QSqlTableModel(this, db);
            db_model_tickets = new QSqlTableModel(this, db);
            db_model_seats = new QSqlTableModel(this, db);
            db_model_seats->setTable("seats");
            db_model_seats->select();
            db_model_tickets->setTable("tickets");
            db_model_tickets->select();
            db_model_routes->setTable("routes");
            db_model_routes->select();
            ui->tableView_routes->setModel(db_model_routes);
            ui->tableView_tickets->setModel(db_model_tickets);
        } else {
            QMessageBox::critical(this, "Ошибка в базе данных!", "Обратитель к разработчику!<routes>");
        }
    }
}

void AdminMenu::on_pushButton_close_clicked()
{
    db.close();
    this->close();
}

/*
 * ------
 */

/*
 * ---[UI Settings]---
 */

void AdminMenu::hideModifiElementsRoutes()
{
    ui->pushButton_correct_route->setDisabled(true);
    update_routes = false;

    ui->pushButton_add_route->setEnabled(true);
    ui->pushButton_clear_route->setEnabled(true);
    ui->pushButton_delete_route->setEnabled(true);
    ui->pushButton_setDefault_route->setEnabled(true);
    ui->spinBox_id_route->setDisabled(true);
    ui->lineEdit_city_from_route->setDisabled(true);
    ui->lineEdit_city_to_route->setDisabled(true);
    ui->spinBox_cnt_seats_route->setDisabled(true);
    ui->dateTimeEdit_route->setDisabled(true);
    ui->label->setDisabled(true);
    ui->label_2->setDisabled(true);
    ui->label_3->setDisabled(true);
    ui->label_9->setDisabled(true);
    ui->label_10->setDisabled(true);
    ui->buttonBox->setDisabled(true);
}

void AdminMenu::showModifiElementsRoutes()
{
    ui->pushButton_add_route->setDisabled(true);
    ui->pushButton_correct_route->setDisabled(true);
    ui->pushButton_clear_route->setDisabled(true);
    ui->pushButton_delete_route->setDisabled(true);
    ui->pushButton_setDefault_route->setDisabled(true);
    ui->lineEdit_city_from_route->setEnabled(true);
    ui->lineEdit_city_to_route->setEnabled(true);
    ui->spinBox_cnt_seats_route->setEnabled(true);
    ui->dateTimeEdit_route->setEnabled(true);
    ui->label->setEnabled(true);
    ui->label_2->setEnabled(true);
    ui->label_3->setEnabled(true);
    ui->label_9->setEnabled(true);
    ui->label_10->setEnabled(true);
    ui->buttonBox->setEnabled(true);
}

/*
 * ------
 */

/*
 * ---[UI-SLOTS for Routes]---
 */

/*
 * Main slot for adding or changing a route, with appropriate sql queries.
 */

void AdminMenu::on_buttonBox_accepted()
{
    if (!update_routes) {
        int id = getMaxIdRoutes() + 1;
        db_query->prepare("INSERT INTO routes (id ,city_from, city_to, date_takeoff, time_takeoff, cnt_seats)"
                              " VALUES (:id, :city_from, :city_to, :date_takeoff, :time_takeoff, :cnt_seats);");
        db_query->bindValue(":id", id);
        db_query->bindValue(":city_from", ui->lineEdit_city_from_route->text());
        db_query->bindValue(":city_to", ui->lineEdit_city_to_route->text());
        db_query->bindValue(":date_takeoff", ui->dateTimeEdit_route->date().toString());
        db_query->bindValue(":time_takeoff", ui->dateTimeEdit_route->time().toString());
        db_query->bindValue(":cnt_seats", ui->spinBox_cnt_seats_route->value());
        if (!db_query->exec()) {
            QMessageBox::critical(this, "Oops!", "Ошибка добавления в базу данных, повторите попытку!");
        } else {
            db_model_routes->setTable("routes");
            db_model_routes->select();

            // добавление в бд набора мест автоматически
            db_query->exec("select cnt_seats from routes where id = " + QString::number(id) + ";");
            db_query->next();
            for (int i = 1; i <= db_query->value(0).toInt(); i++) {
                db_query->prepare("insert into seats (id_route) values (:id_route);");
                db_query->bindValue(":id_route", id);
                db_query->exec();
                // qDebug() << db_query->lastError();
            }
            db_model_seats->setTable("seats");
            db_model_seats->select();
        }
    } else {
        db_query->prepare("UPDATE routes SET id=:id, city_from=:city_from, city_to=:city_to, "
                              "date_takeoff=:date_takeoff, time_takeoff=:time_takeoff, "
                              "cnt_seats=:cnt_seats WHERE id=:id");
        db_query->bindValue(":id", ui->spinBox_id_route->value());
        db_query->bindValue(":city_from", ui->lineEdit_city_from_route->text());
        db_query->bindValue(":city_to", ui->lineEdit_city_to_route->text());
        db_query->bindValue(":date_takeoff", ui->dateTimeEdit_route->date().toString());
        db_query->bindValue(":time_takeoff", ui->dateTimeEdit_route->time().toString());
        db_query->bindValue(":cnt_seats", ui->spinBox_cnt_seats_route->value());
        if (!db_query->exec()) {
            QMessageBox::critical(this, "Oops!", "Ошибка добавления в базу данных, повторите попытку!<UPDATE>");
        } else {
            db_model_routes->setTable("routes");
            db_model_routes->select();
        }
    }
    hideModifiElementsRoutes();
}


void AdminMenu::on_buttonBox_rejected()
{
    hideModifiElementsRoutes();
}


void AdminMenu::on_pushButton_setDefault_route_clicked()
{

    db_query->exec("INSERT INTO routes (id, city_from, city_to, date_takeoff, time_takeoff, cnt_seats) "
                       "VALUES (1, 'Moscow', 'St. Petersburg', '2023-09-01', '12:00:00', 120),"
                       "(2, 'Moscow', 'Yaroslavl', '2023-12-12', '13:44:00', 150);");
    db_model_routes->setTable("routes");
    db_model_routes->select();

    for (int i = 1; i <= 120; i++) {
        db_query->prepare("insert into seats (id_route) values (:id_route);");
        db_query->bindValue(":id_route", 1);
        db_query->exec();
    }
    for (int i = 1; i <= 150; i++) {
        db_query->prepare("insert into seats (id_route) values (:id_route);");
        db_query->bindValue(":id_route", 2);
        db_query->exec();
    }
    db_model_seats->setTable("seats");
    db_model_seats->select();
}


void AdminMenu::on_pushButton_clear_route_clicked()
{
    db_query->exec("delete from seats;");
    db_model_seats->setTable("seats");
    db_model_seats->select();
    db_query->exec("delete from routes;");
    db_model_routes->setTable("routes");
    db_model_routes->select();
}


void AdminMenu::on_pushButton_delete_route_clicked()
{
    db_query->exec("DELETE FROM seats WHERE id_route = " + QString::number(routes_id + 1) + ";");
    db_model_seats->setTable("seats");
    db_model_seats->select();

    db_model_routes->removeRow(routes_id);
    db_model_routes->select();
}


void AdminMenu::on_tableView_routes_clicked(const QModelIndex &index)
{
    ui->pushButton_correct_route->setEnabled(true);
    routes_id = index.row();
}


void AdminMenu::on_pushButton_correct_route_clicked()
{
    showModifiElementsRoutes();
    QString select = "select * from routes where id = " + QString::number(routes_id + 1);
    db_query->exec(select);
    db_query->next();
    ui->spinBox_id_route->setValue(db_query->value(0).toInt());
    ui->lineEdit_city_from_route->setText(db_query->value(1).toString());
    ui->lineEdit_city_to_route->setText(db_query->value(2).toString());
    ui->dateTimeEdit_route->setDate(db_query->value(3).toDate());
    ui->dateTimeEdit_route->setTime(db_query->value(4).toTime());
    ui->spinBox_cnt_seats_route->setValue(db_query->value(5).toInt());
    ui->spinBox_cnt_seats_route->setDisabled(true);
    update_routes = true;
}

int AdminMenu::getMaxIdRoutes()
{
    int id = 0;
    db_query->exec("SELECT MAX(id) FROM routes");
    if (db_query->next()) {
        // qDebug() << routes_query->value(0);
        QVariant value = db_query->value(0);
        if (value.isNull()) {
            id = 1;
        } else {
            id = value.toInt();
        }
    }
    return id;
}

void AdminMenu::on_pushButton_add_route_clicked()
{
    showModifiElementsRoutes();
    int id = getMaxIdRoutes() + 1;
    ui->spinBox_id_route->setValue(id);
}

/*
 * ------
 */

/*
 * ---[UI-SLOTs for tickets]---
 */

int AdminMenu::getMaxIdTickets()
{
    int id = 0;
    db_query->exec("SELECT MAX(id) FROM tickets");
    if (db_query->next()) {
        // qDebug() << db_query->value(0);
        QVariant value = db_query->value(0);
        if (value.isNull()) {
            id = 1;
        } else {
            id = value.toInt();
        }
    }
    return id;
}

/*
 * ------
 */

void AdminMenu::on_pushButton_add_ticket_clicked()
{
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(&local_tm, "%Y_%m_%d_%H_%M_%S");
    std::string time_date = ss.str();

    std::ofstream file_sale;
    std::string file_name = "../sales_report_" + time_date + ".txt";
    file_sale.open(file_name);
    if (file_sale.is_open()) {
        db_query->exec("select * from tickets where id = " + QString::number(tickets_id));
        db_query->next();
        QString inserted = "id_ticket = " + db_query->value(0).toString() + "; id_user = " + db_query->value(1).toString() + "; name_passeger = " + db_query->value(2).toString() + "; lastname_passenger = " + db_query->value(3).toString() + "; passport_data = " + db_query->value(5).toString() + "; id_seat = " + db_query->value(7).toString();
        file_sale << inserted.toStdString();
        qDebug() << "file created <" + file_name + ">";
    } else {
        qDebug() << "sale report file error!";
    }
    file_sale.close();
}


void AdminMenu::on_tableView_tickets_clicked(const QModelIndex &index)
{
    tickets_id = index.row() + 1;
    qDebug() << tickets_id;
}


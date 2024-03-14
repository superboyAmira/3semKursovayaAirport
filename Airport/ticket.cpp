#include "ticket.h"
#include "ui_ticket.h"

Ticket::Ticket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ticket)
{
    ui->setupUi(this);
}

Ticket::~Ticket()
{
    delete ui;
}

/*
 * ---[DB Settings]---
 */

void Ticket::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "./db.db");
    db.setDatabaseName("./db.db");

    if (!db.isOpen()) {
        if (db.open()) {
            qDebug() << "db is open!";
            db_query = new QSqlQuery(db);
            db_model_routes = new QSqlTableModel(this, db);
            db_model_routes->setTable("routes");
            db_model_routes->select();

            db_model_tickets = new QSqlTableModel(this, db);
            db_model_tickets->setTable("tickets");
            db_model_tickets->select();

            db_model_seats = new QSqlTableModel(this, db);
            db_model_seats->setTable("seats");
            db_model_seats->select();

            db_model_users_active = new QSqlTableModel(this, db);
            db_model_users_active->setTable("users_active");
            db_model_users_active->select();
            // ui->tableView_tickets->setModel(users_model);
        } else {
            QMessageBox::critical(this, "Ошибка в базе данных!", "Обратитель к разработчику!<Ticket>");
        }
    }
}

/*
 * ------
 */

/*
 * ---[Buy Settings]---
 */

void Ticket::showBuyMethods(int id_route)
{
    /*
     * ui settings
     */
    ui->tabWidget->setTabVisible(0, true);
    ui->tabWidget->setTabVisible(1, false);

    db_query->exec("select * from routes where id =" + QString::number(id_route) + ";");
    db_query->next();
    ui->label_id_route->setText(QString::number(id_route));
    ui->label_buy_city_from->setText(db_query->value(1).toString());
    ui->label_buy_city_to->setText(db_query->value(2).toString());
    ui->label_buy_date->setText(db_query->value(3).toString());
    ui->label_buy_time->setText(db_query->value(4).toString());
    ui->label_buy_id_seat->setText("None");

    /*
     * seats visual
     */
    QSqlQuery *t_query = new QSqlQuery(db);

    db_query->exec("select cnt_seats from routes where id = " + QString::number(id_route) + ";");
    db_query->next();
    int seat_id = 1;
    for (int i = 0; i < db_query->value(0).toInt() / 6; i++) {
        for (int j = 0; j < 7; j++) {
            QPushButton *tmp = new QPushButton(QString::number(seat_id));
            tmp->setMaximumSize(40, 40);
            if (j == 3) {
                ui->gridLayout_buy_seats->addItem(new QSpacerItem(50, 20, QSizePolicy::Minimum, QSizePolicy::Expanding), i, j);
                continue;
            }
            ui->gridLayout_buy_seats->addWidget(tmp, i, j);
            connect(tmp, &QPushButton::clicked, this, [this, seat_id]() { selectSeat(seat_id); });

            // setDisable if purchaised
            t_query->exec("select * from seats where id_route = " + QString::number(id_route) + ";");
            t_query->next();
            t_query->exec("select * from seats where id =" + QString::number(seat_id + t_query->value(0).toInt() - 1) + ";");
            t_query->next();

            if (t_query->value(2).toBool() == false) tmp->setDisabled(true);
            seat_id++;
        }
    }
}

void Ticket::on_buttonBox_rejected()
{
    QLayoutItem* item;
    while ((item = ui->gridLayout_buy_seats->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    db.close();
    this->close();
}

void Ticket::selectSeat(int seat_id)
{
    ui->label_buy_id_seat->setText(QString::number(seat_id));
}

void Ticket::on_buttonBox_accepted()
{
    /*
     * Поиск текущего покупателя
     */
    db_query->exec("SELECT u.id "
                             "FROM users u "
                             "JOIN users_active ua ON u.login = ua.username "
                             "GROUP BY u.id "
                             "HAVING COUNT(*) = 1;");
    db_query->next();
    int id = db_query->value(0).toInt();

    /*
     * поиск места которое он выбрал
     */

    QSqlQuery *t_query = new QSqlQuery(db);
    t_query->exec("select id from seats where id_route = " + ui->label_id_route->text() + ";");
    t_query->next();

    db_query->prepare("insert into tickets (id_user, name, lastname, gender, passport_data, baggage, id_seat) "
                        "values (:id_user, :name, :lastname, :gender, :passport_data, :baggage, :id_seat)");

    db_query->bindValue(":id_user", id);
    db_query->bindValue(":name", ui->lineEdit_name->text());
    db_query->bindValue(":lastname", ui->lineEdit_lastname->text());
    (ui->comboBox_buy_gender->currentText() == "Мужской") ? db_query->bindValue(":gender", "M") : db_query->bindValue(":gender", "F");
    db_query->bindValue(":passport_data", ui->lineEdit_buy_passport->text());
    (ui->comboBox_buy_baggage->currentText() == "Да") ? db_query->bindValue(":baggage", "true") : db_query->bindValue(":baggage", "false");
    db_query->bindValue(":id_seat",  QString::number(ui->label_buy_id_seat->text().toInt() + t_query->value(0).toInt() - 1));
    db_query->exec();
    qDebug() << db_query->lastError();
    db_model_seats->setTable("tickets");
    db_model_seats->select();
    this->close();

    /*
     * Отметим что место занято
     */
    db_query->exec("UPDATE seats "
                   "SET status = false "
                   "WHERE id = " + QString::number(ui->label_buy_id_seat->text().toInt() + t_query->value(0).toInt() - 1) + ";");
}

/*
 * ------
 */

/*
 * Show Tickets
 */

void Ticket::showMyTickets(QString cities, QString date_time)
{
    ui->label_waiting->setText("Ожиадние прохождения регистрации в самолет!");
    ui->tabWidget->setTabVisible(1, true);
    ui->tabWidget->setTabVisible(0, false);
    ui->pushButton_my_passport_control->setDisabled(true);
    ui->pushButton_my_customer_control->setDisabled(true);
    ui->pushButton_my_fly->setDisabled(true);
    ui->label_my_city_from_to->setText(cities);
    ui->label_my_date_time->setText(date_time);
}

void Ticket::ticketAccepted()
{
    ui->label_waiting->setText("Ожидание прохождения пасспортного контроля!");
    ui->pushButton_my_passport_control->setEnabled(true);
}

void Ticket::borderControlAccepted()
{
    ui->label_waiting->setText("Ожидание прохождения таможенного контроля!");
    ui->pushButton_my_customer_control->setEnabled(true);
}

void Ticket::customerControlAccepted()
{
    ui->label_waiting->setText("Можно садиться на рейс!");
    ui->pushButton_my_fly->setEnabled(true);
}

void Ticket::fly() {
    ui->label_waiting->setText("Улетели, молодцы!!!!");
    ui->pushButton_my_customer_control->setDisabled(true);
    ui->pushButton_my_fly->setDisabled(true);
    ui->pushButton_my_passport_control->setDisabled(true);
}

void Ticket::setSize(int w, int h) {
    setFixedSize(w, h);
}

void Ticket::on_pushButton_my_passport_control_clicked()
{
    QMessageBox::information(this, "Информация", "Вы зарегистрированны на самолет, Ожидайте ответа пограничника!");
}

void Ticket::on_pushButton_my_customer_control_clicked()
{
    QMessageBox::information(this, "Информация", "Ожидайте ответа таможенника!");
}

void Ticket::on_pushButton_my_fly_clicked()
{
    QMessageBox::information(this, "Информация", "Вы в самолете!");
    ui->pushButton_my_customer_control->setDisabled(true);
    ui->pushButton_my_fly->setDisabled(true);
    ui->pushButton_my_passport_control->setDisabled(true);
    ui->label_waiting->setText("Готовы к вылету!");
}

/*
 * ------
 */

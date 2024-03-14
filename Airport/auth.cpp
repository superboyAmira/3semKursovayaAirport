#include "auth.h"
#include "ui_auth.h"

Auth::Auth(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Auth)
{
    ui->setupUi(this);
    openDB();
}

Auth::~Auth()
{
    closeDB();
    delete ui;
}

/*
 * DB Settings
 */

void Auth::openDB() {
    db = QSqlDatabase::addDatabase("QSQLITE", "./db.db");
    db.setDatabaseName("./db.db");

    if (!db.isOpen()) {
        if (db.open()) {
            qDebug() << "db is open!";
            db_query = new QSqlQuery(db);

            // deleteDB();
            createDB();

            db_model = new QSqlTableModel(this, db);
        } else {
            QMessageBox::critical(this, "Ошибка в базе данных!", "Обратитель к разработчику!<users>");
        }
    }
}

void Auth::closeDB() {
    qDebug() << "db is closed!";
    db.close();
}

/*
 * Default setting for DB. It is used only when the project is first
 * launched on a new device to initialize and create tables on local media.
 * It would be possible to implement this on Docker, but due to the size of
 * the project and the lack of plans for further support, we did not use it.
 */

void Auth::createDB() {
    db_query->exec("create table users ( id integer primary key, login text, password text, role text );");
    db_query->exec("create table users_active (username text);");
    db_query->exec("create table tickets"
                    " (id integer primary key, id_user integer,"
                    " name text, lastname text,"
                    " gender text, passport_data text,"
                    " baggage bool, id_seat integer,"
                    " border_control bool default false, baggage_control bool default false,"
                    " registration_control bool default false, leave bool default false,"
                    " FOREIGN KEY (id_user)  REFERENCES users (id),"
                    " FOREIGN KEY (id_seat)  REFERENCES seats (id) );");
    db_query->exec("create table seats ( id integer primary key, id_route integer, status bool default true, FOREIGN KEY (id_route)  REFERENCES routes (id) );");
    db_query->exec("create table routes ( id integer primary key AUTOINCREMENT, city_from text, city_to text, date_takeoff date, time_takeoff time, cnt_seats integer );");
}

/*
 * Default setting for DB. Delete datas in DB.
 */

void Auth::deleteDB() {
    db_query->exec("drop table tickets");
}

/*
 * The function is used to add a user during registration.
 */

void Auth::regUser(QString login, QString password, short role) {
    db_query->prepare("INSERT INTO users (login, password, role)"
                                  "VALUES (:login, :password, :role);");
    db_query->bindValue(":login", login);
    db_query->bindValue(":password", password);
    switch (role) {
        case 0:
            db_query->bindValue(":role", "passenger");
            break;
        case 1:
            db_query->bindValue(":role", "administrator");
            break;
        case 2:
            db_query->bindValue(":role", "staff");
            break;
        case 3:
            db_query->bindValue(":role", "customs");
            break;
        case 4:
            db_query->bindValue(":role", "border");
            break;
    }
    db_query->exec();
}

/*
 * The function is used to verify the existence of a user in the database.
 */

bool Auth::getUser(QString login)
{
    QString select = "select login, password, role from users where login = '" + login + "';";
    bool status = true;
    status = db_query->exec(select);
    status = db_query->next();
    return status;
}

/*
 * The function is used to verify the correctness of the user's data in the database.
 */

bool Auth::validateUser(QString login, QString password)
{
    QString select = "select login, password, role from users where login = '" + login + "';";
    bool status = true;
    status = db_query->exec(select);
    status = db_query->next();

    status = (login == db_query->value(0).toString() && password == db_query->value(1).toString()) ? true : false;
    return status;
}

/*
 * The function is used to get the user role.
 * (it is necessary to open the correct level of access to the database).
 */

QString Auth::getRole(QString login)
{
    QString select = "select login, password, role from users where login = '" + login + "';";
    QString status = "";
    db_query->exec(select);
    db_query->next();
    status = db_query->value(2).toString();

    return status;
}

/*
 * The function adds a logged-in user.
 * (Notes this in the corresponding table).
 */

void Auth::addSession(QString login) {
    db_query->exec("insert into users_active (username) values ('" + login + "');");
    qDebug() << login + " log in";
}

/*
 * The function deletes a logged-in user.
 * (Notes this in the corresponding table).
 */

void Auth::delSession(QString login)
{
    db_query->exec("delete from users_active where username = '" + login + "';");
    qDebug() << login + " log out";
}

/*
 * Regist
 */

void Auth::on_pushButton_regist_clicked()
{
    this->hide();
    reg.exec();
    if (reg.login != "") {
        regUser(reg.login, reg.password, reg.role);
        QMessageBox::information(this, "Успех!", "Регистрация завершена, войдите в систему!");
    }
    this->show();
}

/*
 * Login
 */

void Auth::on_pushButton_login_clicked()
{
    this->hide();
    login.exec();

    if (login.login != "") {
        if (!getUser(login.login)) {
            QMessageBox::critical(this, "Необходима регистрация в системе", "Пользователь не найден, пройдите регистрацию!");
        }
        if (!validateUser(login.login, login.password)) {
            QMessageBox::critical(this, "Пароль не верен", "Неверный пароль, попробуйте еще разочек!");
        } else {
            QMessageBox::information(this, "Успешно", "Вход выполнен!");

            QString role = getRole(login.login);
            addSession(login.login);
            closeDB();
            if (role == "administrator") {
                admin.openDB();
                admin.exec();
            } else if (role == "passenger") {
                pass.openDB();
                pass.exec();
            } else if (role == "border") {
                guard.openDB();
                guard.exec();
            } else if (role == "staff") {
                staff.openDB();
                staff.exec();
            } else if (role == "customs") {
                customs.openDB();
                customs.exec();
            }
            openDB();
            delSession(login.login);
        }
    }


    this->show();
}


void Auth::on_pushButton_test_clicked()
{
    closeDB();
    bases.connectAllDb();
    bases.exec();
    bases.closeAllDb();
    openDB();
}


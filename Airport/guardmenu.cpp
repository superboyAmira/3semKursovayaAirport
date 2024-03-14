#include "guardmenu.h"
#include "ui_guardmenu.h"

GuardMenu::GuardMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GuardMenu)
{
    ui->setupUi(this);
}

GuardMenu::~GuardMenu()
{
    delete ui;
}

void GuardMenu::openDB()
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

void GuardMenu::on_pushButton_exit_clicked()
{
    this->close();
}

void GuardMenu::on_tableView_clicked(const QModelIndex &index)
{
    id_table = index.row() + 1;
}



void GuardMenu::on_pushButton_border_control_clicked()
{
    db_query->exec("update tickets set border_control = true where id = " + QString::number(id_table) + ";");
    db_model->setTable("tickets");
    db_model->select();
}


#include "route.h"
#include "ui_route.h"

Route::Route(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Route)
{
    ui->setupUi(this);
    update = false;
}

Route::~Route()
{
    delete ui;
}

/*
 * ---[UI-SLOTs & UI Settings]---
 */

void Route::setCityFrom(QString city)
{
    ui->label_city_from->setText(city);
}

void Route::setCityTo(QString city)
{
    ui->label_city_to->setText(city);
}

void Route::setDateTime(QString date_time)
{
    ui->labe_dateTime->setText(date_time);
}

void Route::setCntSeats(int cnt_seats)
{
    ui->labe_cnt_seats->setText(QString::number(cnt_seats));
}


void Route::on_pushButton_buy_clicked()
{
    buy_ticket.openDB();
    buy_ticket.showBuyMethods(route_id);
    buy_ticket.exec();
}

/*
 * ------
 */


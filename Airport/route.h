#ifndef ROUTE_H
#define ROUTE_H

#include <QWidget>
#include <QDebug>
#include "ticket.h"

namespace Ui {
class Route;
}

class Route : public QWidget
{
    Q_OBJECT

public:
    explicit Route(QWidget *parent = nullptr);
    ~Route();
    int route_id;
    bool update;

    /*
     * setters and getters
     */
    void setCityFrom(QString city);
    void setCityTo(QString city);
    void setDateTime(QString date_time);
    void setCntSeats(int cnt_seats);

private slots:
    void on_pushButton_buy_clicked();

private:
    Ui::Route *ui;
    Ticket buy_ticket;
};

#endif // ROUTE_H

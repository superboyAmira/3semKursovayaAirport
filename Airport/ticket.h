#ifndef TICKET_H
#define TICKET_H

#include <QDialog>
#include "airport.h"

namespace Ui {
class Ticket;
}

class Ticket : public QDialog
{
    Q_OBJECT

public:
    explicit Ticket(QWidget *parent = nullptr);
    ~Ticket();

    void openDB();
    void hidePurchasedSeats();
    void showBuyMethods(int);
    void showMyTickets(QString, QString);
    void ticketAccepted();
    void borderControlAccepted();
    void customerControlAccepted();
    void fly();
    void setSize(int, int);


private slots:
    void on_buttonBox_rejected();

    void selectSeat(int seat_id);

    void on_buttonBox_accepted();

    void on_pushButton_my_passport_control_clicked();

    void on_pushButton_my_customer_control_clicked();

    void on_pushButton_my_fly_clicked();

private:
    Ui::Ticket *ui;

    QSqlDatabase db;
    QSqlQuery *db_query;
    QSqlTableModel *db_model_routes;
    QSqlTableModel *db_model_tickets;
    QSqlTableModel *db_model_seats;
    QSqlTableModel *db_model_users_active;

};

#endif // TICKET_H

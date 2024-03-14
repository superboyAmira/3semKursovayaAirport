#include "auth.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Auth w;
    w.show();
    return a.exec();
}

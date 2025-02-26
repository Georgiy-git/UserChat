#include "client.hpp"

#include <QApplication>
#include "authorization.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //setlocale(LC_ALL, "RU");

    Authorization* w = new Authorization;
    w->show();

    return a.exec();
}

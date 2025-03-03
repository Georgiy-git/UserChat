#include "client.hpp"

#include <QApplication>
#include "authorization.hpp"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "RU");
    QApplication a(argc, argv);

    Authorization* w = new Authorization;
    w->setWindowIcon(QIcon(":/Images/mine.svg"));
    w->show();

    return a.exec();
}

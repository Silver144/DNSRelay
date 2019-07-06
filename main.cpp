#include <QtCore/QCoreApplication>
#include "server.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DNSRelay t(argc, argv);
    return a.exec();
}

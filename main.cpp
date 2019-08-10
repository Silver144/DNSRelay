#include <QtCore/QCoreApplication>
#include "server.h"
#include <QDebug>

std::int32_t main(std::int32_t argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	try {
		DNSRelay t(argc, argv);
	}
	catch (std::string msg) {
		std::cout << msg;
		exit(0);
	}
    return a.exec();
}

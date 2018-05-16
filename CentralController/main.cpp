#include "view/mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "interface/CanBus.h"
#include "interface/ModBus.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    CanBus *pCanBus = new CanBus("can-bus");
    qDebug()<< pCanBus->getBusName();
    pCanBus->readData();
    pCanBus->writeData();

    ModBus *pModBus = new ModBus("mod-bus");
    qDebug()<< pModBus->getBusName();
    pModBus->readData();
    pModBus->writeData();

    IBus* bus;
    bus = static_cast<IBus*>(pModBus);
    qDebug()<< bus->getBusName();
    bus->readData();
    bus->writeData();
    bus = static_cast<IBus*>(pCanBus);
    qDebug()<< bus->getBusName();
    bus->readData();
    bus->writeData();

    return a.exec();
}

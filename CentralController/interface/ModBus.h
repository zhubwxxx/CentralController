#ifndef MODBUS_H
#define MODBUS_H

#include "IBus.h"
#include "protocol/PModbus.h"

class ModBus : public IBus,PModbus
{
public:
    ModBus(QString busName);

    virtual void readData();
    virtual void writeData();
};

#endif // MODBUS_H

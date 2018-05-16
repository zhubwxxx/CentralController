#ifndef CANBUS_H
#define CANBUS_H

#include "IBus.h"
#include "protocol/PCan.h"

class CanBus : public IBus,PCan
{
public:
    CanBus(QString busName);

    virtual void readData();
    virtual void writeData();
};

#endif // CANBUS_H

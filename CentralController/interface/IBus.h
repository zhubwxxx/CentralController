#ifndef IBUS_H
#define IBUS_H

#include "3rdparty/qtbase.h"

class IBus
{
public:
    IBus(QString busName);

    void setBusName(QString busName);
    QString getBusName();

    virtual void readData() = 0;
    virtual void writeData() = 0;

private:
    QString m_busName;
};

#endif // IBUS_H

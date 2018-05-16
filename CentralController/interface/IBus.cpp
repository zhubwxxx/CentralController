#include "IBus.h"

IBus::IBus(QString busName)
{
    setBusName(busName);
}

void IBus::setBusName(QString busName)
{
    m_busName = busName;
}

QString IBus::getBusName()
{
    return m_busName;
}

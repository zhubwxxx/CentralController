#include "CanBus.h"

CanBus::CanBus(QString busName) :
    IBus(busName),
    PCan()
{

}

void CanBus::readData()
{
    Debug<< __FILE__ << __FUNCTION__;
}

void CanBus::writeData()
{
    Debug<< __FILE__ << __FUNCTION__;
}

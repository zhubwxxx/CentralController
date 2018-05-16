#include "ModBus.h"

ModBus::ModBus(QString busName) :
    IBus(busName),
    PModbus()
{

}

void ModBus::readData()
{
    Debug<< __FILE__ << __FUNCTION__;

}

void ModBus::writeData()
{
    Debug<< __FILE__ << __FUNCTION__;
}


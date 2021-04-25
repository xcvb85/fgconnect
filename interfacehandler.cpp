#include <QDebug>

#include "interfacehandler.h"
#include "interface.h"
#include "interfacefdm.h"

InterfaceHandler::InterfaceHandler(QObject *parent)
	: QObject(parent)
{
	Interfaces.append(new InterfaceFdm(this));

	foreach(Interface* interface, Interfaces)
	{
		SignalNames.append(interface->getSignalNames());
	}
}

bool InterfaceHandler::getSignal(QString requestedSignal, Interface* &signalInterface, int &index)
{
	bool retval = false;

	foreach(Interface* interface, Interfaces)
	{
		index = interface->getSignal(requestedSignal);
		if(index >= 0)
		{
			signalInterface = interface;
			retval = true;
			break;
		}
	}
	return retval;
}

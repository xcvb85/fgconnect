#ifndef INTERFACEHANDLER_H
#define INTERFACEHANDLER_H

#include <QObject>

class Interface;

class InterfaceHandler : public QObject
{
	Q_OBJECT

public:
	InterfaceHandler(QObject *parent = 0);
	QStringList getSignalNames() { return SignalNames; }
	bool getSignal(QString requestedSignal, Interface* &signalInterface, int &index);

private:
	QList<Interface*> Interfaces;
	QStringList SignalNames;
};

#endif // INTERFACEHANDLER_H

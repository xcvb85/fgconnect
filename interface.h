#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>

class Interface : public QObject
{
	Q_OBJECT

public:
	explicit Interface(QObject *parent = 0);
	virtual QStringList getSignalNames() = 0;
	virtual int getSignal(QString requestedSignal) = 0;
	virtual qint16 getValue(quint8 position) = 0;
	virtual void setValue(quint8 position, qint16 value) = 0;
};

#endif // CONNECTOR_H

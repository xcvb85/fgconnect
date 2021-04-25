#ifndef INTERFACEFDM_H
#define INTERFACEFDM_H

#include <QUdpSocket>

#include "interface.h"

class InterfaceFdm : public Interface
{
	Q_OBJECT

	enum FdmSignals
	{
		SignalLongitude = 0,
		SignalLatitude,
		SignalBaroAltitude,
		SignalRadarAltitude,
		SignalRoll,
		SignalPitch,
		SignalYaw,
		SignalAlpha,
		SignalCount
	};

public:
	explicit InterfaceFdm(QObject *parent = 0);
	~InterfaceFdm();

	QStringList getSignalNames() { return SignalNames; }
	int getSignal(QString requestedSignal);
	qint16 getValue(quint8 position);
	void setValue(quint8 position, qint16 value);

private slots:
	void processPendingDatagrams();

private:
	float ReverseFloat(float inFloat);
	double ReverseDouble(double inDouble);

	QUdpSocket *socket;
	QStringList SignalNames;

	short data[SignalCount];
};

#endif // INTERFACEFDM_H

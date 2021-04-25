#include <QUdpSocket>
#include <QtEndian>
#include <QDebug>

#include <cmath>

#include "interfacefdm.h"
#include "net_fdm.hxx"

// ------------------------------------------------------------------------------------------------
InterfaceFdm::InterfaceFdm(QObject */*parent*/)
{
	SignalNames << "Longitude" << "Latitude" << "BaroAltitude" << "RadarAltitude" << "Roll" << "Pitch" << "Yaw" << "Alpha";

	socket = new QUdpSocket(this);
	socket->bind(QHostAddress::Any, 5500);

	processPendingDatagrams();
	connect(socket, &QUdpSocket::readyRead, this, &InterfaceFdm::processPendingDatagrams, Qt::QueuedConnection);
}

// ------------------------------------------------------------------------------------------------
InterfaceFdm::~InterfaceFdm()
{
}

// ------------------------------------------------------------------------------------------------
int InterfaceFdm::getSignal(QString requestedSignal)
{
	int index = -1;

	for(int i=0; i<SignalNames.size(); i++)
	{
		if(SignalNames.at(i) == requestedSignal)
		{
			index = i;
			break;
		}
	}
	return index;
}

// ------------------------------------------------------------------------------------------------
qint16 InterfaceFdm::getValue(quint8 position)
{
	if(position < SignalCount)
	{
		return data[position];
	}
	return 0;
}

// ------------------------------------------------------------------------------------------------
void InterfaceFdm::setValue(quint8 position, qint16 value)
{
	Q_UNUSED(position);
	Q_UNUSED(value);
}

// ------------------------------------------------------------------------------------------------
float InterfaceFdm::ReverseFloat(float inFloat)
{
	float retVal;
	char *floatToConvert = reinterpret_cast<char*>(&inFloat);
	char *returnFloat = reinterpret_cast<char*>(&retVal);

	// swap the bytes into a temporary buffer
	returnFloat[0] = floatToConvert[3];
	returnFloat[1] = floatToConvert[2];
	returnFloat[2] = floatToConvert[1];
	returnFloat[3] = floatToConvert[0];

	return retVal;
}

// ------------------------------------------------------------------------------------------------
double InterfaceFdm::ReverseDouble(double inDouble)
{
	double retVal;
	char *doubleToConvert = reinterpret_cast<char*>(&inDouble);
	char *returnDouble = reinterpret_cast<char*>(&retVal);

	// swap the bytes into a temporary buffer
	returnDouble[0] = doubleToConvert[7];
	returnDouble[1] = doubleToConvert[6];
	returnDouble[2] = doubleToConvert[5];
	returnDouble[3] = doubleToConvert[4];
	returnDouble[4] = doubleToConvert[3];
	returnDouble[5] = doubleToConvert[2];
	returnDouble[6] = doubleToConvert[1];
	returnDouble[7] = doubleToConvert[0];

	return retVal;
}

// ------------------------------------------------------------------------------------------------
void InterfaceFdm::processPendingDatagrams()
{
	QHostAddress sender;
	quint16 port;

	while(socket->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(socket->pendingDatagramSize());
		socket->readDatagram(datagram.data(), datagram.size(), &sender, &port);
		FGNetFDM* fdm = reinterpret_cast<FGNetFDM*>(datagram.data());

		data[SignalLongitude] = 10000*ReverseDouble(fdm->longitude);
		data[SignalLatitude] = 10000*ReverseDouble(fdm->latitude);
		data[SignalBaroAltitude] = 0.1*ReverseDouble(fdm->altitude);
		data[SignalRadarAltitude] = 0.1*ReverseFloat(fdm->agl);
		data[SignalRoll] = 100*(180.0*ReverseFloat(fdm->phi))/M_PI;
		data[SignalPitch] = 100*(180.0*ReverseFloat(fdm->theta))/M_PI;
		data[SignalYaw] = 100*(180.0*ReverseFloat(fdm->psi))/M_PI;
		data[SignalAlpha] = 100*(180.0*ReverseFloat(fdm->alpha))/M_PI;
	}
}

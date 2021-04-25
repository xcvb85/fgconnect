#ifndef DEVICEITEMMODEL_H
#define DEVICEITEMMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include "deviceitem.h"

class QDomNode;
class InterfaceHandler;

class DeviceItemModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	DeviceItemModel(InterfaceHandler* handler, QObject *parent = 0);
	~DeviceItemModel();

	QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	bool setData(const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole) Q_DECL_OVERRIDE;

	QModelIndex index(int row, int column,
					  const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

	bool insertRows(int position, int rows,
					const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
	bool removeRows(int position, int rows,
					const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

	void addDevice();

	void readXml(QString fileName);
	void writeXml(QString fileName);

public slots:
	void trigger();

private:
	void readXml(DeviceItem *parent, QDomNode node);

	DeviceItem *getItem(const QModelIndex &index) const;
	DeviceItem *rootItem;

signals:
	void ModbusData(int id, QList<qint16> data);
};

#endif // DEVICEITEMMODEL_H

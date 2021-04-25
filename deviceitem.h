#ifndef DEVICEITEM_H
#define DEVICEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

enum DeviceItemRoles
{
	ItemFlagsRole = Qt::UserRole
};

enum DeviceItemFlags
{
	ItemNoFlags = 0,
	ItemComboBox = 1,
	ItemLineEdit = 2,
	ItemSpinbox = 3,
	ItemParent = 4,
	ItemRemovable = 8,
	ItemInputs = 16,
	ItemOutputs = 32
};

class QDomDocument;
class QDomElement;

class Interface;
class InterfaceHandler;

class DeviceItem
{
public:
	explicit DeviceItem(const QVariant data, int flags, InterfaceHandler* handler, DeviceItem *parent = 0);
	~DeviceItem();

	DeviceItem *child(int number);
	int childCount() const;
	QVariant data(int role) const;
	void setData(const QVariant &value);
	DeviceItem *parent();
	int childNumber() const;
	DeviceItem* addChild(const QVariant data, int flags);
	bool removeChildren(int position, int count);

	void getValue(QList<qint16>& data);
	void setValue(qint16 value);
	void saveXml(QDomDocument* doc, QDomElement* parent=0);

private:
	DeviceItem *parentItem;
	QVariant itemData;
	int itemFlags;

	InterfaceHandler* interfaceHandler;
	Interface* signalInterface;
	int signalIndex;

	QList<DeviceItem*> childItems;
};

#endif // DEVICEITEM_H

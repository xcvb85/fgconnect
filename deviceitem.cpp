#include <QStringList>
#include <QDomDocument>
#include <QDebug>
#include <QColor>

#include "deviceitem.h"
#include "interfacehandler.h"
#include "interface.h"

DeviceItem::DeviceItem(const QVariant data, int flags, InterfaceHandler* handler, DeviceItem *parent)
	: parentItem(parent)
	, itemData(data)
	, itemFlags(flags)
	, interfaceHandler(handler)
	, signalInterface(0)
	, signalIndex(0)
{
	if((itemFlags & 0x03) == ItemComboBox)
	{
		if(itemData == 0)
		{
			itemData = interfaceHandler->getSignalNames().at(0);
		}
		interfaceHandler->getSignal(itemData.toString(), signalInterface, signalIndex);
	}
}

DeviceItem::~DeviceItem()
{
	qDeleteAll(childItems);
}

DeviceItem *DeviceItem::child(int number)
{
	return childItems.value(number);
}

int DeviceItem::childCount() const
{
	return childItems.count();
}

int DeviceItem::childNumber() const
{
	if(parentItem)
		return parentItem->childItems.indexOf(const_cast<DeviceItem*>(this));

	return 0;
}

QVariant DeviceItem::data(int role) const
{
	switch(role)
	{
		case Qt::DisplayRole:
			switch(itemFlags & 0x03)
			{
				case ItemSpinbox:
					return QString("Id: %1").arg(itemData.toInt());

				default:
					return itemData.toString();
			}

		case Qt::EditRole:
			switch(itemFlags & 0x03)
			{
				case ItemNoFlags:
					return QVariant();

				case ItemSpinbox:
					return itemData.toInt();

				default:
					return itemData.toString();
			}

//		case Qt::BackgroundColorRole:
//			if((itemFlags&0x03) == ItemSpinbox) return QColor(Qt::green);

//		case Qt::CheckStateRole:
//			if((itemFlags&0x03) == ItemSpinbox) return Qt::Checked;

		case ItemFlagsRole:
			return itemFlags;

		default:
			break;
	}
	return QVariant();
}

void DeviceItem::setData(const QVariant &value)
{
	itemData = value;

	if((itemFlags & 0x03) == ItemComboBox)
	{
		interfaceHandler->getSignal(itemData.toString(), signalInterface, signalIndex);
	}
}

DeviceItem *DeviceItem::parent()
{
	return parentItem;
}

DeviceItem* DeviceItem::addChild(const QVariant data, int flags)
{
	DeviceItem *item = new DeviceItem(data, flags, interfaceHandler, this);
	childItems.append(item);
	return item;
}

bool DeviceItem::removeChildren(int position, int count)
{
	if(position < 0 || position + count > childItems.size())
		return false;

	for(int row = 0; row < count; ++row)
		delete childItems.takeAt(position);

	return true;
}

void DeviceItem::getValue(QList<qint16> &data)
{
	if(itemFlags & ItemInputs && signalInterface)
	{
		data.append(signalInterface->getValue(signalIndex));
	}

	foreach(DeviceItem* item, childItems)
	{
		item->getValue(data);
	}
}

void DeviceItem::setValue(qint16 value)
{
	Q_UNUSED(value);
//	source->setValue(pos, value);
}

void DeviceItem::saveXml(QDomDocument* doc, QDomElement* parent)
{
	QDomElement element = doc->createElement("Item");
	element.setAttribute("Name", itemData.toString());
	element.setAttribute("Flags", itemFlags);

	if(parent)
	{
		parent->appendChild(element);
	}
	else
	{
		doc->appendChild(element);
	}

	foreach(DeviceItem* item, childItems)
	{
		item->saveXml(doc, &element);
	}
}

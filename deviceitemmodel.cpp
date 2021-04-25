#include <QtWidgets>
#include <QDomDocument>
#include <QDebug>

#include "deviceitem.h"
#include "deviceitemmodel.h"
#include "interfacehandler.h"

DeviceItemModel::DeviceItemModel(InterfaceHandler* handler, QObject *parent)
	: QAbstractItemModel(parent)
{
	rootItem = new DeviceItem("Devices", ItemParent, handler);
}

DeviceItemModel::~DeviceItemModel()
{
	delete rootItem;
}

int DeviceItemModel::rowCount(const QModelIndex &parent) const
{
	return getItem(parent)->childCount();
}

int DeviceItemModel::columnCount(const QModelIndex & /* parent */) const
{
	return 1;
}

QVariant DeviceItemModel::headerData(int section, Qt::Orientation orientation,
							   int role) const
{
	Q_UNUSED(section);

	if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(role);

	return QVariant();
}

QVariant DeviceItemModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();

	DeviceItem *item = getItem(index);
	return item->data(role);
}

bool DeviceItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(role != Qt::EditRole)
		return false;

	DeviceItem *item = getItem(index);
	item->setData(value);
	emit dataChanged(index, index);
	return true;
}

Qt::ItemFlags DeviceItemModel::flags(const QModelIndex &index) const
{
	if(!index.isValid())
		return 0;

	return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

DeviceItem *DeviceItemModel::getItem(const QModelIndex &index) const
{
	if(index.isValid()) {
		DeviceItem *item = static_cast<DeviceItem*>(index.internalPointer());
		if(item)
			return item;
	}
	return rootItem;
}

QModelIndex DeviceItemModel::index(int row, int column, const QModelIndex &parent) const
{
	if(parent.isValid() && parent.column() != 0)
		return QModelIndex();

	DeviceItem *parentItem = getItem(parent);

	DeviceItem *childItem = parentItem->child(row);
	if(childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex DeviceItemModel::parent(const QModelIndex &index) const
{
	if(!index.isValid())
		return QModelIndex();

	DeviceItem *childItem = getItem(index);
	DeviceItem *parentItem = childItem->parent();

	if(parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool DeviceItemModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	DeviceItem *parentItem = getItem(parent);
	if (!parentItem)
		return false;

	int parentFlags = parentItem->data(ItemFlagsRole).toInt();
	int flags = 0;
	flags |= (parentFlags & ItemInputs);
	flags |= (parentFlags & ItemOutputs);

	beginInsertRows(parent, position, position + rows - 1);
	parentItem->addChild(0, flags | ItemRemovable | ItemComboBox);
	endInsertRows();

	return true;
}

bool DeviceItemModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	DeviceItem *parentItem = getItem(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

void DeviceItemModel::addDevice()
{
	beginInsertRows(QModelIndex(), rootItem->childCount(), rootItem->childCount());
	DeviceItem* item = rootItem->addChild(0, ItemRemovable | ItemSpinbox);
	item->addChild("Name", ItemLineEdit);
	item->addChild("Inputs", ItemParent | ItemInputs);
	item->addChild("Outputs", ItemParent | ItemOutputs);
	endInsertRows();
}

void DeviceItemModel::readXml(DeviceItem* parent, QDomNode node)
{
	if(!parent) return;

	DeviceItem* child = 0;
	QString name;
	int flags = 0;

	QDomNamedNodeMap attributeMap = node.attributes();
//	qDebug() << node.nodeName();

	for(int t = 0; t < attributeMap.count(); t++)
	{
		if(attributeMap.item(t).nodeName() == "Name")
		{
			name = attributeMap.item(t).nodeValue();
		}

		if(attributeMap.item(t).nodeName() == "Flags")
		{
			flags = attributeMap.item(t).nodeValue().toInt();
		}
	}
	child = parent->addChild(name, flags);

	QDomNodeList list = node.childNodes();
	for(int i=0; i < list.size(); i++)
	{
		readXml(child, list.at(i));
	}
}

void DeviceItemModel::readXml(QString fileName)
{
	QFile file(fileName);
	if(file.open(QIODevice::ReadOnly))
	{
		QDomDocument document;
		if(document.setContent(&file))
		{
			QDomNodeList nodes = document.lastChild().childNodes();

			for(int i=0; i<nodes.size(); i++)
				readXml(rootItem, nodes.at(i));
		}
		file.close();
	}
}

void DeviceItemModel::trigger()
{
	QList<qint16> data;

	for(int i=0; i < rootItem->childCount(); i++)
	{
		DeviceItem* item = rootItem->child(i);
		data.clear();
		item->getValue(data);
		emit ModbusData(item->data(Qt::EditRole).toInt(), data);
	}
}

void DeviceItemModel::writeXml(QString fileName)
{
	QFile file(fileName);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug("Failed to open file for writing.");
		return;
	}

	QDomDocument doc;
	doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

	rootItem->saveXml(&doc);

	QTextStream stream(&file);
	stream << doc.toString();
	file.close();
}

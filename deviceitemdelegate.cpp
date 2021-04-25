#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>

#include "deviceitemdelegate.h"
#include "deviceitem.h"
#include "interfacehandler.h"

#include <QDebug>

// ------------------------------------------------------------------------------------------------
DeviceItemDelegate::DeviceItemDelegate(InterfaceHandler* handler, QObject *parent)
	: QStyledItemDelegate(parent)
	, interfaceHandler(handler)
{
}

// ------------------------------------------------------------------------------------------------
DeviceItemDelegate::~DeviceItemDelegate()
{
}

// ------------------------------------------------------------------------------------------------
QWidget *DeviceItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);
	QWidget* widget = 0;

	switch(index.data(Qt::UserRole).toInt() & 0x03)
	{
		case ItemComboBox:
		{
			QComboBox* cb = new QComboBox(parent);
			foreach(QString name, interfaceHandler->getSignalNames())
			{
				cb->addItem(name);
			}
			widget = cb;
			break;
		}

		case ItemLineEdit:
		{
			QLineEdit* le = new QLineEdit(parent);
			widget = le;
			break;
		}

		case ItemSpinbox:
		{
			QSpinBox* sb = new QSpinBox(parent);
			widget = sb;
			break;
		}

		default:
		{
			break;
		}
	}
	return widget;
}

// ------------------------------------------------------------------------------------------------
void DeviceItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QStyledItemDelegate::setEditorData(editor, index);
}

// ------------------------------------------------------------------------------------------------
void DeviceItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	switch(index.data(Qt::UserRole).toInt() & 0x03)
	{
		case ItemComboBox:
		{
			if(QComboBox *cb = qobject_cast<QComboBox*>(editor))
			{
				model->setData(index, cb->currentText(), Qt::EditRole);
			}
			break;
		}

		default:
		{
			QStyledItemDelegate::setModelData(editor, model, index);
			break;
		}
	}
}

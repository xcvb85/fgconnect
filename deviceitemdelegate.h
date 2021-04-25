#ifndef DEVICEDELEGATE_H
#define DEVICEDELEGATE_H

#include <QStyledItemDelegate>

class InterfaceHandler;

class DeviceItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	DeviceItemDelegate(InterfaceHandler* handler, QObject *parent = 0);
	~DeviceItemDelegate();

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
	InterfaceHandler* interfaceHandler;
};

#endif // DEVICEDELEGATE_H

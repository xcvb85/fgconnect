#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QDebug>

#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "deviceitemmodel.h"
#include "deviceitemdelegate.h"
#include "interfacehandler.h"

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::MainWidget)
{
	setupUi();

	Timer = new QTimer;
	Timer->setInterval(50); /// TODO
	Timer->start();

	connect(Model, &DeviceItemModel::ModbusData, this, &MainWidget::modbusData);
	connect(Timer, &QTimer::timeout, Model, &DeviceItemModel::trigger);
}

MainWidget::~MainWidget()
{
	delete ui;
}

void MainWidget::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event);
	Model->writeXml("settings.xml");
}

void MainWidget::setupUi()
{
	ui->setupUi(this);
	setWindowTitle("FGConnect");

	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		ui->serialPortComboBox->addItem(info.portName());
	}

	InterfaceHandler* interface = new InterfaceHandler(this);
	Model = new DeviceItemModel(interface);
	Model->readXml("settings.xml");
	ui->treeView->setModel(Model);
	ui->treeView->setItemDelegate(new DeviceItemDelegate(interface));
}

void MainWidget::addButton()
{
	const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
	if(index.isValid())
	{
		int itemType = Model->data(index, ItemFlagsRole).toInt();
		if(itemType & ItemParent)
			Model->insertRow(index.row(), index);
		else if((itemType & 3) == ItemSpinbox) /// TODO
			Model->addDevice();
	}
	else
	{
		Model->addDevice();
	}
}

void MainWidget::removeButton()
{
	const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
	if(index.isValid())
	{
		if(Model->data(index, ItemFlagsRole).toInt() & ItemRemovable)
			Model->removeRow(index.row(), index.parent());
	}
}

void MainWidget::modbusData(int id, QList<qint16> data)
{
	qDebug() << id << data;
}

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class MainWidget;
}

class QTimer;
class DeviceItemModel;

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MainWidget(QWidget *parent = 0);
	~MainWidget();

protected:
	void closeEvent(QCloseEvent *event);

private:
	void setupUi();

	Ui::MainWidget *ui;
	DeviceItemModel* Model;
	QTimer* Timer;

private slots:
	void modbusData(int id, QList<qint16> data);
	void addButton();
	void removeButton();
};

#endif // MAINWIDGET_H

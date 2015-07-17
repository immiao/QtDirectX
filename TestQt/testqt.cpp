#include "testqt.h"
#include "qdebug.h"
#include "qdatetime.h"
#include "qlayout.h"
#include <QtWidgets\QDialog>
#include <QMouseEvent>

using namespace std;

//#pragma comment ( lib, "D3D11.lib")

TestQt::TestQt(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::TestQtClass)
{
	ui->setupUi(this);
	dxWidget = new DxWidget(0, this);
	//dxWidget->show();
	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(dxWidget);
	ui->centralWidget->setLayout(layout);
	//dxWidget->show();
	connect(ui->drawLine,SIGNAL(triggered()),
		this, SLOT(drawLineTriggered()));
	connect(ui->choose,SIGNAL(triggered()),
		this, SLOT(chooseTriggered()));
	triggerId = -1;
	isDrawLineTriggered = false;
	isChooseTriggered = false;
}

TestQt::~TestQt()
{
	delete dxWidget;
}

void TestQt::drawLineTriggered()
{
	if (isDrawLineTriggered)
	{
		triggerId = -1;
		isDrawLineTriggered = false;
	}
	else
	{
		triggerId = 0;
		isDrawLineTriggered = true;

		// set others unchecked
		ui->choose->setChecked(false);
		isChooseTriggered = false;
	}

}

void TestQt::chooseTriggered()
{
	if (isChooseTriggered)
	{
		triggerId = -1;
		isChooseTriggered = false;
	}
	else
	{
		triggerId = 1;
		isChooseTriggered = true;

		// set others unchecked
		ui->drawLine->setChecked(false);
		isDrawLineTriggered = false;
	}

}
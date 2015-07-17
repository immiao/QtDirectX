#include "testqt.h"
#include "dxwidget.h"
#include "qwidget.h"
#include "qlabel.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TestQt w;
	//DxWidget dxwidget;
	//dxwidget.show();
	//QLabel* label = new QLabel(w.ui->centralWidget);
	//label->setText(QObject::tr("i am a label"));
	//label->resize(250,20);
	//label->show();
	w.show();
	return a.exec();
}

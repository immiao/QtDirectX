///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
// 
// FileName : main.cpp
// Creator  : Miao Kaixiang
// Date     : 2015-7-17 15:00:00
// Comment  : The entry of the application
//
///////////////////////////////////////////////////////////////

#include "testqt.h"
#include "dxwidget.h"
#include "KEPublic2.h"
#include <QLabel>
#include <QWidget>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TestQt w;
	w.Init();
	w.show();
	int result = a.exec();
	w.UnInit();
	return result;
}

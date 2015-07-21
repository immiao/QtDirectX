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

HRESULT main(int argc, char *argv[])
{
	HRESULT hResult = E_FAIL;
	QApplication a(argc, argv);
	TestQt w;
	hResult = w.Init();
	KE_COM_PROCESS_ERROR(hResult);
	w.show();

	hResult = (HRESULT)a.exec();
Exit0:
	return hResult;
}

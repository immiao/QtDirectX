///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
// 
// FileName : testqt.h
// Creator  : Miao Kaixiang
// Date     : 2015-7-17 15:00:00
// Comment  : Declaration of QtMainWindow
//
///////////////////////////////////////////////////////////////

#ifndef TESTQT_H
#define TESTQT_H

#include "ui_testqt.h"
#include "dxwidget.h"
#include <QtWidgets/QMainWindow>
#include <QWidget>
#include <d3d10.h>
#include <d3dx10.h>

class DxWidget;
class TestQt : public QMainWindow
{
	Q_OBJECT

public:
	TestQt(QWidget *parent = 0);
	~TestQt();
	HRESULT Init();
	bool m_bIsDrawLineTriggered;
	bool m_bIsChooseTriggered;

private:
	Ui::TestQtClass* m_ui;
	DxWidget* m_dxWidget;

public slots:
	void DrawLineTriggered();
	void ChooseTriggered();
};

#endif // TESTQT_H

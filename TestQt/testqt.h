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
#include <QLayout>
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
	HRESULT UnInit();
	bool GetIsDrawLineTriggered();
	bool GetIsChooseTriggered();

private:
	Ui::TestQtClass* m_ui;
	DxWidget* m_dxWidget;
	QHBoxLayout* layout;
	bool m_bIsDrawLineTriggered;
	bool m_bIsChooseTriggered;

public slots:
	void DrawLineTriggered();
	void ChooseTriggered();
};

#endif // TESTQT_H

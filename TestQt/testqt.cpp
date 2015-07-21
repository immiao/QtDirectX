///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
// 
// FileName : testqt.cpp
// Creator  : Miao Kaixiang
// Date     : 2015-7-17 15:00:00
// Comment  : Implementation of QtMainWindow
//
///////////////////////////////////////////////////////////////

#include "testqt.h"
#include "KEPublic2.h"
#include <QtWidgets\QDialog>
#include <QMouseEvent>

using namespace std;

TestQt::TestQt(QWidget *parent): QMainWindow(parent)
{
	m_bIsDrawLineTriggered = false;
	m_bIsChooseTriggered = false;
}

HRESULT TestQt::Init()
{
	HRESULT hrResult	= E_FAIL;
	HRESULT hrRetCode	= E_FAIL;

	m_ui = new Ui::TestQtClass;
	KE_PROCESS_ERROR(m_ui);

	m_ui->setupUi(this);

	m_dxWidget = new DxWidget();
	KE_PROCESS_ERROR(m_dxWidget);
	hrRetCode = m_dxWidget->Init(this);
	KE_COM_PROCESS_ERROR(hrRetCode);

	layout = new QHBoxLayout;
	KE_PROCESS_ERROR(layout);
	layout->addWidget(m_dxWidget);
	m_ui->centralWidget->setLayout(layout);

	connect(m_ui->drawLine, SIGNAL(triggered()), this, SLOT(DrawLineTriggered()));
	connect(m_ui->choose, SIGNAL(triggered()), this, SLOT(ChooseTriggered()));

	hrResult = S_OK;
Exit0:
	return hrResult;
}

TestQt::~TestQt()
{
	
}

void TestQt::DrawLineTriggered()
{
	if (m_bIsDrawLineTriggered)
	{
		m_bIsDrawLineTriggered = false;
	}
	else
	{
		m_bIsDrawLineTriggered = true;

		// set others unchecked
		m_ui->choose->setChecked(false);
		m_bIsChooseTriggered = false;
	}
}

void TestQt::ChooseTriggered()
{
	if (m_bIsChooseTriggered)
	{
		m_bIsChooseTriggered = false;
	}
	else
	{
		m_bIsChooseTriggered = true;

		// set others unchecked
		m_ui->drawLine->setChecked(false);
		m_bIsDrawLineTriggered = false;
	}
}

HRESULT TestQt::UnInit()
{
	SAFE_DELETE(m_ui);
	m_dxWidget->UnInit();
	SAFE_DELETE(m_dxWidget);
	SAFE_DELETE(layout);

	return S_OK;
}

bool TestQt::GetIsChooseTriggered()
{
	return m_bIsChooseTriggered;
}

bool TestQt::GetIsDrawLineTriggered()
{
	return m_bIsDrawLineTriggered;
}



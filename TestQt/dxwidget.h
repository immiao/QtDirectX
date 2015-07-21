///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
// 
// FileName : dxwidget.h
// Creator  : Miao Kaixiang
// Date     : 2015-7-17 15:00:00
// Comment  : Declaration of DirectX Widget
//
///////////////////////////////////////////////////////////////

#ifndef DXWIDGET_H
#define DXWIDGET_H

#include "ui_testqt.h"
#include "testqt.h"
#include <QWidget>
#include <d3d10.h>
#include <d3dx10.h>

#define MAX_LINE 1000

struct SIMPLE_VERTEX
{
    D3DXVECTOR3 Pos;
    D3DXVECTOR4 Color;
};

class TestQt;
class DxWidget : public QWidget
{
	Q_OBJECT

public:
	DxWidget();
	~DxWidget();
	virtual QPaintEngine* paintEngine() const {return NULL;}
	HRESULT Init(TestQt* mainWin);
	HRESULT UnInit();

protected:
	virtual void resizeEvent(QResizeEvent* event);
	virtual void paintEvent(QPaintEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	HRESULT InitDevice();
	HRESULT Render();

private:
	// variables for DirectX configuration
	D3D10_VIEWPORT			m_viewPort;
	DXGI_SWAP_CHAIN_DESC	m_swapChainDesc;
	D3D10_DRIVER_TYPE       m_driverType;
	ID3D10Device*           m_pd3dDevice;
	IDXGISwapChain*         m_pSwapChain;
	ID3D10RenderTargetView* m_pRenderTargetView;
	ID3D10EffectTechnique*  m_pTechnique;
	ID3D10InputLayout*      m_pVertexLayout;
	ID3D10Buffer*           m_pVertexBuffer;
	ID3D10Effect*           m_pEffect;
	
	// variables for dragging line
	bool m_bIsDraggingLine;
	QPoint m_qStartPoint;
	int m_nLineIndex;

	TestQt* m_pMainWin;
	SIMPLE_VERTEX m_vertices[MAX_LINE];		
	int m_nVertexCounter;

	HRESULT UpdateVertexBuffer();
	D3DXVECTOR3 ToSimpleVertex(const QPoint &point);
	QPoint ToQPoint(const D3DXVECTOR3 &vertex);
};

#endif // DXWIDGET_H

#ifndef DXWIDGET_H
#define DXWIDGET_H

#include "ui_testqt.h"
#include "testqt.h"
#include <qwidget.h>
#include <d3d10.h>
#include <d3dx10.h>

#define MAXLINE 100

struct SimpleVertex
{
    D3DXVECTOR3 Pos;
    D3DXVECTOR4 Color;
	SimpleVertex()
	{
		Color.x = 0.0f;
		Color.y = 0.0f;
		Color.z = 0.0f;
		Color.w = 1.0f;
	}

};
class TestQt;
class DxWidget : public QWidget
{
	Q_OBJECT
	// Q_DISABLE_COPY(d3DRenderWidget)
public:
	DxWidget(QWidget *parent = 0, TestQt* mainWin = 0);
	~DxWidget();
	
	virtual QPaintEngine* paintEngine() const { return NULL; } // override
protected:
	virtual void resizeEvent(QResizeEvent* evt);
	virtual void paintEvent(QPaintEvent* evt);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	void CleanupDevice();
	HRESULT InitDevice();
	void Render();

private:
	HRESULT UpdateVertexBuffer();
	SimpleVertex toSimpleVertex(const QPoint &point);
	QPoint toQPoint(const SimpleVertex &vertex);
	D3D10_VIEWPORT vp;
	D3D10_DRIVER_TYPE       g_driverType;
	ID3D10Device*           g_pd3dDevice;
	IDXGISwapChain*         g_pSwapChain;
	ID3D10RenderTargetView* g_pRenderTargetView;
	ID3D10EffectTechnique*  g_pTechnique;
	ID3D10InputLayout*      g_pVertexLayout;
	ID3D10Buffer*           g_pVertexBuffer;
	ID3D10Effect*           g_pEffect;
	DXGI_SWAP_CHAIN_DESC sd;
	SimpleVertex vertices[MAXLINE];		
	int mVertexCounter;
	HRESULT hr;

	// variables for dragging line
	bool isDraggingLine;
	QPoint startPoint;
	int lineIndex;

	TestQt* mMainWin;
};

#endif // DXWIDGET_H

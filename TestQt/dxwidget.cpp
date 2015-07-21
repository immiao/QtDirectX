///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
// 
// FileName : dxwidget.cpp
// Creator  : Miao Kaixiang
// Date     : 2015-7-17 15:00:00
// Comment  : Implementation of DirectX Widget
//
///////////////////////////////////////////////////////////////

#include "dxwidget.h"
#include "KEPublic2.h"
#include <QMouseEvent>
#include <QTimer>
#include <QDialog>
#include <QLabel>

DxWidget::DxWidget(TestQt* mainWin)
{
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);

	InitDevice();

	m_nVertexCounter = 0;
	m_pMainWin = mainWin;
	m_bIsDraggingLine = false;

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(20);
}

DxWidget::~DxWidget()
{
	CleanupDevice();
}

// Create Direct3D device and swap chain
HRESULT DxWidget::InitDevice()
{
	HRESULT hResult = E_FAIL;
	m_driverType = D3D10_DRIVER_TYPE_NULL;
	m_pd3dDevice = NULL;
	m_pSwapChain = NULL;
	m_pRenderTargetView = NULL;
	m_pTechnique = NULL;
	m_pVertexLayout = NULL;
	m_pVertexBuffer = NULL;
	m_pEffect = NULL;

    UINT createDeviceFlags = 0;

    D3D10_DRIVER_TYPE driverTypes[] =
    {
        D3D10_DRIVER_TYPE_HARDWARE,
        D3D10_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

    ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));
    m_swapChainDesc.BufferCount = 1;
	m_swapChainDesc.BufferDesc.Width = width();
    m_swapChainDesc.BufferDesc.Height = height();
    m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    m_swapChainDesc.OutputWindow = (HWND)winId();
    m_swapChainDesc.SampleDesc.Count = 1;
    m_swapChainDesc.SampleDesc.Quality = 0;
    m_swapChainDesc.Windowed = TRUE;
	
    for(UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        m_driverType = driverTypes[driverTypeIndex];
        hResult = D3D10CreateDeviceAndSwapChain(NULL, m_driverType, NULL, createDeviceFlags,
			D3D10_SDK_VERSION, &m_swapChainDesc, &m_pSwapChain, &m_pd3dDevice);

        if(SUCCEEDED(hResult))
            break;
    }
	KE_COM_PROCESS_ERROR(hResult);
	KE_PROCESS_ERROR(m_pSwapChain);
	KE_PROCESS_ERROR(m_pd3dDevice);

    // Create a render target view
    ID3D10Texture2D* pBackBuffer;
    hResult = m_pSwapChain->GetBuffer(0, __uuidof( ID3D10Texture2D ), ( LPVOID* )&pBackBuffer);
	KE_COM_PROCESS_ERROR(hResult);
	KE_PROCESS_ERROR(pBackBuffer);

    hResult = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
    pBackBuffer->Release();
	KE_COM_PROCESS_ERROR(hResult);
	KE_PROCESS_ERROR(m_pRenderTargetView);
	
    m_pd3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

    // Setup the viewport
    m_viewPort.Width = width();
    m_viewPort.Height = height();
    m_viewPort.MinDepth = 0.0f;
    m_viewPort.MaxDepth = 1.0f;
    m_viewPort.TopLeftX = 0;
    m_viewPort.TopLeftY = 0;
    m_pd3dDevice->RSSetViewports(1, &m_viewPort);

	 // Create the effect
    DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

    hResult = D3DX10CreateEffectFromFile(L"testqt.fx", NULL, NULL, "fx_4_0", dwShaderFlags, 0,
		m_pd3dDevice, NULL, NULL, &m_pEffect, NULL, NULL );
	KE_COM_PROCESS_ERROR(hResult);
	KE_PROCESS_ERROR(m_pEffect);

    // Obtain the technique
    m_pTechnique = m_pEffect->GetTechniqueByName("Render");

    // Define the input layout
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
    };
    UINT numElements = sizeof(layout) / sizeof(layout[0]);

    // Create the input layout
    D3D10_PASS_DESC PassDesc;
    m_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
    hResult = m_pd3dDevice->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &m_pVertexLayout );
    KE_COM_PROCESS_ERROR(hResult);
	KE_PROCESS_ERROR(m_pVertexLayout);

    // Set the input layout
    m_pd3dDevice->IASetInputLayout(m_pVertexLayout);
	// Set primitive topology
    m_pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	hResult = S_OK;
Exit0:
	return hResult;
}

HRESULT DxWidget::UpdateVertexBuffer()
{
	HRESULT hResult = E_FAIL;
    D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SIMPLE_VERTEX) * m_nVertexCounter;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = m_vertices;
    hResult = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	KE_COM_PROCESS_ERROR(hResult);
	KE_PROCESS_ERROR(m_pVertexBuffer);

    // Set vertex buffer
    UINT stride = sizeof(SIMPLE_VERTEX);
    UINT offset = 0;
    m_pd3dDevice->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	hResult = S_OK;
Exit0:
	return hResult;
}

// Render the frame
void DxWidget::Render()
{
    float ClearColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    m_pd3dDevice->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
    D3D10_TECHNIQUE_DESC techDesc;
    m_pTechnique->GetDesc(&techDesc);
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        m_pTechnique->GetPassByIndex(p)->Apply(0);
        m_pd3dDevice->Draw(m_nVertexCounter, 0);
    }
    m_pSwapChain->Present(0, 0);
}

void DxWidget::resizeEvent(QResizeEvent* evt)
{
	if (m_nVertexCounter != 0)
	{
		UpdateVertexBuffer();
		Render();
	}
}

void DxWidget::paintEvent(QPaintEvent* evt)
{
	if (m_nVertexCounter != 0)
	{
		UpdateVertexBuffer();
		Render();
	}
}

void DxWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (m_pMainWin->m_bIsDrawLineTriggered)
	{
		m_vertices[m_nVertexCounter - 1].Pos = ToSimpleVertex(event->pos());
	}
	else if (m_pMainWin->m_bIsChooseTriggered && m_bIsDraggingLine)
	{
		m_vertices[m_nLineIndex].Pos.x += (event->pos().x() - m_qStartPoint.x()) / (float)width() * 2;
		m_vertices[m_nLineIndex].Pos.y += -(event->pos().y() - m_qStartPoint.y()) / (float)height() * 2;
		m_vertices[m_nLineIndex + 1].Pos.x += (event->pos().x() - m_qStartPoint.x()) / (float)width() * 2;
		m_vertices[m_nLineIndex + 1].Pos.y += -(event->pos().y() - m_qStartPoint.y()) / (float)height() * 2;
		m_qStartPoint = event->pos();
	}
}

void DxWidget::mousePressEvent(QMouseEvent* event)
{
	if (m_pMainWin->m_bIsDrawLineTriggered)
	{
		m_vertices[m_nVertexCounter].Pos = ToSimpleVertex(event->pos());
		m_vertices[m_nVertexCounter].Color.x = 0.0f;
		m_vertices[m_nVertexCounter].Color.y = 0.0f;
		m_vertices[m_nVertexCounter].Color.z = 0.0f;
		m_vertices[m_nVertexCounter].Color.w = 1.0f;
		m_vertices[m_nVertexCounter + 1].Pos = ToSimpleVertex(event->pos());
		m_vertices[m_nVertexCounter + 1].Color.x = 0.0f;
		m_vertices[m_nVertexCounter + 1].Color.y = 0.0f;
		m_vertices[m_nVertexCounter + 1].Color.z = 0.0f;
		m_vertices[m_nVertexCounter + 1].Color.w = 1.0f;
		m_nVertexCounter += 2;
	}
	else if (m_pMainWin->m_bIsChooseTriggered)
	{
		for (int i = 0; i < m_nVertexCounter; i++)
		{
			m_vertices[i].Color.x = 0.0f;
			m_vertices[i].Color.y = 0.0f;
			m_vertices[i].Color.z = 0.0f;
		}
		// search for the line which is selected
		for (int i = 0; i < m_nVertexCounter; i+=2)
		{
			QPoint p1 = ToQPoint(m_vertices[i].Pos);
			QPoint p2 = ToQPoint(m_vertices[i+1].Pos);
			QPoint p = event->pos();

			int minx;
			int maxx;
			int miny;
			int maxy;
			if (p1.x() < p2.x())
			{
				minx = p1.x();
				maxx = p2.x();
			}
			else
			{
				minx = p2.x();
				maxx = p1.x();
			}

			if (p1.y() < p2.y())
			{
				miny = p1.y();
				maxy = p2.y();
			}
			else
			{
				miny = p2.y();
				maxy = p1.y();
			}

			if ( abs((p1.x() - p2.x()) * (p.y() - p1.y()) - (p.x() - p1.x()) * (p1.y() - p2.y())) < 2000
				&& p.x() >= minx && p.x() <= maxx && p.y() >= miny && p.y() <= maxy)
			{
				m_bIsDraggingLine = true;
				m_qStartPoint = event->pos();
				m_nLineIndex = i;
				m_vertices[i].Color.x = 1.0f;
				m_vertices[i + 1].Color.x = 1.0f;
				break;
			}
		}
	}
}

void DxWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_pMainWin->m_bIsDrawLineTriggered)
	{
		m_vertices[m_nVertexCounter].Pos = ToSimpleVertex(event->pos());
	}
	else if (m_pMainWin->m_bIsChooseTriggered)
	{
		m_bIsDraggingLine = false;
	}
}

// Release the device
void DxWidget::CleanupDevice()
{
    if( m_pd3dDevice ) m_pd3dDevice->ClearState();
    if( m_pVertexBuffer ) m_pVertexBuffer->Release();
    if( m_pVertexLayout ) m_pVertexLayout->Release();
    if( m_pEffect ) m_pEffect->Release();
    if( m_pRenderTargetView ) m_pRenderTargetView->Release();
    if( m_pSwapChain ) m_pSwapChain->Release();
    if( m_pd3dDevice ) m_pd3dDevice->Release();
}

// convert QPoint to the range in [-1, 1]
D3DXVECTOR3 DxWidget::ToSimpleVertex(const QPoint &point)
{
	D3DXVECTOR3 vector3;
	vector3.x = point.x() / (float)width() * 2 - 1;
	vector3.y = -(point.y() / (float)height() * 2 - 1);
	vector3.z = 0.5f;
	return vector3;
}

// convert point within [-1, 1] to QPoint
QPoint DxWidget::ToQPoint(const D3DXVECTOR3 &vertex)
{
	QPoint result;
	result.setX((vertex.x + 1)/2 * width());
	result.setY((-vertex.y + 1)/2 * height());
	return result;
}


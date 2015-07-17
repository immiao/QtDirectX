#include "dxwidget.h"
#include <qdebug.h>
#include <qdatetime.h>
#include "qlabel.h"
#include <QtWidgets\QDialog>
#include <QMouseEvent>

DxWidget::DxWidget(QWidget *parent)
{
	//QLabel* label = new QLabel(this);
	//label->setText(QObject::tr("i am a label"));
	//label->resize(250,20);
	//label->show();
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);

	InitDevice();
}

DxWidget::~DxWidget()
{
	CleanupDevice();
}

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT DxWidget::InitDevice()
{
	g_driverType = D3D10_DRIVER_TYPE_NULL;
	g_pd3dDevice = NULL;
	g_pSwapChain = NULL;
	g_pRenderTargetView = NULL;
	g_pTechnique = NULL;
	g_pVertexLayout = NULL;
	g_pVertexBuffer = NULL;
	g_pEffect = NULL;

    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

    D3D10_DRIVER_TYPE driverTypes[] =
    {
        D3D10_DRIVER_TYPE_HARDWARE,
        D3D10_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
	//qDebug() << "HHHH " << width();
	sd.BufferDesc.Width = width();
    sd.BufferDesc.Height = height();

    //sd.BufferDesc.Width = width();
    //sd.BufferDesc.Height = height();
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = (HWND)winId();
	/*sd.OutputWindow = (HWND)winId();*/
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
	
    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D10CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags,
                                            D3D10_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D10Texture2D* pBackBuffer;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;
	
    g_pd3dDevice->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );

    // Setup the viewport
    vp.Width = width();
    vp.Height = height();
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pd3dDevice->RSSetViewports( 1, &vp );

	// Add at 2015/7/16 17:57
	 // Create the effect
    DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3D10_SHADER_DEBUG;
    #endif
    hr = D3DX10CreateEffectFromFile( L"testqt.fx", NULL, NULL, "fx_4_0", dwShaderFlags, 0,
                                         g_pd3dDevice, NULL, NULL, &g_pEffect, NULL, NULL );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be located.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

    // Obtain the technique
    g_pTechnique = g_pEffect->GetTechniqueByName( "Render" );

    // Define the input layout
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = sizeof( layout ) / sizeof( layout[0] );

    // Create the input layout
    D3D10_PASS_DESC PassDesc;
    g_pTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    hr = g_pd3dDevice->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature,
                                          PassDesc.IAInputSignatureSize, &g_pVertexLayout );
    if( FAILED( hr ) )
        return hr;

    // Set the input layout
    g_pd3dDevice->IASetInputLayout( g_pVertexLayout );

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        D3DXVECTOR3( 0.0f, 0.5f, 0.5f ),
        D3DXVECTOR3( 0.5f, -10.0f, 0.5f ),
        //D3DXVECTOR3( -0.5f, -0.5f, 0.5f ),
    };
    D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( vertices );
	qDebug() <<  "SIZE:" << sizeof( SimpleVertex ) << endl;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = vertices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    g_pd3dDevice->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );

    // Set primitive topology
    g_pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP );

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Render the frame
//--------------------------------------------------------------------------------------
void DxWidget::Render()
{
	// Clear the back buffer 
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
    g_pd3dDevice->ClearRenderTargetView( g_pRenderTargetView, ClearColor );

    // Render a triangle
    D3D10_TECHNIQUE_DESC techDesc;
    g_pTechnique->GetDesc( &techDesc );
	//qDebug() << techDesc.Passes;
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        g_pTechnique->GetPassByIndex( p )->Apply( 0 );
        g_pd3dDevice->Draw( 2, 0 );
		//qDebug() << "DONE";
    }

    // Present the information rendered to the back buffer to the front buffer (the screen)
    g_pSwapChain->Present( 0, 0 );
}

void DxWidget::resizeEvent(QResizeEvent* evt)
{
	//releaseBuffers();
	//g_pSwapChain->ResizeBuffers(1, width(), height(), sd.BufferDesc.Format, 0);
	//g_pSwapChain->GetDesc(&sd);
	Render();
	/*qDebug() << "resizeEvent Called";*/
	//createBuffers();
}
int counter=0;
void DxWidget::paintEvent(QPaintEvent* evt)
{
	
	Render();
	Render();
	Render();
	//qDebug() << "paintEvent Called" << counter++;
	//QDateTime current_date_time = QDateTime::currentDateTime();
	//qDebug() << current_date_time;
}

void DxWidget::mouseMoveEvent(QMouseEvent * event)
{
	//qDebug() << event->pos();
}

void DxWidget::mousePressEvent(QMouseEvent * event)
{
	//qDebug() << event->pos();
}

void DxWidget::mouseReleaseEvent(QMouseEvent* event)
{
	//qDebug() << event->pos();
}

//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void DxWidget::CleanupDevice()
{
    if( g_pd3dDevice ) g_pd3dDevice->ClearState();

    if( g_pVertexBuffer ) g_pVertexBuffer->Release();
    if( g_pVertexLayout ) g_pVertexLayout->Release();
    if( g_pEffect ) g_pEffect->Release();
    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();
}

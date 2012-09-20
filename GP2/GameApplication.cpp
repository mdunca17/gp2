#include "GameApplication.h"


CGameApplication::CGameApplication(void)
{
   m_pWindow=NULL;
   m_pD3D10Device=NULL;
   m_pRenderTargetView=NULL;
   m_pSwapChain=NULL;
   m_pVertexBuffer = NULL;
}

CGameApplication::~CGameApplication(void)
{
	 if(m_pD3D10Device)
		 m_pD3D10Device->ClearState();

	 if(m_pVertexBuffer)
		 m_pVertexBuffer->Release();

	 if(m_pRenderTargetView)
		 m_pRenderTargetView->Release();
	 if(m_pSwapChain)
		 m_pSwapChain->Release();
	 if(m_pD3D10Device)
		 m_pD3D10Device->Release();

	 if(m_pWindow)
	 {
		 delete m_pWindow;
		 m_pWindow=NULL;
	 }
}
struct Vertex
{
	D3DXVECTOR3 pos;
}

bool CGameApplication::init()
{
	if(!initWindow())
		  return false;

	if(!initGraphics())
		  return false;
	if(!initGame())
	{
		D3D10_BUFFER_DESC bd;
		bd.Usage = D3D10_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( Vertex)*3;
		bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		return false;
	}
	return true;
}

void CGameApplication::run()
{
	while(m_pWindow->running())
	{
		if (!m_pWindow->checkForWindowMessages())
		{
			update();
			render();
		}
	}
}

void CGameApplication::render()
{

	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f};
	m_pD3D10Device-> ClearRenderTargetView(m_pRenderTargetView,
		ClearColor);

	m_pSwapChain->Present(0, 0);

}

void CGameApplication::update()
{

}

bool CGameApplication::initGraphics()
{
	RECT windowRect;
	GetClientRect(m_pWindow->getHandleToWindow(),&windowRect);

	UINT width = windowRect.right-windowRect.left;
	UINT height = windowRect.bottom-windowRect.top;

	UINT createDeviceFlags=0;
#ifdef _DEBUG
	createDeviceFlags|=D3D10_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd,sizeof(sd));

	if(m_pWindow->isFullScreen())
		sd.BufferCount = 2;
	else 
		sd.BufferCount = 1;

	sd.OutputWindow = m_pWindow->getHandleToWindow();
	sd.Windowed = (BOOL)(!m_pWindow->isFullScreen());
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;

	Vertex vertices[] = 
	{
		D3DXVECTOR3(0.0f,0.5f,0.5f),
		D3DXVECTOR3(0.5f,-0.5f,0.5f),
		D3DXVECTOR3(-0.5f,-0.5f,0.5f),
	};

	D3D10_SUBRESOURCE_DATA lnitData;
	lnitData.pSysMem = vertices;

	if(FAILED(m_pD3D10Device->CreateBuffer(&bd,&lnitData,
		&m_pVertexBuffer)))
	      return false;

	if(FAILED(D3D10CreateDeviceAndSwapChain(NULL,D3D10_DRIVER_TYPE_HARDWARE,NULL,createDeviceFlags,
		D3D10_SDK_VERSION,&sd,&m_pSwapChain,
		&m_pD3D10Device)))
		return false;

	ID3D10Texture2D*pBackBuffer;
	if (FAILED (m_pSwapChain->GetBuffer(0,__uuidof (ID3D10Texture2D),
		(void**)&pBackBuffer)))
		return false;

	if(FAILED(m_pD3D10Device->CreateRenderTargetView( pBackBuffer,
		NULL,
		&m_pRenderTargetView))){

			pBackBuffer->Release();
			return false;
	}

	m_pD3D10Device->OMSetRenderTargets(1,
		&m_pRenderTargetView,
		NULL);

	D3D10_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pD3D10Device->RSSetViewports(1, &vp);

	return true;
}

bool CGameApplication::initWindow()
{
	m_pWindow=new CWin32Window();
	if(!m_pWindow->init(TEXT("Lab 1 - Create Device"), 800, 640,false))
		return false;

	return true;
}
